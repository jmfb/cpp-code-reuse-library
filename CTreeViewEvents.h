////////////////////////////////////////////////////////////////////////////////
// Filename:    CTreeViewEvents.h
// Description: This file declares the tree view notify message cracker.
//
// Created:     2005-03-26 19:54:42
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CTREEVIEWEVENTS_H)
#define CRL_CTREEVIEWEVENTS_H

namespace WIN
{

class ITreeViewEvents
{
public:
    virtual void OnTreeViewDblClk(int id) = 0;
    virtual void OnTreeViewReturn(int id) = 0;
    virtual void OnTreeViewRClick(int id) = 0;
    virtual void OnTreeViewSelChanged(int id, NMTREEVIEW* hdr) = 0;
    virtual void OnTreeViewBeginLabelEdit(int id, NMTVDISPINFO* hdr) = 0;
    virtual void OnTreeViewEndLabelEdit(int id, NMTVDISPINFO* hdr) = 0;
    virtual void OnTreeViewBeginDrag(int id, NMTREEVIEW* hdr) = 0;
};

template <typename TBase, int TID>
class CTreeViewEvents : public TBase, virtual public ITreeViewEvents
{
public:
    typedef TBase base_t;

    virtual void OnNotify(NMHDR* hdr)
    {
        int id = hdr->idFrom;
        if (id == TID)
        {
            switch(hdr->code)
            {
            case NM_DBLCLK:
                OnTreeViewDblClk(id);
                break;
            case NM_RETURN:
                OnTreeViewReturn(id);
                break;
            case NM_RCLICK:
                OnTreeViewRClick(id);
                break;
            case TVN_SELCHANGED:
                OnTreeViewSelChanged(id, reinterpret_cast<NMTREEVIEW*>(hdr));
                break;
            case TVN_BEGINLABELEDIT:
                OnTreeViewBeginLabelEdit(id, reinterpret_cast<NMTVDISPINFO*>(hdr));
                break;
            case TVN_ENDLABELEDIT:
                OnTreeViewEndLabelEdit(id, reinterpret_cast<NMTVDISPINFO*>(hdr));
                break;
            case TVN_BEGINDRAG:
                OnTreeViewBeginDrag(id, reinterpret_cast<NMTREEVIEW*>(hdr));
                break;
            default:
                base_t::NotHandled();
                break;
            }
        }
        else
        {
            base_t::NotHandled();
        }
    }
    
    virtual void OnTreeViewDblClk(int id)
    {
        base_t::NotHandled();
    }
    virtual void OnTreeViewReturn(int id)
    {
        base_t::NotHandled();
    }
    virtual void OnTreeViewRClick(int id)
    {
        base_t::NotHandled();
    }
    virtual void OnTreeViewSelChanged(int id, NMTREEVIEW* hdr)
    {
        base_t::NotHandled();
    }
    void OnTreeViewBeginLabelEdit(int id, NMTVDISPINFO* hdr) override
    {
        base_t::NotHandled();
    }
    void OnTreeViewEndLabelEdit(int id, NMTVDISPINFO* hdr) override
    {
        base_t::NotHandled();
    }    
    void OnTreeViewBeginDrag(int id, NMTREEVIEW* hdr) override
    {
        base_t::NotHandled();
    }
};

}

#endif //CRL_CTREEVIEWEVENTS_H

