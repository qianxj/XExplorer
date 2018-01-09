#include "StdAfx.h"
#include "xbxbase.h"
#include "XWordSerializeSvr.hpp"
#include "xworddataSvr.hpp"
#include "xcell.h"
#include "wpp4.h"
#include "XWordPred.hpp"

using namespace vord;
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XWordSerializeSvr::XWordSerializeSvr(void)
	{
	}

	XWordSerializeSvr::~XWordSerializeSvr(void)
	{
	}
#ifdef V3
	int XWordSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
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
		if(!pWordDataSvr->m_pContentSvr->m_pDocument)
		{
			pWordDataSvr->m_pContentSvr->m_pDocument = new XWordBlockMgr;
		}
		XWordBlock & document = *pWordDataSvr->m_pContentSvr->m_pDocument->GetDocument();
		LPTSTR pStrClass = xbXml::GetStringAttribe(pSheetElement,_T("class"));
		if(pStrClass)
		{
			pWordDataSvr->m_pContentSvr->m_pDocument->GetDocument()->setCssIndex(pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass));
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

			XWordBlock & chapter = (XWordBlock &)pWordDataSvr->m_pContentSvr->m_pDocument->m_pBlocks->push(
				xtree<XWordBlock>::Iterator(&document),XWordBlock(XWordBlockType::chapter));
			LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				chapter.setCssIndex(pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass));
				if(pStrClass)delete pStrClass;
			}

			LPTSTR pStr = xbXml::GetStringAttribe(pElement,_T("breakpage"));
			if(pStr)
			{
				if(::_tcsicmp(pStr,_T("yes"))==0 || ::_tcsicmp(pStr,_T("true"))==0 || ::_tcsicmp(pStr,_T("1"))==0)
					chapter.setBreakPage(true);
				if(pStr)delete pStr; 
			}			
			LPTSTR pStyle = xbXml::GetStringAttribe(pElement,_T("style"));
			if(pStyle)
			{
				XUtil::Css::tagBoxStyle * pBoxStyle = pWordDataSvr->m_pStyleSvr->m_pCssStyle->ParseCssStyle(pStyle);
				pBoxStyle = &((*pBoxStyle) + (*pWordDataSvr->m_pStyleSvr->GetBoxStyle(chapter.getCssIndex()))); 
				chapter.setCssIndex(pWordDataSvr->m_pStyleSvr->m_pCssStyle->AddCssCellStyle(pBoxStyle));
				delete pBoxStyle;
				if(pStyle)delete pStyle; 
			}
			pElement->Release();

			long	nLenParagrap=0;
			pNodeChapter->selectNodes(L"paragraph|p",&pParagraphList);
			pParagraphList->get_length(&nLenParagrap);
			for(int j=0;j<nLenParagrap;j++)
			{
				pParagraphList->get_item(j,&pNodeParagraph);
				pElement = xbXml::GetElement(pNodeParagraph);

				XWordBlock& paragraph = (XWordBlock &)pWordDataSvr->m_pContentSvr->m_pDocument->m_pBlocks->push(
					xtree<XWordBlock>::Iterator(&chapter),XWordBlock(XWordBlockType::paragraph));

				LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
				if(pStrClass)
				{
					paragraph.setCssIndex(pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass));
					if(pStrClass)delete pStrClass;
				}

				LPTSTR pStr = xbXml::GetStringAttribe(pElement,_T("breakpage"));
				if(pStr)
				{
					if(::_tcsicmp(pStr,_T("yes"))==0 || ::_tcsicmp(pStr,_T("true"))==0 || ::_tcsicmp(pStr,_T("1"))==0)
						paragraph.setBreakPage(true);
					if(pStr)delete pStr; 
				}

				LPTSTR pStyle = xbXml::GetStringAttribe(pElement,_T("style"));
				if(pStyle)
				{
					XUtil::Css::tagBoxStyle * pBoxStyle = pWordDataSvr->m_pStyleSvr->m_pCssStyle->ParseCssStyle(pStyle);
					pBoxStyle = &((*pBoxStyle) + (*pWordDataSvr->m_pStyleSvr->GetBoxStyle(paragraph.getCssIndex()))); 
					paragraph.setCssIndex(pWordDataSvr->m_pStyleSvr->m_pCssStyle->AddCssCellStyle(pBoxStyle));
					delete pBoxStyle;
					if(pStyle)delete pStyle; 
				}
				pElement->Release();

				LPTSTR pStrControl = xbXml::GetStringAttribe(pElement,_T("controlclass"));
				if(pStrControl)
				{
					paragraph.setControlClass(pStrControl);
					paragraph.setControlElement(pElement);
					pElement->AddRef();

					//pElement->getAttribute(L"layerflow",&val);
					//if(val.bstrVal)
					//	((XWordDataControlParagraph *)pDataParagraph)->dwFlowLayer = GetLayerFlow(val.bstrVal);

					pElement->getAttribute(L"x",&val);
					if(val.bstrVal)
						paragraph.setLeft(val);
					pElement->getAttribute(L"y",&val);
					if(val.bstrVal)
						paragraph.setTop(val);
					pElement->getAttribute(L"right",&val);
					if(val.bstrVal)
						paragraph.setRight(val);
					pElement->getAttribute(L"bottom",&val);
					if(val.bstrVal)
						paragraph.setBottom(val);
					pElement->getAttribute(L"width",&val);
					if(val.bstrVal)
						paragraph.setRight(paragraph.getLeft()+ (int)val - 1);
					pElement->getAttribute(L"height",&val);
					if(val.bstrVal)
						paragraph.setBottom(paragraph.getTop() + (int)val - 1);
					if(_tcsicmp(pStrControl,L"xcell")==0)
					{
						XCell::XCellSheetSvr* pCellSvr = new XCell::XCellSheetSvr();
						paragraph.setControl(pCellSvr);
					}
				}
					
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

					XWordBlock& block = (XWordBlock &)pWordDataSvr->m_pContentSvr->m_pDocument->m_pBlocks->push(
						xtree<XWordBlock>::Iterator(&paragraph),XWordBlock(XWordBlockType::text));
					LPTSTR pStrControl = xbXml::GetStringAttribe(pElement,_T("controlclass"));
					if(pStrControl || bShape)
					{
						block.setControlClass(pStrControl);
						block.setControlElement(pElement);
						pElement->AddRef();

						//pElement->getAttribute(L"layerflow",&val);
						//if(val.bstrVal)
						//	((XWordDataControlblock *)pDatablock)->dwFlowLayer = GetLayerFlow(val.bstrVal);

						pElement->getAttribute(L"x",&val);
						if(val.bstrVal)
							block.setLeft(val);
						pElement->getAttribute(L"y",&val);
						if(val.bstrVal)
							block.setTop(val);
						pElement->getAttribute(L"right",&val);
						if(val.bstrVal)
							block.setRight(val);
						pElement->getAttribute(L"bottom",&val);
						if(val.bstrVal)
							block.setBottom(val);
						pElement->getAttribute(L"width",&val);
						if(val.bstrVal)
							block.setRight(block.getLeft()+ (int)val - 1);
						pElement->getAttribute(L"height",&val);
						if(val.bstrVal)
							block.setBottom(block.getTop() + (int)val - 1);
						if(_tcsicmp(pStrControl,L"xcell")==0)
						{
							XCell::XCellSheetSvr* pCellSvr = new XCell::XCellSheetSvr();
							block.setControl(pCellSvr);
						}
						if(bShape)
							block.setShapeRef(xbXml::GetStringAttribe(pElement,_T("refname")));
						block.setShape(bShape);
					}else
					{
						BSTR	bstrText=NULL;
						pElement->get_text(&bstrText);
						if(bstrText)
						{
							LPTSTR	pStrText;
							pStrText = XUtil::tcsdupFromBstr(bstrText);
							block.setText(pStrText);
							::SysFreeString(bstrText);
						}
					}
					pElement->Release();
				}
			}
		}

		/*
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
			pDatachapter.m_pDataParent = pWordDataSvr->m_pContentSvr->m_pXWordDataDocument;
			pWordDataSvr->m_pContentSvr->m_pXWordDataDocument->m_pChilds->push_back(pDataChapter);

			LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				pDatachapter.m_nIndexStyle = pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
				if(pStrClass)delete pStrClass;
			}

			LPTSTR pStr = xbXml::GetStringAttribe(pElement,_T("breakpage"));
			if(pStr)
			{
				if(::_tcsicmp(pStr,_T("yes"))==0 || ::_tcsicmp(pStr,_T("true"))==0 || ::_tcsicmp(pStr,_T("1"))==0)
					pDatachapter.m_bBreakPage = true;
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
				pDatachapter.m_pChilds->push_back(pDataParagraph);

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
						pDatablock.m_nIndexStyle = pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass);
						if(pStrClass)delete pStrClass;
					}
					
					pDatablock.m_pDataParent = pDataParagraph;
					pDataParagraph->m_pChilds->push_back(pDataBlock);

					pElement->Release();
				}
			}
		}*/

		return 1;
	}

	int XWordSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
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
				if(pchapter.m_nIndexStyle>=0)
				{
					pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(pchapter.m_nIndexStyle,strClass);
					pChaterElement->setAttribute(L"class",(_variant_t)strClass);
				}
				if(pchapter.m_bBreakPage)
					pChaterElement->setAttribute(L"breakpage",(_variant_t)L"yes");
				for(int k =0;k<pchapter.m_pChilds->size();k++)
				{
					pPara = (XWordDataContentParagraph *)(*pchapter.m_pChilds)[k];
					pXmlDoc->createElement(L"p",&pParaElement);
					if(ppara.m_nIndexStyle>=0)
					{
						pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(ppara.m_nIndexStyle,strClass);
						pParaElement->setAttribute(L"class",(_variant_t)strClass);
					}
					if(ppara.m_bBreakPage)
						pParaElement->setAttribute(L"breakpage",(_variant_t)L"yes");

					if(ppara.eWordDataType==_XWordDataControlParagraph)
					{
						pParaElement->setAttribute(L"controlclass",(_variant_t)((XWordDataControlParagraph *)pPara)->m_pStrControlClass);
						pParaElement->setAttribute(L"x",(_variant_t)((XWordDataControlParagraph *)pPara)->x);
						pParaElement->setAttribute(L"y",(_variant_t)((XWordDataControlParagraph *)pPara)->y);
						pParaElement->setAttribute(L"height",(_variant_t)((XWordDataControlParagraph *)pPara)->height);
						pParaElement->setAttribute(L"width",(_variant_t)((XWordDataControlParagraph *)pPara)->width);
					}
					if(ppara.align== DT_CENTER)
						pParaElement->setAttribute(L"align",(_variant_t)L"center");
					if(ppara.align== DT_RIGHT)
						pParaElement->setAttribute(L"align",(_variant_t)L"right");
					for(int m=0;m<ppara.m_pChilds->size();m++)
					{
						pBlock = (XWordDataSpanBlock  *)(*ppara.m_pChilds)[m];
						if(pblock.eWordDataType ==_XWordDataSpanControlBlock && ((XWordDataSpanControlBlock *)pBlock)->m_bShape)
							pXmlDoc->createElement(L"shape",&pBlockElement);
						else
							pXmlDoc->createElement(L"span",&pBlockElement);
						if(pblock.eWordDataType ==_XWordDataSpanControlBlock)
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

						if(pblock.m_nIndexStyle>=0)
						{
							pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(pblock.m_nIndexStyle,strClass);
							pBlockElement->setAttribute(L"class",(_variant_t)strClass);
						}
						if(pblock.m_bBreakPage)
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
#else
	int XWordSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
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
		vord_t& document = pWordDataSvr->m_pContentSvr->document;
		document.append(block::type,blocktype::document);
		LPTSTR pStrClass = xbXml::GetStringAttribe(pSheetElement,_T("class"));
		if(pStrClass)
		{
			document.append(css::index,
				pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass) + 1);
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

			vord_t chapter(document.groupid());
			chapter.append(block::type,blocktype::chapter);
			document.appendChild(chapter);
			LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
			if(pStrClass)
			{
				chapter.append(css::index,
					pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass) + 1);
				if(pStrClass)delete pStrClass;
			}

			LPTSTR pStr = xbXml::GetStringAttribe(pElement,_T("breakpage"));
			if(pStr)
			{
				if(::_tcsicmp(pStr,_T("yes"))==0 || ::_tcsicmp(pStr,_T("true"))==0 || ::_tcsicmp(pStr,_T("1"))==0)
					chapter.append(css::breakpage,1);
				if(pStr)delete pStr; 
			}			
			LPTSTR pStyle = xbXml::GetStringAttribe(pElement,_T("style"));
			if(pStyle)
			{
				XUtil::Css::tagBoxStyle * pBoxStyle = pWordDataSvr->m_pStyleSvr->m_pCssStyle->ParseCssStyle(pStyle);
				pBoxStyle = &((*pBoxStyle) + (*pWordDataSvr->m_pStyleSvr->GetBoxStyle(chapter(css::index) - 1))); 
				
				chapter[css::index] = 
					pWordDataSvr->m_pStyleSvr->m_pCssStyle->AddCssCellStyle(pBoxStyle) + 1;
				
				delete pBoxStyle;
				if(pStyle)delete pStyle; 
			}
			pElement->Release();

			long	nLenParagrap=0;
			pNodeChapter->selectNodes(L"paragraph|p",&pParagraphList);
			pParagraphList->get_length(&nLenParagrap);
			for(int j=0;j<nLenParagrap;j++)
			{
				pParagraphList->get_item(j,&pNodeParagraph);
				pElement = xbXml::GetElement(pNodeParagraph);

				vord_t paragraph(document.groupid());
				paragraph.append(block::type,blocktype::paragraph);
				chapter.appendChild(paragraph);

				LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
				if(pStrClass)
				{
					paragraph.setItem(css::index,
						pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass) + 1);
					if(pStrClass)delete pStrClass;
				}

				LPTSTR pStr = xbXml::GetStringAttribe(pElement,_T("breakpage"));
				if(pStr)
				{
					if(::_tcsicmp(pStr,_T("yes"))==0 || ::_tcsicmp(pStr,_T("true"))==0 || ::_tcsicmp(pStr,_T("1"))==0)
						paragraph.append(css::breakpage,1);
					if(pStr)delete pStr; 
				}

				LPTSTR pStyle = xbXml::GetStringAttribe(pElement,_T("style"));
				if(pStyle)
				{
					XUtil::Css::tagBoxStyle * pBoxStyle = pWordDataSvr->m_pStyleSvr->m_pCssStyle->ParseCssStyle(pStyle);
					pBoxStyle = &((*pBoxStyle) + (*pWordDataSvr->m_pStyleSvr->GetBoxStyle(paragraph(css::index) - 1))); 
					paragraph[css::index] = 
						pWordDataSvr->m_pStyleSvr->m_pCssStyle->AddCssCellStyle(pBoxStyle) + 1;
					delete pBoxStyle;
					if(pStyle)delete pStyle; 
				}
				pElement->Release();

				LPTSTR pStrControl = xbXml::GetStringAttribe(pElement,_T("controlclass"));
				if(pStrControl)
				{
					paragraph.append(control::type,pStrControl);
					paragraph.append(control::element,pElement);
					pElement->AddRef();

					//pElement->getAttribute(L"layerflow",&val);
					//if(val.bstrVal)
					//	((XWordDataControlParagraph *)pDataParagraph)->dwFlowLayer = GetLayerFlow(val.bstrVal);

					pElement->getAttribute(L"x",&val);
					if(val.bstrVal)
						paragraph.append(control::left,(int)val);
					pElement->getAttribute(L"y",&val);
					if(val.bstrVal)
						paragraph.append(control::top,(int)val);
					pElement->getAttribute(L"right",&val);
					if(val.bstrVal)
						paragraph.append(control::right,(int)val);
					pElement->getAttribute(L"bottom",&val);
					if(val.bstrVal)
						paragraph.append(control::bottom,(int)val);
					pElement->getAttribute(L"width",&val);
					if(val.bstrVal)
						paragraph[control::right] = paragraph(control::left) + (int)val - 1;
					pElement->getAttribute(L"height",&val);
					if(val.bstrVal)
						paragraph[control::bottom] = paragraph(control::top) + (int)val - 1;
					if(_tcsicmp(pStrControl,L"xcell")==0)
					{
						XCell::XCellSheetSvr* pCellSvr = new XCell::XCellSheetSvr();
						paragraph[control::object] = pCellSvr;
					}
				}
					
				long	nLenBlock=0;
				pNodeParagraph->selectNodes(L"block|span|div|shape",&pBlockList);
				pBlockList->get_length(&nLenBlock);
				bool bShape = false;
				for(int k=0;k<nLenBlock;k++)
				{
					pBlockList->get_item(k,&pNodeBlock);
					pElement = xbXml::GetElement(pNodeBlock);

					vord_t block(document.groupid());
					block.append(block::type,blocktype::text);
					paragraph.appendChild(block);

					BSTR bstrTagName=NULL;
					pElement->get_tagName(&bstrTagName);
					bShape = false;
					if(::_wcsicmp(bstrTagName,_T("shape"))==0)bShape = true;
					SysFreeString(bstrTagName);
					
					if(bShape)
						block.setItem(block::type,blocktype::shape);

					LPTSTR pStrClass = xbXml::GetStringAttribe(pElement,_T("class"));
					if(pStrClass)
					{
						block.append(css::index,
							pWordDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(pStrClass) + 1);
						if(pStrClass)delete pStrClass;
					}
					LPTSTR pStyle = xbXml::GetStringAttribe(pElement,_T("style"));
					if(pStyle)
					{
						XUtil::Css::tagBoxStyle * pBoxStyle = pWordDataSvr->m_pStyleSvr->m_pCssStyle->ParseCssStyle(pStyle);
						pBoxStyle = &((*pBoxStyle) + (*pWordDataSvr->m_pStyleSvr->GetBoxStyle(paragraph(css::index) - 1))); 
						block[css::index] = 
							pWordDataSvr->m_pStyleSvr->m_pCssStyle->AddCssCellStyle(pBoxStyle) + 1;
						delete pBoxStyle;
						if(pStyle)delete pStyle; 
					}

					LPTSTR pStrControl = xbXml::GetStringAttribe(pElement,_T("controlclass"));
					if(pStrControl || bShape)
					{
						if(pStrControl)
							block.setItem(block::type,blocktype::control);
						block.append(control::type,pStrControl);
						block.append(control::element,pElement);
						pElement->AddRef();

						//pElement->getAttribute(L"layerflow",&val);
						//if(val.bstrVal)
						//	((XWordDataControlblock *)pDatablock)->dwFlowLayer = GetLayerFlow(val.bstrVal);

						pElement->getAttribute(L"x",&val);
						if(val.bstrVal)
							block.append(control::left,(int)val);
						pElement->getAttribute(L"y",&val);
						if(val.bstrVal)
							block.append(control::top,(int)val);
						pElement->getAttribute(L"right",&val);
						if(val.bstrVal)
							block.append(control::right,(int)val);
						pElement->getAttribute(L"bottom",&val);
						if(val.bstrVal)
							block.append(control::bottom,(int)val);
						pElement->getAttribute(L"width",&val);
						if(val.bstrVal)
							block[control::right] = paragraph(control::left) + (int)val - 1;
						pElement->getAttribute(L"height",&val);
						if(val.bstrVal)
							block[control::bottom] = paragraph(control::top) + (int)val - 1;
						if(_tcsicmp(pStrControl,L"xcell")==0)
						{
							XCell::XCellSheetSvr* pCellSvr = new XCell::XCellSheetSvr();
							block[control::object] = pCellSvr;
						}
						if(bShape)
							block[shape::ref] = xbXml::GetStringAttribe(pElement,_T("refname"));
					}else
					{
						BSTR	bstrText=NULL;
						pElement->get_text(&bstrText);
						if(bstrText)
						{
							LPTSTR	pStrText;
							pStrText = XUtil::tcsdupFromBstr(bstrText);
							block.append(text::content,value_t(pStrText));
							::SysFreeString(bstrText);
						}
					}
					pElement->Release();
				}
			}
		}
		return 1;
	}

	int XWordSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
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
		vord_t doc = pWordDataSvr->m_pContentSvr->document;
		if(doc.id())
		{
			if(doc[css::index]>0)
			{
				pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(doc[css::index] - 1,strClass);
				pElement->setAttribute(L"class",(_variant_t)strClass);
			}

			IXMLDOMElement * pChaterElement;
			IXMLDOMElement * pParaElement;
			IXMLDOMElement * pBlockElement;
			vord_t chapter = doc.get_child();
			for(;chapter;chapter = chapter.get_next())
			{
				pXmlDoc->createElement(L"chapter",&pChaterElement);
				if(chapter[css::index] > 0 )
				{
					pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(chapter[css::index] - 1,strClass);
					pChaterElement->setAttribute(L"class",(_variant_t)strClass);
				}
				if(chapter[css::breakpage]!=0)
					pChaterElement->setAttribute(L"breakpage",(_variant_t)L"yes");

				vord_t para = chapter.get_child();
				for(;para;para = para.get_next())
				{
					pXmlDoc->createElement(L"p",&pParaElement);
					if(para[css::index]>0)
					{
						pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(para[css::index] - 1,strClass);
						pParaElement->setAttribute(L"class",(_variant_t)strClass);
					}
					if(para[css::breakpage]!=0)
					pParaElement->setAttribute(L"breakpage",(_variant_t)L"yes");

					if(para[control::type])
					{
						pParaElement->setAttribute(L"controlclass",(_variant_t)para[control::type]);
						if(para[control::left])
							pParaElement->setAttribute(L"x",(_variant_t)para[control::left]);
						if(para[control::top])
							pParaElement->setAttribute(L"y",(_variant_t)para[control::top]);
						//if(para[control::height]>0)
						//	pParaElement->setAttribute(L"height",(_variant_t)para[control::height]);
						//if(para[control::width]>0)
						//	pParaElement->setAttribute(L"width",(_variant_t)para[control::width]);
						if(para[control::right])
							pParaElement->setAttribute(L"right",(_variant_t)para[control::right]);
						if(para[control::bottom])
							pParaElement->setAttribute(L"bottom",(_variant_t)para[control::bottom]);
						if(para[control::element])
						{
							IXMLDOMElement* e = (IXMLDOMElement*)(para[control::element]);
							IXMLDOMNode * node = 0;
							e->selectSingleNode(L"@src",&node);
							if(node)
							{
								BSTR bstr;
								node->get_text(&bstr);
								pParaElement->setAttribute(L"src",(_variant_t)bstr);
								::SysFreeString(bstr);
								node->Release();
							}
						}
						if(para[shape::ref])
						{
							pParaElement->setAttribute(L"refname",(_variant_t)para[shape::ref]);
						}
					}
					//if(ppara.align== DT_CENTER)
					//	pParaElement->setAttribute(L"align",(_variant_t)L"center");
					//if(ppara.align== DT_RIGHT)
					//	pParaElement->setAttribute(L"align",(_variant_t)L"right");
					vord_t block = para.get_child();
					for(;block;block = block.get_next())
					{
						if(block[shape::ref])
							pXmlDoc->createElement(L"shape",&pBlockElement);
						else
							pXmlDoc->createElement(L"span",&pBlockElement);
						if(block[shape::ref]||block[control::type])
						{
							pBlockElement->setAttribute(L"controlclass",(_variant_t)block[control::type]);
							if(block[control::left]>0)
								pBlockElement->setAttribute(L"x",(_variant_t)block[control::left]);
							if(block[control::top]>0)
								pBlockElement->setAttribute(L"y",(_variant_t)block[control::top]);
							//if(block[control::height]>0)
							//	pBlockElement->setAttribute(L"height",(_variant_t)block[control::height]);
							//if(block[control::width]>0)
							//	pBlockElement->setAttribute(L"width",(_variant_t)block[control::width]);
							if(block[control::right]>0)
								pBlockElement->setAttribute(L"right",(_variant_t)block[control::right]);
							if(block[control::bottom]>0)
								pBlockElement->setAttribute(L"bottom",(_variant_t)block[control::bottom]);
							if(block[control::element])
							{
								IXMLDOMElement* e = (IXMLDOMElement*)(block[control::element]);
								IXMLDOMNode * node = 0;
								e->selectSingleNode(L"@src",&node);
								if(node)
								{
									BSTR bstr;
									node->get_text(&bstr);
									pBlockElement->setAttribute(L"src",(_variant_t)bstr);
									::SysFreeString(bstr);
									node->Release();
								}
							}
							if(block[shape::ref])
							{
								pBlockElement->setAttribute(L"refname",(_variant_t)block[shape::ref]);
							}
						}else
							pBlockElement->put_text(block[text::content]);

						if(block[css::index])
						{
							pDataSvr->m_pStyleSvr->m_pCssStyle->GetClassNameByIndex(block[css::index] - 1,strClass);
							pBlockElement->setAttribute(L"class",(_variant_t)strClass);
						}
						if(block[css::breakpage])
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
#endif
}}}}
