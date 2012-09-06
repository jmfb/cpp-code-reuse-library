////////////////////////////////////////////////////////////////////////////////
// Filename:    CTreeView.h
// Description: This file declares the tree view control wrapper.
//
// Created:     2005-03-26 01:54:25
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CTREEVIEW_H)
#define CRL_CTREEVIEW_H

#include <cstring>
#include <CRL/CWindow.h>

namespace WIN
{

class CTreeView : public CWindow
{
public:
    CTreeView(HWND hwnd = NULL) : CWindow(hwnd)
    {
    }
    ~CTreeView()
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
            WC_TREEVIEW,
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

    void SetImageList(HIMAGELIST img)
    {
        Send(TVM_SETIMAGELIST, TVSIL_NORMAL, img);
    }
    
    HTREEITEM Insert(
        HTREEITEM parent,
        HTREEITEM after,
        const std::string& text,
        int image,
        LPARAM param)
    {
        TVINSERTSTRUCT tvi;
        tvi.hParent = parent;
        tvi.hInsertAfter = after;
        tvi.itemex.mask = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
        tvi.itemex.pszText = const_cast<char*>(text.c_str());
        tvi.itemex.iImage = image;
        tvi.itemex.iSelectedImage = image;
        tvi.itemex.lParam = param;
        return(reinterpret_cast<HTREEITEM>(Send(TVM_INSERTITEM, 0, &tvi)));
    }
    
    void Delete(HTREEITEM item)
    {
        Send(TVM_DELETEITEM, 0, item);
    }
    void Clear()
    {
        Delete(TVI_ROOT);
    }
    
    void ExpandItem(HTREEITEM item, unsigned long flag)
    {
        Send(TVM_EXPAND, flag, item);
    }
    void Expand(HTREEITEM item)
    {
        ExpandItem(item, TVE_EXPAND);
    }
    void Collapse(HTREEITEM item)
    {
        ExpandItem(item, TVE_COLLAPSE);
    }
    void Toggle(HTREEITEM item)
    {
        ExpandItem(item, TVE_TOGGLE);
    }
    
    void SelectItem(HTREEITEM item, unsigned long flag)
    {
        Send(TVM_SELECTITEM, flag, item);
    }
    void Select(HTREEITEM item)
    {
        SelectItem(item, TVGN_CARET);
    }
    void SetFirstVisible(HTREEITEM item)
    {
        SelectItem(item, TVGN_FIRSTVISIBLE);
    }
    void SetDropHilite(HTREEITEM item)
    {
        SelectItem(item, TVGN_DROPHILITE);
    }
    void EnsureVisible(HTREEITEM item)
    {
        Send(TVM_ENSUREVISIBLE, 0, item);
    }
    
    HTREEITEM HitTest(POINT pt, unsigned long* flags = NULL)
    {
        TV_HITTESTINFO info;
        info.pt = pt;
        HTREEITEM item = reinterpret_cast<HTREEITEM>(Send(TVM_HITTEST, 0, &info));
        if (flags) *flags = info.flags;
        return(item);
    }
    
    HWND EditLabel(HTREEITEM item)
    {
        return(reinterpret_cast<HWND>(Send(TVM_EDITLABEL, 0, item)));
    }
    void EndLabelEdit(bool cancel)
    {
        Send(TVM_ENDEDITLABELNOW, cancel ? TRUE : FALSE);
    }
    
    HTREEITEM GetNextItem(HTREEITEM item, unsigned long flag)
    {
        return(reinterpret_cast<HTREEITEM>(Send(TVM_GETNEXTITEM, flag, item)));
    }
    HTREEITEM GetSelection()
    {
        return(GetNextItem(NULL, TVGN_CARET));
    }
    HTREEITEM GetChild(HTREEITEM parent)
    {
        return(GetNextItem(parent, TVGN_CHILD));
    }
    HTREEITEM GetParent(HTREEITEM child)
    {
        return(GetNextItem(child, TVGN_PARENT));
    }
    HTREEITEM GetNext(HTREEITEM sibling)
    {
        return(GetNextItem(sibling, TVGN_NEXT));
    }
    HTREEITEM GetPrev(HTREEITEM sibling)
    {
        return(GetNextItem(sibling, TVGN_PREVIOUS));
    }
    HTREEITEM GetRoot()
    {
        return(GetNextItem(NULL, TVGN_ROOT));
    }
    
    LPARAM GetParam(HTREEITEM item)
    {
        TVITEM tv;
        tv.hItem = item;
        tv.mask = TVIF_PARAM;
        Send(TVM_GETITEM, 0, &tv);
        return(tv.lParam);
    }
    std::string GetText(HTREEITEM item)
    {
        TVITEM tv;
        tv.hItem = item;
        tv.mask = TVIF_TEXT;
        for (unsigned int length = 256; ; length *= 2)
        {
            STRING::CStringPtr buffer(new char[length]);
            tv.pszText = buffer.Get();
            tv.cchTextMax = length;
            LRESULT res = Send(TVM_GETITEM, 0, &tv);
            if (res == FALSE) return("");
            if (::strlen(buffer.Get()) < length - 1) return(buffer.Get());
        }
    }
    
    void SetText(HTREEITEM item, const std::string& value)
    {
        TVITEM tv = {0};
        tv.hItem = item;
        tv.mask = TVIF_TEXT;
        STRING::CStringPtr buffer(new char[value.size() + 1]);
        std::strcpy(buffer.Get(), value.c_str());
        tv.pszText = buffer.Get();
        tv.cchTextMax = value.size();
        Send(TVM_SETITEM, 0, &tv);
    }
    
    template <typename TEnumeratorCallback>
    bool Enumerate(TEnumeratorCallback callback)
    {
        return Enumerate(callback, GetRoot());
    }
    
    template <typename TEnumeratorCallback>
    bool Enumerate(TEnumeratorCallback callback, HTREEITEM item)
    {
        if (!callback(item))
            return false;
        for (auto child = GetChild(item); child != nullptr; child = GetNext(child))
            if (!Enumerate(callback, child))
                return false;
        return true;
    }
    
    void SetBkColor(COLORREF color)
    {
        Send(TVM_SETBKCOLOR, 0, color);
    }
    void SetTextColor(COLORREF color)
    {
        Send(TVM_SETTEXTCOLOR, 0, color);
    }
    void SetLineColor(COLORREF color)
    {
        Send(TVM_SETLINECOLOR, 0, color);
    }
};

}

#endif //CRL_CTREEVIEW_H

