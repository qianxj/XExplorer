#include "StdAfx.h"
#include "dataschemaDlg.h"
#include "xreport.h"
#include "xcell.h"

CDataSchemaDlg::CDataSchemaDlg(void):pSchemaDataSvr(NULL),m_pDocElement(NULL)
{
}


CDataSchemaDlg::~CDataSchemaDlg(void)
{
	this->ClearAllListonsTo();
	if(m_pDocElement)m_pDocElement->Release();
}

int CDataSchemaDlg::Initial(IXMLDOMElement * pElement)
{
	m_pDocElement = pElement;
	pElement->AddRef();

	XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
	SchemaDataSvr * pContentSvr = (SchemaDataSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;

	IXMLDOMNode * pNode;
	m_pDocElement->selectSingleNode(L"schema/*[1]",&pNode);
	if(pNode)
	{
		MakeData(pNode,pContentSvr);
		pNode->Release();
	}else
	{
		IXMLDOMElementPtr pElement;
		IXMLDOMDocumentPtr pDoc;
		m_pDocElement->get_ownerDocument(&pDoc);
		pDoc->createElement(L"schema",&pElement);
		m_pDocElement->appendChild(pElement,NULL);
	}
	return 1;
}
int CDataSchemaDlg::MakeData(IXMLDOMNode * pNode,SchemaDataSvr * pContentSvr )
{
	IXMLDOMElementPtr pSchemaElement;
	pContentSvr->treeItems.clear();
	if(pNode)
	{
		pSchemaElement = pNode;
		pContentSvr->Load(pSchemaElement);
		XOffice::XXmlContentSvr::TTreeItem item;
		for(int i=0;i<pContentSvr->GetRowCount();i++)
		{
			IXMLDOMElement * pElement;
			_variant_t val;
			pElement = pContentSvr->GetRowElement(i + 1);
			pElement->getAttribute(L"type",&val);
			if(val.bstrVal && (::_tcsicmp(val.bstrVal,_T("单行集合"))==0 || ::_tcsicmp(val.bstrVal,_T("多行集合"))==0))
			{
				item.children = true;
				item.expanded = true;
				item.nImage = 22;
			}else
			{
				item.children =  false;
				item.expanded = true;
				item.nImage = 5;
			}
			item.nContentCol = 1;
			item.nContentRow = i + 1;
			item.nImageList = 0;
			item.nLevel = 1;
			if(i>0)
			{
				IXMLDOMNode * pNode;
				IXMLDOMNode * pParentNode;
				pElement->get_parentNode(&pParentNode);
				while(pParentNode)
				{
					item.nLevel++;
					pNode = pParentNode;
					pNode->get_parentNode(&pParentNode);
					pNode->Release();
				}
				item.nLevel--;
			}
			item.nSelectedImage = -1;
			pContentSvr->treeItems.push_back(item);
			pElement->Release();
		}
	}
	return 1;
}

int CDataSchemaDlg::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && (_tcsicmp(pStrID,_T("xmCancel"))==0||_tcsicmp(pStrID,_T("xmOKSchema"))==0))
	{
		::DestroyWindow(GetHWND());
		return 1;
	}else if(pStrID && (_tcsicmp(pStrID,_T("xmShemaVertColumnAdd"))==0||_tcsicmp(pStrID,_T("xmShemaHorzColumnAdd"))==0))
	{
		bool bHorz = true;
		if(_tcsicmp(pStrID,_T("xmShemaVertColumnAdd"))==0)bHorz = false;
		XOffice::XCell::XCellSheetSvr * pSheetSvr = (XOffice::XCell::XCellSheetSvr *)this->m_pParam;
		XOffice::XCell::PStrItem pItem;
		XOffice::XCell::CGridFrameSvr* pGridSvr = pSheetSvr->m_pGridFrameSvrs;

		XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
		XOffice::XReport::XReportSheetSvr * pReport = (XOffice::XReport::XReportSheetSvr *)pReportCtl->m_pSheetSvr;
		
		int nRow = 1;
		int ACol = pGridSvr->GetCol();
		int ARow = pGridSvr->GetRow();
		nRow = pReport->GetNextSelectRow(nRow + 1);
		while(nRow > 0)
		{
			IXMLDOMElement* pElement = pReport->GetRowElement(nRow);
			IXMLDOMNode* pNode = NULL;
			IXMLDOMElement* pParentEle = NULL;
			pElement->get_parentNode(&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pParentEle);

			pItem = pGridSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
			if(!pItem)
			{
				pItem = new XOffice::XCell::StrItem;
				pGridSvr->GetGridDataSvr()->m_pContentSvr->SetItemData(ACol,ARow,pItem);
			}else
			{
				if(pItem->pStrGroup)delete pItem->pStrGroup;
				if(pItem->pStrColumn)delete pItem->pStrColumn;
			}
			pItem->pStrGroup  = NULL;
			pItem->pStrColumn = NULL;

			variant_t v;
			pParentEle->getAttribute(L"colname",&v);
			if(v.bstrVal) pItem->pStrGroup = _tcsdup(v.bstrVal);
			pElement->getAttribute(L"colname",&v);
			if(v.bstrVal) pItem->pStrColumn = _tcsdup(v.bstrVal);

			TGridRect rt;
			pGridSvr->GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,rt);
			if(!bHorz)
				ARow = rt.bottom + 1;
			else
				ACol = rt.right + 1;
			pGridSvr->FCurrent.x = ACol;
			pGridSvr->FCurrent.y = ARow;
			pGridSvr->FAnchor = pGridSvr->FCurrent;
			pGridSvr->InvalidateGrid();
			nRow = pReport->GetNextSelectRow(nRow + 1);
		}
		pReport->SelectRow(0,false);
		pReport->Redraw();

		return 1;
	}
	return 0;
}

