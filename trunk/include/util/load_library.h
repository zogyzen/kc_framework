#pragma once

#ifdef WIN32    // Windows����
    #include <windows.h>
	constexpr const char c_LoadOutLibExtName[] = ".dll";
#else           // linux����
    #include <dlfcn.h>
	constexpr const char c_LoadOutLibExtName[] = ".so";
#endif

#include <string>
#include <memory>
using namespace std;

namespace KC
{
    template<bool bDefLoad = false>
    class KLoadLibrary
    {
    public:
        // ����
        KLoadLibrary(const char* strLibFileName, string& sError) : m_strLibFileName(strLibFileName)
        {
            sError = "";
            m_strLibFileName += m_LibExtName;
            if (bDefLoad) sError = LoadLib();
        }
        KLoadLibrary(const char* strLibFileName) : m_strLibFileName(strLibFileName)
        {
            m_strLibFileName += m_LibExtName;
            if (bDefLoad) LoadLib();
        }
        // ж�ؿ��ļ�
        ~KLoadLibrary()
        {
            #ifdef WIN32    // Windows����
            if (0 != m_lib_handle) ::FreeLibrary(m_lib_handle);
            #else          // linux����
            if (nullptr != m_lib_handle) dlclose(m_lib_handle);
            #endif
        }
        // ���ض�̬��
        string LoadLib(void)
        {
            string sError = "";
            #ifdef WIN32    // Windows����
            if (0 == m_lib_handle)
            {
                m_lib_handle = ::LoadLibrary(m_strLibFileName.c_str());
                if (0 == m_lib_handle)
                    sError = m_strLibFileName + " (Can't load lib file)";
            }
            #else           // linux����
            if (nullptr == m_lib_handle)
            {
                m_lib_handle = dlopen(m_strLibFileName.c_str(), RTLD_LAZY);
                if (nullptr == m_lib_handle)
                    sError = m_strLibFileName + " [Can't load lib file]" + dlerror();
            }
            #endif
            return sError;
        }
        // ���ؿ⺯��
        template<typename _Func>
        inline _Func GetLibFunc(const char* strFuncName, string& sError)
        {
            sError = LoadLib();
            _Func _f = (_Func)GetLibFunc(strFuncName);
            if (sError.empty() && _Func() == _f)
                sError = m_strLibFileName + "::" + strFuncName + " (Can't get lib function)";
            return _f;
        }

        template<typename _Func>
        inline _Func GetLibFunc(const char* strFuncName)
        {
            string sError;
            return this->GetLibFunc<_Func>(strFuncName, sError);
        }
        void* GetLibFunc(const char* strFuncName)
        {
            void* _f = nullptr;
            #ifdef WIN32    // Windows����
            if (0 != m_lib_handle) _f = (void*)::GetProcAddress(m_lib_handle, strFuncName);
            #else           // linux����
            if (nullptr != m_lib_handle) _f = (void*)dlsym(m_lib_handle, strFuncName);
            #endif
            return _f;
        }
        // ���ؿ��ļ���
        const char* GetLibFileName(void) const { return m_LibExtName; }
        // �������ʱ��
        void SetLastTime(void) { m_lastTime = std::time(0); }
        time_t GetLastTime(void) { return m_lastTime; }

    private:
        // ��ʵ�����
        #ifdef WIN32    // Windows����
        HINSTANCE m_lib_handle = 0;
        #else           // linux����
        void* m_lib_handle = nullptr;
        #endif
		// ����չ��
        const char* m_LibExtName = c_LoadOutLibExtName;
        // ���ļ�ȫ��
        string m_strLibFileName = "";
        // �������ʱ��
        time_t m_lastTime = std::time(0);
    };
    typedef std::shared_ptr<KLoadLibrary<>> TKLoadLibraryPtr;
}
