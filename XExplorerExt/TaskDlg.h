#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;

class CTaskDlg :
	public xfWin
{
public:
	CTaskDlg(void);
	~CTaskDlg(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
};
