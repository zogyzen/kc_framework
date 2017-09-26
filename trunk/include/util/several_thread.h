#pragma once

#include <list>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <memory>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <boost/random.hpp>

#include "util/auto_release.h"

namespace KC
{
	// �����߳�
	template<int ID>
	class CSeveralThread
	{
	public:
		CSeveralThread(void) {};
		~CSeveralThread(void) {};

		// �߳���
		typedef std::shared_ptr<std::thread> CRunThread;

		// ���߳�
		template<typename _Functor>
		CRunThread NewThread(string sIndex, _Functor functor)
		{
		    CRunThread thrd;
            void (CSeveralThread::*pFunc)(string, _Functor) = &CSeveralThread::RunThread;
            auto Ftor = boost::lambda::bind(pFunc, this, sIndex, boost::lambda::protect(functor));
		    {
                m_Mutex.lock();
                CAutoRelease _autoB(boost::bind(&std::mutex::unlock, &this->m_Mutex));
                thrd = CRunThread(new std::thread(Ftor));
                m_thrdList.insert(make_pair(sIndex, thrd));
		    }
			//Ftor();
            return thrd;
		}
		template<typename _Functor>
		CRunThread NewThread(_Functor functor)
		{
			string sIndex = MakeUniqueStr("T");
			return NewThread(sIndex, functor);
		}
		template<typename _Functor>
		string NewThreadGetIndex(_Functor functor)
		{
			string sIndex = MakeUniqueStr("T");
			NewThread(sIndex, functor);
			return sIndex;
		}

		// ͨ����ţ��õ��߳�
		CRunThread GetThreadByIndex(string sIndex)
		{
            this->m_Mutex.lock();
            CAutoRelease _autoB(boost::bind(&std::mutex::unlock, &this->m_Mutex));
			CThreadList::iterator iter = this->m_thrdList.find(sIndex);
			return iter->second;
		}

		// �����߳�
		void StopThread(string sIndex)
		{
            this->m_Mutex.lock();
            CAutoRelease _autoB(boost::bind(&std::mutex::unlock, &this->m_Mutex));
			CThreadList::iterator iter = this->m_thrdList.find(sIndex);
			if (this->m_thrdList.end() != iter)
			{
				CRunThread thrd = iter->second;
				//thrd->interrupt();
				pthread_cancel(thrd->native_handle());
				this->m_thrdList.erase(sIndex);
			}
		}

		// �����߳�
		void StopAllThread()
		{
			this->m_Mutex.lock();
            CAutoRelease _autoB(boost::bind(&std::mutex::unlock, &this->m_Mutex));
			for (CThreadList::iterator iter = this->m_thrdList.begin(); iter != this->m_thrdList.end(); ++iter)
			{
				CRunThread thrd = iter->second;
				//thrd->interrupt();
				pthread_cancel(thrd->native_handle());
			}
			this->m_thrdList.clear();
		}

		// �ȴ������߳̽���
		void JoinAllThread()
		{
			for (CThreadList::iterator iter = this->m_thrdList.begin(); iter != this->m_thrdList.end(); ++iter)
			{
				CRunThread thrd = iter->second;
				thrd->join();
			}
		}

	protected:
		// �߳��б�
		typedef std::map<string, CRunThread> CThreadList;
		CThreadList m_thrdList;

		// ��
		std::mutex m_Mutex;

	protected:
		// ִ���߳�
		template<typename _Functor>
		void RunThread(string sIndex, _Functor functor)
		{
			functor();
            this->m_Mutex.lock();
            CAutoRelease _autoB(boost::bind(&std::mutex::unlock, &this->m_Mutex));
			CThreadList::iterator iter = this->m_thrdList.find(sIndex);
			if (this->m_thrdList.end() != iter)
			{
				CRunThread thrd = iter->second;
				this->m_thrdList.erase(sIndex);
			}
		}

		// ���������Ψһ���ַ���
		string MakeUniqueStr(string sHeadStr)
		{
			// �����
			boost::variate_generator<boost::mt19937, boost::uniform_int<> > _Random(boost::mt19937(), boost::uniform_int<>(1000, 9999));
			// ����ַ���
			string strSrc = sHeadStr + boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()) + "R"
				+ boost::lexical_cast<string>(_Random());
			return strSrc;
		}
	};
}
