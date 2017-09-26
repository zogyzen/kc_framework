#include "std.h"
#include "bundle_context.h"
#include "framework.h"
#include "kc_log.h"
#include "kc_signo_catch.h"

//---------------------------------------------------------------------------
// TBundleContext��
CBundleContext::CBundleContext(CFramework& fw, const char* pathModules)
    : m_Framework(fw), m_pathModules(pathModules), m_cfgFile(string(pathModules) + g_ModuleConfigFile)
    , m_LockWork(*this), m_logThrd(*this)
    , m_mtxSrv(m_LockWork), m_mtxBundle(m_LockWork)
{
    // ģ��·��ת��Ϊ����·��
    if (!boost::filesystem::path(m_pathModules).is_complete())
        m_pathModules = fw.GetPath() + "/" + m_pathModules;
    boost::filesystem::path _path(m_pathModules);
    _path = boost::filesystem::system_complete(_path);
    m_pathModules = _path.string();
    // ��ʽ��·��
    m_pathModules = CUtilFunc::FormatPath(m_pathModules);
    // �����ļ�
    m_cfgFile = m_pathModules + "/" + g_ModuleConfigFile;
}

CBundleContext::~CBundleContext()
{
	try
	{
        // ж��©���Ĳ��
        if (!m_BundleMap.empty())
        {
            CKcLock lck(m_mtxBundle, "Unregister Miss Bundle", __FUNCTION__);
            m_BundleMap.clear();
        }
        // ע��©���ķ���
        if (!m_SrvRegMap.empty())
        {
            CKcLock lck(m_mtxSrv, "Unregister Miss Service", __FUNCTION__);
            m_SrvRegMap.clear();
        }
	}
	CATCH_EXCEPTION_TO_WRITELOG(*this, "")
}

// ��װ��ж��ģ��
IBundle* CBundleContext::installBundle(const char* sName, const char* sPath, IBundle::TBundleState iAct)
{
	CBundle* result = nullptr;	try
	{
		// ����ģ�����
		CBundle* bundle = new CBundle(sName, sPath, *this);
		TBundlePtr bundlePtr(bundle);
		// ����
		CKcLock lck(m_mtxBundle, string(this->getHint("Install_Bundle_")) + sName, __FUNCTION__);
		// ����Ƿ��ظ�ע��
		TBundleMap::iterator iter = m_BundleMap.find(bundle->getName());
		if (m_BundleMap.end() != iter)
			throw TFWBundleException(1, __FUNCTION__, string(this->getHint("Repeat_to_install_")) + sName, bundle->getName());
		// ��ʼ���ػ�����
		switch (iAct)
		{
		case IBundle::bsLoaded:   	// ��ʼ����
			bundle->load();
			break;
		case IBundle::bsStarted:	// ��ʼ����
			bundle->start();
			break;
        case IBundle::bsFree:
            break;
		}
		// ����ģ��
		m_BundleMap[bundle->getName()] = bundlePtr;
		// д��װ�ɹ���־
		bundle->WriteLogDebug((string(this->getHint("Install_Bundle_Success_")) + sName).c_str(), __FUNCTION__);
		result = bundle;
	}
	CATCH_EXCEPTION_TO_WRITELOG(*this, sName)
	return result;
}

bool CBundleContext::uninstallBundle(IBundle*& bundle)
{
	try
	{
		if (nullptr != bundle)
		{
			// ����
			CKcLock lck(m_mtxBundle, string(this->getHint("Uninstall_Bundle_")) + bundle->getName(), __FUNCTION__);
			// �ж�ģ���Ƿ����
			TBundleMap::iterator iter = m_BundleMap.find(bundle->getName());
			if (m_BundleMap.end() != iter)
			{
				// ɾ��ģ��
				bundle->WriteLogDebug((this->getHint("Uninstall_Bundle_") + iter->first).c_str(), __FUNCTION__);
				m_BundleMap.erase(iter);
			}
			bundle = nullptr;
		}
	}
	CATCH_EXCEPTION_TO_WRITELOG(*dynamic_cast<IBundleEx*>(bundle), bundle->getName())
	return nullptr == bundle;
}

// ��ȡģ��
bool CBundleContext::ExistsBundle(const char* name) const
{
	return m_BundleMap.end() != m_BundleMap.find(name);
}

// ��ȡģ��
const IBundle& CBundleContext::getBundle(const char* name) const
{
	TBundleMap::const_iterator iter = m_BundleMap.find(name);
	if (m_BundleMap.end() == iter)
		throw TFWBundleException(2, __FUNCTION__, string(this->getHint("Can_t_find_the_bundle_")) + name, name);
	return *iter->second;
}

