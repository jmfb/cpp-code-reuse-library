////////////////////////////////////////////////////////////////////////////////
// Filename:    CWindow.h
// Description: This file declares the CWindow class.  This is a utility wrapper
//              for an HWND and all related functions.
//
// Created:     2005-03-19 19:53:09
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CWINDOW_H)
#define CRL_CWINDOW_H

#include <cstdint>
#include <limits>
#include <CRL/CWinInstance.h>
#include <CRL/ErrorUtility.h>
#include <CRL/StringUtility.h>

namespace WIN
{

static const long CreateWindowUseDefaultPosition = std::numeric_limits<long>::min();

static const RECT RECT_DEFAULT =
{
    CreateWindowUseDefaultPosition,
    CreateWindowUseDefaultPosition,
    0,
    0
};
   
class CWindow
{
public:
    CWindow(HWND hwnd = NULL) : mHwnd(hwnd)
    {
    }
    CWindow(const CWindow& rhs) : mHwnd(rhs.mHwnd)
    {
    }
    virtual ~CWindow()
    {
    }
    
    CWindow& operator=(const CWindow& rhs)
    {
        mHwnd = rhs.mHwnd;
        return(*this);
    }
    CWindow& operator=(HWND hwnd)
    {
        mHwnd = hwnd;
        return(*this);
    }
    
    bool IsWindow() const
    {
        return(::IsWindow(mHwnd) != FALSE);
    }
    bool IsVisible() const
    {
        return(::IsWindowVisible(mHwnd) != FALSE);
    }
    HWND GetHWND() const
    {
        return(mHwnd);
    }
    operator HWND() const
    {
        return(mHwnd);
    }
    
    void Attach(HWND hwnd)
    {
        mHwnd = hwnd;
    }
    HWND Detach()
    {
        HWND hwnd = mHwnd;
        mHwnd = NULL;
        return(hwnd);
    }
    
    static HWND Create(
        const char* cls,
        HWND parent,
        HMENU menu = NULL,
        const char* name = NULL,
        unsigned long style = 0,
        unsigned long exstyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int cx = CW_USEDEFAULT,
        int cy = CW_USEDEFAULT,
        void* param = NULL)
    {
        HWND hwnd = ::CreateWindowEx(
            exstyle,
            cls,
            name,
            style,
            x,
            y,
            cx,
            cy,
            parent,
            menu,
            CWinInstance::Get(),
            param);
        ERR::CheckWindowsError(hwnd == NULL, "CWindow::Create", "CreateWindowEx");
        return(hwnd);
    }
    
    void Destroy()
    {
        ::DestroyWindow(mHwnd);
    }
    
    CWindow GetParent()
    {
        return(::GetParent(mHwnd));
    }
    
    POINT GetCursorPos()
    {
        POINT pt;
        ::GetCursorPos(&pt);
        ::ScreenToClient(mHwnd, &pt);
        return(pt);
    }
    
