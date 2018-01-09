#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;

class FavoritesDlg :
	public xfWin
{
public:
	FavoritesDlg(void);
	~FavoritesDlg(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
public:
	bool EvtNotify(TEvent* pEvent,LPARAM lParam);

};
