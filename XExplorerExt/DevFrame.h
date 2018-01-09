#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
using namespace Hxsoft::XFrame;

class DevFrame:public xfMdiFrame
{
public:
	DevFrame(void);
	~DevFrame(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int AddSheet(xfWin * pWin);
	virtual int SelectSheet(int nIndex);
	virtual int PreSelectSheet(int nIndex);
public:
	int OnOpen();
	int OnClose();
};


