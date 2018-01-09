#include "StdAfx.h"
#include "XFlowChartSerializeSvr.hpp"
#include "XFlowChartDataSvr.hpp"
#include "XFlowChartContentSvr.hpp"
#include "XFlowChartObject.hpp"
#include "XFlowChartLink.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowChartSerializeSvr::XFlowChartSerializeSvr(void)
	{

	}

	XFlowChartSerializeSvr::~XFlowChartSerializeSvr(void)
	{

	}
	int XFlowChartSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state )
	{
		XFlowChartDataSvr * pFlowChartDataSvr = (XFlowChartDataSvr *)pDataSvr;

		IXMLDOMElement* pElement;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMElementPtr pStyleSheet;
		IXMLDOMNodeListPtr	pNodeList;

		IXMLDOMNodeListPtr pStyleList;
		IXMLDOMNodeListPtr pDrawObjectList;
		IXMLDOMNodeListPtr pFlowObjectList;
		IXMLDOMNodeListPtr pFlowLinkList;

		IXMLDOMNodePtr pNodeStyle;
		IXMLDOMNodePtr pNodeShape;
		IXMLDOMNodePtr pNodeDrawObject;
		IXMLDOMNodePtr pNodeFlowObject;
		IXMLDOMNodePtr pNodeFlowLink;
		
		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xflowchart")) != 0)
			{
				delete pControlName;
				return 0;
			}
			delete pControlName;
		}else
			return 0;

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
				pFlowChartDataSvr->m_pStyleSvr->AddCssClass(pStrClass,bstrText);
				::SysFreeString(bstrText);
			}
			if(pStrClass)delete pStrClass;
			pElement->Release();
		}


		if(!pFlowChartDataSvr->m_pContentSvr)
		pFlowChartDataSvr-> m_pContentSvr = new XFlowChartContentSvr; 
		LPTSTR pStrClass = xbXml::GetStringAttribe(pSheetElement,_T("class"));
		if(pStrClass)
		{
			pFlowChartDataSvr->m_pContentSvr->m_nIndexStyle = pFlowChartDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
			if(pStrClass)delete pStrClass;
		}

		_variant_t	val;
		pSheetElement->selectSingleNode((_bstr_t)"shapes",&pNodeShape);
		if(pNodeShape)
		{
			pElement = xbXml::GetElement(pNodeShape);
			pFlowChartDataSvr->m_pContentSvr->m_pShapeSetItem->LoadShapeSetItem(pElement);
			pElement->Release();
		}

		long	nLenFlowObject;
		pSheetElement->selectNodes(L"flowobjects/shape",&pFlowObjectList);
		pFlowObjectList->get_length(&nLenFlowObject);
		for(int j=0;j<nLenFlowObject;j++)
		{
			pFlowObjectList->get_item(j,&pNodeFlowObject);
			pElement = xbXml::GetElement(pNodeFlowObject);

			LPTSTR pStr = NULL;
			XFlowChartObject * pObject = new XFlowChartObject();

			variant_t var;
			pElement->getAttribute(L"guid",&var);
			if(var.bstrVal)pObject->m_pGuid = _tcsdup(var.bstrVal);
			
			pObject->LoadObject(pElement);
			pElement->Release();
			pObject->m_pDataSvr = pFlowChartDataSvr;
			(*pFlowChartDataSvr-> m_pContentSvr->m_pDrawObjects).push_back(pObject);
		}
		
		long	nLenFlowLink;
		pSheetElement->selectNodes(L"flowobjects/link",&pFlowLinkList);
		pFlowLinkList->get_length(&nLenFlowLink);
		for(int j=0;j<nLenFlowLink;j++)
		{
			pFlowLinkList->get_item(j,&pNodeFlowLink);
			pElement = xbXml::GetElement(pNodeFlowLink);

			LPTSTR pStr = NULL;
			XFlowChartLink * pLink = new XFlowChartLink();
			variant_t var;
			pElement->getAttribute(L"guid",&var);
			if(var.bstrVal)pLink->m_pGuid = _tcsdup(var.bstrVal);

			pLink->LoadObject(*pFlowChartDataSvr-> m_pContentSvr->m_pDrawObjects,pElement);
			pElement->Release();
			pFlowChartDataSvr-> m_pContentSvr->m_pDrawLinks->push_back(pLink);
		}
		return 1;
	}

	int GetIndex(vector<class XFlowChartObject *> &DrawObjects,XFlowChartObject * pObject)
	{
		for(int i=0;i< (int)DrawObjects.size();i++)
		{
			if(DrawObjects[i]==pObject) return i;
		}		
		return  -1;
	}
	int XFlowChartSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xflowchart");

		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release();

		XFlowChartDataSvr * pFlowChartDataSvr = (XFlowChartDataSvr *)pDataSvr;
		
		vector<class XFlowChartObject *> &DrawObjects = *pFlowChartDataSvr->m_pContentSvr->m_pDrawObjects;
		IXMLDOMElement * pShapeElement;
		IXMLDOMElement * pShapesElement;
		pXmlDoc->createElement(L"flowobjects",&pShapesElement);

		XFlowChartObject * pObject;
		RECT rc;
		TCHAR StrName[255];
		for(int i=0;i< (int)DrawObjects.size();i++)
		{
			pObject = DrawObjects[i];
			pXmlDoc->createElement(L"shape",&pShapeElement);
			if(pObject->m_pRefUri)
			{
				TCHAR Buf[255];
				_stprintf_s(Buf,255,_T("%s:%s"),pObject->m_pRefUri,pObject->m_pRefName);
				pShapeElement->setAttribute(L"refname",(_variant_t)Buf);
			}else
				pShapeElement->setAttribute(L"refname",(_variant_t)pObject->m_pRefName);
			
			if(pObject->m_pStrID)
				pShapeElement->setAttribute(L"ID",(_variant_t)pObject->m_pStrID);

			
			_stprintf_s(StrName,255,_T("s%i"),i);
			pShapeElement->setAttribute(L"name",(_variant_t)StrName);

			rc = pObject->FRect;
			pShapeElement->setAttribute(L"startx",(_variant_t)rc.left);
			pShapeElement->setAttribute(L"starty",(_variant_t)rc.top);
			pShapeElement->setAttribute(L"endx",(_variant_t)rc.right);
			pShapeElement->setAttribute(L"endy",(_variant_t)rc.bottom);

			if(pObject->m_pGuid)pShapeElement->setAttribute(L"guid",(_variant_t)pObject->m_pGuid);

			if(pObject->m_pStrCaption)
				pShapeElement->setAttribute(L"caption",(_variant_t)pObject->m_pStrCaption);
			if(pObject->m_pDataElement)
				pShapeElement->appendChild(pObject->m_pDataElement,NULL);

			pShapesElement->appendChild(pShapeElement,NULL);
			pShapeElement->Release();
		}

		vector<XFlowChartLink *> &DrawLinks = *pFlowChartDataSvr->m_pContentSvr->m_pDrawLinks;
		IXMLDOMElement * pLinkElement;
		XFlowChartLink * pLink;
		int nIndex;
		for(int i=0;i< (int)DrawLinks.size();i++)
		{
			pLink = DrawLinks[i];
			pXmlDoc->createElement(L"link",&pLinkElement);
			if(!pLink) continue;
			if(pLink->m_pStartObject)
			{
				nIndex =  GetIndex(DrawObjects,pLink->m_pStartObject);
				if(nIndex >=0)
				{
					_stprintf_s(StrName,255,_T("s%i"),nIndex);
					pLinkElement->setAttribute(L"startshape",(_variant_t)StrName);
					pLinkElement->setAttribute(L"startshapepos",(_variant_t)pLink->m_nStartIndex);

				}
			}

			if(pLink->m_pEndObject)
			{
				nIndex =  GetIndex(DrawObjects,pLink->m_pEndObject);
				if(nIndex >=0)
				{
					_stprintf_s(StrName,255,_T("s%i"),nIndex);
					pLinkElement->setAttribute(L"endshape",(_variant_t)StrName);
					pLinkElement->setAttribute(L"endshapepos",(_variant_t)pLink->m_nEndIndex);

				}
			}

			std::wstring wx,wy;
			//set x,y
			wx=_T("");
			wy=_T("");
			TCHAR cx[40],cy[40];
			for(int j=0;j<(int)pLink->m_pPloys->size();j++)
			{
				::_ltot_s((*pLink->m_pPloys)[j].x,cx,40,10);
				::_ltot_s((*pLink->m_pPloys)[j].y,cy,40,10);
				if(j==0)
					wx = cx;
				else
					wx += _T(",") + (std::wstring)cx;
				
				if(j==0)
					wy = cy;
				else
					wy += _T(",") + (std::wstring)cy;
			}
			pLinkElement->setAttribute(L"x",(_variant_t)wx.c_str());
			pLinkElement->setAttribute(L"y",(_variant_t)wy.c_str());

			wx=_T("");
			wy=_T("");
			for(int j=0;j<(int)pLink->m_pstPoints->size();j++)
			{
				::_ltot_s((*pLink->m_pstPoints)[j].x,cx,40,10);
				::_ltot_s((*pLink->m_pstPoints)[j].y,cy,40,10);
				if(j==0)
					wx = cx;
				else
					wx += _T(",") + (std::wstring)cx;
				
				if(j==0)
					wy = cy;
				else
					wy += _T(",") + (std::wstring)cy;
			}
			pLinkElement->setAttribute(L"sx",(_variant_t)wx.c_str());
			pLinkElement->setAttribute(L"sy",(_variant_t)wy.c_str());

			//set x,y
			wx=_T("");
			wy=_T("");
			for(int j=0;j<(int)pLink->m_phtPoints->size();j++)
			{
				::_ltot_s((*pLink->m_phtPoints)[j].x,cx,40,10);
				::_ltot_s((*pLink->m_phtPoints)[j].y,cy,40,10);
				if(j==0)
					wx = cx;
				else
					wx += _T(",") + (std::wstring)cx;
				
				if(j==0)
					wy = cy;
				else
					wy += _T(",") + (std::wstring)cy;
			}
			pLinkElement->setAttribute(L"hx",(_variant_t)wx.c_str());
			pLinkElement->setAttribute(L"hy",(_variant_t)wy.c_str());
			
			if(pLink->m_pGuid)pLinkElement->setAttribute(L"guid",(_variant_t)pLink->m_pGuid);

			pShapesElement->appendChild(pLinkElement,NULL);
			pLinkElement->Release();
		}
		pElement->appendChild(pShapesElement,NULL);
		pShapesElement->Release();

		return 1;
	}
}}}}

