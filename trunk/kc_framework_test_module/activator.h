#pragma once

#include "stdn.h"

namespace KC
{
	class CActivator : public IBundleActivator
	{
	public:
		CActivator(const IBundle&);
        virtual CALL_TYPE ~CActivator() = default;

		// ����ģ�飨ע�����ȣ�
        bool CALL_TYPE start(void) override;
		// ֹͣģ�飨ע������ȣ�
        bool CALL_TYPE stop(void) override;

    private:
        // ���������
        IBundleContext& m_context;
        const IBundle& m_bundle;

        // �����
        IKCTestMod *m_srv = nullptr;
        IServiceRegistration *m_sreg = nullptr;
	};
}
