////////////////////////////////////////////////////////////////////////////////
// Filename:    CMenu.h
// Description: This file declares the CMenu class and CPopupMenu class.
//
// Created:     2012-08-25 14:07:41
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>

namespace WIN
{

class CMenu
{
public:
    CMenu(HMENU menu = 0);

    CMenu& operator=(HMENU menu);

    operator HMENU();
    HMENU GetHMENU() const;
    void SetHMENU(HMENU menu);
    HMENU Detach();

    bool Append(int flags, int id, const char* str);
    int CheckItem(int item, int how = MF_BYCOMMAND|MF_CHECKED);
    bool CheckRadioItem(int first, int last, int check, int how = MF_BYCOMMAND);
    bool Create();
    bool CreatePopup();
    bool Delete(int item, int how = MF_BYCOMMAND);
    bool Destroy();
    bool EnableItem(int item, int how = MF_BYCOMMAND|MF_ENABLED);
    bool End();
    int GetDefaultItem(bool bypos = false, int flags = 0);
    MENUINFO GetMenuInfo();
    int GetItemCount();
    int GetItemID(int pos);
    MENUITEMINFO GetItemInfo(int item, bool bypos = false);
    int GetState(int item, int how = MF_BYCOMMAND);
    CMenu GetSubMenu(int pos);
    bool InsertItem(int item, bool bypos, const MENUITEMINFO& info);
    bool IsValid();
    bool Load(int id);
    bool LoadIndirect(const MENUTEMPLATE* menu);
    bool Modify(int item, int how, int id, const char* str);
    bool Remove(int item, int how = MF_BYCOMMAND);
    bool SetDefaultItem(int item, bool bypos = false);
    bool SetMenuInfo(const MENUINFO& info);
    bool SetItemBitmaps(int item, int how, HBITMAP unchecked, HBITMAP checked);
    bool SetItemInfo(int item, bool bypos, const MENUITEMINFO& info);
    int TrackPopupMenu(int flags, const POINT& pt, HWND hwnd);
   
