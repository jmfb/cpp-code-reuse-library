////////////////////////////////////////////////////////////////////////////////
// Filename:    CFont.h
// Description: This file declares the font wrapper class.
//
// Created:     2005-03-26 23:29:06
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CFONT_H)
#define CRL_CFONT_H

#include <CRL/CScope.h>

namespace WIN
{

class CFontPolicy
{
public:
    typedef HFONT data_t;
    static HFONT GetDefault() {return(NULL);}
    static void Destroy(HFONT h) {::DeleteObject(h);}
};

class CFont : public PTR::CScope<CFontPolicy>
{
public:
    CFont()
    {
    }
    ~CFont()
    {
    }
    
    static int CalcHeight(HDC dc, int point)
    {
        return(-::MulDiv(point, ::GetDeviceCaps(dc, LOGPIXELSY), 72));
    }
    
    void Create(
        const std::string& family,
        int height,
        unsigned long weight = FW_DONTCARE,
        bool italic = false,
        bool underline = false,
        bool strikeout = false)
    {
        Attach(::CreateFont(
            height,
            0,
            0,
            0,
            weight,
            italic ? TRUE : FALSE,
            underline ? TRUE : FALSE,
            strikeout ? TRUE : FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            family.c_str()));
    }
};

}

#endif //CRL_CFONT_H

