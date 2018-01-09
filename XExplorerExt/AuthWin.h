#pragma once
#include "xframe.h"
using namespace Hxsoft::XFrame;

class CAuthWin :
	public xfWin
{
public:
	CAuthWin(void);
	~CAuthWin(void);
public:
	int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);

public:
	WNDPROC m_PrevWndFunc;
	static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	bool m_CheckPass;
	int OnOpen();
};