// ע�ᡢע������
IServiceRegistration* CBundleContext::registerService(IService& srv)
{
	CServiceRegistration *reg = nullptr;
	try
	{
	    // �õ�����Ψһ��ʶ����������
		string  symbolic = srv.getGUID(),
                sName = srv.getName();
		// ���������벻��Ϊ��
		if (symbolic.empty())
            throw TFWSrvRegException(0, __FUNCTION__, this->getHint("The_service_GUID_is_null_") + sName, sName, symbolic);
		// ��������ע��
		reg = new CServiceRegistration(srv, *this);
		TSrvRegPtr regPtr(reg);
		// ����
        CKcLock lck(m_mtxSrv, this->getHint("Register_Service_") + sName + " [" + symbolic + "]", __FUNCTION__);
		// ����Ƿ��ظ�ע��
		TSrvRegMap::iterator iter = m_SrvRegMap.find(symbolic);
		if (m_SrvRegMap.end() != iter)
            throw TFWSrvRegException(1, __FUNCTION__, this->getHint("Repeat_to_register_service_") + sName, sName, symbolic);
		// �������ע��
		m_SrvRegMap[symbolic] = regPtr;
		((IServiceEx&)srv).WriteLogDebug((this->getHint("Register_Service_Success_") + sName).c_str(), __FUNCTION__);
		// ���е�������Ϊ��Ч
		TSrvRefMap::iterator iterRef = m_SrvRefMap.find(symbolic);
		if (m_SrvRefMap.end() != iterRef)
			for (TSrvRefLst::iterator it = iterRef->second.begin(); iterRef->second.end() != it; ++it)
				it->second->setService(&srv);
	}
	CATCH_EXCEPTION_TO_WRITELOG((IServiceEx&)srv, srv.getGUID())
	return reg;
}

// ע������
bool CBundleContext::unregisterService(IServiceRegistration*& reg)
{
	if (nullptr != reg && this->unregisterService(reg->getGUID()))        reg = nullptr;
	return nullptr == reg;
}
bool CBundleContext::unregisterService(const char* symbolic)
{	bool bResult = false;
	try
	{
		// ����
		CKcLock lck(m_mtxSrv, this->getHint("Unregister_Service_") + string("[") + symbolic + "]", __FUNCTION__);
		// �жϷ����Ƿ����
		TSrvRegMap::iterator iter = m_SrvRegMap.find(symbolic);
		if (m_SrvRegMap.end() != iter)
		{
			// ���е�������Ϊ��Ч
			TSrvRefMap::iterator iterRef = m_SrvRefMap.find(symbolic);
			if (m_SrvRefMap.end() != iterRef)
				for (TSrvRefLst::iterator it = iterRef->second.begin(); iterRef->second.end() != it; ++it)
					it->second->setService(nullptr);
			// ɾ��ע��
			((IServiceEx&)iter->second->getService()).WriteLogDebug((string(this->getHint("Unregister_Service_")) + iter->second->getService().getName()).c_str(), __FUNCTION__);
			m_SrvRegMap.erase(iter);
		}
		bResult = true;
	}
	CATCH_EXCEPTION_TO_WRITELOG(*this, symbolic)
	return bResult;
}

// ��������á��ͷ�
IServiceReference* CBundleContext::takeServiceReference(const char* symbolic)
{
	CServiceReference *refSrv = nullptr;
	try
	{
		// ����
		CKcLock lck(m_mtxSrv, this->getHint("Take_Reference_Service") + string(" - [") + symbolic + "]", __FUNCTION__);
		// ����Ƿ���ڷ���
		IService* srv = nullptr;
		TSrvRegMap::iterator iter = m_SrvRegMap.find(symbolic);
		if (m_SrvRegMap.end() != iter)
			srv = &iter->second->getService();
        if (nullptr == srv) this->WriteLogWarning((string(this->getHint("The_service_don_t_exist_")) + symbolic).c_str(), __FUNCTION__, symbolic);
		// �����Ӧ�������б�
		TSrvRefLst& refLst = m_SrvRefMap[symbolic];
		long RefID = 1;
		if (refLst.size() > 0) RefID = refLst.rbegin()->first + 1;
		// ������������
		refSrv = new CServiceReference(symbolic, RefID, srv, *this);
		refLst[RefID] = refSrv;
	}
	CATCH_EXCEPTION_TO_WRITELOG(*this, symbolic)
    refSrv->WriteLogTrace(this->getHint("Take_Reference_Service"), __FUNCTION__, symbolic);
	return refSrv;
}

