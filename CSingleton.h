////////////////////////////////////////////////////////////////////////////////
// Filename:    CSingleton.h
// Description: This file declares the CSingleton class.  The singleton class
//              serves as a base class for another class where exactly one instance
//              must exist.  The instance is created upon first access through
//              the Get static function.  You can let the singleton go out of
//              scope through normal static variable cleanup, or you can call
//              Free to force it to occur.
//
// Created:     2005-03-19 18:14:36
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CSINGLETON_H)
#define CRL_CSINGLETON_H

#include <CRL/CScope.h>

namespace PTR
{

template <typename T>
class CSingleton
{
public:
    typedef T singleton_t;
    typedef CSingleton<singleton_t> this_t;

    static singleton_t& Get()
    {
        if (mScopeP.IsNull())
        {
            mP = new singleton_t;
            mScopeP = mP;
        }
        return(*mP);
    }
    
    static void Free()
    {
        mP = NULL;
        mScopeP.Release();
    }
    
    static bool Exists()
    {
        return(!mScopeP.IsNull());
    }
    
protected:
    //make the constructor protected so that you must derive from this class
    CSingleton()
    {
    }
    
    //make the desctructor protected so that no clients can invoke it
    virtual ~CSingleton()
    {
    }
    
    //make the pointer policy a friend so that it can delete the singleton
    friend class CPtrPolicy<this_t>;
    
private:
    //prevent copy and copy assignment
    CSingleton(const this_t& rhs);
    this_t& operator=(const this_t& rhs);

private:
    static singleton_t* mP;                     //Pointer to top level object
    static CScope<CPtrPolicy<this_t> > mScopeP; //Pointer to base singleton
};

template <typename T> T* CSingleton<T>::mP = NULL;
template <typename T> CScope<CPtrPolicy<CSingleton<T> > > CSingleton<T>::mScopeP;

}

#endif //CRL_CSINGLETON_H

