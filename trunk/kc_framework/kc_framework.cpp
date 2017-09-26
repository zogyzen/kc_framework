#include "std.h"
#include "framework.h"

extern "C"
{
	// �������Ŀ��
	IFramework& CALL_TYPE GetFramework(const char* Path)
	{
	    // ת��Ϊ����·��
	    boost::filesystem::path _path(Path);
	    _path = boost::filesystem::system_complete(_path);
        string sPath = _path.string();
        // ��ʽ��·���ַ���
        sPath = CUtilFunc::FormatPath(sPath);
        // �������������Ŀ��
        static CFramework fw(sPath);
		return fw;
	}
}
