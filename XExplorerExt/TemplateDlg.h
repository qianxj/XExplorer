#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;

class CTemplateDlg :
	public xfWin
{
public:
	CTemplateDlg(void);
	~CTemplateDlg(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
public:
	bool bEditModified;
public:
	bool EvtCommand(TEvent* pEvent,LPARAM lParam);
	bool EvtNotify(TEvent* pEvent,LPARAM lParam);
	bool EvtActive(TEvent* pEvent,LPARAM lParam);
private:
	int GetLevel(HTREEITEM hItem);
};

