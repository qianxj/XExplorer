#include "stdafx.h"
#include "xofbase.h"
#include "xcellserializesvr.hpp"
#include "griddatasvr.hpp"
#include "dwtable.hpp"
#include "ContentSvr.hpp"


namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XCellSerializeSvr::XCellSerializeSvr(void)
	{
	}
	XCellSerializeSvr::~XCellSerializeSvr(void)
	{
	}

	int XCellSerializeSvr::LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state)
	{
		CGridDataSvr * pGridDataSvr = dynamic_cast<CGridDataSvr *>(pDataSvr);
		if(!pGridDataSvr) return 1;

		BSTR eleName = NULL;
		pSheetElement->get_tagName(&eleName);
		if(wcsstr(eleName,L"stylesheet")||wcsstr(eleName,L"html"))
		{
			SysFreeString(eleName);
			return LoadSheetTable(pSheetElement,pDataSvr);
		}
		
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
			if(::_tcsicmp(pControlName,_T("xcell")) != 0)
			{
				delete pControlName;
				return 0;
			}
			delete pControlName;
		}else
			return 0;
		
		LPTSTR pSchemaSource = NULL;
		pSchemaSource = xbXml::GetStringAttribe(pSheetElement,_T("schemasource"));
		if(pSchemaSource)
			pGridDataSvr->m_pSchemaSource = pSchemaSource;

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

		IXMLDOMElementPtr pStyleElement;
		IXMLDOMElementPtr pStyleColElement;
		IXMLDOMElementPtr pStyleItemElement;
		IXMLDOMNodeListPtr pStyleColList;
		XCROW	row;
		XCCOL	col;
		_variant_t	val;
		long	len=-1,rowHeight=0;

		pSheetElement->selectNodes(L"cellstyles/col",&pStyleColList);
		pStyleColList->get_length(&nLenStyle);
		for(int i=0;i<nLenStyle;i++)
		{
			XCCOL sCol;
			_variant_t	val;
			pStyleColList->get_item(i,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"colindex",&val);
			sCol = val;
			pNode->selectNodes(L"item",&pNodeList);
			pNodeList->get_length(&len);
			int rows,nEndRow = 0;
			int nIndex;
			for(int k=0;k<len;k++)
			{
				pNodeList->get_item(k,&pNode);
				((IXMLDOMElementPtr)pNode)->getAttribute(L"rowend",&val);
				rows = (int)val - nEndRow;
				nEndRow = val;
				((IXMLDOMElementPtr)pNode)->getAttribute(L"class",&val);
				if(val.bstrVal && val.bstrVal[0]!='\0')
				{
					nIndex = pDataSvr->m_pStyleSvr->GetBoxStyleIndexByClass(val.bstrVal);
				}else
					nIndex = -1;
				(*pGridDataSvr->GetStyleSvr()->m_pCssClass)[sCol]->SetValue(nEndRow - rows + 1,nEndRow,nIndex);
			}
		}

		pSheetElement->selectNodes(L"axis/col/item",&pNodeList);
		pNodeList->get_length(&nLenStyle);
		int nStartCol=1,nEndCol = 0;
		for(int i=0;i<nLenStyle;i++)
		{
			_variant_t	val;
			pNodeList->get_item(i,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"colend",&val);
			if(0==(int)val) continue;
			nEndCol = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"width",&val);
			if(val.bstrVal)
				pGridDataSvr->m_pAxisSvr->m_pColWidths->SetValue(nStartCol,nEndCol,(int)val);
			nStartCol = nEndCol + 1;
		}
		pSheetElement->selectNodes(L"axis/row/item",&pNodeList);
		pNodeList->get_length(&nLenStyle);
		int nStartRow=1,nEndRow = 0;
		for(int i=0;i<nLenStyle;i++)
		{
			_variant_t	val;
			pNodeList->get_item(i,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"rowend",&val);
			if(0==(int)val) continue;
			nEndRow = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"height",&val);
			if(val.bstrVal)
			{
				pGridDataSvr->m_pAxisSvr->m_pRowHeights->SetValue(nStartRow,nEndRow,(int)val);
			}
			nStartRow = nEndRow + 1;
		}

		pSheetElement->selectNodes(L"spans/span",&pNodeList);
		pNodeList->get_length(&len);
		RECT	rect;
		for(int m=0;m<len;m++)
		{
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"left",&val);
			rect.left = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"right",&val);
			rect.right = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"top",&val);
			rect.top= val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"bottom",&val);
			rect.bottom = val;
			pGridDataSvr->m_pSpanSvr->SpanRects.push_back(rect);
		}

		pSheetElement->selectNodes(L"cells/row",&pNodeList);
		pNodeList->get_length(&len);
		long	colLen = 0;
		row = 0;
		for(int m=0;m<len;m++)
		{
			BSTR bstrText = NULL;
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"rowindex",&val);
			row = val.bstrVal?val:row+1;

			pNode->selectNodes(L"cell",&pColNodeList);
			pColNodeList->get_length(&colLen);
			col = 0;
			for(int n=0;n<colLen;n++)
			{
				pColNodeList->get_item(n,&pColNode);
				((IXMLDOMElementPtr)pColNode)->getAttribute(L"colindex",&val);
				col = val.bstrVal?val:col+1;
				
				_variant_t	value;
				PStrItem pItem = new StrItem();
				((IXMLDOMElementPtr)pColNode)->getAttribute(L"coltype",&value);
				if(value.vt !=VT_NULL)pItem->eDTItem = (CContentSvr::eDataType)(int)value;

				((IXMLDOMElementPtr)pColNode)->getAttribute(L"group",&value);
				if(value.vt !=VT_NULL) pItem->pStrGroup = XUtil::tcsdupFromBstr(value.bstrVal);

				((IXMLDOMElementPtr)pColNode)->getAttribute(L"column",&value);
				if(value.vt !=VT_NULL)pItem->pStrColumn = XUtil::tcsdupFromBstr(value.bstrVal);

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"text", &pNodeEx);
				if(pNodeEx)
				{
					((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
					if(bstrText)
					{
						pItem->pStrContent = XUtil::tcsdupFromBstr(bstrText);
						::SysFreeString(bstrText);
					}
				}
				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"display", &pNodeEx);
				if(pNodeEx)
				{
					((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
					if(bstrText)
					{
						pItem->pStrText = XUtil::tcsdupFromBstr(bstrText);
						::SysFreeString(bstrText);
					}
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"formula", &pNodeEx);
				if(pNodeEx)
				{
					((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
					if(bstrText)
					{
						pItem->pStrFormula = XUtil::tcsdupFromBstr(bstrText);
						::SysFreeString(bstrText);
					}
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"xml", &pNodeEx);
				if(pNodeEx)
				{
					((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
					if(bstrText)
					{
						pItem->pStrXml = XUtil::tcsdupFromBstr(bstrText);
						::SysFreeString(bstrText);
					}
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"prestr", &pNodeEx);
				if(pNodeEx)
				{
					((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
					if(bstrText)
					{
						pItem->pStrPre = XUtil::tcsdupFromBstr(bstrText);
						::SysFreeString(bstrText);
					}
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"lastpre", &pNodeEx);
				if(pNodeEx)
				{
					((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
					if(bstrText)
					{
						pItem->pStrPreLast = XUtil::tcsdupFromBstr(bstrText);
						::SysFreeString(bstrText);
					}
				}
				pGridDataSvr->m_pContentSvr->SetItemData(col,row,pItem);
			}
		}
		
		IXMLDOMElement * pElementReport;
		IXMLDOMNode	   * pNodeReport;
		IXMLDOMElement * pElementBand;
		IXMLDOMNode	   * pNodeBand;
		_variant_t	var;

		pSheetElement->selectSingleNode(L"report",&pNodeReport);
		if(pNodeReport)
		{
			pGridDataSvr->m_enView = CGridDataSvr::envGridReportDesign;
			_variant_t pText;
			pSheetElement->getAttribute(L"print",&pText);
			if(pText.vt !=VT_NULL)
				pGridDataSvr->m_enView = CGridDataSvr::envPrintDesign;

			pNodeReport->QueryInterface(IID_IXMLDOMElement,(void **) & pElementReport);
			pNodeReport->Release();

			pGridDataSvr->m_ReportBand->m_ReportBand.bValid = true;
			pElementReport->getAttribute(L"left",&var);
			pGridDataSvr->m_ReportBand->m_ReportBand.left = var;
			pElementReport->getAttribute(L"right",&var);
			pGridDataSvr->m_ReportBand->m_ReportBand.right = var;

			pElementReport->selectSingleNode(L"PageHeader",&pNodeBand);
			if(pNodeBand)
			{
				pNodeBand->QueryInterface(IID_IXMLDOMElement,(void **) & pElementBand);
				pNodeBand->Release();
				pGridDataSvr->m_ReportBand->m_ReportBand.PageHeader.bValid = true;
				pElementBand->getAttribute(L"startRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.PageHeader.startRow = var;
				pElementBand->getAttribute(L"endRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.PageHeader.endRow = var;
				pElementBand->Release();
			}

			pElementReport->selectSingleNode(L"Header",&pNodeBand);
			if(pNodeBand)
			{
				pNodeBand->QueryInterface(IID_IXMLDOMElement,(void **) & pElementBand);
				pNodeBand->Release();
				pGridDataSvr->m_ReportBand->m_ReportBand.Header.bValid = true;
				pElementBand->getAttribute(L"startRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Header.startRow = var;
				pElementBand->getAttribute(L"endRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Header.endRow = var;
				pElementBand->Release();
			}

			pElementReport->selectSingleNode(L"Title",&pNodeBand);
			if(pNodeBand)
			{
				pNodeBand->QueryInterface(IID_IXMLDOMElement,(void **) & pElementBand);
				pNodeBand->Release();
				pGridDataSvr->m_ReportBand->m_ReportBand.Title.bValid = true;
				pElementBand->getAttribute(L"startRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Title.startRow = var;
				pElementBand->getAttribute(L"endRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Title.endRow = var;
				pElementBand->Release();
			}

			pElementReport->selectSingleNode(L"Detail",&pNodeBand);
			if(pNodeBand)
			{
				pNodeBand->QueryInterface(IID_IXMLDOMElement,(void **) & pElementBand);
				pNodeBand->Release();
				pGridDataSvr->m_ReportBand->m_ReportBand.Detail.bValid = true;
				pElementBand->getAttribute(L"startRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Detail.startRow = var;
				pElementBand->getAttribute(L"endRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Detail.endRow = var;
				pElementBand->Release();
			}

			pElementReport->selectSingleNode(L"Summury",&pNodeBand);
			if(pNodeBand)
			{
				pNodeBand->QueryInterface(IID_IXMLDOMElement,(void **) & pElementBand);
				pNodeBand->Release();
				pGridDataSvr->m_ReportBand->m_ReportBand.Summury.bValid = true;
				pElementBand->getAttribute(L"startRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Summury.startRow = var;
				pElementBand->getAttribute(L"endRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Summury.endRow = var;
				pElementBand->Release();
			}

			pElementReport->selectSingleNode(L"Footer",&pNodeBand);
			if(pNodeBand)
			{
				pNodeBand->QueryInterface(IID_IXMLDOMElement,(void **) & pElementBand);
				pNodeBand->Release();
				pGridDataSvr->m_ReportBand->m_ReportBand.Footer.bValid = true;
				pElementBand->getAttribute(L"startRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Footer.startRow = var;
				pElementBand->getAttribute(L"endRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.Footer.endRow = var;
				pElementBand->Release();
			}

			pElementReport->selectSingleNode(L"PageFooter",&pNodeBand);
			if(pNodeBand)
			{
				pNodeBand->QueryInterface(IID_IXMLDOMElement,(void **) & pElementBand);
				pNodeBand->Release();
				pGridDataSvr->m_ReportBand->m_ReportBand.PageFooter.bValid = true;
				pElementBand->getAttribute(L"startRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.PageFooter.startRow = var;
				pElementBand->getAttribute(L"endRow",&var);
				pGridDataSvr->m_ReportBand->m_ReportBand.PageFooter.endRow = var;
				pElementBand->Release();
			}
			pElementReport->Release();
			//pGridDataSvr->m_enView = CGridDataSvr::envGridReportDesign;
		}
		
		//pGridDataSvr->AddChild(5,10);

		return 1;
	}

	int XCellSerializeSvr::LoadSheetData20(IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr)
	{
		CGridDataSvr * pGridDataSvr = (CGridDataSvr *)pDataSvr;

		IXMLDOMDocumentPtr pDoc; 
		IXMLDOMElementPtr  pXmlRoot ; 
		IXMLDOMElementPtr  xmlpNode ; 
		IXMLDOMProcessingInstructionPtr pi;
		IXMLDOMElementPtr pElement;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMNodePtr pColNode;
		IXMLDOMElementPtr pStyleSheet;
		IXMLDOMElementPtr pFonts;
		IXMLDOMElementPtr pFont;
		IXMLDOMElementPtr pSpans;
		IXMLDOMElementPtr pCells;
		IXMLDOMElementPtr pCell;
		IXMLDOMElementPtr pRow;
		IXMLDOMElementPtr pStyles;
		IXMLDOMElementPtr pStyle;
		IXMLDOMElementPtr pBand;
		IXMLDOMElementPtr pBandRows;
		IXMLDOMElementPtr pBandCols;
		IXMLDOMElementPtr pBandRow;
		IXMLDOMElementPtr pBandCol;
		IXMLDOMNodeListPtr	pNodeList;
		IXMLDOMNodeListPtr	pColNodeList;

		BSTR bstrTag;
		pSheetElement->get_tagName(&bstrTag);
		if(bstrTag)
		{
			if(::_wcsicmp(bstrTag,L"hxcell") != 0)
			{
				::SysFreeString(bstrTag);
				return 0;
			}
			::SysFreeString(bstrTag);
		}else
			return 0;

		pSheetElement->selectNodes(L"band/rows/row",&pNodeList);
		long	len=-1,rowHeight=0;
		XCROW	row;
		_variant_t	val;

		pNodeList->get_length(&len);
		int m;
		for(m=0;m<len;m++)
		{
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"rowid",&val);
			row = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"height",&val);
			if(val.vt != VT_NULL) 
			{
				rowHeight = val; 
				if (row>0) pGridDataSvr->m_pAxisSvr->SetRowHeight(row,rowHeight);
			}
			/*
			std::wstring	cs;
			((IXMLDOMElementPtr)pNode)->selectSingleNode(L"prestr", &pNodeEx);
			if(pNodeEx)
			{
				BSTR bstrText = NULL;
				((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
				cs = bstrText;
				pItem->FPreStr = new TCHAR[cs.GetLength()+1];
				::lstrcpy(pItem->FPreStr,cs);
				pItem->mask = STI_PRESTR;
				this->SetCellsItem(0,row,pItem);
				delete pItem->FPreStr;
				pItem->FPreStr = NULL;

				::SysFreeString(bstrText);
			}
			((IXMLDOMElementPtr)pNode)->selectSingleNode(L"lastpre", &pNodeEx);
			if(pNodeEx)
			{
				BSTR bstrText = NULL;
				((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);

				cs = bstrText;
				pItem->FLastPre = new TCHAR[cs.GetLength()+1];
				::lstrcpy(pItem->FLastPre,cs);
				pItem->mask = STI_LASTPRE;
				this->SetCellsItem(0,row,pItem);
				delete pItem->FLastPre;
				pItem->FPreStr = NULL;

				::SysFreeString(bstrText);
			}
			*/
		}

		pSheetElement->selectNodes(L"band/cols/col",&pNodeList);
		long colWidth=0;
		int	col;
		pNodeList->get_length(&len);
		for(m=0;m<len;m++)
		{
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"colid",&val);
			col = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"width",&val);
			if(val.vt != VT_NULL)
			{
				colWidth = val; 
				if(col > 0 )
					pGridDataSvr->m_pAxisSvr->SetColWidth(col,colWidth);
			}
			/*
			std::wstring	cs;
			((IXMLDOMElementPtr)pNode)->selectSingleNode(L"prestr", &pNodeEx);
			if(pNodeEx)
			{
			BSTR bstrText = NULL;
			((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);
			cs = bstrText;
			pItem->FPreStr = new TCHAR[cs.GetLength()+1];
			::lstrcpy(pItem->FPreStr,cs);
			pItem->mask = STI_PRESTR;
			this->SetCellsItem(col,0,pItem);
			delete pItem->FPreStr;
			pItem->FPreStr = NULL;

			::SysFreeString(bstrText);
			}
			((IXMLDOMElementPtr)pNode)->selectSingleNode(L"lastpre", &pNodeEx);
			if(pNodeEx)
			{
			BSTR bstrText = NULL;
			((IXMLDOMElementPtr)pNodeEx)->get_text(&bstrText);

			cs = bstrText;
			pItem->FLastPre = new TCHAR[cs.GetLength()+1];
			::lstrcpy(pItem->FLastPre,cs);
			pItem->mask = STI_LASTPRE;
			this->SetCellsItem(0,col,pItem);
			delete pItem->FLastPre;
			pItem->FPreStr = NULL;

			::SysFreeString(bstrText);
			}*/
		}

		pSheetElement->selectNodes(L"spans/span",&pNodeList);
		pNodeList->get_length(&len);
		RECT	rect;
		for(m=0;m<len;m++)
		{
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"left",&val);
			rect.left = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"right",&val);
			rect.right = val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"top",&val);
			rect.top= val;
			((IXMLDOMElementPtr)pNode)->getAttribute(L"bottom",&val);
			rect.bottom = val;
			pGridDataSvr->m_pSpanSvr->SpanRects.push_back(rect);
		}

		/*
		for(int ifont=0;ifont<=m_Fonts.GetUpperBound();ifont++)
		{
			if(m_Fonts[ifont])
			{
				m_Fonts[ifont]->DeleteObject();
				delete m_Fonts[ifont];
			}
		}
		m_Fonts.RemoveAll();
		pSheetElement->selectNodes(L"stylesheet/fonts/font",&pNodeList);
		pNodeList->get_length(&len);
		for(m=0;m<len;m++)
		{
			BSTR bstrText = NULL;
			std::wstring	cs;
			int curPos = 0;
			int	pos = 0;
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->get_text(&bstrText);
			cs = bstrText;

			std::wstring	resToken;
			std::wstring	resItem,resValue;
			int tokenIndex=0;

			if(cs.IsEmpty())
				resToken="";
			else
			{
				curPos =cs.Find(';',tokenIndex);
				if(curPos >=0)
					resToken = cs.Mid(tokenIndex,curPos - tokenIndex );
				else
					resToken ="";
				tokenIndex = curPos + 1;
			}
			//resToken= cs.Tokenize(";",curPos);

			LOGFONT	logFont;
			CFont * pFont = NULL;
			pFont = new CFont();
			_variant_t	var;
			while (resToken != "")
			{
				pos = resToken.Find(':');
				if(pos > 0 )
				{
					resItem = resToken.Left(pos);
					resItem.TrimLeft("\r\n\t ");
					resValue = resToken.Mid(pos + 1);
					resValue.TrimRight("\r\n\t ");
					var = resValue;
					if(resItem=="facename")
						lstrcpy(logFont.lfFaceName,resValue);
					if(resItem=="charset")
						logFont.lfCharSet = var;
					if(resItem=="height")
						logFont.lfHeight = var;
					if(resItem=="width")
						logFont.lfWidth = var;
					if(resItem=="escapement")
						logFont.lfEscapement = var;
					if(resItem=="orientation")
						logFont.lfOrientation = var;
					if(resItem=="weight")
						logFont.lfWeight = var;
					if(resItem=="italic")
						logFont.lfItalic = var;
					if(resItem=="underline")
						logFont.lfUnderline= var;
					if(resItem=="strikeout")
						logFont.lfStrikeOut= var;
					if(resItem=="outprecision")
						logFont.lfOutPrecision= var;
					if(resItem=="clipprecision")
						logFont.lfClipPrecision= var;
					if(resItem=="quality")
						logFont.lfQuality= var;
					if(resItem=="pitchandfamily")
						logFont.lfPitchAndFamily= var;
				}
				//resToken= cs.Tokenize(";",curPos);
				curPos =cs.Find(';',tokenIndex);
				if(curPos >=0)
					resToken = cs.Mid(tokenIndex,curPos - tokenIndex );
				else
					resToken ="";
				tokenIndex = curPos + 1;
			};
			pFont->CreateFontIndirect(&logFont);
			m_Fonts.Add(pFont);

			::SysFreeString(bstrText);
		}

		std::wstringArray	csaStyle;
		std::wstringArray	csaStyleName;
		pSheetElement->selectNodes(L"stylesheet/styles/style",&pNodeList);
		pNodeList->get_length(&len);
		for(m=0;m<len;m++)
		{
			BSTR bstrText = NULL;
			std::wstring	cs;
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->get_text(&bstrText);
			csaStyle.Add((std::wstring)bstrText);
			::SysFreeString(bstrText);

			((IXMLDOMElementPtr)pNode)->getAttribute(L"name",&val);
			csaStyleName.Add((std::wstring)val);
		}
		*/

		pSheetElement->selectNodes(L"cells/row",&pNodeList);
		pNodeList->get_length(&len);
		long	colLen = 0;
		for(m=0;m<len;m++)
		{
			BSTR bstrText = NULL;
			//std::wstring	cs,classname;
			pNodeList->get_item(m,&pNode);
			((IXMLDOMElementPtr)pNode)->getAttribute(L"rowindex",&val);
			row = val;

			pNode->selectNodes(L"cell",&pColNodeList);
			pColNodeList->get_length(&colLen);
			for(int n=0;n<colLen;n++)
			{
				pColNodeList->get_item(n,&pColNode);
				((IXMLDOMElementPtr)pColNode)->getAttribute(L"colindex",&val);
				col = val;

				((IXMLDOMElementPtr)pColNode)->getAttribute(L"class",&val);
				if(val.vt != VT_NULL)
				{
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"attrib", &pNodeEx);
				if(pNodeEx)
				{
				}

				_variant_t	value;
				((IXMLDOMElementPtr)pColNode)->getAttribute(L"celltype",&value);
				if(value.vt !=VT_NULL)
				{
				}

				((IXMLDOMElementPtr)pColNode)->getAttribute(L"edittype",&value);
				if(value.vt !=VT_NULL)
				{
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"formula", &pNodeEx);
				if(pNodeEx)
				{
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"note", &pNodeEx);
				if(pNodeEx)
				{
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"name", &pNodeEx);
				if(pNodeEx)
				{
				}

				((IXMLDOMElementPtr)pColNode)->selectSingleNode(L"text", &pNodeEx);
				if(pNodeEx)
				{
					((IXMLDOMElementPtr)pNodeEx)->get_text
						(&bstrText);
					if(bstrText)
					{
						LPTSTR	pStr = XUtil::tcsdupFromBstr(bstrText);
						pGridDataSvr->m_pContentSvr->SetItemString(col,row,pStr);
						::SysFreeString(bstrText);
					}
				}
			}
		}
		return 1;
	}

	int XCellSerializeSvr::LoadSheetTable(IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr)
	{
		int sRow = 1;
		int sCol = 1;

		CGridDataSvr * pGridDataSvr = (CGridDataSvr *)pDataSvr;

		IXMLDOMNode * pNode = 0;
		IXMLDOMElement * pTable = 0;

		pSheetElement->selectSingleNode(L"//*[name()='table']",&pNode); 
		if(!pNode) return 1;


		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pTable);
		pNode->Release();
		if(!pTable) return 1;

		bool readContent = true;
		/*pTable->selectSingleNode(L"xml/hxcell",&pNode);
		if(pNode)
		{
			readContent = false;
			pNode->Release();
		}*/

		bool	tableBorder = false;
		_variant_t val;
		_variant_t style;

		pTable->getAttribute(L"border",&val);
		if(val.bstrVal)
		{
			if(val.bstrVal[0]=='1') tableBorder = true;
		}
		if(!tableBorder)
		{
			pTable->getAttribute(L"style",&style);
			if(style.bstrVal && ::wcsstr(style.bstrVal,L"border:solid")) tableBorder = true;
		}
		pTable->getAttribute(L"firststr",&val);
		if(val.bstrVal && val.bstrVal[0])
		{
			PStrItem pItem = pGridDataSvr->m_pContentSvr->GetItemData(0,0);
			if(!pItem)
			{
				pItem = new StrItem();
				pGridDataSvr->m_pContentSvr->SetItemData(0,0,pItem);
			}
			if(pItem->pStrPre)delete pItem->pStrPre;
			pItem->pStrPre = _wcsdup(val.bstrVal);
		}
		pTable->getAttribute(L"laststr",&val);
		if(val.bstrVal && val.bstrVal[0])
		{
			PStrItem pItem = pGridDataSvr->m_pContentSvr->GetItemData(0,0);
			if(!pItem)
			{
				pItem = new StrItem();
				pGridDataSvr->m_pContentSvr->SetItemData(0,0,pItem);
			}
			if(pItem->pStrPreLast)delete pItem->pStrPreLast;
			pItem->pStrPreLast = _wcsdup(val.bstrVal);
		}
		
		int colCount = 0;
		IXMLDOMNodeList * pRows = 0;
		IXMLDOMNodeList * pCols = 0;
		IXMLDOMElement * pRow = 0;
		IXMLDOMElement * pCol = 0;
		long rowCount = 0;
		pTable->selectNodes(L"//*[name()='tr' or name()='TR']",&pRows);
		pRows->get_length(&rowCount);

		int row = -1;
		for(long i=0;i<rowCount;i++)
		{
			row++;
			pRows->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pRow);

			int xlrowspan = 0;
			pRow->getAttribute(L"style",&val);
			if(val.bstrVal && wcsstr(val.bstrVal,L"mso-xlrowspan:"))
			{
				wchar_t* pStr = wcsstr(val.bstrVal,L"mso-xlrowspan:")
						+ wcslen(L"mso-xlrowspan:");
				while(*pStr && *pStr!=';')
				{
					xlrowspan = xlrowspan * 10 + *pStr - L'0';
					pStr++;
				}
			}

			pRow->getAttribute(L"height",&val);
			if(val.bstrVal && val.bstrVal[0])
			{
				if(xlrowspan <=1)
					pGridDataSvr->m_pAxisSvr->SetRowHeight(row + sRow,val);
				else
				{
					for(int m=0;m<xlrowspan;m++)
					{
						pGridDataSvr->m_pAxisSvr->SetRowHeight(row + sRow + m,val);
					}
					sRow += xlrowspan;
					continue;
				}
			}

			int col = -1;
			int startcol = sCol;
			int rowspan = 1;
			int colspan = 1;
			long columnCount = 0;
			pRow->selectNodes(L"*[name()='td' or name()='TD' or name()='th' or name()='TH']",&pCols);
			pCols->get_length(&columnCount);
			for(long k=0; k<columnCount; k++)
			{
				col++;

				//found startcolumn
				for(;startcol< colCount; startcol++)
				{
					if(!pGridDataSvr->m_pSpanSvr->isSpaned(startcol,row + sRow))break;
				}

				pCols->get_item(k,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void**)&pCol);
				pNode->Release();

				pCol->getAttribute(L"rowspan",&val);
				rowspan = 1;
				if(val.bstrVal && val.bstrVal[0])
					rowspan = val;
				pCol->getAttribute(L"colspan",&val);
				colspan = 1;
				if(val.bstrVal && val.bstrVal[0])
					colspan = val;

				pCol->getAttribute(L"style",&style);
				if(rowspan>1 ||colspan>1)
				{
					TGridRect rect;
					rect.left = startcol;
					rect.top = row + sRow ;
					rect.right = startcol  + colspan - 1;
					rect.bottom = row + sRow + rowspan - 1;

					pGridDataSvr->m_pSpanSvr->Merge(rect);

					//if(::strstr(val,"mso-ignore:colspan"))unColSpans.Add(m_spans.GetUpperBound());
					//if(::strstr(val,"mso-ignore:rowspan"))unRowSpans.Add(m_spans.GetUpperBound());
				}
				
				std::wstring styleext;
				IXMLDOMNode * pNode=0;
				
				pCol->selectSingleNode(L".//*[name()='STRONG' or name()='strong' or name()='Strong']",&pNode);
				if(pNode)styleext += (std::wstring)(styleext==L""?L"":L";") + L"font-weight:bold";
				if(pNode) pNode->Release();

				pCol->selectSingleNode(L".//*[name()='EM' or name()='em']",&pNode);
				if(pNode)styleext += (std::wstring)(styleext==L""?L"":L";") + L"font-style:italic";
				if(pNode) pNode->Release();

				pCol->selectSingleNode(L".//*[name()='FONT' or name()='Font' or name()='font']/@*[name()='SIZE' or name()='Size' or name()='size']",&pNode);
				if(pNode)
				{
					BSTR str;
					pNode->get_text(&str);
					if(str)
					{
						if(str[0]=='1') styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-size:50%";
						if(str[0]=='2') styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-size:75%";
						if(str[0]=='3') styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-size:100%";
						if(str[0]=='4') styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-size:125%";
						if(str[0]=='5') styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-size:150%"; 
						if(str[0]=='6') styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-size:200%";
						if(str[0]=='6') styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-size:250%";
						::SysFreeString(str);
					}
					pNode->Release();
				}

				pCol->selectSingleNode(L".//*[name()='FONT' or name()='Font' or name()='font']/@*[name()='FACE' or name()='Face' or name()='face']",&pNode);
				if(pNode)
				{
					BSTR str;
					pNode->get_text(&str);
					if(str)
					{
						styleext +=(std::wstring)(styleext==L""?L"":L";") + L"font-family:"+str;
					}
					pNode->Release();
				}

				pCol->selectSingleNode(L".//@*[name()='ALIGN' or name()='Align' or name()='align']",&pNode);
				if(pNode)
				{
					BSTR str;
					pNode->get_text(&str);
					if(str)
					{
						 styleext +=(std::wstring)(styleext==L""?L"":L";") + L"text-align:" + str;
						 ::SysFreeString(str);
					}
				}

				if(style.bstrVal && style.bstrVal[0])
				{
					if(styleext==L"") 
						styleext += style.bstrVal;
					else 
						styleext +=(std::wstring)L";"+ style.bstrVal;
				}
				if(styleext != L"")
				{
					XUtil::Css::TBoxStyle * pStyle = XUtil::xuCCssStyle::ParseCssStyle(styleext.c_str());
					pGridDataSvr->GetStyleSvr()->SetCellCssStyle(startcol,row + sRow,pStyle);
				}

				//bkcolor
				/*long	bkColor = 0;
				if(::strstr(val,"background-color:"))
				{
					val+=";";
					ls_style = ::strstr(val,"background-color:")+strlen("background-color:");
					ls_style = ls_style.Left(ls_style.Find(";"));
					if(ls_style!="" && !ls_style.IsEmpty())
					{
						ls_style = ls_style.Mid(5,2) + ls_style.Mid(3,2) + ls_style.Mid(1,2);
						bkColor = this->Hex(ls_style);
					}
				}*/

				//set content
				if(readContent)
				{
					BSTR bstr = 0;
					pCol->get_text(&bstr);
					int pos = 0;
					while(bstr[pos] && (bstr[pos]==' ' || bstr[pos]=='\r' || bstr[pos]=='\n' || bstr[pos]=='\t'))pos++;
					if(bstr && bstr[pos])
					{
						PStrItem pItem = pGridDataSvr->m_pContentSvr->GetItemData(startcol, row + sRow);
						if(!pItem)
						{
							pItem = new StrItem();
							pGridDataSvr->m_pContentSvr->SetItemData(startcol,row + sRow,pItem);
						}
						if(pItem->pStrContent)delete pItem->pStrContent;
						pItem->pStrContent = _wcsdup(bstr+pos);
						SysFreeString(bstr);
					}
				}
				//set colwidth
				pCol->getAttribute(L"width",&val);
				if(val.bstrVal && val.bstrVal[0])
					pGridDataSvr->m_pAxisSvr->SetColWidth(startcol, val);

				//set row height
				pCol->getAttribute(L"height",&val);
				if(val.bstrVal && val.bstrVal[0])
					pGridDataSvr->m_pAxisSvr->SetRowHeight(row + sRow, val);

				//set border
				//set color
				//set font
				//set align

				//adjust startcol
				startcol += colspan;
				pCol->Release();
			}

			pCols->Release();

			//adjust start columnCount
			if(colCount <startcol - 1) colCount = startcol - 1;

			pRow->getAttribute(L"firststr",&val);
			if(val.bstrVal && val.bstrVal[0])
			{
				PStrItem pItem = pGridDataSvr->m_pContentSvr->GetItemData(0,row + sRow);
				if(!pItem)
				{
					pItem = new StrItem();
					pGridDataSvr->m_pContentSvr->SetItemData(0,row + sRow,pItem);
				}
				if(pItem->pStrPre)delete pItem->pStrPre;
				pItem->pStrPre = _wcsdup(val.bstrVal);
			}
			pRow->getAttribute(L"laststr",&val);
			if(val.bstrVal && val.bstrVal[0])
			{
				PStrItem pItem = pGridDataSvr->m_pContentSvr->GetItemData(0,row + sRow);
				if(!pItem)
				{
					pItem = new StrItem();
					pGridDataSvr->m_pContentSvr->SetItemData(0,row + sRow,pItem);
				}
				if(pItem->pStrPreLast)delete pItem->pStrPreLast;
				pItem->pStrPreLast = _wcsdup(val.bstrVal);
			}
			pRow->Release();
		}
		pRows->Release();
		//draw obj

		//data
		IXMLDOMNodeList * pItems = 0;
		IXMLDOMElement * pEleItem = 0;
		long countItems = 0;
		pTable->selectNodes(L"*[name()='xml']/*[name()='hxcell']/*[name()='item']",&pItems);
		pItems->get_length(&countItems);
		for(long i=0;i<countItems;i++)
		{
			pItems->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pEleItem);
			pNode->Release();

			BSTR bstr = 0;

			pEleItem->selectSingleNode(L"*[name()='row']",&pNode);
			pNode->get_text(&bstr);
			int row = _wtol(bstr);
			pNode->Release();
			::SysFreeString(bstr);

			pEleItem->selectSingleNode(L"*[name()='col']",&pNode);
			pNode->get_text(&bstr);
			int col = _wtol(bstr);
			pNode->Release();
			::SysFreeString(bstr);
			
			PStrItem pItem = pGridDataSvr->m_pContentSvr->GetItemData(col,row);
			if(!pItem)
			{
				pItem = new StrItem();
				pGridDataSvr->m_pContentSvr->SetItemData(col,row,pItem);
			}
			
			pEleItem->selectSingleNode(L"*[name()='前缀']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				if(pItem->pStrPre)delete pItem->pStrPre;
				pItem->pStrPre = _wcsdup(bstr);
				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}

			pEleItem->selectSingleNode(L"*[name()='后缀']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();
				if(pItem->pStrPreLast)delete pItem->pStrPreLast;
				pItem->pStrPreLast = _wcsdup(bstr);
				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}

			//skip 链接
	
			pEleItem->selectSingleNode(L"*[name()='公式']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();
				if(pItem->pStrFormula)delete pItem->pStrFormula;

				pItem->pStrFormula = _wcsdup(bstr);
				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}
			
			pEleItem->selectSingleNode(L"*[name()='备注']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();
				if(pItem->pNote)delete pItem->pNote;
				pItem->pNote = _wcsdup(bstr);
				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}

			pEleItem->selectSingleNode(L"*[name()='建议']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();
				if(pItem->pStrAdvance)delete pItem->pStrAdvance;
				pItem->pStrAdvance = _wcsdup(bstr);
				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}

			pEleItem->selectSingleNode(L"*[name()='其他']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();
				if(pItem->pStrXml)delete pItem->pStrXml;
				pItem->pStrXml = _wcsdup(bstr);
				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}

			//new version
			pEleItem->selectSingleNode(L"*[name()='内容']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();
				if(pItem->pStrContent)delete pItem->pStrContent;
				pItem->pStrContent = _wcsdup(bstr);
				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}

			pEleItem->selectSingleNode(L"*[name()='数据列']",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();
				if(pItem->pStrContent)delete pItem->pStrContent;

				//found text
				int len = (int)wcslen(bstr);
				int pos = 0;
				int cc=0;
				while(bstr[pos])
				{
					if(bstr[pos]=='<')
					{
						bstr[pos]=0;
						pos++;
						while(bstr[pos] && bstr[pos]!='>')
						{
							bstr[pos]=0;
							pos++;
						}
						if(!bstr[pos])break;
						bstr[pos]=0;
					}else
						cc++;
					pos++;
				}
				wchar_t* pStr = new wchar_t[cc+1];
				int cp  = 0;
				pos = 0;
				while(pos<len)
				{
					if(bstr[pos])
					{
						pStr[cp] = bstr[pos];
						cp++;
					}
					pos++;
				}
				pStr[cp] = 0;
				if(cp!=cc)
				{
					int k=cc;
				}
				pItem->pStrContent = pStr;

				SysFreeString(bstr);
				pEleItem->Release();
				continue;
			}
		}
		pTable->Release();

		return 1;
	}

	int XCellSerializeSvr::SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)
	{
		CGridDataSvr * pGridDataSvr = (CGridDataSvr *)pDataSvr;
		pElement->setAttribute(L"version",(_variant_t)L"3.0");
		pElement->setAttribute(L"editer",(_variant_t)L"xcell");

		if(pGridDataSvr->GetSchemaSource())
				pElement->setAttribute(L"schemasource",(_variant_t)pGridDataSvr->GetSchemaSource());
		IXMLDOMElement * pSheetElement;
		pXmlDoc->createElement(L"stylesheet",&pSheetElement);
		pDataSvr->m_pStyleSvr->m_pCssStyle->SetStyleSheet(pSheetElement,L"item");

		pElement->appendChild(pSheetElement,NULL);
		pSheetElement->Release();

		LPColCssClass lp;
		int nIndex;
		TCHAR strClass[255];
		IXMLDOMElement * pStyleElement;
		IXMLDOMElement * pStyleColElement;
		IXMLDOMElement * pStyleItemElement;
		pXmlDoc->createElement(L"cellstyles",&pStyleElement);
		for(int i=0;i<pGridDataSvr->GetStyleSvr()->m_pCssClass->size();i++)
		{
			lp = (*pGridDataSvr->GetStyleSvr()->m_pCssClass)[i];
			if(lp->GetEntryCount() >=2)
			{
				pXmlDoc->createElement(L"col",&pStyleColElement);
				pStyleColElement->setAttribute(L"colindex",(_variant_t)i);
				for(int k=0;k<lp->GetEntryCount() - 1;k++)
				{
					pXmlDoc->createElement(L"item",&pStyleItemElement);
					pStyleItemElement->setAttribute(L"rowend",(_variant_t)lp->GetDataEntry(k).nEnd);
					nIndex = lp->GetDataEntry(k).aValue;
					pGridDataSvr->GetStyleSvr()->m_pCssStyle->GetClassNameByIndex(nIndex,strClass);
					pStyleItemElement->setAttribute(L"class",(_variant_t)strClass);
					pStyleColElement->appendChild(pStyleItemElement,NULL);
					pStyleItemElement->Release();
				}
				pStyleElement->appendChild(pStyleColElement,NULL);
				pStyleColElement->Release();
			}
		}
		pElement->appendChild(pStyleElement,NULL);
		pStyleElement->Release();


		IXMLDOMElement * pAxisElement;
		IXMLDOMElement * pAxisRowElement;
		IXMLDOMElement * pAxisColElement;
		IXMLDOMElement * pAxisItemElement;
		pXmlDoc->createElement(L"axis",&pAxisElement);
		pGridDataSvr->m_pAxisSvr->m_pColWidths->GetEntryCount();
		if(pGridDataSvr->m_pAxisSvr->m_pColWidths->GetEntryCount() >=2)
		{
			pXmlDoc->createElement(L"col",&pAxisColElement);
			for(int k=0;k<pGridDataSvr->m_pAxisSvr->m_pColWidths->GetEntryCount() - 1;k++)
			{
				pXmlDoc->createElement(L"item",&pAxisItemElement);
				pAxisItemElement->setAttribute(L"colend",(_variant_t)pGridDataSvr->m_pAxisSvr->m_pColWidths->GetDataEntry(k).nEnd);
				pAxisItemElement->setAttribute(L"width",(_variant_t)pGridDataSvr->m_pAxisSvr->m_pColWidths->GetDataEntry(k).aValue);
				pAxisColElement->appendChild(pAxisItemElement,NULL);
				pAxisItemElement->Release();
			}
			pAxisElement->appendChild(pAxisColElement,NULL);
			pAxisColElement->Release();
		}
		pGridDataSvr->m_pAxisSvr->m_pRowHeights->GetEntryCount();
		if(pGridDataSvr->m_pAxisSvr->m_pRowHeights->GetEntryCount() >=2)
		{
			pXmlDoc->createElement(L"row",&pAxisRowElement);
			for(int k=0;k<pGridDataSvr->m_pAxisSvr->m_pRowHeights->GetEntryCount() - 1;k++)
			{
				pXmlDoc->createElement(L"item",&pAxisItemElement);
				pAxisItemElement->setAttribute(L"rowend",(_variant_t)pGridDataSvr->m_pAxisSvr->m_pRowHeights->GetDataEntry(k).nEnd);
				pAxisItemElement->setAttribute(L"height",(_variant_t)pGridDataSvr->m_pAxisSvr->m_pRowHeights->GetDataEntry(k).aValue);
				pAxisRowElement->appendChild(pAxisItemElement,NULL);
				pAxisItemElement->Release();
			}
			pAxisElement->appendChild(pAxisRowElement,NULL);
			pAxisRowElement->Release();
		}
		pElement->appendChild(pAxisElement,NULL);
		pAxisElement->Release();

		IXMLDOMElement * pSpansElement;
		IXMLDOMElement * pSpanElement;
		pGridDataSvr->m_pSpanSvr->RefreshSpans();
		pXmlDoc->createElement(L"spans",&pSpansElement);
		for(int i=0;i<pGridDataSvr->m_pSpanSvr->SpanRects.size();i++)
		{
			RECT rect=pGridDataSvr->m_pSpanSvr->SpanRects[i];
			pXmlDoc->createElement(L"span",&pSpanElement);
			pSpanElement->setAttribute(L"left",(_variant_t)rect.left);
			pSpanElement->setAttribute(L"top",(_variant_t)rect.top);
			pSpanElement->setAttribute(L"right",(_variant_t)rect.right);
			pSpanElement->setAttribute(L"bottom",(_variant_t)rect.bottom);
			pSpansElement->appendChild(pSpanElement,NULL);
			pSpanElement->Release();
		}
		pElement->appendChild(pSpansElement,NULL);
		pSpansElement->Release();

		IXMLDOMElement * pCellsElement;
		IXMLDOMElement * pRowElement;
		IXMLDOMElement * pCellElement;
		IXMLDOMElement * pItemElement;
		IXMLDOMCDATASection * pCDData;
		pXmlDoc->createElement(L"cells",&pCellsElement);
		int ACol,ARow;
		PStrItem	pItem;
		ARow = pGridDataSvr->m_pContentSvr->GetValidRow();
		for(int nRow=0;nRow <= ARow;nRow++)
		{
			ACol = pGridDataSvr->m_pContentSvr->GetValidCol(nRow);
			if(ACol >=0)
			{
				pXmlDoc->createElement(L"row",&pRowElement);
				pRowElement->setAttribute(L"rowindex",_variant_t(nRow));
				for(int nCol=0;nCol <= ACol;nCol++)
				{
					pItem = pGridDataSvr->m_pContentSvr->GetItemData(nCol,nRow);
					if(!pItem) continue;
					pXmlDoc->createElement(L"cell",&pCellElement);
					pCellElement->setAttribute(L"coltype",_variant_t(pItem->eDTItem));
					pCellElement->setAttribute(L"colindex",_variant_t(nCol));
					if(pItem->pStrGroup)
						pCellElement->setAttribute(L"group",_variant_t(pItem->pStrGroup));
					if(pItem->pStrColumn)
						pCellElement->setAttribute(L"column",_variant_t(pItem->pStrColumn));
					if(pItem->pStrContent)
					{
						pXmlDoc->createElement(L"text",&pItemElement);
						if(_tcschr(pItem->pStrContent,'\r'))
						{
							pXmlDoc->createCDATASection(pItem->pStrContent,&pCDData);
							pItemElement->appendChild(pCDData,NULL);
							pCDData->Release();
						}else
							pItemElement->put_text(pItem->pStrContent);
						pCellElement->appendChild(pItemElement,NULL);
						pItemElement->Release();
					}
					if(pItem->pStrFormula)
					{
						pXmlDoc->createElement(L"formula",&pItemElement);
						pItemElement->put_text(pItem->pStrFormula);
						pCellElement->appendChild(pItemElement,NULL);
						pItemElement->Release();
					}
					if(pItem->pStrXml)
					{
						pXmlDoc->createElement(L"xml",&pItemElement);
						pItemElement->put_text(pItem->pStrXml);
						pCellElement->appendChild(pItemElement,NULL);
						pItemElement->Release();
					}
					if(pItem->pStrText)
					{
						pXmlDoc->createElement(L"display",&pItemElement);
						pItemElement->put_text(pItem->pStrXml);
						pCellElement->appendChild(pItemElement,NULL);
						pItemElement->Release();
					}
					if(pItem->pStrPre)
					{
						pXmlDoc->createElement(L"prestr",&pItemElement);
						pItemElement->put_text(pItem->pStrPre);
						pCellElement->appendChild(pItemElement,NULL);
						pItemElement->Release();
					}
					if(pItem->pStrPreLast)
					{
						pXmlDoc->createElement(L"lastpre",&pItemElement);
						pItemElement->put_text(pItem->pStrPreLast);
						pCellElement->appendChild(pItemElement,NULL);
						pItemElement->Release();
					}
					pRowElement->appendChild(pCellElement,NULL);
					pCellElement->Release();
				}
				pCellsElement->appendChild(pRowElement,NULL);
				pRowElement->Release();
			}
		}
		pElement->appendChild(pCellsElement,NULL);
		pCellsElement->Release();
		

		if(pGridDataSvr->m_enView ==CGridDataSvr::envGridReportDesign || pGridDataSvr->m_enView ==CGridDataSvr::envPrintDesign)
		{
			if(pGridDataSvr->m_enView ==CGridDataSvr::envGridReportDesign)
				pElement->setAttribute(L"report",(_variant_t)L"yes");
			else if(pGridDataSvr->m_enView ==CGridDataSvr::envPrintDesign)
				pElement->setAttribute(L"print",(_variant_t)L"yes");
			CReportBandSvr::TReportBand * pReport = &pGridDataSvr->m_ReportBand->m_ReportBand; 
			if(pReport->bValid)
			{
				IXMLDOMElement * pReportElement;
				IXMLDOMElement * pBandElement;
				pXmlDoc->createElement(L"report",&pReportElement);

				pReportElement->setAttribute(L"left",(_variant_t)pReport->left);
				pReportElement->setAttribute(L"right",(_variant_t)pReport->right);
				if(pReport->PageHeader.bValid)
				{
					pXmlDoc->createElement(L"PageHeader",&pBandElement);
					pBandElement->setAttribute(L"startRow",(_variant_t)pReport->PageHeader.startRow);
					pBandElement->setAttribute(L"endRow",(_variant_t)pReport->PageHeader.endRow);
					pReportElement->appendChild(pBandElement,NULL);
					pBandElement->Release();
				}

				if(pReport->Header.bValid)
				{
					pXmlDoc->createElement(L"Header",&pBandElement);
					pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Header.startRow);
					pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Header.endRow);
					pReportElement->appendChild(pBandElement,NULL);
					pBandElement->Release();
				}

				if(pReport->Title.bValid)
				{
					pXmlDoc->createElement(L"Title",&pBandElement);
					pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Title.startRow);
					pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Title.endRow);
					pReportElement->appendChild(pBandElement,NULL);
					pBandElement->Release();
				}

				if(pReport->m_pGroups->size() >=1)
				{
					/*
					if(pReport->Header.bValid)
					{
						pXmlDoc->createElement(L"Header",&pBandElement);
						pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Header.startRow);
						pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Header.endRow);
					}

					if(pReport->Title.bValid)
					{
						pXmlDoc->createElement(L"Title",&pBandElement);
						pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Title.startRow);
						pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Title.endRow);
					}

					if(pReport->Detail.bValid)
					{
						pXmlDoc->createElement(L"Detail",&pBandElement);
						pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Detail.startRow);
						pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Detail.endRow);
					}

					if(pReport->Summury.bValid)
					{
						pXmlDoc->createElement(L"Summury",&pBandElement);
						pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Summury.startRow);
						pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Summury.endRow);
					}

					if(pReport->Footer.bValid)
					{
						pXmlDoc->createElement(L"Footer",&pBandElement);
						pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Footer.startRow);
						pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Footer.endRow);
					}*/
				}else
				{
					if(pReport->Detail.bValid)
					{
						pXmlDoc->createElement(L"Detail",&pBandElement);
						pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Detail.startRow);
						pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Detail.endRow);
						pReportElement->appendChild(pBandElement,NULL);
						pBandElement->Release();
					}
				}

				if(pReport->Summury.bValid)
				{
					pXmlDoc->createElement(L"Summury",&pBandElement);
					pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Summury.startRow);
					pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Summury.endRow);
					pReportElement->appendChild(pBandElement,NULL);
					pBandElement->Release();
				}

				if(pReport->Footer.bValid)
				{
					pXmlDoc->createElement(L"Footer",&pBandElement);
					pBandElement->setAttribute(L"startRow",(_variant_t)pReport->Footer.startRow);
					pBandElement->setAttribute(L"endRow",(_variant_t)pReport->Footer.endRow);
					pReportElement->appendChild(pBandElement,NULL);
					pBandElement->Release();
				}

				if(pReport->PageFooter.bValid)
				{
					pXmlDoc->createElement(L"PageFooter",&pBandElement);
					pBandElement->setAttribute(L"startRow",(_variant_t)pReport->PageFooter.startRow);
					pBandElement->setAttribute(L"endRow",(_variant_t)pReport->PageFooter.endRow);
					pReportElement->appendChild(pBandElement,NULL);
					pBandElement->Release();
				}
				pElement->appendChild(pReportElement,NULL);
				pReportElement->Release();
			}
		}
		
		return 1;
	}

	
	LPTSTR XCellSerializeSvr::SerializeSheetDataToHtml(XOfficeData * pDataSvr)
	{

		CGridDataSvr * pGridDataSvr = (CGridDataSvr *)pDataSvr;

		std::wstring    head;
		std::wstring	csStr;
		std::wstring	cs;
		std::wstring	ls_xml;
		BOOL		bFirstStr = FALSE;
		std::wstring	FirstStr = _T(""),LastStr;
		TCHAR strClass[255];
		
		int ARow,ACol, col, row,startCol;

		//write header
		cs += _T("\r\n<html xmlns:o=\"urn:schemas-microsoft-com:office:office\"");
		cs += _T("\r\nxmlns:O=\"urn:schemas-microsoft-com:office:office\"");
		cs += _T("\r\nxmlns:x=\"urn:schemas-microsoft-com:office:excel\"");
		cs += _T("\r\nxmlns:X=\"urn:schemas-microsoft-com:office:excel\"");
		cs += _T("\r\nxmlns:w=\"urn:schemas-microsoft-com:office:word\"");
		cs += _T("\r\nxmlns:W=\"urn:schemas-microsoft-com:office:word\"");
		cs += _T("\r\nxmlns=\"http://www.w3.org/TR/REC-html40\" encoding=\"utf-16\">");

		std::wstring	normalStyle;

		//write style in header
		cs +=_T("<head>");
		cs +=_T("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-16\"></meta>");
		cs += (std::wstring)L"<style>\r\n"+
			L"td\r\n{ border: windowtext 0.5pt;font-size:9.0pt;mso-number-format:\"\\@\";}";
		head = cs;

		std::map<std::wstring,std::wstring> styleClasses;
		pDataSvr->m_pStyleSvr->m_pCssStyle->GetStyles(styleClasses);
		cs = L"";

		//pDataSvr->m_pStyleSvr->m_pCssStyle->GetStylesString() +L"</style>";
		cs = _T("</style>");
		cs +=_T("</head>");
		

		//write body
		cs +=_T("\r\n<body style=\"margin:0\">");
			
		//write table
		pGridDataSvr->m_pContentSvr->GetValidRowCol(ACol,ARow);
		//adjust ARow && ACol

		for(int m=0;m<pGridDataSvr->m_pSpanSvr->SpanRects.size();m++)
		{
			if(pGridDataSvr->m_pSpanSvr->SpanRects[m].bottom >ARow) ARow = pGridDataSvr->m_pSpanSvr->SpanRects[m].bottom;
			if(pGridDataSvr->m_pSpanSvr->SpanRects[m].right >ACol) ACol = pGridDataSvr->m_pSpanSvr->SpanRects[m].right;
		}
		
		
		int width=0;
		for(int i1=1;i1<=ACol;i1++)
		{
			width += pGridDataSvr->m_pAxisSvr->GetColWidth(i1);
		}
		cs += _T("\r\n<table ");
		
		TCHAR buf[255];
		_stprintf_s(buf,255,_T("%s%d%s"),_T(" width=\""),width,_T("\""));
		cs += buf;
		cs +=_T(" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" style=\"")+
				(std::wstring)_T("border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt;")+
				(std::wstring)_T("mso-padding-alt:0cm 5.4pt 0cm 5.4pt\">");
		
		row = 1;
		int	height = 0;
		int i;
		for(i=1;i<=ARow;i++)
		{
			height += pGridDataSvr->m_pAxisSvr->GetRowHeight(i);
			col = pGridDataSvr->m_pContentSvr->GetValidCol(i);

			for(int m=0;m<=pGridDataSvr->m_pSpanSvr->SpanRects.size();m++)
			{
				if(pGridDataSvr->m_pSpanSvr->SpanRects[m].bottom >= i &&pGridDataSvr->m_pSpanSvr->SpanRects[m].right > col)
				{
					col = pGridDataSvr->m_pSpanSvr->SpanRects[m].right;
				}
			}
			
			if(col<1)
			{
				if( i != ARow)continue;

				//write top
				_stprintf_s(buf,255,_T("%s%d%s"),_T("\r\n <tr  height=\""),height,_T("\""));
				cs += buf;
				if(i - row >=1)
				{
					_stprintf_s(buf,255,_T("%s%d%s"),_T(" style='mso-xlrowspan:"),i - row + 1,_T("'"));
					cs += buf;
				}
				
				PStrItem	pItem;
				pItem = pGridDataSvr->m_pContentSvr->GetItemData(0,i);
				/*if(pItem && pItem->FAttrib)
				{
					cs = cs + _T(" ")+ pItem->FAttrib;
				}*/
				cs += _T(">");

				_stprintf_s(buf,255,_T("%s%d%s"),_T("\r\n  <td valign=\"top\" colspan=\""),ACol,_T("\" style='mso-ignore:colspan'></td>"));
				cs += buf;
				cs += _T("\r\n  </tr>");
				break;
			}
			
			if(i >= row)
			{
				//write top
				if(i>row)
				{
					_stprintf_s(buf,255,_T("%s%d%s"),  _T("\r\n <tr height=\"") ,height - pGridDataSvr->m_pAxisSvr->GetRowHeight(i) , _T("\"")) ;
					cs += buf;

					if(i - row >1)
					{
						_stprintf_s(buf,255,_T("%s%d%s"),_T(" style='mso-xlrowspan:"),i - row,_T("'"));
						cs += buf;
					}
					//PStrItem	pItem;
					pGridDataSvr->m_pContentSvr->GetItemData(0,i);
					/*if(pItem && pItem->FAttrib)
					{
						cs = cs + _T(" ")+ pItem->FAttrib;
					}*/
					cs += _T(">");
					_stprintf_s(buf,255,_T("%s%d%s"),_T("\r\n  <td valign=\"top\" colspan=\""),ACol,_T("\" style='mso-ignore:colspan'></td>"));
					cs += buf;
					cs += _T("\r\n  </tr>");
				}

				
				//write currow
				PStrItem	pItem;
				TGridRect	rect;
				BOOL		bSpaned;
				
				startCol = 1;
				
				//process firststr
				std::wstring	xslStr;
				pItem = pGridDataSvr->m_pContentSvr->GetItemData(0,i);
				cs +=_T("\r\n   <tr");
				_stprintf_s(buf,255,_T("%s%d%s"), _T(" height=\"") ,pGridDataSvr->m_pAxisSvr->GetRowHeight(i),_T("\""));
				cs += buf;
				cs +=_T(">");

				std::wstring classstr;
				for(int j=1;j<=ACol;j++)
				{
					bSpaned = FALSE;
					if(pGridDataSvr->m_pSpanSvr->isSpaned(j,i,rect))
					{
						//write left
						if(j>startCol)
						{
							_stprintf_s(buf,255,_T("%s%d%s%s%s"),_T("\r\n    <td valign='top' colspan='"),j - startCol,_T("' style='mso-ignore:colspan'"),
								classstr==L""?L"":(L" class='"+classstr+L"'").c_str(),L"></td>");
							cs += buf;
						}
						if(rect.left != j || rect.top  != i)
						{
							startCol = j+1;
							continue;
						}
						
						pItem = pGridDataSvr->m_pContentSvr->GetItemData(j,i);
						if(!pItem)
						{
							_stprintf_s(buf,255,_T("%s%d%s%d%s"),_T("\r\n    <td valign='top' rowspan ='"),rect.bottom - rect.top +1 ,_T("' colspan='"),rect.right - rect.left + 1,L"'");
							cs += buf;
							int nIndex =  (*pGridDataSvr->GetStyleSvr()->m_pCssClass)[j]->GetValue(i);
							pGridDataSvr->GetStyleSvr()->m_pCssStyle->GetClassNameByIndex(nIndex,strClass);
							cs += (std::wstring)L" class='"+ IXFUtil::EnsureComboStyleClassName(strClass,styleClasses).c_str() + L"'";
							cs +=_T("></td>");
							startCol = rect.right + 1;
							continue; 
						}else
						{
							//write curspan
							bSpaned = TRUE;
							_stprintf_s(buf,255,_T("%s%d%s%d%s"),_T("\r\n    <td valign='top' rowspan ='"),rect.bottom - rect.top +1 ,_T("' colspan='"),rect.right - rect.left + 1,_T("'"));
							cs += buf;
						}
					}else
					{
						pItem = pGridDataSvr->m_pContentSvr->GetItemData(j,i);
						if(!pItem)
						{
							int nIndex =  (*pGridDataSvr->GetStyleSvr()->m_pCssClass)[j]->GetValue(i);
							pGridDataSvr->GetStyleSvr()->m_pCssStyle->GetClassNameByIndex(nIndex,strClass);
							std::wstring curClassStr = IXFUtil::EnsureComboStyleClassName(strClass,styleClasses);
							if(j==startCol)classstr = curClassStr;
							if(j== ACol)
							{
								if(j> startCol )
								{
									if(curClassStr != classstr)
									{
										//write left
										_stprintf_s(buf,255,_T("%s%d%s%s%s"),_T("\r\n    <td valign='top' colspan='"),j - startCol,_T("'"),classstr!=L""?(L" class='"+classstr+L"'").c_str():L"",_T(" style='mso-ignore:colspan'></td>"));
										cs += buf;

										//write owner
										_stprintf_s(buf,255,_T("%s%s%s"),_T("\r\n    <td valign='top'"),classstr!=L""?(L" class='"+classstr+L"'").c_str():L"",_T("></td>"));
										cs += buf;
									}else
									{
										//write left with owner
										_stprintf_s(buf,255,_T("%s%d%s%s%s"),_T("\r\n    <td valign='top' colspan='"),j - startCol + 1,_T("'"),classstr!=L""?(L" class='"+classstr+L"'").c_str():L"",_T(" style='mso-ignore:colspan'></td>"));
										cs += buf;
									}
								}
								else
								{
									//write owner
									_stprintf_s(buf,255,_T("%s%s%s"),_T("\r\n    <td valign='top' "),classstr!=L""?(L" class='"+classstr+L"'").c_str():L"",_T("></td>"));
									cs += buf;
									break;
								}
								break;
							}else
							{
								if(curClassStr == classstr && curClassStr==L"") continue;
								if(j> startCol )
								{
									//write left
									_stprintf_s(buf,255,_T("%s%d%s%s%s"),_T("\r\n    <td valign='top' colspan='"),j - startCol,_T("'"),classstr!=L""?(L" class='"+classstr+L"'").c_str():L"",_T(" style='mso-ignore:colspan'></td>"));
									cs += buf;
								}
								if(curClassStr==L"")
								{
									startCol = j;
									classstr = curClassStr;
								}else
								{
									//write owner
									_stprintf_s(buf,255,_T("%s%s%s"),_T("\r\n    <td valign='top'"),classstr!=L""?(L" class='"+classstr+L"'").c_str():L"",_T("></td>"));
									cs += buf;
									startCol = j + 1;
									classstr = curClassStr;
								}
								continue;
							}
						}
						else
						{
							//write left
							if(j>startCol)
							{
								//write left
								_stprintf_s(buf,255,_T("%s%d%s%s%s"),_T("\r\n    <td valign='top' colspan='"),j - startCol,_T("'"),classstr!=L""?(L" class='"+classstr+L"'").c_str():L"",_T(" style='mso-ignore:colspan'></td>"));
								cs += buf;
							}
						}
					}
					
					//write curcol
					if(bSpaned)
						cs += _T(" ");
					else
						cs += _T("\r\n    <td valign='top'");

					int nIndex =  (*pGridDataSvr->GetStyleSvr()->m_pCssClass)[j]->GetValue(i);
					pGridDataSvr->GetStyleSvr()->m_pCssStyle->GetClassNameByIndex(nIndex,strClass);
					if(strClass[0])cs += (std::wstring)L" class='"+ IXFUtil::EnsureComboStyleClassName(strClass,styleClasses).c_str() + L"'";
					cs += _T(">");
					if(pItem->pStrContent)
					{
						wchar_t* t = XUtil::ToXmlText(pItem->pStrContent);
						if(!t) t = pItem->pStrContent;
						cs += t; //??
						if(t != pItem->pStrContent) delete t;
					}
					cs += _T("</td>");
					startCol = j+1;
				}
				cs += _T("\r\n  </tr>"); 
				row = i + 1;
				height = 0;
			}
		}
		
		cs +=_T("\r\n <tr height='0'>");
		for(i=1;i<=ACol;i++)
		{
			_stprintf_s(buf,255,_T("%s%d%s"),_T("\r\n  <td valign='top' width='"),pGridDataSvr->m_pAxisSvr->GetColWidth(i),_T("'></td>"));
			cs += buf;
		}
		cs +=_T("\r\n </tr>");
		cs +=_T("\r\n</table>");


		cs += _T("\r\n</body>");
		cs += _T("\r\n</html>");

		std::wstring style;
		std::map<std::wstring,std::wstring>::const_iterator iter = styleClasses.begin();
		while(iter != styleClasses.end())
		{
			if(iter->first!=L"" && iter->second!=L"")
			{
				style = style + L"\r\n"+ L"." + iter->first +  L"\r\n{" +  iter->second + L"}\r\n";
			}
			iter++;
		}
		cs = head + style + cs;
		return _tcsdup(cs.c_str());
	}

#if 0
	
	#define XT_HTML 0
	#define XT_XSL  1

	void CStringGrid::SaveToXML(LPCSTR szFileName, DWORD xmlType, CString &cs)
	{
		CString	csStr;
		CString	ls_xml;
		PStrItem	pItem;
		BOOL		bFirstStr = FALSE;
		CString		FirstStr = "",LastStr;
		
		int ARow,ACol, col, row,startCol;
		CFile f;
		if(this->FMainGrid)
		{
			if(szFileName)
			{
				if( !f.Open( szFileName, CFile::modeCreate | CFile::modeWrite ) )
				{
	#ifdef _DEBUG
					afxDump << "Unable to open file" << "\n";
					exit( 1 );
	#endif
				}
			}
			
			//write header
			cs = "<?xml  version=\"1.0\" encoding=\"gb2312\" ?>";
			//	cs +="\r\n<!DOCTYPE xsl:stylesheet [<!ENTITY nbsp \"&#x20;\">]>";
			if(xmlType == XT_XSL)
			{
				cs += "\r\n<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\" version=\"1.0\">";
				cs += "\r\n<xsl:template match=\"/\">";
			}
			cs += "\r\n<html xmlns:o=\"urn:schemas-microsoft-com:office:office\"";
			cs += "\r\nxmlns:O=\"urn:schemas-microsoft-com:office:office\"";
			cs += "\r\nxmlns:x=\"urn:schemas-microsoft-com:office:excel\"";
			cs += "\r\nxmlns:X=\"urn:schemas-microsoft-com:office:excel\"";
			cs += "\r\nxmlns:w=\"urn:schemas-microsoft-com:office:word\"";
			cs += "\r\nxmlns:W=\"urn:schemas-microsoft-com:office:word\"";
			cs += "\r\nxmlns:hrt=\"urn:schemas-hangxinsoft-com:hxrobot\"";
			cs += "\r\nxmlns=\"http://www.w3.org/TR/REC-html40\" encoding=\"gb2312\">";
			
			CString	normalStyle;
			((CHxrobotView *)m_pView)->GetNormalStyle(normalStyle);
			//write style in header
			cs +="<head>";
			cs +="<meta http-equiv=\"Content-Type\" content=\"text/html; charset=GB2312\"></meta>";
			if(xmlType != XT_XSL)
			{
				if(m_style.Find(normalStyle)<0)
					cs +="<style>"+m_style+"\r\n"+normalStyle+"</style>";
				else
					cs +="<style>"+m_style+"</style>";
			}
			else
			{
				if(m_style.Find(normalStyle)<0)
					cs +="<style><xsl:comment>"+m_style+"\r\n"+normalStyle+"</xsl:comment></style>";	
				else
					cs +="<style><xsl:comment>" + m_style + "</xsl:comment></style>";	
				
			}
			cs +="</head>";
			
			//write body
			cs +="\r\n<body style=\"margin:0\">";
			
		}else
		{
			cs.Format("%s%i%s%i%s",cs + "\r\n<hrt:sets rowcount=\"",this->FRowCount,"\" colcount=\"",this->FColCount,"\">");
			cs +="\r\n<div style=\"left:0;top:0\">";
		}

		//firstStr
		pItem = this->GetCellsItem(0,0);
		if(pItem && pItem->FPreStr && pItem->FPreStr !='\0')
		{
			FirstStr = pItem->FPreStr;
			if(xmlType ==XT_XSL)cs += "\r\n" + FirstStr;
			bFirstStr = TRUE;
		}
		else
		{
			//default
	//		if(xmlType ==XT_XSL)cs +="\r\n<xsl:for-each select=\"data/row\">";
		}

		//write table
		this->GetValidRowCol(ACol,ARow);
		//adjust ARow && ACol
		for(int m=0;m<=this->m_spans.GetUpperBound();m++)
		{
			if(m_spans[m].bottom >ARow) ARow = m_spans[m].bottom;
			if(m_spans[m].right >ACol) ACol = m_spans[m].right;
		}

		int width=0;
		for(int i1=1;i1<=ACol;i1++)
		{
			width += this->GetColWidths(i1);
		}
		cs += "\r\n<table ";
		cs.Format("%s%i%s",cs+"width=\"",width,"\"");
		cs +=" border=\"0\" cellspacing=\"0\" cellpadding=\"0\" style=\""+
				(CString)"border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt;"+
				(CString)"mso-padding-alt:0cm 5.4pt 0cm 5.4pt\">";

		row = 1;

		ls_xml ="<xml>\r\n\t<hxcell>";
		for(int j = 0 ;j<=0/*j<=FColCount - 1*/;j++)
		{
			pItem = this->GetCellsItem(0,j);
			if(!pItem)continue;
			if(pItem->FPreStr && pItem->FPreStr !='\0')
			{
				ls_xml += (CString)"\r\n\t\t"+"<item>";
				ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",0,"</row>");
				ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
				ls_xml = ls_xml +"\r\n\t\t\t"+"<前缀><![CDATA[" + pItem->FPreStr + "]]></前缀></item>";
			}
			if(pItem->FLastPre && pItem->FLastPre !='\0')
			{
				ls_xml += (CString)"\r\n\t\t"+"<item>";
				ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",0,"</row>");
				ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
				ls_xml = ls_xml +"\r\n\t\t\t"+"<后缀><![CDATA[" + pItem->FLastPre + "]]></后缀></item>";
			}
			if(pItem->FNote && pItem->FNote !='\0')
			{
				ls_xml += (CString)"\r\n\t\t"+"<item>";
				ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",0,"</row>");
				ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
				ls_xml = ls_xml +"\r\n\t\t\t"+"<备注><![CDATA[" + pItem->FNote + "]]></备注></item>";
			}
			if(pItem->FFormula && pItem->FFormula !='\0')
			{
				ls_xml += (CString)"\r\n\t\t"+"<item>";
				ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",0,"</row>");
				ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
				ls_xml = ls_xml +"\r\n\t\t\t"+"<公式><![CDATA[" + pItem->FFormula + "]]></公式></item>";
			}
			if(pItem->FLink && pItem->FLink !='\0')
			{
				ls_xml += (CString)"\r\n\t\t"+"<item>";
				ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",0,"</row>");
				ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
				ls_xml = ls_xml +"\r\n\t\t\t"+"<链接><![CDATA[" + pItem->FLink + "]]></链接></item>";
			}
			if(pItem->FXml && pItem->FXml !='\0')
			{
				ls_xml += (CString)"\r\n\t\t"+"<item>";
				ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",0,"</row>");
				ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
				ls_xml = ls_xml +"\r\n\t\t\t"+"<其他><![CDATA[" + pItem->FXml + "]]></其他></item>";
			}
			CString	csX;
			csX = pItem->FString;
			if(pItem->FString && pItem->FString !='\0' && (csX.Find("@{")>=0 || (pItem->FLink && pItem->FLink !='\0')))
			{
				CString	val;
				val = pItem->FString;
				ls_xml += (CString)"\r\n\t\t"+"<item>";
				ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",0,"</row>");
				ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
				ls_xml = ls_xml +"\r\n\t\t\t"+"<数据列><![CDATA[" + val + "]]></数据列></item>";
			}
		}

		int	height = 0;
		for(int i=1;i<=ARow;i++)
		{
			height += this->GetRowHeights(i);
			this->GetValidCol(col,i);
			for(int j = 0 ;j<=col;j++)
			{
				pItem = this->GetCellsItem(j,i);
				if(!pItem)continue;
				if(pItem->FPreStr && pItem->FPreStr !='\0')
				{
					ls_xml += (CString)"\r\n\t\t"+"<item>";
					ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",i,"</row>");
					ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
					ls_xml = ls_xml +"\r\n\t\t\t"+"<前缀><![CDATA[" + pItem->FPreStr + "]]></前缀></item>";
				}
				if(pItem->FLastPre && pItem->FLastPre !='\0')
				{
					ls_xml += (CString)"\r\n\t\t"+"<item>";
					ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",i,"</row>");
					ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
					ls_xml = ls_xml +"\r\n\t\t\t"+"<后缀><![CDATA[" + pItem->FLastPre + "]]></后缀></item>";
				}
				if(pItem->FNote && pItem->FNote !='\0')
				{
					ls_xml += (CString)"\r\n\t\t"+"<item>";
					ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",i,"</row>");
					ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
					ls_xml = ls_xml +"\r\n\t\t\t"+"<备注><![CDATA[" + pItem->FNote + "]]></备注></item>";
				}
				if(pItem->FFormula && pItem->FFormula !='\0')
				{
					ls_xml += (CString)"\r\n\t\t"+"<item>";
					ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",i,"</row>");
					ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
					ls_xml = ls_xml +"\r\n\t\t\t"+"<公式><![CDATA[" + pItem->FFormula + "]]></公式></item>";
				}
				if(pItem->FLink && pItem->FLink !='\0')
				{
					ls_xml += (CString)"\r\n\t\t"+"<item>";
					ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",i,"</row>");
					ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
					ls_xml = ls_xml +"\r\n\t\t\t"+"<链接><![CDATA[" + pItem->FLink + "]]></链接></item>";
				}
				if(pItem->FXml && pItem->FXml !='\0')
				{
					ls_xml += (CString)"\r\n\t\t"+"<item>";
					ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",i,"</row>");
					ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
					ls_xml = ls_xml +"\r\n\t\t\t"+"<其他><![CDATA[" + pItem->FXml + "]]></其他></item>";
				}
				CString	csX;
				csX = pItem->FString;
				if(pItem->FString && pItem->FString !='\0' && (csX.Find("@{")>=0 || (pItem->FLink && pItem->FLink !='\0')))
				{
					CString	val;
					val = pItem->FString;
					ls_xml += (CString)"\r\n\t\t"+"<item>";
					ls_xml.Format("%s%i%s",ls_xml +  "\r\n\t\t\t" + "<row>",i,"</row>");
					ls_xml.Format("%s%i%s",ls_xml +"\r\n\t\t\t"+(CString)"<col>",j,"</col>");
					ls_xml = ls_xml +"\r\n\t\t\t"+"<数据列><![CDATA[" + val + "]]></数据列></item>";
				}
			}
			for(int m=0;m<=this->m_spans.GetUpperBound();m++)
			{
				if(m_spans[m].bottom >= i && m_spans[m].right > col)
				{
					col = m_spans[m].right;
				}
			}
			if(col<1)
			{
				if( i != ARow)continue;

				//write top
				cs.Format("%s%i%s", cs+ "\r\n <tr height=\"" ,height, "\"") ;
				if(i - row >=1)cs.Format("%s%i%s",cs+" style='mso-xlrowspan:",i - row + 1,"'");
				
				PStrItem	pItem;
				pItem = this->GetCellsItem(0,i);
				if(pItem && pItem->FAttrib)
				{
					cs = cs + " "+ pItem->FAttrib;
				}
				cs += ">";
				cs.Format("%s%i%s",cs+"\r\n  <td valign=\"top\" colspan=\"",ACol,"\" style='mso-ignore:colspan'></td>");
				cs += "\r\n  </tr>";
				break;
			}
			if(i >= row)
			{
				//write top
				if(i>row)
				{
					cs.Format("%s%i%s", cs+ "\r\n <tr height=\"" ,height - this->GetRowHeights(i) , "\"") ;
					if(i - row >1)cs.Format("%s%i%s",cs+" style='mso-xlrowspan:",i - row,"'");
					PStrItem	pItem;
					pItem = this->GetCellsItem(0,i);
					if(pItem && pItem->FAttrib)
					{
						cs = cs + " "+ pItem->FAttrib;
					}
					cs += ">";
					cs.Format("%s%i%s",cs+"\r\n  <td valign=\"top\" colspan=\"",ACol,"\" style='mso-ignore:colspan'></td>");
					cs += "\r\n  </tr>";
				}

				//write currow
				PStrItem	pItem;
				CRect		rect;
				BOOL		bSpaned;
				
				startCol = 1;
				
				//process firststr
				CString	xslStr;
				pItem = this->GetCellsItem(0,i);
				if(pItem && pItem->FPreStr && pItem->FPreStr !='\0')
				{
					xslStr = pItem->FPreStr;
					if(xmlType == XT_XSL) cs = cs + "\r\n  "+xslStr;
				}
				cs +="\r\n   <tr";
				cs.Format("%s%i%s", cs + " height=\"" ,this->GetRowHeights(i),"\"" );
				if(pItem && pItem->FAttrib)
					cs = cs + " "+ pItem->FAttrib;
				cs +=">";

				for(int j=1;j<=ACol;j++)
				{
					bSpaned = FALSE;
					if(this->isSpaned(j,i,rect))
					{
						//write left
						if(j>startCol)cs.Format("%s%i%s",cs+"\r\n    <td valign=\"top\" colspan=\"",j - startCol,"\" style='mso-ignore:colspan'></td>");
						if(rect.left != j || rect.top  != i)
						{
							startCol = j+1;
							continue;
						}
						
						pItem = this->GetCellsItem(j,i);
						if(!pItem)
						{
							cs.Format("%s%i%s%i%s",cs+"\r\n    <td valign=\"top\" rowspan =\"",rect.bottom - rect.top +1 ,"\" colspan=\"",rect.right - rect.left + 1,"\"></td>");
							startCol = rect.right + 1;
							continue;
						}else
						{
							//write curspan
							bSpaned = TRUE;
							cs.Format("%s%i%s%i%s",cs+"\r\n    <td valign=\"top\" rowspan =\"",rect.bottom - rect.top +1 ,"\" colspan=\"",rect.right - rect.left + 1,"\"");
						}
					}else
					{
						pItem = this->GetCellsItem(j,i);
						if(!pItem)
						{
							if(j != ACol) continue;
							cs.Format("%s%i%s",cs+"\r\n    <td valign=\"top\" colspan=\"",j - startCol + 1,"\" style='mso-ignore:colspan'></td>");
							break;
						}
						//write left
						if(j>startCol)cs.Format("%s%i%s",cs+"\r\n    <td valign=\"top\" colspan=\"",j - startCol,"\" style='mso-ignore:colspan'></td>");
					}
					
					//write curcol
					if(bSpaned)
						cs += "  style='border:none";
					else
						cs += "\r\n  <td valign=\"top\" style='border:none";

					if(pItem->FBorder & 0x01)cs += ";border-top:solid windowtext .5pt";
					if(pItem->FBorder & 0x02)cs += ";border-bottom:solid windowtext .5pt";
					if(pItem->FBorder & 0x04)cs += ";border-left:solid windowtext .5pt";
					if(pItem->FBorder & 0x08)cs += ";border-right:solid windowtext .5pt";
					
					CString	align = "" ,style ="",textColor ="";
					CString	prevStr="" ,endStr="";
					char buf[12];

					if((int)pItem->FBkColor >0)
					{
						::ltoa(pItem->FBkColor,buf,16);
						textColor += (CString)"000000" + buf;
						textColor = textColor.Right(6);
						textColor = textColor.Right(2) + textColor.Mid(2,2)+textColor.Left(2);
						cs += ";background-color:#" + textColor;
					}

					if(pItem->FString)
					{
						if(pItem->FAlign & DT_LEFT) cs += ";text-align:left";
						if(pItem->FAlign & DT_RIGHT) cs += ";text-align:right";
						if(pItem->FAlign & DT_CENTER) cs += ";text-align:center";
						if(pItem->FAlign & DT_TOP) cs += ";vertical-align:top";
						if(pItem->FAlign & DT_VCENTER)cs += ";vertical-align:middle";
						if(pItem->FAlign & DT_BOTTOM) cs += ";tvertical-align:bottom";
					}
					
					if(pItem->FString)
					{
						if(pItem->FAlign & DT_LEFT) align = "align=\"left\"";
						if(pItem->FAlign & DT_RIGHT) align = "align=\"right\"";
						if(pItem->FAlign & DT_CENTER) align = "align=\"center\"";
						if(pItem->FAlign & DT_TOP) align += " valign=\"top\"";
						if(pItem->FAlign & DT_VCENTER) align += " valign=\"center\"";
						if(pItem->FAlign & DT_BOTTOM) align += " valign=\"bottom\"";
					}
					if((int)pItem->FTextColor >0)
					{
						::ltoa(pItem->FTextColor,buf,16);
						textColor += (CString)"000000" + buf;
						textColor = textColor.Right(6);
						textColor = textColor.Right(2) + textColor.Mid(2,2)+textColor.Left(2);
						style += "color:#" + textColor;
					}
					if((int)pItem->FBkColor >0)
					{
						::ltoa(pItem->FBkColor,buf,16);
						textColor += (CString)"000000" + buf;
						textColor = textColor.Right(6);
						textColor = textColor.Right(2) + textColor.Mid(2,2)+textColor.Left(2);
						style += "background-color:#" + textColor;
					}
					
					LOGFONT	logFont;
					if(pItem->FFontIndex)
						this->m_Fonts[pItem->FFontIndex]->GetLogFont(&logFont);
					else
						this->m_Fonts[0]->GetLogFont(&logFont);
					
					if(logFont.lfWeight == FW_BOLD)
					{
						prevStr +="<b>";
						endStr  ="</b>" + endStr;
					}
					if(logFont.lfItalic)
					{
						prevStr +="<i>";
						endStr  ="</i>" + endStr;
					}
					if(logFont.lfUnderline)
					{
						prevStr +="<u>";
						endStr  ="</u>" + endStr;
					}
					
					style +=";font-family:"+(CString)logFont.lfFaceName;
					style.Format("%s%5.2dpt", style+";font-size:", ::MulDiv(abs(logFont.lfHeight),72,96));
					if(style.Left(1)==";")style = style.Mid(2);
					CString	CStr,link,link1;
					CStr = (CString)pItem->FString;
					hxhtml	lnv_html;
					CString dest;
					lnv_html.ConvertToXml(&CStr,&dest);
					CStr = dest;
					if(CStr.Find("@{")>=0 || pItem->FLink)
					{
						if(pItem->FLink)
						{
							link = pItem->FLink;
							this->ToXmlText(link);
							link1 = link;

							int	ll_pos=0,ll_pos1=0;
							ll_pos = link1.Find("@{",0);
							while(ll_pos>=0)
							{
								ll_pos1= link1.Find("}",ll_pos + 1);
								if(ll_pos1 < 0)break;
								link1 = link1.Left(ll_pos) + "<xsl:value-of select=\""+link1.Mid(ll_pos+2,ll_pos1 - ll_pos - 2)+"\"/>"+link1.Mid(ll_pos1+1);
								ll_pos = link1.Find("@{",0);
							}
						}
						
						CString	content="";
						content = CStr;
						int	ll_pos=0,ll_pos1=0;
						ll_pos = content.Find("@{",0);
						while(ll_pos>=0)
						{
							ll_pos1= content.Find("}",ll_pos + 1);
							if(ll_pos1<0)break;
							content = content.Left(ll_pos) + "<xsl:value-of select=\""+content.Mid(ll_pos+2,ll_pos1 - ll_pos - 2)+"\"/>"+content.Mid(ll_pos1+1);
							ll_pos = content.Find("@{",0);
						}
						CString	content1="";
						content1 = CStr;
						ll_pos = content1.Find("@{",0);
						while(ll_pos>=0)
						{
							ll_pos1= content1.Find("}",ll_pos + 1);
							if(ll_pos1<0)break;
							content1 = content1.Left(ll_pos) + "&nbsp;"+ content1.Mid(ll_pos1+1);
							ll_pos = content1.Find("@{",0);
						}

						if(xmlType == XT_XSL)
						{
							if(pItem->FLink)
							{
								CStr = "<span><a><xsl:attribute name=\"href\">"+link1+"</xsl:attribute>" + content + "</a></span>";
							}else
							{
								CStr = "<span>" + content + "</span>";
							}
						}
						else
						{
							if(pItem->FLink)
							{
								if(CStr.Find("@{")>=0)
									CStr = "<span><a href=\""+link+"\"/>"+content1+"</span>";
								else
									CStr = "<span><a href=\""+link+"\">"+CStr+"</a></span>";
							}else
							{
								CStr = "<span>"+content1;
							}
						}
					}else
					{
						if(xmlType == XT_XSL)
							CStr.Replace("&nbsp;","&#x20;");
						//	if(CStr.FindOneOf("\r\n&><")>=0)CStr = "<![CDATA["+CStr+"]]>";
					}
					if(pItem->FTable)
					{
						CStr = "<div style=\"left:0;top:0\">" + CStr;
						CStr +="</div>";
					}

	#ifdef EDIT
					cs += "'><P " + align + " style='"+ style + "'>" + prevStr+ CStr + endStr + "</P>";
	#else
					cs += "'>"+prevStr+ CStr + endStr;
	#endif
					if(pItem->FTable)
					{
						((CStringGrid *)pItem->FTable)->SaveToXML(NULL,xmlType,cs);
					}
					cs += "</td>";
					startCol = j+1;
				}
				cs += "\r\n  </tr>";
				//process lastxslstr
				pItem = this->GetCellsItem(0,i);
				if(pItem && pItem->FLastPre && pItem->FLastPre !='\0')
				{
					xslStr = pItem->FLastPre;
					if(xmlType == XT_XSL) cs = cs + "\r\n  "+xslStr;
				}
				row = i + 1;
				height = 0;
			}
		}
		
		cs +="\r\n <tr height=\"0\" hrt:adjust=\"yes\">";
		for(int i=1;i<=ACol;i++)
		{
			cs.Format("%s%i%s",cs+"\r\n  <td valign=\"top\" width=\"",GetColWidths(i),"\"></td>");
		}
		cs +="\r\n </tr>";
		cs += "\r\n"+ls_xml+"\r\n\t</hxcell>\r\n</xml>";
		cs +="\r\n</table>";

		cs +="\r\n<hrt:objs>";
		CRect	ARect;
		POSITION pos = m_objects.GetTailPosition();
		while (pos != NULL)
		{
			CDrawObj* pObj = m_objects.GetPrev(pos);
			ARect = pObj->m_position;
			ARect.NormalizeRect();
			cs.Format("%s%i%s%i%s%i%s%i%s",
				cs + "<div style=\"position:absolute;top:",ARect.top - FBound.top,";left:", ARect.left - FBound.left,
					";width:",ARect.right - ARect.left ,";height:",ARect.bottom - ARect.top,"\">");
			
							
			CString	content="";
			int	ll_pos=0,ll_pos1=0;
			content = pObj->m_Content;
			ll_pos = content.Find("@{",0);
			if(ll_pos >=0)
			{
				while(ll_pos>=0)
				{
					ll_pos1= content.Find("}",ll_pos + 1);
					content = content.Left(ll_pos) + "<xsl:value-of select=\""+content.Mid(ll_pos+2,ll_pos1 - ll_pos - 2)+"\"/>"+content.Mid(ll_pos1+1);
					ll_pos = content.Find("@{",0);
				}

				CString	content1="";
				content1 = pObj->m_Content;
				ll_pos = content1.Find("@{",0);
				while(ll_pos>=0)
				{
					ll_pos1= content1.Find("}",ll_pos + 1);
					content1 = content1.Left(ll_pos) + "&nbsp;"+ content1.Mid(ll_pos1+1);
					ll_pos = content1.Find("@{",0);
				}
				if(xmlType == XT_XSL)
					cs += content;
				else
					cs += content1;

				cs += "\r\n<xml>"+pObj->m_Content+"</xml>";
			}
			else
				cs += pObj->m_Content;	
			cs +="</div>";
		}
		cs +="\r\n</hrt:objs>";

		CString endStr,xslStr;
		pItem = this->GetCellsItem(0,0);
		if(pItem && pItem->FLastPre && pItem->FLastPre !='\0')
		{
			endStr = pItem->FLastPre;
			if(xmlType == XT_XSL) cs += "\r\n" + endStr;
		}else
		{
			if(xmlType ==XT_XSL && bFirstStr)cs +="\r\n</xsl:for-each>";
		}

		if(this->FMainGrid)
		{
			cs += "\r\n</body>";
			cs += "\r\n</html>";
			if(xmlType==XT_XSL)
			{
				cs += "\r\n</xsl:template>";
				cs += "\r\n</xsl:stylesheet>";
			}
			cs.Replace("<?XML:NAMESPACE PREFIX=\"O\">"," ");
			cs.Replace("</?XML:NAMESPACE>"," ");
			if(xmlType == XT_XSL)
				cs.Replace("&nbsp;"," ");
			
			cs.Replace("<BR></BR>","");
			cs.Replace("<BR>","<p>");
			cs.Replace("</BR>","</p>");
			
			if(szFileName)
			{
				f.Write(cs,cs.GetLength());
				f.Close();
			}
		}else
		{
			cs +="\r\n</div>";
			cs +="\r\n</hrt:sets>";
		}
	}

	void CStringGrid::SaveToHtml(PCSTR  szFileName)
	{
		CString	cs;
		this->SaveToXML(szFileName,XT_HTML,cs);
	}

	void CStringGrid::SaveToXsl( LPCSTR szFileName)
	{
		CString	cs;
		this->SaveToXML(szFileName,XT_XSL,cs);
	}

	int CStringGrid::LoadTable(hxhtml &nvhtml,int &sCol,int &sRow,int ll_start)
	{
		this->SetColWidths(0,0);
		this->SetRowHeights(0,0);

		BOOL	tableBorder = FALSE;
		BOOL	bExtent;
		CString height,width;
		CString	contents="",content="";
		CString	ls_styles,ls_style;
		CString			tag;
		CString	val;
		int startcol;
		int row,col,rowspan,colspan;
		int	colCount;
		CArray<long,long> ll_ids,ll_items,ll_childs;
		CArray<long,long> cols;
		CArray<long,long> rows;
		CArray<long,long> unColSpans;
		CArray<long,long> unRowSpans;
		int	ll_id;
		int	ll_level;
	//	int	rowCnt,colCnt;

		HDC	dc=::GetDC(NULL);

		nvhtml.of_getattrib(ll_start,"border",val);
		if(val=="1")
		{
			tableBorder = TRUE;
		}else
		{
			nvhtml.of_getattrib(ll_start,"style",val);
			if(::strstr(val,"border:solid"))tableBorder = TRUE;
		}

		//firstStr
		nvhtml.of_getattrib(ll_start,"firststr",val);
		this->UnXMLText(val);
		if(!(val.IsEmpty() && val ==""))
		{
			PStrItem	pItem = NewStrItem();
			pItem->mask = STI_PRESTR;
			pItem->FPreStr = new char[val.GetLength()+1];
			::strcpy(pItem->FPreStr,val);
			this->SetCellsItem(0,0,pItem);
			DisposeStrItem(pItem);
		}

		//endStr
		nvhtml.of_getattrib(ll_start,"laststr",val);
		this->UnXMLText(val);
		if(!(val.IsEmpty() && val ==""))
		{
			PStrItem	pItem = NewStrItem();
			pItem->mask = STI_LASTPRE;
			pItem->FLastPre = new char[val.GetLength()+1];
			::strcpy(pItem->FLastPre,val);
			this->SetCellsItem(0,0,pItem);
			DisposeStrItem(pItem);
		}


	/*
		m_table.rows = ll_ids.GetUpperBound();	//from 0
		m_table.cols = m_table.defcols - 1;

		m_table.cWidths = new int[m_table.cols + 1];
		m_table.rHeights = new int[m_table.rows + 1];

		::memset(m_table.cWidths,0,sizeof(int)*(m_table.cols + 1));
		::memset(m_table.rHeights,0,sizeof(int)*(m_table.rows + 1));
	*/
		ll_level = nvhtml.il_levels[ll_start];

		colCount = 0;
		CString	element;
		row = -1;

		int ll_xmlId;
		ll_xmlId = ll_start + 1;
		while(ll_xmlId <= nvhtml.is_elements.GetUpperBound())
		{
			if(nvhtml.il_levels[ll_xmlId] <=ll_level)break;
			element = nvhtml.is_elements[ll_xmlId];
			element.MakeLower();
			if(element !="tr")
			{
				ll_xmlId++;
				continue;
			}
			row++;

			ll_id = ll_xmlId;
			ll_xmlId++;

			bExtent = TRUE;

			//get attrib
			CString	ls_attrib,ls_stemp;
			ls_attrib = " "+nvhtml.is_attribs[ll_id]+" ";
			ls_attrib.MakeLower();
			if(ls_attrib.Find(" height=")>=0)
			{
				ls_attrib = ls_attrib.Left(ls_attrib.Find(" height=")) +
					ls_attrib.Mid(ls_attrib.Find(" ",ls_attrib.Find(" height=") +2));
				ls_attrib.TrimLeft();
				ls_attrib.TrimRight();
				PStrItem	pItem = this->GetCellsItem(0,row + sRow);
				if(pItem && pItem->FAttrib)delete pItem->FAttrib;
				if(ls_attrib.IsEmpty() || ls_attrib =="")
				{
					if(pItem)pItem->FAttrib = NULL;
				}else
				{
					if(!pItem)
					{
						pItem  = NewStrItem();
						pItem->mask = STI_ENTRY;
						this->SetCellsItem(0,row + sRow,pItem);
					}
					pItem->FAttrib = new char[ls_attrib.GetLength()+1];
					::strcpy(pItem->FAttrib,ls_attrib);
				}
			}

			int xlrowspan;
			xlrowspan = 0;
			nvhtml.of_getattrib(ll_id,"style",val);
			if(::strstr(val,"mso-xlrowspan:"))
			{
				val = ::strstr(val,"mso-xlrowspan:") + strlen("mso-xlrowspan:")+(CString)";";
				val = val.Left(val.Find(";"));
				xlrowspan = ::atol(val);
			}
			//get height**********************************
			nvhtml.of_getattrib(ll_id,"height",height);
			if(height.IsEmpty())
			{
				nvhtml.of_getattrib(ll_id,"style",height);
				height+=";";
				if(::strstr(height,"height:"))
				{
					height = ::strstr(height,"height:") + strlen("height:");
					height = height.Left(height.Find(";"));
				}else
				{
					bExtent = FALSE;
				}
			}
			if(bExtent)
			{
				if(::strstr(height,"pt"))
				{
					height = height.Left(height.Find("pt"));
					if(xlrowspan <=1 )
						this->FRowHeights[row + sRow] = ::MulDiv((int)(::atof(height)*100),96,7200);
					else
					{
						for(int m=0;m<xlrowspan;m++)
						{
							this->FRowHeights[row + sRow + m] = ::MulDiv((int)(::atof(height)*100),96,7200*xlrowspan);
						}
						if(xlrowspan>1) sRow += xlrowspan - 1;
						continue;
					}
				}else
				{
					if(xlrowspan <=1 )
						this->FRowHeights[row + sRow] = ::atol(height);
					else
					{
						for(int m=0;m<xlrowspan;m++)
						{
							this->FRowHeights[row + sRow + m] = ::atol(height)/xlrowspan;
						}
						if(xlrowspan > 1) sRow += xlrowspan - 1;
						continue;
					}
				}
			}
			//***********************************************************

			startcol = 0;
			int ll_cid;

			col =-1;
			if(ll_xmlId == 51)
			{
				int l = 1;
			}
			while(ll_xmlId <= nvhtml.is_elements.GetUpperBound())
			{
				if(nvhtml.il_levels[ll_xmlId] <=nvhtml.il_levels[ll_id])break;
				element = nvhtml.is_elements[ll_xmlId];
				element.MakeLower();
				if(element !="td" && element !="th")
				{
					ll_xmlId ++;
					continue;
				}
				col++;
				ll_cid = ll_xmlId;
				ll_xmlId ++;

				//get startcol*********
				for(;startcol<this->GetColCount(); startcol++)
				{
					if(this->isSpaned(startcol+sCol,row+sRow) == FALSE)break;
				}

				nvhtml.of_getattrib(ll_cid,"rowspan",val);
				if(val.IsEmpty()) val="1";
				rowspan = ::atol(val);

				nvhtml.of_getattrib(ll_cid,"colspan",val);
				if(val.IsEmpty()) val="1";
				colspan = ::atol(val);
				
				if(rowspan>=1 ||colspan>=1)
				{

					nvhtml.of_getattrib(ll_cid,"style",val);
					CRect	rect;
					rect.SetRect(startcol + sCol,row + sRow,startcol + sCol + colspan - 1,row + sRow + rowspan - 1);
					m_spans.Add(rect);
					if(::strstr(val,"mso-ignore:colspan"))unColSpans.Add(m_spans.GetUpperBound());
					if(::strstr(val,"mso-ignore:rowspan"))unRowSpans.Add(m_spans.GetUpperBound());
				}

				//bkcolor
				long	bkColor;
				bkColor = 0;
				nvhtml.of_getattrib(ll_cid,"style",val);
				if(::strstr(val,"background-color:"))
				{
					val+=";";
					ls_style = ::strstr(val,"background-color:")+strlen("background-color:");
					ls_style = ls_style.Left(ls_style.Find(";"));
					if(ls_style!="" && !ls_style.IsEmpty())
					{
						ls_style = ls_style.Mid(5,2) + ls_style.Mid(3,2) + ls_style.Mid(1,2);
						bkColor = this->Hex(ls_style);
					}
				}
	#ifdef EDIT
				//set content
				contents="";
				content ="";
				ls_styles="";
				ls_style="";
				LOGFONT	logFont;
				this->m_Fonts[0]->GetLogFont(&logFont);
				for(int i =ll_cid ;i<=nvhtml.is_elements.GetUpperBound();i++)
				{
					if(i >ll_cid && nvhtml.il_levels[i]<=nvhtml.il_levels[ll_cid])break;
					nvhtml.of_getattrib(i,"style",ls_style);
					if(!(ls_style.IsEmpty() || ls_style==""))ls_styles +=";" + ls_style;
					nvhtml.of_getattrib(i,"align",ls_style);
					if(!(ls_style.IsEmpty() || ls_style==""))
					{
						ls_styles +=";text-align:" + ls_style;
					}
					nvhtml.of_getattrib(i,"valign",ls_style);
					if(!(ls_style.IsEmpty() || ls_style==""))ls_styles +=";text-valign:" + ls_style;

					tag = nvhtml.is_elements[i];
					tag.MakeLower();
					if(tag=="p" || (CString)tag=="br")
					{
						if(contents!="" && contents.IsEmpty())contents += "&ln;";
					}
					if(tag=="b")logFont.lfWeight = FW_BOLD;
					if(tag=="i")logFont.lfItalic = TRUE;
					if(tag=="u")logFont.lfUnderline = TRUE;
	#ifndef  HXDLL
					nvhtml.of_getattrib(i,"value",content);
					content.TrimLeft("\r\n\t ");
					if(content=="" || content.IsEmpty())
					{
						content = nvhtml.is_contents[i];
						content.TrimLeft("\r\n\t ");
						content.MakeLower();
						if(content=="http:" ||content=="mailto:" ||content=="ftp:")content="";
					}else
					{
						if(content.Find("{{")>=0)
						{
							PStrItem	pItem;
							pItem = NewStrItem();
							pItem->mask = STI_LINK|STI_TEXTCOLOR;
							pItem->FTextColor=0x99f;

							CString	csStr;
							csStr = content.Mid(content.Find("{{")+2,content.Find("}}") - content.Find("{{") - 2);
							this->UnXMLText(csStr);

							pItem->FLink = new char[content.Find("}}") - content.Find("{{") - 1];
							::strcpy(pItem->FLink,csStr);
							this->SetCellsItem(startcol + sCol,row + sRow,pItem);
							DisposeStrItem(pItem);
							content = content.Left(content.Find("{{"));
						}
					}
	#else
					content = nvhtml.is_contents[i];
					content.TrimLeft("\r\n\t ");
	#endif
					if(!content.IsEmpty()) contents +=  content;
				}
				ls_style.MakeLower();
				ls_styles +=";";
				contents.Replace("\r","");
				contents.Replace("\n","");
				contents.Replace("&nbsp;"," ");
				contents.Replace("&ln;","\r\n");
				contents.Replace("&amp;","&&");
				if(contents!="")
				{
					this->SetCells(startcol + sCol,row + sRow,contents);
					cols.Add(startcol + sCol);
					rows.Add(row + sRow);
				}
				if(::strstr(ls_styles,"font-family:"))
				{
					ls_style = ::strstr(ls_styles,"font-family:")+strlen("font-family:");
					ls_style = ls_style.Left(ls_style.Find(";"));
					if(ls_style!="" && !ls_style.IsEmpty() && ls_style.GetLength()<30)
					{
						::strcpy(logFont.lfFaceName ,ls_style);
					}
				}
				if(::strstr(ls_styles,"font-size:"))
				{
					ls_style = ::strstr(ls_styles,"font-size:")+strlen("font-size:");
					ls_style = ls_style.Left(ls_style.Find(";"));
					if(ls_style!="" && !ls_style.IsEmpty())
					{
						if(::strstr(ls_style,"pt"))
						{
							ls_style = ls_style.Left(ls_style.Find("pt"));
							logFont.lfHeight = -1*::MulDiv((int)(::atof(ls_style)*100),96,7200); 
						}else
						{
							logFont.lfHeight = -1* ::atol(ls_style);
						}
					}
				}
				CFont*	pFont = new CFont();
				pFont->CreateFontIndirect(&logFont);
				this->m_Fonts.Add(pFont);
	#else
				CString	ls_str,ls_ele;
				contents ="";
				contents= nvhtml.is_contents[ll_xmlId - 1];
				contents.TrimLeft("\r\n\t ");
				while(ll_xmlId <= nvhtml.is_elements.GetUpperBound())
				{
					if(nvhtml.il_levels[ll_xmlId] <=nvhtml.il_levels[ll_cid])break;
					if(nvhtml.il_levels[ll_xmlId] != nvhtml.il_levels[ll_cid] +1)
					{
						ll_xmlId ++;
						continue;
					}
					element = nvhtml.is_elements[ll_xmlId];
					element.MakeLower();
					if(element == "hrt:sets" || element=="table")
					{
						if(::GetKeyState(VK_CONTROL)>=0)
						{
							int	ll_tlevel;
							ll_tlevel = nvhtml.il_levels[ll_xmlId];

							CString rcount,ccount;
							int ircount,iccount;
							nvhtml.of_getattrib(ll_xmlId,"rowcount",rcount);
							nvhtml.of_getattrib(ll_xmlId,"colcount",ccount);
					
							PStrItem	pItem;
							pItem = ::NewStrItem();
							pItem->mask =STI_TABLE;
							

							ircount = ::atol(rcount);
							iccount = ::atol(ccount);
							if(ircount < 1)ircount = 3;
							if(iccount < 1)iccount = 12;

							//add table
							CStringGrid * pGrid;
							pGrid = new CStringGrid();
							pGrid->SetView(NULL);
							pGrid->FScrollBars = ssNone;
							pGrid->FRowCount = ircount + 1;
							pGrid->FColCount = iccount;
							
							pGrid->FChild = TRUE;
							pGrid->FParentGrid = this;
							pGrid->FCurChildGrid = NULL;
							pGrid->FMainGrid = FALSE;
							
							int row1 = 1,col1 = 1;
							if(element !="hrt:sets")
								pGrid->LoadTable(nvhtml,row1,col1,ll_xmlId);
							else
								pGrid->LoadTable(nvhtml,row1,col1,ll_xmlId + 2);
							pGrid->FRowCount --;

							element = nvhtml.is_elements[ll_xmlId];
							pGrid->m_pView = this->m_pView;
							pItem->FTable = pGrid;
							
							this->SetCellsItem(startcol + sCol,row + sRow,pItem);
							pItem->FTable = NULL;
							::DisposeStrItem(pItem);

							cols.Add(startcol + sCol);
							rows.Add(row + sRow);
							
							ll_xmlId ++ ;
							while(ll_xmlId <= nvhtml.is_elements.GetUpperBound())
							{
								if(nvhtml.il_levels[ll_xmlId] <=ll_tlevel)break;
								ll_xmlId ++;
							}
							
							contents = "";
							continue;
						}else
						{
							nvhtml.of_getxml(ll_xmlId,ls_str);
							contents += ls_str;
						}
					}else
					{
						nvhtml.of_getxml(ll_xmlId,ls_str);
						contents += ls_str;
					}
					ll_xmlId++;
				}
				if(contents!="")
				{
					this->SetCells(startcol + sCol,row + sRow,contents);
					cols.Add(startcol + sCol);
					rows.Add(row + sRow);
				}

	#endif
				//set colWidth
				bExtent = TRUE;
				if(colspan==1)
				{
					nvhtml.of_getattrib(ll_cid,"width",width);
					if(width.IsEmpty())
					{
						nvhtml.of_getattrib(ll_cid,"style",width);
						width+=";";
						if(::strstr(width,"width:"))
						{
							width = ::strstr(width,"width:") + strlen("width:");
							width = width.Left(width.Find(";"));
						}else
						{
							bExtent = FALSE;
						}
					}
					if(bExtent)
					{
						if(::strstr(width,"pt"))
						{
							width = width.Left(width.Find("pt"));
							this->FColWidths[startcol + sCol] = ::MulDiv((int)(::atof(width)*100),96,7200); 
						}else
						{
							this->FColWidths[startcol + sCol] = ::atol(width);
						}
					}
				}
				
				//set rowHeight
				bExtent = TRUE;
				if( rowspan==1)
				{
					nvhtml.of_getattrib(ll_cid,"height",height);
					if(height.IsEmpty())
					{
						nvhtml.of_getattrib(ll_cid,"style",height);
						height+=";";
						if(::strstr(height,"height:"))
						{
							height = ::strstr(height,"height:") + strlen("height:");
							height = height.Left(height.Find(";"));
						}else
						{
							bExtent = FALSE;
						}
					}
					if(bExtent)
					{
						if(::strstr(height,"pt"))
						{
							height = height.Left(height.Find("pt"));
							this->FRowHeights[row + sRow] = ::MulDiv((int)(::atof(height)*100),96,7200); 
						}else
						{
							this->FRowHeights[row + sRow] = ::atol(height);
						}
					}
				}

				//setborder && Font &&align 
				PStrItem	pItem;
				BOOL		bNewItem;
				ls_styles.MakeLower();
				pItem = this->GetCellsItem(startcol+sCol,row + sRow);
				{
					bNewItem = FALSE;
					if(!pItem)
					{
						pItem = NewStrItem();
						bNewItem = TRUE;
					}
					//font
					pItem->FFontIndex = this->m_Fonts.GetUpperBound();
					//align
					if(::strstr(ls_styles,"text-align:"))
					{
						ls_style = ::strstr(ls_styles,"text-align:")+strlen("text-align:");
						ls_style = ls_style.Left(ls_style.Find(";"));
						if(ls_style!="" && !ls_style.IsEmpty() && ls_style.GetLength()<30)
						{
							if(ls_style=="center")
							{
								pItem->FAlign |= DT_CENTER;
								pItem->FAlign &= (0xFFFFFFFFF^DT_LEFT);
								pItem->FAlign &= (0xFFFFFFFFF^DT_RIGHT);
							}
							if(ls_style=="right")
							{
								pItem->FAlign |= DT_RIGHT;
								pItem->FAlign &= (0xFFFFFFFFF^DT_CENTER);
								pItem->FAlign &= (0xFFFFFFFFF^DT_LEFT);
							}
						}
					}
					//valign
					if(::strstr(ls_styles,"text-valign:"))
					{
						ls_style = ::strstr(ls_styles,"text-valign:")+strlen("text-valign:");
						ls_style = ls_style.Left(ls_style.Find(";"));
						if(ls_style!="" && !ls_style.IsEmpty() && ls_style.GetLength()<30)
						{
							if(ls_style=="vcenter")
							{
								pItem->FAlign |= DT_VCENTER|DT_SINGLELINE;
								pItem->FAlign &= (0xFFFFFFFFF^DT_TOP);
								pItem->FAlign &= (0xFFFFFFFFF^DT_BOTTOM);
							}
							if(ls_style=="bottom")
							{
								pItem->FAlign |= DT_BOTTOM|DT_SINGLELINE;
								pItem->FAlign &= (0xFFFFFFFFF^DT_TOP);
								pItem->FAlign &= (0xFFFFFFFFF^DT_VCENTER);
							}
						}
					}
					//color
					if(::strstr(ls_styles,"color:"))
					{
						ls_style = ::strstr(ls_styles,"color:")+strlen("color:");
						ls_style = ls_style.Left(ls_style.Find(";"));
						if(ls_style!="" && !ls_style.IsEmpty())
						{
							ls_style = ls_style.Mid(5,2) + ls_style.Mid(3,2) + ls_style.Mid(1,2);
							if(!pItem->FLink)pItem->FTextColor = this->Hex(ls_style);
						}
					}
					if(bkColor !=0)pItem->FBkColor=bkColor;

					CString	stemp,svalue;
					int bleft=-1,bright=-1,btop = -1,bbottom = -1,bborder=-1;
					nvhtml.of_getattrib(ll_cid,"style",val);
					val.MakeLower();
					val +=";";
					while(val.Find(";")>=0)
					{
						stemp = val.Left(val.Find(";"));
						val = val.Mid(val.Find(";") + 1);
						if(stemp.Find(":")>=0)
						{
							svalue = stemp.Mid(stemp.Find(":")+1);
							stemp = stemp.Left(stemp.Find(":"));
							stemp.TrimLeft();
							stemp.TrimRight();
							if(stemp =="border" && svalue.Find("solid")>=0)bborder = 1;
							if(stemp =="border" && svalue.Find("none")>=0)bborder = 0;
							if(stemp =="border-top" && svalue.Find("solid")>=0)btop = 1;
							if(stemp =="border-top" && svalue.Find("none")>=0)btop = 0;
							if(stemp =="border-left" && svalue.Find("solid")>=0)bleft = 1;
							if(stemp =="border-left" && svalue.Find("none")>=0)bleft = 0;
							if(stemp =="border-bottom" && svalue.Find("solid")>=0)bbottom = 1;
							if(stemp =="border-bottom" && svalue.Find("none")>=0)bbottom = 0;
							if(stemp =="border-right" && svalue.Find("solid")>=0)bright = 1;
							if(stemp =="border-right" && svalue.Find("none")>=0)bright = 0;
						}
					}
					if(tableBorder)
					{
						if(bborder==0)
						{
							pItem->FBorder = 0x0;
							if(btop == 1)pItem->FBorder |=0x01;
							if(bbottom==1)pItem->FBorder |=0x02;
							if(bleft==1)pItem->FBorder |=0x04;
							if(bright==1)pItem->FBorder |= 0x08;
						}else
						{
							pItem->FBorder = 0x0;
							pItem->FBorder |=0x01|0x02|0x04|0x08;
							if(btop==0)pItem->FBorder &= (0xFFFFFFFF^0x01);
							if(bbottom==0)pItem->FBorder &=(0xFFFFFFFF^0x02);
							if(bleft==0)pItem->FBorder &=(0xFFFFFFFF^0x04);
							if(bright==0)pItem->FBorder &= (0xFFFFFFFF^0x08);
						}
					}else
					{
						if(bborder==1)
						{
							pItem->FBorder |=0x01|0x02|0x04|0x08;
							if(btop==0)pItem->FBorder &= (0xFFFFFFFF^0x01);
							if(bbottom==0)pItem->FBorder &=(0xFFFFFFFF^0x02);
							if(bleft==0)pItem->FBorder &=(0xFFFFFFFF^0x04);
							if(bright==0)pItem->FBorder &= (0xFFFFFFFF^0x08);
						}else
						{
							if(btop==1)pItem->FBorder |=0x01;
							if(bbottom==1)pItem->FBorder |=0x02;
							if(bleft==1)pItem->FBorder |=0x04;
							if(bright==1)pItem->FBorder |= 0x08;
						}
					}

					if(bNewItem /*&& pItem->FBorder !=0*/)
					{
						pItem->mask = STI_ENTRY;
						this->SetCellsItem(startcol + sCol,row + sRow,pItem);
					}
				}
				//adjust startcol
				startcol += colspan;
			}
			//adjust start columnCount
			if(colCount <startcol - 1) colCount = startcol - 1;

			//firstStr
			nvhtml.of_getattrib(ll_id,"firststr",val);
			this->UnXMLText(val);
			if(!(val.IsEmpty() && val ==""))
			{
				PStrItem	pItem = NewStrItem();
				pItem->mask = STI_PRESTR;
				pItem->FPreStr = new char[val.GetLength()+1];
				::strcpy(pItem->FPreStr,val);
				this->SetCellsItem(0,row + sRow,pItem);
				DisposeStrItem(pItem);
			}
			
			//endStr
			nvhtml.of_getattrib(ll_id,"laststr",val);
			this->UnXMLText(val);
			if(!(val.IsEmpty() && val ==""))
			{
				PStrItem	pItem = NewStrItem();
				pItem->mask = STI_LASTPRE;
				pItem->FLastPre = new char[val.GetLength()+1];
				::strcpy(pItem->FLastPre,val);
				this->SetCellsItem(0,row + sRow,pItem);
				DisposeStrItem(pItem);
			}
		}

		if(this->FMainGrid)
		{
			nvhtml.of_finditems(0,"//hrt:objs/div",ll_ids);
			for(int i=0;i<=ll_ids.GetUpperBound();i++)
			{
				CDrawRect*	pDrawObj;
				pDrawObj = new CDrawRect(CRect(0,0,0,0),CDrawRect::rectangle);

				int	top,left,height,width;
				
				CString	ls_style;
				CString	value;
				nvhtml.of_getattrib(ll_ids[i],"style",ls_style);
				ls_style +=";";

				//height
				if(::strstr(ls_style,"height:"))
				{
					value = ::strstr(ls_style,"height:") + strlen("height:");
					value = value.Left(value.Find(";"));
					height = ::atol(value);
				}
				//width
				if(::strstr(ls_style,"width:"))
				{
					value = ::strstr(ls_style,"width:") + strlen("width:");
					value = value.Left(value.Find(";"));
					width = ::atol(value);
				}
				//top
				if(::strstr(ls_style,"top:"))
				{
					value = ::strstr(ls_style,"top:") + strlen("top:");
					value = value.Left(value.Find(";"));
					top = ::atol(value);
				}
				//left
				if(::strstr(ls_style,"left:"))
				{
					value = ::strstr(ls_style,"left:") + strlen("left:");
					value = value.Left(value.Find(";"));
					left = ::atol(value);
				}
				pDrawObj->m_position.SetRect(left + FBound.left,top + FBound.top,left + FBound.left +width,top  + FBound.top + height);
	#ifdef HXDLL			
				nvhtml.of_getChildXml(ll_ids[i],pDrawObj->m_Content);
				if(pDrawObj->m_Content.Find("<xml>")>0)
					pDrawObj->m_Content = pDrawObj->m_Content.Left(pDrawObj->m_Content.Find("<xml>"));
	#else
				nvhtml.of_finditems(ll_ids[i],"xml",ll_items);
				if(ll_items.GetUpperBound()>=0)
				{
					 nvhtml.of_getChildXml(ll_items[0],pDrawObj->m_Content);
				}
				else
					nvhtml.of_getChildXml(ll_ids[i],pDrawObj->m_Content);
	#endif
				this->Add(pDrawObj);
			}
		}

		nvhtml.of_finditems(ll_start,"xml/hxcell",ll_ids);
		if(ll_ids.GetUpperBound() < 0  && FMainGrid)
			nvhtml.of_finditems(0,"//xml/hxcell",ll_ids);
		if(ll_ids.GetUpperBound() >=0)
		{
			int row,col;
			CString	val;
			nvhtml.of_finditems(ll_ids[ll_ids.GetUpperBound()],"item",ll_ids);
			for(int i=0;i<=ll_ids.GetUpperBound();i++)
			{
				nvhtml.of_finditems(ll_ids[i],"row",ll_items);
				if(ll_items.GetUpperBound()>=0)nvhtml.of_getitem(ll_items[0],"content",val);
				row = ::atol(val);
				nvhtml.of_finditems(ll_ids[i],"col",ll_items);
				if(ll_items.GetUpperBound()>=0)nvhtml.of_getitem(ll_items[0],"content",val);
				col = ::atol(val);
				if(row>=0 && col>=0 && row<=FRowCount - 1 && col<=FColCount - 1)
				{
					PStrItem	pItem;
					pItem = ::NewStrItem();
					//前缀
					nvhtml.of_finditems(ll_ids[i],"前缀",ll_items);
					if(ll_items.GetUpperBound()>=0)
					{
						nvhtml.of_getitem(ll_items[0],"content",val);
						if((!val.IsEmpty()) && val != "")
						{
							pItem->FPreStr = new char[::strlen(val)+1];
							::strcpy(pItem->FPreStr,val);
							pItem->mask = STI_PRESTR;
						}
					}
					//后缀
					nvhtml.of_finditems(ll_ids[i],"后缀",ll_items);
					if(ll_items.GetUpperBound()>=0)
					{
						nvhtml.of_getitem(ll_items[0],"content",val);
						if((!val.IsEmpty()) && val != "")
						{
							pItem->FLastPre = new char[::strlen(val)+1];
							::strcpy(pItem->FLastPre,val);
							pItem->mask = STI_LASTPRE;
						}
					}
					//公式
					nvhtml.of_finditems(ll_ids[i],"公式",ll_items);
					if(ll_items.GetUpperBound()>=0)
					{
						nvhtml.of_getitem(ll_items[0],"content",val);
						if((!val.IsEmpty()) && val != "")
						{
							pItem->FFormula = new char[::strlen(val)+1];
							::strcpy(pItem->FFormula,val);
							pItem->mask = STI_FORMULA;
						}
					}
					//链接
					nvhtml.of_finditems(ll_ids[i],"链接",ll_items);
					if(ll_items.GetUpperBound()>=0)
					{
						nvhtml.of_getitem(ll_items[0],"content",val);
						if((!val.IsEmpty()) && val != "")
						{
							pItem->FLink = new char[::strlen(val)+1];
							::strcpy(pItem->FLink,val);
							pItem->mask = STI_LINK;
						}
					}
					//备注
					nvhtml.of_finditems(ll_ids[i],"备注",ll_items);
					if(ll_items.GetUpperBound()>=0)
					{
						nvhtml.of_getitem(ll_items[0],"content",val);
						if((!val.IsEmpty()) && val != "")
						{
							pItem->FNote = new char[::strlen(val)+1];
							::strcpy(pItem->FNote,val);
							pItem->mask = STI_NOTE;
						}
					}
					//其他
					nvhtml.of_finditems(ll_ids[i],"其他",ll_items);
					if(ll_items.GetUpperBound()>=0)
					{
						nvhtml.of_getitem(ll_items[0],"content",val);
						if((!val.IsEmpty()) && val != "")
						{
							pItem->FXml = new char[::strlen(val)+1];
							::strcpy(pItem->FXml,val);
							pItem->mask = STI_XML;
						}
					}
					//数据列
					nvhtml.of_finditems(ll_ids[i],"数据列",ll_items);
					if(ll_items.GetUpperBound()>=0)
					{
	#ifndef  HXDLL
						nvhtml.of_getitem(ll_items[0],"content",val);
						if((!val.IsEmpty()) && val != "")
						{
							pItem->FString = new char[::strlen(val)+1];
							::strcpy(pItem->FString,val);
							pItem->mask = STI_STRING;
						}
	#endif
					}
					if(pItem->mask)this->SetCellsItem(col,row,pItem);
					::DisposeStrItem(pItem);
				}
			}
		}

		int	i;
	#ifdef EDIT
		//recalc height
		CRect	rect,cRect;
		PStrItem	pItem;
		HFONT	hFont;
		int		ll_width,ll_height,fontIndex;
		for(i=rows.GetUpperBound();i>=0;i--)
		{
			pItem = this->GetCellsItem(cols[i],rows[i]);
			if(!pItem)continue;
			if(!pItem->FString)continue;
			this->isSpaned(cols[i],rows[i],rect);

			ll_width = this->GetColWidths(rect.left);
			for(int j=rect.left + 1;j<=rect.right;j++)
			{
				ll_width += this->FGridLineWidth + this->GetColWidths(j);
			}

			ll_height = this->GetRowHeights(rect.top);
			for(j=rect.top+1;j<=rect.bottom;j++)
			{
				ll_height += this->FGridLineWidth + this->GetRowHeights(j);
			}
			fontIndex = pItem->FFontIndex;
			if(fontIndex<0)fontIndex = 0;
			hFont = (HFONT)::SelectObject(dc,this->m_Fonts[fontIndex]->GetSafeHandle());
			cRect.SetRect(0,0,ll_width - 4,ll_height - 4);
			::DrawText(dc,pItem->FString,strlen(pItem->FString),cRect,pItem->FAlign|DT_CALCRECT|DT_WORDBREAK);
			if(cRect.Height()>ll_height - 4)this->FRowHeights[rows[i]] = 
				this->GetRowHeights(rows[i])+cRect.Height() - ll_height + 4;
			::SelectObject(dc,hFont);
		}
	#endif

		for(i=0;i<unColSpans.GetUpperBound();i++)
		{
			m_spans[unColSpans[i]].right = m_spans[unColSpans[i]].left;
		}
		for(i=0;i<unRowSpans.GetUpperBound();i++)
		{
			m_spans[unRowSpans[i]].bottom = m_spans[unRowSpans[i]].top;
		}

		RefreshSpans();

		if(this->FMainGrid)
		{
			for(i=1;i<this->GetRowCount();i++)
			{
				if(this->GetRowHeights(i)==0)
				{
					if(i>1)this->DeleteRow(i);
					if(i>1)this->DeleteRow(i - 1);
					this->SetRow(1);
					break;
				}
			}
		}

		return ll_xmlId;
	}

	BOOL CStringGrid::LoadTable(CString &html)
	{
		hxhtml	nvhtml;
		nvhtml.of_loadxml(html);
		
		HCURSOR	hCursor;
		hCursor = ::SetCursor(LoadCursor(NULL,IDC_WAIT));
		CArray<long,long>	ll_ids;
		int sCol = 1,sRow = 1;

		nvhtml.of_finditems(0,"//TABLE",ll_ids);
		if(ll_ids.GetUpperBound()<0) return FALSE;
		this->FMainGrid = TRUE;
		this->LoadTable(nvhtml,sCol,sRow,ll_ids[0]);
	//	this->OnSetBorder(sCol,sRow,sCol + colCount - 1 + 1,sRow + ll_ids.GetUpperBound() - 1,IDM_ABORDER);
	//	this->SetColWidths(1,10);
		this->ShowGrid(FALSE);
		::SetCursor(hCursor);

		m_style = nvhtml.m_style;
		return TRUE;
	}
#endif
}}}}
