////////////////////////////////////////////////////////////////////////////////
// Filename:    CPen.h
// Description: This file declares the pen wrapper class.
//
// Created:     2005-03-27 00:52:54
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CPEN_H)
#define CRL_CPEN_H

#include <CRL/CScope.h>

namespace WIN
{

class CPenPolicy
{
public:
    typedef HPEN data_t;
    static HPEN GetDefault() {return(NULL);}
    static void Destroy(HPEN h) {::DeleteObject(h);}
};

class CPen : public PTR::CScope<CPenPolicy>
{
public:
    CPen()
    {
    }
    ~CPen()
    {
    }
    
    operator HPEN() const
    {
        return(Get());
    }
    
    void Create(int style, int width, COLORREF color)
    {
        Attach(::CreatePen(style, width, color));
    }
};

}

#endif //CRL_CPEN_H

