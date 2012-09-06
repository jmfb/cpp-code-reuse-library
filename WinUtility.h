////////////////////////////////////////////////////////////////////////////////
// Filename:    WinUtility.h
// Description: This file includes all WIN namespace files into one include.
//
// Created:     2005-03-19 21:21:56
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_WINUTILITY_H)
#define CRL_WINUTILITY_H

//Core Window Framework
#include <CRL/CWinInstance.h>
#include <CRL/CWindow.h>
#include <CRL/CMessageMap.h>
#include <CRL/CWindowImpl.h>
#include <CRL/CDialogImpl.h>
#include <CRL/CHandle.h>
#include <CRL/CPipe.h>
#include <CRL/CWaitCursor.h>

//Utility Functions
namespace WIN
{
    inline HICON LoadIcon(int id, int cx, int cy)
    {
        return(reinterpret_cast<HICON>(::LoadImage(
            CWinInstance::Get(),
            MAKEINTRESOURCE(id),
            IMAGE_ICON,
            cx,
            cy,
            LR_DEFAULTCOLOR)));
    }

    inline HICON LoadLargeIcon(int id)
    {
        return(LoadIcon(id, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON)));
    }

    inline HICON LoadSmallIcon(int id)
    {
        return(LoadIcon(id, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON)));
    }

    inline std::string GetClipText()
    {
        std::string str;
        if (::OpenClipboard(0))
        {
            HGLOBAL h = ::GetClipboardData(CF_TEXT);
            if (h)
            {
                void* p = ::GlobalLock(h);
                if (p)
                {
                    str = reinterpret_cast<char*>(p);
                    ::GlobalUnlock(h);
                }
                ::CloseHandle(h);
            }
            ::CloseClipboard();
        }
        return(str);
    }

    inline void Shell(
        const std::string& command,
        const std::string& dir,
        std::string& output,
        std::string& error)
    {
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        CPipe hinput[2], houtput[2], herror[2];
        CreatePipe(hinput[0], hinput[1], &sa);
        CreatePipe(houtput[0], houtput[1], &sa);
        CreatePipe(herror[0], herror[1], &sa);

        STARTUPINFO si;
        ::memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = hinput[0].Get();
        si.hStdOutput = houtput[1].Get();
        si.hStdError = herror[1].Get();

        PROCESS_INFORMATION pi;
        ::memset(&pi, 0, sizeof(pi));

        BOOL b = ::CreateProcess(
            NULL,
            const_cast<char*>(command.c_str()),
            NULL,
            NULL,
            TRUE,
            CREATE_NO_WINDOW,
            NULL,
            dir.c_str(),
            &si,
            &pi);
        ERR::CheckWindowsError(!b, "WIN::Shell", "CreateProcess");

        ::WaitForSingleObject(pi.hProcess, INFINITE);
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);

        output = houtput[0].ReadString();
        error = herror[0].ReadString();
    }
}

//Window Wrappers
#include <CRL/CEdit.h>
#include <CRL/CMenu.h>
#include <CRL/CComboBox.h>

//Common Control Wrappers
#include <CRL/CStatusBar.h>
#include <CRL/CTreeView.h>
#include <CRL/CListView.h>
#include <CRL/CImageList.h>

//GDI Wrappers
#include <CRL/CFont.h>
#include <CRL/CBrush.h>
#include <CRL/CPen.h>

//Custom Window Controls
#include <CRL/CSplitter.h>
#include <CRL/CCloseButton.h>
#include <CRL/CMDITab.h>

//Window Event Cracker Shims
#include <CRL/CButtonEvents.h>

//Common Control Event Cracker Shims
#include <CRL/CTreeViewEvents.h>

#endif //CRL_WINUTILITY_H

