////////////////////////////////////////////////////////////////////////////////
// Filename:    CCallTrace.h
// Description: ...
//
// Created:     2005-04-10 21:46:09
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CCALLTRACE_H)
#define CRL_CCALLTRACE_H

#include <CRL/CCallStack.h>

namespace ERR
{
    
class CCallTrace
{
public:
    CCallTrace(const std::string& function) : mFunction(function)
    {
        CCallStack::Get().PushCall(mFunction);
    }
    ~CCallTrace()
    {
        CCallStack::Get().PopCall();
    }
    
    operator const std::string&() const
    {
        return(mFunction);
    }
    
    void AddNote(const std::string& note)
    {
        CCallStack::Get().AddNote(note);
    }
    
    void SetValue(const std::string& name, const std::string& value)
    {
        CCallStack::Get().SetValue(name, value);
    }
    void RemoveValue(const std::string& name)
    {
        CCallStack::Get().RemoveValue(name);
    }
    
private:
    std::string mFunction;
};

}

#endif //CRL_CCALLTRACE_H

