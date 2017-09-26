#pragma once

#include "framework/bundle_i.h"

namespace KC
{
	class IService
	{
	public:
		// �õ�����������
		virtual const char* CALL_TYPE getGUID(void) const = 0;
		// �õ���������������return typeid(*this).name();��
		virtual const char* CALL_TYPE getName(void) const = 0;
		// ��Ӧ��ģ��
		virtual const IBundle& CALL_TYPE getBundle(void) const = 0;

    protected:
        virtual CALL_TYPE ~IService() = default;
	};
}
