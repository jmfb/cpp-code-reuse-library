////////////////////////////////////////////////////////////////////////////////
// Filename:    CStatusBar.h
// Description: This file declares the status bar window wrapper class.
//
// Created:     2005-03-20 02:29:57
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CSTATUSBAR_H)
#define CRL_CSTATUSBAR_H

#include <CRL/CWindow.h>

namespace WIN
{

class CStatusBar : public CWindow
{
public:
    CStatusBar(HWND hwnd = NULL) : CWindow(hwnd)
    {
    }
    ~CStatusBar()
    {
    }
    
    void Create(
        HWND parent,
        int id,
        unsigned long style = 0,
        const std::string& text = "")
    {
        Attach(CWindow::Create(
            STATUSCLASSNAME,
            parent,
            reinterpret_cast<HMENU>(id),
            text.c_str(),
            WS_CHILD|WS_VISIBLE|style));
    }
    
    void UpdateSize()
    {
        Send(WM_SIZE);
    }
    
    void SetText(int part, const std::string& text, unsigned long flags = 0)
    {
        Send(SB_SETTEXT, part | flags, text.c_str());
    }
};

}

#endif //CRL_CSTATUSBAR_H

