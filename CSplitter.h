////////////////////////////////////////////////////////////////////////////////
// Filename:    CSplitter.h
// Description: This file declares the splitter window class.
//
// Created:     2005-03-26 00:21:35
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CSPLITTER_H)
#define CRL_CSPLITTER_H

#include <CRL/CWindowImpl.h>
#include <CRL/MathUtility.h>

namespace WIN
{

enum SplitPaneMode
{
    SPLIT_NONE,
    SPLIT_BOTH,
    SPLIT_TOP,
    SPLIT_BOTTOM,
    SPLIT_LEFT = SPLIT_TOP,
    SPLIT_RIGHT = SPLIT_BOTTOM
};

static const int SPLIT_SIZE = 4;
static const int SPLIT_MIN = 30;

class CSplitter : public CWindowImpl<CSplitter>
{
public:
    CSplitter() : mVertical(false), mPos(0), mMode(SPLIT_NONE), mInverted(false), mInDrag(false)
    {
        mPanes[0] = NULL;
        mPanes[1] = NULL;
        ::SetRectEmpty(&mClient);
        ::SetRectEmpty(&mRects[0]);
        ::SetRectEmpty(&mRects[1]);
        ::SetRectEmpty(&mSplitRect);
    }
    ~CSplitter()
    {
    }
    
    typedef CWindowImpl<CSplitter> base_t;

    static void SetupClass(WNDCLASSEX& cls)
    {
        cls.lpszClassName = "CSplitter";
        cls.style = CS_HREDRAW|CS_VREDRAW;
        cls.hCursor = NULL;
    }
    
    void Create(
        HWND parent,
        const RECT& rect,
        bool vertical = true,
        int pos = -1,
        enum SplitPaneMode mode = SPLIT_BOTH,
        bool inverted = false)
    {
        //set custom members first
        mVertical = vertical;
        mPos = pos != -1 ? pos : vertical ?
            (rect.right - rect.left) / 2 :
            (rect.bottom - rect.top) / 2;
        mMode = mode;
        mInverted = inverted;
        
        //create the window
        base_t::Create(
            parent,
            NULL,
            NULL,
            WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
            0,
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top,
            NULL);
    }
    
    void OnSize(unsigned long flag, unsigned short w, unsigned short h)
    {
        UpdateLayout();
    }
    
    void OnPaint()
    {
        PAINTSTRUCT ps;
        HDC dc = ::BeginPaint(GetHWND(), &ps);
        
        switch(mMode)
        {
        case SPLIT_NONE:
            ::FillRect(dc, &mClient, ::GetSysColorBrush(COLOR_APPWORKSPACE));
            break;
            
        case SPLIT_BOTH:
            ::FillRect(dc, &mSplitRect, ::GetSysColorBrush(COLOR_3DFACE));
            if (!mPanes[0].IsWindow())
            {
                ::FillRect(dc, &mRects[0], ::GetSysColorBrush(COLOR_APPWORKSPACE));
            }
            if (!mPanes[1].IsWindow())
            {
                ::FillRect(dc, &mRects[1], ::GetSysColorBrush(COLOR_APPWORKSPACE));
            }
            break;
            
        case SPLIT_TOP:
            if (!mPanes[0].IsWindow())
            {
                ::FillRect(dc, &mClient, ::GetSysColorBrush(COLOR_APPWORKSPACE));
            }
            break;
            
        case SPLIT_BOTTOM:
            if (!mPanes[1].IsWindow())
            {
                ::FillRect(dc, &mClient, ::GetSysColorBrush(COLOR_APPWORKSPACE));
            }
            break;
        }
        
        ::EndPaint(GetHWND(), &ps);
    }

    void OnMouseMove(unsigned long flags, short x, short y)
    {
        POINT pt = {x, y};
        if (mInDrag)
        {
            ::SetCursor(::LoadCursor(NULL, mVertical ? IDC_SIZEWE : IDC_SIZENS));
            SetPos(
                mVertical ?
                    mInverted ?
                        mClient.right - x :
                        x :
                    mInverted ?
                        mClient.bottom - y :
                        y);
        }
        else if (mMode == SPLIT_BOTH && ::PtInRect(&mSplitRect, pt))
        {
            ::SetCursor(::LoadCursor(NULL, mVertical ? IDC_SIZEWE : IDC_SIZENS));
        }
        else
        {
            ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
        }
    }
    
