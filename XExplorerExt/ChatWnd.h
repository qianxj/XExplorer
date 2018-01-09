#pragma once
#include "xframe.h"
using namespace Hxsoft::XFrame;

class CChatWnd :
	public xfWin
{
public:
	CChatWnd(void);
	~CChatWnd(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
public:
	bool EvtUpdateCmdUI(TEvent* pEvent,LPARAM lParam);
};