// �ͷ�����
bool CBundleContext::freeServiceReference(IServiceReference*& refSrv)
{
	try
	{
	    if (nullptr != refSrv)
        {
            // ����
            CKcLock lck(m_mtxSrv, this->getHint("Free_Reference_Service") + string(" - [") + refSrv->getGUID() + "]", __FUNCTION__);
            // �ж������Ƿ����
            TSrvRefMap::iterator iter = m_SrvRefMap.find(refSrv->getGUID());
            if (m_SrvRefMap.end() != iter)
            {
                TSrvRefLst::iterator iterRef = iter->second.find(refSrv->getID());
                if (iter->second.end() != iterRef)
                {
                    // ɾ������
                    refSrv->WriteLogTrace(this->getHint("Free_Reference_Service"), __FUNCTION__, refSrv->getGUID());
                    iter->second.erase(iterRef);
                    if (iter->second.empty()) m_SrvRefMap.erase(iter);
                }
            }
            refSrv = nullptr;
        }
	}
	CATCH_EXCEPTION_TO_WRITELOG(*dynamic_cast<IServiceReferenceEx*>(refSrv), refSrv->getGUID())
	return nullptr == refSrv;
}

// �õ�ģ��Ŀ¼
const char* CBundleContext::getPath(void) const
{
	return m_pathModules.c_str();
}

// �õ����õ�Ŀ¼
const char* CBundleContext::getPathCfg(const char* cfg) const
{
    auto iter = m_CfgDirs.find(cfg);
    if (m_CfgDirs.end() == iter)
        throw TFWException(0, __FUNCTION__, string(this->getHint("The_path_config_don_t_exist_")) + cfg);
    return iter->second.c_str();
}

// ��ȡ���ػ���ʾ��Ϣ
const char* CBundleContext::getHint(const char* key) const
{
    auto iter = m_ptHints.find(key);
    if (m_ptHints.end() != iter)
        return iter->second.c_str();
    return key;
}

// �õ����õ���־�ȼ�
int CBundleContext::GetCfgLogLevel(void) const
{
    return (int)m_logLevel;
}

// д��־
bool CBundleContext::WriteLog(TLogInfo log) const
{
    log.m_ProcessID = getpid();
    log.m_threadID = pthread_self();
    m_logThrd.AppendLog(log);
	return true;
}

// �ź�
void CBundleContext::Signal(void)
{
    CSignoCatch::Signal();
}

ISignoCatch& CBundleContext::SignoCatch(void)
{
    return GetSignoCatch();
}

IKcLockWork& CBundleContext::LockWork(void)
{
    return m_LockWork;
}

int CBundleContext::GetTimeOutSeconds(void) const
{
    return m_timeout_seconds;
}

// ����
void CBundleContext::startup(void)
{
	try
	{
        if (!boost::filesystem::exists(m_cfgFile))
            throw TFWException(0, __FUNCTION__, "Don't exists config file [" + m_cfgFile + "].");
        // ��ʼ����Ϣ��ʾ��Ϣ
        this->init_hint();
        // ��ʼ����־
        this->init_log();
        // ��ʼ����ʱʱ��
        this->init_timeout();
        // ����д��־�߳�
        this->m_logThrd.Start();
	    // ���������ļ�
	    boost::property_tree::ptree pt;
        read_xml(m_cfgFile, pt);
        // ѭ�����е�Ŀ¼
        if (pt.get_child_optional("Config.Directories"))
        {
            BOOST_FOREACH(property_tree::ptree::value_type &v, pt.get_child("Config.Directories"))
            {
                m_CfgDirs[v.first] = this->GetFullPath(v.second.get<string>("<xmlattr>.Path"));
            }
        }
        // ѭ�����е�ģ��
        if (pt.get_child_optional("Config.Modules"))
        {
            BOOST_FOREACH(property_tree::ptree::value_type &v, pt.get_child("Config.Modules"))
            {
                IBundle::TBundleState bdlStt = IBundle::bsStarted;
                string sAct = v.second.get<string>("<xmlattr>.Activation");
                trim(sAct);
                if (sAct == "0") bdlStt = IBundle::bsFree;
                else if (sAct == "1") bdlStt = IBundle::bsLoaded;
                else bdlStt = IBundle::bsStarted;
                string sDllPath = "";
                if (v.second.get_child_optional("<xmlattr>.Path"))
                    sDllPath = v.second.get<string>("<xmlattr>.Path");
                this->installBundle(v.first.c_str(), sDllPath.c_str(), bdlStt);
            }
        }
	}
	catch(TException& ex)
	{
		TLogInfo log(ex.error_info(), __FUNCTION__, TLogInfo::lglvError);
		log.m_excpType = typeid(ex).name();
		log.m_excpID = ex.error_id();
		this->WriteLog(log);
		throw;
	}
	catch(std::exception& ex)
	{
		TLogInfo log(ex.what(), __FUNCTION__, TLogInfo::lglvFatal);
		log.m_excpType = typeid(ex).name();
		this->WriteLog(log);
		throw;
	}
	catch(const char* ex)
	{
		TLogInfo log(ex, __FUNCTION__, TLogInfo::lglvFatal);
		log.m_excpType = "const char*";
		this->WriteLog(log);
		throw;
	}
	catch(...)
	{
		this->WriteLog(TLogInfo(this->getHint("Unknown_exception"), __FUNCTION__, TLogInfo::lglvFatal));
		throw;
	}
}

