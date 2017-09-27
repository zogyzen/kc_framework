#pragma once

#include <string>
#include <iostream>
using namespace std;

#include <boost/dll.hpp>
using namespace boost;

#include "framework/service_i.h"
#include "framework/framework_i.h"

extern const char* c_dll_extname;

namespace KC
{
    class BundleContextHelper
    {
    public:
        BundleContextHelper(string sDir)
        {
            string sDllPath = sDir + "/" + c_frameworkDllName + c_dll_extname;
            system::error_code ec;
            m_lib.load(sDllPath, ec);
            if (m_lib.is_loaded())
            {
                if (m_lib.has(c_frameworFuncName))
                {
                    auto _getFramework = m_lib.get<IFramework&(const char*)>(c_frameworFuncName);
                    IFramework& fx = _getFramework(sDir.c_str());
                    m_fx = &fx;
                    m_context = fx.NewContext((sDir + "/" + g_ModuleDirectory).c_str());
                }
            }
        }
        ~BundleContextHelper(void)
        {
            if (m_lib.is_loaded() && nullptr != m_fx && nullptr != m_context)
                m_fx->FreeContext(m_context);
            if (m_lib.is_loaded())
                m_lib.unload();
        }

        bool isSuccess(void)
        {
            return m_lib.is_loaded() && nullptr != m_context;
        }

        IBundleContext& getContext(void)
        {
            if (!this->isSuccess())
                throw std::runtime_error("The framework not loaded.");
            return *m_context;
        }

    private:
        dll::shared_library m_lib;
        IFramework* m_fx = nullptr;
        IBundleContext* m_context = nullptr;
    };
}
