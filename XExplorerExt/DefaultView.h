#pragma once

#include "xframe.h"
using namespace Hxsoft::XFrame;

class CDefaultView :
	public xfWin
{
public:
	CDefaultView(void);
	~CDefaultView(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	int OnPostOpen();

public:
	WNDPROC m_PrevWndFunc;
	static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	bool m_CheckPass;
};
