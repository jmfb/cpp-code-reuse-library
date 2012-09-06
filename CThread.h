////////////////////////////////////////////////////////////////////////////////
// Filename:    CThread.h
// Description: ...
//
// Created:     2005-04-03 17:40:26
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CTHREAD_H)
#define CRL_CTHREAD_H

#include <windows.h>
#include <process.h>
#include <CRL/CHandle.h>
#include <CRL/ErrorUtility.h>

namespace PTR
{

class CThread
{
public:
    CThread() : mThreadID(0)
    {
    }
    virtual ~CThread()
    {
    }
    
    virtual unsigned int OnRun() = 0;
    
    void Create(bool suspended = false)
    {
        mHandle = reinterpret_cast<HANDLE>(::_beginthreadex(
            NULL,
            0,
            ThreadProc,
            this,
            suspended ? CREATE_SUSPENDED : 0,
            &mThreadID));
        ERR::CheckWindowsError(mHandle.IsNull(), "PTR::CThread::Create", "_beginthreadex");
    }

    unsigned long Suspend()
    {
        unsigned long count = ::SuspendThread(mHandle.Get());
        ERR::CheckWindowsError(count == 0xffffffff, "PTR::CThread::Suspend", "SuspendThread");
        return(count);
    }
    unsigned long Resume()
    {
        unsigned long count = ::ResumeThread(mHandle.Get());
        ERR::CheckWindowsError(count == 0xffffffff, "PTR::CThread::Resume", "ResumeThread");
        return(count);
    }
    bool Wait(unsigned long timeout = INFINITE)
    {
        unsigned long ret = ::WaitForSingleObject(mHandle.Get(), timeout);
        if (ret == WAIT_OBJECT_0) return(true);
        ERR::CheckWindowsError(ret != WAIT_TIMEOUT, "PTR::CThread::Wait", "WaitForSingleObject");
        return(false);
    }
    void Kill(unsigned long code = 0)
    {
        BOOL b = ::TerminateThread(mHandle.Get(), code);
        ERR::CheckWindowsError(!b, "PTR::CThread::Kill", "TerminateThread");
    }
    unsigned long GetExitCode() const
    {
        unsigned long code = 0;
        BOOL b = ::GetExitCodeThread(mHandle.Get(), &code);
        ERR::CheckWindowsError(!b, "PTR::CThread::GetExitCode", "GetExitCodeThread");
        return(code);
    }
    bool StillActive() const
    {
        return(GetExitCode() == STILL_ACTIVE);
    }
    
    bool IsNull() const
    {
        return(mHandle.IsNull());
    }
    unsigned long GetThreadID() const
    {
        return(mThreadID);
    }
    HANDLE GetHandle() const
    {
        return(mHandle.Get());
    }
    
private:
    static unsigned int __stdcall ThreadProc(void* p)
    {
        return(reinterpret_cast<CThread*>(p)->OnRun());
    }
    
    WIN::CHandle mHandle;
    unsigned int mThreadID;
    
private:
    CThread(const CThread& rhs);
    CThread& operator=(const CThread& rhs);
};

}

#endif //CRL_CTHREAD_H

