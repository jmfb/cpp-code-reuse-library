////////////////////////////////////////////////////////////////////////////////
// Filename:    CInternetServer.h
// Description: ...
//
// Created:     2005-04-03 16:07:12
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CINTERNETSERVER_H)
#define CRL_CINTERNETSERVER_H

#include <string>
#include <CRL/CInternet.h>

namespace NET
{
    
class CInternetServer : public WIN::CInetHandle
{
public:
    CInternetServer()
    {
    }
    ~CInternetServer()
    {
    }
    
    void Connect(
        CInternet& internet,
        const std::string& server,
        const std::string& username = "",
        const std::string& password = "")
    {
        Attach(::InternetConnect(
            internet.Get(),
            server.c_str(),
            INTERNET_DEFAULT_HTTP_PORT,
            username.c_str(),
            password.c_str(),
            INTERNET_SERVICE_HTTP,
            0,
            0));
        ERR::CheckWindowsError(IsNull(), "NET::CInternetServer::Connect", "InternetConnect");
        mName = server;
    }
    
    const std::string& GetName() const
    {
        return(mName);
    }

private:
    std::string mName;
    
private:
    CInternetServer(const CInternetServer& rhs);
    CInternetServer& operator=(const CInternetServer& rhs);
};

}

#endif //CRL_CINTERNETSERVER_H

