#pragma once


#include "xframe.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;

class XFileDataSvr: public XOffice::XXmlContentSvr
{
public:
	XFileDataSvr(void){};
	~XFileDataSvr(void){};
public:
	vector<TTreeItem> treeItems;
	virtual int GetTreeData(vector<TTreeItem>  * & pTreeData)
	{
		pTreeData = &treeItems;
		return 1;
	}
};

class XFileSheet :
	public xfWin
{
public:
	XFileSheet(void);
	~XFileSheet(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
public:
	virtual int PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame);
public:
	virtual int OnOpen();
public:
	XFileDataSvr * pXFileDataSvr;
public:
	int OnCodeFind();
	int OnCodeReplace();
public:
	int m_nSheet;
	static int MakeData(IXMLDOMNode * pNode,XFileDataSvr * pContentSvr );
};
