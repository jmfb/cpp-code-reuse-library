////////////////////////////////////////////////////////////////////////////////
// Filename:    CRegKey.h
// Description: ...
//
// Created:     2005-03-19 22:46:41
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CREGKEY_H)
#define CRL_CREGKEY_H

#include <windows.h>
#include <CRL/CScope.h>
#include <CRL/ErrorUtility.h>
#include <CRL/StringUtility.h>

namespace REG
{

class CRegKeyPolicy
{
public:
    typedef HKEY data_t;
    static HKEY GetDefault() {return(NULL);}
    static void Destroy(HKEY key) {::RegCloseKey(key);}
};

typedef PTR::CScope<CRegKeyPolicy> CRegKeyPtr;
    
class CRegKey : public CRegKeyPtr
{
public:
    CRegKey()
    {
    }
    CRegKey(HKEY parent, const std::string& name, REGSAM access = KEY_ALL_ACCESS)
    {
        Create(parent, name, access);
    }
    ~CRegKey()
    {
    }
    
    operator HKEY() const
    {
        return(Get());
    }
    
    void Create(HKEY parent, const std::string& name, REGSAM access = KEY_ALL_ACCESS)
    {
        HKEY key = NULL;
        unsigned long ret = ::RegCreateKeyEx(
            parent,
            name.c_str(),
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            access,
            NULL,
            &key,
            NULL);
        ERR::CheckWindowsError(ret != ERROR_SUCCESS, ret, "CRegKey::Create", "RegCreateKeyEx");
        Attach(key);
    }
    
    void SetNumber(const std::string& name, unsigned long val)
    {
        long ret = ::RegSetValueEx(
            Get(),
            name.c_str(),
            0,
            REG_DWORD,
            reinterpret_cast<const BYTE*>(&val),
            sizeof(val));
        ERR::CheckWindowsError(ret != ERROR_SUCCESS, ret, "CRegKey::SetNumber", "RegSetValueEx");
    }
    
    void SetString(const std::string& name, const std::string& val)
    {
        long ret = ::RegSetValueEx(
            Get(),
            name.c_str(),
            0,
            REG_SZ,
            reinterpret_cast<const BYTE*>(val.c_str()),
            val.size() + 1);
        ERR::CheckWindowsError(ret != ERROR_SUCCESS, ret, "CRegKey::SetString", "RegSetValueEx");
    }
    
    unsigned long GetNumber(const std::string& name)
    {
        static const std::string FUNCTION = "CRegKey::GetNumber";
        unsigned long val = 0;
        unsigned long type = 0;
        unsigned long size = sizeof(val);
        long ret = ::RegQueryValueEx(Get(), name.c_str(), NULL, &type, reinterpret_cast<BYTE*>(&val), &size);
        ERR::CheckWindowsError(ret != ERROR_SUCCESS, ret, FUNCTION, "RegQueryValueEx");
        ERR::CheckError(type != REG_DWORD, 0, FUNCTION, "", "Type was not REG_DWORD.");
        return(val);
    }
    
    std::string GetString(const std::string& name)
    {
        static const std::string FUNCTION = "CRegKey::GetString";
        unsigned long size = 0;
        unsigned long type = 0;
        
        long ret = ::RegQueryValueEx(Get(), name.c_str(), NULL, &type, NULL, &size);
        ERR::CheckWindowsError(ret != ERROR_SUCCESS, ret, FUNCTION, "ReqQueryValueEx for size.");
        ERR::CheckError(type != REG_SZ, 0, FUNCTION, "", "Type was not REG_SZ.");

        STRING::CStringPtr buffer(new char[size]);
        ret = ::RegQueryValueEx(Get(), name.c_str(), NULL, &type, reinterpret_cast<BYTE*>(buffer.Get()), &size);
        ERR::CheckWindowsError(ret != ERROR_SUCCESS, ret, FUNCTION, "RegQueryValueEx for string.");        
        return(buffer.Get());
    }
};

}

#endif //CRL_CREGKEY_H

