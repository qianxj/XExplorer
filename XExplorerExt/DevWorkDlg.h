#pragma once

#include "xframe.h"
#include "xccontrol.h"
using namespace Hxsoft::XFrame;

class CDevWorkDlg :
	public xfWin
{
public:
	CDevWorkDlg(void);
	~CDevWorkDlg(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
};
