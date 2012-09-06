////////////////////////////////////////////////////////////////////////////////
// Filename:    CAutoPtr.h
// Description: ...
//
// Created:     2005-04-03 16:20:55
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CAUTOPTR_H)
#define CRL_CAUTOPTR_H

#include <cstddef>

namespace PTR
{

template <typename T>
class CAutoPtr
{
public:
    typedef T data_t;
    
    CAutoPtr(T* pointer = NULL) : mPointer(pointer), mRefCount(NULL)
    {
        if (mPointer)
        {
            mRefCount = new unsigned long(1);
        }
    }
    CAutoPtr(const CAutoPtr<T>& rhs) : mPointer(rhs.mPointer), mRefCount(rhs.mRefCount)
    {
        if (mPointer)
        {
            ++*mRefCount;
        }
    }
    ~CAutoPtr()
    {
        Release();
    }
    
    CAutoPtr<T>& operator=(const CAutoPtr<T>& rhs)
    {
        if (this != &rhs && mPointer != rhs.mPointer)
        {
            CAutoPtr<T> temp(*this);
            Release();
            mPointer = rhs.mPointer;
            mRefCount = rhs.mRefCount;
            if (mPointer)
            {
                ++*mRefCount;
            }
        }
        return(*this);
    }
    
    T& operator*() const
    {
        return(*mPointer);
    }
    T* operator->() const
    {
        return(mPointer);
    }
    
    bool IsNull() const
    {
        return(mPointer == NULL);
    }
    T* Get() const
    {
        return(mPointer);
    }
    void Release()
    {
        if (mPointer)
        {
            if (--*mRefCount == 0)
            {
                delete mRefCount;
                delete mPointer;
            }
            mRefCount = NULL;
            mPointer = NULL;
        }
    }
    T* Detach()
    {
        T* temp = mPointer;
        if (mPointer)
        {
            if (--*mRefCount == 0)
            {
                delete mRefCount;
            }
            mRefCount = NULL;
            mPointer = NULL;
        }
        return(temp);
    }
    
    bool operator==(const CAutoPtr<T>& rhs) const
    {
        return(mPointer == rhs.mPointer);
    }
    bool operator!=(const CAutoPtr<T>& rhs) const
    {
        return(mPointer != rhs.mPointer);
    }
    bool operator<(const CAutoPtr<T>& rhs) const
    {
        return(mPointer < rhs.mPointer);
    }
    
private:
    T* mPointer;
    unsigned long* mRefCount;
};

}

#endif //CRL_CAUTOPTR_H

