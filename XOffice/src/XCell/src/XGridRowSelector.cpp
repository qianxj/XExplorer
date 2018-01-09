#include "StdAfx.h"
#include "..\include\XGridRowSelector.hpp"
#include "XGridReport.hpp"

extern HMODULE hCellModula;
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridRowSelector::XGridRowSelector(void):m_pGridReportSvr(NULL)
	{
		::SetRect(&FRect,0,0,0,0);
		m_ptMouse.x=-1;
		m_ptMouse.y=-1;
		SetEventListons();
	}
	XGridRowSelector::~XGridRowSelector(void)
	{
		UnSetEventListons();
	}

	bool XGridRowSelector::SetEventListons()
	{
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XGridRowSelector::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XGridRowSelector::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XGridRowSelector::EvtMouseMove);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XGridRowSelector::EvtSetCursor);
		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&XGridRowSelector::EvtLDoubleClick);

		return true;
	}

	bool XGridRowSelector::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool XGridRowSelector::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;
		POINT point = pEvent->mouse.point;
		FState = CalcSizingState(point,FRect,ARow,nPos);
		if(FState == gsRowSizing)
		{
			m_nSRow = ARow;
			m_nSPos = nPos;
			DrawSizingLine(point);
			this->SetCapture();
		}
		return true;
	}

	bool XGridRowSelector::EvtLDoubleClick(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XGridRowSelector::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(FState == gsRowSizing)
		{
			DrawSizingLine(point);
			FState = gsNormal;
			int height = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(m_nSRow);
			height += point.y - m_nSPos;
			if(height > 0) 
				m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr->SetRowHeight(m_nSRow, height);
			this->ReleaseCapture();
			::InvalidateRect(GetHWND(),NULL,FALSE);
		}
		return true;
	}

	bool XGridRowSelector::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(FState == gsRowSizing)
		{
			DrawSizingLine(m_ptMouse);
			DrawSizingLine(point);
		}
		this->m_ptMouse = point;
		return true;
	}

	bool XGridRowSelector::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;

		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);
		TGridState State = CalcSizingState(point,FRect,ARow,nPos);
		if(State == gsRowSizing)
		{
			HCURSOR hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_ARROWNS));
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}
		//FState = gsNormal;
		return true;
	}

	void XGridRowSelector::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		if(FState == gsRowSizing)
		{
			::MoveToEx(hDC,0, pt.y,NULL);
			//::LineTo(hDC,FRect.right , pt.y);
			::LineTo(hDC,m_pGridReportSvr->FRect.right , pt.y);
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	void XGridRowSelector::OnDraw(HDC hdc,RECT &rect)
	{
		if(!m_pGridReportSvr) return;
		CAxisSvr * pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return;

		int width,height;
		width = pAxisSvr->GetColWidth(0);
		height = rect.bottom - rect.top;

		int startCell = 0;
		int LineY = 0;
		int ARow= m_pGridReportSvr->FTopLeft.y;

		RECT	ARect;
		TCHAR	values[40];
		ARect.top = rect.top + pAxisSvr->GetRowHeight(0) + m_pGridReportSvr->FGridLineHeight;
		ARect.left = rect.left;
		ARect.right = rect.left + pAxisSvr->GetColWidth(0);
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

		TGridRect Selection = m_pGridReportSvr->GetSelection();
		int valueRow = ARow - 1;
		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		if(((XXmlContentSvr *)m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
		{
			ARow = XXmlContentSvr::GetTreeIndex(pTreeItems,ARow) + 1;
		}
		while(LineY <= rect.bottom)
		{
			int i=0;
			if(ARow <= m_pGridReportSvr->GetRowCount())
			{
				if(m_pGridReportSvr->IsTreeRowShow(ARow))
				{
					ARect.bottom = ARect.top + pAxisSvr->GetRowHeight(ARow) - 2;
					wsprintf(values,_T("%d\0"),valueRow + 1);
					valueRow++;

					//draw background
					COLORREF color;
					if(valueRow >=Selection.top && valueRow<=Selection.bottom)
						color= RGB(0xc8,0xd7,0xec);//RGB(0xcc,0xd8,0xcc);
					else
						color= RGB(0xd8,0xe7,0xfc);//RGB(0xcc,0xd8,0xcc);

					TRIVERTEX	vertex[2];
					GRADIENT_RECT grect[1];

					vertex[0].x = rect.left;
					vertex[0].y = ARect.top + 1;
					vertex[0].Red  =  GetRValue(color)*256;
					vertex[0].Green = GetGValue(color)*256;
					vertex[0].Blue =  GetBValue(color)*256;
					//vertex[0].Red  = (GetRValue(color)>25? GetRValue(color) - 5:0)*256;
					//vertex[0].Green = (GetGValue(color)>25? GetGValue(color) - 5:0)*256;
					//vertex[0].Blue = (GetBValue(color)>25? GetBValue(color) - 5:0)*256;

					vertex[1].x = rect.left + width;
					vertex[1].y = ARect.bottom - 1;
					vertex[1].Red  =  GetRValue(color)*256;
					vertex[1].Green = GetGValue(color)*256;
					vertex[1].Blue =  GetBValue(color)*256;
					//vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
					//vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
					//vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;

					grect[0].UpperLeft = 0;
					grect[0].LowerRight = 1;

					GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
						
					if(valueRow >=Selection.top && valueRow<=Selection.bottom)
					{
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
						::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
					}
					else
					{
						::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
					}
					ARect.top = ARect.bottom + m_pGridReportSvr->FGridLineHeight;
				}
				ARow++;				
			}else
			{
				RECT bRect;
				::SetRect(&bRect,rect.left, ARect.bottom + 1, ARect.right, rect.bottom);
				HBRUSH hBBrush = ::CreateSolidBrush(RGB(216,231,252));
				::FillRect(hdc,&bRect,hBBrush);
				::DeleteObject(hBBrush);
				break;
			}
			LineY = ARect.top ;

			if(LineY < rect.bottom && ARow == m_pGridReportSvr->GetRowCount() + m_pGridReportSvr->FGridLineHeight)
			{
				ARect.bottom = rect.bottom;
				//draw background
				COLORREF color;
				if(valueRow >=Selection.top && valueRow<=Selection.bottom)
					color= RGB(0xc8,0xd7,0xec);//RGB(0xcc,0xd8,0xcc);
				else
					color= RGB(0xd8,0xe7,0xfc);//RGB(0xcc,0xd8,0xcc);

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
			}
		}

		hFont = (HFONT)::SelectObject(hdc, hFont);
		::DeleteObject(hFont);
		::DeleteObject(hFontB);
		::SetBkMode(hdc,hbr);
	}
	TGridState XGridRowSelector::CalcSizingState(POINT point,RECT &rect)
	{
		int ARow;
		int nPos;
		return CalcSizingState(point,rect,ARow,nPos);
	}

	TGridState XGridRowSelector::CalcSizingState(POINT point,RECT &rect,int &ARow, int &nPos)
	{
		if(!m_pGridReportSvr) return gsNormal;
		CAxisSvr * pAxisSvr = m_pGridReportSvr->GetGridDataSvr()->m_pAxisSvr;
		if(!pAxisSvr) return gsNormal;

		int startCell = m_pGridReportSvr->FTopLeft.y;
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
			if(ARow < m_pGridReportSvr->GetRowCount())
			{
				LineY += pAxisSvr->GetRowHeight(ARow) - 2;// + 1;
				if(abs(LineY - point.y)<=3)
				{
					nPos = LineY;// +  1;
					return gsRowSizing;
				}
				ARow++;
				if(LineY > point.y) return gsNormal;
			}else
				return gsNormal;
		}
	
		return gsNormal;
	}
	
	bool XGridRowSelector::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND XGridRowSelector::GetHWND()
	{
		return this->m_pGridReportSvr->GetHWND();
	}
}}}}
