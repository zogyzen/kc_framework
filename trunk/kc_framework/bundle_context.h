#pragma once

#include "service_registration.h"
#include "service_reference.h"
#include "bundle.h"

#include "kc_lock_work.h"
#include "kc_log_thread.h"

namespace KC
{
    class CFramework;
    class CBundleContext : public IBundleContextEx
    {
        friend class CFramework;

    public:
        CBundleContext(CFramework&, const char*);
        virtual CALL_TYPE ~CBundleContext(void);

        // ��װ��ж��ģ��
        IBundle* CALL_TYPE installBundle(const char*, const char*, IBundle::TBundleState) override;
        bool CALL_TYPE uninstallBundle(IBundle*&) override;
        // ��ȡģ��
        bool CALL_TYPE ExistsBundle(const char*) const override;
        const IBundle& CALL_TYPE getBundle(const char*) const override;
        // ע�ᡢע������
        IServiceRegistration* CALL_TYPE registerService(IService&) override;
        bool CALL_TYPE unregisterService(IServiceRegistration*&) override;
        // ��������á��ͷ�
        IServiceReference* CALL_TYPE takeServiceReference(const char*) override;
        bool CALL_TYPE freeServiceReference(IServiceReference*&) override;
        // �õ�ģ��Ŀ¼
        const char* CALL_TYPE getPath(void) const override;
        // �õ����õ�Ŀ¼
        const char* CALL_TYPE getPathCfg(const char*) const override;
        // ��ȡ���ػ���ʾ��Ϣ
        const char* CALL_TYPE getHint(const char*) const override;
        // �õ����õ���־�ȼ�
        int CALL_TYPE GetCfgLogLevel(void) const override;
        // д��־
        bool CALL_TYPE WriteLog(TLogInfo) const override;
        // �ź�
        void CALL_TYPE Signal(void) override;
        ISignoCatch& CALL_TYPE SignoCatch(void) override;
        // ��
        IKcLockWork& CALL_TYPE LockWork(void) override;
        // ��ʱʱ��
        int CALL_TYPE GetTimeOutSeconds(void) const override;

    protected:
        // ע������
        bool unregisterService(const char*);

    private:
        // ����������
        void startup(void);
        // �ر�������
        void shutoff(void);
        // ��ʼ����־
        void init_log(void);
        // ��ʼ����Ϣ��ʾ��Ϣ
        void init_hint(void);
        // ��ʼ����ʱʱ��
        void init_timeout(void);
        // ��ȡ����·��
        string GetFullPath(string);

    private:
        // ���
        CFramework& m_Framework;
        // ģ������Ŀ¼
        string m_pathModules;
        // �����ļ�
        string m_cfgFile;
        // ���õ���־�ȼ�
        log::trivial::severity_level m_logLevel = log::trivial::warning;
        // ��Ϣ��ʾ��Ϣ
        map<string, string> m_ptHints;
        // ���õ�Ŀ¼�б�
        map<string, string> m_CfgDirs;
        // ��ʱʱ��
        int m_timeout_seconds = 7200;
        // ������
        CKcLockWork m_LockWork;
        // д��־���߳�
        mutable CLogThread m_logThrd;

    private:
        // �����
        CKcMutex m_mtxSrv;
        // ����ע���б�
        typedef std::shared_ptr<CServiceRegistration> TSrvRegPtr;
        typedef map<string, TSrvRegPtr> TSrvRegMap;
        TSrvRegMap m_SrvRegMap;
        // ���������б�
        typedef map<long, CServiceReference*> TSrvRefLst;        typedef map<string, TSrvRefLst> TSrvRefMap;
        TSrvRefMap m_SrvRefMap;

    private:
        // ģ���
        CKcMutex m_mtxBundle;
        // ģ���б�        typedef std::shared_ptr<CBundle> TBundlePtr;        typedef map<string, TBundlePtr> TBundleMap;
        TBundleMap m_BundleMap;
    };
}