    void Show(int cmd)
    {
        ::ShowWindow(mHwnd, cmd);
    }
    void Move(int x, int y, int cx, int cy)
    {
        ::SetWindowPos(mHwnd, NULL, x, y, cx, cy, SWP_NOZORDER|SWP_NOACTIVATE);
    }
    void Move(const RECT& rect)
    {
        Move(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    void Center()
    {
        RECT rect = GetWindowRect();
        RECT window = GetParent().GetWindowRect();
        
        int x = window.left + (window.right - window.left - rect.right + rect.left) / 2;
        int y = window.top + (window.bottom - window.top - rect.bottom + rect.top) / 2;
    
        ::SetWindowPos(mHwnd, NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);
    }
    void SetFocus()
    {
        ::SetFocus(mHwnd);
    }
    
    bool IsEnabled()
    {
        return(::IsWindowEnabled(mHwnd) != FALSE);
    }
    void Enable(bool enable = true)
    {
        ::EnableWindow(mHwnd, enable ? TRUE : FALSE);
    }
    void Disable(bool disable = true)
    {
        ::EnableWindow(mHwnd, disable ? FALSE : TRUE);
    }

    void InvalidateRect(const RECT* rect, bool erase)
    {
        ::InvalidateRect(mHwnd, rect, erase ? TRUE : FALSE);
    }
    void Invalidate(const RECT* rect = NULL)
    {
        InvalidateRect(rect, false);
    }
    void Erase(const RECT* rect = NULL)
    {
        InvalidateRect(rect, true);
    }

    void SetFont(HFONT font, bool redraw = true)
    {
        Send(WM_SETFONT, font, MAKELPARAM(redraw ? TRUE : FALSE, 0));
    }
    HFONT GetFont()
    {
        return(reinterpret_cast<HFONT>(Send(WM_GETFONT)));
    }
    
    void SetText(const std::string& str)
    {
        ::SetWindowText(mHwnd, str.c_str());
    }
    int GetTextLength()
    {
        return(::GetWindowTextLength(mHwnd));
    }
    std::string GetText()
    {
        int size = GetTextLength() + 1;
        STRING::CStringPtr buffer(new char[size]);
        ::GetWindowText(mHwnd, buffer.Get(), size);
        return(buffer.Get());
    }
    
    CWindow GetDlgItem(int id)
    {
        return(::GetDlgItem(mHwnd, id));
    }
    unsigned int GetDlgItemInt(int id, bool* success = NULL)
    {
        BOOL b = FALSE;
        unsigned int ret = ::GetDlgItemInt(mHwnd, id, &b, FALSE);
        if (success) *success = b != FALSE;
        return(ret);
    }
    int GetDlgItemSignedInt(int id, bool* success = NULL)
    {
        BOOL b = FALSE;
        int ret = ::GetDlgItemInt(mHwnd, id, &b, TRUE);
        if (success) *success = b != FALSE;
        return(ret);
    }
    std::string GetDlgItemText(int id)
    {
        return(GetDlgItem(id).GetText());
    }
    void SetDlgItemInt(int id, unsigned int value)
    {
        ::SetDlgItemInt(mHwnd, id, value, FALSE);
    }
    void SetDlgItemInt(int id, int value)
    {
        ::SetDlgItemInt(mHwnd, id, value, TRUE);
    }
    void SetDlgItemText(int id, const std::string& value)
    {
        ::SetDlgItemText(mHwnd, id, value.c_str());
    }
    bool IsDlgItemChecked(int id)
    {
        return GetDlgItem(id).Send(BM_GETCHECK) == BST_CHECKED;
    }
    void SetDlgItemChecked(int id, int status = BST_CHECKED)
    {
        GetDlgItem(id).Send(BM_SETCHECK, status);
    }
    
    void SetBigIcon(HICON icon)
    {
        Send(WM_SETICON, ICON_BIG, icon);
    }
    void SetSmallIcon(HICON icon)
    {
        Send(WM_SETICON, ICON_SMALL, icon);
    }
    HICON GetBigIcon()
    {
        return(reinterpret_cast<HICON>(Send(WM_GETICON, ICON_BIG)));
    }
    HICON GetSmallIcon()
    {
        return(reinterpret_cast<HICON>(Send(WM_GETICON, ICON_SMALL)));
    }
    
    HMENU GetMenu()
    {
        return(::GetMenu(mHwnd));
    }
    int GetID()
    {
        return(static_cast<int>(reinterpret_cast<std::intptr_t>(::GetMenu(mHwnd))));
    }

    RECT GetWindowRect()
    {
        RECT rc;
        ::GetWindowRect(mHwnd, &rc);
        return(rc);
    }
    RECT GetClientRect()
    {
        RECT rc;
        ::GetClientRect(mHwnd, &rc);
        return(rc);
    }
    
    bool IsChild(HWND hwnd)
    {
        return ::IsChild(mHwnd, hwnd) != FALSE;
    }
    bool HasFocus()
    {
        auto focus = ::GetFocus();
        return focus == mHwnd || IsChild(focus);
    }
    
    void SetTimer(UINT_PTR id, UINT elapse)
    {
        ::SetTimer(mHwnd, id, elapse, nullptr);
    }
    void KillTimer(UINT_PTR id)
    {
        ::KillTimer(mHwnd, id);
    }
    
    int MsgBox(const std::string& text, const std::string& title = "", int style = MB_OK)
    {
        return(::MessageBox(mHwnd, text.c_str(), title.c_str(), style));
    }

    //Post message functions that take any type
    void Post(UINT msg)
    {
        ::PostMessage(mHwnd, msg, 0, 0);
    }
    template <typename TW>
    void Post(UINT msg, TW wparam)
    {
        ::PostMessage(mHwnd, msg, (WPARAM)wparam, 0);
    }
    template <typename TW, typename TL>
    void Post(UINT msg, TW wparam, TL lparam)
    {
        ::PostMessage(mHwnd, msg, (WPARAM)wparam, (LPARAM)lparam);
    }
    
    //Send message functions that take any type
    LRESULT Send(UINT msg)
    {
        return(::SendMessage(mHwnd, msg, 0, 0));
    }
    template <typename TW>
    LRESULT Send(UINT msg, TW wparam)
    {
        return(::SendMessage(mHwnd, msg, (WPARAM)wparam, 0));
    }
    template <typename TW, typename TL>
    LRESULT Send(UINT msg, TW wparam, TL lparam)
    {
        return(::SendMessage(mHwnd, msg, (WPARAM)wparam, (LPARAM)lparam));
    }

    //Sending a command message to the parent window
    void PostCommand(WORD code)
    {
        GetParent().Post(WM_COMMAND, MAKEWPARAM(GetID(), code), mHwnd);
    }
    
private:
    HWND mHwnd;
};

}

#endif //CRL_CWINDOW_H

