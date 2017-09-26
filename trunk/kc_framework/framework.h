#pragma once

#include "bundle_context.h"

namespace KC
{
    class CFramework : public IFramework
    {
    public:
        CFramework(string path);
        virtual CALL_TYPE ~CFramework();

		// �������ͷŲ�������ģ�����Ϊ�����ļ���
		virtual IBundleContext* CALL_TYPE NewContext(const char*);
		virtual bool CALL_TYPE FreeContext(IBundleContext*&);

        // ��ȡ���·��
        string GetPath() const;

    private:
        // ������ڴ���Ŀ¼
        const string m_path;
    };
}
