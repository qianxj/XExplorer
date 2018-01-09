#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
using namespace Hxsoft::XFrame;

class CDevView:public xfMdiFrame
{
public:
	struct TPara
	{
		xfWin * pWin;
		LPTSTR pStrID;
		IXMLDOMElement * pElement;
	};
	//define control
	class XDevTreeCtrl : public xcTreeCtrl
	{
	public:
		xbXml	m_xml;
		IXMLDOMNode * pNodeAgent;
	public:
		XDevTreeCtrl();
		~XDevTreeCtrl();
	public:
		XFRAME_EVENT_(DoubleClicked);
		XFRAME_EVENT_(SelectChanged);
		int GetLevel(HTREEITEM hItem);
	public:
		struct tagAgent
		{
			LPTSTR pType;
			xfNode * pNode;
		};
		vector<tagAgent> * m_pAgents;
	};
	//end define control
public:
	CDevView(void);
	~CDevView(void);
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


