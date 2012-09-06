////////////////////////////////////////////////////////////////////////////////
// Filename:    CWaitCursor.h
// Description: ...
//
// Created:     2005-04-03 22:18:36
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CWAITCURSOR_H)
#define CRL_CWAITCURSOR_H

#include <windows.h>

namespace WIN
{

class CWaitCursor
{
public:
    CWaitCursor() : mCursor(NULL)
    {
        mCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
    }
    ~CWaitCursor()
    {
        ::SetCursor(mCursor);
    }

private:
    HCURSOR mCursor;
    
private:
    CWaitCursor(const CWaitCursor& rhs);
    CWaitCursor& operator=(const CWaitCursor& rhs);
};

}

#endif //CRL_CWAITCURSOR_H

