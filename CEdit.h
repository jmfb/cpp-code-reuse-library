////////////////////////////////////////////////////////////////////////////////
// Filename:    CEdit.h
// Description: This file declares the edit control wrapper class.
//
// Created:     2005-03-26 03:21:18
// Author:      Jacob Buysse
//                                                              Modification Log
// When       Who               Description
// ---------- ----------------- ------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#if !defined(CRL_CEDIT_H)
#define CRL_CEDIT_H

#include <CRL/CWindow.h>

namespace WIN
{

class CEdit : public CWindow
{
public:
	CEdit(HWND hwnd = NULL) : CWindow(hwnd)
	{
	}
	~CEdit()
	{
	}

	typedef CWindow base_t;

	void Create(
		HWND parent,
		unsigned int id,
		const std::string& text,
		unsigned long style,
		unsigned long exstyle,
		const RECT& rect = RECT_DEFAULT)
	{
		Attach(base_t::Create(
			"EDIT",
			parent,
			reinterpret_cast<HMENU>(id),
			text.c_str(),
			style,
			exstyle,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL));
	}

	void ScrollCaret()
	{
		Send(EM_SCROLLCARET);
	}

	void SetSel(int start, int end, bool noscroll = false)
	{
		Send(EM_SETSEL, start, end);
		if (!noscroll)
		{
			ScrollCaret();
		}
	}

	std::pair<int,int> GetSel()
	{
		unsigned long start = 0, end = 0;
		Send(EM_GETSEL, &start, &end);
		return(std::make_pair(start, end));
	}

	int LineFromChar(int index = -1)
	{
		return(Send(EM_LINEFROMCHAR, index));
	}

	std::string GetLine(int line)
	{
		std::string str;
		int pos = LineIndex(line);
		if (pos != -1)
		{
			int size = LineLength(pos);
			if (size > 0)
			{
				char* buffer = new char[size + 1];
				*reinterpret_cast<unsigned short*>(buffer) = size + 1;
				int end = Send(EM_GETLINE, line, buffer);
				buffer[end] = 0;
				str = buffer;
				delete [] buffer;
			}
		}
		return(str);
	}

	int LineIndex(int line = -1)
	{
		return(Send(EM_LINEINDEX, line));
	}
	int LineLength(int pos = -1)
	{
		return(Send(EM_LINELENGTH, pos));
	}
	
	void ReplaceSel(const std::string& message, bool canundo = false)
	{
		Send(EM_REPLACESEL, canundo ? TRUE : FALSE, message.c_str());
	}

	void InsertText(int after, const std::string& message, bool noscroll = false, bool canundo = false)
	{
		SetSel(after, after, noscroll);
		ReplaceSel(message, canundo);
	}

	void AppendText(const std::string& message, bool noscroll = false, bool canundo = false)
	{
		InsertText(GetTextLength(), message, noscroll, canundo);
	}
};

}

#endif //CRL_CEDIT_H