int CDataSchemaDlg::OnOpen()
{
	pSchemaDataSvr = new SchemaDataSvr();
	xbXml xml;
	xml.LoadXml(_T("<datasource xroot=\"//*\"> \
				  <col name=\"colname\" xpath=\"@colname\" desc=\"列名\" />  \
				  <col name=\"type\" xpath=\"@type\" desc=\"类型\" /> \
				  <col name=\"name\" xpath=\"@name\" desc=\"名称\" />  \
				  </datasource>"));
	IXMLDOMElement * pElement ;
	xml.m_pXmlDoc->get_documentElement(&pElement);
	pSchemaDataSvr->Initial(pElement);

	XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)this->GetControl(_T("xreport"));
	XOffice::XXmlContentSvr * pContentSvr = (XOffice::XXmlContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;
	if(pContentSvr)
	{
		delete pContentSvr;
		((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr = pSchemaDataSvr;
	}
	((XOffice::XReport::XReportSheetSvr *)pReportCtl->m_pSheetSvr)->SetSelectionMode(3);

	XOffice::XCell::XCellSheetSvr * pSheetSvr = (XOffice::XCell::XCellSheetSvr *)m_pParam;
	LPTSTR pSchema = ((XOffice::XCell::CGridDataSvr *)pSheetSvr->m_pDataSvr)->m_pSchemaSource;

	xbXml xs;
	if(pSchema)
	{
		IXAServerArg arg;
		if(pSchema[0]=='/')
		{
			arg.AddArg(L"url",pSchema);
			if(xfUriLoader::FetchXml(NULL,L"xquery:[service.url.xq]",arg.GetString(),xs)==1)
			{
				//BSTR s = xs.GetXml();
				IXMLDOMElement * pRoot ;
				xbXml x;
				xbXml::GetSchemaXml(xs.GetXml(),x);
				x.m_pXmlDoc->get_documentElement(&pRoot);
				if(pRoot)
				{
					this->Initial(pRoot);
					pRoot->Release();
				}
			}
		}
		if(xfUriLoader::FetchXml(NULL,pSchema,L"",xs)==1)
		{
			IXMLDOMElement * pRoot ;
			xbXml x;
			xbXml::GetSchemaXml(xs.GetXml(),x);
			x.m_pXmlDoc->get_documentElement(&pRoot);
			this->Initial(pRoot);
			pRoot->Release();
		}
	}else
	{
		pSchema = ((XOffice::XCell::CGridDataSvr *)pSheetSvr->m_pDataSvr)->m_pSchemaStr;
		if(pSchema)
		{
			if(xs.LoadXml(pSchema))
			{
				IXMLDOMElement * pRoot ;
				xbXml x;
				xbXml::GetSchemaXml(xs.GetXml(),x);
				x.m_pXmlDoc->get_documentElement(&pRoot);
				this->Initial(pRoot);
				pRoot->Release();
			}
		}
	}

	return 1;
}

