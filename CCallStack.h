////////////////////////////////////////////////////////////////////////////////
// Filename:    CCallStack.h
// Description: ...
//
// Created:     2005-04-10 21:24:30
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CCALLSTACK_H)
#define CRL_CCALLSTACK_H

#include <windows.h>
#include <list>
#include <string>
#include <map>
#include <sstream>
#include <CRL/CSingleton.h>
#include <CRL/CLock.h>

namespace ERR
{

class CCallStack : public PTR::CSingleton<CCallStack>
{
public:
    typedef std::list<std::string> notes_t;
    struct call_t
    {
        std::string mName;
        notes_t mNotes;
    };
    typedef std::list<call_t> stack_t;
    typedef std::map<std::string, std::string> values_t;
    struct data_t
    {
        stack_t mStack;
        values_t mValues;
    };
    typedef std::map<unsigned long, data_t> map_t;

    void PushCall(const std::string& function)
    {
        PTR::CLocker lock(mLock);
        call_t call;
        call.mName = function;
        mData[::GetCurrentThreadId()].mStack.push_front(call);
    }
    
    void AddNote(const std::string& note)
    {
        PTR::CLocker lock(mLock);
        mData[::GetCurrentThreadId()].mStack.front().mNotes.push_back(note);
    }
    
    void PopCall()
    {
        PTR::CLocker lock(mLock);
        mData[::GetCurrentThreadId()].mStack.pop_front();
    }
    
    void SetValue(const std::string& name, const std::string& value)
    {
        PTR::CLocker lock(mLock);
        mData[::GetCurrentThreadId()].mValues[name] = value;
    }
    
    void RemoveValue(const std::string& name)
    {
        PTR::CLocker lock(mLock);
        mData[::GetCurrentThreadId()].mValues.erase(name);
    }
    
    std::string Format()
    {
        PTR::CLocker lock(mLock);
        const data_t& data = mData[::GetCurrentThreadId()];
        
        std::ostringstream out;
        if (!data.mValues.empty())
        {
            for (values_t::const_iterator iter = data.mValues.begin(); iter != data.mValues.end(); ++iter)
            {
                out << iter->first << "=" << iter->second << std::endl;
            }
            out << std::endl;
        }
        
        for (stack_t::const_iterator iter = data.mStack.begin(); iter != data.mStack.end(); ++iter)
        {
            out << iter->mName << std::endl;
            for (notes_t::const_iterator note = iter->mNotes.begin(); note != iter->mNotes.end(); ++note)
            {
                out << " - " << *note << std::endl;
            }
        }
        
        return(out.str());
    }
    
private:
    CCallStack()
    {
    }
    ~CCallStack()
    {
    }
    
    PTR::CLock mLock;
    map_t mData;

    friend class PTR::CSingleton<CCallStack>;

private:
    CCallStack(const CCallStack& rhs);
    CCallStack& operator=(const CCallStack& rhs);
};

}

#endif //CRL_CCALLSTACK_H

