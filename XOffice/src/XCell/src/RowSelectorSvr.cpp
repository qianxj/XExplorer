#include "stdafx.h"
#include "xofbase.h"
#include "RowSelectorsvr.hpp"
#include "axissvr.hpp"
#include "xgroupdlg.hpp"
#include "XCellSheetSvr.hpp"

extern HMODULE hCellModula;
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CRowSelectorSvr::CRowSelectorSvr(void):m_pGridFrameSvr(NULL),m_fold(0)
	{
		::SetRect(&FRect,0,0,0,0);
		m_ptMouse.x=-1;
		m_ptMouse.y=-1;
		SetEventListons();
	}
	CRowSelectorSvr::~CRowSelectorSvr(void)
	{
		UnSetEventListons();
	}

	bool CRowSelectorSvr::SetEventListons()
	{
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&CRowSelectorSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&CRowSelectorSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&CRowSelectorSvr::EvtMouseMove);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&CRowSelectorSvr::EvtSetCursor);
		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&CRowSelectorSvr::EvtLDoubleClick);

		return true;
	}

	bool CRowSelectorSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool CRowSelectorSvr::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;
		POINT point = pEvent->mouse.point;
		m_pGridFrameSvr->FGridState = CalcSizingState(point,FRect,ARow,nPos);
		if(m_pGridFrameSvr->FGridState == gsRowSizing)
		{
			m_nSRow = ARow;
			m_nSPos = nPos;
			DrawSizingLine(point);
			this->SetCapture();
		}
		return true;
	}

	bool CRowSelectorSvr::EvtLDoubleClick(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		POINT point = pEvent->mouse.point;
		ARow = m_pGridFrameSvr->CalcRowFromPoint(point.x,point.y);

		XGroupDlg * pWin = new XGroupDlg();
		xfXPage* pPage = new xfXPage();

		PStrItem pItem;
		pItem = m_pGridFrameSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(0,ARow);
		if(pItem)
		{
			pWin->m_pPreStr = pItem->pStrPre;
			pWin->m_pPreLastStr = pItem->pStrPreLast;
		}

		TCHAR path[MAX_PATH];
		::GetModuleFileName(NULL,path,MAX_PATH);
		::PathRemoveFileSpec(path);
		TCHAR buf[MAX_PATH];
		::_stprintf_s(buf,MAX_PATH,_T("%s\\agent\\xcell\\groupdlg.xml"),path);
		pPage->Load(buf); 
		pWin->Open(pPage,m_pGridFrameSvr->m_pSheetSvr->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_hWnd);
		if(pWin->m_bOk)
		{
			if(!pItem)
			{
				pItem  = new StrItem();
				m_pGridFrameSvr->GetGridDataSvr()->m_pContentSvr->SetItemData(0,ARow,pItem);
			}
			if(!(pWin->m_pPreStr && pWin->m_pNewPreStr  && ::_tcsicmp(pWin->m_pPreStr,pWin->m_pNewPreStr)==0))
			{
				if(pItem->pStrPre)delete pItem->pStrPre;
				pItem->pStrPre = NULL;
				if(pWin->m_pNewPreStr) pItem->pStrPre = ::_tcsdup(pWin->m_pNewPreStr);
			}

			if(!(pWin->m_pPreLastStr && pWin->m_pNewPreLastStr && ::_tcsicmp(pWin->m_pPreLastStr,pWin->m_pNewPreLastStr)==0))
			{
				if(pItem->pStrPreLast)delete pItem->pStrPreLast;
				pItem->pStrPreLast = NULL;
				if(pWin->m_pNewPreLastStr) pItem->pStrPreLast = ::_tcsdup(pWin->m_pNewPreLastStr);
			}

			//this->InvalidateCell(CellHit.x,CellHit.y);
			//m_pGridData->ResetTemplData();
			//m_pGridData->AddTemplData(m_pGridData,false);
			//m_pGridData->CalcRptBands();
			m_pGridFrameSvr->InvalidateGrid();	
		}
		delete pWin;
		return true;
	}

	bool CRowSelectorSvr::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(m_pGridFrameSvr->FGridState == gsRowSizing)
		{
			DrawSizingLine(point);
			m_pGridFrameSvr->FGridState = gsNormal;
			int height = m_pGridFrameSvr->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(m_nSRow);
			height += point.y - m_nSPos;
			if(height > 0) 
				m_pGridFrameSvr->GetGridDataSvr()->m_pAxisSvr->SetRowHeight(m_nSRow, height);
			this->ReleaseCapture();
			::InvalidateRect(GetHWND(),NULL,FALSE);
		}
		return true;
	}

	bool CRowSelectorSvr::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(m_pGridFrameSvr->FGridState == gsRowSizing)
		{
			DrawSizingLine(m_ptMouse);
			DrawSizingLine(point);
		}
		this->m_ptMouse = point;
		return true;
	}

	bool CRowSelectorSvr::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;

		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);
		if(m_pGridFrameSvr->FGridState != gsDrawLining && m_pGridFrameSvr->FGridState != gsDeleteLining)
		{
			m_pGridFrameSvr->FGridState = CalcSizingState(point,FRect,ARow,nPos);
			if(m_pGridFrameSvr->FGridState == gsRowSizing)
			{
				HCURSOR hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_ARROWNS));
				hCursor = ::SetCursor(hCursor);
				pEvent->dwRet = 1;
			}
			m_pGridFrameSvr->FGridState = gsNormal;
		}
		return true;
	}

	void CRowSelectorSvr::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		if(m_pGridFrameSvr->FGridState == gsRowSizing)
		{
			::MoveToEx(hDC,0, pt.y,NULL);
			::LineTo(hDC,m_pGridFrameSvr->FRect.right , pt.y);
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	void CRowSelectorSvr::OnDraw(HDC hdc,RECT &rect)
	{
		if(!m_pGridFrameSvr) return;
		CAxisSvr * pAxisSvr = m_pGridFrameSvr->GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return;
		if(pAxisSvr->GetColWidth(0) < 2)return;

		int width,height;
		width = pAxisSvr->GetColWidth(0) - m_fold;
		height = rect.bottom - rect.top;

		int startCell = 0;
		int LineY = 0;
		int ARow= m_pGridFrameSvr->FTopLeft.y;

		RECT	ARect;
		TCHAR	values[40];
		ARect.top = rect.top + pAxisSvr->GetRowHeight(0) + 1;
		ARect.left = rect.left;
		ARect.right = rect.left + pAxisSvr->GetColWidth(0) - m_fold;
		LineY =  ARect.top;

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

		TGridRect Selection = m_pGridFrameSvr->GetSelection();
		int j = 0;
		while(LineY <= rect.bottom )
		{
			int i=0;
			if(ARow < m_pGridFrameSvr->GetRowCount())
			{
				if(pAxisSvr->GetRowHeight(ARow) < 1) 
				{
					ARow++;
					j++;
					continue;
				}
				ARect.bottom = ARect.top + pAxisSvr->GetRowHeight(ARow);
				wsprintf(values,_T("%d\0"),ARow-j);

				//draw background
				COLORREF color;
				if(ARow >=Selection.top && ARow<=Selection.bottom)
					color= RGB(0xc8,0xd7,0xec);//RGB(0xcc,0xd8,0xcc);
				else
				{
					if(m_pGridFrameSvr->GetGridDataSvr()->GetChildIndex(ARow)>=0)
						color= RGB(0xd0,0xdf,0xf2);
					else
						color= RGB(0xd8,0xe7,0xfc);//RGB(0xcc,0xd8,0xcc);
				}

				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = rect.left;
				vertex[0].y = ARect.top ;
				vertex[0].Red  =  GetRValue(color)*256;
				vertex[0].Green = GetGValue(color)*256;
				vertex[0].Blue =  GetBValue(color)*256;
				//vertex[0].Red  = (GetRValue(color)>25? GetRValue(color) - 5:0)*256;
				//vertex[0].Green = (GetGValue(color)>25? GetGValue(color) - 5:0)*256;
				//vertex[0].Blue = (GetBValue(color)>25? GetBValue(color) - 5:0)*256;

				vertex[1].x = rect.left + width;
				vertex[1].y = ARect.bottom;
				vertex[1].Red  =  GetRValue(color)*256;
				vertex[1].Green = GetGValue(color)*256;
				vertex[1].Blue =  GetBValue(color)*256;
				//vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
				//vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
				//vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;

				GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
				
				if(ARect.right - ARect.left > 12)
				{
					if(ARow >=Selection.top && ARow<=Selection.bottom)
					{
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
						::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
					}
					else
					{
						::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
					}
				}

				PStrItem	pItem;
				pItem = m_pGridFrameSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(0,ARow);
				if(pItem && (pItem->pStrPre|| pItem->pStrPreLast))
				{
					HBRUSH hbrush = ::CreateSolidBrush(RGB(128,0,0));
					hbrush = (HBRUSH)::SelectObject(hdc,hbrush);
					PatBlt(hdc,ARect.right - 4,ARect.top+1,4,4,PATCOPY);
					hbrush = (HBRUSH)::SelectObject(hdc,hbrush);
					::DeleteObject(hbrush);
				}

					
				ARow++;
				ARect.top = ARect.bottom + 1;
			}else
				break;
			LineY = ARect.top ;
		}
		hFont = (HFONT)::SelectObject(hdc, hFont);
		::DeleteObject(hFont);
		::DeleteObject(hFontB);
		::SetBkMode(hdc,hbr);
	}
	TGridState CRowSelectorSvr::CalcSizingState(POINT point,RECT &rect)
	{
		int ARow;
		int nPos;
		return CalcSizingState(point,rect,ARow,nPos);
	}

	TGridState CRowSelectorSvr::CalcSizingState(POINT point,RECT &rect,int &ARow, int &nPos)
	{
		if(!m_pGridFrameSvr) return gsNormal;
		CAxisSvr * pAxisSvr = m_pGridFrameSvr->GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return gsNormal;

		int startCell = m_pGridFrameSvr->FTopLeft.y;
		int LineY;
		int EndY;

		LineY = rect.top  + 1;
		EndY = rect.bottom;

		if(point.x < rect.left || point.x > rect.right) return gsNormal;
		if(point.y < LineY || point.y > EndY) return gsNormal;

		ARow = startCell;
		while(LineY <= EndY )
		{
			int i=0;
			if(ARow < m_pGridFrameSvr->GetRowCount())
			{
				if(pAxisSvr->GetRowHeight(ARow)<1)
				{
					ARow++;
					continue;
				}
				LineY += pAxisSvr->GetRowHeight(ARow) + 1;
				if(abs(LineY - point.y)<=3)
				{
					nPos = LineY +  1;
					return gsRowSizing;
				}
				ARow++;
				if(LineY > point.y) return gsNormal;
			}else
				return gsNormal;
		}
	
		return gsNormal;
	}
	
	bool CRowSelectorSvr::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND CRowSelectorSvr::GetHWND()
	{
		return this->m_pGridFrameSvr->GetHWND();
	}
}}}}
