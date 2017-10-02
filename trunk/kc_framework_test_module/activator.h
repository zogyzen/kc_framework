#pragma once

#include "stdn.h"

namespace KC
{
	class CActivator : public IBundleActivator
	{
	public:
		CActivator(const IBundle&);
        virtual CALL_TYPE ~CActivator() = default;

		// 启动模块（注册服务等）
        bool CALL_TYPE start(void) override;
		// 停止模块（注销服务等）
        bool CALL_TYPE stop(void) override;

    private:
        // 插件上下文
        IBundleContext& m_context;
        const IBundle& m_bundle;

        // 服务层
        IKCTestMod *m_srv = nullptr;
        IServiceRegistration *m_sreg = nullptr;
	};
}
