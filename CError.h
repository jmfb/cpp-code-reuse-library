////////////////////////////////////////////////////////////////////////////////
// Filename:    CError.h
// Description: This file declares the CError class used for all error reporting.
//
// Created:     2005-03-19 18:47:10
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CERROR_H)
#define CRL_CERROR_H

#include <windows.h>
#include <string>
#include <sstream>
#include <CRL/StringUtility.h>

namespace ERR
{

class CError
{
public:
    CError() : mCode(0)
    {
    }
    CError(
        long code,
        const std::string& function,
        const std::string& location,
        const std::string& description,
        const std::string& callstack = "")
    :   mCode(code),
        mFunction(function),
        mLocation(location),
        mDescription(description),
        mCallStack(callstack)
    {
    }
    CError(const CError& rhs)
    :   mCode(rhs.mCode),
        mFunction(rhs.mFunction),
        mLocation(rhs.mLocation),
        mDescription(rhs.mDescription),
        mCallStack(rhs.mCallStack)
    {
    }
    ~CError()
    {
    }

    CError& operator=(const CError& rhs)
    {
        mCode = rhs.mCode;
        mFunction = rhs.mFunction;
        mLocation = rhs.mLocation;
        mDescription = rhs.mDescription;
        mCallStack = rhs.mCallStack;
        return(*this);
    }
    void Clear()
    {
        mCode = 0;
        mFunction.erase();
        mLocation.erase();
        mDescription.erase();
        mCallStack.erase();
    }
    
    void SetError(
        long code,
        const std::string& function,
        const std::string& location,
        const std::string& description,
        const std::string& callstack = "")
    {
        mCode = code;
        mFunction = function;
        mLocation = location;
        mDescription = description;
        mCallStack = callstack;
    }
    void SetCode(long code)
    {
        mCode = code;
    }
    void SetFunction(const std::string& function)
    {
        mFunction = function;
    }
    void SetLocation(const std::string& location)
    {
        mLocation = location;
    }
    void SetDescription(const std::string& description)
    {
        mDescription = description;
    }
    void SetCallStack(const std::string& callstack)
    {
        mCallStack = callstack;
    }
    
    long GetCode() const
    {
        return(mCode);
    }
    const std::string& GetFunction() const
    {
        return(mFunction);
    }
    const std::string& GetLocation() const
    {
        return(mLocation);
    }
    const std::string& GetDescription() const
    {
        return(mDescription);
    }
    const std::string& GetCallStack() const
    {
        return(mCallStack);
    }
    
    std::string Format() const
    {
        std::ostringstream out;
        if (mCode != 0)
        {
            out << "Code: " << mCode << " (0x" << STRING::hex(mCode) << ")" << std::endl;
        }
        if (!mFunction.empty())
        {
            out << "Function: " << mFunction << std::endl;
        }
        if (!mLocation.empty())
        {
            out << "Location: " << mLocation << std::endl;
        }
        if (!mDescription.empty())
        {
            out << mDescription << std::endl;
        }
        if (!mCallStack.empty())
        {
            out << std::endl << mCallStack;
        }
        return(out.str());
    }
    int Show(
        HWND parent = NULL,
        const std::string& title = "Error",
        unsigned int type = MB_OK|MB_ICONERROR) const
    {
        return(::MessageBox(parent, Format().c_str(), title.c_str(), type));
    }
    
private:
    long mCode;
    std::string mFunction;
    std::string mLocation;
    std::string mDescription;
    std::string mCallStack;
};

}

#endif //CRL_CERROR_H

