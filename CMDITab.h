////////////////////////////////////////////////////////////////////////////////
// Filename:    CMDITab.h
// Description: This file declares the custom MDI tabbed control.
//
// Created:     2005-03-26 20:56:28
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CMDITAB_H)
#define CRL_CMDITAB_H

#include <list>
#include <limits>
#include <CRL/CWindowImpl.h>
#include <CRL/CImageList.h>
#include <CRL/CCloseButton.h>
#include <CRL/CButtonEvents.h>
#include <CRL/CFont.h>
#include <CRL/CBrush.h>
#include <CRL/CPen.h>

namespace WIN
{

class CMDITabItem
{
public:
	bool mSelected;
	int mIcon;
	std::string mText;
	unsigned long mID;
};

class IMDITabEvents
{
public:
	virtual unsigned long OnMDITabUnselectItem(int id, unsigned long item)
	{
		return 0;
	}
	virtual void OnMDITabSelectItem(int id, unsigned long item)
	{
	}
	virtual void OnMDITabNoSelection(int id)
	{
	}
	virtual void OnMDITabCloseSelection(int id, unsigned long item)
	{
	}
};

static const int IDC_CLOSE_BUTTON = 1001;
static const COLORREF MDITAB_BACK_COLOR = RGB(247, 243, 233);

class CMDITab : public CButtonEvents<CWindowImpl<CMDITab>, IDC_CLOSE_BUTTON>
{
public:
	CMDITab() : mCallback(NULL)
	{
	}
	~CMDITab()
	{
	}

	typedef CWindowImpl<CMDITab> base_t;
	typedef std::list<CMDITabItem> list_t;

	static void SetupClass(WNDCLASSEX& cls)
	{
		cls.lpszClassName = "CMDITab";
		cls.style = CS_HREDRAW|CS_VREDRAW;
	}

	void Create(HWND parent, int id, const RECT& rect, IMDITabEvents* callback)
	{
		mCallback = callback;
		base_t::Create(parent, reinterpret_cast<HMENU>(id), NULL, WS_CHILD|WS_VISIBLE|
			WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top);
	}

	virtual bool OnCreate(CREATESTRUCT* cs)
	{
		HDC dc = ::GetDC(GetHWND());
		int height = CFont::CalcHeight(dc, 8);
		::ReleaseDC(GetHWND(), dc);

		mFont.Create("Tahoma", height);
		mBold.Create("Tahoma", height, FW_BOLD);

		mBack.Create(MDITAB_BACK_COLOR);
		mPen.Create(PS_SOLID, 1, ::GetSysColor(COLOR_GRAYTEXT));

		mImageList.CreateSmallIconList();
		mClose.Create(GetHWND(), IDC_CLOSE_BUTTON);
		return(true);
	}

	virtual void OnSize(unsigned long flag, unsigned short w, unsigned short h)
	{
		RECT client = GetClientRect();

		RECT close;
		close.top = client.top + 4;
		close.right = client.right - 4;
		close.left = close.right - 16;
		close.bottom = close.top + 16;

		mClose.Move(close);
	}

