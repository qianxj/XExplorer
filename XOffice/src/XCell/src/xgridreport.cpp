#include "StdAfx.h"
#include "XGridReport.hpp"
#include "xgridsheetsvrex.hpp"
#include "IGridReportObserver.hpp"
#include "xgridrowselector.hpp"
#include "optionsvr.hpp"
#include "xgridheaderex.hpp"
#include "XGridQueryDlg.hpp"
#include "..\include\IGridReport.hpp"
#include "XGridComplier.hpp"
#include "XGridSummury.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridReport::XGridReport(void):m_pSheetSvr(NULL), m_pDataTempl(NULL),m_SelectBkColor(RGB(128,128,255)),m_pSortColumns(NULL),m_pSortOrders(NULL),
		m_bShared(false),m_pPrimaryReport(NULL),FAskMask(false),m_bSort(false),m_bClickEdit(true),m_pBuffers(0),m_ColDecimalFormat(0),m_ColFormule(0),
		m_pSortColumnTypes(NULL)
	{
		::SetRect(&FRect,0,0,0,0);

		m_pEditSvr = new CEditSvrEx();
		m_pEditSvr->m_pGridReport = this;

		Initialize();
		SetEventListons();
		m_pRowSelects = new XUtil::xuCsArray< XCROW, BYTE>( MAXROW,0);
		m_nSelectionLastRow = 0;
		m_SelectionMode = 0;
		FGridLineHeight = 0;
	}

	XGridReport::~XGridReport(void)
	{
		UnSetEventListons();
		if(m_pRowSelects) delete m_pRowSelects;
		if(m_pSortColumns)delete m_pSortColumns;
		if(m_pSortOrders) delete m_pSortOrders;
		if(m_pSortColumnTypes) delete m_pSortColumnTypes;
		m_ReadOnlyColumn.clear();

		m_pSortOrders = new vector<bool>;
		if(m_pBuffers) delete m_pBuffers;
		if(m_ColDecimalFormat)delete m_ColDecimalFormat;
		if(m_ColFormule)delete m_ColFormule;

	}

	bool XGridReport::DwShareTo(XGridReport * pPrimaryReport)
	{
		m_bShared = true;
		m_pPrimaryReport = pPrimaryReport;

		if(this->GetGridDataSvr()->m_pContentSvr)delete GetGridDataSvr()->m_pContentSvr;
		GetGridDataSvr()->m_pContentSvr = pPrimaryReport->GetGridDataSvr()->m_pContentSvr;
		GetGridDataSvr()->m_bShared = true;
		return true;
	}

	
	int XGridReport::SetAskMask(bool bAskMask)
	{
		FAskMask = bAskMask;
		return 1;
	}

	LPTSTR XGridReport::GetItemFormula(LPTSTR Col)
	{
		if(!Col) return NULL;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return NULL;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return NULL;

		int startRow,endRow;
		startRow = pReportBand->Detail.startRow;
		endRow = pReportBand->Detail.endRow - 1;

		LPTSTR pCellSource = NULL;
		bool isFind = false;
		for(int ARow = startRow;ARow <=endRow; ARow++)
		{
			for(int ACol=1; ACol<=this->GetColCount(); ACol++)
			{
				PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow);

				if(pItem && pItem->pStrColumn)
					if(::_tcscmp(pItem->pStrColumn, Col) != 0) continue;

				if(pItem && pItem->pStrXml)
				{
					IXMLDOMElement * pElement1 = NULL;
					IXMLDOMNode * pNode;
					_variant_t var;

					xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
						pElement1->getAttribute(L"cellsource",&var);
						if(var.bstrVal)
							pCellSource = _tcsdup(var.bstrVal);
						pNode->Release();
						pElement1->Release();
					}
				}
				isFind = true;
				break;
			}
			if(isFind) break;
		}

		return pCellSource;
	}

	void XGridReport::OnBaseDraw(HDC hdc,RECT &rect, int nSDrawCol, int nEDrawCol)
	{
		RECT dRect = rect;
		bool bnake = false;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return;

		CAxisSvr * pAxisSvrEx = GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvrEx) return;

		int startRow,endRow;
		startRow = pReportBand->Detail.startRow;
		endRow = pReportBand->Detail.endRow - 1;

		/*
		LOGFONT	logFont;
		logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("宋体")) + 1, _T("宋体"));
		logFont.lfItalic = 0;
		logFont.lfOrientation = 0;
		logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logFont.lfPitchAndFamily = DEFAULT_PITCH;
		logFont.lfQuality = CLEARTYPE_QUALITY;
		logFont.lfStrikeOut = 0;
		logFont.lfUnderline = 0;
		logFont.lfWeight = FW_NORMAL;
		logFont.lfWidth = 0;

		HFONT	hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);

		logFont.lfWeight = FW_BOLD;
		HFONT	hFontB =::CreateFontIndirect(&logFont);*/
		int hbr;
		hbr = ::SetBkMode(hdc,TRANSPARENT);

		int width;
		width = dRect.right- dRect.left;

		int StartCol = nSDrawCol;
		int StartRow = FTopLeft.y;

		int startCell = StartCol;
		int LineX = 0;
		int ACol ;

		int EndCol = 0;
		bool bDraw = true;
		for(int ARow = startRow;ARow <=endRow; ARow++)
		{
			ACol = m_pDataTempl->m_pContentSvr->GetValidCol(ARow);
			TGridRect gRect;
			m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect);
			if(gRect.right > EndCol) EndCol = gRect.right;
		}

		if(nEDrawCol > 0 && EndCol > nEDrawCol) EndCol = nEDrawCol;

		LPTSTR pCssStr = _tcsdup(_T("background-color:#ebead8 none #ffffff none"));
		XUtil::Css::TBoxStyle * pStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
		delete pCssStr;

		/*if(pStyle)
		{
			RECT rc = dRect;
			rc.left =  dRect.left + pAxisSvr->GetColWidth(0) + 1;
			rc.bottom = dRect.top + this->GetDetailHeight() - 1;
			XUtil::Css::DrawCssFrame(hdc,&rc,pStyle);
		}*/
		
		int LineY = dRect.top  +  pAxisSvrEx->GetRowHeight(0) + 1;
		int RowLineY;
		bool bDestroyText = false;
		int RowCounts = GetRowCount();
		int j=StartRow - 1;
		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		if(((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
		{
			int sRow2 = 0;
			int k;
			for(k=1; k<=RowCounts;k++)
			{
				if(!IsTreeRowShow(k)) continue;
				sRow2++;
				if(sRow2 == StartRow) break;
			}
			StartRow = k;
		}
		bool bShowOrigin = ((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->FShowOrigin;
		for(int row = StartRow;row<=RowCounts;row++)
		{
			if(!IsTreeRowShow(row)) continue;
			j++;

			bool bRowNew = false;
			bool bRowModify = false;
			bool bRowDelete = false;
			if(bShowOrigin)
			{
				LPTSTR vs = NULL;
				vs = (LPTSTR)GetItemStringEx(j, L"@update.new");
				if(vs && _tcscmp(vs, L"1") == 0) bRowNew = true;
				vs = (LPTSTR)GetItemStringEx(j, L"@update.modify");
				if(vs && _tcscmp(vs, L"1") == 0) bRowModify = true;
				vs = (LPTSTR)GetItemStringEx(j, L"@update.delete");
				if(vs && _tcscmp(vs, L"1") == 0) bRowDelete = true;
				//if(vs) delete vs;
				/*
				IXMLDOMElement * pRowElement = GetGridDataSvr()->m_pContentSvr->GetRowElement(row);
				if(pRowElement)
				{
					_variant_t vs;

					pRowElement->getAttribute(L"update.new",&vs);
					if(vs.bstrVal && vs.bstrVal[0]=='1') bNew = true;
					pRowElement->getAttribute(L"update.modify",&vs);
					if(vs.bstrVal && vs.bstrVal[0]=='1') bModify = true;
					pRowElement->getAttribute(L"update.delete",&vs);
					if(vs.bstrVal && vs.bstrVal[0]=='1') bDelete = true;
				}*/
			}

			COLORREF color;
			if(IsRowSelected(j))
				color = ::SetTextColor(hdc,RGB(255,255,255));
			else if(j==FCurrent.y)//if(row==FCurrent.y)
				color = ::SetTextColor(hdc,RGB(0,0,255));
			
			RowLineY = LineY;
			if(IsRowSelected(j))
			{
				RECT	ARect;
				::SetRect(&ARect,0,0,0,0);
				ARect.top = LineY;
				ARect.left = dRect.left + pAxisSvr->GetColWidth(0) + 1;
				ARect.bottom  = ARect.top;
				for(int ARow = startRow;ARow <=endRow; ARow++)
				{
					ARect.bottom +=  pAxisSvr->GetRowHeight(ARow);
					if(ARow != startRow)ARect.bottom += FGridLineHeight;
				}
				ARect.right = rect.right;
				HBRUSH hBrush = ::CreateSolidBrush(m_SelectBkColor);
				::InflateRect(&ARect,-1,-1);
				::FillRect(hdc,&ARect,hBrush);
				DeleteObject(hBrush);
			}
			for(int ARow = startRow;ARow <=endRow; ARow++)
			{
				RECT	ARect;
				::SetRect(&ARect,0,0,0,0);
				ARect.top = LineY-1;
				ARect.left = dRect.left + pAxisSvr->GetColWidth(0) + 1;
				ARect.bottom  = ARect.top +  pAxisSvr->GetRowHeight(ARow);
				LineX =  ARect.left;
				ACol = startCell;
				TGridRect Selection = GetSelection();
				while(LineX <= dRect.right )
				{
					ARect.bottom  = ARect.top + pAxisSvr->GetRowHeight(ARow);
					
					int i=0;
					bDraw = true;
					if(ACol <= EndCol)
					{
						DWASKSTYLENMHDR hdr;
						const XUtil::Css::TBoxStyle* pBoxStyleEx = m_pDataTempl->GetStyleSvr()->GetCellCssStyle(ACol,ARow);
						XUtil::Css::TBoxStyle* pBoxStyle = (XUtil::Css::TBoxStyle*)pBoxStyleEx;

						if(FAskMask)
						{
							memset(&hdr,0,sizeof(DWASKSTYLENMHDR));
							hdr.hwndFrom = GetHWND();
							hdr.idFrom = 0;
							hdr.code = DWV_ASKSTYLE;
							hdr.row = j;
							hdr.col = ACol;
							hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(ACol);
							hdr.rt = ARect;
							::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
							if(hdr.mask & DMS_CSSSTYLE)
							{
								XUtil::Css::TBoxStyle* pBoxStyle1 =  m_pDataTempl->GetStyleSvr()->m_pCssStyle->ParseCssStyle(hdr.pCssStyle);
								*pBoxStyle1 = (*pBoxStyle1) + (*pBoxStyle);
								pBoxStyle = pBoxStyle1;
							}
						}


	
						LPCTSTR pText = NULL;
						TGridRect gRect;
						bDraw = true;
						PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow);
						if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
						{
							if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
							{
								if(pItem && pItem->pStrColumn)
								{
									pText = GetGridDataSvr()->m_pContentSvr->GetItemString(pItem->pStrColumn,row);
								}
								else if(pItem && pItem->pStrContent)
									pText = pItem->pStrContent;
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
								ARect.bottom = ARect.top - 1;
								while(nRow <= gRect.bottom)
								{
									ARect.bottom += pAxisSvr->GetRowHeight(nRow) + 1;
									nRow++;
								}
							}
							ACol = gRect.right;
						}else
						{
							ARect.right = ARect.left + pAxisSvr->GetColWidth(ACol); 
							if(pItem && pItem->pStrColumn)
							{
								//pText = GetGridDataSvr()->m_pContentSvr->GetItemDisplayString(pItem->pStrColumn,row);
								//if(!(pText && pText[0]))pText = GetGridDataSvr()->m_pContentSvr->GetItemString(pItem->pStrColumn,row);
								pText = this->GetItemDisplayString(j, this->GetColumnIndex(pItem->pStrColumn));
								if(!(pText && pText[0]))pText = this->GetItemString(j, this->GetColumnIndex(pItem->pStrColumn));
								bDestroyText = true;
							}
							else if(pItem && pItem->pStrContent)
								pText = pItem->pStrContent;
						}

						if(ARect.right <= ARect.left) bDraw = false;

						if(bDraw)
						{
							if(pBoxStyle && pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
							{
								HBRUSH hBrush;
								hBrush = ::CreateSolidBrush(pBoxStyle->pBackGround->color.color[0]);
								::FillRect(hdc,&ARect,hBrush);
								::DeleteObject(hBrush);
							}

							if(!IsRowSelected(j) && !(pBoxStyle && pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround))
							{
								UINT bkColor;
								bool Protected = false;
								LPTSTR protect = this->GetColumnProp(ARow, ACol,_T("cellprotect"));
								if(protect)
								{
									if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
										_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
									{
										Protected = true;
									}			
								}
								HBRUSH hBrush;
								if(!(FOptions | goAllowEdit))
								{
									if(j % 2 == 0)
										bkColor = RGB(226,229,222);
									else
										bkColor = RGB(242,248,242);
								}else
								{
									bkColor = RGB(236,248,236);
									if(Protected) bkColor =RGB(247,250,247);
								}

								hBrush = ::CreateSolidBrush(bkColor);
								RECT rc;
								::SetRect(&rc,ARect.left ,ARect.top ,ARect.right,ARect.bottom );
								::FillRect(hdc,&rc,hBrush);
								::DeleteObject(hBrush);
							}

							LOGFONT	LogFont;
							HFONT hFont = NULL;
							if(pBoxStyle && pBoxStyle->mask & cssMaskFont) //m_pDataTempl->GetStyleSvr()->GetFont(ACol,ARow,LogFont) == 1)
							{
								XUtil::Css::GetFont(pBoxStyle,LogFont);

								if(bShowOrigin && bRowDelete) LogFont.lfStrikeOut = true;
								if(j==FCurrent.y && bnake ) LogFont.lfHeight *= 1.2;

								hFont = ::CreateFontIndirect(&LogFont);
								hFont = (HFONT)::SelectObject(hdc,hFont);
							}else
							{
								XUtil::Css::GetDefaultFont(LogFont);
								
								if(bShowOrigin && bRowDelete) LogFont.lfStrikeOut = true;

								//LogFont.lfHeight *= 1.3;
								if(j==FCurrent.y  && bnake ) LogFont.lfHeight *= 1.2;

								hFont = ::CreateFontIndirect(&LogFont);
								hFont = (HFONT)::SelectObject(hdc,hFont);
							}

							COLORREF TextColor = ::GetTextColor(hdc);

							if(bShowOrigin && !IsRowSelected(j) && j != FCurrent.y)
							{
								if(bRowNew) ::SetTextColor(hdc, RGB(255,0,0));
								else if(bRowDelete) ::SetTextColor(hdc, RGB(128,0,0));
								else if(bRowModify)
								{
									wstring path = (wstring)pItem->pStrColumn + (wstring)_T("/@update.origin");
									LPTSTR pOriText = (LPTSTR)GetItemStringEx(j, (LPTSTR)path.c_str());
									if(pOriText) ::SetTextColor(hdc, RGB(255,0,0));			
								}
							}

							UINT nAlign = DT_LEFT;
							if(pBoxStyle)
							{
								if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
								{
									if((!IsRowSelected(j) && j!=FCurrent.y) ||(FAskMask && hdr.mask & DMS_CSSSTYLE))
										::SetTextColor(hdc,pBoxStyle->pColor->color[0]);
								}

								if(pBoxStyle->pText && pBoxStyle->mask & cssMaskText &&
									pBoxStyle->pText->mask & cssMaskTextAlign)
								{
									if(pBoxStyle->pText->textalign == XUtil::Css::_left)
										nAlign = DT_LEFT;
									else if(pBoxStyle->pText->textalign == XUtil::Css::_right)
										nAlign = DT_RIGHT;
									else if(pBoxStyle->pText->textalign == XUtil::Css::_center)
										nAlign = DT_CENTER;
								}
							}
							
							RECT dRect = ARect;
							::InflateRect(&dRect,-1,-1);
							dRect.left += 2;

							if(FAskMask && hdr.mask & DMS_INDENT)dRect.left += hdr.indent;

							int cx=0,cy=0;
							if(FAskMask && hdr.mask & DMS_IMAGE && m_pSheetSvr->m_pSheetCtrl && m_pSheetSvr->m_pSheetCtrl->m_pxfNode)
							{
								Hxsoft::XUtil::xuVector<HIMAGELIST> & hImageLists = m_pSheetSvr->m_pSheetCtrl->m_pxfNode->GetXPage()->m_pResourceMgr->GetImageList();
								if(hdr.imagelist >=0 && hdr.imagelist < hImageLists.size())
								{
									int cx,cy;
									HIMAGELIST hImageList = hImageLists[hdr.imagelist];
									ImageList_GetIconSize(hImageList,&cx,&cy);
									RECT sRect = dRect;

									if(hdr.cximage >0) cx = hdr.cximage;
									if(hdr.cyimage >0) cy = hdr.cyimage;

									sRect.top = sRect.top + (sRect.bottom - sRect.top)/2 - cy/2;
									dRect.left = sRect.left + cx + hdr.indent1;
									::ImageList_Draw(hImageList,hdr.image,hdc, sRect.left,sRect.top ,ILD_NORMAL);
								}
							}

							if(j==FCurrent.y  && bnake )
							{
								dRect.left -= (dRect.right - dRect.left) *0.2;
								dRect.right +=(dRect.right - dRect.left) *0.2;
								dRect.top -= (dRect.bottom - dRect.top)*0.2;
								dRect.bottom += (dRect.bottom - dRect.top)*0.2;
							}

							LPTSTR pEditFormat = NULL;
							LPTSTR pDataType = NULL;
							LPTSTR pCellSource = NULL;
							bool bDrawText = true;
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
								if(pItem->pStrColumn )
								{
									if(pEditType && _tcsicmp(pEditType,_T("checkbox"))==0) //checkbox
									{
										bDrawText = false;

										LPTSTR pCheckText = NULL;
										_variant_t vCheckText;
										LPCTSTR pCheckText1 = pText;//(LPTSTR)GetGridDataSvr()->m_pContentSvr->GetItemString(pItem->pStrColumn,row);
										
										if(pElement)
										{
											pElement->getAttribute(L"editchecked",&vCheckText);
											if(vCheckText.bstrVal)
												pCheckText = _tcsdup(vCheckText.bstrVal);
										}

										HBITMAP hbp = NULL;
										TCHAR path[MAX_PATH];
										::GetModuleFileName(NULL,path,MAX_PATH);
										::PathRemoveFileSpec(path);
										TCHAR buf[MAX_PATH];

										if(pCheckText && pCheckText1 && _tcsicmp(pCheckText,pCheckText1)==0)
											::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\true.bmp"),path);
										else
											::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\false.bmp"),path);

										hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
										HDC hMemDC = CreateCompatibleDC(hdc);
										hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

										int dTop = (dRect.bottom - dRect.top)/2 - 8;
										BitBlt(hdc,dRect.left,dRect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

										hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
										::DeleteObject(hbp);
										::DeleteDC(hMemDC);

										dRect.left += 16;
										if(pCheckText) delete pCheckText;
										//if(pCheckText1) delete pCheckText1;
									}
									else if(pEditType && _tcsicmp(pEditType,_T("query"))==0) //Query
									{
										pElement->getAttribute(L"dddwshowbutton",&var);

										if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0 &&
											(ACol == GetCol() && /*row*/j==GetRow()))
										{
											HBITMAP hbp = NULL;
											TCHAR path[MAX_PATH];
											::GetModuleFileName(NULL,path,MAX_PATH);
											::PathRemoveFileSpec(path);
											TCHAR buf[MAX_PATH];
											::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\query.bmp"),path);

											hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
											HDC hMemDC = CreateCompatibleDC(hdc);
											hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

											int dTop = (dRect.bottom - dRect.top)/2 - 8;
											BitBlt(hdc,dRect.right-16,dRect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

											hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
											::DeleteObject(hbp);
											::DeleteDC(hMemDC);

											dRect.right -= 16;
										}
									}
									else if(pEditType && _tcsicmp(pEditType,_T("ddlb")) == 0) //combobox
									{
										LPTSTR pTextDisplay = (LPTSTR)this->GetItemDisplayString(j, this->GetColumnIndex(pItem->pStrColumn));
										if(pText && !(pTextDisplay && pTextDisplay[0]))
										{
											_variant_t varxml;
											IXMLDOMElement * pElementEdit = NULL;
											IXMLDOMNode * pNodeEdit = NULL;
											pElement->getAttribute(L"ddlbxml",&varxml);
											if(varxml.bstrVal)
											{
												xbXml xEditxml;
												xEditxml.LoadXml(_tcsdup(varxml.bstrVal));
												LPTSTR pCondStr = new TCHAR[255];
												::_stprintf_s(pCondStr,255,_T("//row[数据列='%s']/显示列"),pText);
												xEditxml.m_pXmlDoc->selectSingleNode(pCondStr,&pNodeEdit);
												if(pNodeEdit)
												{
													pNodeEdit->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEdit);
													BSTR pEditDisplayStr;
													pElementEdit->get_text(&pEditDisplayStr);
													if(pEditDisplayStr) pText = _tcsdup(pEditDisplayStr);
													pNodeEdit->Release();
												}
											}
											if(pElementEdit) pElementEdit->Release();
										}
									}
								}
								if(pElement) pElement->Release();
							}

							if(bDrawText && FAskMask && hdr.mask & DMS_TEXT && hdr.pStr)
							{
								DrawText(hdc,hdr.pStr,(int)::_tcslen(hdr.pStr),&dRect,nAlign|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
							}else if(bDrawText)
							{
								if(pCellSource)
								{
									if(pCellSource[0]=='=')
									{
										CXGridComplier x;
										if(x.SetSource(pCellSource + 1, this, 0, j, ACol))
											pText = x.GetString();
									}
									if(pCellSource) delete pCellSource;
								}
								if(pText)
								{
									LPTSTR pItemStr = (LPTSTR)_tcsdup(pText);
									bool bDelete = true;//bDestroyText;
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

									::DrawText(hdc,pItemStr,(int)::_tcslen(pItemStr),&dRect,nAlign|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
									//::DrawText(hdc,pText,(int)::_tcslen(pText),&dRect,nAlign|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
									if(bShowOrigin && bRowModify && !bRowNew && !bRowDelete && pItem && pItem->pStrColumn)
									{
										SIZE sz;
										GetTextExtentPoint32(hdc,pItemStr,(int)::_tcslen(pItemStr),&sz);
										if(sz.cx < dRect.right - dRect.left)						
										{
											wstring path = (wstring)pItem->pStrColumn + (wstring)_T("/@update.origindisplaystring");
											LPTSTR pOriText = (LPTSTR)GetItemStringEx(j, (wchar_t*)path.c_str());
											if(!pOriText)
											{
												path = (wstring)pItem->pStrColumn + (wstring)_T("/@update.origin");
												pOriText = (LPTSTR)GetItemStringEx(j, (wchar_t*)path.c_str());
											}
											if(pOriText)
											{
												RECT rc = dRect;
												rc.left = dRect.left + sz.cx + 4;
												::SetTextColor(hdc,RGB(128,0,0));
												LOGFONT LogFont1 = LogFont;
												LogFont1.lfStrikeOut = true;
												HFONT hFont1 = ::CreateFontIndirect(&LogFont1);
												hFont1 = (HFONT)::SelectObject(hdc,hFont1);
												::DrawTextW(hdc,pOriText,(int)::_tcslen(pOriText),&rc,nAlign|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
												hFont1 = (HFONT)::SelectObject(hdc,hFont1);
												::DeleteObject(hFont1);
											}
										}
									}
									if(bDelete) delete pItemStr;
								}
							}
							::SetTextColor(hdc,TextColor);
							hFont = (HFONT)::SelectObject(hdc, hFont);
							::DeleteObject(hFont);
						}//bDraw
						if(pBoxStyle && FAskMask && hdr.mask & DMS_CSSSTYLE)delete pBoxStyle;
						ACol++;
						ARect.left = ARect.right + 1;
					}else
						break;
					LineX = ARect.left ;
				}
				LineY +=  m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow) + FGridLineHeight;
			}
			if((IsRowSelected(j) || j==FCurrent.y))
				::SetTextColor(hdc,color);
			LineY = RowLineY + pAxisSvrEx->GetRowHeight(row) + FGridLineHeight - 2;
			if(LineY > rect.bottom) break;
		}
		::SetBkMode(hdc,hbr);

		if(pStyle)	delete pStyle;

		if(nEDrawCol > 0) rect.right=LineX;
	}

	void XGridReport::OnFDraw(HDC hdc,RECT &rect)
	{
		if(m_pSheetSvr->m_pColHeaderSvr->m_nFECol < 1) return;

		OnBaseDraw(hdc, rect, m_pSheetSvr->m_pColHeaderSvr->m_nFSCol, m_pSheetSvr->m_pColHeaderSvr->m_nFECol);
		
		rect.right = rect.right - m_pDataTempl->m_pAxisSvr->GetColWidth(0);
	}

	void XGridReport::OnDraw(HDC hdc,RECT &rect)
	{
		if(!m_pDataTempl) return ;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return;

		CAxisSvr * pAxisSvrEx = GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return;

		RECT fr;
		::SetRect(&fr,rect.left,rect.top,rect.right,rect.bottom);
		OnFDraw(hdc, fr);
		if(fr.right != rect.right) rect.left = fr.right;
/*
		int s = this->GetColCount();
		int j = 0;
		int i = 1;
		for(i=1; i<=s; i++)
		{
			int width = pAxisSvr->GetColWidth(i);
			if(width <= 1) continue;
			j++;
			if(j == FTopLeft.x) break;
		}
		OnBaseDraw(hdc, rect, i);*/
		OnBaseDraw(hdc, rect, FTopLeft.x);
		if(fr.right != rect.right) rect.left = fr.left;

		return;
#if 0
		if(!m_pDataTempl) return ;

		RECT dRect = rect;
		bool bnake = false;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return;

		CAxisSvr * pAxisSvrEx = GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return;

		int startRow,endRow;
		startRow = pReportBand->Detail.startRow;
		endRow = pReportBand->Detail.endRow - 1;

		/*
		LOGFONT	logFont;
		logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("宋体")) + 1, _T("宋体"));
		logFont.lfItalic = 0;
		logFont.lfOrientation = 0;
		logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logFont.lfPitchAndFamily = DEFAULT_PITCH;
		logFont.lfQuality = CLEARTYPE_QUALITY;
		logFont.lfStrikeOut = 0;
		logFont.lfUnderline = 0;
		logFont.lfWeight = FW_NORMAL;
		logFont.lfWidth = 0;

		HFONT	hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);

		logFont.lfWeight = FW_BOLD;
		HFONT	hFontB =::CreateFontIndirect(&logFont);*/
		int hbr;
		hbr = ::SetBkMode(hdc,TRANSPARENT);

		int width;
		width = dRect.right- dRect.left;

		int StartCol = FTopLeft.x;
		int StartRow = FTopLeft.y;

		int startCell = FTopLeft.x;
		int LineX = 0;
		int ACol ;

		int EndCol = 0;
		bool bDraw = true;
		for(int ARow = startRow;ARow <=endRow; ARow++)
		{
			ACol = m_pDataTempl->m_pContentSvr->GetValidCol(ARow);
			TGridRect gRect;
			m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect);
			if(gRect.right > EndCol) EndCol = gRect.right;
		}

		LPTSTR pCssStr = _tcsdup(_T("background-color:#ebead8 none #ffffff none"));
		XUtil::Css::TBoxStyle * pStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
		delete pCssStr;

		/*if(pStyle)
		{
			RECT rc = dRect;
			rc.left =  dRect.left + pAxisSvr->GetColWidth(0) + 1;
			rc.bottom = dRect.top + this->GetDetailHeight() - 1;
			XUtil::Css::DrawCssFrame(hdc,&rc,pStyle);
		}*/
		
		int LineY = dRect.top  +  pAxisSvrEx->GetRowHeight(0) + 1;
		int RowLineY;
		bool bDestroyText = false;
		int RowCounts = GetRowCount();
		for(int row = StartRow;row<=RowCounts;row++)
		{
			COLORREF color;
			if(IsRowSelected(row))
				color = ::SetTextColor(hdc,RGB(255,255,255));
			else
			{
				if(row==FCurrent.y)
					color = ::SetTextColor(hdc,RGB(0,0,255));
			}
			RowLineY = LineY;
			if(IsRowSelected(row))
			{
				RECT	ARect;
				::SetRect(&ARect,0,0,0,0);
				ARect.top = LineY;
				ARect.left = dRect.left + (isFDraw?0:pAxisSvr->GetColWidth(0)) + 1;
				ARect.bottom  = ARect.top;
				for(int ARow = startRow;ARow <=endRow; ARow++)
				{
					ARect.bottom +=  pAxisSvr->GetRowHeight(ARow);
					if(ARow != startRow)ARect.bottom += FGridLineHeight;
				}
				ARect.right = rect.right;
				HBRUSH hBrush = ::CreateSolidBrush(m_SelectBkColor);
				::InflateRect(&ARect,-1,-1);
				::FillRect(hdc,&ARect,hBrush);
				DeleteObject(hBrush);
			}
			for(int ARow = startRow;ARow <=endRow; ARow++)
			{
				RECT	ARect;
				::SetRect(&ARect,0,0,0,0);
				ARect.top = LineY-1;
				ARect.left = dRect.left + (isFDraw?0:pAxisSvr->GetColWidth(0)) + 1;
				ARect.bottom  = ARect.top +  pAxisSvr->GetRowHeight(ARow);
				LineX =  ARect.left;
				ACol = startCell;
				TGridRect Selection = GetSelection();
				while(LineX <= dRect.right )
				{
					ARect.bottom  = ARect.top + pAxisSvr->GetRowHeight(ARow);
					
					int i=0;
					bDraw = true;
					if(ACol <= EndCol)
					{
						DWASKSTYLENMHDR hdr;
						const XUtil::Css::TBoxStyle* pBoxStyleEx = m_pDataTempl->GetStyleSvr()->GetCellCssStyle(ACol,ARow);
						XUtil::Css::TBoxStyle* pBoxStyle = (XUtil::Css::TBoxStyle*)pBoxStyleEx;

						if(FAskMask)
						{
							memset(&hdr,0,sizeof(DWASKSTYLENMHDR));
							hdr.hwndFrom = GetHWND();
							hdr.idFrom = 0;
							hdr.code = DWV_ASKSTYLE;
							hdr.row = row;
							hdr.col = ACol;
							hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(ACol);
							hdr.rt = ARect;
							::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
							if(hdr.mask & DMS_CSSSTYLE)
							{
								XUtil::Css::TBoxStyle* pBoxStyle1 =  m_pDataTempl->GetStyleSvr()->m_pCssStyle->ParseCssStyle(hdr.pCssStyle);
								*pBoxStyle1 = (*pBoxStyle1) + (*pBoxStyle);
								pBoxStyle = pBoxStyle1;
							}
						}

	
						LPCTSTR pText = NULL;
						TGridRect gRect;
						bDraw = true;
						PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow);
						if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
						{
							if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
							{
								if(pItem && pItem->pStrColumn)
								{
									pText = GetGridDataSvr()->m_pContentSvr->GetItemString(pItem->pStrColumn,row);
								}
								else if(pItem && pItem->pStrContent)
									pText = pItem->pStrContent;
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
								ARect.bottom = ARect.top - 1;
								while(nRow <= gRect.bottom)
								{
									ARect.bottom += pAxisSvr->GetRowHeight(nRow) + 1;
									nRow++;
								}
							}
							ACol = gRect.right;
						}else
						{
							ARect.right = ARect.left + pAxisSvr->GetColWidth(ACol); 
							if(pItem && pItem->pStrColumn)
							{
								//pText = GetGridDataSvr()->m_pContentSvr->GetItemDisplayString(pItem->pStrColumn,row);
								//if(!(pText && pText[0]))pText = GetGridDataSvr()->m_pContentSvr->GetItemString(pItem->pStrColumn,row);
								pText = this->GetItemDisplayString(row, this->GetColumnIndex(pItem->pStrColumn));
								if(!(pText && pText[0]))pText = this->GetItemString(row, this->GetColumnIndex(pItem->pStrColumn));
								bDestroyText = true;
							}
							else if(pItem && pItem->pStrContent)
								pText = pItem->pStrContent;
						}
						if(bDraw)
						{
							if(pBoxStyle && pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
							{
								HBRUSH hBrush;
								hBrush = ::CreateSolidBrush(pBoxStyle->pBackGround->color.color[0]);
								::FillRect(hdc,&ARect,hBrush);
								::DeleteObject(hBrush);
							}

							if(!IsRowSelected(row) && !(pBoxStyle && pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround))
							{
								HBRUSH hBrush;
								if(row % 2 == 0)
									hBrush = ::CreateSolidBrush(RGB(226,229,222));
								else
									hBrush = ::CreateSolidBrush(RGB(242,248,242));
								RECT rc;
								::SetRect(&rc,ARect.left ,ARect.top ,ARect.right,ARect.bottom );
								::FillRect(hdc,&rc,hBrush);
								::DeleteObject(hBrush);
							}

							LOGFONT	LogFont;
							HFONT hFont = NULL;
							if(pBoxStyle && pBoxStyle->mask & cssMaskFont) //m_pDataTempl->GetStyleSvr()->GetFont(ACol,ARow,LogFont) == 1)
							{
								XUtil::Css::GetFont(pBoxStyle,LogFont);

								if(row==FCurrent.y && bnake ) LogFont.lfHeight *= 1.2;

								hFont = ::CreateFontIndirect(&LogFont);
								hFont = (HFONT)::SelectObject(hdc,hFont);
								
							}else
							{
								XUtil::Css::GetDefaultFont(LogFont);
								//LogFont.lfHeight *= 1.3;
								if(row==FCurrent.y  && bnake ) LogFont.lfHeight *= 1.2;

								hFont = ::CreateFontIndirect(&LogFont);
								hFont = (HFONT)::SelectObject(hdc,hFont);
							}

							COLORREF	TextColor = ::GetTextColor(hdc);
							UINT nAlign = DT_LEFT;
							if(pBoxStyle)
							{
								if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
								{
									if(!IsRowSelected(row) && row!=FCurrent.y )
										::SetTextColor(hdc,pBoxStyle->pColor->color[0]);
								}

								if(pBoxStyle->pText && pBoxStyle->mask & cssMaskText &&
									pBoxStyle->pText->mask & cssMaskTextAlign)
								{
									if(pBoxStyle->pText->textalign == XUtil::Css::_left)
										nAlign = DT_LEFT;
									else if(pBoxStyle->pText->textalign == XUtil::Css::_right)
										nAlign = DT_RIGHT;
									else if(pBoxStyle->pText->textalign == XUtil::Css::_center)
										nAlign = DT_CENTER;
								}
							}
							
							RECT dRect = ARect;
							::InflateRect(&dRect,-1,-1);
							dRect.left += 2;

							if(FAskMask && hdr.mask & DMS_INDENT)dRect.left += hdr.indent;

							int cx=0,cy=0;
							if(FAskMask && hdr.mask & DMS_IMAGE && m_pSheetSvr->m_pSheetCtrl && m_pSheetSvr->m_pSheetCtrl->m_pxfNode)
							{
								Hxsoft::XUtil::xuVector<HIMAGELIST> & hImageLists = m_pSheetSvr->m_pSheetCtrl->m_pxfNode->GetXPage()->m_pResourceMgr->GetImageList();
								if(hdr.imagelist >=0 && hdr.imagelist < hImageLists.size())
								{
									int cx,cy;
									HIMAGELIST hImageList = hImageLists[hdr.imagelist];
									ImageList_GetIconSize(hImageList,&cx,&cy);
									RECT sRect = dRect;

									if(hdr.cximage >0) cx = hdr.cximage;
									if(hdr.cyimage >0) cy = hdr.cyimage;

									sRect.top = sRect.top + (sRect.bottom - sRect.top)/2 - cy/2;
									dRect.left = sRect.left + cx + hdr.indent1;
									::ImageList_Draw(hImageList,hdr.image,hdc, sRect.left,sRect.top ,ILD_NORMAL);
								}
							}

							if(row==FCurrent.y  && bnake )
							{
								dRect.left -= (dRect.right - dRect.left) *0.2;
								dRect.right +=(dRect.right - dRect.left) *0.2;
								dRect.top -= (dRect.bottom - dRect.top)*0.2;
								dRect.bottom += (dRect.bottom - dRect.top)*0.2;
							}

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
									pNode->Release();
									pElement1->Release();
								}
								if(pItem->pStrColumn )
								{
									if(pEditType && _tcsicmp(pEditType,_T("checkbox"))==0) //checkbox
									{
										LPTSTR pCheckText = NULL;
										_variant_t vCheckText;
										LPTSTR pCheckText1 = (LPTSTR)GetGridDataSvr()->m_pContentSvr->GetItemString(pItem->pStrColumn,row);
										
										if(pElement)
										{
											pElement->getAttribute(L"editchecked",&vCheckText);
											if(vCheckText.bstrVal)
												pCheckText = _tcsdup(vCheckText.bstrVal);
										}

										HBITMAP hbp = NULL;
										TCHAR path[MAX_PATH];
										::GetModuleFileName(NULL,path,MAX_PATH);
										::PathRemoveFileSpec(path);
										TCHAR buf[MAX_PATH];

										if(pCheckText && pCheckText1 && _tcsicmp(pCheckText,pCheckText1)==0)
											::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\true.bmp"),path);
										else
											::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\false.bmp"),path);

										hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
										HDC hMemDC = CreateCompatibleDC(hdc);
										hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

										int dTop = (dRect.bottom - dRect.top)/2 - 8;
										BitBlt(hdc,dRect.left,dRect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

										hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
										::DeleteObject(hbp);
										::DeleteDC(hMemDC);

										dRect.left += 16;
										if(pCheckText) delete pCheckText;
										if(pCheckText1) delete pCheckText1;
									}
									else if(pEditType && _tcsicmp(pEditType,_T("query"))==0) //Query
									{
										pElement->getAttribute(L"dddwshowbutton",&var);

										if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0 &&
											(ACol == GetCol() && row==GetRow()))
										{
											HBITMAP hbp = NULL;
											TCHAR path[MAX_PATH];
											::GetModuleFileName(NULL,path,MAX_PATH);
											::PathRemoveFileSpec(path);
											TCHAR buf[MAX_PATH];
											::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\query.bmp"),path);

											hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
											HDC hMemDC = CreateCompatibleDC(hdc);
											hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

											int dTop = (dRect.bottom - dRect.top)/2 - 8;
											BitBlt(hdc,dRect.right-16,dRect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

											hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
											::DeleteObject(hbp);
											::DeleteDC(hMemDC);

											dRect.right -= 16;
										}
									}
								}
								pElement->Release();
							}

							if(FAskMask && hdr.mask & DMS_TEXT && hdr.pStr)
							{
								DrawText(hdc,hdr.pStr,(int)::_tcslen(hdr.pStr),&dRect,nAlign|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
							}else
								if(pText)
								{
									LPTSTR pItemStr = (LPTSTR)_tcsdup(pText);
									bool bDelete = true;
									if(pDataType && _tcscmp(pDataType,L"xs:decimal")==0)
									{
										if(pEditFormat && _tcscmp(pEditFormat,L"")!=0)
										{
											if(pItemStr[0]=='.')
											{
												int nlen = _wcslen(pItemStr,'.');
												LPTSTR pTemp = new TCHAR[nlen + 2];
												pTemp[0]='0';
												wcsncpy_s(pTemp + 1,nlen + 1,pItemStr,nlen);
												if(bDelete) delete pItemStr;
												bDelete = true;
												pItemStr = pTemp;

											}
											if(_tcschr(pItemStr,'.'))
											{
												int len0 = _tcslen(pItemStr) - 1;
												while(len0>-1 && (pItemStr[len0] == '0'))pItemStr[len0]='\0', len0--;		
												if(pItemStr[len0] == '.') pItemStr[len0+1] = '0', pItemStr[len0+2]='\0';
											}
											LPTSTR pStr = _tcschr(pEditFormat,'.');
											if(!pStr) pStr = L"\0";
											if(pStr)
											{
												pStr++;
												int len1 = 0;
												int len2 = 0;
												while(*pStr=='0')pStr++,len1++;
												while(*pStr=='#')pStr++,len2++;

												if(len1==0 && len2==0)
												{
													if(_tcschr(pItemStr,'.'))
													{
														if(bDelete) 
															*(_tcschr(pItemStr,'.'))=0;
														else
														{
															int nlen = _tcschr(pItemStr,'.') - pItemStr;
															LPTSTR pTemp = new TCHAR[nlen + 1];
															wcsncpy_s(pTemp,nlen + 1,pItemStr,nlen);
															pTemp[nlen] = 0;
															pItemStr = pTemp;
															bDelete = true;
														}
													}
												}else
												{
													if(len1 > 0)
													{
														int len = 0;
														LPTSTR pStr = _tcschr(pItemStr,'.');
														if(pStr) len += _tcslen(pStr+1);
														if(len < len1)
														{
															int tlen = _tcslen(pItemStr) + len1 - len ;
															if(!pStr) tlen += 1;
															LPTSTR pTemp = new TCHAR[tlen + 1];
															wcscpy_s(pTemp,tlen +1,pItemStr);
															if(!pStr) pTemp[tlen - len1 - 1]='.';
															for(int i=0;i<len1 - len;i++)pTemp[tlen - i - 1]='0';
															pTemp[tlen]=0;
															if(bDelete) delete pItemStr;
															pItemStr = pTemp;
															bDelete = true;
														}
													}
													LPTSTR pStr = _tcschr(pItemStr,'.');
													if( _tcslen(pStr + 1) > len1 + len2)
													{
														if(bDelete) 
															*( pStr + 1 + len1 + len2 )=0;
														else
														{
															int tlen = _tcslen(pItemStr);
															LPTSTR pTemp = new TCHAR[tlen + 1];
															wcscpy_s(pTemp,tlen + 1,pItemStr);
															int len = _tcslen(pStr + 1); 
															pTemp[tlen  - (len - len1 - len2)] = 0;
															if(bDelete) delete pItemStr;
															pItemStr = pTemp;
															bDelete = true;
														}
													}
												}
											}
										}
									}
									if(pEditFormat) delete pEditFormat;
									if(pDataType) delete pDataType;
									::DrawText(hdc,pItemStr,(int)::_tcslen(pItemStr),&dRect,nAlign|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
									//::DrawText(hdc,pText,(int)::_tcslen(pText),&dRect,nAlign|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
									if(bDelete) delete pItemStr;
								}
							::SetTextColor(hdc,TextColor);

							hFont = (HFONT)::SelectObject(hdc, hFont);
							::DeleteObject(hFont);
						}//bDraw
						if(pBoxStyle && FAskMask && hdr.mask & DMS_CSSSTYLE)delete pBoxStyle;
						ACol++;
						ARect.left = ARect.right + 1;
					}else
						break;
					LineX = ARect.left ;
					//画列分割线start
					HPEN hRPen = CreatePen(PS_SOLID,1,RGB(159,171,187));
					hRPen = (HPEN)SelectObject(hdc,hRPen);
					::MoveToEx(hdc,ARect.right,ARect.top, NULL);
					::LineTo(hdc,ARect.right,ARect.bottom);
					hRPen = (HPEN)SelectObject(hdc,hRPen);
					::DeleteObject(hRPen);
					//画列分割线end
				}
				LineY +=  m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow) + FGridLineHeight;
			}
			if(IsRowSelected(row) || row==FCurrent.y)
				::SetTextColor(hdc,color);
			LineY = RowLineY + pAxisSvrEx->GetRowHeight(row) + FGridLineHeight - 2;
			//画行分割线start
			HPEN hRPen = CreatePen(PS_SOLID,1,RGB(159,171,187));
			hRPen = (HPEN)SelectObject(hdc,hRPen);
			::MoveToEx(hdc,dRect.left + (isFDraw?0:pAxisSvr->GetColWidth(0))+1,LineY - pAxisSvrEx->GetRowHeight(row), NULL);
			::LineTo(hdc,LineX,LineY - pAxisSvrEx->GetRowHeight(row));
			hRPen = (HPEN)SelectObject(hdc,hRPen);
			::DeleteObject(hRPen);
			//画行分割线end
			if(LineY > rect.bottom) break;
		}
		if(LineY < rect.bottom)
		{
			//画行分割线start
			HPEN hRPen = CreatePen(PS_SOLID,1,RGB(159,171,187));
			hRPen = (HPEN)SelectObject(hdc,hRPen);
			::MoveToEx(hdc,dRect.left + (isFDraw?0:pAxisSvr->GetColWidth(0))+1,LineY - 2, NULL);
			::LineTo(hdc,LineX,LineY - 2);
			hRPen = (HPEN)SelectObject(hdc,hRPen);
			::DeleteObject(hRPen);
			//画行分割线end		
		}
		//hFont = (HFONT)::SelectObject(hdc, hFont);
		//::DeleteObject(hFont);
		//::DeleteObject(hFontB);
		::SetBkMode(hdc,hbr);

		if(pStyle)	delete pStyle;
#endif
	}

	XGridDataSvrEx * XGridReport::GetGridDataSvr()
	{
		return dynamic_cast<XGridDataSvrEx*>(this->m_pSheetSvr->m_pDataSvr);
	}

	int XGridReport::GetRowCount()
	{
		//if(m_pSheetSvr && m_pSheetSvr->m_pSheetCtrl && m_pSheetSvr->m_pSheetCtrl->m_pBindData &&
		//	m_pSheetSvr->m_pSheetCtrl->m_pBindData->rowcount)
		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			return m_pSheetSvr->m_pSheetCtrl->bindNotifyGetRowCount(GetGridDataSvr()->m_pContentSvr->GetXPath());
		}
		return GetGridDataSvr()->m_pContentSvr->GetRowCount();
	}
	void XGridReport::Initialize()
	{
		FCurrent.x = 1;
		FCurrent.y = 1;
		FTopLeft.x = 1;
		FTopLeft.y = 1;

		HandleAllocated = true;
		FEditorMode = false;

		FCanEditModify = true;
		FInplaceCol = -1;
		FInplaceRow = -1;

		m_bWheelVert = true;

		FGridLineWidth = 1;
		FOptions = /*goRowSelect|*/goEditing|goThumbTracking|goAllowEdit|goFixedVertLine|goFixedHorzLine|goVertLine|goHorzLine|goRangeSelect|goColSizing|goRowSizing|goDrawFixed|goDrawBound/*|goAlwaysShowEditor*/;
		FOptions = FOptions|goRowMoving|goColMoving;
		FScrollBars = ssBoth;
		FBorderStyle = bsSingle;
		FGridState = gsNormal;

		FSortType = 1;
		m_pSortColumns = new vector<wstring>;
		m_pSortOrders = new vector<bool>;
		m_pSortColumnTypes = new vector<wstring>;
		m_EditUpperMode = false;
		//FAskMask = false;
	}

	bool XGridReport::SetEventListons()
	{
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XGridReport::EvtSetFocus);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XGridReport::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XGridReport::EvtLButtonUp);
		this->AttachListon(WM_RBUTTONDOWN,(FNEvent)&XGridReport::EvtRButtonDown);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XGridReport::EvtMouseMove);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XGridReport::EvtKeyDown);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XGridReport::EvtSetCursor);

		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XGridReport::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&XGridReport::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&XGridReport::EvtHScroll);

		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&XGridReport::EvtDoubleClicked);
		this->AttachListon(WM_BINDERNOTIFY,(FNEvent)&XGridReport::EvtBinderNotify);
		return true;
	}
	bool XGridReport::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}
	bool XGridReport::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND XGridReport::GetHWND()
	{
		return m_pSheetSvr->GetHWND();
	}
	bool XGridReport::EvtMouseWheel(TEvent * pEvent, LPARAM lParam)
	{
		if((::GetKeyState(VK_CONTROL)<0 && m_bWheelVert) || (!m_bWheelVert && !(::GetKeyState(VK_CONTROL)<0)) )
		{
			if(pEvent->wheel.zDelta < 0)
				if(this->FTopLeft.x <this->GetColCount()) FTopLeft.x++;
			if(pEvent->wheel.zDelta > 0)
				if(this->FTopLeft.x > 1) FTopLeft.x--;
		}else
		{
			int m_PageRow = GetRowCount() - GetPageRowCount() + 1;
			if(pEvent->wheel.zDelta < 0)
			{
				if(this->FTopLeft.y <= m_PageRow) FTopLeft.y++;
			}
			if(pEvent->wheel.zDelta > 0)
			{
				if(this->FTopLeft.y > 1) FTopLeft.y--;
			}
			if (FTopLeft.y < 2)
			{
				GetVScrollBar()->SetPosition(FTopLeft.y);
			}
			else
			{
				int nMax = GetVScrollBar()->nMax;
				int nMove = nMax*FTopLeft.y/m_PageRow;
				GetVScrollBar()->SetPosition(nMove);
			}
		}
		
		RECT rc=FRect;
		rc.left = 0;
		rc.top = 0;
		if(GetHWND()) ::InvalidateRect(GetHWND(),&rc,false);

		return true;
	}	
	
	int XGridReport::GetRowHeight()
	{
		if(!m_pDataTempl) return -1;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return -1;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return -1;

		int startRow,endRow;
		startRow = pReportBand->Detail.startRow;
		endRow = pReportBand->Detail.endRow - 1;
		
		int nRowHeight = 0;
		for(int row=startRow;row<=endRow;row++)
		{
			if(row!=startRow) nRowHeight += this->FGridLineHeight;
			nRowHeight += pAxisSvr->GetRowHeight(row);
		}

		return nRowHeight + FGridLineHeight ;
	}
	int XGridReport::GetPageRowCount()
	{
		int nRowHeight = this->GetRowHeight();
		if(nRowHeight < 1)  return 1;
		
		int height = FRect.bottom - FRect.top;
		return height/nRowHeight - 1;
	}


	TGridRect XGridReport::GetSelection()
	{
		TGridRect rc;
		rc.left = GetCol();
		rc.top = GetRow();
		rc.bottom = GetRow();
		rc.right = GetCol();
		return rc;
	}

	int XGridReport::CalcRowFromPoint(int X, int Y)
	{
		if(Y < FRect.top + 1) return -1;
		if(Y > FRect.bottom - this->m_pSheetSvr->m_pXGridSummurySvr->m_nTotailHeight) return -1;
		int LineY = FRect.top + FGridLineHeight;
		int ARow = this->FTopLeft.y;
		int rRow = ARow;
		ARow = this->GetTreeRow(ARow);
		LineY += this->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) - 2;
		while(LineY < Y) 
		{
			ARow++;
			if(!this->IsTreeRowShow(ARow)) continue;
			rRow++;
			if(ARow > this->GetRowCount())break;
			LineY += this->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) -2 + this->FGridLineHeight;
		}
		return rRow;
	}

	int XGridReport::CalcColFromPoint(int X, int Y)
	{
		if(X < FRect.left + 1) return -1;
		int LineX = FRect.left + 1;

		if(m_pSheetSvr->m_pColHeaderSvr->m_nFECol > 0)
		{
			int fwidth = m_pSheetSvr->m_pColHeaderSvr->GetFColumnWidth();
			if(fwidth >= X) return -1;
			X -= fwidth;
		}

		int ACol = this->FTopLeft.x;
		LineX += this->GetGridDataSvr()->m_pAxisSvr->GetColWidth(ACol);
		while(LineX < X )
		{
			ACol++;
			if(ACol > this->GetColCount()) break;
			LineX += this->GetGridDataSvr()->m_pAxisSvr->GetColWidth(ACol)  + this->FGridLineWidth;
		}
		if(ACol > this->GetColCount()) return -1;
		return ACol;
	}

	TGridCoord XGridReport::CalcCoordFromPoint(int X, int Y)
	{
		TGridCoord Coord;

		Coord.y = CalcRowFromPoint(X,Y);
		Coord.x = CalcColFromPoint(X,Y);

		return Coord;
	}

	bool XGridReport::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		XGridDataSvrEx* pDataSvr = GetGridDataSvr();
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = nChar;
		hdr.code = DWV_KEYDOWN;
		hdr.DwTable = this->m_pSheetSvr->m_pSheetCtrl->GetInterface();
		hdr.oldrow = -1;
		hdr.row = -1;
		if(GetKeyState(VK_CONTROL)){
			hdr.row = 1;
		}
		else if(GetKeyState(VK_SHIFT)){
			hdr.row = 2;
		}
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		switch(nChar)
		{
			case VK_END:
				{
					int nRowPage = this->GetPageRowCount();
					if( nRowPage < this->GetRowCount())
					{
						FTopLeft.y = this->GetRowCount() - nRowPage + 2;
						if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
						if(this->GetHWND()) ::InvalidateRect(m_pSheetSvr->m_pSelectorSvr->GetHWND(),&m_pSheetSvr->m_pSelectorSvr->FRect,false);
					}
					break;
				}
			case VK_HOME:
				{
					FTopLeft.y = 1;
					if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
					if(this->GetHWND())::InvalidateRect(m_pSheetSvr->m_pSelectorSvr->GetHWND(),&m_pSheetSvr->m_pSelectorSvr->FRect,false);
					break;
				}

			case VK_PRIOR:
				{
					int nRowPage = this->GetPageRowCount();

					if(FTopLeft.y>nRowPage)
					{
						FTopLeft.y -= nRowPage;
					}else
						FTopLeft.y = 1;

					if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
					if(m_pSheetSvr->m_pSelectorSvr->GetHWND())::InvalidateRect(m_pSheetSvr->m_pSelectorSvr->GetHWND(),&m_pSheetSvr->m_pSelectorSvr->FRect,false);
					break;
				}
			case VK_NEXT:
				{
					int nRowPage = this->GetPageRowCount();

					if(FTopLeft.y + nRowPage < GetRowCount())
					{
						FTopLeft.y += nRowPage;
						if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
						if(m_pSheetSvr->m_pSelectorSvr->GetHWND())::InvalidateRect(m_pSheetSvr->m_pSelectorSvr->GetHWND(),&m_pSheetSvr->m_pSelectorSvr->FRect,false);
					}
					break;
				}
			case VK_UP:
				{
					if(FCurrent.y>1)
					{
						FCurrent.y--;
						if(FCurrent.y <this->FTopLeft.y)FTopLeft.y = FCurrent.y;

						if(m_SelectionMode == 1)//single
						{
							this->SelectRow(0,false);
							this->SelectRow(FCurrent.y,true);
						}
						if(m_SelectionMode < 2)
							m_nSelectionLastRow = FCurrent.y;
						if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
						this->OnRowFocusChange(FCurrent.y + 1,FCurrent.y);
					}
					
					break;
				}
			case VK_DOWN:
				{
					if(FCurrent.y < pDataSvr->m_pContentSvr->GetRowCount())
					{
						FCurrent.y++;
						int nRowPage = this->GetPageRowCount();
						if(FCurrent.y >= FTopLeft.y + nRowPage)
							FTopLeft.y = FCurrent.y - nRowPage;
						if(m_SelectionMode == 1)//single
						{
							this->SelectRow(0,false);
							this->SelectRow(FCurrent.y,true);
						}
						if(m_SelectionMode < 2)
							m_nSelectionLastRow = FCurrent.y;
						if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
						this->OnRowFocusChange(FCurrent.y - 1,FCurrent.y);
					}
					break;
				}
				break;
		}
		return true;
	}

	bool XGridReport::EvtSetFocus(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}
	
	bool XGridReport::EvtBinderNotify(TEvent * pEvent, LPARAM lParam)
	{
		if(pEvent->lParam==BN_RETRIEVED)
		{
			Initialize();
			SetRow(1);
			SelectRow(0,false);

			if(GetVScrollBar())GetVScrollBar()->SetPosition(0);
			if(GetHScrollBar())GetHScrollBar()->SetPosition(0);
			
			GetVScrollBar()->nMax = GetRowCount();
			if(GetVScrollBar()->nMax<=1 )
				GetVScrollBar()->nMax = 10;
		}
		return true;
	}

	bool XGridReport::EvtDoubleClicked(TEvent * pEvent, LPARAM lParam)
	{
		SetFocus(GetHWND());
		POINT point = pEvent->mouse.point;
		XGridDataSvrEx* pDataSvr = GetGridDataSvr();

		int nRow = CalcRowFromPoint(point.x,point.y );
		if(nRow > 0)
		{
			vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
			if(((XXmlContentSvr *)pDataSvr->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
			{
				int nIndex = XXmlContentSvr::GetTreeIndex(pTreeItems,nRow);
				if(nIndex < 0) return false;
				if((*pTreeItems)[nIndex].children)
				{
					(*pTreeItems)[nIndex].expanded = ! (*pTreeItems)[nIndex].expanded;
					if(this->GetHWND())//::InvalidateRect(this->GetHWND(),&FRect,false);
						::InvalidateRect(this->GetHWND(),NULL,false);
				}
			}else
			{
				POINT point = pEvent->mouse.point;
				XGridDataSvrEx* pDataSvr = GetGridDataSvr();


				int nRow = CalcRowFromPoint(point.x,point.y );
				int nCol = CalcColFromPoint(point.x,point.y );
				if( nRow >0 ) event_DoubleClicked(nRow,nCol);
			}
		}

		return true;
	}

	bool XGridReport::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		if(event_EditorLoseFocus() < 0) return true;

		SetFocus(GetHWND());
		POINT point = pEvent->mouse.point;
		XGridDataSvrEx* pDataSvr = GetGridDataSvr();

		int nRow = CalcRowFromPoint(point.x,point.y );
		int nCol = CalcColFromPoint(point.x,point.y );

		if(FEditorMode)	HideEditor();
		if(OnRowFocusChangeIng(FCurrent.y, nRow) < 0) return true;

		if(nCol > this->GetColumnCount())return true;
		if(this->GetColumnWidth(nCol) < 1)return true;
		if(nRow > GetTreeRowCount()) return true;


		int y = 0;
		if(nRow > 0)
		{
			//pDataSvr->GetRowAtPoint(point,y);

			vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
			if(((XXmlContentSvr *)pDataSvr->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
			{
				int nIndex = XXmlContentSvr::GetTreeIndex(pTreeItems,nRow);
				if(nIndex > 0)
				{
					if((*pTreeItems)[nIndex].children)
					{
						RECT rect;
						this->GetClientRect(&rect);
						if(y >pDataSvr->m_pAxisSvr->GetRowHeight(nRow)/2 - 5 && y <pDataSvr->m_pAxisSvr->GetRowHeight(nRow)/2 + 5)
						{
							int px = rect.left + 7 + ((*pTreeItems)[nIndex].nLevel - 2)*24 - 3;
							if(point.x >=px && point.x <=px + 20) //hited
							{
								(*pTreeItems)[nIndex].expanded = !(*pTreeItems)[nIndex].expanded;
								if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
								return true;
							}
						}
					}
				}
			}
		}

		if(nRow > 0)
		{
			event_Clicked(nRow,nCol);
		}
		
		bool pEdit = true;
		bool bShowOrigin = pDataSvr->m_pContentSvr->FShowOrigin;
		if(bShowOrigin)
		{
			LPTSTR vs = NULL;
			vs = (LPTSTR)GetItemStringEx(nRow, L"@update.delete");
			if(vs && _tcscmp(vs, L"1") == 0) pEdit = false;
		}

		int i;
		if(pEdit && nCol>0 && nCol<=this->GetColumnCount())
		{
			//2017.8.23 LPCTSTR pColName = this->GetColumnName(nCol + 1);
			LPCTSTR pColName = this->GetColumnName(nCol);
			if(pColName)
			{
				for(i=0; i < int(m_ReadOnlyColumn.size()); i++)
				{
					if(_tcscmp(pColName, m_ReadOnlyColumn[i]) == 0)
					{
						pEdit = false;
						if(FEditorMode)
							HideEditor();
						break;
					}
				}
			}
		}

		LPTSTR protect = this->GetColumnProp(nRow, nCol,_T("cellprotect"));
		if(protect)
		{
			if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
				_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
			{
				pEdit = false;
				if(FEditorMode)
					HideEditor();
			}			
		}

		if(pEdit && goEditing & FOptions && goAllowEdit & FOptions && nCol > 0 && nCol < this->GetColCount())
		{
			int startRow;
			CReportBandSvr::TReportBand *  pReportBand = &this->m_pDataTempl->m_ReportBand->m_ReportBand;
			startRow = pReportBand->Detail.startRow;
			PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(nCol,startRow);
			if(pItem && pItem->pStrXml)
			{
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement;
				IXMLDOMNode * pNode;
				xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop/edit",&pNode);
				if(pNode)
				{
					_variant_t var;
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					if(pElement)
					{
						pElement->getAttribute(L"edittype",&var);
						if(var.bstrVal)
						{
							if(_tcscmp(var.bstrVal, L"checkbox") == 0)
							{
								if(FEditorMode)
								{
									HideEditor();
								}
								_variant_t ck;
								_variant_t unck;
								pElement->getAttribute(L"editchecked",&ck);
								pElement->getAttribute(L"uneditchecked",&unck);
								LPTSTR pStr = (LPTSTR)GetItemString(nRow, nCol);
								LPTSTR pStr1 = NULL;
								if(ck.bstrVal && pStr && _tcscmp(ck.bstrVal, pStr) == 0)
								{
									if(unck.bstrVal) //pItem->pStrContent = _tcsdup(unck.bstrVal);
										pStr1 = _tcsdup(unck.bstrVal);
								}
								else
								{
									if(ck.bstrVal) //pItem->pStrContent = _tcsdup(ck.bstrVal);
										pStr1 = _tcsdup(ck.bstrVal);
								}
								if(event_ItemChanging(nRow,nCol,pStr1) < 0) return true;
								if(pStr1)
								{
									SetItemString(nRow, nCol,pStr1);
									pItem->pStrContent = pStr1;
								}
								//SetItemModified(aRow,aCol);
								//if(!GetItemDisplayString(nRow, nCol + 1)) //pItem->pStrText = L"";
								//	SetItemDisplayString(nRow, nCol + 1,L" ");
								event_ItemChanged(nRow,nCol,pItem->pStrContent);
								this->Redraw();
								return true;
							}
						}
						pElement->Release();
					}
					pNode->Release();
				}
			}
		}
		if(pEdit && goEditing & FOptions && goAllowEdit & FOptions && nCol > 0)
		{
			if(m_bClickEdit)
			{
				if(FEditorMode) HideEditor();
				int dx = FCurrent.x;
				int dy = FCurrent.y;
				FCurrent.x = nCol;
				FCurrent.y = nRow;
				ShowEditor();
				FCurrent.x = dx;
				FCurrent.y = dy;
			}
			else
			{
				if((nCol == FCurrent.x) && (nRow == FCurrent.y))
				{
					ShowEditor();
				}else if(FEditorMode)
				{
					HideEditor();
				}
			}
			/*
			bool bButton;
			bool bQuery = this->IsEditQuery(nCol,nRow,bButton);
			if(bQuery)
			{
				RECT rc;
				this->GetCellRect(nCol,nRow,rc);
				if(point.x > rc.right - 16 && point.y < rc.top + 16)
				{
					ShowQueryDlg();
				}
			}*/
		}
		if(nCol > 0 && nCol < this->GetColCount()) FCurrent.x = nCol;

		if(nRow > 0 && nRow == FCurrent.y && nRow <= GetRowCount())
		{
			if(m_SelectionMode == 1)//single
			{
				if(pEvent->mouse.nFlags & MK_CONTROL)
				{
					this->SelectRow(nRow,!IsRowSelected(nRow));
				}else
				{
					this->SelectRow(0,false);
					this->SelectRow(nRow,true);
				}
				if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
			}
		}
		else if(nRow > 0 && nRow != FCurrent.y && nRow <= GetRowCount())
		{
			OnRowFocusChange(FCurrent.y,nRow);
			FCurrent.y = nRow;
			if(m_SelectionMode == 1)//single
			{
				if(pEvent->mouse.nFlags & MK_CONTROL)
				{
					this->SelectRow(nRow,!IsRowSelected(nRow));
				}else
				{
					this->SelectRow(0,false);
					this->SelectRow(nRow,true);
				}
			}
			if(m_SelectionMode < 2)
				m_nSelectionLastRow = nRow;
			if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
		}
		if(pEdit && goEditing & FOptions && goAllowEdit & FOptions && nCol > 0)
		{
			bool bButton;
			bool bQuery = this->IsEditQuery(nCol,nRow,bButton);
			if(bQuery)
			{
				RECT rc;
				this->GetCellRect(nCol,nRow,rc);
				if(point.x > rc.right - 16 && point.y < rc.top + 16)
				{
					ShowQueryDlg();
					return true;
				}
			}
		}
		if(nRow > 0 && nRow <= GetRowCount())
		{
			if(m_SelectionMode == 2)
			{
				//this->SelectRow(0,false);
				if(pEvent->mouse.nFlags & MK_SHIFT)
				{
					if(m_nSelectionLastRow > 0)
						this->SelectRow(m_nSelectionLastRow,nRow,true);
					else
						this->SelectRow(nRow,true);
				}else if(pEvent->mouse.nFlags & MK_CONTROL)
				{
					this->SelectRow(nRow,!IsRowSelected(nRow));
				}else
					this->SelectRow(nRow,true);
				m_nSelectionLastRow = nRow;
				if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
			}else if(m_SelectionMode == 3)
			{
				//this->SelectRow(0,false);
				if(pEvent->mouse.nFlags & MK_SHIFT)
				{
					if(m_nSelectionLastRow > 0)
						this->SelectRow(m_nSelectionLastRow,nRow,true);
					else
						this->SelectRow(nRow,true);
				}else 
				{
					this->SelectRow(nRow,!IsRowSelected(nRow));
				}
				m_nSelectionLastRow = nRow;
				if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
			}else if(m_SelectionMode == 4)
			{
				//this->SelectRow(0,false);
				if(pEvent->mouse.nFlags & MK_SHIFT)
				{
					if(m_nSelectionLastRow > 0)
						this->SelectRow(m_nSelectionLastRow,nRow,true);
					else
						this->SelectRow(nRow,true);
				}else if(pEvent->mouse.nFlags & MK_CONTROL)
				{
					this->SelectRow(nRow,!IsRowSelected(nRow));
				}else{
					bool bSRow = IsRowSelected(nRow);
					if(bSRow)
					{
						int iSRow = GetNextSelectRow(0);
						while(iSRow > 0)
						{
							if(iSRow != nRow)
							{
								bSRow = false;
								break;
							}
							iSRow = GetNextSelectRow(iSRow + 1);
						}
					}
					this->SelectRow(0,false);
					this->SelectRow(nRow,!bSRow);
				}
				m_nSelectionLastRow = nRow;
				if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
			}
		}
		return true;
	}

	bool XGridReport::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XGridReport::EvtRButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		SetFocus(GetHWND());
		POINT point = pEvent->mouse.point;
		XGridDataSvrEx* pDataSvr = GetGridDataSvr();


		int nRow = CalcRowFromPoint(point.x,point.y );
		int nCol = CalcColFromPoint(point.x,point.y );

		int y = 0;

		if(nCol > 0 && nCol < this->GetColCount()) FCurrent.x = nCol;

		//if(nRow > 0)
		//{
			event_RClicked(nRow,nCol,pEvent->pt);
		//}

		return true;
	}

	bool XGridReport::EvtRButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XGridReport::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XGridReport::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	int XGridReport::GetColCount()
	{
		return GetGridDataSvr()->m_pContentSvr->GetColCount();
	}

	int XGridReport::OnRowFocusChangeIng(int nOldRow,int nNewRow)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ROWFOCUSCHANGEING;
		hdr.DwTable = this->m_pSheetSvr->m_pSheetCtrl->GetInterface();
		hdr.row = nNewRow;
		hdr.oldrow = nOldRow;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IGridReportObserver *)m_pObServers[i])->RowFoucsChanging((IGridReport *)GetInterface(),nNewRow,nOldRow);
		return hdr.idFrom;
	}

	void XGridReport::OnRowFocusChange(int nOldRow,int nNewRow)
	{
		::InvalidateRect(m_pSheetSvr->m_pSelectorSvr->GetHWND(),&m_pSheetSvr->m_pSelectorSvr->FRect,false);
		event_RowFocusChanged(nNewRow,nOldRow);
	}

	int XGridReport::GetRow()
	{
		if(GetRowCount() < 1) return -1;
		return FCurrent.y;
	}
	
	int XGridReport::SetRow(int row)
	{
		if(GetRowCount() < 1) return -1;
		if(row == 0) row = GetRowCount() ;
		int nOldRow = FCurrent.y;
		FCurrent.y = row;

		this->OnRowFocusChange(nOldRow,row);

		return row;
			
	}
	bool XGridReport::Redraw()
	{
		::InvalidateRect(this->GetHWND(),&FRect,false);
		return true;
	}

	int XGridReport::GetColumnCount()
	{
		return GetGridDataSvr()->m_pContentSvr->GetColCount();
	}

	LPCTSTR XGridReport::GetColumnName(int col)
	{
		return ((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetColumnName(col);
	}

	int XGridReport::GetColumnIndex(LPCTSTR pColumn)
	{
		return ((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetColumnIndex(pColumn);
	}

	LPCTSTR XGridReport::GetColumnFormule(LPCTSTR pStrCol)
	{
		if(!pStrCol) return NULL;
		if(!m_ColFormule)m_ColFormule = new std::map<std::wstring,std::wstring>;
		if(m_ColFormule->find(pStrCol) ==m_ColFormule->end())
		{
			m_ColFormule->insert(std::pair<std::wstring,std::wstring>(pStrCol,L""));

			if(!m_pDataTempl) return NULL;
			CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
			if(!pReportBand->Detail.bValid) return NULL;
			int ColIndex = GetColumnIndex(pStrCol);
			if(ColIndex < 1) return 0;
			int ARow = pReportBand->Detail.startRow;
			//2017.8.23	PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ColIndex - 1,ARow);
			PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ColIndex,ARow);
			wchar_t* pDataType = 0;
			wchar_t* pEditFormat = 0;
			if(pItem && pItem->pStrXml)
			{
				IXMLDOMElement * pElement1 = NULL;
				IXMLDOMNode * pNode;
				_variant_t var;

				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
					pElement1->getAttribute(L"cellsource",&var);
					LPTSTR pItemStr = NULL;
					if(var.bstrVal && ::_tcscmp(var.bstrVal, L"") != 0)
						(*m_ColFormule)[pStrCol]=var.bstrVal;
					pNode->Release();
					pElement1->Release();
				}
			}
		}
		if((*m_ColFormule)[pStrCol]==L"") return 0;
		return (*m_ColFormule)[pStrCol].c_str();
	}

	void XGridReport::SetColumnFormule(LPCTSTR pStrCol,LPCTSTR pFormule)
	{
		if(!m_ColFormule)m_ColFormule = new std::map<std::wstring,std::wstring>;
		if(m_ColFormule->find(pStrCol) == m_ColFormule->end())
		{
			m_ColFormule->insert(std::pair<std::wstring,std::wstring>(pStrCol,pFormule));
		}else
			(*m_ColFormule)[pStrCol]==pFormule;
	}

	void XGridReport::SetColumnDecimalFormat(LPCTSTR pStrCol,LPCTSTR pFormat)
	{
		if(!m_ColDecimalFormat)m_ColDecimalFormat = new std::map<std::wstring,std::wstring>;
		if(m_ColDecimalFormat->find(pStrCol) == m_ColDecimalFormat->end())
		{
			m_ColDecimalFormat->insert(std::pair<std::wstring,std::wstring>(pStrCol,pFormat));
		}else
			(*m_ColDecimalFormat)[pStrCol]==pFormat;
	}

	LPCTSTR XGridReport::GetColumnDecimalFormat(LPCTSTR pStrCol)
	{
		if(!pStrCol) return NULL;
		if(!m_ColDecimalFormat)m_ColDecimalFormat = new std::map<std::wstring,std::wstring>;
		if(m_ColDecimalFormat->find(pStrCol) == m_ColDecimalFormat->end())
		{
			m_ColDecimalFormat->insert(std::pair<std::wstring,std::wstring>(pStrCol,L""));

			if(!m_pDataTempl) return NULL;
			CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
			if(!pReportBand->Detail.bValid) return NULL;
			int ColIndex = GetColumnIndex(pStrCol);
			if(ColIndex < 1) return 0;
			int ARow = pReportBand->Detail.startRow;
			//2017.8.23	PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ColIndex - 1,ARow);
			PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ColIndex,ARow);
			wchar_t* pDataType = 0;
			wchar_t* pEditFormat = 0;
			if(pItem && pItem->pStrXml)
			{
				IXMLDOMElement * pElement1 = NULL;
				IXMLDOMNode * pNode;
				_variant_t var;

				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
					pElement1->getAttribute(L"celltype",&var);
					pNode->Release();
					if(var.bstrVal)
						pDataType = _tcsdup(var.bstrVal);
					pElement1->getAttribute(L"cellformat",&var);
					if(var.bstrVal)
						pEditFormat = _tcsdup(var.bstrVal);	
					pElement1->Release();
				}
				if(pDataType && _tcscmp(pDataType,L"xs:decimal")==0 && pEditFormat)
					(*m_ColDecimalFormat)[pStrCol]=pEditFormat;
				if(pDataType) delete pDataType;
				if(pEditFormat) delete pEditFormat;
			}
		}
		if((*m_ColDecimalFormat)[pStrCol]==L"") return 0;
		return (*m_ColDecimalFormat)[pStrCol].c_str();
	}

	bool XGridReport::hasBindData()
	{
		return this->m_pSheetSvr->m_pSheetCtrl->hasBindData();
	}

	LPCTSTR XGridReport::GetItemString(int Row,int Col)
	{
		int tmpRow = Row;
		Row = GetTreeRow(Row);

		/*
		if(!m_pDataTempl) return NULL;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return NULL;

		wchar_t* pDataType = 0;
		wchar_t* pEditFormat = 0;
		int ARow = pReportBand->Detail.startRow;
		PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(Col - 1,ARow);
		if(pItem && pItem->pStrXml)
		{
			IXMLDOMElement * pElement1 = NULL;
			IXMLDOMNode * pNode;
			_variant_t var;

			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
				pElement1->getAttribute(L"cellsource",&var);
				LPTSTR pItemStr = NULL;
				if(var.bstrVal && ::_tcscmp(var.bstrVal, L"") != 0)
				{
					pItemStr = _tcsdup(var.bstrVal);
					if(pItemStr[0]=='=')
					{
						CXGridComplier x;
						if(x.SetSource(pItemStr, this, 0, tmpRow, Col))
							pItemStr = _tcsdup(x.GetString());
					}
				}
				pNode->Release();

				pElement1->getAttribute(L"celltype",&var);
				if(var.bstrVal)
					pDataType = _tcsdup(var.bstrVal);
				pElement1->getAttribute(L"cellformat",&var);
				if(var.bstrVal)
					pEditFormat = _tcsdup(var.bstrVal);	
				pElement1->Release();
				if(pItemStr) return pItemStr;
			}
		}
		*/
		const wchar_t* pEditFormat = GetColumnDecimalFormat(GetColumnName(Col));
		const wchar_t* pFormaul = GetColumnFormule(GetColumnName(Col));
		
		wchar_t* pItemStr = (wchar_t*)pFormaul;
		if(pItemStr && pItemStr[0]=='=')
		{
			CXGridComplier x;
			if(x.SetSource(pItemStr, this, 0, tmpRow, Col))
				pItemStr =x.GetString();
			if(!m_pBuffers)m_pBuffers = new set<wstring>;
				m_pBuffers->insert(pItemStr);

		}
		LPTSTR pText = 0;
		if(pItemStr)
			pText = pItemStr;
		else if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
			pText = m_pSheetSvr->m_pSheetCtrl->bindGetData(GetGridDataSvr()->m_pContentSvr->GetXPath(), Row, (wchar_t*)GetColumnName(Col));
		else
			pText = (LPTSTR)((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetItemString(Col,Row);
		if(pEditFormat)
		{
			bool bDelete = false;
			pText = Hxsoft::XUtil::FormatDecimalString(pText, (LPTSTR)pEditFormat,bDelete);
			if(bDelete)
			{
				if(!m_pBuffers)m_pBuffers = new set<wstring>;
				m_pBuffers->insert(pText);
				wchar_t* pStr = pText;
				pText = (LPTSTR)(*m_pBuffers->find(pText)).c_str();
				delete pStr;
			}

		}
		return pText;
	}
	
	int XGridReport::SetItemString(int Row,int Col,LPTSTR pStr)
	{
		Row = GetTreeRow(Row);

		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			return m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged(GetGridDataSvr()->m_pContentSvr->GetXPath(), Row, (wchar_t*)GetColumnName(Col),pStr);
		}
		return GetGridDataSvr()->m_pContentSvr->SetItemString(Col,Row,pStr);
	}

	int XGridReport::SetItemStringEx(int Row,LPTSTR ColName,LPTSTR pStr)
	{
		Row = GetTreeRow(Row);

		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			return m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged(GetGridDataSvr()->m_pContentSvr->GetXPath(), Row, ColName,pStr);
		}
		return GetGridDataSvr()->m_pContentSvr->SetItemStringEx(ColName,Row,pStr);
	}

	int XGridReport::DeleteRow(int nRow)
	{
		HideEdit();

		int nRet;

		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
			nRet = m_pSheetSvr->m_pSheetCtrl->bindNotifyDeleteRow(GetGridDataSvr()->m_pContentSvr->GetXPath(),nRow);
		else
			nRet = GetGridDataSvr()->m_pContentSvr->DeleteRow(nRow);
		if(nRet && FCurrent.y >= nRow) FCurrent.y = FCurrent.y - 1;
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,NULL,false);
		return nRet;

	}
	int XGridReport::InsertRow(int nRow)
	{
		int nRowex = -1;
		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
			nRowex = m_pSheetSvr->m_pSheetCtrl->bindNotifyInsertRow(GetGridDataSvr()->m_pContentSvr->GetXPath(),nRow);
		else
			nRowex = GetGridDataSvr()->m_pContentSvr->InsertRow(nRow);
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,NULL,false);

		return nRowex;
	}


	int  XGridReport::LinePos(TGridDrawInfo& AxisInfo ,int  Line)
	{
		return 0;
	}
	bool XGridReport::GetCellRect(TGridDrawInfo& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan)
	{

		return true;
	}

	bool XGridReport::GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan)
	{
		::SetRect(&rect,0,0,0,0);
		RECT dRect;
		::GetClientRect(GetHWND(),&dRect);

		int fwidth = m_pSheetSvr->m_pColHeaderSvr->GetFColumnWidth();
		dRect.left += fwidth;

		if(!m_pDataTempl) return false;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return false;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return false;

		CAxisSvr * pAxisSvrEx = GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return false;

		int startRow,endRow;
		startRow = pReportBand->Detail.startRow;
		endRow = pReportBand->Detail.endRow - 1;

		int width;
		width = dRect.right- dRect.left;

		int StartCol = FTopLeft.x;
		int StartRow = FTopLeft.y;

		int startCell = FTopLeft.x;
		int LineX = 0;
		int nCol;

		int EndCol = 0;
		for(int i = startRow;i <=endRow; i++)
		{
			nCol = m_pDataTempl->m_pContentSvr->GetValidCol(i);
			TGridRect gRect;
			m_pDataTempl->m_pSpanSvr->isSpaned(nCol,i,gRect);
			if(gRect.right > EndCol) EndCol = gRect.right;
		}

		if(ACol < StartCol) return false;
		if(ACol > EndCol) return false;
		if(ARow < StartRow) return false;
		if(ARow > GetRowCount()) return false;


		int LineY = dRect.top  +  pAxisSvrEx->GetRowHeight(0) + 1;
		int RowLineY;


		for(int row = StartRow;row<=GetRowCount();row++)
		{
			RowLineY = LineY;
			for(int nRow = startRow;nRow <=endRow; nRow++)
			{
				RECT	ARect;
				::SetRect(&ARect,0,0,0,0);
				ARect.top = LineY;
				ARect.left = dRect.left + pAxisSvr->GetColWidth(0) + 1;
				ARect.bottom  = ARect.top +  pAxisSvr->GetRowHeight(nRow);
				
				if(ARow == row)
				{
					LineX =  ARect.left;
					nCol = startCell;
					while(LineX <= dRect.right )
					{
						ARect.bottom  = ARect.top +  pAxisSvr->GetRowHeight(nRow);
						if(nCol <= EndCol)
						{
							TGridRect gRect;
							if(m_pDataTempl->m_pSpanSvr->isSpaned(nCol,nRow,gRect))
							{
								ARect.right = ARect.left - 1;
								while(nCol <= gRect.right)
								{
									ARect.right += pAxisSvr->GetColWidth(ACol) + 1;
									nCol++;
								}
								nCol = gRect.right;
							}else
							{
								ARect.right = ARect.left + pAxisSvr->GetColWidth(nCol); 
							}

							if(nCol==ACol)
							{
								rect = ARect;
								return true;
							}

							nCol++;
							ARect.left = ARect.right + 1;
						}else
							break;
						LineX = ARect.left ;
					}
				}
				LineY +=  m_pDataTempl->m_pAxisSvr->GetRowHeight(nRow) + FGridLineHeight;
			}
			LineY = RowLineY + pAxisSvrEx->GetRowHeight(row) + FGridLineHeight - 2;
			if(LineY > dRect.bottom) break;
		}
		return false;
	}

	bool XGridReport::BoxRect(TGridDrawInfo& AxisInfo,TGridRect gridRect,RECT &screenRect)
	{
		return true;
	}

	bool XGridReport::CanEditAcceptKey(char Key)
	{
		return true;
	}
	bool XGridReport::CanGridAcceptKey(WORD Key, TShiftState Shift)
	{
		return true;
	}
	bool XGridReport::CanEditModify()
	{
		return FCanEditModify;
	}

	bool XGridReport::CanEditShow()
	{
		return  (!(goRowSelect &FOptions) && goAllowEdit&FOptions) &&
			FEditorMode &&  HandleAllocated &&
			((goAlwaysShowEditor & FOptions) || IsActiveControl());
	}

	bool XGridReport::IsActiveControl()
	{
		return true;
	}

	void XGridReport::InvalidateEditor()
	{
		FInplaceCol = -1;
		FInplaceRow = -1;
		UpdateEdit();
	}

	void XGridReport::UpdateEditor()
	{
		m_pEditSvr->SelectCell(GetCol(), GetRow());
	}

	void XGridReport::UpdateEdit()
	{ 
		if(CanEditShow())
		{
			int nRow = GetRow();
			if(nRow != -1){
				m_pEditSvr->SelectCell(GetCol(), nRow);
				FInplaceCol = GetCol();
				FInplaceRow = GetRow();
			}
		}
	}
	void XGridReport::ShowEditor()
	{
		FEditorMode = TRUE;
		UpdateEdit();
	}

	void XGridReport::HideEdit()
	{
		this->m_pEditSvr->HiddenEditor();
		FInplaceCol = -1;
		FInplaceRow = -1;
	}

	void XGridReport::HideEditor()
	{
		FEditorMode = FALSE;
		HideEdit();
	}

	bool XGridReport::SelectCell(int ACol, int ARow)
	{
		return true;
	}

	int XGridReport::DwUpdate(IXMLDOMDocument2 * pXmlDoc)
	{
		int nColCount = this->GetColumnCount();
		vector<LPTSTR> xslPaths;
		LPTSTR pXslPath;

		//2017.8.13 for(int nCol = 0;nCol <nColCount;nCol++)
		for(int nCol = 1;nCol <=nColCount;nCol++)
		{
			//2017.8.23 xslPaths.push_back((LPTSTR)GetColumnName(nCol + 1));
			xslPaths.push_back((LPTSTR)GetColumnName(nCol));
		}
		pXslPath = ((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetXPath();

		long gcnt = 1;
		IXMLDOMNodeListPtr gNodes;
		IXMLDOMNodePtr gNode;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr Node;
		IXMLDOMNodePtr  currentNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMNodeListPtr pRowNodes;
		IXMLDOMElement* pNewElement;
		IXMLDOMNodeList* pTempNodes; 
		IXMLDOMNodePtr NewNode;
		IXMLDOMElementPtr tNode;
		IXMLDOMElementPtr NewElement;
		IXMLDOMElementPtr NewElement1;
		vector<IXMLDOMElement * > vcElements;
		wstring StrName;

		gcnt = 1;
		for(int group = 1;group <=gcnt;group++)
		{
			gNode = pXmlDoc;

			//if no exists pNode then create parent Node
			wstring StrPath = pXslPath;
			int nPosLast = -1;
			nPosLast = (int)StrPath.find_last_of(_T("/"));

			for(vector<IXMLDOMElement * >::const_iterator tr=vcElements.begin();tr!=vcElements.end();tr++)(*tr)->Release();
			vcElements.clear();

			wstring StrName;
			if(nPosLast >=0)
			{
				StrName = StrPath.substr(nPosLast + 1);
				StrPath = StrPath.substr(0,nPosLast);
				while(StrPath.compare(_T(""))!=0)
				{
					pNode = NULL;
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);
					if(!pNode)break;
					nPosLast = (int)StrPath.find_last_of(_T("/"));
					if(nPosLast < 0)break;
					StrName = StrPath.substr(nPosLast + 1);
					StrPath = StrPath.substr(0,nPosLast);
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
					pXmlDoc->createElement((LPTSTR)StrName.c_str(),&pNewElement);
					vcElements.push_back(pNewElement);
				}
				if(StrPath.compare(_T(""))!=0)
					pNode = gNode;
				else
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);

				for(int i=(int)vcElements.size() - 1;i>1;i--)
				{
					vcElements[i]->appendChild(vcElements[i - 1],NULL);
				}
				if(vcElements.size() > 0)
				{
					pNode->appendChild(vcElements[vcElements.size() - 1],NULL);
					pNodeEx = vcElements[0];
				}else
					pNodeEx = pNode;
			}else
			{
				StrName = StrPath;
				if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
				if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
				StrPath = _T("");
				pNodeEx = gNode;
			}
			//end for if no exists pNode then create parent node

			gNode->selectNodes(pXslPath,&pRowNodes);
			int trow =0;
			long nRowLen;
			pRowNodes->get_length(&nRowLen);
			int row = 1;
			for(row=1;row <= this->GetRowCount();row++)
			{
				trow++;
				if(row <= nRowLen)
				{
					pRowNodes->get_item(row -1,&Node);
					for(int i=1 ;i<=(int)xslPaths.size();i++)
					{
						Node->selectSingleNode(xslPaths[i - 1],&currentNode);
						if(!currentNode)
						{
							//create not exits column node
							wstring path = xslPaths[i - 1];
							if(path[0]=='/' &&path[1]=='/')continue; 
							if(path[0]=='/')path = path.substr(1);
							int pos = (int)path.find(_T("/"));
							if(pos >0)
							{
								StrName = path.substr(0,pos);
								path = path.substr(pos +1);
							}else
							{
								StrName = path;
								path = _T("");
							}
							if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
							if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

							Node->selectNodes((LPTSTR)StrName.c_str(),&pTempNodes);
							long nTempLen = 0;
							pTempNodes->get_length(&nTempLen);
							if(nTempLen < 1)
							{
								if(StrName[0]=='@')
								{
									StrName = StrName.substr(1);
									((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
									continue;
								}else
								{
									pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement);
									Node->appendChild(NewElement,NULL);
								}
							}else
							{
								if(StrName[0]=='@')continue;
								 pTempNodes->get_item(0,&NewNode);
								 NewElement = NewNode;
							}
							while(path.compare(_T(""))!=0)
							{
								int pos = (int)path.find(_T("/"));
								if(pos >=0)
								{
									StrName =  path.substr(0,pos);
									path = path.substr(pos + 1);
								}else
								{
									StrName = path;
									path = _T("");
								}
								NewElement->selectNodes((LPTSTR)StrName.c_str(),&pTempNodes);
								pTempNodes->get_length(&nTempLen);
								if(nTempLen < 1)
								{
									if(StrName[0]=='@')
									{
										StrName = StrName.substr(1);
										((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
										continue;
									}else
									{
										pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement1);
										NewElement->appendChild(NewElement1,NULL);
										NewElement = NewElement1;
									}
								}else
								{
									if(StrName[0]=='@')continue;
									 pTempNodes->get_item(0,&NewNode);
									 NewElement = NewNode;
								}
							}
							//end create not exists column
						}
					}
				}else
				{
					//create data frame
					if(!tNode)
					{
						//create template data node
						wstring StrPath = pXslPath;
						int posLast = (int)StrPath.find_last_of(_T("/"));
						if(posLast>=0)
							StrPath = StrPath.substr(nPosLast + 1);
						else
							StrName = StrPath;
						
						if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
						if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

						pXmlDoc->createElement((LPTSTR)StrName.c_str(),&tNode);

						//end create template data node
						for(int i=1 ;i<=(int)xslPaths.size();i++)
						{
							wstring path = xslPaths[i - 1];
							if(path[0]=='/' &&path[1]=='/')continue; 
							if(path[0]=='/')path = path.substr(1);
							int pos = (int)path.find(_T("/"));
							if(pos >0)
							{
								StrName = path.substr(0,pos);
								path = path.substr(pos +1);
							}else
							{
								StrName = path;
								path = _T("");
							}
							if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
							if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
							if(StrName[0]=='@')
							{
								StrName = StrName.substr(1);
								if(Node)
									((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
								continue;
							}else
							{
								pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement);
								tNode->appendChild(NewElement,NULL);
							}
							while(path.compare(_T(""))!=0)
							{
								int pos = (int)path.find(_T("/"));
								if(pos >=0)
								{
									StrName =  path.substr(0,pos);
									path = path.substr(pos + 1);
								}else
								{
									StrName = path;
									path = _T("");
								}
								if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
								if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
								if(StrName[0]=='@')
								{
									StrName = StrName.substr(1);
									((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
									continue;
								}else
								{
									pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement1);
									NewElement->appendChild(NewElement1,NULL);
									NewElement = NewElement1;
								}
							}//while
						}//next
					}//end if tnod
					//end create data frame

					tNode->cloneNode(VARIANT_TRUE,&Node);
					pNodeEx->appendChild(Node,NULL);
				}//row
				
				//set data
				//set key value
				//long ll_value;
				_variant_t var;
				((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetRowElement(trow)->getAttribute(L"guid",&var);

				if(!var.bstrVal || var.bstrVal[0]=='\0')
				{

					GUID guid;
					CoCreateGuid(&guid);
					LPTSTR pStrGuid = new TCHAR[48];
					StringFromGUID2(guid,pStrGuid,48);
					((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pStrGuid);
				}else
				{
					((IXMLDOMElementPtr)Node)->setAttribute(L"guid",var);
				}


				LPCTSTR pStrValue;
				DOMNodeType	iNodeType;
				for(int col=1;col <= (int)xslPaths.size();col++)
				{
					pStrValue = this->GetItemString(trow,col);
					currentNode = NULL;
					Node->selectSingleNode(xslPaths[col - 1],&currentNode);
					if(currentNode)
					{
						currentNode->get_nodeType(&iNodeType);
						if(iNodeType == NODE_ELEMENT)
						{
							if(!pStrValue)
							{
								((IXMLDOMElementPtr)currentNode)->setAttribute(L"isnull",(_variant_t)_T("1"));
								currentNode->put_text(L"");
							}else
							{
								((IXMLDOMElementPtr)currentNode)->setAttribute(L"isnull",(_variant_t)_T("0"));
								((IXMLDOMElementPtr)currentNode)->removeAttribute(L"isnull");
								currentNode->put_text((BSTR)pStrValue);
							}
						}
						else //attribute
						{
							if(!pStrValue)
								currentNode->put_text(L"");
							else
								currentNode->put_text((BSTR)pStrValue);
						}//NodeType
					}//currentNode
				}//for
			}//for row

			IXMLDOMNode * pNodeItem;
			for(int k = nRowLen;k>=row;k--)
			{
				pRowNodes->get_item( k - 1,&pNodeItem);
				pNodeEx->removeChild(pNodeItem,NULL);
				pNodeItem->Release();
			}
		}
		//end update data to xml node
		return 1;
	}
	int XGridReport::SetReadOnly(BOOL bReadOnly)
	{
		if(!bReadOnly)
			FOptions |= goAllowEdit;
		else
			FOptions &= ~goAllowEdit;
		
		return 1;
			
	}

	int  XGridReport::GetXml(BSTR &bstr)
	{
		return GetGridDataSvr()->m_pContentSvr->GetContent(bstr);
	}
	int  XGridReport::GetXml(BSTR &bstr,LPTSTR pGuid)
	{
		return GetGridDataSvr()->m_pContentSvr->GetContent(bstr,pGuid);
	}
	int  XGridReport::SetXml(BSTR bstr)
	{
		return GetGridDataSvr()->m_pContentSvr->LoadXml(bstr);
	}
	
	int XGridReport::SelectRow(int nRow,bool bSelect)
	{
		if(nRow==0)
		{
			if(bSelect)
			{
				m_pRowSelects->SetValue(1,GetRowCount(),1);
				return 1;
			}else
			{
				m_pRowSelects->Reset(0);
				return 1;
			}
		}
		if(nRow > GetRowCount()) return -1;
		if(nRow < 1) return -1;

		m_pRowSelects->SetValue(nRow -1,bSelect?1:0);

		return 1;
	}
	
	int XGridReport::SelectRow(int sRow,int eRow, bool bSelect)
	{
		int nRow1 = sRow < eRow ? sRow:eRow;
		int nRow2 = sRow > eRow ? sRow:eRow;
		
		if(nRow2 < 1) return -1;
		if(nRow1 > GetRowCount()) return -1;
		if(nRow1 < 1) nRow1 = 1;
		if(nRow2 > GetRowCount()) nRow2 = GetRowCount();
		m_pRowSelects->SetValue(nRow1 -1 ,nRow2 - 1, bSelect?1:0);
		return 1;
	}

	int XGridReport::GetNextSelectRow(int nStartRow)
	{
		if(nStartRow < 1) nStartRow = 1 ;
		int Rows = GetRowCount();
		while(nStartRow <= Rows)
		{
			if(m_pRowSelects->GetValue(nStartRow - 1) ==1) return nStartRow;
			nStartRow ++;
		}
		return -1;
	}

	int XGridReport::GetPrevSelectRow(int nStartRow)
	{
		int Rows = GetRowCount();
		if(nStartRow > Rows) nStartRow = Rows ;
		while(nStartRow > 0)
		{
			if(m_pRowSelects->GetValue(nStartRow - 1) ==1) return nStartRow;
			nStartRow --;
		}
		return -1;
	}

	bool XGridReport::IsRowSelected(int nRow)
	{
		//nRow = GetTreeRow(nRow);
		if(nRow < 1) return false;
		if(nRow > GetRowCount())return false;
		return m_pRowSelects->GetValue(nRow - 1)==1?true:false;
	}
	int XGridReport::SetSelectBkColor(COLORREF color)
	{
		m_SelectBkColor = color;
		return 1;
	}
	int XGridReport::SetSelectionMode(int nMode)
	{
		this->m_SelectionMode = nMode;
		return 1;
	}

	int XGridReport::AddObserver(xbObserver * pObserver)
	{
		m_pObServers.push_back(pObserver);
		return 1;
	}
	int XGridReport::RemoveObserver(xbObserver * pObserver)
	{
		for(int i= 0;i < (int)m_pObServers.size();i++)
		{
			if(m_pObServers[i] == pObserver)
			{
				m_pObServers.erase(m_pObServers.begin()+i);
				break;
			}
		}
		return 1;
	}
	int XGridReport::ClearObserver()
	{
		m_pObServers.clear();
		return 1;
	}
	
	int XGridReport::event_DoubleClicked(int nRow,int nCol)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_DOUBLECLICKED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(nCol);

		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		return 1;
	}

	int XGridReport::event_Clicked(int nRow,int nCol)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_CLICKED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(nCol);
		hdr.data = (nRow>0 && nCol >= 0)?L"":(LPTSTR)GetItemString(nRow, nCol + 1);

		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		return 1;
	}

	int XGridReport::event_RClicked(int nRow,int nCol,POINT pt)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = (UINT_PTR)this->GetInterface();
		hdr.code = DWV_RCLICKED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(nCol);
		hdr.data = NULL;

		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		if(hdr.idFrom <= 0) return 1;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;				
		if(!pReportBand->Title.bValid) return true;
		//ARow一行标题的情况，多行标题现不适用
		int ARow = pReportBand->Title.startRow;

		TGridRect re;
		if(m_pDataTempl->m_pSpanSvr->isSpaned(nCol,ARow,re)) nCol = re.right;

		HMENU M = CreatePopupMenu();
		//AppendMenu(M,MF_STRING, 0x501, L"剪切");
		AppendMenu(M,MF_STRING, 0x502, L"复制");
		//AppendMenu(M,MF_STRING, 0x503, L"粘贴");
		AppendMenu(M,MF_STRING, 0x0, NULL);
		AppendMenu(M,MF_STRING, 0x101, L"顺序");
		AppendMenu(M,MF_STRING, 0x102, L"倒序");
		AppendMenu(M,MF_STRING, 0x0, NULL);
		AppendMenu(M,MF_STRING, 0x3, L"隐藏");
		HMENU M2 = CreatePopupMenu();
		int s = GetColumnCount();
		for(int i=1; i<s; i++)
		{
			TGridRect gRect;
			if(m_pDataTempl->m_pSpanSvr->isSpaned(i,ARow,gRect))
				if(i > gRect.left && i <=gRect.right) continue;

			LPTSTR Name = (LPTSTR)GetTitleName(i);
			if(!Name || _tcscmp(Name, L"") == 0) continue;
			int width = GetColumnWidth(i);
			if(width < 1)
				AppendMenu(M2,MF_STRING, 0x200+i, Name);
		}
		AppendMenu(M,MF_POPUP, (UINT_PTR)M2, L"显示");
		AppendMenu(M,MF_STRING, 0x0, NULL);
		AppendMenu(M,MF_STRING, 0x1, L"冻结");
		AppendMenu(M,MF_STRING, 0x2, L"取消冻结");
		AppendMenu(M,MF_STRING, 0x0, NULL);
		AppendMenu(M,MF_STRING, 0x4, L"筛选");

		xbXml xml;
		IXMLDOMNodeList * pList = NULL;
		if(hdr.data && _tcscmp(hdr.data, L"") != 0)
		{
			if(xml.LoadXml(hdr.data))
			{
				xml.GetXmlDoc()->selectNodes(L"/node()/node()", &pList);
				long len;
				pList->get_length(&len);
				if(len > 0) AppendMenu(M,MF_STRING, 0x0, NULL);
				for(int i=0; i<len; i++)
				{
					IXMLDOMNode *pNode = NULL;
					IXMLDOMElement * pElement = NULL;
					pList->get_item(i,&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();	
					}
					if(pElement)
					{
						_variant_t caption = NULL;
						pElement->getAttribute(L"value", &caption);
						if(caption.bstrVal)
						{
							LPTSTR pStr = _tcsdup(caption.bstrVal);
							if(_tcscmp(pStr, L"") == 0 || _tcscmp(pStr, L"-") == 0)
								AppendMenu(M,MF_STRING, 0x0, NULL);
							else
								AppendMenu(M,MF_STRING, 0x1000 + i, pStr);
						}
						pElement->Release();	
					}
				}
			}
		}
		RECT xr;
		int ret = TrackPopupMenu(M, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, ::GetParent(GetHWND()), &xr);
		int ACol = nCol;
		if(ret == 0x101)
		{
			if(ACol < 1 || ACol > GetColumnCount()) return true;
			//2017.8.23 SetRowSort((LPTSTR)GetColumnName(ACol+1), true);
			SetRowSort((LPTSTR)GetColumnName(ACol), true);
		}
		else if(ret == 0x102)
		{
			if(ACol < 1 || ACol > GetColumnCount()) return true;
			//2017.8.23 SetRowSort((LPTSTR)GetColumnName(ACol+1), false);
			SetRowSort((LPTSTR)GetColumnName(ACol), false);
		}
		else if(ret == 0x3)
		{
			if(ACol < 1 || ACol > GetColumnCount()) return true;
			int i;
			for(i=re.left; i<=re.right; i++)
				SetColumnState(i, false);
			::InvalidateRect(GetHWND(),NULL,false);
		}
		else if(ret > 0x200 && ret < 0x300)
		{
			ACol = ret - 0x200;
			TGridRect gRect;
			if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
			{
				int i;
				for(i=gRect.left; i<=gRect.right; i++)
					SetColumnState(i, true);				
			}
			else
				SetColumnState(ACol, true);
			::InvalidateRect(GetHWND(),NULL,false);
		}
		else if(ret == 0x1)
		{
			if(ACol < 1 || ACol > GetColumnCount()) return true;
			m_pSheetSvr->m_pColHeaderSvr->m_nFSCol = FTopLeft.x;
			m_pSheetSvr->m_pColHeaderSvr->m_nFECol = ACol;
			FTopLeft.x = ACol + 1;
			::InvalidateRect(GetHWND(),NULL,false);
		}
		else if(ret == 0x2)
		{
			m_pSheetSvr->m_pColHeaderSvr->m_nFSCol = -1;
			m_pSheetSvr->m_pColHeaderSvr->m_nFECol = -1;
			::InvalidateRect(GetHWND(),NULL,false);
		}
		else if(ret == 0x4)
		{
			if(m_pSheetSvr->m_pColHeaderSvr->m_bColQuery)
			{
				m_pSheetSvr->m_pColHeaderSvr->m_bColQuery = false;
				if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
					m_pSheetSvr->m_pSheetCtrl->bindGetData(GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, (wchar_t*)((std::wstring)L"filter:,").c_str());
				else
					GetGridDataSvr()->m_pContentSvr->ColumnFilter(NULL, NULL);
			}
			else
				m_pSheetSvr->m_pColHeaderSvr->m_bColQuery = true;
			::InvalidateRect(GetHWND(),NULL,false);
		}
		else if(ret == 0x502)
		{
			LPCTSTR pStr = this->GetItemDisplayString(nRow, nCol);
			if(!pStr) pStr = this->GetItemString(nRow, nCol);
			if(!pStr) pStr = L"";
			SetDataToClip(pStr);
		}else if(ret >= 0x1000 && pList)
		{
			ret -= 0x1000;
			IXMLDOMNode *pNode = NULL;
			IXMLDOMElement * pElement = NULL;
			pList->get_item(ret,&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pNode->Release();	
			}
			if(pElement)
			{
				_variant_t comdid;
				pElement->getAttribute(L"name", &comdid);
				if(comdid.bstrVal) 
					::SendMessage(::GetParent(GetHWND()),0x401,(WPARAM)comdid.bstrVal,NULL);
				pElement->Release();
			}
		}
		if(pList) pList->Release();
		return 1;
	}

	int XGridReport::event_ItemFocusChanged(int nRow,int nCol)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMFOCUSCHANGED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(nCol);
			// (LPTSTR)this->GetColumnName(nCol);
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IGridReportObserver *)m_pObServers[i])->ItemFocueChanged((IGridReport *)GetInterface(),nRow,nCol);
		return 1;
	}
	int XGridReport::event_RowFocusChanged(int nRow,int nOldRow)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ROWFOCUSCHANGED;
		hdr.DwTable = this->m_pSheetSvr->m_pSheetCtrl->GetInterface();
		hdr.row = nRow;
		hdr.oldrow = nOldRow;

		if(m_pSheetSvr && m_pSheetSvr->m_pSheetCtrl)
		{
			int ARow11 = GetTreeRow(nRow);
			LPTSTR path = GetGridDataSvr()->m_pContentSvr->GetXPath();
			m_pSheetSvr->m_pSheetCtrl->bindNotifyRowChanged((wchar_t*)path,ARow11);
		}

		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IGridReportObserver *)m_pObServers[i])->RowFoucsChanged((IGridReport *)GetInterface(),nRow,nOldRow);
		return 1;
	}
	int XGridReport::event_GetFocus( )
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_GETFOCUS;
		hdr.DwTable = 0;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
		{
			((IGridReportObserver *)m_pObServers[i])->GetFocus();
		}
		return 1;
	}
	int XGridReport::event_LoseFocus()
	{
		this->HideEdit();
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_LOSEFOCUS;
		hdr.DwTable = 0;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
		{
			((IGridReportObserver *)m_pObServers[i])->LoseFocus();
		}
		return 1;
	}

	int XGridReport::event_EditorLoseFocus()
	{
		if(!this->m_pEditSvr->m_pEdit) return 1;
		LPTSTR pStr1 = this->m_pEditSvr->m_pEdit->GetText();
		LPTSTR pStr2 = (LPTSTR)this->GetItemString(FCurrent.y, FCurrent.x + 1);
		if(pStr1 && pStr2 && _tcscmp(pStr1, pStr2) == 0) return 1;

		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom =GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_EDITORLOSEFOCUS;
		hdr.DwTable = NULL;
		hdr.row = FCurrent.y;
		hdr.col = FCurrent.x;
		hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(FCurrent.x);
		hdr.data = pStr1;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		if(hdr.idFrom < 0)
		{
			::SetFocus(this->m_pEditSvr->m_pEdit->GetHWND());
			FEditorMode = true;
		}
		return hdr.idFrom;
		/*for(int i=0;i<(int)m_pObServers.size();i++)
		{
			((IGridReportObserver *)m_pObServers[i])->EditorLoseFocus();
		}*/
		return 1;
	}

	int XGridReport::event_ItemChanged(int nRow,int nCol,LPTSTR pData,LPTSTR pDisplayData)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom =GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMCHANGED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(nCol);
		hdr.data = pData;

		if(m_pSheetSvr && m_pSheetSvr->m_pSheetCtrl)
		{
			int ARow11 = GetTreeRow(nRow);
			LPTSTR path = GetGridDataSvr()->m_pContentSvr->GetXPath();
			m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged((wchar_t*)path,ARow11,hdr.colname,hdr.data);
			if(pDisplayData)
			{
				wstring colname = (wstring)hdr.colname;
				colname += (wstring)_T("/@_displaystring");
				m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged((wchar_t*)path,ARow11,(wchar_t*)colname.c_str(),pDisplayData);
			}
		}

		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IGridReportObserver *)m_pObServers[i])->ItemChanged((IGridReport *)GetInterface(),nRow,nCol,pData);

		return 1;
	}
	int XGridReport::event_ItemChanging(int nRow,int nCol,LPTSTR pData)
	{
		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom =GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMCHANGING;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetDwColumnAtCellColumn(nCol);
		hdr.data = pData;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IGridReportObserver *)m_pObServers[i])->ItemChanging((IGridReport *)GetInterface(),nRow,nCol,pData);

		return hdr.idFrom;
	}

	LPCTSTR XGridReport::GetDwColumnAtCellColumn(int ACol)
	{
		if(ACol > this->GetColumnCount()) return NULL;
		if(!m_pDataTempl) return NULL;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Detail.bValid) return NULL;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return NULL;

		CAxisSvr * pAxisSvrEx = GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return NULL;

		int startRow,endRow;
		startRow = pReportBand->Detail.startRow;
		endRow = pReportBand->Detail.endRow - 1;

		int StartCol = FTopLeft.x;
		int StartRow = FTopLeft.y;

		int startCell = FTopLeft.x;

		int ARow = startRow;
		TGridRect gRect;
		PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow);
		if(pItem && pItem->pStrColumn)
		{
			if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
			{
				if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
				{
					return pItem->pStrColumn;
				}
			}else
			{
				return pItem->pStrColumn;
			}
		}

		return NULL;
	}

	int XGridReport::CalcScrollBar(int Value, Cardinal ScrollCode, Cardinal ScrollBar, TGridDrawInfoEx& DrawInfo, int Pos)
	{
		int Result;
		Result = Value;
		switch(ScrollCode) 
		{
		case SB_LINEUP:
			Result = Value - 1;
			break;
		case SB_LINEDOWN:
			Result = Value + 1;
			break;
		case SB_PAGEUP:
			Result = Value - PageUp(ScrollBar,DrawInfo);
			break;
		case SB_PAGEDOWN:
			Result = Value + PageDown(ScrollBar);
			break;
		case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
			if((goThumbTracking && FOptions) || (ScrollCode == SB_THUMBPOSITION))
			{
				TGridCoord MaxTopLeft;
				MaxTopLeft.x = GetColCount() ;
				MaxTopLeft.y = GetRowCount() ;
				MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
				int nMax = ScrollBar == SB_HORZ? GetHScrollBar()->nMax:GetVScrollBar()->nMax;
				Result = ScrollBarMin(ScrollBar) + 
					MulDiv(Pos, ScrollBarMax(ScrollBar,MaxTopLeft)
					- ScrollBarMin(ScrollBar), nMax + 1);
				//Result = ScrollBarMin(ScrollBar) + 
				//	MulDiv(Pos, ScrollBarMax(ScrollBar,MaxTopLeft)
				//	- ScrollBarMin(ScrollBar), 100/*MaxShortInt*/);
				break;
			}
		case SB_BOTTOM:
			Result = ScrollBarMin(ScrollBar);
			break;
		case SB_TOP:
			Result = ScrollBarMin(ScrollBar);
			break;
		}
		return Result;
	}

	int XGridReport::ScrollBarMin(Cardinal ScrollBar)
	{
		return 1;
	}

	int XGridReport::ScrollBarMax(Cardinal ScrollBar,TGridCoord MaxTopLeft)
	{
		return ScrollBar == SB_HORZ? MaxTopLeft.x:MaxTopLeft.y;
	}

	
	int XGridReport::InitDrawInfo(TGridDrawInfoEx * pDrawInfo)
	{
		return m_pSheetSvr->InitDrawInfo(pDrawInfo);
	}

	void XGridReport::ModifyScrollBar(Cardinal ScrollBar, Cardinal ScrollCode, Cardinal Pos)
	{
		TGridCoord NewTopLeft, MaxTopLeft;
		TGridDrawInfoEx DrawInfo;

		//	if(/*Visible && CanFocus && */TabStop)
		//		m_pView->SendMessage(WM_SETFOCUS,0,0);
		InitDrawInfo(&DrawInfo);
		if(ScrollBar == SB_HORZ)
			DrawInfo.bCalcVert = false;
		else
			DrawInfo.bCalcVert = true;

		if((ScrollBar == SB_HORZ) && (GetColCount() == 1))
		{
			//ModifyPixelScrollBar(ScrollCode, Pos,DrawInfo);
			return;
		}

		MaxTopLeft.x = GetColCount() ;
		MaxTopLeft.y = GetRowCount() ;
		MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
		NewTopLeft = FTopLeft;
		if(ScrollBar == SB_HORZ) 
			NewTopLeft.x = CalcScrollBar(NewTopLeft.x,ScrollCode, ScrollBar, DrawInfo,Pos);
		else
			NewTopLeft.y = CalcScrollBar(NewTopLeft.y,ScrollCode, ScrollBar, DrawInfo,Pos);

		if(NewTopLeft.x < 1)
			NewTopLeft.x =1;
		else if(NewTopLeft.x > MaxTopLeft.x )
			NewTopLeft.x = MaxTopLeft.x;
		if(NewTopLeft.y < 1)
			NewTopLeft.y = 1;
		else if(NewTopLeft.y >= MaxTopLeft.y)
			NewTopLeft.y = MaxTopLeft.y;

		/*CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(pAxisSvr)
		{
			int width = pAxisSvr->GetColWidth(NewTopLeft.x);
			while(width <= 1 && NewTopLeft.x < GetColCount())
				width = pAxisSvr->GetColWidth(++NewTopLeft.x);		
		}*/

		if((NewTopLeft.x != FTopLeft.x) || (NewTopLeft.y != FTopLeft.y))
			MoveTopLeft(NewTopLeft.x, NewTopLeft.y);
	}

	void XGridReport::MoveTopLeft(int ALeft, int ATop)
	{
		TGridCoord OldTopLeft;

		if( (ALeft == FTopLeft.x) && (ATop == FTopLeft.y)) return;
		Update();
		OldTopLeft = FTopLeft;
		FTopLeft.x = ALeft;
		FTopLeft.y = ATop;
		if(FTopLeft.y < 1) FTopLeft.y=1;
		TopLeftMoved(OldTopLeft);
	}

	void XGridReport::TopLeftMoved(TGridCoord OldTopLeft)
	{

		UpdateScrollPos();

		InvalidateGrid();
		/*
		TGridDrawInfo DrawInfo;
		TGridCoord Delta;
		InitDrawInfo(&DrawInfo);

		bool bcs0,bcs1;
		DrawInfo.bCalcVert=false;
		bcs0 = CalcScroll(DrawInfo, OldTopLeft.x, FTopLeft.x, Delta.x);
		DrawInfo.bCalcVert=true;
		bcs1 = CalcScroll(DrawInfo, OldTopLeft.y, FTopLeft.y, Delta.y);
		if(bcs0 && bcs1) ScrollDataInfo(Delta.x, Delta.y, DrawInfo);
		*/
		TopLeftChanged();
	}
	
	int XGridReport::InvalidateGrid()
	{
		::InvalidateRect(this->GetHWND(),NULL,false);
		return 1;
	}

	TGridCoord  XGridReport::CalcMaxTopLeft(TGridCoord Coord, TGridDrawInfoEx& DrawInfo)
	{
		TGridCoord Result;

		DrawInfo.bCalcVert = false;
		Result.x = CalcMaxCell(DrawInfo, Coord.x);
		DrawInfo.bCalcVert = true;
		Result.y = CalcMaxCell(DrawInfo, Coord.y);

		return Result;
	}

	int XGridReport::PageUp(Cardinal ScrollBar,TGridDrawInfoEx& DrawInfo)
	{
		TGridCoord MaxTopLeft;

		int Result;
		MaxTopLeft = CalcMaxTopLeft(FTopLeft, DrawInfo);
		if(ScrollBar == SB_HORZ)
			Result = FTopLeft.x - MaxTopLeft.x; else
			Result = FTopLeft.y - MaxTopLeft.y;
		if(Result < 1) Result = 1;

		return Result;
	}

	int  XGridReport::PageDown(Cardinal ScrollBar)
	{
		TGridDrawInfoEx DrawInfo;
		this->InitDrawInfo(&DrawInfo);

		int Result;
		if(ScrollBar == SB_HORZ)
			Result =  DrawInfo.LastFullVisibleCol - FTopLeft.x;
		else
			Result =  DrawInfo.LastFullVisibleRow - FTopLeft.y;

		if(Result < 1)Result = 1;

		return Result;
	}

	bool XGridReport::EvtVScroll(TEvent *pEvent ,LPARAM lParam)
	{
		this->HideEdit();
		ModifyScrollBar(SB_VERT, pEvent->scroll.nSBCode & 0xff , (((pEvent->scroll.nSBCode)>>8)<<16) | pEvent->scroll.nPos);
		return true;
	}

	bool XGridReport::EvtHScroll(TEvent *pEvent ,LPARAM lParam)
	{
		this->HideEdit();
		ModifyScrollBar(SB_HORZ, pEvent->scroll.nSBCode & 0xff , (((pEvent->scroll.nSBCode)>>8)<<16) | pEvent->scroll.nPos);
		return true;
	}

	void XGridReport::UpdateScrollPos()
	{
		TGridDrawInfoEx DrawInfo;
		TGridCoord MaxTopLeft;

		//int GridSpace, ColWidth;

		if(FScrollBars == ssNone) return;
		InitDrawInfo(&DrawInfo);
		MaxTopLeft.x = GetColCount();
		MaxTopLeft.y = GetRowCount() ;
		if(MaxTopLeft.y < 1) MaxTopLeft.y = 1;
		MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
		if(FScrollBars & ssHorizontal || FScrollBars &  ssBoth)
		{
			if(GetColCount() == 1) 
			{
				/*ColWidth = GetFColWidths()[DrawInfo.Horz.FirstGridCell];
				GridSpace = GetClientWidth() - DrawInfo.Horz.FixedBoundary;
				if((FColOffset > 0) && (GridSpace > (ColWidth - FColOffset)))
				ModifyScrollBar(SB_HORZ, SB_THUMBPOSITION, ColWidth - GridSpace);
				else
				SetScroll(SB_HORZ, FColOffset);*/
			}
			else
			{
				if(GetHScrollBar())GetHScrollBar()->SetPosition(MulDiv(FTopLeft.x - 1, 100 /*MaxShortInt*/, MaxTopLeft.x - 1));
			}
		}
		if(FScrollBars & ssVertical || FScrollBars & ssBoth)
			if(GetVScrollBar())GetVScrollBar()->SetPosition(MulDiv(FTopLeft.y - 1, GetVScrollBar()->nMax /*MaxShortInt*/, MaxTopLeft.y - 1));
			//if(GetVScrollBar())GetVScrollBar()->SetPosition(MulDiv(FTopLeft.y - 1, 100 /*MaxShortInt*/, MaxTopLeft.y - 1));
	}

	void XGridReport::ScrollData(int DX, int DY)
	{
		TGridDrawInfoEx DrawInfo;

		InitDrawInfo(&DrawInfo);
		ScrollDataInfo(DX, DY, DrawInfo);
	}

	void XGridReport::ScrollDataInfo(int DX, int DY, TGridDrawInfoEx &DrawInfo)
	{
		RECT ScrollArea;
		int ScrollFlags;

		//TRect	UpdateRect;
		ScrollFlags = SW_INVALIDATE;
		ScrollFlags = ScrollFlags | SW_ERASE;
		//{ Scroll the area }
		if( DY == 0)
		{
			//   { Scroll both the column titles && data area at the same time }
			SetRect(&ScrollArea, m_pSheetSvr->m_pColHeaderSvr->FRect.left, m_pSheetSvr->m_pColHeaderSvr->FRect.top, FRect.right, FRect.bottom);
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);
		}
		else if( DX == 0)
		{
			//{ Scroll both the row titles && data area at the same time }
			SetRect(&ScrollArea, m_pSheetSvr->m_pSelectorSvr->FRect.left, m_pSheetSvr->m_pSelectorSvr->FRect.top, FRect.right, FRect.bottom);
			::ScrollWindowEx(GetHWND(),0, DY, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);
		}
		else
		{
			//{ Scroll titles && data area separately }
			//{ Column titles }
			ScrollArea =  m_pSheetSvr->m_pColHeaderSvr->FRect;
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);

			//{ Row titles }
			ScrollArea =  m_pSheetSvr->m_pSelectorSvr->FRect;
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);

			//{data Area}
			ScrollArea = FRect;
			::ScrollWindowEx(GetHWND(),DX, DY, &ScrollArea, &ScrollArea, 0,NULL, ScrollFlags);
		}
		UpdateWindow(GetHWND());
	}

	bool XGridReport::CalcScroll(TGridDrawInfoEx &AxisInfo, int OldPos, int CurrentPos, int &Amount)
	{
		int Start, Stop;
		bool Result;

		Result = false;

		if(OldPos < CurrentPos)
		{
			Start = OldPos;
			Stop = CurrentPos;
		}
		else
		{
			Start = CurrentPos;
			Stop = OldPos;
		}
		Amount = 0;
		for(int i = Start;i<=Stop - 1;i++)
		{
			Amount += (AxisInfo.bCalcVert?AxisInfo.pAxisSvr->GetRowHeight(i):AxisInfo.pAxisSvr->GetColWidth(i)) + AxisInfo.EffectiveLineWidth;
			if(Amount > (AxisInfo.bCalcVert? AxisInfo.GridRect.right - AxisInfo.GridRect.left : AxisInfo.GridRect.bottom - AxisInfo.GridRect.top))
			{
				//{ Scroll amount too big, redraw the whole thing }
				InvalidateGrid();
				return Result;
			}
		}
		if(OldPos < CurrentPos)Amount = -Amount;

		return true;
	}

	CXScrollBar * XGridReport::GetHScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pHScrollBar;
	}

	CXScrollBar * XGridReport::GetVScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pVScrollBar;
	}


	void XGridReport::TopLeftChanged()
	{
		//if(FEditorMode && FInplaceEdit) FInplaceEdit->UpdateLoc(CellRect(GetCol(), GetRow()));
	}

	int XGridReport::CalcMaxCell(TGridDrawInfoEx &Axis, int Start)
	{
		int Line;
		int Result;

		Result = Start;
		Line = Axis.bCalcVert?Axis.GridRect.bottom:Axis.GridRect.right;
		Line -= Axis.EffectiveLineWidth;
		for(int i= Start;i>=1;i--)
		{
			Line -= Axis.bCalcVert?Axis.pAxisSvr->GetRowHeight(i):Axis.pAxisSvr->GetColWidth(i);
			Line -= Axis.EffectiveLineWidth;
			if(Line < Axis.GridRect.left)
				break;
			Result = i;
		}
		return Result;
	}

	int XGridReport::SetColumnProps(int nCol,LPCTSTR pXmlProp)
	{
		int nRow = GetRowCount();
		int i;
		for(i=4;i<7;i++)
		{
			//2017.8.23	PStrItem pStrItem = m_pDataTempl->m_pContentSvr->GetItemData(nCol - 1,i);
			PStrItem pStrItem = m_pDataTempl->m_pContentSvr->GetItemData(nCol,i);
			if(!pStrItem) pStrItem = new StrItem;

			if(!pStrItem->pStrXml)
			{
				int nAlloc = _tcslen(pXmlProp) + 2 * _tcslen(L"<cell>") + 2;
				TCHAR * pStr = new TCHAR[nAlloc];
				_stprintf_s(pStr,nAlloc,L"<cell>%s</cell>",pXmlProp);
				
				pStrItem->pStrXml = pStr;
			}
			if(pStrItem && pStrItem->pStrXml)
			{
				xbXml xml;
				xml.LoadXml(pStrItem->pStrXml);
				IXMLDOMElement * pElement;
				IXMLDOMElement * pRoot;
				IXMLDOMNode * pNode;
				xml.m_pXmlDoc->selectSingleNode(L"cell/cellprop",&pNode);

				xbXml x;
				x.LoadXml(pXmlProp);
				xml.m_pXmlDoc->get_documentElement(&pRoot);
				if(!pRoot) return 1;
				if(pNode)
				{
					pRoot->removeChild(pNode,NULL);
					pNode->Release();
				}

				x.GetXmlDoc()->get_documentElement(&pElement);
				pRoot->appendChild(pElement,NULL);
				pElement->Release();

				if(pStrItem->pStrXml) delete pStrItem->pStrXml;
				pStrItem->pStrXml = NULL;
				BSTR bstr =NULL;
				pRoot->get_xml(&bstr);
				pStrItem->pStrXml = _tcsdup(bstr);
				::SysFreeString(bstr);
				pRoot->Release();
			}
			//m_pDataTempl->m_pContentSvr->SetItemData(nCol - 1,i,pStrItem);
/*
			if(pStrItem->pStrXml)
				pStrItem->pStrXml=NULL;
			if(!pStrItem->pStrXml)
			{
				pStrItem->pStrXml = (LPTSTR)pXmlProp;
				m_pDataTempl->m_pContentSvr->SetItemData(nCol - 1,i,pStrItem);
			}
*/		}
		return 1;
	}

	int XGridReport::CreateTree(LPCTSTR pPath,int nCol, int nImage1,int nImage2)
	{
		IXMLDOMElementPtr pSchemaElement;
		vector<XOffice::XXmlContentSvr::TTreeItem>* pTreeData = NULL;
		//((XGridDataSvrEx *)m_pSheetSvr->m_pDataSvr)->m_pContentSvr->GetTreeData(pTreeData);
		GetGridDataSvr()->m_pContentSvr->GetTreeData(pTreeData);
		if(!pTreeData) pTreeData = new vector<XOffice::XXmlContentSvr::TTreeItem>;
		XOffice::XXmlContentSvr::TTreeItem item;
		for(int i=0;i<GetRowCount();i++)
		{
			BSTR val=_T("@#$%^&*");
			BSTR pval=_T("@#$%^&*1");
			BSTR nval=_T("@#$%^&*2");

			IXMLDOMElement * pElement;
			IXMLDOMElement * ppElement;
			IXMLDOMElement * npElement;

			IXMLDOMNodePtr gNode;
			IXMLDOMNodePtr pgNode;
			IXMLDOMNodePtr ngNode;
			
			pElement = GetGridDataSvr()->m_pContentSvr->GetRowElement(i + 1);
			pElement->selectSingleNode((BSTR)pPath,&gNode);
			gNode->get_text(&val);
			pElement->Release();
			//gNode->Release();
	
			if(i>0)
			{
				ppElement = GetGridDataSvr()->m_pContentSvr->GetRowElement(i);
				ppElement->selectSingleNode((BSTR)pPath,&pgNode);
				pgNode->get_text(&pval);
				ppElement->Release();
				//pgNode->Release();
			}
			if(i<GetRowCount() - 1)
			{
				npElement = GetGridDataSvr()->m_pContentSvr->GetRowElement(i + 2);
				npElement->selectSingleNode((BSTR)pPath,&ngNode);
				ngNode->get_text(&nval);
				npElement->Release();
				//ngNode->Release();
			}
			wstring v = _bstr_t(val);
			wstring pv = _bstr_t(pval);
			wstring nv = _bstr_t(nval);

			if(v.compare(_T(""))==0)
			{
				item.nLevel = 0;
				item.children =  false;
				item.nImage = nImage1;				
			}
			else
				if(v.compare(pv)==0)//(val==pval)
				{
					item.nLevel = 1;
					item.children =  false;
					item.nImage = nImage2;
				}
				else
				{
					item.nLevel = 0;
					item.nImage = nImage1;
					if(v.compare(nv)==0)//(val==nval)
						item.children =  true;
					else
						item.children =  false;
				}
			item.nContentCol = nCol;
			item.nContentRow = i + 1;
			item.nImageList = 0;
			//item.nLevel = 1;
			//item.children =  false;
			item.expanded = false;
			item.nSelectedImage = -1;
			//item.nImage = nImage1;
			pTreeData->push_back(item);
		}
		//((XGridDataSvrEx *)m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetTreeData(pTreeData);	
		GetGridDataSvr()->m_pContentSvr->SetTreeData(pTreeData);	
		return 1;
	}

	LPCTSTR XGridReport::GetItemStringEx(int nRow,LPTSTR nColumn,DWBuffer dwBuffer)
	{
		nRow = GetTreeRow(nRow);

		if(!m_pDataTempl) return NULL;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Detail.bValid) return NULL;
		int startRow,endRow;
		int ARow = pReportBand->Detail.startRow;
		//int Col = this->GetColumnIndex(nColumn) - 1;
		int Col = this->GetColumnIndex(nColumn) ;
		PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(Col,ARow);
		if(pItem && pItem->pStrXml)
		{
			IXMLDOMElement * pElement1 = NULL;
			IXMLDOMNode * pNode;
			_variant_t var;

			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
				pElement1->getAttribute(L"cellsource",&var);
				LPTSTR pItemStr = NULL;
				if(var.bstrVal && ::_tcscmp(var.bstrVal, L"") != 0)
				{
					pItemStr = _tcsdup(var.bstrVal);
					if(pItemStr[0]=='=')
					{
						CXGridComplier x;
						if(x.SetSource(pItemStr, this, 0, nRow, Col))
							pItemStr = _tcsdup(x.GetString());
					}
				}
				pNode->Release();
				pElement1->Release();
				if(pItemStr) return pItemStr;
			}
		}

		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
			return m_pSheetSvr->m_pSheetCtrl->bindGetData(GetGridDataSvr()->m_pContentSvr->GetXPath(), nRow, (wchar_t*)nColumn);

		return GetGridDataSvr()->m_pContentSvr->GetItemString(nColumn,nRow);
	}

	LPCTSTR XGridReport::GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer)
	{
		nRow = GetTreeRow(nRow);
		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			wstring path = (wstring)this->GetColumnName(nColumn) + (wstring)_T("/@_displaystring");
			return m_pSheetSvr->m_pSheetCtrl->bindGetData(GetGridDataSvr()->m_pContentSvr->GetXPath(), nRow, (wchar_t*)path.c_str());
		}
		return GetGridDataSvr()->m_pContentSvr->GetItemDisplayString(nColumn,nRow);
	}

	bool XGridReport::SetItemDisplayString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer)
	{
		nRow = GetTreeRow(nRow);
		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			wstring path = (wstring)this->GetColumnName(nColumn) + (wstring)_T("/@_displaystring");
			return m_pSheetSvr->m_pSheetCtrl->bindNotifyDataChanged(GetGridDataSvr()->m_pContentSvr->GetXPath(), nRow, (wchar_t*)path.c_str(),(wchar_t*)lptStr);
		}
		GetGridDataSvr()->m_pContentSvr->SetItemDisplayString(nColumn,nRow,(LPTSTR)lptStr);
