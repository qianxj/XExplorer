#pragma once

#include "xframe.h"
using namespace Hxsoft::XFrame;

class CMdiFrame :
	public xfMdiFrame
{
#define XWM_CLOSE WM_USER + 0x301

public:
	CMdiFrame(void);
	~CMdiFrame(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int OnOpen();
	virtual int AddSheet(xfWin * pWin);
public:
	bool EvtWorkEnvSelected(TEvent* pEvent,LPARAM lParam);
	bool EvtClose(TEvent* pEvent,LPARAM lParam);
	bool EvtTimer(TEvent *pEvent ,LPARAM lParam);
public:
	bool CanClosed;
};