    void OnLButtonDown(unsigned long flags, short x, short y)
    {
        POINT pt = {x, y};
        if (mMode == SPLIT_BOTH && ::PtInRect(&mSplitRect, pt))
        {
            mInDrag = true;
            ::SetCapture(GetHWND());
        }
    }
    
    void OnLButtonUp(unsigned long flags, short x, short y)
    {
        if (mInDrag)
        {
            mInDrag = false;
            ::ReleaseCapture();
        }
    }
    
    void SetPanes(HWND pane1, HWND pane2)
    {
        mPanes[0] = pane1;
        mPanes[1] = pane2;
        UpdateLayout();
        Invalidate();
    }
    
    void SetPaneMode(enum SplitPaneMode mode)
    {
        mMode = mode;
        UpdateLayout();
        Invalidate();
    }

    void SetPos(int pos)
    {
        if (mVertical)
        {
            mPos = MATH::Bound(SPLIT_MIN, mClient.right - mClient.left - SPLIT_MIN, pos);
        }
        else
        {
            mPos = MATH::Bound(SPLIT_MIN, mClient.bottom - mClient.top - SPLIT_MIN, pos);
        }
        UpdateLayout();
        Invalidate();
    }
    
private:

    void UpdateLayout()
    {
        //Get the client rect, and the two different pane rects
        mClient = GetClientRect();
        mRects[0] = mClient;
        mRects[1] = mClient;
        mSplitRect = mClient;
        if (mVertical)
        {
            if (mInverted)
            {
                mRects[1].left = mRects[1].right - mPos;
                mRects[0].right = mRects[1].left - SPLIT_SIZE;
            }
            else
            {
                mRects[0].right = mRects[0].left + mPos;
                mRects[1].left = mRects[0].right + SPLIT_SIZE;
            }
            mSplitRect.left = mRects[0].right;
            mSplitRect.right = mRects[1].left;
        }
        else
        {
            if (mInverted)
            {
                mRects[1].top = mRects[1].bottom - mPos;
                mRects[0].bottom = mRects[1].top - SPLIT_SIZE;
            }
            else
            {
                mRects[0].bottom = mRects[0].top + mPos;
                mRects[1].top = mRects[0].bottom + SPLIT_SIZE;
            }
            mSplitRect.top = mRects[0].bottom;
            mSplitRect.bottom = mRects[1].top;
        }
        
        //depending on the mode, move and show/hide the panes
        switch(mMode)
        {
        case SPLIT_NONE:
            mPanes[0].Show(SW_HIDE);
            mPanes[1].Show(SW_HIDE);
            break;
            
        case SPLIT_BOTH:
            mPanes[0].Move(mRects[0]);
            mPanes[0].Show(SW_SHOWNORMAL);
            mPanes[1].Move(mRects[1]);
            mPanes[1].Show(SW_SHOWNORMAL);
            break;
            
        case SPLIT_TOP:
            mPanes[1].Show(SW_HIDE);
            mPanes[0].Move(mClient);
            mPanes[0].Show(SW_SHOWNORMAL);
            break;
            
        case SPLIT_BOTTOM:
            mPanes[0].Show(SW_HIDE);
            mPanes[1].Move(mClient);
            mPanes[1].Show(SW_SHOWNORMAL);
            break;
        }
    }
    
private:
    bool mVertical;
    int mPos;
    enum SplitPaneMode mMode;
    bool mInverted;
    CWindow mPanes[2];
    RECT mClient;
    RECT mRects[2];
    RECT mSplitRect;
    bool mInDrag;
    
private:
    CSplitter(const CSplitter& rhs);
    CSplitter& operator=(const CSplitter& rhs);
};

}

#endif //CRL_CSPLITTER_H

