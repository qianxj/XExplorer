#include "StdAfx.h"
#include "..\include\XGridHeaderEx.hpp"
#include "XGridDataSvrEx.hpp"
#include "XGridReport.hpp"
#include "XGridSheetSvrEx.hpp"
#include "XGridFilterDlg.hpp"

extern HMODULE hCellModula;
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridHeaderEx::XGridHeaderEx(void):m_pDataTempl(NULL),FState(gsNormal),m_nFECol(-1),m_bColQuery(false),m_pColumnQueryList(NULL)
	{
		SetEventListons();
	}
	XGridHeaderEx::~XGridHeaderEx(void)
	{
		UnSetEventListons();
	}

	int XGridHeaderEx::GetTitleHeight()
	{
		if(!m_pDataTempl) return 0;
		return m_pDataTempl->m_ReportBand->GetTitleHeight(m_pDataTempl->m_pAxisSvr);
		
	}

	bool XGridHeaderEx::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		if(!m_pDataTempl) return true;
		if(!m_pGridReportSvr) return true;

		POINT point = pEvent->mouse.point;
		int ACol;
		int nPos;
		CalcSizingState(point,FRect,ACol,nPos);
		if(m_bColQuery && nPos - point.x <=16)
		{
			FState = gsNormal;
			return true;
		}
		bool FSort = this->m_pGridReportSvr->m_bSort;
		DrawSizingLine(point);
		if(FState == gsColSizing)
		{
			FState = gsNormal;
			if(m_pDataTempl)
			{
				int width = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr->GetColWidth(m_nSCol);
				width += point.x - m_nSPos;
				if(width > 0)
				{
					m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr->SetColWidth(m_nSCol, width);
					this->m_pDataTempl->m_pAxisSvr->SetColWidth(m_nSCol, width);
				}
			}
			//this->ReleaseCapture();
		}
		else if(FState == gsColMoving && ACol != m_nSCol)
		{
			FState = gsNormal;
			int clcounts = m_pDataTempl->m_pContentSvr->GetValidCol();//m_pData->GetCount();
			if((m_nSCol >0 && m_nSCol <= clcounts) && (ACol >0 && ACol <= clcounts))
			{				
				MoveColumn(m_nSCol, ACol);

				/*m_pDataTempl->m_pContentSvr->MoveCol(m_nSCol, ACol);

				CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
				CAxisSvr * pAxisSvr1 = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;
				if(!pAxisSvr || !pAxisSvr1) return -1;

				int tmp = pAxisSvr->m_pColWidths->GetValue(m_nSCol);
				pAxisSvr->m_pColWidths->Remove(m_nSCol, 1);
				pAxisSvr->m_pColWidths->Insert(ACol, 1);
				pAxisSvr->m_pColWidths->SetValue(ACol, tmp);

				tmp = pAxisSvr1->m_pColWidths->GetValue(m_nSCol);
				pAxisSvr1->m_pColWidths->Remove(m_nSCol, 1);
				pAxisSvr1->m_pColWidths->Insert(ACol, 1);
				pAxisSvr1->m_pColWidths->SetValue(ACol, tmp);
				
				LPTSTR tmp1 = (*((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths)[m_nSCol];
				((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->erase(((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->begin()+m_nSCol);
				((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->insert(((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->begin()+ACol, _tcsdup(tmp1));
				
				tmp1 = (*m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols)[m_nSCol];
				m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->erase(m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->begin()+m_nSCol);
				m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->insert(m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->begin()+ACol, _tcsdup(tmp1));

				CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
				if(!pReportBand->Title.bValid) return true;
				//ARow一行标题的情况，多行标题现不适用
				int ARow = pReportBand->Title.startRow;
				int sty = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[m_nSCol]->GetValue(ARow);
				int sty1 = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[ACol]->GetValue(ARow);
				(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[m_nSCol]->SetValue(ARow, sty1);
				(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[ACol]->SetValue(ARow, sty);
				*/
			}
			//this->ReleaseCapture();
			//if(GetHWND()) ::InvalidateRect(GetHWND(),NULL,FALSE); 
			//m_pGridReportSvr->Redraw();
		}
		/*else if(FSort)
		{
			int ACol = this->m_pGridReportSvr->CalcColFromPoint(point.x,point.y);
			m_pGridReportSvr->FCurrent.x = ACol;
			LPCTSTR pColumn = m_pGridReportSvr->GetDwColumnAtCellColumn(ACol);
			if(pColumn)
			{
				bool order = true;
				bool bFound = false;
				int i=0;
				for(i=0;i<(int)m_pGridReportSvr->m_pSortColumns->size();i++)
				{
					if(_tcsicmp((*m_pGridReportSvr->m_pSortColumns)[i].c_str(),pColumn)==0)
					{
						order = (*m_pGridReportSvr->m_pSortOrders)[i];
						bFound = true;
						break;
					}
				}
				if(::GetKeyState(VK_CONTROL)<0)
				{
					if(bFound)
						(*m_pGridReportSvr->m_pSortOrders)[i] = !(*m_pGridReportSvr->m_pSortOrders)[i];
					else
					{
						if((int)m_pGridReportSvr->m_pSortColumns->size() > 0)
						{
							int last = (int)m_pGridReportSvr->m_pSortColumns->size() - 1;
							m_pGridReportSvr->m_pSortOrders->push_back((*m_pGridReportSvr->m_pSortOrders)[last]);
						}else
							m_pGridReportSvr->m_pSortOrders->push_back(order);
						m_pGridReportSvr->m_pSortColumns->push_back(pColumn);

					}
				}else
				{
					if(bFound) order = !(*m_pGridReportSvr->m_pSortOrders)[i];
					m_pGridReportSvr->m_pSortOrders->clear();
					m_pGridReportSvr->m_pSortColumns->clear();
					m_pGridReportSvr->m_pSortColumns->push_back(pColumn);
					m_pGridReportSvr->m_pSortOrders->push_back(order);
				}
				m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->Sort(m_pGridReportSvr->m_pSortColumns,
					m_pGridReportSvr->m_pSortOrders);
				this->ReleaseCapture();
				if(GetHWND()) ::InvalidateRect(GetHWND(),NULL,FALSE); 
			}
		}*/
		else if(ACol == m_nSCol)
		{
			int nCol = ACol;
			CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;				
			if(!pReportBand->Title.bValid) return true;
			//ARow一行标题的情况，多行标题现不适用
			int ARow = pReportBand->Title.startRow;
			TGridRect re;
			if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,re)) ACol = re.left;
			DWNMHDR hdr;
			memset(&hdr,0,sizeof(DWNMHDR));
			hdr.hwndFrom = m_pGridReportSvr->GetHWND();
			hdr.idFrom = 0;
			hdr.code = DWV_CLICKED;
			hdr.DwTable = NULL;
			hdr.row = 0;
			hdr.col = ACol;
			hdr.colname = (LPTSTR)m_pGridReportSvr->GetDwColumnAtCellColumn(nCol);
			hdr.data = (LPTSTR)m_pGridReportSvr->GetTitleName(ACol);
			::SendMessage(::GetParent(m_pGridReportSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		}
		this->ReleaseCapture();
		if(GetHWND()) ::InvalidateRect(GetHWND(),NULL,FALSE); 
		FState = gsNormal;
		return true;
	}

	int XGridHeaderEx::MoveingColumn(int oldColumn, int newColumn)
	{
		int ACol = newColumn;
		int _Col = oldColumn;

		CAxisSvr * pAxisSvr = m_pDataTempl->m_pAxisSvr;
		CAxisSvr * pAxisSvr1 = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr || !pAxisSvr1) return -1;

		m_pDataTempl->m_pContentSvr->MoveCol(_Col, ACol);

		int tmp = pAxisSvr->m_pColWidths->GetValue(_Col);
		pAxisSvr->m_pColWidths->Remove(_Col, 1);
		pAxisSvr->m_pColWidths->Insert(ACol, 1);
		pAxisSvr->m_pColWidths->SetValue(ACol, tmp);

		tmp = pAxisSvr1->m_pColWidths->GetValue(_Col);
		pAxisSvr1->m_pColWidths->Remove(_Col, 1);
		pAxisSvr1->m_pColWidths->Insert(ACol, 1);
		pAxisSvr1->m_pColWidths->SetValue(ACol, tmp);
		
		LPTSTR tmp1 = (*((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths)[_Col];
		((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->erase(((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->begin()+_Col);
		((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->insert(((XGridDataSvrEx *)m_pGridReportSvr->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->m_pXPaths->begin()+ACol, _tcsdup(tmp1));
		
		tmp1 = (*m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols)[_Col];
		m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->erase(m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->begin()+_Col);
		m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->insert(m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->m_pCols->begin()+ACol, _tcsdup(tmp1));

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		if(!pReportBand->Title.bValid) return true;
		//ARow一行标题的情况，多行标题现不适用
		int ARow = pReportBand->Title.startRow;
		if(oldColumn > newColumn) //往前移
		{
			int sty = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[oldColumn]->GetValue(ARow);
			for(int i= oldColumn; i>newColumn; i--)
			{
				int sty1 = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[i-1]->GetValue(ARow);
				(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[i]->SetValue(ARow, sty1);
			}
			(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[newColumn]->SetValue(ARow, sty);
		}
		else
		{
			int sty = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[oldColumn]->GetValue(ARow);
			for(int i= oldColumn; i<newColumn; i++)
			{
				int sty1 = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[i+1]->GetValue(ARow);
				(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[i]->SetValue(ARow, sty1);
			}
			(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[newColumn]->SetValue(ARow, sty);			
		}
		/*int sty = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[oldColumn]->GetValue(ARow);
		int sty1 = (*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[newColumn]->GetValue(ARow);
		(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[oldColumn]->SetValue(ARow, sty1);
		(*m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->m_pCssClass)[newColumn]->SetValue(ARow, sty);*/
		return 1;
	}

	int XGridHeaderEx::MoveColumn(int oldColumn, int newColumn)
	{
		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;				
		if(!pReportBand->Title.bValid) return -99;
		//ARow一行标题的情况，多行标题现不适用
		int ARow = pReportBand->Title.startRow;
		int ACol = newColumn;
		int _Col = oldColumn;

		TGridRect oldRect;
		bool oldSpan = m_pDataTempl->m_pSpanSvr->isSpaned(_Col,ARow,oldRect);

		TGridRect newRect;
		bool newSpan = m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,newRect);

		//同一个格子
		if(oldRect.left == newRect.left) return 1;
		//相邻格子 还是拉倒老地方
		if((oldRect.right + 1) == newRect.left) return 1;

		int sCol = oldRect.left;
		int eCol = oldRect.right;
		int newLeft = newRect.left;
		if(oldSpan)
		{
			m_pDataTempl->m_pSpanSvr->UnMerge(oldRect);
		}
		if(oldRect.left > newRect.left) //后面拉到前面
		{
			for(int i = 0;i<m_pDataTempl->m_pSpanSvr->SpanRects.size();i++)
			{
				RECT rect = m_pDataTempl->m_pSpanSvr->SpanRects[i];
				if(rect.left >= newLeft && rect.right <= oldRect.left)
				{
					m_pDataTempl->m_pSpanSvr->SpanRects[i].left += eCol - sCol + 1;
					m_pDataTempl->m_pSpanSvr->SpanRects[i].right += eCol - sCol + 1;
				}
			}
			for(int i=eCol; i>=sCol; i--)
			{
				MoveingColumn(eCol, newLeft);
			}
		}
		else  //前面拉到后面
		{
			for(int i = 0;i<m_pDataTempl->m_pSpanSvr->SpanRects.size();i++)
			{
				RECT rect = m_pDataTempl->m_pSpanSvr->SpanRects[i];
				if(rect.left >= oldRect.right && rect.right <= newRect.right)
				{
					m_pDataTempl->m_pSpanSvr->SpanRects[i].left -= eCol - sCol + 1;
					m_pDataTempl->m_pSpanSvr->SpanRects[i].right -= eCol - sCol + 1;
				}
			}
			for(int i=sCol; i<=eCol; i++)
			{
				MoveingColumn(sCol, newRect.right);
			}
			newLeft = newRect.right - (eCol - sCol);
		}
		if(oldSpan)
		{
			oldRect.left = newLeft;
			oldRect.right = oldRect.left + eCol - sCol;
			m_pDataTempl->m_pSpanSvr->Merge(oldRect);
		}

		return 1;
	}

	void XGridHeaderEx::OnBaseDraw(HDC hdc,RECT &rect, int nSDrawCol, int nEDrawCol)
	{
		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Title.bValid) return;
		CAxisSvr * pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;//m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return;

		int startRow,endRow;
		startRow = pReportBand->Title.startRow;
		endRow = pReportBand->Title.endRow - 1;

		LOGFONT	logFont;
		logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
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
		HFONT	hFontB =::CreateFontIndirect(&logFont);
		int hbr;
		hbr = ::SetBkMode(hdc,TRANSPARENT);

		int width = rect.right- rect.left;

		int StartCol = nSDrawCol;
		int StartRow = m_pGridReportSvr->FTopLeft.y;

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

		LPTSTR pCssStr = _tcsdup(_T("background-color:#D8E7FC"));
		XUtil::Css::TBoxStyle * pStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
		delete pCssStr;
		
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
					TCHAR * pText = NULL;
					TGridRect gRect;
					bDraw = true;
					PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow);
					bool spaned = false;
					if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
					{
						if(gRect.left != gRect.right) spaned = true;
						if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
						{
							if(pItem && pItem->pStrContent) pText = pItem->pStrContent;
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
						if(pItem && pItem->pStrContent) pText = pItem->pStrContent;
					}

					COLORREF TextColor = ::GetTextColor(hdc);
					HFONT hFont2 = NULL;
					if(bDraw)
					{
						XUtil::Css::DrawCssFrame(hdc,&ARect,pStyle);

						LOGFONT	LogFont2;
						if(m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->GetFont(ACol,ARow,LogFont2) == 1)
						{
							hFont2 = ::CreateFontIndirect(&LogFont2);
							hFont2 = (HFONT)::SelectObject(hdc,hFont2);
						}

						const XUtil::Css::TBoxStyle* pBoxStyle = m_pGridReportSvr->m_pDataTempl->GetStyleSvr()->GetCellCssStyle(ACol,ARow);
						if(pBoxStyle)
						{
							
							if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
							{
								::SetTextColor(hdc,pBoxStyle->pColor->color[0]);
							}
						}
					}

					if(pText)
					{
						if(_tcschr(pText,'\r'))
							::DrawTextW(hdc,pText,(int)::_tcslen(pText),&ARect,DT_CENTER|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
						else
						{
							SIZE sz;
							GetTextExtentPoint32(hdc,pText,(int)::_tcslen(pText),&sz);
							if(sz.cx < ARect.right - ARect.left)
								::DrawText(hdc,pText,(int)::_tcslen(pText),&ARect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_VCENTER|DT_EDITCONTROL|DT_NOPREFIX);
							else
								::DrawTextW(hdc,pText,(int)::_tcslen(pText),&ARect,DT_CENTER|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
							}
					}
					if(m_bColQuery && !spaned && pText)
					{
						HBITMAP hbp = NULL;
						TCHAR path[MAX_PATH];
						::GetModuleFileName(NULL,path,MAX_PATH);
						::PathRemoveFileSpec(path);
						TCHAR buf[MAX_PATH];
						::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\down.bmp"),path);

						hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
						HDC hMemDC = CreateCompatibleDC(hdc);
						hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

						int dTop = (ARect.bottom - ARect.top)/2 - 8;
						BitBlt(hdc,ARect.right-16,ARect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

						hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
						::DeleteObject(hbp);
						::DeleteDC(hMemDC);
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
				}else if(nEDrawCol < 0)
				{
					RECT bRect;
					::SetRect(&bRect,ARect.right+1, ARect.top, rect.right, ARect.bottom);
					HBRUSH hBBrush = ::CreateSolidBrush(RGB(216,231,252));
					::FillRect(hdc,&bRect,hBBrush);
					::DeleteObject(hBBrush);
					break;
				}
				else
				{
					break;
				}
				LineX = ARect.left ;
			}
			LineY +=  m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow) + 1;
		}
		if(nEDrawCol > 0) rect.right = LineX;

		hFont = (HFONT)::SelectObject(hdc, hFont);
		::DeleteObject(hFont);
		::DeleteObject(hFontB);
		::SetBkMode(hdc,hbr);

		if(pStyle)	delete pStyle;
	}
	void XGridHeaderEx::OnFDraw(HDC hdc,RECT &rect)
	{
		if(m_nFECol < 1) return;

		CAxisSvr * pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;//m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return;

		OnBaseDraw(hdc, rect, m_nFSCol, m_nFECol);

		rect.right = rect.right - pAxisSvr->GetColWidth(0);
	}
	int XGridHeaderEx::GetFColumnWidth()
	{
		if(m_nFECol < 1) return 0;

		CAxisSvr * pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;//m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return 0;
		
		int ret = 0;
		for(int i=m_nFSCol; i<=m_nFECol; i++)
		{
			ret += pAxisSvr->GetColWidth(i) + 1;
		}
		return ret;
	}
	void XGridHeaderEx::OnDraw(HDC hdc,RECT &rect)
	{
		if(!m_pDataTempl) return ;
		if(!m_pGridReportSvr) return;

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;
		
		if(!pReportBand->Title.bValid) return;
		CAxisSvr * pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;//m_pDataTempl->m_pAxisSvr;
		if(!pAxisSvr) return;

		RECT fr;
		::SetRect(&fr,rect.left,rect.top,rect.right,rect.bottom);
		OnFDraw(hdc, fr);
		if(fr.right != rect.right) rect.left = fr.right;
/*
		int s = m_pGridReportSvr->GetColCount();
		int j = 0;
		int i = 1;
		for(i=1; i<=s; i++)
		{
			int width = m_pDataTempl->m_pAxisSvr->GetColWidth(i);
			if(width <= 1) continue;
			j++;
			if(j == m_pGridReportSvr->FTopLeft.x) break;
		}

		OnBaseDraw(hdc, rect, i);
*/
		OnBaseDraw(hdc, rect, m_pGridReportSvr->FTopLeft.x);
		if(fr.right != rect.right) rect.left = fr.left;
		
		return;
#if 0
		int startRow,endRow;
		startRow = pReportBand->Title.startRow;
		endRow = pReportBand->Title.endRow - 1;


		LOGFONT	logFont;
		logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
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
		HFONT	hFontB =::CreateFontIndirect(&logFont);
		int hbr;
		hbr = ::SetBkMode(hdc,TRANSPARENT);

		int width;
		width = rect.right- rect.left;

		int StartCol = m_pGridReportSvr->FTopLeft.x;
		int StartRow = m_pGridReportSvr->FTopLeft.y;

		int startCell = m_pGridReportSvr->FTopLeft.x;
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

		//LPTSTR pCssStr = _tcsdup(_T("background-color:#ebead8 none #ffffff none"));
		LPTSTR pCssStr = _tcsdup(_T("background-color:#D8E7FC"));
		XUtil::Css::TBoxStyle * pStyle = XUtil::xuCCssStyle::ParseCssStyle(pCssStr);
		delete pCssStr;

		/*
		if(pStyle)
		{
			RECT rc = rect;
			rc.left =  rect.left + pAxisSvr->GetColWidth(0) + 1;
			rc.bottom = rect.top + this->GetTitleHeight() - 1;
			XUtil::Css::DrawCssFrame(hdc,&rc,pStyle);

			HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(0xce,0xce,0xce));
			hPen = (HPEN)::SelectObject(hdc,hPen);

			::MoveToEx(hdc,rc.left,rc.bottom + 1,NULL);
			::LineTo(hdc,rc.right,rc.bottom + 1);
			hPen = (HPEN)::SelectObject(hdc,hPen);
			::DeleteObject(hPen);
		}*/
		
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
					TCHAR * pText = NULL;
					TGridRect gRect;
					bDraw = true;
					PStrItem	pItem = m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow);
					if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
					{
						if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
						{
							if(pItem && pItem->pStrContent) pText = pItem->pStrContent;
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
						if(pItem && pItem->pStrContent) pText = pItem->pStrContent;
					}

					if(bDraw) XUtil::Css::DrawCssFrame(hdc,&ARect,pStyle);

					/*
					//draw background
					COLORREF color;
					if(ACol >=Selection.left && ACol<=Selection.right)
						color= RGB(0xc8,0xd7,0xec);
					else
						color= RGB(0xd8,0xe7,0xfc);

					TRIVERTEX	vertex[2];
					GRADIENT_RECT grect[1];

					vertex[0].x = ARect.left;
					vertex[0].y = ARect.top + 1;
					vertex[0].Red  =  GetRValue(color)*256;
					vertex[0].Green = GetGValue(color)*256;
					vertex[0].Blue =  GetBValue(color)*256;

					vertex[1].x = ARect.right;
					vertex[1].y = ARect.bottom + 1 ;
					vertex[1].Red  =  GetRValue(color)*256;
					vertex[1].Green = GetGValue(color)*256;
					vertex[1].Blue =  GetBValue(color)*256;

					grect[0].UpperLeft = 0;
					grect[0].LowerRight = 1;

					if(bDraw)GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);*/

					//if(pText)::DrawText(hdc,pText,(int)::_tcslen(pText),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					if(pText)
					{
						if(_tcschr(pText,'\r'))
							::DrawTextW(hdc,pText,(int)::_tcslen(pText),&ARect,DT_CENTER|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
						else
						{
							SIZE sz;
							GetTextExtentPoint32(hdc,pText,(int)::_tcslen(pText),&sz);
							if(sz.cx < ARect.right - ARect.left)
								::DrawText(hdc,pText,(int)::_tcslen(pText),&ARect,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_VCENTER|DT_EDITCONTROL|DT_NOPREFIX);
							else
								::DrawTextW(hdc,pText,(int)::_tcslen(pText),&ARect,DT_CENTER|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
							}
					}
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
				}else
				{
					RECT bRect;
					::SetRect(&bRect,ARect.right+1, ARect.top, rect.right, ARect.bottom);
					HBRUSH hBBrush = ::CreateSolidBrush(RGB(216,231,252));
					::FillRect(hdc,&bRect,hBBrush);
					::DeleteObject(hBBrush);
					break;
				}
				LineX = ARect.left ;
			}
			if(ARow == startRow && LineX < rect.right)
			{
/*				ARect.bottom = rect.top + this->GetTitleHeight()  - 1;
				ARect.right = rect.right - 2;

				//draw background
				COLORREF color;
				color= RGB(0xd8,0xe7,0xfc);

				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = ARect.left;
				vertex[0].y = ARect.top + 1;
				vertex[0].Red  =  GetRValue(color)*256;
				vertex[0].Green = GetGValue(color)*256;
				vertex[0].Blue =  GetBValue(color)*256;

				vertex[1].x = ARect.right;
				vertex[1].y = ARect.bottom + 1 ;
				vertex[1].Red  =  GetRValue(color)*256;
				vertex[1].Green = GetGValue(color)*256;
				vertex[1].Blue =  GetBValue(color)*256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;
*/
				ARect.bottom = rect.top + this->GetTitleHeight();
				ARect.right = rect.right;

				//draw background
				COLORREF color;
				color= RGB(0xd8,0xe7,0xfc);

				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = ARect.left;
				vertex[0].y = ARect.top;
				vertex[0].Red  =  GetRValue(color)*256;
				vertex[0].Green = GetGValue(color)*256;
				vertex[0].Blue =  GetBValue(color)*256;

				vertex[1].x = ARect.right;
				vertex[1].y = ARect.bottom + 1;
				vertex[1].Red  =  GetRValue(color)*256;
				vertex[1].Green = GetGValue(color)*256;
				vertex[1].Blue =  GetBValue(color)*256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;

				GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
			}
			LineY +=  m_pDataTempl->m_pAxisSvr->GetRowHeight(ARow) + 1;
		}

		hFont = (HFONT)::SelectObject(hdc, hFont);
		::DeleteObject(hFont);
		::DeleteObject(hFontB);
		::SetBkMode(hdc,hbr);

		if(pStyle)	delete pStyle;

		if(fr.right != rect.right) rect.left = fr.left;
#endif
	}
	bool XGridHeaderEx::SetEventListons()
	{
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XGridHeaderEx::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XGridHeaderEx::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XGridHeaderEx::EvtMouseMove);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XGridHeaderEx::EvtSetCursor);
		this->AttachListon(WM_RBUTTONDOWN,(FNEvent)&XGridHeaderEx::EvtRButtonDown);

		return true;
	}

	bool XGridHeaderEx::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool XGridHeaderEx::EvtRButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		POINT pt = pEvent->pt;
		POINT point = pEvent->mouse.point;
		int ACol;
		int nPos;
		CalcSizingState(point,FRect,ACol,nPos);

		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;				
		if(!pReportBand->Title.bValid) return true;
		//ARow一行标题的情况，多行标题现不适用
		int ARow = pReportBand->Title.startRow;

		TGridRect re;

		//calc row
		int ypos = point.y - FRect.top;
		while(true)
		{
			m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,re);
			bool bFound = false;
			for(int i=ARow; i<=re.bottom;i++)
			{
				if(ypos < m_pDataTempl->m_pAxisSvr->GetRowHeight(i))
				{
					bFound = true;
					break;
				}
				ypos -= m_pDataTempl->m_pAxisSvr->GetRowHeight(i) + 1;
			}
			if(bFound)break;
			if(re.bottom + 1 < pReportBand->Title.endRow)
			{
				ARow =  re.bottom + 1;
			}else
				break;
		}
		if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,re)) ACol = re.right;


		HMENU M = CreatePopupMenu();
		AppendMenu(M,MF_STRING, 0x101, L"顺序");
		AppendMenu(M,MF_STRING, 0x102, L"倒序");
		AppendMenu(M,MF_STRING, 0x0, NULL);
		AppendMenu(M,MF_STRING, 0x3, L"隐藏");
		HMENU M2 = CreatePopupMenu();
		int s = m_pGridReportSvr->GetColumnCount();
		for(int i=1; i<=s; i++)
		{
			TGridRect gRect;
			if(m_pDataTempl->m_pSpanSvr->isSpaned(i,ARow,gRect))
				if(i > gRect.left && i <=gRect.right) continue;

			LPTSTR Name = (LPTSTR)m_pGridReportSvr->GetTitleName(i);
			if(!Name || _tcscmp(Name, L"") == 0) continue;
			int width = m_pGridReportSvr->GetColumnWidth(i);
			if(width < 1)
				AppendMenu(M2,MF_STRING, 0x200+i, Name);
		}
		AppendMenu(M,MF_POPUP, (UINT_PTR)M2, L"显示");
		AppendMenu(M,MF_STRING, 0x0, NULL);
		AppendMenu(M,MF_STRING, 0x1, L"冻结");
		AppendMenu(M,MF_STRING, 0x2, L"取消冻结");
		AppendMenu(M,MF_STRING, 0x0, NULL);
		AppendMenu(M,MF_STRING, 0x4, L"筛选");

		RECT xr;
		int ret = TrackPopupMenu(M, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, GetHWND(), &xr);
		if(ret == 0x101)
		{
			if(ACol < 1 || ACol > m_pGridReportSvr->GetColumnCount()) return true;
			//2017.8.23 m_pGridReportSvr->SetRowSort((LPTSTR)m_pGridReportSvr->GetColumnName(ACol+1), true);
			m_pGridReportSvr->SetRowSort((LPTSTR)m_pGridReportSvr->GetColumnName(ACol), true);
		}
		else if(ret == 0x102)
		{
			if(ACol < 1 || ACol > m_pGridReportSvr->GetColumnCount()) return true;
			//2017.8.23 m_pGridReportSvr->SetRowSort((LPTSTR)m_pGridReportSvr->GetColumnName(ACol+1), false);
			m_pGridReportSvr->SetRowSort((LPTSTR)m_pGridReportSvr->GetColumnName(ACol), false);
		}
		else if(ret == 0x3)
		{
			if(ACol < 1 || ACol > m_pGridReportSvr->GetColumnCount()) return true;
			int i;
			for(i=re.left; i<=re.right; i++)
				m_pGridReportSvr->SetColumnState(i, false);
			::InvalidateRect(m_pGridReportSvr->GetHWND(),NULL,false);
		}
		else if(ret > 0x200 && ret < 0x300)
		{
			ACol = ret - 0x200;
			TGridRect gRect;
			if(m_pDataTempl->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
			{
				int i;
				for(i=gRect.left; i<=gRect.right; i++)
					m_pGridReportSvr->SetColumnState(i, true);				
			}
			else
				m_pGridReportSvr->SetColumnState(ACol, true);
			::InvalidateRect(m_pGridReportSvr->GetHWND(),NULL,false);
		}
		else if(ret == 0x1)
		{
			if(ACol < 1 || ACol > m_pGridReportSvr->GetColumnCount()) return true;
			m_nFSCol = m_pGridReportSvr->FTopLeft.x;
			m_nFECol = ACol;
			m_pGridReportSvr->FTopLeft.x = ACol + 1;
			::InvalidateRect(m_pGridReportSvr->GetHWND(),NULL,false);
		}
		else if(ret == 0x2)
		{
			m_nFSCol = -1;
			m_nFECol = -1;
			::InvalidateRect(m_pGridReportSvr->GetHWND(),NULL,false);
		}
		else if(ret == 0x4)
		{
			CloseColQueryDlg();
			if(m_bColQuery)
			{
				m_bColQuery = false;
				if(m_pGridReportSvr->m_pSheetSvr->m_pSheetCtrl->hasBindData())
						m_pGridReportSvr->m_pSheetSvr->m_pSheetCtrl->bindGetData(m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, (wchar_t*)((std::wstring)L"filter:,").c_str());
				else
					m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->ColumnFilter(NULL, NULL);
			}
			else
				m_bColQuery = true;
			::InvalidateRect(m_pGridReportSvr->GetHWND(),NULL,false);
		}
		return true;
	}
	bool XGridHeaderEx::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		int ACol;
		int nPos;

		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);
		TGridState State = CalcSizingState(point,FRect,ACol,nPos);
		if(State == gsColSizing)
		{
			HCURSOR hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_ARROWWE));
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}		
		else if(FState == gsColMoving)
		{
			HCURSOR hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_MOVE));
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}

		return true;
	}

	bool XGridHeaderEx::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		if(!m_pDataTempl) return true;
		if(!m_pGridReportSvr) return true;

		int ACol;
		int nPos;
		POINT point = pEvent->mouse.point;
		FState = CalcSizingState(point,FRect,ACol,nPos);
		CloseColQueryDlg();
		if(FState != gsColSizing && m_bColQuery && nPos - point.x <=16)
		{
			FState = gsNormal;
			ShowColQueryDlg(ACol);
			return true;
		}
		if(FState == gsColSizing || FState == gsColMoving)
		{
			m_nSCol = ACol;
			m_nSPos = nPos;
			if(FState == gsColMoving) point.x = -1;
			DrawSizingLine(point);
			this->m_ptMouse = point;
			this->SetCapture();
			if(FState == gsColSizing) return true;
		}

		return true;
	}

	bool XGridHeaderEx::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(FState == gsColSizing || FState == gsColMoving)
		{
			DrawSizingLine(m_ptMouse);
			DrawSizingLine(point);
		}
		this->m_ptMouse = point;
		return true;
	}

	void XGridHeaderEx::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		if(FState == gsColSizing || FState == gsColMoving)
		{
			::MoveToEx(hDC,pt.x, 0,NULL);
			::LineTo(hDC,pt.x,m_pGridReportSvr->FRect.bottom );
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	TGridState XGridHeaderEx::CalcSizingState(POINT point,RECT &rect)
	{
		int ACol;
		int nPos;
		return CalcSizingState(point,rect,ACol,nPos);
	}

	TGridState XGridHeaderEx::CalcSizingState(POINT point,RECT &rect,int &ACol, int &nPos)
	{
		if(!m_pDataTempl) return gsNormal;
		if(!m_pGridReportSvr) return gsNormal;
		CAxisSvr * pAxisSvr;
		/*
		if( m_pDataTempl)
			pAxisSvr = m_pDataTempl->m_pAxisSvr;//m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;
		else
			pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;
		*/
		pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return gsNormal;

		int startCell = m_pGridReportSvr->FTopLeft.x;
		int LineX;
		int EndX;

		LineX = rect.left + 1;
		EndX = rect.right;

		if(point.y < rect.top || point.y > rect.bottom) return gsNormal;
		if(point.x < LineX || point.x > EndX) return gsNormal;

		int fwidth = GetFColumnWidth();
		if(fwidth >= point.x) return gsNormal;
		LineX += fwidth;

		ACol = startCell;
		while(LineX <= EndX)
		{
			int Count;
			int RCount;
			m_pDataTempl->m_pContentSvr->GetValidRowCol(Count,RCount);

			int i=0;
			if(ACol <= Count)
			{
				if(pAxisSvr->GetColWidth(ACol) <= 1)
				{
					ACol++;
					continue;
				}
				LineX += pAxisSvr->GetColWidth(ACol) + 1;
				if(abs(LineX - point.x)<=3)
				{
					nPos = LineX +  1;
					return gsColSizing;
				}
				else if(LineX > point.x)
				{
					nPos = LineX +  1;
					return gsColMoving;
				}
				ACol++;
				if(LineX > point.x) return gsNormal;
			}else
				return gsNormal;
		}
	
		return gsNormal;
	}

	bool XGridHeaderEx::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}

	HWND XGridHeaderEx::GetHWND()
	{
		return this->m_pGridReportSvr->GetHWND();
	}

	struct  StringLessEx
	{
		bool operator()(const wstring & lhs, const wstring &rhs) const
		{	
			return _tcsicoll(lhs.c_str(), rhs.c_str())<0?true:false;
		}
	};

	struct  DoubleLessEx
	{
		bool operator()(const wstring & lhs, const wstring &rhs) const
		{	
			return wcstod(lhs.c_str(),0) < wcstod(rhs.c_str(),0);
		}
	};

	struct  IntLessEx
	{
		bool operator()(const wstring & lhs, const wstring &rhs) const
		{	
			return wcstol(lhs.c_str(),0,10) < wcstol(rhs.c_str(),0,10);
		}
	};

	int XGridHeaderEx::ShowColQueryDlg(int col)
	{
		CReportBandSvr::TReportBand *  pReportBand = &m_pDataTempl->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Title.bValid) return -1;
		CloseColQueryDlg();
		//单行
		int height = m_pDataTempl->m_pAxisSvr->GetRowHeight(pReportBand->Title.startRow);
		RECT rect;
		GetClientRect(&rect);
		int LineX = rect.left + 1;
		int EndX = rect.right;		
		int ACol = m_pGridReportSvr->FTopLeft.x;
		while(LineX <= EndX)
		{
			if(ACol < col)
			{
				if(m_pDataTempl->m_pAxisSvr->GetColWidth(ACol) <= 1)
				{
					ACol++;
					continue;
				}
				LineX += m_pDataTempl->m_pAxisSvr->GetColWidth(ACol) + 1;
				ACol++;
			}
			else
				break;
		}
		//TGridRect gRect;
		//m_pGridReportSvr->m_pDataTempl->m_pSpanSvr->isSpaned(col + 1,pReportBand->Header.startRow,gRect);
		//if(gRect.left < col) col = gRect.left;
		
		int h = 250;
		RECT rc;
		::SetRect(&rc,LineX + GetFColumnWidth(),height,LineX+m_pDataTempl->m_pAxisSvr->GetColWidth(col) + GetFColumnWidth(),height+h);
		
		CGridHeaderListBoxQuery * pEdit = new CGridHeaderListBoxQuery();
		pEdit->m_dwStyle = WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL;
		pEdit->m_pszWinClass =_tcsdup(_T("listbox"));
		pEdit->CreateControl(NULL,rc,(HWND)GetHWND(),0,xfApp::GetApp()->GetInstance());
		LOGFONT	logFont;
		XUtil::Css::GetDefaultFont(logFont);
		HFONT	hFont =::CreateFontIndirect(&logFont);
		::SendMessage(pEdit->GetHWND(),WM_SETFONT,(WPARAM)hFont,false);	

		::SendMessage(pEdit->GetHWND(), LB_ADDSTRING, NULL, (WPARAM)L"(全部)");
		::SendMessage(pEdit->GetHWND(), LB_ADDSTRING, NULL, (WPARAM)L"(自定义...)");
		int counts = m_pGridReportSvr->GetRowCount();

		LPTSTR celltype = m_pGridReportSvr->GetColumnProp(col,L"celltype");
		if(celltype && _tcscmp(celltype,L"xs:decimal")==0)
		{
			std::set<wstring,DoubleLessEx> values;
			for(int i=1;i<=counts; i++)
			{
				//2018.8.23 LPTSTR pText = (LPTSTR)m_pGridReportSvr->GetItemDisplayString(i, col + 1);
				LPTSTR pText = (LPTSTR)m_pGridReportSvr->GetItemDisplayString(i, col);
				//2018.8.23 if(!pText) pText = (LPTSTR)m_pGridReportSvr->GetItemString(i, col + 1);
				if(!pText) pText = (LPTSTR)m_pGridReportSvr->GetItemString(i, col);

				if(pText && _tcscmp(pText, L"") != 0) values.insert(pText);
			}
			for(std::set<wstring,DoubleLessEx>::const_iterator tr=values.begin();tr!=values.end();tr++)
			{
				::SendMessage(pEdit->GetHWND(), LB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
			}
		}
		else if(celltype && _tcscmp(celltype,L"xs:integer")==0)
		{
			std::set<wstring,IntLessEx> values;
			for(int i=1;i<=counts; i++)
			{
				//2018.8.23 LPTSTR pText = (LPTSTR)m_pGridReportSvr->GetItemDisplayString(i, col + 1);
				LPTSTR pText = (LPTSTR)m_pGridReportSvr->GetItemDisplayString(i, col);
				//2018.8.23 if(!pText) pText = (LPTSTR)m_pGridReportSvr->GetItemString(i, col + 1);
				if(!pText) pText = (LPTSTR)m_pGridReportSvr->GetItemString(i, col);
				if(pText && _tcscmp(pText, L"") != 0) values.insert(pText);
			}
			for(std::set<wstring,IntLessEx>::const_iterator tr=values.begin();tr!=values.end();tr++)
			{
				::SendMessage(pEdit->GetHWND(), LB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
			}
		}else
		{
			std::set<wstring,StringLessEx> values;
			for(int i=1;i<=counts; i++)
			{
				//2018.8.23 LPTSTR pText = (LPTSTR)m_pGridReportSvr->GetItemDisplayString(i, col + 1);
				LPTSTR pText = (LPTSTR)m_pGridReportSvr->GetItemDisplayString(i, col);
				//2018.8.23 if(!pText) pText = (LPTSTR)m_pGridReportSvr->GetItemString(i, col + 1);
				if(!pText) pText = (LPTSTR)m_pGridReportSvr->GetItemString(i, col);
				if(pText && _tcscmp(pText, L"") != 0) values.insert(pText);
			}
			for(std::set<wstring,StringLessEx>::const_iterator tr=values.begin();tr!=values.end();tr++)
			{
				::SendMessage(pEdit->GetHWND(), LB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
			}
		}
		::SendMessage(pEdit->GetHWND(), LB_ADDSTRING, NULL, (WPARAM)L"(空白)");
		::SendMessage(pEdit->GetHWND(), LB_ADDSTRING, NULL, (WPARAM)L"(非空白)");
		::SetFocus(pEdit->GetHWND());
		m_nQueryColIndex = col;
		pEdit->pGridHeaderEx = this;
		m_pColumnQueryList = pEdit;
		m_pColumnQueryList->AttachProcEdit();

		return 1;
	}

	int XGridHeaderEx::CloseColQueryDlg()
	{
		if(m_pColumnQueryList && m_pColumnQueryList->GetHWND() && ::IsWindow(m_pColumnQueryList->GetHWND()))
		{
			m_pColumnQueryList->DettachProcEdit();
			//::DestroyWindow(m_pColumnQueryList->GetHWND());
			::PostMessage(m_pColumnQueryList->GetHWND(), WM_CLOSE, NULL, NULL);
		}
		return 1;
	}

	int XGridHeaderEx::ColumnQuery(LPTSTR pStr)
	{
		//2017.8.23 LPTSTR pColumn = (LPTSTR)m_pGridReportSvr->GetColumnName(m_nQueryColIndex + 1);
		LPTSTR pColumn = (LPTSTR)m_pGridReportSvr->GetColumnName(m_nQueryColIndex);
		return ColumnQuery(pColumn, pStr);
	}

	int XGridHeaderEx::ColumnQuery(LPTSTR pColumn, LPTSTR pStr)
	{
		if(!pStr) pStr = L"";
		LPTSTR celltype = m_pGridReportSvr->GetColumnProp(m_pGridReportSvr->GetColumnIndex(pColumn),L"celltype");

		CloseColQueryDlg(); 
		m_pGridReportSvr->HideEdit();

		LPTSTR pText = new TCHAR[1024]; 
		if(_tcscmp(pStr, L"(全部)") == 0)
			pText = NULL;
		else if(_tcscmp(pStr, L"(自定义...)") == 0)
		{
			XGridFilterDlg  * pWin = new XGridFilterDlg();
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
			xml.m_pXmlDoc->selectSingleNode(L"/xpages/xpage[@name='filterwin']/xframe",&pNode);
			if(!pNode) return 1;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
			pXPage->Load(pElement);
			pElement->Release();

			pWin->Open(pXPage,GetHWND(),NULL,(LPVOID)this);

			return 1;
		}
		else if(_tcscmp(pStr, L"(空白)") == 0)
			_stprintf_s(pText,1024,_T("(count(%s)=0 or (count(%s)=1 and %s=''))") ,pColumn, pColumn, pColumn);
		else if(_tcscmp(pStr, L"(非空白)") == 0)
			_stprintf_s(pText,1024,_T("(%s != '')") ,pColumn);
		else if(_tcscmp(pStr, L"") != 0)
		{
			if(celltype && _tcscmp(celltype,L"xs:decimal")==0)
				_stprintf_s(pText,1024,_T("%s - %s <= 0.01 and %s - %s >= - 0.01") ,pColumn, pStr,pColumn, pStr);
			else if(celltype && _tcscmp(celltype,L"xs:date")==0)
				_stprintf_s(pText,1024,_T("starts-with(normalize-space(%s),normalize-space('%s'))") ,pColumn, pStr);
			else
				_stprintf_s(pText,1024,_T("(normalize-space(%s)=normalize-space('%s') or (count(%s/@_displaystring)=1 and normalize-space(%s/@_displaystring)=normalize-space('%s')))") ,pColumn, pStr, pColumn, pColumn, pStr);
		}
		else
			pText = NULL;

		if(m_pGridReportSvr->m_pSheetSvr->m_pSheetCtrl->hasBindData())
			m_pGridReportSvr->m_pSheetSvr->m_pSheetCtrl->bindGetData(m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, (wchar_t*)((std::wstring)L"filter:"+ pColumn+L","+pStr).c_str());
		else
		{
			if(pColumn && pColumn[0]=='*' && pColumn[1]==0)
			{
				m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->ColumnFilter(pColumn, pStr);
			}else
				m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->ColumnFilter(pColumn, pText);
		}
		::InvalidateRect(GetHWND(),NULL,false);
		return 1;
	}

	LRESULT CALLBACK CGridHeaderListBoxQuery::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_LBUTTONDBLCLK: //LBN_DBLCLK:
				{
					int nIndex = ::SendMessage(hwnd, LB_GETCURSEL, NULL,NULL); //LOWORD(wParam);
					if(nIndex > -1)
					{
						int nLen = ::SendMessage(hwnd, LB_GETTEXTLEN, nIndex, NULL);
						LPTSTR  pText = new TCHAR[nLen + 1];
						::SendMessage(hwnd, LB_GETTEXT, nIndex, (LPARAM)pText);
						CGridHeaderListBoxQuery * pThis = ((CGridHeaderListBoxQuery *)GetProp(hwnd,L"thisex"));
						pThis->pGridHeaderEx->ColumnQuery(pText);
						return NULL;
					}
					break;
				}
			case WM_KILLFOCUS:
				{
					CGridHeaderListBoxQuery * pThis = ((CGridHeaderListBoxQuery *)GetProp(hwnd,L"thisex"));
					pThis->pGridHeaderEx->CloseColQueryDlg();  
					break;
				}
		}
		return CallWindowProc(((CGridHeaderListBoxQuery *)GetProp(hwnd,L"thisex"))->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
	
	}

	int CGridHeaderListBoxQuery::AttachProcEdit()
	{				
		m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(GetHWND(),GWLP_WNDPROC);
		SetWindowLongPtr(GetHWND(),GWLP_WNDPROC,(long)&CGridHeaderListBoxQuery::WindowProc);
		SetProp(GetHWND(),L"thisex",(HANDLE)this);
		return 1;
	}

	int CGridHeaderListBoxQuery::DettachProcEdit()
	{
		SetWindowLongPtr(GetHWND(),GWLP_WNDPROC,(long)m_PrevWndFunc);
		return 1;
	}

}}}}
