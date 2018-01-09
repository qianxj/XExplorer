#pragma once

#include "xframe.h"
using namespace Hxsoft::XFrame;

class COfficeFrame :
	public xfMdiFrame
{
public:
	COfficeFrame(void);
	~COfficeFrame(void);
public:
	int m_nDocIndex;
	int AddSheet(xfWin * pWin);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
};
