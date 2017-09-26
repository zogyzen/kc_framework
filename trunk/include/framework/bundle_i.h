#pragma once

#include "for_user/common_define.h"

namespace KC
{
    class IBundleContext;
	class IBundle
	{
	public:
		// ģ��״̬
		enum TBundleState
		{
			bsFree = 0,  		// δ����
			bsLoaded,     		// �Ѽ��أ�δ����
			bsStarted			// ������
		};

	public:
		// �õ�ģ�����ƣ���Ŀ¼��
		virtual const char* CALL_TYPE getName(void) const = 0;
		// �õ�ģ��Ŀǰ״̬
		virtual TBundleState CALL_TYPE getState(void) const = 0;
		// �õ�����Ŀ¼
		virtual const char* CALL_TYPE getPath(void) const = 0;
		// ����DLL
		virtual bool CALL_TYPE load(void) const = 0;
		// �ͷ�DLL
		virtual bool CALL_TYPE free(void) const = 0;
		// ����ģ��
		virtual bool CALL_TYPE start(void) const = 0;
		// ֹͣģ��
		virtual bool CALL_TYPE stop(void) const = 0;
		// �õ�ģ��������
		virtual IBundleContext& CALL_TYPE getContext(void) const = 0;
		// д��־
        virtual bool CALL_TYPE WriteLogTrace(const char* info, const char* place, const char* other = "") const = 0;
        virtual bool CALL_TYPE WriteLogDebug(const char* info, const char* place, const char* other = "") const = 0;
        virtual bool CALL_TYPE WriteLogInfo(const char* info, const char* place, const char* other = "") const = 0;
        virtual bool CALL_TYPE WriteLogError(const char* info, const char* place, const char* other = "") const = 0;
        virtual bool CALL_TYPE WriteLogFatal(const char* info, const char* place, const char* other = "") const = 0;

    protected:
        virtual CALL_TYPE ~IBundle() = default;
	};
}
