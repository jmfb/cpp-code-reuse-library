////////////////////////////////////////////////////////////////////////////////
// Filename:    CPipe.h
// Description: ...
//
// Created:     2005-04-02 19:11:57
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CPIPE_H)
#define CRL_CPIPE_H

#include <CRL/CHandle.h>
#include <CRL/ErrorUtility.h>

namespace WIN
{

class CPipe : public CHandle
{
public:
    CPipe()
    {
    }
    ~CPipe()
    {
    }
    
    unsigned long PeekSize()
    {
        unsigned long size = 0;
        BOOL b = ::PeekNamedPipe(Get(), NULL, 0, NULL, &size, NULL);
        ERR::CheckWindowsError(!b, "WIN::CPipe::PeekSize", "PeekNamedPipe");
        return(size);
    }
    
    unsigned long Read(void* buffer, unsigned long size)
    {
        unsigned long read = 0;
        BOOL b = ::ReadFile(Get(), buffer, size, &read, NULL);
        ERR::CheckWindowsError(!b, "WIN::CPipe::Read", "ReadFile");
        return(read);
    }
    
    std::string ReadString()
    {
        std::ostringstream out;
        static const unsigned long SIZE = 1024;
        char buffer[SIZE];
        if (PeekSize() > 0)
        {
            for (;;)
            {
                unsigned long read = Read(buffer, SIZE);
                out.write(buffer, read);
                if (read < SIZE) break;
            }
        }
        return(out.str());
    }
};

inline void CreatePipe(CPipe& read, CPipe& write, SECURITY_ATTRIBUTES* sa = NULL)
{
    BOOL b = ::CreatePipe(&read, &write, sa, 0);
    ERR::CheckWindowsError(!b, "WIN::CreatePipe", "CreatePipe");
}
    
}

#endif //CRL_CPIPE_H

