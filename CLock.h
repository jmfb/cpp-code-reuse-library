////////////////////////////////////////////////////////////////////////////////
// Filename:    CLock.h
// Description: ...
//
// Created:     2005-04-03 18:27:37
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CLOCK_H)
#define CRL_CLOCK_H

#include <windows.h>
#include <cstddef>

namespace PTR
{

class CLock
{
public:
    CLock()
    {
        ::InitializeCriticalSection(&mCS);
    }
    ~CLock()
    {
        ::DeleteCriticalSection(&mCS);
    }
    
    void Enter()
    {
        ::EnterCriticalSection(&mCS);
    }
    void Leave()
    {
        ::LeaveCriticalSection(&mCS);
    }
    
private:
    CRITICAL_SECTION mCS;
    
private:
    CLock(const CLock& rhs);
    CLock& operator=(const CLock& rhs);
};

class CLocker
{
public:
    CLocker(CLock& lock) : mLock(&lock)
    {
        mLock->Enter();
    }
    ~CLocker()
    {
        Leave();
    }
    
    void Leave()
    {
        if (mLock)
        {
            mLock->Leave();
            mLock = NULL;
        }
    }

private:
    CLock* mLock;
    
private:
    CLocker(const CLocker& rhs);
    CLocker& operator=(const CLocker& rhs);
};

}

#endif //CRL_CLOCK_H

