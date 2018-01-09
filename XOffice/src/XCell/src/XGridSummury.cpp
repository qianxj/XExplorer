#include "StdAfx.h"
#include "XGridSummury.hpp"
#include "XGridDataSvrEx.hpp"
#include "XGridReport.hpp"
#include "XGridSheetSvrEx.hpp"
#include "XGridComplier.hpp"

extern HMODULE hCellModula;
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridSummury::XGridSummury(void):m_pDataTempl(NULL), m_nTotailHeight(0)
	{
	}

	XGridSummury::~XGridSummury(void)
	{
	}

	void XGridSummury::OnDraw(HDC hdc,RECT &xrect)
	{
		if(!m_pDataTempl) return;
		if(!m_pGridReportSvr) return;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		if(!pReportBand->Summury.bValid) return;

		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return;

		int startRow,endRow;
		startRow = pReportBand->Summury.startRow;
		endRow = pReportBand->Summury.endRow - 1;
		if(startRow > endRow) return;

		LOGFONT	logFont;
		XUtil::Css::GetDefaultFont(logFont);
		HFONT hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);
		logFont.lfWeight = FW_BOLD;
		HFONT	hFontB =::CreateFontIndirect(&logFont);
		int hbr = ::SetBkMode(hdc,TRANSPARENT);

		int StartCol = m_pGridReportSvr->FTopLeft.x;
		int StartRow = m_pGridReportSvr->FTopLeft.y;

		int startCell = m_pGridReportSvr->FTopLeft.x;
		int LineX = 0;
		int ACol ;

		int EndCol = 0;
		bool bDraw = true;
		int heights = 0;
		for(int ARow = startRow;ARow <=endRow; ARow++)
		{
			heights += m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow);
			ACol = m_pDataTempl->m_pContentSvr->GetValidCol(ARow);
			TGridRect gRect;
			m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect);
			if(gRect.right > EndCol) EndCol = gRect.right;
		}

		RECT rect;
		::SetRect(&rect,xrect.left,xrect.bottom - heights,xrect.right,xrect.bottom);
		HBRUSH hBrush = ::CreateSolidBrush(RGB(225,229,238));
		::FillRect(hdc,&rect,hBrush);
		::DeleteObject(hBrush);
		xrect.bottom -= heights;

		//LPTSTR pCssStr = _tcsdup(_T("background-color:#9999CC"));
		//XUtil::Css::TBoxStyle * pStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
		//delete pCssStr;

		int LineY = rect.top  + 1;
		for(int ARow = startRow;ARow <=endRow; ARow++)
		{
			RECT	ARect;
			::SetRect(&ARect,0,0,0,0);
			ARect.top = LineY;
			ARect.left = rect.left + pAxisSvr->GetColWidth(0) + 1;
			ARect.bottom  = ARect.top + m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow);// pAxisSvr->GetRowHeight(ARow);
			
			LineX =  ARect.left;
			ACol = startCell;
			TGridRect Selection = m_pGridReportSvr->GetSelection();
			while(LineX <= rect.right )
			{
				ARect.bottom  = ARect.top + m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow) + 1;// pAxisSvr->GetRowHeight(ARow);
				
				int i=0;
				bDraw = true;
				if(ACol <= EndCol)
				{
					if(pAxisSvr->GetColWidth(ACol) <= 1)
					{
						ACol++;
						continue;
					}
					const XUtil::Css::TBoxStyle* pBoxStyleEx = m_pDataTempl->GetStyleSvr()->GetCellCssStyle(ACol,ARow);
					XUtil::Css::TBoxStyle* pStyle = (XUtil::Css::TBoxStyle*)pBoxStyleEx;	

					TCHAR * pText = NULL;
					TGridRect gRect;
					bDraw = true;
					PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow);
					if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
					{
						if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
						{
							if(pItem && pItem->pStrContent && _tcscmp(pItem->pStrContent, L"") != 0) 
								pText = pItem->pStrContent;
							//else
							//	bDraw = false;
						}else
							bDraw = false;

						ARect.right = ARect.left - 1;
						while(ACol <= gRect.right)
						{
							ARect.right += pAxisSvr->GetColWidth(ACol) + 1;
							ACol++;
						}
						if(bDraw)
						{
							int nRow = ARow;
							ARect.bottom = ARect.top + 0;
							while(nRow <= gRect.bottom)
							{
								ARect.bottom += m_pDataTempl->m_pAxisSvr->GetRowHeight(nRow) + 1; //pAxisSvr->GetRowHeight(nRow) + 1;
								nRow++;
							}
						}
						ACol = gRect.right;
					}else
					{
						ARect.right = ARect.left + pAxisSvr->GetColWidth(ACol); 
						if(pItem && pItem->pStrContent && _tcscmp(pItem->pStrContent, L"") != 0) 
							pText = pItem->pStrContent;
						//else
							//bDraw = false;
					}

					COLORREF TextColor = ::GetTextColor(hdc);
					HFONT hFont2 = NULL;
					UINT nAlign = DT_CENTER;
					if(bDraw)
					{
						XUtil::Css::DrawCssFrame(hdc,&ARect,pStyle);
						
						LOGFONT	LogFont2;
						if(m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->GetFont(ACol,ARow,LogFont2) == 1)
						{
							hFont2 = ::CreateFontIndirect(&LogFont2);
							hFont2 = (HFONT)::SelectObject(hdc,hFont2);
						}
						if(pStyle)
						{
							if(pStyle->pColor && pStyle->mask & cssMaskColor)
							{
								::SetTextColor(hdc,pStyle->pColor->color[0]);
							}
							if(pStyle->pText && pStyle->mask & cssMaskText &&
								pStyle->pText->mask & cssMaskTextAlign)
							{
								if(pStyle->pText->textalign == XUtil::Css::_left)
									nAlign = DT_LEFT;
								else if(pStyle->pText->textalign == XUtil::Css::_right)
									nAlign = DT_RIGHT;
								else if(pStyle->pText->textalign == XUtil::Css::_center)
									nAlign = DT_CENTER;
							}
						}
						//if(pStyle)	delete pStyle;
					}

					LPTSTR pCellSource = NULL;
					LPTSTR pEditFormat = NULL;
					LPTSTR pDataType = NULL;
					if(pItem && pItem->pStrXml)
					{
						IXMLDOMElement * pElement = NULL;
						IXMLDOMElement * pElement1 = NULL;
						IXMLDOMNode * pNode;
						LPTSTR pEditType = NULL;
						_variant_t var;

						xbXml xml;
						xml.LoadXml(pItem->pStrXml);
						xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
						if(pNode)
						{
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
							pElement->getAttribute(L"edittype",&var);
							if(var.bstrVal)
								pEditType = _tcsdup(var.bstrVal);
							pNode->Release();
							//if(pEditType) delete pEditType;
						}
						xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
						if(pNode)
						{
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
							pElement1->getAttribute(L"celltype",&var);
							if(var.bstrVal)
								pDataType = _tcsdup(var.bstrVal);
							pElement1->getAttribute(L"cellformat",&var);
							if(var.bstrVal)
								pEditFormat = _tcsdup(var.bstrVal);	
							pElement1->getAttribute(L"cellsource",&var);
							if(var.bstrVal)
								pCellSource = _tcsdup(var.bstrVal);
							pNode->Release();
							pElement1->Release();
						}
					}
					if(pCellSource)
					{
						if(pCellSource[0]=='=')
						{
							CXGridComplier x;
							if(x.SetSource(pCellSource + 1, this->m_pGridReportSvr))
								pText = x.GetString();
						}
						if(pCellSource) delete pCellSource;
					}

					if(pText)
					{
						if(pText[0]=='=')
						{
							CXGridComplier x;
							LPTSTR pText2 = ::_tcsdup(pText + 1);
							if(x.SetSource(pText2, this->m_pGridReportSvr))
								pText = x.GetString();
							if(pText2) delete pText2;
						}

						LPTSTR pItemStr = (LPTSTR)_tcsdup(pText);
						bool bDelete = true;
						if(pDataType && _tcscmp(pDataType,L"xs:decimal")==0)
						{
							if(pItemStr[0]=='.')
							{
								int nlen = _tcslen(pItemStr);
								LPTSTR pTemp = new TCHAR[nlen + 2];
								pTemp[0]='0';
								wcsncpy_s(pTemp + 1,nlen + 1,pItemStr,nlen);
								if(bDelete) delete pItemStr;
								bDelete = true;
								pItemStr = pTemp;
							}
							if(pEditFormat && _tcscmp(pEditFormat,L"")!=0)
							{
								pItemStr = XUtil::FormatDecimalString(pItemStr,pEditFormat,bDelete);
							}

						}
						else if(pDataType && _tcscmp(pDataType,L"xs:date")==0)
						{
							if(pEditFormat && (_tcscmp(pEditFormat,L"yyyy-mm-dd")==0 || _tcscmp(pEditFormat,L"yyyy/mm/dd")==0))
							{
								int len = 0;
								while(pItemStr[len] != ' ' && pItemStr[len] != 'T'&& pItemStr[len] != '\0') len++;
								pItemStr[len--]='\0';
							}
						}
						if(pEditFormat) delete pEditFormat;
						if(pDataType) delete pDataType;

						if(_tcschr(pItemStr,'\r'))
							::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&ARect,nAlign|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
						else
						{
							SIZE sz;
							GetTextExtentPoint32(hdc,pItemStr,(int)::_tcslen(pItemStr),&sz);
							if(sz.cx < ARect.right - ARect.left)
								::DrawText(hdc,pItemStr,(int)::_tcslen(pItemStr),&ARect,nAlign|DT_VCENTER|DT_SINGLELINE|DT_VCENTER|DT_EDITCONTROL|DT_NOPREFIX);
							else
								::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&ARect,nAlign|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
						}
						if(pItemStr) delete pItemStr;
					}
					if(hFont2)
					{
						hFont2 = (HFONT)::SelectObject(hdc,hFont2);
						::DeleteObject(hFont2);
					}
					::SetTextColor(hdc,TextColor);

					if(m_pGridReportSvr)
					{
						LPCTSTR pColumn = m_pGridReportSvr->GetDwColumnAtCellColumn(ACol);
						if(pColumn)
						{
							for(int i=0;i<(int)m_pGridReportSvr->m_pSortColumns->size();i++)
							{
								if(_tcsicmp((*m_pGridReportSvr->m_pSortColumns)[i].c_str(),pColumn)==0)
								{
									POINT points[3];
									HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(128,128,128));
									hPen = (HPEN)::SelectObject(hdc,hPen);
									HBRUSH hBrush = ::CreateSolidBrush(RGB(255,255,0));
									hBrush = (HBRUSH)::SelectObject(hdc,hBrush);

									if((*m_pGridReportSvr->m_pSortOrders)[i])
									{
										points[0].y = ARect.bottom - 8;
										points[0].x = ARect.right - 7;
										points[1].y = ARect.bottom - 3;
										points[1].x = ARect.right - 9;
										points[2].y = ARect.bottom - 3;
										points[2].x = ARect.right - 4;
									}else
									{
										points[0].y = ARect.bottom - 3;
										points[0].x = ARect.right - 7;
										points[1].y = ARect.bottom - 8;
										points[1].x = ARect.right - 9;
										points[2].y = ARect.bottom - 8;
										points[2].x = ARect.right - 4;
									}

									Polygon(hdc, points,3);
									hPen = (HPEN)::SelectObject(hdc,hPen);
									::DeleteObject(hPen);
									hBrush = (HBRUSH)::SelectObject(hdc,hBrush);
									::DeleteObject(hBrush);
								}
							}
						}
					}
					
					ACol++;
					ARect.left = ARect.right + 1;
				}
				else
				{
					break;
				}
				LineX = ARect.left ;
			}
			LineY +=  m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow) + 1;

			m_nTotailHeight = ARect.bottom - ARect.top;
		}

		hFont = (HFONT)::SelectObject(hdc, hFont);
		::DeleteObject(hFont);
		::DeleteObject(hFontB);
		::SetBkMode(hdc,hbr);

		//if(pStyle)	delete pStyle;
	}
}}}}