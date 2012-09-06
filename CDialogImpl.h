////////////////////////////////////////////////////////////////////////////////
// Filename:    CDialogImpl.h
// Description: ...
//
// Created:     2005-03-27 02:56:16
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CDIALOGIMPL_H)
#define CRL_CDIALOGIMPL_H

#include <CRL/CError.h>
#include <CRL/CWindow.h>
#include <CRL/CMessageMap.h>

namespace WIN
{

template <typename T, typename TMap = CMessageCrack, typename TBase = CWindow>
class CDialogImpl : public TBase, public TMap
{
public:
    typedef T dialog_t;
    typedef TBase base_t;
    typedef TMap message_t;
    typedef CDialogImpl<dialog_t, message_t, base_t> this_t;
    
    CDialogImpl()
    {
        mMsg = NULL;
        mDestroyed = false;
    }
    ~CDialogImpl()
    {
    }
    
    INT_PTR DoModal(HWND parent = ::GetActiveWindow(), LPARAM param = 0)
    {
        static const std::string FUNCTION = "CDialogImpl<T>::DoModal";
        ERR::CheckError(mNewDialog != NULL, 0, FUNCTION, "", "mNewDialog was not NULL on entry.");
        mNewDialog = dynamic_cast<dialog_t*>(this);
        ERR::CheckError(mNewDialog == NULL, 0, FUNCTION, "", "Unable to dynamic cast to dialog class.");
        return(::DialogBoxParam(
            CWinInstance::Get(),
            MAKEINTRESOURCE(T::IDD),
            parent,
            StartDialogProc,
            param));
    }
    
    void Create(HWND parent, LPARAM param = 0)
    {
        static const std::string FUNCTION = "CDialogImpl<T>::Create";
        ERR::CheckError(mNewDialog != NULL, 0, FUNCTION, "", "mNewDialog was not NULL on entry.");
        mNewDialog = dynamic_cast<dialog_t*>(this);
        ERR::CheckError(mNewDialog == NULL, 0, FUNCTION, "", "Unable to dynamic cast to dialog class.");
        HWND hwnd = ::CreateDialogParam(
            CWinInstance::Get(),
            MAKEINTRESOURCE(T::IDD),
            parent,
            StartDialogProc,
            param);
        ERR::CheckWindowsError(hwnd == NULL, FUNCTION, "CreateDialogParam returned NULL.");
        ERR::CheckError(hwnd != base_t::GetHWND(), 0, FUNCTION, "", "mHwnd did not get assigned during creation.");
        ERR::CheckError(mNewDialog != NULL, 0, FUNCTION, "", "mNewDialog did not get set to NULL.");
    }        

    void End(INT_PTR result)
    {
        ::EndDialog(base_t::GetHWND(), result);
    }
    
private:
    static INT_PTR __stdcall StartDialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        mNewDialog->Attach(hwnd);
        ::SetWindowLongPtr(hwnd, DWLP_USER, reinterpret_cast<LONG_PTR>(mNewDialog));
        mNewDialog = NULL;
        ::SetWindowLongPtr(hwnd, DWLP_DLGPROC, reinterpret_cast<LONG_PTR>(DialogProc));
        return(DialogProc(hwnd, msg, wparam, lparam));
    }
    
    static INT_PTR __stdcall DialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        //Get the dialog class associated with this HWND
        dialog_t* p = reinterpret_cast<dialog_t*>(::GetWindowLongPtr(hwnd, DWLP_USER));
        if (!p)
        {
            return(FALSE);
        }
        
        //Store the current message in the window, store the old message
        MSG newmsg = {hwnd, msg, wparam, lparam, 0, {0, 0}};
        MSG* oldmsg = p->mMsg;
        p->mMsg = &newmsg;
        
        //Have the window class process its own message
        bool handled = false;
        LRESULT result = 0;
        try
        {
            result = p->ProcessMsg(msg, wparam, lparam, handled);
        }
        catch (const ERR::CError& error)
        {
            p->MsgBox(error.Format(), "Error", MB_OK|MB_ICONERROR);
            handled = false;
        }        
        
        //Check for the WM_NCDESTROY message and mark the window destroyed
        if (msg == WM_NCDESTROY)
        {
            p->mDestroyed = true;
        }

        //Restore the old message in the window
        p->mMsg = oldmsg;
        
        //Check for top level window message after window has been destroyed
        if (p->mMsg == NULL && p->mDestroyed)
        {
            ::SetWindowLongPtr(hwnd, DWLP_USER, 0);
            p->OnFinalMessage(p->Detach());
        }

        //If handled and special, return the result, otherwise use SetWindowLong
        if (handled)
        {
            switch(msg)
            {
            case WM_COMPAREITEM:
            case WM_VKEYTOITEM:
            case WM_CHARTOITEM:
            case WM_INITDIALOG:
            case WM_QUERYDRAGICON:
            case WM_CTLCOLORMSGBOX:
            case WM_CTLCOLOREDIT:
            case WM_CTLCOLORLISTBOX:
            case WM_CTLCOLORBTN:
            case WM_CTLCOLORDLG:
            case WM_CTLCOLORSCROLLBAR:
            case WM_CTLCOLORSTATIC:
                return(result);
            }
            
            ::SetWindowLongPtr(hwnd, DWLP_MSGRESULT, result);
            return(TRUE);
        }
        return(FALSE);
    }
    
    virtual void OnFinalMessage(HWND hwnd)
    {
    }    
    
private:
    static dialog_t* mNewDialog;

    MSG* mMsg;
    bool mDestroyed;
    
private:
    CDialogImpl(const this_t& rhs);
    this_t& operator=(const this_t& rhs);
};

template <typename T, typename TMap, typename TBase> T* CDialogImpl<T,TMap,TBase>::mNewDialog = NULL;

}

#endif //CRL_CDIALOGIMPL_H

