#include "StdAfx.h"
#include "..\include\XDFSerializeSvr.hpp"
#include "xdfdatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	XDFSerializeSvr::XDFSerializeSvr(void)
	{
	}

	XDFSerializeSvr::~XDFSerializeSvr(void)
	{
	}
	
	int ReadDrawObject(IXMLDOMElement * pElement, XDFDrawObject * pObject)
	{
		BSTR bstrTag;
		_variant_t	val;
		pElement->get_tagName(&bstrTag);
		if(bstrTag)
		{
			pObject->eObject = FindDrawObject(bstrTag);
			::SysFreeString(bstrTag);
		}

		pElement->getAttribute(L"startx",&val);
		if(val.bstrVal)
		{
			if(val.bstrVal[0]=='.'||val.bstrVal[0]=='@')
			{
				pObject->strStartX = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			else
			{
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&pObject->startx);
			}
		}
		pElement->getAttribute(L"starty",&val);
		if(val.bstrVal)
		{
			if(val.bstrVal[0]=='.'||val.bstrVal[0]=='@')
			{
				pObject->strStartY = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			else
			{
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&pObject->starty);
			}
		}
		pElement->getAttribute(L"endx",&val);
		if(val.bstrVal)
		{
			if(val.bstrVal[0]=='.'||val.bstrVal[0]=='@')
			{
				pObject->strEndX = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			else
			{
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&pObject->endx);
			}
		}
		pElement->getAttribute(L"endy",&val);
		if(val.bstrVal)
		{
			if(val.bstrVal[0]=='.'||val.bstrVal[0]=='@')
			{
				pObject->strEndY = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			else
			{
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&pObject->endy);
			}
		}
		pElement->getAttribute(L"width",&val);
		if(val.bstrVal)
		{
			if(val.bstrVal[0]=='.'||val.bstrVal[0]=='@')
			{
				pObject->strWidth = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			else
			{
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&pObject->width);
			}
		}

		pElement->getAttribute(L"height",&val);
		if(val.bstrVal)
		{
			if(val.bstrVal[0]=='.'||val.bstrVal[0]=='@')
			{
				pObject->strHeight = XUtil::tcsdupFromBstr(val.bstrVal);
			}
			else
			{
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&pObject->height);
			}
		}

		pElement->getAttribute(L"src",&val);
		if(val.bstrVal)
		{
			pObject->pStrUrl = XUtil::tcsdupFromBstr(val.bstrVal);
		}

		pElement->getAttribute(L"name",&val);
		if(val.bstrVal)
		{
			pObject->name = XUtil::tcsdupFromBstr(val.bstrVal);
		}else
			pObject->name = NULL;

		pElement->getAttribute(L"refname",&val);
		if(val.bstrVal)
		{
			pObject->refname = XUtil::tcsdupFromBstr(val.bstrVal);
		}else
			pObject->refname = NULL;

		pElement->getAttribute(L"id",&val);
		pObject->id = val.bstrVal?(int)val:-1;

		pElement->getAttribute(L"refid",&val);
		pObject->refid = val.bstrVal?(int)val:-1;

		BSTR bstrText;
		pElement->get_text(&bstrText);
		if(bstrText)
		{
			pObject->pStrText = XUtil::tcsdupFromBstr(bstrText);
			::SysFreeString(bstrText);
		}
		return 0;
	}
	int ReadShape(IXMLDOMElement * pElement,tagShape * pShape)
	{
		_variant_t	val;
		IXMLDOMElement*  pElementEx;

		IXMLDOMNodeListPtr pDrawObjectList;
		IXMLDOMNodePtr pNodeShape;
		IXMLDOMNodePtr pNodeDrawObject;

		LPTSTR pStr;
		pStr = xbXml::GetStringAttribe(pElement,_T("name"));
		pShape->name = pStr?pStr:NULL;
		pElement->getAttribute(L"id",&val);
		pShape->id = val.bstrVal?(int)val:-1;

		//read drawobj
		long nLenDrawObject;
		pElement->selectNodes(L"*",&pDrawObjectList);
		pDrawObjectList->get_length(&nLenDrawObject);
		for(int j=0;j<nLenDrawObject;j++)
		{
			XDFDrawObject*	pObject = new XDFDrawObject();
			pDrawObjectList->get_item(j,&pNodeDrawObject);
			pElementEx = xbXml::GetElement(pNodeDrawObject);

			ReadDrawObject(pElementEx,pObject);
			pShape->m_DrawObjs.push_back(pObject);
			pElementEx->Release();
		}
		return 0;
	}
	int XDFSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		XDFDataSvr * pDFDataSvr = (XDFDataSvr *)pDataSvr;

		IXMLDOMElement* pElement;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMElementPtr pStyleSheet;
		IXMLDOMNodeListPtr	pNodeList;

		IXMLDOMNodeListPtr pStyleList;
		IXMLDOMNodeListPtr pPageList;
		IXMLDOMNodeListPtr pShapeList;
		IXMLDOMNodeListPtr pDrawObjectList;

		IXMLDOMNodePtr pNodeStyle;
		IXMLDOMNodePtr pNodepage;
		IXMLDOMNodePtr pNodeShape;
		IXMLDOMNodePtr pNodeDrawObject;

		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xdf")) != 0)
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
				pDFDataSvr->m_pStyleSvr->AddCssClass(pStrClass,bstrText);
				::SysFreeString(bstrText);
			}
			if(pStrClass)delete pStrClass;
			pElement->Release();
		}


		if(!pDFDataSvr->m_pPagesSvr)
		pDFDataSvr->m_pPagesSvr = new XDFPagesSvr; 
		LPTSTR pStrClass = xbXml::GetStringAttribe(pSheetElement,_T("class"));
		if(pStrClass)
		{
			pDFDataSvr->m_pPagesSvr->m_nIndexStyle = pDFDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
			if(pStrClass)delete pStrClass;
		}

		_variant_t	val;
		long nLenShape;
		long	len=-1;
		long	nLenDrawObject;
		pSheetElement->selectNodes((_bstr_t)"shapes/shape",&pShapeList);
		pShapeList->get_length(&nLenShape);
		for(int i=0;i<nLenShape;i++)
		{
			tagShape * pShape = new tagShape;
			pShapeList->get_item(i,&pNodeShape);
			pElement = xbXml::GetElement(pNodeShape);
			ReadShape(pElement,pShape);
			pElement->Release();
			pDFDataSvr->m_pPagesSvr->m_Shapes.push_back(pShape);
		}

		pSheetElement->selectNodes((_bstr_t)"pages/page",&pPageList);
		pPageList->get_length(&len);
		for(int i=0;i<len;i++)
		{
			pPageList->get_item(i,&pNodepage);
			pElement = xbXml::GetElement(pNodepage);

			XDFPageItem* pPageItem = new  XDFPageItem;
			pElement->getAttribute(L"id",&val);
			pPageItem->id =  val;
			
			pElement->getAttribute(L"pagew",&val);
			if(val.bstrVal)
			{
				XUtil::Css::TMeasure mes;
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&mes);
				pPageItem->pagew = (int)XUtil::Css::GetPixelExtent(&mes,true);
			}
			pElement->getAttribute(L"pageh",&val);
			if(val.bstrVal)
			{
				XUtil::Css::TMeasure mes;
				XUtil::Css::ProcessCssMeasure(val.bstrVal,&mes);
				pPageItem->pageh = (int)XUtil::Css::GetPixelExtent(&mes);
			}

			LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				pPageItem->m_nIndexStyle = pDFDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}
			pPageItem->m_pXDFPagesSvr = pDFDataSvr->m_pPagesSvr;
			pDFDataSvr->m_pPagesSvr->m_Pages.push_back(pPageItem);

			pElement->selectNodes(L"*",&pDrawObjectList);
			pElement->Release();
			pDrawObjectList->get_length(&nLenDrawObject);
			for(int j=0;j<nLenDrawObject;j++)
			{
				pDrawObjectList->get_item(j,&pNodeDrawObject);
				pElement = xbXml::GetElement(pNodeDrawObject);

				BSTR bstrTag;
				pElement->get_tagName(&bstrTag);
				if(::_tcsicmp(bstrTag,L"shapes")==0)
				{
					pElement->selectNodes((_bstr_t)"shapes/shape",&pShapeList);
					pElement->Release();
					pShapeList->get_length(&nLenShape);
					for(int i=0;i<nLenShape;i++)
					{
						tagShape * pShape = new tagShape;
						pShapeList->get_item(i,&pNodeShape);
						pElement = xbXml::GetElement(pNodeShape);
						ReadShape(pElement,pShape);
						pElement->Release();
						pPageItem->m_Shapes.push_back(pShape);
					}
				}else
				{
					XDFDrawObject*	pObject = new XDFDrawObject();
					ReadDrawObject(pElement,pObject);
					pElement->Release();
					pPageItem->m_DrawObjs.push_back(pObject);
				}
			}
		}
		return 1;
	}
	int XDFSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xdf");

		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release();
		return 1;
	}
}}}}
