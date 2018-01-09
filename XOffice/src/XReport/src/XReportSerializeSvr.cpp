#include "StdAfx.h"
#include "..\include\XReportSerializeSvr.hpp"
#include "xreportdatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XReportSerializeSvr::XReportSerializeSvr(void)
	{
		
	}

	XReportSerializeSvr::~XReportSerializeSvr(void)
	{
		
	}

	int ProcessDetailItem(IXMLDOMElement* pElement,XReportBandItem* pItem,XReportDataSvr * &pReportDataSvr)
	{
		_variant_t	val;
		pElement->getAttribute(L"refcol",&val);
		pItem->colid =  val;
		pElement->getAttribute(L"width",&val);
		if(val.bstrVal)
		{
			XUtil::Css::TMeasure mes;
			XUtil::Css::ProcessCssMeasure(val.bstrVal,&mes);
			pItem->m_nWidth = (int)XUtil::Css::GetPixelExtent(&mes,true);
		}
		pElement->getAttribute(L"html",&val);
		if(val.bstrVal && (_tcsicmp(val.bstrVal,L"true")==0||
			_tcsicmp(val.bstrVal,L"1")==0|| 
			_tcsicmp(val.bstrVal,L"yes")==0))
		{
			pItem->m_bHtml = true;
		}
		pItem->pStrText = xbXml::GetStringAttribe(pElement,_T("text"));

		LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
		if(pStrClass)
		{
			pItem->m_nIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
			if(pStrClass)delete pStrClass;
		}

		pStrClass	= xbXml::GetStringAttribe(pElement,_T("editor-class"));
		if(pStrClass && pReportDataSvr->m_pClassEditor)
		{
			for(int i=0;i<(int)pReportDataSvr->m_pClassEditor->size();i++)
			{
				if(_tcsicmp(pStrClass,(*pReportDataSvr->m_pClassEditor)[i])==0)
				{
					pItem->m_nIndexEditor = i;
					break;
				}
			}
			if(pStrClass)delete pStrClass;
		}

		pStrClass		= xbXml::GetStringAttribe(pElement,_T("format-class"));
		if(pStrClass && pReportDataSvr->m_pClassFormat)
		{
			for(int i=0;i<(int)pReportDataSvr->m_pClassFormat->size();i++)
			{
				if(_tcsicmp(pStrClass,(*pReportDataSvr->m_pClassFormat)[i])==0)
				{
					pItem->m_nIndexFormat = i;
					break;
				}
			}
			if(pStrClass)delete pStrClass;
		}

		return 0;
	}

	int ProcessHeaderItem(IXMLDOMElement* pElement,ColHeaderItem* pColItem,XReportDataSvr * pReportDataSvr)
	{
		_variant_t	val;
		pElement->getAttribute(L"refcol",&val);
		pColItem->colid =  val;
		pElement->getAttribute(L"width",&val);
		if(val.bstrVal)
		{
			XUtil::Css::TMeasure mes;
			XUtil::Css::ProcessCssMeasure(val.bstrVal,&mes);
			pColItem->m_nWidth = (int)XUtil::Css::GetPixelExtent(&mes,true);
		}
		pColItem->pStrText = xbXml::GetStringAttribe(pElement,_T("text"));

		LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
		if(pStrClass)
		{
			pColItem->m_nIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
			if(pStrClass)delete pStrClass;
		}

		IXMLDOMNodeListPtr	pNodeList;
		IXMLDOMNodePtr pNode;
		IXMLDOMElement* pElementEx;
		long	nLen;
		pElement->selectNodes(L"text",&pNodeList);
		pNodeList->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			pNodeList->get_item(i,&pNode);
			pElementEx = xbXml::GetElement(pNode);
			ColHeaderItem* pColItemEx = new ColHeaderItem();
			ProcessHeaderItem(pElementEx,pColItemEx,pReportDataSvr);
			pColItem->m_HeaderItems.push_back(pColItemEx);
			pElementEx->Release();
		}

		return 0;
	}

	int XReportSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		XReportDataSvr * pReportDataSvr = (XReportDataSvr *)pDataSvr;

		IXMLDOMElement* pElement;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMElementPtr pStyleSheet;
		IXMLDOMNodeListPtr	pNodeList;

		IXMLDOMNodeListPtr pStyleList;
		IXMLDOMNodeListPtr pColList;


		IXMLDOMNodePtr pNodeStyle;
		IXMLDOMNodePtr pNodeCol;

		IXMLDOMElementPtr pEditorSheet;
		IXMLDOMNodeListPtr pEditorList;
		IXMLDOMNodePtr pNodeEditor;

		IXMLDOMElementPtr pFormatSheet;
		IXMLDOMNodeListPtr pFormatList;
		IXMLDOMNodePtr pNodeFormat;

		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xreport")) != 0)
			{
				delete pControlName;
				return 0;
			}
			delete pControlName;
		}else
			return 0;

		pReportDataSvr->m_pQueryUrl = xbXml::GetStringAttribe( pSheetElement,_T("querypage"));

		long	nLenStyle=-1;
		pSheetElement->selectNodes((_bstr_t)"stylesheet/item",&pStyleList);
		pStyleList->get_length(&nLenStyle);
		for(int i=0;i<nLenStyle;i++)
		{
			LPTSTR pStrClass;
			pStyleList->get_item(i,&pNodeStyle);
			pElement = xbXml::GetElement(pNodeStyle);
			pStrClass = xbXml::GetStringAttribe(pElement,_T("name"));
			if(!pStrClass) continue;

			BSTR bstrText;
			pElement->get_text(&bstrText);
			if(bstrText)
			{
				pReportDataSvr->m_pStyleSvr->AddCssClass(pStrClass,bstrText);
				::SysFreeString(bstrText);
			}
			if(pStrClass)delete pStrClass;
			pElement->Release();
		}

		long	nLenEditor=-1;
		pSheetElement->selectNodes((_bstr_t)"editor/item",&pEditorList);
		pEditorList->get_length(&nLenEditor);
		for(int i=0;i<nLenEditor;i++)
		{
			LPTSTR pStrClass;
			pEditorList->get_item(i,&pNodeEditor);
			pElement = xbXml::GetElement(pNodeEditor);
			pStrClass = xbXml::GetStringAttribe(pElement,_T("name"));
			if(!pStrClass) continue;

			BSTR bstrText;
			pElement->get_text(&bstrText);
			if(bstrText)
			{
				if(!pReportDataSvr->m_pEditor)pReportDataSvr->m_pEditor = new vector<LPTSTR>;
				if(!pReportDataSvr->m_pClassEditor)pReportDataSvr->m_pClassEditor = new vector<LPTSTR>;
				pReportDataSvr->m_pEditor->push_back(_tcsdup(bstrText));
				pReportDataSvr->m_pClassEditor->push_back(pStrClass);
				::SysFreeString(bstrText);
			}else
				if(pStrClass)delete pStrClass;
			pElement->Release();
		}

		long	nLenFormat=-1;
		pSheetElement->selectNodes((_bstr_t)"format/item",&pFormatList);
		pFormatList->get_length(&nLenFormat);
		for(int i=0;i<nLenFormat;i++)
		{
			LPTSTR pStrClass;
			pFormatList->get_item(i,&pNodeFormat);
			pElement = xbXml::GetElement(pNodeFormat);
			pStrClass = xbXml::GetStringAttribe(pElement,_T("name"));
			if(!pStrClass) continue;

			BSTR bstrText;
			pElement->get_text(&bstrText);
			if(bstrText)
			{
				if(!pReportDataSvr->m_pFormat)pReportDataSvr->m_pFormat = new vector<LPTSTR>;
				if(!pReportDataSvr->m_pClassFormat)pReportDataSvr->m_pClassFormat = new vector<LPTSTR>;
				pReportDataSvr->m_pFormat->push_back(_tcsdup(bstrText));
				pReportDataSvr->m_pClassFormat->push_back(pStrClass);
				::SysFreeString(bstrText);
			}else
				if(pStrClass)delete pStrClass;
			pElement->Release();
		}

		if(!pReportDataSvr->m_pHeaderSvr)
			pReportDataSvr->m_pHeaderSvr = new XReportHeaderSvr; 
		pReportDataSvr->m_pHeaderSvr->m_pDataSvr = pReportDataSvr;

		LPTSTR pStrClass = xbXml::GetStringAttribe(pSheetElement,_T("class"));
		if(pStrClass)
		{
			pReportDataSvr->m_pHeaderSvr->m_nIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
			if(pStrClass)delete pStrClass;
		}

		IXMLDOMNodePtr pRsNode;
		IXMLDOMElementPtr pRsElement;
		_variant_t var;

		pSheetElement->selectSingleNode(L"rowselector",&pRsNode);
		if(pRsNode)
		{
			pRsElement = pRsNode;
			pRsElement->getAttribute(L"rowno",&var);
			pReportDataSvr->FOption |= troRowSelectorEx;
			if(var.bstrVal)
			{
				if(_tcsicmp(var.bstrVal,_T("yes"))==0 || _tcsicmp(var.bstrVal,_T("true"))==0 ||_tcsicmp(var.bstrVal,_T("1"))==0)
					pReportDataSvr->FOption |= troRowSelector;
			}
			pRsElement->getAttribute(L"width",&var);
			if(var.bstrVal)
			{
				pReportDataSvr->m_pRowSelectorSvr->m_nWidth = var;
				pReportDataSvr->m_pAixsSvr->SetColWidth(0,var);
			}

			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRowSelectorSvr->m_nIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("selection-class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRowSelectorSvr->m_nSelectionIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("cell-class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRowSelectorSvr->m_nCellIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("corner-class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRowSelectorSvr->m_nCornerIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
		}
		pSheetElement->selectSingleNode(L"rightselector",&pRsNode);
		if(pRsNode)
		{
			pRsElement = pRsNode;
			pReportDataSvr->FOption |= troRightSelector;
			pRsElement->getAttribute(L"width",&var);
			if(var.bstrVal)
				pReportDataSvr->m_pRightRowSelectorSvr->m_nWidth = var;

			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRightRowSelectorSvr->m_nIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("corner-class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRightRowSelectorSvr->m_nCornerIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("selection-class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRightRowSelectorSvr->m_nSelectionIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pStrClass = xbXml::GetStringAttribe(pRsElement,_T("cell-class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pRightRowSelectorSvr->m_nCellIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
		}

		//process datasource
		XXmlContentSvr * pContentSvr;
		if(!pReportDataSvr->m_pContentSvr)
		{
			pContentSvr = new XXmlContentSvr();
			pReportDataSvr->m_pContentSvr = pContentSvr; 
		}else
		{
			pContentSvr = dynamic_cast<XXmlContentSvr *>(pReportDataSvr->m_pContentSvr);
			if(!pContentSvr)
			{
				delete pReportDataSvr->m_pContentSvr;
				pContentSvr = new XXmlContentSvr();
				pReportDataSvr->m_pContentSvr = pContentSvr; 
			}
		}
		pSheetElement->selectSingleNode(L"datasource",&pNode);
		if(pNode)
		{
			pElement = xbXml::GetElement(pNode);
			pContentSvr->Initial(pElement);
			pContentSvr->m_pUrl = _tcsdup(pStrUrl);
			pContentSvr->Retrieve();
			pElement->Release();
		}


		//process header
		_variant_t	val;
		pSheetElement->selectSingleNode((_bstr_t)"band/headers",&pNode);
		if(pNode)
		{
			pElement = xbXml::GetElement(pNode);
			pElement->getAttribute(L"rowheight",&val);
			if(val.bstrVal)
			{
				XUtil::Css::TMeasure mes;
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&mes);
				pReportDataSvr->m_pHeaderSvr->m_nRowHeight= (int)XUtil::Css::GetPixelExtent(&mes,true);
			}
			pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pHeaderSvr->m_nIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pElement->Release();
		}

		pSheetElement->selectNodes((_bstr_t)"band/headers/*",&pColList);
		long	len=-1;
		pColList->get_length(&len);
		for(int i=0;i<len;i++)
		{
			pColList->get_item(i,&pNodeCol);
			pElement = xbXml::GetElement(pNodeCol);
			ColHeaderItem* pColItem = new ColHeaderItem();
			ProcessHeaderItem(pElement,pColItem,pReportDataSvr);
			pReportDataSvr->m_pHeaderSvr->m_HeaderItems.push_back(pColItem);
			pElement->Release();
		}

		//process detail
		if(!pReportDataSvr->m_pDetailSvr)
			pReportDataSvr->m_pDetailSvr = new XReportDetailSvr; 
		pReportDataSvr->m_pDetailSvr->m_pDataSvr = pReportDataSvr;
		pSheetElement->selectSingleNode((_bstr_t)"band/detail",&pNode);
		if(pNode)
		{
			pElement = xbXml::GetElement(pNode);
			pElement->getAttribute(L"height",&val);
			if(val.bstrVal)
			{
				XUtil::Css::TMeasure mes;
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&mes);
				pReportDataSvr->m_pDetailSvr->m_nHeight= (int)XUtil::Css::GetPixelExtent(&mes,true);
			}
			pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				pReportDataSvr->m_pDetailSvr->m_nIndexStyle = pReportDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pElement->Release();
		}

		pSheetElement->selectNodes((_bstr_t)"band/detail/*",&pColList);
		pColList->get_length(&len);
		for(int i=0;i<len;i++)
		{
			pColList->get_item(i,&pNodeCol);
			pElement = xbXml::GetElement(pNodeCol);
			XReportBandItem* pItem = new XReportBandItem();
			ProcessDetailItem(pElement,pItem,pReportDataSvr);
			pReportDataSvr->m_pDetailSvr->m_DetailItems.push_back(pItem);
			pElement->Release();
		}

		return 1;
	}
	int XReportSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xreport");

		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release();
		return 1;
	}
}}}}
