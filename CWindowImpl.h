////////////////////////////////////////////////////////////////////////////////
// Filename:    CWindowImpl.h
// Description: This file declares the CWindowImpl class.  This class serves as
//              a base class for custom window classes.
//
// Created:     2005-03-19 20:06:01
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CWINDOWIMPL_H)
#define CRL_CWINDOWIMPL_H

#include <CRL/CError.h>
#include <CRL/CWindow.h>
#include <CRL/CMessageMap.h>

namespace WIN
{

template <typename T, typename TMap = CMessageCrack, typename TBase = CWindow>
class CWindowImpl : public TBase, public TMap
{
public:
	typedef T window_t;
	typedef TBase base_t;
	typedef TMap message_t;
	typedef CWindowImpl<window_t, message_t, base_t> this_t;

	CWindowImpl()
	{
		mMsg = NULL;
		mDestroyed = false;
	}
	~CWindowImpl()
	{
	}

	static const char* GetClassName()
	{
		if (mAtom == 0)
		{
			mClass.cbSize = sizeof(mClass);
			mClass.hInstance = CWinInstance::Get();
			mClass.lpszClassName = NULL;
			mClass.style = 0;
			mClass.lpfnWndProc = StartWindowProc;
			mClass.hIcon = NULL;
			mClass.hIconSm = NULL;
			mClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
			mClass.hbrBackground = NULL;
			mClass.lpszMenuName = NULL;
			mClass.cbClsExtra = 0;
			mClass.cbWndExtra = 0;
			T::SetupClass(mClass);
			mAtom = ::RegisterClassEx(&mClass);
			ERR::CheckWindowsError(mAtom == 0, "CWindowImpl<T>::GetClassName", "RegisterClassEx");
		}
		return(MAKEINTRESOURCE(mAtom));
	}

	void Create(
		HWND parent,
		HMENU menu = NULL,
		const char* name = NULL,
		unsigned long style = 0,
		unsigned long exstyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int cx = CW_USEDEFAULT,
		int cy = CW_USEDEFAULT,
		void* param = NULL)
	{
		static const std::string FUNCTION = "CWindowImpl<T>::Create";
		ERR::CheckError(mNewWindow != NULL, 0, FUNCTION, "", "mNewWindow was not NULL on entry.");
		mNewWindow = dynamic_cast<window_t*>(this);
		ERR::CheckError(mNewWindow == NULL, 0, FUNCTION, "", "Unable to dynamic cast to window class.");
		HWND hwnd = base_t::Create(GetClassName(), parent, menu, name, style, exstyle, x, y, cx, cy, param);
		ERR::CheckWindowsError(hwnd == NULL, FUNCTION, "CWindow::Create returned NULL.");
		ERR::CheckError(hwnd != base_t::GetHWND(), 0, FUNCTION, "", "mHwnd did not get assigned during creation.");
		ERR::CheckError(mNewWindow != NULL, 0, FUNCTION, "", "mNewWindow did not get set to NULL.");
	}

private:
	static LRESULT __stdcall StartWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		mNewWindow->Attach(hwnd);
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(mNewWindow));
		mNewWindow = NULL;
		::SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
		return(WindowProc(hwnd, msg, wparam, lparam));
	}

	static LRESULT __stdcall WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//Find the window class associated with this HWND
		window_t* p = reinterpret_cast<window_t*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (!p)
		{
			return(::DefWindowProc(hwnd, msg, wparam, lparam));
		}

		//Store the current message in the window, store the old message
		MSG newmsg = {hwnd, msg, wparam, lparam, 0, {0, 0}};
		MSG* oldmsg = p->mMsg;
		p->mMsg = &newmsg;

		//Have the window class process its own message
		bool handled = false;
		LRESULT result = 0;
		try
		{
			result = p->ProcessMsg(msg, wparam, lparam, handled);
		}
		catch (const ERR::CError& error)
		{
			p->MsgBox(error.Format(), "Error", MB_OK|MB_ICONERROR);
			handled = false;
		}
		if (!handled)
		{
			result = ::DefWindowProc(hwnd, msg, wparam, lparam);
		}

		//Check for the WM_NCDESTROY message and mark the window destroyed
		if (msg == WM_NCDESTROY)
		{
			p->mDestroyed = true;
		}

		//Restore the old message in the window
		p->mMsg = oldmsg;

		//Check for top level window message after window has been destroyed
		if (p->mMsg == NULL && p->mDestroyed)
		{
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
			p->OnFinalMessage(p->Detach());
		}

		return(result);
	}

	virtual void OnFinalMessage(HWND hwnd)
	{
	}

private:
	static window_t* mNewWindow;
	static ATOM mAtom;
	static WNDCLASSEX mClass;

	MSG* mMsg;
	bool mDestroyed;

private:
	CWindowImpl(const this_t& rhs);
	this_t& operator=(const this_t& rhs);
};

template <typename T, typename TMap, typename TBase> T* CWindowImpl<T,TMap,TBase>::mNewWindow = NULL;
template <typename T, typename TMap, typename TBase> ATOM CWindowImpl<T,TMap,TBase>::mAtom = 0;
template <typename T, typename TMap, typename TBase> WNDCLASSEX CWindowImpl<T,TMap,TBase>::mClass;

}

#endif //CRL_CWINDOWIMPL_H

