////////////////////////////////////////////////////////////////////////////////
// Filename:    CHandle.h
// Description: ...
//
// Created:     2005-04-02 19:12:46
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CHANDLE_H)
#define CRL_CHANDLE_H

#include <wininet.h>
#include <CRL/CScope.h>

namespace WIN
{
    class CHandlePolicy
    {
    public:
        typedef HANDLE data_t;
        static HANDLE GetDefault() {return(NULL);}
        static void Destroy(HANDLE h) {::CloseHandle(h);}
    };
    
    typedef PTR::CScope<CHandlePolicy> CHandle;
    
    class CInetHandlePolicy
    {
    public:
        typedef HINTERNET data_t;
        static HINTERNET GetDefault() {return(NULL);}
        static void Destroy(HINTERNET h) {::InternetCloseHandle(h);}
    };
    
    typedef PTR::CScope<CInetHandlePolicy> CInetHandle;
}

#endif //CRL_CHANDLE_H