// �ر�������
void CBundleContext::shutoff(void)
{
    // ֹͣ���
    for(TBundleMap::iterator it = m_BundleMap.begin(); m_BundleMap.end() != it; ++it)
        it->second->stop();
    // ж�ز��
    vector<IBundle*> bundleList;
    for(TBundleMap::iterator it = m_BundleMap.begin(); m_BundleMap.end() != it; ++it)
        bundleList.push_back(it->second.get());
    for(vector<IBundle*>::iterator it = bundleList.begin(); bundleList.end() != it; ++it)
        this->uninstallBundle(*it);
    // д������־
    this->WriteLogInfo(((boost::format(this->getHint("End_framework")) % c_KnewcodeVersion % m_cfgFile).str()).c_str(), __FUNCTION__);
    // ����д��־�߳�
    this->m_logThrd.Stop();
}

// ��ʼ����־
void CBundleContext::init_log(void)
{
    // ��־�Ǽ�
    if (boost::filesystem::exists(m_cfgFile))
    {
        boost::property_tree::ptree pt;
        read_xml(m_cfgFile, pt);
        if (pt.get_child_optional("Config.Parameters.logging"))
        {
            string sLv = pt.get<string>( "Config.Parameters.logging.<xmlattr>.Level" );
            if ("trace" == sLv) m_logLevel = log::trivial::trace;
            else if ("debug" == sLv) m_logLevel = log::trivial::debug;
            else if ("info" == sLv) m_logLevel = log::trivial::info;
            else if ("warning" == sLv) m_logLevel = log::trivial::warning;
            else if ("error" == sLv) m_logLevel = log::trivial::error;
        }
    }
    // ��־Ŀ¼
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    int iYear = today.year(), iWeek = today.week_number();
    if (++iWeek > 53) iWeek = 0;
    string strYearWeek = lexical_cast<string>(iYear * 100 + iWeek);
    string sLogDir = string(this->getPath()) + "/" + c_LogDirectoryName + strYearWeek;
    MyLog::Init(sLogDir.c_str(), m_logLevel);
    // д��ʼ��־
    this->WriteLogInfo(((boost::format(this->getHint("Start_framework")) % c_KnewcodeVersion % m_cfgFile).str()).c_str(), __FUNCTION__);
}

// ��ʼ����Ϣ��ʾ��Ϣ
void CBundleContext::init_hint(void)
{
    if (boost::filesystem::exists(m_cfgFile))
    {
        boost::property_tree::ptree pt;
        read_xml(m_cfgFile, pt);
        // ��Ϣ��ʾ�ļ�
        string sHintFile = "";
        if (pt.get_child_optional("Config.Parameters.hint_file"))
            sHintFile = pt.get<string>( "Config.Parameters.hint_file.<xmlattr>.File" );
        if (!boost::filesystem::path(sHintFile).is_complete())
            sHintFile = m_pathModules + "/" + sHintFile;
        // ��ȡ��Ϣ��ʾ�ļ�
        if (boost::filesystem::exists(sHintFile))
        {
            boost::property_tree::ptree ptHint;
            read_xml(sHintFile, ptHint);
            // ѭ�����е���Ϣ��ʾ
            if (ptHint.get_child_optional("Hint"))
            {
                BOOST_FOREACH(property_tree::ptree::value_type &v, ptHint.get_child("Hint"))
                {
                    m_ptHints[v.first] = v.second.get<string>("<xmlattr>.String");
                }
            }
        }
    }
}

// ��ʼ����ʱʱ��
void CBundleContext::init_timeout(void)
{
    if (boost::filesystem::exists(m_cfgFile))
    {
        boost::property_tree::ptree pt;
        read_xml(m_cfgFile, pt);
        if (pt.get_child_optional("Config.Parameters.timeout"))
        {
            string sSeconds = pt.get<string>( "Config.Parameters.timeout.<xmlattr>.seconds" );
            m_timeout_seconds = lexical_cast<int>(sSeconds);
        }
    }
}

// ��ȡ����·��
string CBundleContext::GetFullPath(string sPath)
{
    string sResult = sPath;
    if (!filesystem::path(sPath).is_complete())
        sResult = m_pathModules + "/" + sPath;
    return CUtilFunc::FormatPath(sResult);
}
