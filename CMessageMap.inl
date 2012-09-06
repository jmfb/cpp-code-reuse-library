////////////////////////////////////////////////////////////////////////////////
// Filename:    CMessageMap.inl
// Description: This file implements all default message cracker bodies.
//              Each one simply calls NotHandled() in order to do default processing.
//
// Created:     2005-03-19 21:16:28
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////

inline LRESULT CMessageCrack::ProcessMsg(
    UINT msg,
    WPARAM wparam,
    LPARAM lparam,
    bool& handled)
{
    //Store the current handled member and set handled to true
    bool oldhandled = mHandled;
    mHandled = true;
        
    LRESULT result = 0;
    switch(msg)
    {
    case WM_CREATE:
        result = OnCreate(reinterpret_cast<CREATESTRUCT*>(lparam)) ? 0 : -1;
        break;
    case WM_INITDIALOG:
        result = OnInitDialog(lparam) ? TRUE : FALSE;
        break;
    case WM_CLOSE:
        OnClose();
        break;
    case WM_DESTROY:
        OnDestroy();
        break;
    case WM_COMMAND:
        OnCommand(HIWORD(wparam), LOWORD(wparam), reinterpret_cast<HWND>(lparam));
        break;
    case WM_NOTIFY:
        OnNotify(reinterpret_cast<NMHDR*>(lparam));
        break;
    case WM_SIZE:
        OnSize(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_PAINT:
        OnPaint();
        break;
    case WM_MOUSEMOVE:
        OnMouseMove(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_MOUSEWHEEL:
        OnMouseWheel(HIWORD(wparam), LOWORD(wparam), LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_LBUTTONDOWN:
        OnLButtonDown(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_LBUTTONUP:
        OnLButtonUp(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_LBUTTONDBLCLK:
        OnLButtonDblClk(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_RBUTTONDOWN:
        OnRButtonDown(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_RBUTTONUP:
        OnRButtonUp(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_RBUTTONDBLCLK:
        OnRButtonDblClk(wparam, LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_CHAR:
        OnChar(static_cast<char>(wparam), lparam);
        break;
    case WM_KEYDOWN:
        OnKeyDown(wparam, lparam);
        break;
    case WM_SYSKEYDOWN:
        OnSysKeyDown(wparam, lparam);
        break;
    case WM_VSCROLL:
        OnScroll(true, LOWORD(wparam), HIWORD(wparam), reinterpret_cast<HWND>(lparam));
        break;
    case WM_HSCROLL:
        OnScroll(false, LOWORD(wparam), HIWORD(wparam), reinterpret_cast<HWND>(lparam));
        break;
    case WM_DROPFILES:
        OnDropFiles(reinterpret_cast<HDROP>(wparam));
        break;
    case WM_ACTIVATE:
        OnActivate(LOWORD(wparam), HIWORD(wparam), reinterpret_cast<HWND>(lparam));
        break;
    case WM_SETFOCUS:
        OnSetFocus(reinterpret_cast<HWND>(wparam));
        break;
    case WM_KILLFOCUS:
        OnKillFocus(reinterpret_cast<HWND>(wparam));
        break;
    case WM_MOUSEACTIVATE:
        result = OnMouseActivate(reinterpret_cast<HWND>(wparam), LOWORD(lparam), HIWORD(lparam));
        break;
    case WM_CTLCOLORSTATIC:
        result = reinterpret_cast<LRESULT>(OnCtlColorStatic(reinterpret_cast<HWND>(lparam), reinterpret_cast<HDC>(wparam)));
        break;
    case WM_TIMER:
        OnTimer(wparam);
        break;
        
    default:
        NotHandled();
    }
        
    //Set whether the message was handled and restore old member
    handled = mHandled;
    mHandled = oldhandled;
        
    return(result);
}

inline bool CMessageCrack::OnCreate(CREATESTRUCT* cs)
{
    NotHandled();
    return(false);
}

inline bool CMessageCrack::OnInitDialog(LPARAM param)
{
    NotHandled();
    return(false);
}

inline void CMessageCrack::OnClose()
{
    NotHandled();
}

inline void CMessageCrack::OnDestroy()
{
    NotHandled();
}

inline void CMessageCrack::OnCommand(WORD code, WORD id, HWND hwnd)
{
    NotHandled();
}

inline void CMessageCrack::OnNotify(NMHDR* hdr)
{
    NotHandled();
}

inline void CMessageCrack::OnSize(unsigned long flag, unsigned short w, unsigned short h)
{
    NotHandled();
}

inline void CMessageCrack::OnPaint()
{
    NotHandled();
}

inline void CMessageCrack::OnMouseMove(unsigned long flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnMouseWheel(short delta, unsigned short flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnLButtonDown(unsigned long flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnLButtonUp(unsigned long flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnLButtonDblClk(unsigned long flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnRButtonDown(unsigned long flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnRButtonUp(unsigned long flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnRButtonDblClk(unsigned long flags, short x, short y)
{
    NotHandled();
}

inline void CMessageCrack::OnChar(char c, unsigned long flags)
{
    NotHandled();
}

inline void CMessageCrack::OnKeyDown(unsigned long key, unsigned long flags)
{
    NotHandled();
}

inline void CMessageCrack::OnSysKeyDown(unsigned long key, unsigned long flags)
{
    NotHandled();
}

inline void CMessageCrack::OnScroll(bool vertical, unsigned short code, unsigned short thumb, HWND from)
{
    NotHandled();
}

inline void CMessageCrack::OnDropFiles(HDROP drop)
{
    NotHandled();
}

inline void CMessageCrack::OnActivate(unsigned short code, unsigned short state, HWND hwnd)
{
    NotHandled();
}

inline void CMessageCrack::OnSetFocus(HWND prev)
{
    NotHandled();
}

inline void CMessageCrack::OnKillFocus(HWND next)
{
    NotHandled();
}

inline LRESULT CMessageCrack::OnMouseActivate(HWND parent, unsigned short hit, unsigned short msg)
{
    NotHandled();
    return(0);
}

inline HBRUSH CMessageCrack::OnCtlColorStatic(HWND control, HDC dc)
{
    NotHandled();
    return(NULL);
}

inline void CMessageCrack::OnTimer(UINT_PTR id)
{
    NotHandled();
}

