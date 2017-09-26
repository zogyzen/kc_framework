#include "std.h"
#include "service_registration.h"
#include "framework.h"

//---------------------------------------------------------------------------
// TServiceRegistration��
CServiceRegistration::CServiceRegistration(IService& srv, CBundleContext& ct)
		: m_service(srv), m_Context(ct)
{
}

CServiceRegistration::~CServiceRegistration()
{
}

// �õ�������
const char* CServiceRegistration::getGUID(void) const
{
	return m_service.getGUID();
}

// �õ���������������
const char* CServiceRegistration::getName(void) const
{
	return m_service.getName();
}

// ��ȡ����ӿ�
IService& CServiceRegistration::getService(void) const
{
 	return m_service;
}

// ��ȡģ��ӿ�
const IBundle& CServiceRegistration::getBundle(void) const
{
	return m_service.getBundle();
}
