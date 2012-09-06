////////////////////////////////////////////////////////////////////////////////
// Filename:    CEvent.h
// Description: ...
//
// Created:     2005-04-03 21:22:59
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CEVENT_H)
#define CRL_CEVENT_H

#include <CRL/CHandle.h>
#include <CRL/ErrorUtility.h>

namespace PTR
{

class CEvent
{
public:
    CEvent() : mOwned(false)
    {
    }
    CEvent(const CEvent& rhs) : mOwned(false), mEvent(rhs.mEvent.Get())
    {
    }
    ~CEvent()
    {
        Release();
    }
    
    CEvent& operator=(const CEvent& rhs)
    {
        if (this != &rhs)
        {
            Release();
            mOwned = false;
            mEvent = rhs.mEvent.Get();
        }
        return(*this);
    }
    
    void Create(bool manual, bool state, SECURITY_ATTRIBUTES* sa = NULL)
    {
        Release();
        mOwned = true;
        mEvent.Attach(::CreateEvent(sa, manual ? TRUE : FALSE, state ? TRUE : FALSE, NULL));
    }
    
    void Reset()
    {
        ::ResetEvent(mEvent.Get());
    }
    void Set()
    {
        ::SetEvent(mEvent.Get());
    }
    
    bool Wait(unsigned long timeout = INFINITE)
    {
        unsigned long ret = ::WaitForSingleObject(mEvent.Get(), timeout);
        if (ret == WAIT_OBJECT_0) return(true);
        ERR::CheckWindowsError(ret != WAIT_TIMEOUT, "PTR::CEvent::Wait", "WaitForSingleObject");
        return(false);
    }
    
    void Release()
    {
        if (mOwned)
        {
            mEvent.Release();
        }
        else
        {
            mEvent.Detach();
        }
        mOwned = false;
    }
    
private:
    bool mOwned;
    WIN::CHandle mEvent;
};

}

#endif //CRL_CEVENT_H

