#pragma once

#include "stdn.h"

namespace KC
{
    class CKCTestMod : public IKCTestMod
    {
    public:
        CKCTestMod(const IBundle& bundle);
        virtual CALL_TYPE ~CKCTestMod();

        // 得到服务特征码
        const char* CALL_TYPE getGUID(void) const override;
        // 对应的模块
        const IBundle& CALL_TYPE getBundle(void) const override;

        // 启动主程序
        void run(void) override;

    private:
        // 插件及上下文
        IBundleContext& m_context;
        const IBundle& m_bundle;
    };
}
