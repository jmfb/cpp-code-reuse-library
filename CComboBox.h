////////////////////////////////////////////////////////////////////////////////
// Filename:    CComboBox.h
// Description: This file declares the CComboBox common control wrapper.
//
// Created:     2012-08-30 22:38:04
// Author:      Jacob Buysse
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "CWindow.h"

namespace WIN
{
	
class CComboBox : public CWindow
{
public:
	CComboBox(HWND hwnd = 0);

	CComboBox& operator=(HWND hwnd);

	static HWND Create(
		HWND parent,
		const std::string& caption = "",
		unsigned long style = 0,
		const RECT& rc = RECT_DEFAULT,
		unsigned long id = 0);

	int AddString(const std::string& str);
	int DeleteString(int index);
	int FindString(const std::string& str, int after = -1);
	int FindStringExact(const std::string& str, int after = -1);
	COMBOBOXINFO GetComboBoxInfo();
	int GetCount();
	int GetCurSel();
	RECT GetDroppedControlRect();
	bool GetDroppedState();
	int GetDroppedWidth();
	std::pair<int,int> GetEditSel();
	bool GetExtendedUI();
	int GetHorizontalExtent();
	unsigned long GetItemData(int index);
	int GetItemHeight(int index = -1);
	std::string GetLBText(int index);
	int GetTopIndex();
	int InitStorage(int items, unsigned long bytes);
	int InsertString(int index, const std::string& str);
	int LimitText(int count);
	void ResetContent();
	int SelectString(const std::string& str, int after = -1);
	int SetCurSel(int index);
	int SetDroppedWidth(int width);
	void SetEditSel(int start, int end);
	void SetExtendedUI(bool extended = true);
	void SetHorizontalExtent(int extent);
	void SetItemData(int index, unsigned long data);
	void SetItemHeight(int index, int height);
	bool SetTopIndex(int index);
	void ShowDropDown(bool show = true);
};

////////////////////////////////////////////////////////////////////////////////
//  CBN_CLOSEUP
//  CBN_DBLCLK
//  CBN_DROPDOWN
//  CBN_EDITCHANGE
//  CBN_EDITUPDATE
//  CBN_ERRSPACE
//  CBN_KILLFOCUS
//  CBN_SELCHANGE
//  CBN_SELENDCANCEL
//  CBN_SELENDOK
//  CBN_SETFOCUS
//  WM_COMPAREITEM (COMPAREITEMSTRUCT*)
//  WM_DRAWITEM (DRAWITEMSTRUCT*)
//  WM_MEASUREITEM (MEASUREITEMSTRUCT*)
////////////////////////////////////////////////////////////////////////////////

inline CComboBox::CComboBox(HWND hwnd) : CWindow(hwnd)
{
}

inline CComboBox& CComboBox::operator=(HWND hwnd)
{
	Attach(hwnd);
	return(*this);
}

inline HWND CComboBox::Create(
	HWND parent,
	const std::string& caption,
	unsigned long style,
	const RECT& rc,
	unsigned long id)
{
	return(CWindow::Create(
		WC_COMBOBOX,
		parent,
		reinterpret_cast<HMENU>(id),
		caption.c_str(),
		style,
		0,
		rc.left,
		rc.top,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr));
}

inline int CComboBox::AddString(const std::string& str)
{
	return(Send(CB_ADDSTRING, 0, str.c_str()));
}
inline int CComboBox::DeleteString(int index)
{
	return(Send(CB_DELETESTRING, index));
}
inline int CComboBox::FindString(const std::string& str, int after)
{
	return(Send(CB_FINDSTRING, after, str.c_str()));
}
inline int CComboBox::FindStringExact(const std::string& str, int after)
{
	return(Send(CB_FINDSTRINGEXACT, after, str.c_str()));
}
inline COMBOBOXINFO CComboBox::GetComboBoxInfo()
{
	COMBOBOXINFO info;
	info.cbSize = sizeof(info);
	::GetComboBoxInfo(GetHWND(), &info);
	return(info);
}
inline int CComboBox::GetCount()
{
	return(Send(CB_GETCOUNT));
}
inline int CComboBox::GetCurSel()
{
	return(Send(CB_GETCURSEL));
}
inline RECT CComboBox::GetDroppedControlRect()
{
	RECT rc;
	Send(CB_GETDROPPEDCONTROLRECT, 0, &rc);
	return(rc);
}
inline bool CComboBox::GetDroppedState()
{
	return(Send(CB_GETDROPPEDSTATE) != 0);
}
inline int CComboBox::GetDroppedWidth()
{
	return(Send(CB_GETDROPPEDWIDTH));
}
inline std::pair<int,int> CComboBox::GetEditSel()
{
	unsigned long start = 0, end = 0;
	Send(CB_GETEDITSEL, &start, &end);
	return(std::make_pair(start, end));
}
inline bool CComboBox::GetExtendedUI()
{
	return(Send(CB_GETEXTENDEDUI) != 0);
}
inline int CComboBox::GetHorizontalExtent()
{
	return(Send(CB_GETHORIZONTALEXTENT));
}
inline unsigned long CComboBox::GetItemData(int index)
{
	return(Send(CB_GETITEMDATA, index));
}
inline int CComboBox::GetItemHeight(int index)
{
	return(Send(CB_GETITEMHEIGHT, index));
}
inline std::string CComboBox::GetLBText(int index)
{
	std::string retval;
	int size = Send(CB_GETLBTEXTLEN, index);
	if (size > 0)
	{
		char* buffer = new char[size + 1];
		Send(CB_GETLBTEXT, index, buffer);
		retval = buffer;
		delete [] buffer;
	}
	return(retval);
}
inline int CComboBox::GetTopIndex()
{
	return(Send(CB_GETTOPINDEX));
}
inline int CComboBox::InitStorage(int items, unsigned long bytes)
{
	return(Send(CB_INITSTORAGE, items, bytes));
}
inline int CComboBox::InsertString(int index, const std::string& str)
{
	return(Send(CB_INSERTSTRING, index, str.c_str()));
}
inline int CComboBox::LimitText(int count)
{
	return(Send(CB_LIMITTEXT, count));
}
inline void CComboBox::ResetContent()
{
	Send(CB_RESETCONTENT);
}
inline int CComboBox::SelectString(const std::string& str, int after)
{
	return(Send(CB_SELECTSTRING, after, str.c_str()));
}
inline int CComboBox::SetCurSel(int index)
{
	return(Send(CB_SETCURSEL, index));
}
inline int CComboBox::SetDroppedWidth(int width)
{
	return(Send(CB_SETDROPPEDWIDTH, width));
}
inline void CComboBox::SetEditSel(int start, int end)
{
	Send(CB_SETEDITSEL, 0, MAKELPARAM(start, end));
}
inline void CComboBox::SetExtendedUI(bool extended)
{
	Send(CB_SETEXTENDEDUI, extended ? TRUE : FALSE);
}
inline void CComboBox::SetHorizontalExtent(int extent)
{
	Send(CB_SETHORIZONTALEXTENT, extent);
}
inline void CComboBox::SetItemData(int index, unsigned long data)
{
	Send(CB_SETITEMDATA, index, data);
}
inline void CComboBox::SetItemHeight(int index, int height)
{
	Send(CB_SETITEMHEIGHT, index, height);
}
inline bool CComboBox::SetTopIndex(int index)
{
	return(Send(CB_SETTOPINDEX, index) == 0);
}
inline void CComboBox::ShowDropDown(bool show)
{
	Send(CB_SHOWDROPDOWN, show ? TRUE : FALSE);
}

}

