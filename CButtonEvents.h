////////////////////////////////////////////////////////////////////////////////
// Filename:    CButtonEvents.h
// Description: This file declares the button event cracker class.
//
// Created:     2005-03-26 22:13:31
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CBUTTONEVENTS_H)
#define CRL_CBUTTONEVENTS_H

namespace WIN
{

class IButtonEvents
{
public:
    virtual void OnButtonClicked(int id) = 0;
};

template <typename TBase, int TID>
class CButtonEvents : public TBase, virtual public IButtonEvents
{
public:
    typedef TBase base_t;

    virtual void OnCommand(WORD code, WORD id, HWND hwnd)
    {
        if (id == TID)
        {
            switch(code)
            {
            case BN_CLICKED:
                OnButtonClicked(id);
                break;
            default:
                base_t::NotHandled();
            }
        }
        else
        {
            base_t::NotHandled();
        }
    }
    
    virtual void OnButtonClicked(int id)
    {
        base_t::NotHandled();
    }
};

}

#endif //CRL_CBUTTONEVENTS_H

