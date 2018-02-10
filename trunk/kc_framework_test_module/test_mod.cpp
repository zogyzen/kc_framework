#include "test_mod.h"

////////////////////////////////////////////////////////////////////////////////
// CKCTestMod类
CKCTestMod::CKCTestMod(const IBundle& bundle)
    : m_context(bundle.getContext()), m_bundle(bundle)
{
}

CKCTestMod::~CKCTestMod()
{
}

// 得到服务特征码
const char* CKCTestMod::getGUID(void) const
{
    return c_KCTestModSrvGUID;
}

// 对应的模块
const IBundle& CKCTestMod::getBundle(void) const
{
    return m_bundle;
}

// 启动
void CKCTestMod::run(void)
{
    cout << "Run module success - kc_framework_test_module" << endl;
}
