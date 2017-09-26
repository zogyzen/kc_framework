#include "std.h"
#include "bundle.h"
#include "bundle_context.h"

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
        string sError;
        if (nullptr == m_libPtr)
            m_libPtr = new KLoadLibrary<true>((m_path + "/" + m_name).c_str(), sError);
        if (sError.length() > 0)
            throw TFWBundleException(1, __FUNCTION__, sError, m_name);
        // ��ʼ������
        typedef IBundleActivator& CALL_TYPE (*TFuncInitActor)(const IBundle&);
        TFuncInitActor _Init = m_libPtr->GetLibFunc<TFuncInitActor>(g_ModuleInitActor, sError);
        if (sError.length() > 0)
            throw TFWBundleException(2, __FUNCTION__, sError, m_name);
        if (nullptr == _Init)
            throw TFWBundleException(3, __FUNCTION__, this->getContext().getHint("Can_t_get_the_function_") + m_name + "::InitActor", m_name);
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
        if (nullptr != m_libPtr)
        {
            // ж�غ���
            typedef void CALL_TYPE (*TFuncUninitActor)(void);
            TFuncUninitActor _Uninit = m_libPtr->GetLibFunc<TFuncUninitActor>(g_ModuleUninitActor);
            _Uninit();
            m_actor = nullptr;
            // ж�ض�̬��
            delete m_libPtr;
            m_libPtr = nullptr;
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
