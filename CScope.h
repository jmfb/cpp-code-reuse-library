////////////////////////////////////////////////////////////////////////////////
// Filename:    CScope.h
// Description: This file declares the CScope class.  This class is used to scope
//              something.  Data can be stored and retrieved, and the destructor
//              will invoke a handler function.  The data type should be a simple
//              data type (built in type).
//
// Created:     2005-03-19 17:46:27
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CSCOPE_H)
#define CRL_CSCOPE_H

#include <windows.h>
#include <cstddef>

namespace PTR
{
    
template <typename T>
class CScope
{
public:
    typedef T policy_t;
    typedef CScope<policy_t> this_t;
    typedef typename policy_t::data_t data_t;

    CScope(data_t data = policy_t::GetDefault()) : mData(data)
    {
    }

    ~CScope()
    {
        Release();
    }

    this_t& operator=(data_t data)
    {
        Attach(data);
        return(*this);
    }
    
    bool IsNull() const
    {
        return(mData == policy_t::GetDefault());
    }
    
    void Release()
    {
        if (mData != policy_t::GetDefault())
        {
            policy_t::Destroy(mData);
            mData = policy_t::GetDefault();
        }
    }
    
    void Attach(data_t data)
    {
        if (data != mData)
        {
            Release();
            mData = data;
        }
    }
    
    data_t Detach()
    {
        data_t data = mData;
        mData = policy_t::GetDefault();
        return(data);
    }
    
    data_t Get() const
    {
        return(mData);
    }
    
    data_t* operator&()
    {
        return(&mData);
    }

private:
    CScope(const this_t& rhs);
    this_t& operator=(const this_t& rhs);

private:
    data_t mData;
};

//policy class for pointers
template <typename T, bool TArray = false>
class CPtrPolicy
{
public:
    typedef T* data_t;
    
    static T* GetDefault()
    {
        return(NULL);
    }
    
    static void Destroy(T* data)
    {
        if (TArray)
        {
            delete [] data;
        }
        else
        {
            delete data;
        }
    }
};

//Policy class for Local memory
template <typename T>
class CLocalPolicy
{
public:
    typedef T* data_t;
    static T* GetDefault() {return(NULL);}
    static void Destroy(T* h) {::LocalFree(h);}
};

//Policy class for Global memory
template <typename T>
class CGlobalPolicy
{
public:
    typedef T* data_t;
    static T* GetDefault() {return(NULL);}
    static void Destroy(T* h) {::GlobalFree(h);}
};

}

#endif //CRL_CSCOPE_H