	virtual void OnPaint()
	{
		static const int HPAD = 4;
		static const int VPAD = 2;

		PAINTSTRUCT ps;
		HDC dc = ::BeginPaint(GetHWND(), &ps);

		RECT client = GetClientRect();

		//Fill the bottom few pixels with 3dface
		RECT bottom = client;
		bottom.top = bottom.bottom - 3;
		client.bottom = bottom.top;
		::FillRect(dc, &bottom, ::GetSysColorBrush(COLOR_3DFACE));

		//Draw a sunken edge around the close button box area
		RECT close = client;
		close.left = close.right - 24;
		//::DrawEdge(dc, &close, BDR_SUNKENINNER, BF_ADJUST|BF_RECT);
		//::FillRect(dc, &close, mBack);
		::FillRect(dc, &close, ::GetSysColorBrush(COLOR_3DFACE));

		//Draw a sunken edge around the tab area (not the bottom)
		RECT area = client;
		area.right -= 26;
		::DrawEdge(dc, &area, BDR_SUNKENINNER, BF_ADJUST|BF_LEFT|BF_RIGHT|BF_TOP);

		//Draw a left side pad
		RECT pad = area;
		pad.right = pad.left + 4;
		area.left = pad.right;
		::DrawEdge(dc, &pad, BDR_SUNKENINNER, BF_ADJUST|BF_BOTTOM);
		::FillRect(dc, &pad, mBack);

		//Draw the tab items
		for (list_t::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
		{
			RECT item = area;

			//Select the correct font (bold for selected item)
			::SelectObject(dc, iter->mSelected ? mBold.Get() : mFont.Get());

			//Determine the height of the text
			TEXTMETRIC tm;
			::GetTextMetrics(dc, &tm);
			int h = tm.tmHeight + VPAD + VPAD + 1;

			//Determine all of the width elements
			int w = HPAD + HPAD;
			if (iter->mIcon != 0)
			{
				w += 16 + HPAD;
			}
			if (iter->mSelected)
			{
				w += 1 + 1;
			}

			//Calculate the width of the text
			RECT rctext;
			rctext.left = 0;
			rctext.right = 0;
			rctext.top = 0;
			rctext.bottom = tm.tmHeight;
			::DrawText(dc, iter->mText.c_str(), -1, &rctext, DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
			w += rctext.right;

			//Set the rect dimensions for this tab
			item.right = item.left + w;
			item.top = item.bottom - h;

			//Remove the used area from the remaining area
			area.left = item.right;

			//Find the top area and fill it in
			RECT top = item;
			top.bottom = top.top;
			top.top = area.top;
			::FillRect(dc, &top, mBack);

			//Draw a raised edge for selected item, otherwise a bottom border
			if (iter->mSelected)
			{
				::DrawEdge(dc, &item, BDR_RAISEDOUTER, BF_ADJUST|BF_LEFT|BF_TOP|BF_RIGHT);
				::FillRect(dc, &item, ::GetSysColorBrush(COLOR_3DFACE));

				//Draw the icon if set
				if (iter->mIcon != 0)
				{
					mImageList.Draw(dc, mImageList[iter->mIcon], item.left + HPAD, item.top + VPAD);
					item.left += 16 + HPAD;
				}

				//Draw the text
				item.top += VPAD;
				item.bottom -= VPAD;
				item.right -= HPAD;
				item.left += HPAD;
				::SetBkColor(dc, ::GetSysColor(COLOR_3DFACE));
				::SetTextColor(dc, ::GetSysColor(COLOR_WINDOWTEXT));
				::DrawText(dc, iter->mText.c_str(), -1, &item, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			}
			else
			{
				::DrawEdge(dc, &item, BDR_SUNKENINNER, BF_ADJUST|BF_BOTTOM);
				::FillRect(dc, &item, mBack);

				//Draw the icon if set
				if (iter->mIcon != 0)
				{
					mImageList.Draw(dc, mImageList[iter->mIcon], item.left + HPAD, item.top + VPAD);
					item.left += 16 + HPAD;
				}

				//Draw the text
				item.top += VPAD;
				item.bottom -= VPAD;
				item.right -= HPAD;
				item.left += HPAD;
				::SetBkColor(dc, MDITAB_BACK_COLOR);
				::SetTextColor(dc, ::GetSysColor(COLOR_GRAYTEXT));
				::DrawText(dc, iter->mText.c_str(), -1, &item, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

				//Draw the vertical line to separate items
				list_t::iterator next = iter;
				++next;
				if (next == mItems.end() || !next->mSelected)
				{
					::SelectObject(dc, mPen.Get());
					::MoveToEx(dc, item.right + HPAD - 1, item.top, NULL);
					::LineTo(dc, item.right + HPAD - 1, item.bottom);
				}
			}

			//Stop drawing once no more is visible
			if (area.left >= area.right)
			{
				break;
			}
		}

		//Draw the remaining border
		pad = area;
		::DrawEdge(dc, &pad, BDR_SUNKENINNER, BF_ADJUST|BF_BOTTOM);
		::FillRect(dc, &pad, mBack);

		//Fill in the small area between the area and the close box
		RECT fill = client;
		fill.right -= 24;
		fill.left = fill.right - 2;
		::FillRect(dc, &fill, ::GetSysColorBrush(COLOR_3DFACE));

		//Done painting
		::EndPaint(GetHWND(), &ps);
	}

	virtual void OnLButtonDown(unsigned long flags, short x, short y)
	{
		static const int HPAD = 4;
		static const int VPAD = 2;

		POINT pt = {x, y};

		RECT area = GetClientRect();
		area.left += 5;
		area.right -= 27;
		area.top += 1;
		area.bottom -= 3;

		HDC dc = ::GetDC(GetHWND());

		//Draw the tab items
		for (list_t::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
		{
			RECT item = area;

			//Select the correct font (bold for selected item)
			::SelectObject(dc, iter->mSelected ? mBold.Get() : mFont.Get());

			//Determine the height of the text
			TEXTMETRIC tm;
			::GetTextMetrics(dc, &tm);
			int h = tm.tmHeight + VPAD + VPAD + 1;

			//Determine all of the width elements
			int w = HPAD + HPAD;
			if (iter->mIcon != 0)
			{
				w += 16 + HPAD;
			}
			if (iter->mSelected)
			{
				w += 1 + 1;
			}

			//Calculate the width of the text
			RECT rctext;
			rctext.left = 0;
			rctext.right = 0;
			rctext.top = 0;
			rctext.bottom = tm.tmHeight;
			::DrawText(dc, iter->mText.c_str(), -1, &rctext, DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
			w += rctext.right;

			//Set the rect dimensions for this tab
			item.right = item.left + w;
			item.top = item.bottom - h;

			//Remove the used area from the remaining area
			area.left = item.right;

			//Select this item
			if (::PtInRect(&item, pt))
			{
				Select(iter->mID);
				break;
			}

			//Stop searching once no more is visible
			if (area.left >= area.right)
			{
				break;
			}
		}

		::ReleaseDC(GetHWND(), dc);
	}

	virtual void OnButtonClicked(int id)
	{
		list_t::iterator iter = FindSelectedItem();
		if (iter != mItems.end())
		{
			RaiseOnCloseSelection(iter->mID);
		}
	}

	void AddItem(const std::string& text, int icon, unsigned long id)
	{
		CMDITabItem item;
		item.mSelected = mItems.empty();
		item.mText = text;
		item.mIcon = icon;
		item.mID = id;
		mItems.push_back(item);
		if (item.mSelected)
		{
			RaiseOnSelectItem(item.mID);
		}
		Invalidate();
	}

	void SetItemText(unsigned long id, const std::string& text)
	{
		auto iter = FindItem(id);
		if (iter == mItems.end())
			return;
		iter->mText = text;
		Invalidate();
	}

	void RemoveItem(unsigned long id)
	{
		list_t::iterator iter = FindItem(id);
		if (iter != mItems.end())
		{
			if (iter->mSelected)
			{
				auto nextId = RaiseOnUnselectItem(iter->mID);

				list_t::iterator sel = iter;
				if (sel == mItems.begin())
				{
					++sel;
				}
				else
				{
					--sel;
				}

				auto nextIter = FindItem(nextId);
				if (nextIter != iter && nextIter != mItems.end())
					sel = nextIter;

				if (sel != mItems.end())
				{
					sel->mSelected = true;
					RaiseOnSelectItem(sel->mID);
				}
				else
				{
					RaiseOnNoSelection();
				}
			}

			mItems.erase(iter);
			Invalidate();
		}
	}

	void Select(unsigned long id)
	{
		list_t::iterator iter = FindItem(id);
		if (iter != mItems.end() && !iter->mSelected)
		{
			list_t::iterator sel = FindSelectedItem();
			if (sel != mItems.end())
			{
				sel->mSelected = false;
				RaiseOnUnselectItem(sel->mID);
			}

			iter->mSelected = true;
			RaiseOnSelectItem(iter->mID);
			Invalidate();
		}
	}

	unsigned long GetSelectedItem() const
	{
		for (const auto& item: mItems)
			if (item.mSelected)
				return item.mID;
		return std::numeric_limits<unsigned long>::max();
	}

	unsigned long GetItemCount() const
	{
		return mItems.size();
	}

	void Next()
	{
		list_t::iterator sel = FindSelectedItem();
		if (sel != mItems.end())
		{
			list_t::iterator iter = sel;
			++iter;
			if (iter != mItems.end())
			{
				sel->mSelected = false;
				RaiseOnUnselectItem(sel->mID);
				iter->mSelected = true;
				RaiseOnSelectItem(iter->mID);
				Invalidate();
			}
		}
	}

	void Prev()
	{
		list_t::iterator sel = FindSelectedItem();
		if (sel != mItems.end() && sel != mItems.begin())
		{
			sel->mSelected = false;
			RaiseOnUnselectItem(sel->mID);
			--sel;
			sel->mSelected = true;
			RaiseOnSelectItem(sel->mID);
			Invalidate();
		}
	}

private:
	list_t::iterator FindItem(unsigned long id)
	{
		for (list_t::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
		{
			if (iter->mID == id)
			{
				return(iter);
			}
		}
		return(mItems.end());
	}

	list_t::iterator FindSelectedItem()
	{
		for (list_t::iterator iter = mItems.begin(); iter != mItems.end(); ++iter)
		{
			if (iter->mSelected)
			{
				return(iter);
			}
		}
		return(mItems.end());
	}


	unsigned long RaiseOnUnselectItem(unsigned long id)
	{
		if (mCallback)
		{
			return mCallback->OnMDITabUnselectItem(GetID(), id);
		}
		return 0;
	}
	void RaiseOnSelectItem(unsigned long id)
	{
		if (mCallback)
		{
			mCallback->OnMDITabSelectItem(GetID(), id);
		}
	}
	void RaiseOnNoSelection()
	{
		if (mCallback)
		{
			mCallback->OnMDITabNoSelection(GetID());
		}
	}
	void RaiseOnCloseSelection(unsigned long id)
	{
		if (mCallback)
		{
			mCallback->OnMDITabCloseSelection(GetID(), id);
		}
	}

private:
	IMDITabEvents* mCallback;
	CImageList mImageList;

	list_t mItems;

	CCloseButton mClose;

	CFont mFont;
	CFont mBold;
	CBrush mBack;
	CPen mPen;
};

}

#endif //CRL_CMDITAB_H

