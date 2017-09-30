#pragma once

#include "common/log_info.h"
#include "framework/bundle_context_i.h"
#include "framework_ex/service_reference_ex_i.h"
#include "framework_ex/framework_exception.h"

namespace KC
{
    class IBundleContextEx : public IBundleContext
    {
    public:
        // 写日志
        virtual bool CALL_TYPE WriteLogTrace(const char* info, const char* place, const char* other = "") const
        {
            bool bResult = true;
            if (this->GetCfgLogLevel() <= (int)boost::log::trivial::trace)
            {
                TLogInfo log(info, place, TLogInfo::lglvTrace, other);
                bResult = this->WriteLog(log);
            }
            return bResult;
        }
        virtual bool CALL_TYPE WriteLogDebug(const char* info, const char* place, const char* other = "") const
        {
            bool bResult = true;
            if (this->GetCfgLogLevel() <= (int)boost::log::trivial::debug)
            {
                TLogInfo log(info, place, TLogInfo::lglvDebug, other);
                return this->WriteLog(log);
            }
            return bResult;
        }
        virtual bool CALL_TYPE WriteLogInfo(const char* info, const char* place, const char* other = "") const
        {
            bool bResult = true;
            if (this->GetCfgLogLevel() <= (int)boost::log::trivial::info)
            {
                TLogInfo log(info, place, TLogInfo::lglvInfo, other);
                return this->WriteLog(log);
            }
            return bResult;
        }
        virtual bool CALL_TYPE WriteLogWarning(const char* info, const char* place, const char* other = "") const
        {
            bool bResult = true;
            if (this->GetCfgLogLevel() <= (int)boost::log::trivial::warning)
            {
                TLogInfo log(info, place, TLogInfo::lglvWarning, other);
                return this->WriteLog(log);
            }
            return bResult;
        }
        virtual bool CALL_TYPE WriteLogError(const char* info, const char* place, const char* other = "") const
        {
            bool bResult = true;
            if (this->GetCfgLogLevel() <= (int)boost::log::trivial::error)
            {
                TLogInfo log(info, place, TLogInfo::lglvError, other);
                return this->WriteLog(log);
            }
            return bResult;
        }
        virtual bool CALL_TYPE WriteLogFatal(const char* info, const char* place, const char* other = "") const
        {
            bool bResult = true;
            if (this->GetCfgLogLevel() <= (int)boost::log::trivial::fatal)
            {
                TLogInfo log(info, place, TLogInfo::lglvFatal, other);
                return this->WriteLog(log);
            }
            return bResult;
        }

    public:
        // 写日志
        virtual bool CALL_TYPE WriteLog(TLogInfo) const = 0;
        // 获取服务接口
        IServiceReferenceEx& getServiceRef(const char* GUID)
        {
            IServiceReferenceEx* srvRef = dynamic_cast<IServiceReferenceEx*>(this->takeServiceReference(GUID));
            if (nullptr == srvRef)
                throw TFWSrvRefException(0, __FUNCTION__, "Can't get service reference.", "", GUID, -1);
            return *srvRef;
        }
        template<typename IF>
        IF& getService(const char* GUID)
        {
            return this->getServiceRef(GUID).getServiceSafe<IF>();
        }

    protected:
        virtual CALL_TYPE ~IBundleContextEx(void) = default;
    };
}
