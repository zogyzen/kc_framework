#pragma once

namespace KC
{
	struct DataSeparator
	{
	public:
		// ���ָ���ȡ��
		bool Separator(const char*& head, const char*& tail)
		{
			head = SearchPos(head, tail);
			if (nullptr != head && head < tail - m_sep_count)
                tail = head + m_sep_count;
            else if (nullptr == head)
                tail = nullptr;
			return nullptr != head && nullptr != tail;
		}

        // �ڴ滺��������
		static const char* MemFind(const char* head, const char* tail, const char* sep, int len)
		{
		    if (nullptr == head || nullptr == tail || tail <= head) return nullptr;
			for (; head < tail - len; ++head)
			    if (memcmp(head, sep, len) == 0)
                    return head;
			return tail;
		}

        // ȥ����β�Ļ��з�
		static const char* Remove_CR_LF_head(const char* head, const char* tail)
		{
		    if (nullptr == head || nullptr == tail || tail <= head) return nullptr;
			for (; head < tail; ++head) if ('\n' != *head && '\r' != *head) break;
			return head;
		}
		static const char* Remove_CR_LF_tail(const char* head, const char* tail)
		{
		    if (nullptr == head || nullptr == tail || tail <= head) return nullptr;
			for (; head < tail; --tail) if ('\n' != *tail && '\r' != *tail) break;
			return tail;
		}

	private:
		//�ҵ��ָ�����λ��
		const char* SearchPos(const char* head, const char* tail)
		{
		    return DataSeparator::MemFind(head, tail, m_sep, m_sep_count);
		}

	public:
		//���캯����ָ���ָ���
		DataSeparator(const char* sep, int num) : m_sep(new char[num + 1]), m_sep_count(num)
		{
		    memset(m_sep, 0, m_sep_count + 1);
		    memcpy(m_sep, sep, num);
		}
		~DataSeparator()
		{
            delete[] m_sep;
		}

	private:
	    // �ָ���
		char* m_sep = nullptr;
		int m_sep_count = 0;
	};
}
