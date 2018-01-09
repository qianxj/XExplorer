#pragma once


#include "xframe.h"
using namespace Hxsoft::XFrame;
 
class xdvSheet :
	public xfWin
{
public:
	xdvSheet(void);
	~xdvSheet(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame);
public:
	virtual int OnOpen();
	bool EvtViewTypeSelection(TEvent * pEvent, LPARAM lParam);
public:
	int OnCodeFind();
	int OnCodeReplace();
public:
	int m_nSheet;
};
