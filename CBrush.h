////////////////////////////////////////////////////////////////////////////////
// Filename:    CBrush.h
// Description: ...
//
// Created:     2005-03-27 00:16:05
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CBRUSH_H)
#define CRL_CBRUSH_H

#include <CRL/CScope.h>

namespace WIN
{
    
class CBrushPolicy
{
public:
    typedef HBRUSH data_t;
    static HBRUSH GetDefault() {return(NULL);}
    static void Destroy(HBRUSH h) {::DeleteObject(h);}
};

class CBrush : public PTR::CScope<CBrushPolicy>
{
public:
    CBrush()
    {
    }
    ~CBrush()
    {
    }
    
    operator HBRUSH() const
    {
        return(Get());
    }
    
    void Create(COLORREF color)
    {
        Attach(::CreateSolidBrush(color));
    }
};

}

#endif //CRL_CBRUSH_H

