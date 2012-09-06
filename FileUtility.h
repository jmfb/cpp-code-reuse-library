#if !defined(CRL_FILEUTILITY_H)
#define CRL_FILEUTILITY_H

#if (_WIN32_WINNT < 0x0500)
#error You must define _WIN32_WINNT to 0x0500 or higher.
#endif

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <string>
#include <cstring>
#include <CRL/CScope.h>
#include <CRL/ErrorUtility.h>
#include <CRL/TimeUtility.h>

namespace FSYS
{
    class CFileHandlePolicy
    {
    public:
        typedef HANDLE data_t;
        static HANDLE GetDefault() {return(INVALID_HANDLE_VALUE);}
        static void Destroy(HANDLE h) {::CloseHandle(h);}
    };
    typedef PTR::CScope<CFileHandlePolicy> CFileHandle;
    
    inline std::string GetModuleFileName(HMODULE module = NULL)
    {
        const unsigned long SIZE = 4096;
        char buffer[SIZE];
        unsigned long ret = ::GetModuleFileName(module, buffer, SIZE);
        ERR::CheckWindowsError(ret == 0, "FSYS::GetModuleFileName", "GetModuleFileName");
        return(buffer);
    }
    
    inline std::string GetFileName(const std::string& fullpath)
    {
        std::string::size_type pos = fullpath.rfind('\\');
        if (pos == std::string::npos)
        {
            return(fullpath);
        }
        return(fullpath.substr(pos + 1));
    }
    
    inline std::string GetFilePath(const std::string& fullpath)
    {
        std::string::size_type pos = fullpath.rfind('\\');
        if (pos == std::string::npos)
        {
            return("");
        }
        return(fullpath.substr(0, pos));
    }
    
    inline std::string GetFolderName(const std::string& fullpath)
    {
        return(GetFileName(GetFilePath(fullpath)));
    }
    
    inline std::string GetFileTitle(const std::string& fullpath)
    {
        std::string name = GetFileName(fullpath);
        std::string::size_type pos = name.rfind('.');
        if (pos == std::string::npos)
        {
            return(name);
        }
        return(name.substr(0, pos));
    }
    
    inline std::string GetFileExt(const std::string& fullpath)
    {
        std::string name = GetFileName(fullpath);
        std::string::size_type pos = name.rfind('.');
        if (pos == std::string::npos)
        {
            return("");
        }
        return(name.substr(pos + 1));
    }
    
    inline std::string GetAppName()
    {
        return(GetFileTitle(GetModuleFileName()));
    }
    
    inline std::string FormatPath(std::string path, const std::string& file, char sep = '\\')
    {
        if (!path.empty() && path.at(path.size() - 1) != sep)
        {
            path.append(1, sep);
        }
        
        if (!file.empty() && file.at(0) == sep)
        {
            path.append(file.begin() + 1, file.end());
        }
        else
        {
            path.append(file);
        }
        
        return(path);
    }
    
    inline bool FileExists(const std::string& filename)
    {
        CFileHandle file(::CreateFile(filename.c_str(), 0, 0, NULL, OPEN_EXISTING, 0, NULL));
        return(!file.IsNull());
    }
    
    inline bool PathExists(const std::string& path)
    {
        CFileHandle file(::CreateFile(path.c_str(), 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL));
        return(!file.IsNull());
    }
    
    inline void CreatePath(const std::string& path)
    {
        if (!PathExists(path))
        {
            int retval = ::SHCreateDirectoryEx(NULL, path.c_str(), NULL);
            ERR::CheckWindowsError(retval != ERROR_SUCCESS, retval, "FSYS::CreateDirectory", "SHCreateDirectoryEx");
        }
    }
    
    inline std::chrono::system_clock::time_point GetFileLastWriteTime(const std::string& path)
    {
        WIN32_FILE_ATTRIBUTE_DATA fileData = {0};
        std::memset(&fileData, 0, sizeof(fileData));
        ::GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fileData);
        return TIME::ConvertFileTimeUtcToTimePoint(fileData.ftLastWriteTime);
    }
}

#endif //CRL_FILEUTILITY_H

