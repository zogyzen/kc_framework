#pragma once

namespace KC
{
    class CBundleContext;
    class CServiceRegistration : public IServiceRegistration
    {
    public:
        CServiceRegistration(IService& srv, CBundleContext& ct);
        virtual CALL_TYPE ~CServiceRegistration();

        // �õ�������
        virtual const char* CALL_TYPE getGUID(void) const;
        // �õ���������������
        virtual const char* CALL_TYPE getName(void) const;
        // ��ȡ����ӿ�
        virtual IService& CALL_TYPE getService(void) const;
        // ��ȡģ��ӿ�
        virtual const IBundle& CALL_TYPE getBundle(void) const;

    private:
        // ����ӿ�
        IService& m_service;
        // ���
        CBundleContext& m_Context;
    };
}
