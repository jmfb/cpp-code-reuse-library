////////////////////////////////////////////////////////////////////////////////
// Filename:    CInternet.h
// Description: ...
//
// Created:     2005-04-03 16:02:23
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CINTERNET_H)
#define CRL_CINTERNET_H

#include <windows.h>
#include <wininet.h>
#include <CRL/CHandle.h>
#include <CRL/ErrorUtility.h>

namespace NET
{

class CInternet : public WIN::CInetHandle
{
public:
    CInternet()
    {
    }
    ~CInternet()
    {
    }
    
    void Connect(
        const std::string& agent = "",
        unsigned long access = INTERNET_OPEN_TYPE_DIRECT,
        unsigned long flags = 0)
    {
        Attach(::InternetOpen(agent.c_str(), access, NULL, NULL, flags));
        ERR::CheckWindowsError(IsNull(), "NET::CInternet::Connect", "InternetOpen");
    }

private:
    CInternet(const CInternet& rhs);
    CInternet& operator=(const CInternet& rhs);
};

}

#endif //CRL_CINTERNET_H

