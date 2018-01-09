#pragma once


#include "xframe.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;
 
class XRulerDataSvr: public XOffice::XXmlContentSvr
{
public:
	XRulerDataSvr(void){};
	~XRulerDataSvr(void){};
public:
	vector<TTreeItem> treeItems;
	virtual int GetTreeData(vector<TTreeItem>  * & pTreeData)
	{
		pTreeData = &treeItems;
		return 1;
	}
};

class RulerSheet :
	public xfWin
{
public:
	RulerSheet(void);
	~RulerSheet(void);
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
public:
	XRulerDataSvr * pXRulerDataSvr;
public:
	static int MakeData(IXMLDOMNode * pNode,XRulerDataSvr * pContentSvr );

};
