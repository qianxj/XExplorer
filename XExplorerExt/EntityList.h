#pragma once

#include "xframe.h"
#include "xccontrol.h"

using namespace Hxsoft::XFrame;

class EXTEXPLORER_API CEntityList :
	public xfWin
{
public:
	CEntityList(void);
	~CEntityList(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int OnOpen();
public:
	virtual bool EvtSetFocus(TEvent * pEvent, LPARAM lParam);
public:
	LPTSTR m_pEntityName;
	LPTSTR m_pStrAgent;
public:
	int SetAgent();
	int Retrieve(LPTSTR  pXmlArgs);
};

