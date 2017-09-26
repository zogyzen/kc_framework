#pragma once

namespace KC
{
    class CBundleContext;
    class CBundle : public IBundleEx
    {
    public:
        CBundle(const char* name, const char* path, CBundleContext& ct);
        virtual CALL_TYPE ~CBundle();

        // �õ�ģ�����ƣ���Ŀ¼��
        virtual const char* CALL_TYPE getName(void) const;
        // �õ�ģ��Ŀǰ״̬
        virtual TBundleState CALL_TYPE getState(void) const;
        // �õ�����Ŀ¼
        virtual const char* CALL_TYPE getPath(void) const;
        // ����DLL
        virtual bool CALL_TYPE load(void) const;
        // �ͷ�DLL
        virtual bool CALL_TYPE free(void) const;
        // ����ģ��
        virtual bool CALL_TYPE start(void) const;
        // ֹͣģ��
        virtual bool CALL_TYPE stop(void) const;
		// �õ�ģ��������
		virtual IBundleContext& CALL_TYPE getContext(void) const;
        // д��־
        virtual bool CALL_TYPE WriteLog(TLogInfo) const;

    private:
        // ģ�����ƣ���Ŀ¼��
        const string m_name;
        // ģ��·��
        string m_path;
        // ���������
        CBundleContext& m_Context;
        // ��̬�����
        mutable KLoadLibrary<true>* m_libPtr = nullptr;
        // ģ����������
        mutable IBundleActivator* m_actor = nullptr;
        // ģ��״̬
        mutable TBundleState m_state;
    };
}
