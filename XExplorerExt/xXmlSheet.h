#pragma once


#include "xframe.h"
using namespace Hxsoft::XFrame;
 
class XXmlSheet :
	public xfWin
{
public:
	XXmlSheet(void);
	~XXmlSheet(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int OnOpen();
public:
	int OnCodeFind();
	int OnCodeReplace();
};
