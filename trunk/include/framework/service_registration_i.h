#pragma once

#include "framework/service_i.h"

namespace KC
{
	class IServiceRegistration
	{
	public:
		// �õ�����������
		virtual const char* CALL_TYPE getGUID(void) const = 0;
		// �õ���������������
		virtual const char* CALL_TYPE getName(void) const = 0;
		// ��ȡ����ӿ�
		virtual IService& CALL_TYPE getService(void) const = 0;
		// ��ȡģ��ӿ�
		virtual const IBundle& CALL_TYPE getBundle(void) const = 0;

  protected:
		virtual CALL_TYPE ~IServiceRegistration() = default;
	};
}
