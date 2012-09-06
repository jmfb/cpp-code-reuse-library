////////////////////////////////////////////////////////////////////////////////
// Filename:    CWinInstance.h
// Description: This file declare the CWinInstance singleton.
//
// Created:     2005-03-19 18:41:29
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CWININSTANCE_H)
#define CRL_CWININSTANCE_H

#include <windows.h>
#include <commctrl.h>
#include <CRL/CSingleton.h>
#include <CRL/ErrorUtility.h>

namespace WIN
{

class CWinInstance : public PTR::CSingleton<CWinInstance>
{
public:
    void SetInstance(HINSTANCE instance)
    {
        mInstance = instance;
    }
    
    HINSTANCE GetInstance() const
    {
        return(mInstance);
    }
    
    operator HINSTANCE() const
    {
        return(mInstance);
    }
    
private:
    CWinInstance()
    {
        mInstance = NULL;
        
        //Load all common controls
        INITCOMMONCONTROLSEX icc;
        icc.dwSize = sizeof(icc);
        icc.dwICC = ICC_ANIMATE_CLASS | ICC_BAR_CLASSES | ICC_COOL_CLASSES |
            ICC_DATE_CLASSES | ICC_HOTKEY_CLASS | ICC_INTERNET_CLASSES |
            ICC_LISTVIEW_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_PROGRESS_CLASS |
            ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES | ICC_UPDOWN_CLASS |
            ICC_USEREX_CLASSES;
        BOOL b = ::InitCommonControlsEx(&icc);
        ERR::CheckWindowsError(!b, "CWinInstance::CWinInstance", "InitCommonControlsEx");
    }
    ~CWinInstance()
    {
    }
    
    friend class PTR::CSingleton<CWinInstance>;
    
    HINSTANCE mInstance;
    
private:
    CWinInstance(const CWinInstance& rhs);
    CWinInstance& operator=(const CWinInstance& rhs);
};

}

#endif //CRL_CWININSTANCE_H

