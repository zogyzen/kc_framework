#include "std.h"
#include "bundle.h"
#include "bundle_context.h"

#ifdef WIN32    // Windows����
    const char* c_dll_extname = ".dll";
#else           // linux����
    const char* c_dll_extname = ".so";
#endif

//---------------------------------------------------------------------------
// TBundle��
CBundle::CBundle(const char* name, const char* path, CBundleContext& ct)
		: m_name(name), m_path(path), m_Context(ct), m_state(bsFree)
{
    // ģ��·��ת��Ϊ����·��
    if (!boost::filesystem::path(m_path).is_complete())
        m_path = string(ct.getPath()) + "/" + m_path;
    // ��ʽ��·���ַ���
    m_path = CUtilFunc::FormatPath(m_path);
}

CBundle::~CBundle()
{
	try
	{
		this->free();
	}
	CATCH_EXCEPTION_TO_WRITELOG(*this, this->getName())
}

// �õ�ģ�����ƣ���Ŀ¼��
const char* CBundle::getName(void) const
{
	return m_name.c_str();
}

// �õ�ģ��Ŀǰ״̬
CBundle::TBundleState CBundle::getState(void) const
{
	return m_state;
}

// �õ�����Ŀ¼
const char* CBundle::getPath(void) const
{
	return m_path.c_str();
}

// ����DLL
bool CBundle::load(void) const
{
	if (bsFree == m_state)
	{
        // ���ض�̬��
        if (!m_lib.is_loaded())
        {
            string sDllPath = m_path + "/" + m_name + c_dll_extname;
            system::error_code ec;
            m_lib.load(sDllPath, ec);
            if (ec)
                throw TFWBundleException(1, __FUNCTION__, "[" + sDllPath + "] " + ec.message(), m_name);
        }
        // ��ʼ������
        if (!m_lib.has(g_ModuleInitActor))
            throw TFWBundleException(2, __FUNCTION__, this->getContext().getHint("Can_t_get_the_function_") + m_name + "::InitActor", m_name);
        auto _Init = m_lib.get<IBundleActivator&(const IBundle&)>(g_ModuleInitActor);
        m_actor = &_Init(*this);
        if (nullptr != m_actor) m_state = bsLoaded;
    }
	return bsFree != m_state;
}

// �ͷ�DLL
bool CBundle::free(void) const
{
	if (bsStarted == m_state)
		this->stop();
	if (bsLoaded == m_state)
    {
        if (m_lib.is_loaded())
        {
            // ж�غ���
            if (m_lib.has(g_ModuleUninitActor))
            {
                auto _Uninit = m_lib.get<void(void)>(g_ModuleInitActor);
                _Uninit();
            }
            m_actor = nullptr;
            // ж�ض�̬��
            m_lib.unload();
        }
		m_state = bsFree;
    }
	return bsFree == m_state;
}

// ����ģ��
bool CBundle::start(void) const
{
	try
	{
		if (bsFree == m_state)
			this->load();
		if (bsLoaded == m_state && nullptr != m_actor && m_actor->start())
			m_state = bsStarted;
	}
	CATCH_EXCEPTION_TO_WRITELOG(*this, this->getName())
	return bsStarted == m_state;
}

// ֹͣģ��
bool CBundle::stop(void) const
{
	try
	{
		if (bsStarted == m_state && nullptr != m_actor && m_actor->stop())
			m_state = bsLoaded;
		if (nullptr == m_actor)
			m_state = bsFree;
	}
	CATCH_EXCEPTION_TO_WRITELOG(*this, this->getName())
	return bsStarted != m_state;
}

// �õ�ģ��������
IBundleContext& CBundle::getContext(void) const
{
    return m_Context;
}

// д��־
bool CBundle::WriteLog(TLogInfo log) const
{
	try
	{
		if (0 == log.m_LogType) log.m_LogType = 1;
		log.m_bundleName = m_name;
	}
	catch(...) {}
	return m_Context.WriteLog(log);
}
