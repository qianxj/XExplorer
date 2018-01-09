#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;


class SchemaContentSvr: public XOffice::XXmlContentSvr
{
public:
	SchemaContentSvr(void){};
	~SchemaContentSvr(void){};
public:
	vector<TTreeItem> treeItems;
	virtual int GetTreeData(vector<TTreeItem>  * & pTreeData);
};


class CDevSchema :
	public xfWin
{
public:
	CDevSchema(void);
	~CDevSchema(void);
public:
	IXMLDOMElement * m_pDocElement;
public:
	int Initial(IXMLDOMElement * pElement);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
	virtual int OnOpen(xfWin *pWin);
public:
	SchemaContentSvr * pSchemaContentSvr;
public:
	bool EvtNotify(TEvent* pEvent,LPARAM lParam);
private:
	int GetLevel(xfControl * pControl,HTREEITEM hItem);
	int ReadSheet(IXMLDOMElement * pElement);
private:
	int GetSerializeXml(IXMLDOMElement *pElement);
	int ShowSerialize(IXMLDOMElement *pElement);
};
