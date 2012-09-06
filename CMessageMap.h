////////////////////////////////////////////////////////////////////////////////
// Filename:    CMessageMap.h
// Description: This file declars the CMessageMap interface and CMessageCrack
//              utility class.
//
// Created:     2005-03-19 20:57:04
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CMESSAGEMAP_H)
#define CRL_CMESSAGEMAP_H

#include <windows.h>
#include <shellapi.h>

namespace WIN
{

class CMessageMap
{
public:
    virtual ~CMessageMap()
    {
    }
    
    virtual LRESULT ProcessMsg(
        UINT msg,
        WPARAM wparam,
        LPARAM lparam,
        bool& handled) = 0;
};

class CMessageCrack : public CMessageMap
{
public:
    CMessageCrack() : mHandled(false)
    {
    }
    ~CMessageCrack()
    {
    }
    
    virtual LRESULT ProcessMsg(
        UINT msg,
        WPARAM wparam,
        LPARAM lparam,
        bool& handled);
    
//Overridable message crackers
public:
    virtual bool OnCreate(CREATESTRUCT* cs);
    virtual bool OnInitDialog(LPARAM param);
    virtual void OnClose();
    virtual void OnDestroy();
    virtual void OnCommand(WORD code, WORD id, HWND hwnd);
    virtual void OnNotify(NMHDR* hdr);
    virtual void OnSize(unsigned long flag, unsigned short w, unsigned short h);
    virtual void OnPaint();
    virtual void OnMouseMove(unsigned long flags, short x, short y);
    virtual void OnMouseWheel(short delta, unsigned short flags, short x, short y);
    virtual void OnLButtonDown(unsigned long flags, short x, short y);
    virtual void OnLButtonUp(unsigned long flags, short x, short y);
    virtual void OnLButtonDblClk(unsigned long flags, short x, short y);
    virtual void OnRButtonDown(unsigned long flags, short x, short y);
    virtual void OnRButtonUp(unsigned long flags, short x, short y);
    virtual void OnRButtonDblClk(unsigned long flags, short x, short y);
    virtual void OnChar(char c, unsigned long flags);
    virtual void OnKeyDown(unsigned long key, unsigned long flags);
    virtual void OnSysKeyDown(unsigned long key, unsigned long flags);
    virtual void OnScroll(bool vertical, unsigned short code, unsigned short thumb, HWND from);
    virtual void OnDropFiles(HDROP drop);
    virtual void OnActivate(unsigned short code, unsigned short state, HWND hwnd);
    virtual void OnSetFocus(HWND prev);
    virtual void OnKillFocus(HWND next);
    virtual LRESULT OnMouseActivate(HWND parent, unsigned short hit, unsigned short msg);
    virtual HBRUSH OnCtlColorStatic(HWND control, HDC dc);
    virtual void OnTimer(UINT_PTR id);
    
protected:
    //Call this function to continue with normal message processing from
    //within a cracked message.  Always call at the end of the function.
    void NotHandled()
    {
        mHandled = false;
    }
    
private:
    bool mHandled;
};

#include <CRL/CMessageMap.inl>

}

#endif //CRL_CMESSAGEMAP_H

