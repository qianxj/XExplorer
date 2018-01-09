#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xreport.h"

using namespace Hxsoft::XFrame;

class CEntityExplorer :
	public xfWin
{
public:
	CEntityExplorer(void);
	~CEntityExplorer(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int OnOpen();
};

