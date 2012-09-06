////////////////////////////////////////////////////////////////////////////////
// Filename:    CInternetRequest.h
// Description: ...
//
// Created:     2005-04-03 16:12:47
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CINTERNETREQUEST_H)
#define CRL_CINTERNETREQUEST_H

#include <iostream>
#include <sstream>
#include <CRL/CInternetServer.h>
#include <CRL/StringUtility.h>

namespace NET
{
    
class CInternetRequest : public WIN::CInetHandle
{
public:
    CInternetRequest()
    {
    }
    ~CInternetRequest()
    {
    }
    
    void Open(
        CInternetServer& server,
        const std::string& url,
        const std::string& referrer = "")
    {
        static const std::string FUNCTION = "NET::CInternetRequest::Open";
        static const char* TYPES[] = {"text/*", "image/*", "video/*", "audio/*", NULL};
        Attach(::HttpOpenRequest(
            server.Get(),
            "GET",
            url.c_str(),
            "HTTP/1.1",
            referrer.c_str(),
            TYPES,
            0,
            0));
        ERR::CheckWindowsError(IsNull(), FUNCTION, "HttpOpenRequest");
        
        BOOL b = ::HttpSendRequest(Get(), NULL, 0, NULL, 0);
        ERR::CheckWindowsError(!b, FUNCTION, "HttpSendRequest");
        
        //Check the status code response
        unsigned long code = GetStatusCode();
        if (code != HTTP_STATUS_OK)
        {
            std::ostringstream out;
            out << "Server:   " << server.GetName() << std::endl
                << "URL:      " << url << std::endl
                << "Referrer: " << referrer << std::endl
                << code << " - " << GetStatusText();
            throw(ERR::CError(code, FUNCTION, "Checking Status Code", out.str()));
        }
    }
    
    unsigned long GetStatusCode()
    {
        static const std::string FUNCTION = "NET::CInternetRequest::GetStatusCode";
        unsigned long code = 0;
        unsigned long size = sizeof(code);
        BOOL b = ::HttpQueryInfo(Get(), HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER, &code, &size, NULL);
        ERR::CheckWindowsError(!b, FUNCTION, "HttpQueryInfo");
        return(code);
    }
    
    std::string GetStatusText()
    {
        static const std::string FUNCTION = "NET::CInternetRequest::GetStatusText";
        unsigned long size = 0;
        BOOL b = ::HttpQueryInfo(Get(), HTTP_QUERY_STATUS_TEXT, NULL, &size, NULL);
        unsigned long code = ::GetLastError();
        if (b || (code == ERROR_INSUFFICIENT_BUFFER && size == 0))
        {
            return("");
        }
        else if (code == ERROR_INSUFFICIENT_BUFFER)
        {
            STRING::CStringPtr buffer(new char[size]);
            b = ::HttpQueryInfo(Get(), HTTP_QUERY_STATUS_TEXT, buffer.Get(), &size, NULL);
            ERR::CheckWindowsError(!b, FUNCTION, "HttpQueryInfo(text)");
            return(buffer.Get());
        }
        throw(ERR::CError(code, FUNCTION, "HttpQueryInfo(size)", ERR::FormatWindowsError(code)));
    }
    
    void Read(std::ostream& out)
    {
        static const std::string FUNCTION = "NET::CInternetRequest::Read";
        static const unsigned long SIZE = 1 << 16;
        unsigned long size = SIZE;
        char buffer[SIZE];
        
        do
        {
            BOOL b = ::InternetReadFile(Get(), buffer, SIZE, &size);
            ERR::CheckWindowsError(!b, FUNCTION, "InternetReadFile");
            out.write(buffer, size);
        }
        while (size > 0);
    }
    
    std::string GetText()
    {
        std::ostringstream out;
        Read(out);
        return(out.str());
    }
    
private:
    CInternetRequest(const CInternetRequest& rhs);
    CInternetRequest& operator=(const CInternetRequest& rhs);
};

}

#endif //CRL_CINTERNETREQUEST_H

