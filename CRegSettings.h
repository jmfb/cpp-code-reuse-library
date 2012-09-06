////////////////////////////////////////////////////////////////////////////////
// Filename:    CRegSettings.h
// Description: This file declares the CRegSettings class.  This class wraps a
//              node under the software registry hive of the current user.  Save
//              and load miscellaneous settings for the current app using this
//              class.
//
// Created:     2005-03-20 01:26:13
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CREGSETTINGS_H)
#define CRL_CREGSETTINGS_H

#include <CRL/CRegKey.h>
#include <CRL/FileUtility.h>

namespace REG
{

class CRegSettings
{
public:
    CRegSettings() : mKey(CRegKey(HKEY_CURRENT_USER, "Software"), FSYS::GetAppName())
    {
    }
    
    void SetBool(const std::string& name, bool val)
    {
        SetNumber(name, val ? 1 : 0);
    }
    template <typename T>
    void SetEnum(const std::string& name, T val)
    {
        SetNumber(name, static_cast<unsigned long>(val));
    }
    void SetNumber(const std::string& name, unsigned long val)
    {
        mKey.SetNumber(name, val);
    }
    void SetString(const std::string& name, const std::string& val)
    {
        mKey.SetString(name, val);
    }
    
    bool GetBool(const std::string& name, bool def = false)
    {
        try
        {
            return(mKey.GetNumber(name) != 0);
        }
        catch (const ERR::CError&)
        {
            return(def);
        }
    }
    template <typename T>
    T GetEnum(const std::string& name, T def)
    {
        try
        {
            return(static_cast<T>(mKey.GetNumber(name)));
        }
        catch (const ERR::CError&)
        {
            return(def);
        }
    }
    unsigned long GetNumber(const std::string& name, unsigned long def = 0)
    {
        try
        {
            return(mKey.GetNumber(name));
        }
        catch (const ERR::CError&)
        {
            return(def);
        }
    }
    std::string GetString(const std::string& name, const std::string& def = "")
    {
        try
        {
            return(mKey.GetString(name));
        }
        catch (const ERR::CError&)
        {
            return(def);
        }
    }
    
private:
    CRegKey mKey;
};

}

#endif //CRL_CREGSETTINGS_H