/*		PStrItem pStrItem = m_pDataTempl->m_pContentSvr->GetItemData(nColumn,nRow);
		if(!pStrItem) pStrItem = new StrItem;
		if(pStrItem)
		{
			if(pStrItem->pStrText) delete pStrItem->pStrText;
			pStrItem->pStrText = (LPTSTR)lptStr;
		}
*/		return true;
	}

	int XGridReport::AllowSort(bool sort)
	{
		m_bSort = sort;
		return 1;
	}

	int XGridReport::FindByColumn(int startRow, int columnIndex, LPCTSTR columnValue)
	{
		int rows = this->GetRowCount();
		if (startRow < 1)
			startRow = 1;
		if (startRow > rows)
			return -1;
		int cols = this->GetColumnCount();
		if(columnIndex < 1 || columnIndex > cols)
			return -1;
		int i;
		for(i=startRow; i<=rows; i++)
		{
			LPCTSTR str = this->GetItemString(i, columnIndex);
			if(str && columnValue && _tcsicmp(str, columnValue) == 0)
				return i;
		}
		return -1;
	}

	int XGridReport::FindByColumn(int startRow, LPCTSTR column, LPCTSTR columnValue)
	{
		int columnIndex = this->GetColumnIndex(column);
		if(columnIndex < 0)
			return -1;
		return FindByColumn(startRow, columnIndex, columnValue);
	}

	int XGridReport::SetReadOnlyColumn(LPCTSTR column, bool flag)
	{
		if(flag)
		{
			int i;
			for(i=0; i < int(m_ReadOnlyColumn.size()); i++)
			{
				if(_tcscmp(column, m_ReadOnlyColumn[i]) == 0)
				{
					return 1;
				}
			}
			m_ReadOnlyColumn.push_back(column);
			return 1;
		}
		else
		{
			for(vector<LPCTSTR>::iterator it=m_ReadOnlyColumn.begin(); it!=m_ReadOnlyColumn.end();)
			{
				if(column == *it)
				{
					m_ReadOnlyColumn.erase(it);
					return 1;
				}
				else
					++it;
			}
		}
		return -1;
	}

	int XGridReport::SetReadOnlyColumn(int columnIndex, bool flag)
	{
		if(columnIndex < 0 || columnIndex>this->GetColumnCount()) return -1;
		LPCTSTR column = this->GetColumnName(columnIndex);
		if(!column)
			return -1;
		return SetReadOnlyColumn(column, flag);
	}

	int XGridReport::SetColumnWidth(int columnIndex, int width)
	{
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		CAxisSvr * pAxisSvr1 = GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr || !pAxisSvr1) return -1;
		pAxisSvr->SetColWidth(columnIndex, width);
		pAxisSvr1->SetColWidth(columnIndex, width);
		RECT re;
		::GetClientRect(this->GetHWND(), &re);
		m_pSheetSvr->m_pColHeaderSvr->OnDraw(::GetDC(this->GetHWND()), re);
		this->Redraw();
		return 1;
	}

	int XGridReport::GetColumnWidth(int columnIndex)
	{
		if(!m_pDataTempl) return -1;
		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return -1;
		return pAxisSvr->GetColWidth(columnIndex, true);
	}

	int XGridReport::SetColumnState(int columnIndex, bool state)
	{
		int ordWidth = GetColumnWidth(columnIndex);
		if(state)
			SetColumnWidth(columnIndex, abs(ordWidth));
		else
			SetColumnWidth(columnIndex, abs(ordWidth) * -1);
		return ordWidth;
	}

	void XGridReport::SetDataToClip(LPCTSTR DataStr)
	{    
		//char* szText = XUtil::strdupFromBstr((BSTR)DataStr);
		LPCTSTR szText = DataStr;
		int nLen = (wcslen(szText) + 1) * sizeof(szText[0]);
		if (::OpenClipboard (NULL)) {
			::EmptyClipboard ();
			HANDLE hData = ::GlobalAlloc (GMEM_MOVEABLE, nLen);
			LPWSTR pData = (LPWSTR)::GlobalLock (hData);
			//::lstrcpy (pData, szText);
			memcpy(pData, szText, nLen); 
			::GlobalUnlock(hData);

			::SetClipboardData(CF_UNICODETEXT, hData);
			::CloseClipboard ();
		}
		return;    
	} 

	int XGridReport::ShowRowTo(int row)
	{
		int xrow = this->GetRowCount();
		if(xrow == 0) return -1;

		int prow = this->GetPageRowCount();

		/*if(row<2 || row < prow || xrow < prow)
		{
			MoveTopLeft(FTopLeft.x, 1);
		}
		else
		{
			if(xrow < row) row = xrow;
			MoveTopLeft(FTopLeft.x, row - prow - 1);
		}*/
		if(xrow < row) row = xrow;
		if(row != 1)
		{
			if((xrow - row) < prow)
				row = xrow - prow + 2;
			if(row < 1) row = 1;
		}
		else
		{
			this->SelectRow(row);
			this->SetRow(row);		
		}
		MoveTopLeft(FTopLeft.x, row);
		this->Redraw();
		return 1;
	}

	int XGridReport::SetRowSort(LPTSTR colName, bool colOrder)
	{
		//if(!m_bSort) m_bSort = true;

		m_pSortOrders->clear();
		m_pSortColumns->clear();
		m_pSortColumnTypes->clear();
		m_pSortColumns->push_back(colName);
		m_pSortOrders->push_back(colOrder);

		LPTSTR celltype = GetColumnProp(this->GetColumnIndex(colName),L"celltype");
		if(!celltype) celltype=L"";
		m_pSortColumnTypes->push_back(celltype);
		if(this->hasBindData())
		{
			wchar_t buf[255];
			swprintf(buf,255,L"sort:%s %s %s",colName,colOrder?L"ASC":L"DESC",celltype[0]?celltype:L"xs:string");
			this->m_pSheetSvr->m_pSheetCtrl->bindGetData(GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, buf);
		}
		else
			GetGridDataSvr()->m_pContentSvr->Sort(m_pSortColumns, m_pSortOrders,m_pSortColumnTypes,colOrder);
		if(GetHWND()) ::InvalidateRect(GetHWND(),NULL,FALSE); 
		
		return 1;
	}

	bool XGridReport::SetRowMove(int row, int movecount)
	{
		return GetGridDataSvr()->m_pContentSvr->MoveRow(row, movecount);
	}	

	bool XGridReport::IsEditQuery(int ACol,int ARow,bool &bShowButton)
	{
		bShowButton = false;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Detail.bValid) return false;
		int startRow = pReportBand->Detail.startRow;
		PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,startRow);
		if(!pItem) return false;

		IXMLDOMElement * pElement = NULL;
		IXMLDOMNode * pNode;
		LPTSTR pEditType = NULL;
		_variant_t var;
		if(pItem && pItem->pStrXml)
		{
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
			}
		}
		bool bQuery = false;
		if(pElement)
		{
			if(pEditType && _tcsicmp(pEditType,_T("query"))==0) bQuery = true;
			pElement->getAttribute(L"dddwcanedit",&var);
			if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0) bShowButton = true;

			pElement->Release();
		}
		return bQuery;
	}

	int XGridReport::ShowQueryDlgEx(LPTSTR pData)
	{
		bool bxpage = false;
		TCHAR buf[512];
		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Detail.bValid) return false;
		int startRow = pReportBand->Detail.startRow;
		PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(GetCol(),startRow);
		if(pItem && pItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"dddwxpageurl",&var);
				if(var.bstrVal && var.bstrVal[0] !=0)
				{
					LPTSTR pCellSource = _tcsdup(var.bstrVal);
					LPTSTR pText = var.bstrVal;
					if(pCellSource[0]=='=')
					{
						CXGridComplier x;
						if(x.SetSource(pCellSource + 1, this, 0, startRow, GetCol()))
							pText = x.GetString();
					}
					if(pCellSource) delete pCellSource;
					if(_tcsstr(pText,_T("dev:xpage"))==pText)
						::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pText);
					else
						::_stprintf_s(buf,512,_T("%s"),pText);
					
					bxpage = true;
				}
				pElement->Release();
				pNode->Release();
			}
		}
		
		if(!bxpage)
		{
			XGridQueryDlg  * pWin = new XGridQueryDlg();
			xfXPage* pXPage = new xfXPage();

			TCHAR path[MAX_PATH];
			::GetModuleFileName(NULL,path,MAX_PATH);
			::PathRemoveFileSpec(path);
			TCHAR buf[MAX_PATH];
			::_stprintf_s(buf,MAX_PATH,_T("%s\\agent\\xcell\\querypage.xml"),path);
			
			xbXml xml;
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.Load(buf);
			xml.m_pXmlDoc->selectSingleNode(L"/xpages/xpage[@name='querywin']/xframe",&pNode);
			if(!pNode) return 1;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
			pXPage->Load(pElement);
			pElement->Release();

			RECT rc;
			GetCellRect(FCurrent.x,FCurrent.y,rc);

			POINT ptStart,ptEnd;
			ptStart.x = rc.left;
			ptStart.y = rc.top;
			ClientToScreen(GetHWND(),&ptStart);
			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;
			ClientToScreen(GetHWND(),&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);

			pWin->Open(pXPage,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,NULL,NULL,(LPVOID)this);
			::SetProp(pWin->GetHWND(),_T("this__"),this->m_pSheetSvr->m_pSheetCtrl->GetWin());
			pWin->OnOpenQueryEx(pData);
		}else
		{
			RECT rc;
			GetCellRect(FCurrent.x,FCurrent.y,rc);

			POINT ptStart,ptEnd;
			ptStart.x = rc.left;
			ptStart.y = rc.top;
			ClientToScreen(GetHWND(),&ptStart);
			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;
			ClientToScreen(GetHWND(),&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
			
			this->HideEdit();

			xfWin* pWin = xfWin::LoadXPage(NULL,buf,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,NULL,NULL,(LPVOID)this->GetInterface());
			//xfWin * xx = this->m_pSheetSvr->m_pSheetCtrl->GetWin();
			::SetProp(pWin->GetHWND(),_T("this__"),this->m_pSheetSvr->m_pSheetCtrl->GetWin());
			if(pData)
			{
				TCHAR buf[512];
				::_stprintf_s(buf,512,_T("InitText:%s"),pData);
				::SendMessage(pWin->GetHWND(), WM_XCOMMAND, (WPARAM)buf, (LPARAM)pWin);
			}
		}
		return 1;
	}

	int XGridReport::ShowQueryDlg(LPTSTR pData)
	{
		bool bxpage = false;
		TCHAR buf[512];
		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Detail.bValid) return false;
		int startRow = pReportBand->Detail.startRow;
		PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(GetCol(),startRow);
		if(pItem && pItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"dddwxpageurl",&var);
				if(var.bstrVal && var.bstrVal[0] !=0)
				{
					LPTSTR pCellSource = _tcsdup(var.bstrVal);
					LPTSTR pText = var.bstrVal;
					if(pCellSource[0]=='=')
					{
						CXGridComplier x;
						if(x.SetSource(pCellSource + 1, this, 0, startRow, GetCol()))
							pText = x.GetString();
					}
					if(pCellSource) delete pCellSource;
					if(_tcsstr(pText,_T("dev:xpage"))==pText)
						::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pText);
					else
						::_stprintf_s(buf,512,_T("%s"),pText);
					
					bxpage = true;
				}
				pElement->Release();
				pNode->Release();
			}
		}
		
		if(!bxpage)
		{
			XGridQueryDlg  * pWin = new XGridQueryDlg();
			xfXPage* pXPage = new xfXPage();

			TCHAR path[MAX_PATH];
			::GetModuleFileName(NULL,path,MAX_PATH);
			::PathRemoveFileSpec(path);
			TCHAR buf[MAX_PATH];
			::_stprintf_s(buf,MAX_PATH,_T("%s\\agent\\xcell\\querypage.xml"),path);
			
			xbXml xml;
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.Load(buf);
			xml.m_pXmlDoc->selectSingleNode(L"/xpages/xpage[@name='querywin']/xframe",&pNode);
			if(!pNode) return 1;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
			pXPage->Load(pElement);
			pElement->Release();

			RECT rc;
			GetCellRect(FCurrent.x,FCurrent.y,rc);

			POINT ptStart,ptEnd;
			ptStart.x = rc.left;
			ptStart.y = rc.top;
			ClientToScreen(GetHWND(),&ptStart);
			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;
			ClientToScreen(GetHWND(),&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);

			pWin->Open(pXPage,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,NULL,NULL,(LPVOID)this);
			::SetProp(pWin->GetHWND(),_T("this__"),this->m_pSheetSvr->m_pSheetCtrl->GetWin());
			pWin->OnOpenQuery(pData);
		}else
		{
			RECT rc;
			GetCellRect(FCurrent.x,FCurrent.y,rc);

			POINT ptStart,ptEnd;
			ptStart.x = rc.left;
			ptStart.y = rc.top;
			ClientToScreen(GetHWND(),&ptStart);
			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;
			ClientToScreen(GetHWND(),&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
			
			this->HideEdit();

			xfWin* pWin = xfWin::LoadXPage(NULL,buf,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,NULL,NULL,(LPVOID)this->GetInterface());
			//xfWin * xx = this->m_pSheetSvr->m_pSheetCtrl->GetWin();
			::SetProp(pWin->GetHWND(),_T("this__"),this->m_pSheetSvr->m_pSheetCtrl->GetWin());
			if(pData)
			{
				TCHAR buf[512];
				::_stprintf_s(buf,512,_T("InitText:%s"),pData);
				::SendMessage(pWin->GetHWND(), WM_XCOMMAND, (WPARAM)buf, (LPARAM)pWin);
			}
		}
		return 1;
	}
	int XGridReport::SetClickEditFlag(bool f)
	{
		m_bClickEdit = f;
		return 1;
	}

	LPCTSTR XGridReport::GetTitleName(int col)
	{
		if(col < 1 || col > this->GetColCount()) return NULL;
		CReportBandSvr::TReportBand *  pReportBand = &(m_pDataTempl)->m_ReportBand->m_ReportBand;
		if(!pReportBand->Title.bValid) return NULL;
		int endRow = pReportBand->Title.endRow - 1;
		TGridRect arect;
		m_pDataTempl->m_pSpanSvr->isSpaned(col,endRow,arect);
		PStrItem pItem = m_pDataTempl->m_pContentSvr->GetItemData(arect.left,arect.top);
		if(pItem && pItem->pStrContent) return ::_tcsdup(pItem->pStrContent);
		return NULL;
	}

	xbObject*  XGridReport::GetInterface()
	{
		if(m_pInterFace) return m_pInterFace;
		m_pInterFace = (xbObject *)new IGridReport();
		((IGridReport *)m_pInterFace)->m_pGridReport = this;
		return m_pInterFace;
	}

	bool XGridReport::IsTreeRowShow(int row)
	{
		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		if(((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
		{
			int nIndex = row - 1;//XXmlContentSvr::GetTreeIndex(pTreeItems,row);
			if(nIndex < 0 || nIndex >= (int)(*pTreeItems).size()) return true;
			int pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,nIndex);
			while(pIndex > -1)
			{
				if(!(*pTreeItems)[pIndex].expanded) return false;
				pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,pIndex);	
			}
		}	
		return true;
	}

	int XGridReport::GetTreeRow(int row)
	{
		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		if(((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
		{
			int nIndex = XXmlContentSvr::GetTreeIndex(pTreeItems,row);
			if(nIndex >=0) row = nIndex + 1;
		}
		return row;
	}

	int XGridReport::GetTreeRowCount()
	{
		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		if(((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
		{
			int nIndex = 1;
			int nRows = 0;
			while(nIndex <= (int)(*pTreeItems).size())
			{
				if(IsTreeRowShow(nIndex)) nRows++;
				nIndex++;
			}
			return nRows;
		}
		
		return GetRowCount();
	}

	int XGridReport::TreeRowExpand(int row, bool exp)
	{
		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		if(((XXmlContentSvr *)GetGridDataSvr()->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
		{
			int nIndex = XXmlContentSvr::GetTreeIndex(pTreeItems,row);
			if(nIndex > -1 && (*pTreeItems)[nIndex].children == true)
			{
				(*pTreeItems)[nIndex].expanded = exp;
				::InvalidateRect(this->GetHWND(),NULL,false);
			}
		}
		GetVScrollBar()->nMax = GetTreeRowCount();
		return 1;
	}

	int XGridReport::SetHeaderText(int row, int col, LPTSTR name)
	{
		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Detail.bValid) return -1;

		int startRow = pReportBand->Header.startRow;
		m_pDataTempl->m_pContentSvr->SetItemString(col, startRow + row, name);
		return 1;
	}

	LPTSTR XGridReport::GetColumnProp(int nCol,LPCTSTR pAttribute)
	{
		int startRow;
		CReportBandSvr::TReportBand *  pReportBand = &this->m_pDataTempl->m_ReportBand->m_ReportBand;
		startRow = pReportBand->Detail.startRow;

		return GetColumnProp(startRow, nCol, pAttribute);
		/*PStrItem pStrItem = m_pDataTempl->m_pContentSvr->GetItemData(nCol,startRow);
	
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				_variant_t var;
				LPTSTR pItemStr = NULL;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					pElement->getAttribute((BSTR)pAttribute,&var);
					pElement->Release();
					if(var.bstrVal)//return _tcsdup(var.bstrVal);
					{
						pItemStr = var.bstrVal;
						if(pItemStr[0]=='=')
						{
							CXGridComplier x;
							if(x.SetSource(pItemStr, this, 0, startRow, nCol))
								pItemStr = _tcsdup(x.GetString());							
						}
					}
				}
				pNode->Release();
				return pItemStr;
			}
		}
		return NULL;*/
	}

	LPTSTR XGridReport::GetColumnProp(int row, int nCol,LPCTSTR pAttribute)
	{
		int startRow;
		CReportBandSvr::TReportBand *  pReportBand = &this->m_pDataTempl->m_ReportBand->m_ReportBand;
		startRow = pReportBand->Detail.startRow;
		PStrItem pStrItem = m_pDataTempl->m_pContentSvr->GetItemData(nCol,startRow);
	
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				_variant_t var;
				LPTSTR pItemStr = NULL;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					pElement->getAttribute((BSTR)pAttribute,&var);
					pElement->Release();
					if(var.bstrVal)//return _tcsdup(var.bstrVal);
					{
						pItemStr = var.bstrVal;
						if(pItemStr[0]=='=')
						{
							CXGridComplier x;
							if(x.SetSource(pItemStr, this, 0, row, nCol))
								pItemStr = _tcsdup(x.GetString());							
						}
					}
				}
				pNode->Release();
				return pItemStr;
			}
		}
		return NULL;
	}
	void XGridReport::SetEditUpperMode(bool upper)
	{
		m_EditUpperMode = upper;
	}
	bool XGridReport::GetEditUpperMode()
	{
		return m_EditUpperMode;
	}

}}}}
