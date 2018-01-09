#include "StdAfx.h"
#include "..\include\XBIReportSerializeSvr.hpp"
#include "XBIReportDataSvr.hpp"
#include "XBIReportObject.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XBIReportSerializeSvr::XBIReportSerializeSvr(void)
	{
	}

	XBIReportSerializeSvr::~XBIReportSerializeSvr(void)
	{
	}

	int XBIReportSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		XBIReportDataSvr * pGridDataSvr = (XBIReportDataSvr *)pDataSvr;
		
		IXMLDOMElement* pElement;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodeListPtr	pNodeList;
		IXMLDOMNodeListPtr	pColNodeList;
		IXMLDOMNodePtr pColNode;
		IXMLDOMNodePtr pNodeEx;

		IXMLDOMElementPtr pStyleSheet;
		IXMLDOMNodeListPtr pStyleList;
		IXMLDOMNodePtr pNodeStyle;

		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xbireport")) != 0)
			{
				delete pControlName;
				return 0;
			}
			delete pControlName;
		}else
			return 0;

		long	nLenStyle=-1;
		pSheetElement->selectNodes(L"stylesheet/item",&pStyleList);
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
				pGridDataSvr->m_pStyleSvr->AddCssClass(pStrClass,bstrText);
				::SysFreeString(bstrText);
			}
			if(pStrClass)delete pStrClass;
			pElement->Release();
		}

		IXMLDOMNodeListPtr pObjList;
		IXMLDOMNodePtr pObjNode;
		IXMLDOMElementPtr pObjElement;
		long nLen;
		pSheetElement->selectNodes(L"objects/obj",&pObjList);
		pObjList->get_length(&nLen);
		_variant_t v;
		for(int i=0;i<nLen;i++)
		{
			pObjList->get_item(i,&pObjNode);
			pObjElement = pObjNode; 
			pObjElement->getAttribute(L"editer",&v);
			if(v.bstrVal && _tcsicmp(v.bstrVal,L"xbiobject")==0)
			{
				XBIReportObject * pObj = new XBIReportObject;
				pObj->Load(pObjElement);
				pGridDataSvr->m_pObjects->push_back(pObj);
			}else
			{
				pControlName = _tcsdup(v.bstrVal);
				if(pControlName && _tcsicmp(pControlName,_T("xcell"))==0 && state & 0x200000)
				{
					_variant_t v;
					pObjElement->getAttribute(L"src",&v);
					if(v.bstrVal)
					{
						xbXml x;
						if(xfUriLoader::FetchXml(pStrUrl,v.bstrVal,NULL,x)==1)
						{
							x.GetXmlDoc()->get_documentElement(&pObjElement);
						}
					}

					IXMLDOMNode * pNodeReport = NULL;
					pObjElement->selectSingleNode(L"report",&pNodeReport);
					if(pNodeReport)
					{
						delete pControlName;
						pControlName = _tcsdup(_T("xgridex"));
					}
				}

				xbObject * pObject;
				xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,pControlName,pObject);
				if(pControlName)delete pControlName;
				
				XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet *>(pObject);
				if(pSheet)
				{
					pSheet->m_Standalone = true;
					pGridDataSvr->m_pObjects->push_back(pSheet);

					_variant_t v;
					pObjElement->getAttribute(L"src",&v);
					if(v.bstrVal)
					{
						xbXml x;
						if(xfUriLoader::FetchXml(pStrUrl,v.bstrVal,NULL,x)==1)
						{
							x.GetXmlDoc()->get_documentElement(&pObjElement);
							BSTR bstr = x.GetXml();
							::SysFreeString(bstr);
						}
					}
					
					pSheet->m_pSheetSvr->LoadTemplate(pObjElement,state);
					//pSheet->m_hWnd = GetHWND();
					//pSheet->m_Standalone = false;
					//pSheet->m_pxfNode = 
				}else
					pGridDataSvr->m_pObjects->push_back(pObject);
				
				Hxsoft::XFrame::xfControl * pControl = dynamic_cast<Hxsoft::XFrame::xfControl *>(pObject);
				if(pControl) pControl->Initial(pObjElement);
			}
		}

		return 1;
	}

	int XBIReportSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		return 1;
	}

}}}}
