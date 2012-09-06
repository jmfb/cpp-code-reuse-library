////////////////////////////////////////////////////////////////////////////////
// Filename:    ErrorUtility.h
// Description: This file includes all error classes and defines utility functions.
//
// Created:     2005-03-19 19:30:31
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_ERRORUTILITY_H)
#define CRL_ERRORUTILITY_H

#include <windows.h>
#include <wininet.h>
#include <CRL/CError.h>
#include <CRL/CCallStack.h>
#include <CRL/CCallTrace.h>
#include <CRL/CScope.h>
#include <CRL/StringUtility.h>

namespace ERR
{
    inline void CheckError(
        bool error, 
        long code,
        const std::string& function,
        const std::string& location,
        const std::string& description)
    {
        if (error)
        {
            throw(CError(code, function, location, description, CCallStack::Get().Format()));
        }
    }
    
    inline std::string FormatWindowsError(unsigned long code)
    {
        PTR::CScope<PTR::CLocalPolicy<char> > buffer;
        //Use format message to get the error code description from the system
        unsigned long ret = ::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<char*>(&buffer),
            0,
            NULL);
        if (ret != 0)
        {
            return(buffer.Get());
        }
        else if (code == ERROR_INTERNET_EXTENDED_ERROR)
        {
            //Read the extended internet error information from the last response info
            unsigned long size = 0;
            BOOL b = ::InternetGetLastResponseInfo(&code, NULL, &size);
            if (b && size > 0)
            {
                ++size;
                STRING::CStringPtr description(new char[size]);
                b = ::InternetGetLastResponseInfo(&code, description.Get(), &size);
                if (b)
                {
                    return(description.Get());
                }
            }
            return("InternetGetLastResponseInfo did not return an error description.");
        }
        else if (code >= INTERNET_ERROR_BASE && code <= INTERNET_ERROR_LAST)
        {
            //Lookup internet error codes using the wininet.dll module error table
            ret = ::FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_HMODULE |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                ::GetModuleHandle("wininet.dll"),
                code,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<char*>(&buffer),
                0,
                NULL);
            if (ret != 0)
            {
                return(buffer.Get());
            }
        }
        return("FormatMessage did not return an error description.");
    }
    
    inline void CheckWindowsError(
        bool error,
        unsigned long code,
        const std::string& function,
        const std::string& location)
    {
        if (error)
        {
            throw(CError(code, function, location, FormatWindowsError(code), CCallStack::Get().Format()));
        }
    }
    
    inline void CheckWindowsError(
        bool error,
        const std::string& function,
        const std::string& location)
    {
        if (error)
        {
            unsigned long code = ::GetLastError();
            throw(CError(code, function, location, FormatWindowsError(code), CCallStack::Get().Format()));
        }
    }
}

#endif //CRL_ERRORUTILITY_H

