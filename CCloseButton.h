////////////////////////////////////////////////////////////////////////////////
// Filename:    CCloseButton.h
// Description: This is a class that mimics a close caption bar button.
//
// Created:     2005-03-26 21:48:35
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CCLOSEBUTTON_H)
#define CRL_CCLOSEBUTTON_H

#include <CRL/CWindowImpl.h>

namespace WIN
{
    
class CCloseButton : public CWindowImpl<CCloseButton>
{
public:
    CCloseButton() : mInClick(false)
    {
    }
    ~CCloseButton()
    {
    }
    
    typedef CWindowImpl<CCloseButton> base_t;
    
    static void SetupClass(WNDCLASSEX& cls)
    {
        cls.lpszClassName = "CCloseButton";
        cls.style = CS_HREDRAW|CS_VREDRAW;
    }
        
    void Create(HWND parent, int id, const RECT& rect = RECT_DEFAULT)
    {
        base_t::Create(parent, reinterpret_cast<HMENU>(id), NULL,
            WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0,
            rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }

    virtual void OnPaint()
    {
        PAINTSTRUCT ps;
        HDC dc = ::BeginPaint(GetHWND(), &ps);
        
        RECT client = GetClientRect();
        
        unsigned long style = DFCS_CAPTIONCLOSE;

        if (mInClick && ::PtInRect(&client, GetCursorPos()))
        {
            style |= DFCS_PUSHED;
        }
        
        ::DrawFrameControl(dc, &client, DFC_CAPTION, style);
        
        ::EndPaint(GetHWND(), &ps);
    }

    virtual void OnMouseMove(unsigned long flags, short x, short y)
    {
        if (mInClick)
        {
            Invalidate();
        }
    }
    
    virtual void OnLButtonDown(unsigned long flags, short x, short y)
    {
        mInClick = true;
        ::SetCapture(GetHWND());
        Invalidate();
    }
    
    virtual void OnLButtonUp(unsigned long flags, short x, short y)
    {
        if (mInClick)
        {
            mInClick = false;
            ::ReleaseCapture();

            POINT pt = {x, y};
            RECT client = GetClientRect();
            if (::PtInRect(&client, pt))
            {
                PostCommand(BN_CLICKED);
            }

            Invalidate();
        }
    }
    
private:
    bool mInClick;
};

}

#endif //CRL_CCLOSEBUTTON_H

