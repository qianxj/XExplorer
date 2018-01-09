#pragma once


#include "xframe.h"
using namespace Hxsoft::XFrame;
 
class XModeSheet :
	public xfWin
{
public:
	XModeSheet(void);
	~XModeSheet(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame);
public:
	virtual int OnOpen();
public:
	int OnCodeFind();
	int OnCodeReplace();
public:
	int m_nSheet;
};
