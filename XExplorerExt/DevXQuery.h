#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
using namespace Hxsoft::XFrame;

class DevXQuery :
	public xfWin
{
public:
	DevXQuery(void);
	~DevXQuery(void);

public:
	virtual bool SetEventListons();
	virtual bool UnSetEventListons();
public:
	virtual bool GetClientRect(RECT *pRect);
	virtual HWND GetHWND();
public:
	bool EvtDatasourceSelected(TEvent *pEvent ,LPARAM lParam);
public:
	IXMLDOMElement* m_pElementEx;
	int LoadContent(IXMLDOMElement * pElement);
	int UpdateContent();
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
public:
	int  DevXQuery::OnCodeFind();
	int  DevXQuery::OnCodeReplace();
};