    bool InsertCommand(int pos, const std::string& text, int id);
    bool InsertPlaceholder(int pos, const std::string& text);
    int GetItemPos(int id);

private:
    HMENU mMenu;
};

class CPopupMenu : public CMenu
{
public:
    CPopupMenu();
    ~CPopupMenu();
};


inline CMenu::CMenu(HMENU menu) : mMenu(menu)
{    
}

inline CMenu& CMenu::operator=(HMENU menu)
{
    mMenu = menu;
    return(*this);
}

inline CMenu::operator HMENU()
{
    return(mMenu);
}

inline HMENU CMenu::GetHMENU() const
{
    return(mMenu);
}

inline void CMenu::SetHMENU(HMENU menu)
{
    mMenu = menu;
}

inline HMENU CMenu::Detach()
{
    HMENU menu = mMenu;
    mMenu = 0;
    return(menu);
}

inline bool CMenu::Append(int flags, int id, const char* str)
{
    return(::AppendMenu(mMenu, flags, id, str) != 0);
}

inline int CMenu::CheckItem(int item, int how)
{
    return(::CheckMenuItem(mMenu, item, how));
}

inline bool CMenu::CheckRadioItem(int first, int last, int check, int how)
{
    return(::CheckMenuRadioItem(mMenu, first, last, check, how) != 0);
}

inline bool CMenu::Create()
{
    mMenu = ::CreateMenu();
    return(mMenu != 0);
}

inline bool CMenu::CreatePopup()
{
    mMenu = ::CreatePopupMenu();
    return(mMenu != 0);
}

inline bool CMenu::Delete(int item, int how)
{
    return(::DeleteMenu(mMenu, item, how) != 0);
}

inline bool CMenu::Destroy()
{
    bool b = ::DestroyMenu(mMenu) != 0;
    mMenu = 0;
    return(b);
}

inline bool CMenu::EnableItem(int item, int how)
{
    return(::EnableMenuItem(mMenu, item, how) != 0);
}

inline bool CMenu::End()
{
    return(::EndMenu() != 0);
}

inline int CMenu::GetDefaultItem(bool bypos, int flags)
{
    return(::GetMenuDefaultItem(mMenu, bypos ? TRUE : FALSE, flags));
}

inline MENUINFO CMenu::GetMenuInfo()
{
    MENUINFO info;
    info.cbSize = sizeof(info);
    ::GetMenuInfo(mMenu, &info);
    return(info);
}

inline int CMenu::GetItemCount()
{
    return(::GetMenuItemCount(mMenu));
}

inline int CMenu::GetItemID(int pos)
{
    return(::GetMenuItemID(mMenu, pos));
}

inline MENUITEMINFO CMenu::GetItemInfo(int item, bool bypos)
{
    MENUITEMINFO info;
    info.cbSize = sizeof(info);
    ::GetMenuItemInfo(mMenu, item, bypos ? TRUE : FALSE, &info);
    return(info);
}

inline int CMenu::GetState(int item, int how)
{
    return(::GetMenuState(mMenu, item, how));
}

inline CMenu CMenu::GetSubMenu(int pos)
{
    return(::GetSubMenu(mMenu, pos));
}

inline bool CMenu::InsertItem(int item, bool bypos, const MENUITEMINFO& info)
{
    return(::InsertMenuItem(mMenu, item, bypos ? TRUE : FALSE, &info) != 0);
}

inline bool CMenu::IsValid()
{
    return(::IsMenu(mMenu) != 0);
}

inline bool CMenu::Load(int id)
{
    mMenu = ::LoadMenu(CWinInstance::Get(), MAKEINTRESOURCE(id));
    return(mMenu != 0);
}

inline bool CMenu::LoadIndirect(const MENUTEMPLATE* menu)
{
    mMenu = ::LoadMenuIndirect(menu);
    return(mMenu != 0);
}

inline bool CMenu::Modify(int item, int how, int id, const char* str)
{
    return(::ModifyMenu(mMenu, item, how, id, str) != 0);
}

inline bool CMenu::Remove(int item, int how)
{
    return(::RemoveMenu(mMenu, item, how) != 0);
}

inline bool CMenu::SetDefaultItem(int item, bool bypos)
{
    return(::SetMenuDefaultItem(mMenu, item, bypos ? TRUE : FALSE) != 0);
}

inline bool CMenu::SetMenuInfo(const MENUINFO& info)
{
    return(::SetMenuInfo(mMenu, &info) != 0);
}

inline bool CMenu::SetItemBitmaps(int item, int how, HBITMAP unchecked, HBITMAP checked)
{
    return(::SetMenuItemBitmaps(mMenu, item, how, unchecked, checked) != 0);
}

inline bool CMenu::SetItemInfo(int item, bool bypos, const MENUITEMINFO& info)
{
    return(::SetMenuItemInfo(mMenu, item, bypos ? TRUE : FALSE, &info) != 0);
}

inline int CMenu::TrackPopupMenu(int flags, const POINT& pt, HWND hwnd)
{
    return(::TrackPopupMenu(mMenu, flags, pt.x, pt.y, 0, hwnd, 0));
}

inline bool CMenu::InsertCommand(int pos, const std::string& text, int id)
{
    MENUITEMINFO info;
    info.cbSize = sizeof(info);
    info.fType = MFT_STRING;
    info.fMask = MIIM_STRING | MIIM_ID;
    info.cch = text.size();
    info.dwTypeData = const_cast<char*>(text.c_str());
    info.wID = id;
    return(InsertItem(pos, true, info));
}

inline bool CMenu::InsertPlaceholder(int pos, const std::string& text)
{
    MENUITEMINFO info;
    info.cbSize = sizeof(info);
    info.fType = MFT_STRING;
    info.fMask = MIIM_STRING | MIIM_STATE;
    info.cch = text.size();
    info.dwTypeData = const_cast<char*>(text.c_str());
    info.fState = MFS_DISABLED | MFS_GRAYED;
    return(InsertItem(pos, true, info));
}

inline int CMenu::GetItemPos(int id)
{
    int pos = 0;
    for (; pos < GetItemCount(); ++pos)
        if (GetItemID(pos) == id)
            break;
    return(pos);
}

inline CPopupMenu::CPopupMenu()
{    
    CreatePopup();
}

inline CPopupMenu::~CPopupMenu()
{
    Destroy();
}

}
