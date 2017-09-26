#pragma once

#include <string>
using namespace std;

#include <boost/log/trivial.hpp>

#include "common/base_type.h"

namespace KC
{
	// ��־������
	struct TLogInfo
	{
	public:
		// ��־�ȼ�
		enum TLogLevel
		{
			lglvTrace = boost::log::trivial::trace,         // ����
			lglvDebug = boost::log::trivial::debug,         // ����
			lglvInfo = boost::log::trivial::info,			// ��Ϣ
			lglvWarning = boost::log::trivial::warning,		// ����
			lglvError = boost::log::trivial::error,			// ����
			lglvFatal = boost::log::trivial::fatal          // ����
		};
	public:
	    //TLogInfo(void) = delete;
		TLogInfo(string info, string place, TLogLevel level = lglvInfo, string other = "")
			: m_info(info), m_place(place), m_level(level)
			, m_other(other), m_excpID(0), m_LogType(2), m_refID(0) {}
		TLogInfo(TException& ex, string info = "")
			: m_info(ex.CurrPosInfo() + info + ex.error_info()), m_place(ex.error_place()), m_level(lglvError)
			, m_other(ex.OtherInfo()), m_excpID(ex.error_id()), m_excpType(ex.ExceptType()), m_LogType(2), m_refID(0) {}
        TLogInfo(const TLogInfo& c)
            : m_info(c.m_info), m_place(c.m_place), m_level(c.m_level), m_other(c.m_other)
            , m_excpType(c.m_excpType), m_excpID(c.m_excpID)
            , m_LogType(c.m_LogType), m_bundleName(c.m_bundleName), m_serviceName(c.m_serviceName), m_serviceGUID(c.m_serviceGUID), m_refID(c.m_refID)
            , m_ProcessID(c.m_ProcessID), m_threadID(c.m_threadID) {}
	public:
		// �û�������Ϣ
		string m_info;			            // ��־��Ϣ
		string m_place;			            // ���õ�λ��
		TLogLevel m_level = lglvInfo;		// ��־�ĵȼ�
		string m_other;			            // ������־��Ϣ

		// �쳣��Ϣ
		string m_excpType;		            // �쳣������
		int m_excpID = 0;			        // �쳣���

		// �̶�����Ϣ
		int m_LogType = 0;			        // �������ͣ�0�����־��1ģ����־��2������־��3������־
		string m_bundleName;	            // ģ�������
		string m_serviceName;	            // ��������ƣ�������
		string m_serviceGUID;	            // �����GUID
		long m_refID;			            // �������õı��

		// ������Ϣ
		unsigned int m_ProcessID = getpid();    // ���̱��
		unsigned long int m_threadID = 0;       // �̱߳��
	};
}
