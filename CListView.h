////////////////////////////////////////////////////////////////////////////////
// Filename:    CListView.h
// Description: ...
//
// Created:     2005-04-04 01:35:23
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CLISTVIEW_H)
#define CRL_CLISTVIEW_H

#include <CRL/CWindow.h>

namespace WIN
{

class CListView : public CWindow
{
public:
    CListView(HWND hwnd = NULL) : CWindow(hwnd)
    {
    }
    ~CListView()
    {
    }
    
    typedef CWindow base_t;
    
    void Create(
        HWND parent,
        unsigned int id,
        unsigned long style,
        unsigned long exstyle,
        const RECT& rect = RECT_DEFAULT)
    {
        Attach(base_t::Create(
            WC_LISTVIEW,
            parent,
            reinterpret_cast<HMENU>(id),
            NULL,
            style,
            exstyle,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            NULL));
    }
    
    unsigned long SetExtendedListViewStyle(unsigned long style, unsigned long mask = 0)
    {
        return(Send(LVM_SETEXTENDEDLISTVIEWSTYLE, mask, style));
    }
    
    void InsertColumn(int id, const std::string& text, int width, int format = LVCFMT_LEFT)
    {
        LVCOLUMN column;
        ::memset(&column, 0, sizeof(column));
        column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
        column.fmt = format;
        column.cx = width;
        column.pszText = const_cast<char*>(text.c_str());
        column.iSubItem = id;
        Send(LVM_INSERTCOLUMN, id, &column);
    }
    
    unsigned long GetItemCount()
    {
        return(Send(LVM_GETITEMCOUNT));
    }
    
    void InsertItem(int index, const std::string& text, unsigned long lparam = 0)
    {
        LVITEM item;
        ::memset(&item, 0, sizeof(item));
        item.mask = LVIF_TEXT | LVIF_PARAM;
        item.pszText = const_cast<char*>(text.c_str());
        item.lParam = lparam;
        item.iItem = index;
        Send(LVM_INSERTITEM, 0, &item);
    }
    
    void SetItemText(int index, int subitem, const std::string& text)
    {
        LVITEM item;
        ::memset(&item, 0, sizeof(item));
        item.mask = LVIF_TEXT;
        item.pszText = const_cast<char*>(text.c_str());
        item.iItem = index;
        item.iSubItem = subitem;
        Send(LVM_SETITEMTEXT, index, &item);
    }
    void SetItemInt(int index, int subitem, unsigned long value)
    {
        std::ostringstream out;
        out << value;
        SetItemText(index, subitem, out.str());
    }
    
    int Find(unsigned long lparam, int index = -1)
    {
        LVFINDINFO info;
        ::memset(&info, 0, sizeof(info));
        info.flags = LVFI_PARAM;
        info.lParam = lparam;
        return(Send(LVM_FINDITEM, index, &info));
    }
    
    void Delete(int index)
    {
        Send(LVM_DELETEITEM, index);
    }
    void Clear()
    {
        Send(LVM_DELETEALLITEMS);
    }
};

}

#endif //CRL_CLISTVIEW_H

