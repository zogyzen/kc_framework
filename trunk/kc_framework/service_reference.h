#pragma once

namespace KC
{
    class CBundleContext;
    class CServiceReference : public IServiceReferenceEx
    {
    public:
        CServiceReference(string symbolic, long ID, IService* srv, CBundleContext& ct);
        virtual CALL_TYPE ~CServiceReference();

        // �õ����ñ��
        virtual long CALL_TYPE getID(void) const;
        // �õ�����������
        virtual const char* CALL_TYPE getGUID(void) const;
        // �õ���������������
        virtual const char* CALL_TYPE getName(void) const;
        // �����Ƿ���ʧЧ��ģ��ж�ء�����ע���ȣ�
        virtual bool CALL_TYPE disable(void) const;
        // ��ȡ����ӿ�
        virtual IService& CALL_TYPE getService(void) const;        // ��ȡģ��ӿ�        virtual const IBundle& CALL_TYPE getBundle(void) const;
        // д��־        virtual bool CALL_TYPE WriteLog(TLogInfo) const;
    public:
        // ���÷���        void setService(IService* srv);

    private:
        // Ψһ������
        string m_symbolic;
        // ����
        mutable string m_name;
        // ���
        const long m_ID;
        // ����ӿ�
        IService* m_service = nullptr;
        // ���
        CBundleContext& m_Context;
    };
}
