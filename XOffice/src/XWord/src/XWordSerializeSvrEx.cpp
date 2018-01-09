#include "StdAfx.h"
#include "xbxbase.h"
#include "..\include\XWordSerializeSvrEx.hpp"
#include "xworddataSvr.hpp"
#include "xcell.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XWordSerializeSvrEx::XWordSerializeSvrEx(void)
	{
	}

	XWordSerializeSvrEx::~XWordSerializeSvrEx(void)
	{
	}

	int XWordSerializeSvrEx::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		XWordDataSvr * pWordDataSvr = (XWordDataSvr *)pDataSvr;

		IXMLDOMElement* pElement;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMElementPtr pStyleSheet;
		IXMLDOMNodeListPtr	pNodeList;
		IXMLDOMNodeListPtr	pColNodeList;

		IXMLDOMNodeListPtr pStyleList;
		IXMLDOMNodeListPtr pChapterList;
		IXMLDOMNodeListPtr pParagraphList;
		IXMLDOMNodeListPtr pBlockList;
		IXMLDOMNodeListPtr pAreaList;

		IXMLDOMNodePtr pNodeStyle;
		IXMLDOMNodePtr pNodeChapter;
		IXMLDOMNodePtr pNodeParagraph;
		IXMLDOMNodePtr pNodeBlock;
		IXMLDOMNodePtr pNodeArea;

		LPTSTR pControlName = NULL;
		pControlName = xbXml::GetStringAttribe( pSheetElement,_T("editer"));
		if(pControlName)
		{
			if(::_tcsicmp(pControlName,_T("xword")) != 0)
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
				pWordDataSvr->m_pStyleSvr->AddCssClass(pStrClass,bstrText);
				::SysFreeString(bstrText);
			}
			if(pStrClass)delete pStrClass;
			pElement->Release();
		}


		if(!pWordDataSvr->m_pContentSvr)
		pWordDataSvr->m_pContentSvr = new XWordContentSvr; 
		if(!pWordDataSvr->m_pContentSvr->m_pXWordDataDocument)
		{
			pWordDataSvr->m_pContentSvr->m_pXWordDataDocument = new XWordDataDocument;
		}

		LPTSTR pStrClass = xbXml::GetStringAttribe(pSheetElement,_T("class"));
		if(pStrClass)
		{
			pWordDataSvr->m_pContentSvr->m_pXWordDataDocument->m_nIndexStyle = pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
			if(pStrClass)delete pStrClass;
		}

		_variant_t	val;
		pSheetElement->selectNodes((_bstr_t)"chapter",&pChapterList);
		long	len=-1;
		pChapterList->get_length(&len);
		for(int i=0;i<len;i++)
		{
			pChapterList->get_item(i,&pNodeChapter);
			pElement = xbXml::GetElement(pNodeChapter);

			XWordDataChapter* pDataChapter = new  XWordDataChapter;
			pDataChapter->m_pDataParent = pWordDataSvr->m_pContentSvr->m_pXWordDataDocument;
			pWordDataSvr->m_pContentSvr->m_pXWordDataDocument->m_pChilds->push_back(pDataChapter);

			LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				pDataChapter->m_nIndexStyle = pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}

			LPTSTR pStr = xbXml::GetStringAttribe(pElement,_T("breakpage"));
			if(pStr)
			{
				if(::_tcsicmp(pStr,_T("yes"))==0 || ::_tcsicmp(pStr,_T("true"))==0 || ::_tcsicmp(pStr,_T("1"))==0)
					pDataChapter->m_bBreakPage = true;
				if(pStr)delete pStr; 
			}

			pElement->Release();
			
			long	nLenParagrap=0;
			pNodeChapter->selectNodes(L"paragraph|p",&pParagraphList);
			pParagraphList->get_length(&nLenParagrap);
			for(int j=0;j<nLenParagrap;j++)
			{
				pParagraphList->get_item(j,&pNodeParagraph);
				pElement = xbXml::GetElement(pNodeParagraph);

				LPTSTR pStrControl = xbXml::GetStringAttribe(pElement,_T("controlclass"));
				XWordDataContentParagraph * pDataParagraph;
				if(pStrControl)
				{
					pDataParagraph = new XWordDataControlParagraph;
					((XWordDataControlParagraph *)pDataParagraph)->m_pStrControlClass = pStrControl;
					((XWordDataControlParagraph *)pDataParagraph)->param = (LPARAM)pElement;
					pElement->AddRef();//leak memory

					pElement->getAttribute(L"layerflow",&val);
					if(val.bstrVal)
						((XWordDataControlParagraph *)pDataParagraph)->dwFlowLayer = GetLayerFlow(val.bstrVal);

					pElement->getAttribute(L"x",&val);
					if(val.bstrVal)
						((XWordDataControlParagraph *)pDataParagraph)->x = val;
					pElement->getAttribute(L"y",&val);
					if(val.bstrVal)
						((XWordDataControlParagraph *)pDataParagraph)->y = val;
					pElement->getAttribute(L"width",&val);
					if(val.bstrVal)
						((XWordDataControlParagraph *)pDataParagraph)->width = val;
					pElement->getAttribute(L"height",&val);

					if(_tcsicmp(pStrControl,L"xcell")==0)
					{
						XCell::XCellSheetSvr* pCellSvr = new XCell::XCellSheetSvr();
						((XWordDataControlParagraph *)pDataParagraph)->m_pObj = pCellSvr;
					}
					if(val.bstrVal)
						((XWordDataControlParagraph *)pDataParagraph)->height = val;
				}else
					pDataParagraph = new  XWordDataParagraph;


				LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
				if(pStrClass)
				{
					pDataParagraph->m_nIndexStyle = pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
					if(pStrClass)delete pStrClass;
				}

				pStr = xbXml::GetStringAttribe(pElement,_T("breakpage"));
				if(pStr)
				{
					if(::_tcsicmp(pStr,_T("yes"))==0 || ::_tcsicmp(pStr,_T("true"))==0 || ::_tcsicmp(pStr,_T("1"))==0)
						pDataParagraph->m_bBreakPage = true;
					if(pStr)delete pStr; 
				}

				pDataParagraph->m_pDataParent = pDataChapter;
				pDataChapter->m_pChilds->push_back(pDataParagraph);

				LPTSTR pStrAlign = xbXml::GetStringAttribe(pElement,_T("align"));
				if(pStrAlign)
				{
					if(::_tcsicmp(pStrAlign,_T("center"))==0)
						pDataParagraph->align = DT_CENTER;
					if(::_tcsicmp(pStrAlign,_T("right"))==0)
						pDataParagraph->align = DT_RIGHT;
					if(::_tcsicmp(pStrAlign,_T("left"))==0)
						pDataParagraph->align = DT_LEFT;
					delete pStrAlign ;
				}
				pElement->Release();

				long	nLenBlock=0;
				pNodeParagraph->selectNodes(L"block|span|div|shape",&pBlockList);
				pBlockList->get_length(&nLenBlock);
				bool bShape = false;
				for(int k=0;k<nLenBlock;k++)
				{
					pBlockList->get_item(k,&pNodeBlock);

					pElement = xbXml::GetElement(pNodeBlock);
					
					BSTR bstrTagName=NULL;
					pElement->get_tagName(&bstrTagName);
					bShape = false;
					if(::_wcsicmp(bstrTagName,_T("shape"))==0)bShape = true;
					SysFreeString(bstrTagName);

					XWordDataSpanBlock * pDataBlock;
					LPTSTR pStrControl = NULL;
					pStrControl = xbXml::GetStringAttribe(pElement,_T("controlclass"));
					if(pStrControl ||bShape)
					{
						pDataBlock = new XWordDataSpanControlBlock;
						((XWordDataSpanControlBlock *)pDataBlock)->m_pStrControlClass = pStrControl;
						((XWordDataSpanControlBlock *)pDataBlock)->param = (LPARAM)pElement;
						pElement->AddRef();//leak memory
						pElement->getAttribute(L"layerflow",&val);
						if(val.bstrVal)
							((XWordDataSpanControlBlock *)pDataBlock)->dwFlowLayer = GetLayerFlow(val.bstrVal);

						pElement->getAttribute(L"x",&val);
						if(val.bstrVal)
							((XWordDataSpanControlBlock *)pDataBlock)->x = val;
						pElement->getAttribute(L"y",&val);
						if(val.bstrVal)
							((XWordDataSpanControlBlock *)pDataBlock)->y = val;
						pElement->getAttribute(L"width",&val);
						if(val.bstrVal)
							((XWordDataSpanControlBlock *)pDataBlock)->width = val;
						pElement->getAttribute(L"height",&val);
						if(val.bstrVal)
							((XWordDataSpanControlBlock *)pDataBlock)->height = val;

						if(bShape)
							((XWordDataSpanControlBlock *)pDataBlock)->m_pStrShape = xbXml::GetStringAttribe(pElement,_T("refname"));

						((XWordDataSpanControlBlock *)pDataBlock)->m_bShape = bShape;
					}else
					{
						pDataBlock = new  XWordDataTextBlock;
						BSTR	bstrText=NULL;
						pElement->get_text(&bstrText);
						if(bstrText)
						{
							LPTSTR	pStrText;
							pStrText = XUtil::tcsdupFromBstr(bstrText);
							((XWordDataTextBlock*)pDataBlock)->m_pStrText = pStrText;
							::SysFreeString(bstrText);
						}
					}

					LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
					if(pStrClass)
					{
						pDataBlock->m_nIndexStyle = pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
						if(pStrClass)delete pStrClass;
					}
					
					pDataBlock->m_pDataParent = pDataParagraph;
					pDataParagraph->m_pChilds->push_back(pDataBlock);

					pElement->Release();
				}
			}
		}

		return 1;
	}

	int XWordSerializeSvrEx::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xword");

		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release();

		TCHAR strClass[255];
		XWordDataSvr * pWordDataSvr = (XWordDataSvr *)pDataSvr;
		XWordDataDocument* pDoc = pWordDataSvr->m_pContentSvr->m_pXWordDataDocument;
		if(pDoc)
		{
			if(pDoc->m_nIndexStyle>=0)
			{
				pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(pDoc->m_nIndexStyle,strClass);
				pElement->setAttribute(L"class",(_variant_t)strClass);
			}

			IXMLDOMElement * pChaterElement;
			IXMLDOMElement * pParaElement;
			IXMLDOMElement * pBlockElement;
			XWordDataChapter * pChapter;
			XWordDataContentParagraph * pPara;
			XWordDataSpanBlock * pBlock;
			for(int i=0;i<pDoc->m_pChilds->size();i++)
			{
				pChapter = (XWordDataChapter *)(*pDoc->m_pChilds)[i];
				pXmlDoc->createElement(L"chapter",&pChaterElement);
				if(pChapter->m_nIndexStyle>=0)
				{
					pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(pChapter->m_nIndexStyle,strClass);
					pChaterElement->setAttribute(L"class",(_variant_t)strClass);
				}
				if(pChapter->m_bBreakPage)
					pChaterElement->setAttribute(L"breakpage",(_variant_t)L"yes");
				for(int k =0;k<pChapter->m_pChilds->size();k++)
				{
					pPara = (XWordDataContentParagraph *)(*pChapter->m_pChilds)[k];
					pXmlDoc->createElement(L"p",&pParaElement);
					if(pPara->m_nIndexStyle>=0)
					{
						pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(pPara->m_nIndexStyle,strClass);
						pParaElement->setAttribute(L"class",(_variant_t)strClass);
					}
					if(pPara->m_bBreakPage)
						pParaElement->setAttribute(L"breakpage",(_variant_t)L"yes");

					if(pPara->eWordDataType==_XWordDataControlParagraph)
					{
						pParaElement->setAttribute(L"controlclass",(_variant_t)((XWordDataControlParagraph *)pPara)->m_pStrControlClass);
						pParaElement->setAttribute(L"x",(_variant_t)((XWordDataControlParagraph *)pPara)->x);
						pParaElement->setAttribute(L"y",(_variant_t)((XWordDataControlParagraph *)pPara)->y);
						pParaElement->setAttribute(L"height",(_variant_t)((XWordDataControlParagraph *)pPara)->height);
						pParaElement->setAttribute(L"width",(_variant_t)((XWordDataControlParagraph *)pPara)->width);
					}
					if(pPara->align== DT_CENTER)
						pParaElement->setAttribute(L"align",(_variant_t)L"center");
					if(pPara->align== DT_RIGHT)
						pParaElement->setAttribute(L"align",(_variant_t)L"right");
					for(int m=0;m<pPara->m_pChilds->size();m++)
					{
						pBlock = (XWordDataSpanBlock  *)(*pPara->m_pChilds)[m];
						if(pBlock->eWordDataType ==_XWordDataSpanControlBlock && ((XWordDataSpanControlBlock *)pBlock)->m_bShape)
							pXmlDoc->createElement(L"shape",&pBlockElement);
						else
							pXmlDoc->createElement(L"span",&pBlockElement);
						if(pBlock->eWordDataType ==_XWordDataSpanControlBlock)
						{
							pBlockElement->setAttribute(L"controlclass",(_variant_t)((XWordDataSpanControlBlock *)pBlock)->m_pStrControlClass);
							pBlockElement->setAttribute(L"x",(_variant_t)((XWordDataSpanControlBlock *)pBlock)->x);
							pBlockElement->setAttribute(L"y",(_variant_t)((XWordDataSpanControlBlock *)pBlock)->y);
							pBlockElement->setAttribute(L"height",(_variant_t)((XWordDataSpanControlBlock *)pBlock)->height);
							pBlockElement->setAttribute(L"width",(_variant_t)((XWordDataSpanControlBlock *)pBlock)->width);
							if(((XWordDataSpanControlBlock *)pBlock)->m_pStrControlClass && 
								!((XWordDataSpanControlBlock *)pBlock)->m_bShape)
							{
								_variant_t v;
								((IXMLDOMElement*)((XWordDataSpanControlBlock *)pBlock)->param)->getAttribute(L"src",&v);
								pBlockElement->setAttribute(L"src",v);
							}
							if(((XWordDataSpanControlBlock *)pBlock)->m_bShape)
							{
								pBlockElement->setAttribute(L"refname",(_variant_t)((XWordDataSpanControlBlock *)pBlock)->m_pStrShape);
							}
						}else
							pBlockElement->put_text(((XWordDataTextBlock*)pBlock)->m_pStrText);

						if(pBlock->m_nIndexStyle>=0)
						{
							pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(pBlock->m_nIndexStyle,strClass);
							pBlockElement->setAttribute(L"class",(_variant_t)strClass);
						}
						if(pBlock->m_bBreakPage)
							pBlockElement->setAttribute(L"breakpage",(_variant_t)L"yes");

						pParaElement->appendChild(pBlockElement,NULL);
						pBlockElement->Release();
					}
					pChaterElement->appendChild(pParaElement,NULL);
					pParaElement->Release();

				}
				pElement->appendChild(pChaterElement,NULL);
				pChaterElement->Release();
			}
		}
		return 1;
	}
}}}}
