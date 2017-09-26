#pragma once

#include "framework/service_i.h"

namespace KC
{
	// �������ýӿ�
	class IServiceReference
	{
	public:
		// �õ����ñ��
		virtual long CALL_TYPE getID(void) const = 0;
		// �õ�����������
		virtual const char* CALL_TYPE getGUID(void) const = 0;
		// �õ���������������
		virtual const char* CALL_TYPE getName(void) const = 0;
		// �����Ƿ���ʧЧ��ģ��ж�ء�����ע���ȣ�
		virtual bool CALL_TYPE disable(void) const = 0;
		// ��ȡ����ӿ�
		virtual IService& CALL_TYPE getService(void) const = 0;
		// ��ȡģ��ӿ�
		virtual const IBundle& CALL_TYPE getBundle(void) const = 0;
		// д��־
        virtual bool CALL_TYPE WriteLogTrace(const char* info, const char* place, const char* other = "") const = 0;
        virtual bool CALL_TYPE WriteLogDebug(const char* info, const char* place, const char* other = "") const = 0;
		virtual bool CALL_TYPE WriteLogInfo(const char* info, const char* place, const char* other = "") const = 0;
		virtual bool CALL_TYPE WriteLogWarning(const char* info, const char* place, const char* other = "") const = 0;
		virtual bool CALL_TYPE WriteLogError(const char* info, const char* place, const char* other = "") const = 0;

    protected:
        virtual CALL_TYPE ~IServiceReference() = default;
    };
}
