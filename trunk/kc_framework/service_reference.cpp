#include "std.h"
#include "service_reference.h"
#include "framework.h"

//---------------------------------------------------------------------------
// TServiceReference��
CServiceReference::CServiceReference(string symbolic, long ID, IService* srv, CBundleContext& ct)
		: m_symbolic(symbolic), m_ID(ID), m_service(srv), m_Context(ct)
{
}

CServiceReference::~CServiceReference()
{
    IServiceReference* ref = this;
	m_Context.freeServiceReference(ref);
}

// �õ����ñ��
long CServiceReference::getID(void) const
{
 	return m_ID;
}

// �õ�������
const char* CServiceReference::getGUID(void) const
{
	return m_symbolic.c_str();
}

// �õ���������������
const char* CServiceReference::getName(void) const
{
	m_name = m_symbolic;
	if (!this->disable())
		m_name = m_service->getName();
	return m_name.c_str();
}

// �����Ƿ�ʧЧ��ģ��ж�ء�����ע���ȣ�
bool CServiceReference::disable(void) const
{
	return nullptr == m_service;
}

// ��ȡ����ӿ�
IService& CServiceReference::getService(void) const
{
	if (this->disable())
		throw TFWSrvRefException(1, __FUNCTION__, string(m_Context.getHint("The_service_don_t_exist_")) + this->getName(), this->getName(), this->getGUID(), this->getID());
 	return *m_service;
}

// ��ȡģ��ӿ�
const IBundle& CServiceReference::getBundle(void) const
{
	return this->getService().getBundle();
}

// д��־
bool CServiceReference::WriteLog(TLogInfo log) const
{
	try
	{
		if (0 == log.m_LogType) log.m_LogType = 3;
		if (!this->disable())
		{
			log.m_bundleName = this->getBundle().getName();
			log.m_serviceName = this->getName();
		}
		log.m_serviceGUID = this->getGUID();
		log.m_refID = this->getID();
	}
	catch(...) {}
	return m_Context.WriteLog(log);
}

// ���÷���
void CServiceReference::setService(IService* srv)
{
    if (nullptr == srv) this->WriteLogWarning((m_Context.getHint("The_service_don_t_exist_") + m_symbolic).c_str(), __FUNCTION__, m_symbolic.c_str());
	m_service = srv;
}
