#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;

class SchemaDataSvr: public XOffice::XXmlContentSvr
{
public:
	SchemaDataSvr(void){};
	~SchemaDataSvr(void){};
public:
	vector<TTreeItem> treeItems;
	virtual int GetTreeData(vector<TTreeItem>  * & pTreeData)
	{
		pTreeData = &treeItems;
		return 1;
	}
};

class CDataSchemaDlg :
	public xfWin
{
public:
	CDataSchemaDlg(void);
	~CDataSchemaDlg(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
public:
	SchemaDataSvr * pSchemaDataSvr;
public:
	IXMLDOMElement * m_pDocElement;
public:
	int Initial(IXMLDOMElement * pElement);
public:
	bool EvtActive(TEvent* pEvent,LPARAM lParam);
	static int MakeData(IXMLDOMNode * pNode,SchemaDataSvr * pContentSvr );
};

