#include "stdafx.h"
#include "xofbase.h"
#include "xRowSelectorsvr.hpp"
#include "XReportSheetSvr.hpp"
#include "XReportDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XRowSelectorSvr::XRowSelectorSvr(void):m_pAxisSvr(NULL),m_nWidth(0),m_pSheetSvr(NULL),m_nIndexStyle(-1),m_bDrawRowNo(true),
			m_nCellIndexStyle(-1),m_nSelectionIndexStyle(-1),m_nCornerIndexStyle(-1)
	{
		::SetRect(&FRect,0,0,0,0);
		m_ptMouse.x=-1;
		m_ptMouse.y=-1;
		SetEventListons();
	}
	XRowSelectorSvr::~XRowSelectorSvr(void)
	{
		UnSetEventListons();
	}

	bool XRowSelectorSvr::SetEventListons()
	{
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XRowSelectorSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XRowSelectorSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XRowSelectorSvr::EvtMouseMove);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XRowSelectorSvr::EvtSetCursor);

		return true;
	}

	bool XRowSelectorSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool XRowSelectorSvr::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;
		POINT point = pEvent->mouse.point;
		m_pSheetSvr->FGridState = CalcSizingState(point,FRect,ARow,nPos);
		if(m_pSheetSvr->FGridState == gsRowSizing)
		{
			m_nSRow = ARow;
			m_nSPos = nPos;
			DrawSizingLine(point);
			this->SetCapture();
		}
		return true;
	}
	bool XRowSelectorSvr::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(m_pSheetSvr->FGridState == gsRowSizing)
		{
			DrawSizingLine(point);
			m_pSheetSvr->FGridState = gsNormal;
			int height = m_pAxisSvr->GetRowHeight(m_nSRow);
			height += point.y - m_nSPos;
			if(height > 0) 
				m_pAxisSvr->SetRowHeight(m_nSRow, height);
			this->ReleaseCapture();
			::InvalidateRect(GetHWND(),NULL,FALSE);
		}
		return true;
	}

	bool XRowSelectorSvr::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(m_pSheetSvr->FGridState == gsRowSizing)
		{
			DrawSizingLine(m_ptMouse);
			DrawSizingLine(point);
		}
		this->m_ptMouse = point;
		return true;
	}

	bool XRowSelectorSvr::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;

		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);
		m_pSheetSvr->FGridState = CalcSizingState(point,FRect,ARow,nPos);
		if(m_pSheetSvr->FGridState == gsRowSizing)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_SIZENS);
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}
		m_pSheetSvr->FGridState = gsNormal;
		return true;
	}

	void XRowSelectorSvr::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		if(m_pSheetSvr->FGridState == gsRowSizing)
		{
			::MoveToEx(hDC,0, pt.y,NULL);
			::LineTo(hDC,m_pSheetSvr->FRect.right , pt.y);
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	int	XRowSelectorSvr::DrawCorner(HDC hdc,RECT &rc)
	{
		const XUtil::Css::TBoxStyle * pStyle = this->m_pSheetSvr->GetReportDataSvr()->m_pStyleSvr->GetBoxStyle(m_nCornerIndexStyle);
		if(pStyle)
			XUtil::Css::DrawCssFrame(hdc,&rc,pStyle);
		else
			this->DrawDefaultFrame(hdc,0,rc);

		return 1;
	}

	int XRowSelectorSvr::DrawDefaultFrame(HDC hdc,int ARow,RECT &rc)
	{
		//draw background
		COLORREF color;
		if(ARow == m_pSheetSvr->GetRow())
			color= RGB(0xc8,0xd7,0xec);//RGB(0xcc,0xd8,0xcc);
		else
			color= RGB(0xd8,0xe7,0xfc);//RGB(0xcc,0xd8,0xcc);

		RECT drect = rc;
		TRIVERTEX	vertex[2];
		GRADIENT_RECT grect[1];

		vertex[0].x = drect.left;
		vertex[0].y = drect.top ;
		//vertex[0].Red  =  GetRValue(color)*256;
		//vertex[0].Green = GetGValue(color)*256;
		//vertex[0].Blue =  GetBValue(color)*256;
		vertex[0].Red  = (GetRValue(color)>25? GetRValue(color) - 5:0)*256;
		vertex[0].Green = (GetGValue(color)>25? GetGValue(color) - 5:0)*256;
		vertex[0].Blue = (GetBValue(color)>25? GetBValue(color) - 5:0)*256;

		vertex[1].x = drect.right;
		vertex[1].y = drect.bottom;
		//vertex[1].Red  =  GetRValue(color)*256;
		//vertex[1].Green = GetGValue(color)*256;
		//vertex[1].Blue =  GetBValue(color)*256;
		vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
		vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
		vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;

		grect[0].UpperLeft = 0;
		grect[0].LowerRight = 1;

		GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);

		return 1;
	}

	void XRowSelectorSvr::OnDraw(HDC hdc,RECT &rect)
	{
		if(!m_pSheetSvr) return;
		XReportAxisSvr * pAxisSvr = m_pAxisSvr;
		if(!pAxisSvr) return;

		int height;
		height = rect.bottom - rect.top;

		int startCell = 0;
		int LineY = 0;
		int ARow= m_pSheetSvr->FTopLeft.y;

		RECT	ARect;
		TCHAR	values[40];
		ARect.top = rect.top ;
		ARect.left = rect.left;
		ARect.right = rect.right;
		LineY =  ARect.top;

		const XUtil::Css::TBoxStyle * pStyle = this->m_pSheetSvr->GetReportDataSvr()->m_pStyleSvr->GetBoxStyle(m_nIndexStyle);

		LOGFONT	logFont;
		XUtil::Css::GetFont(pStyle,logFont);
		HFONT	hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);

		logFont.lfWeight = FW_BOLD;
		HFONT	hFontB =::CreateFontIndirect(&logFont);

		int hbr;
		hbr = ::SetBkMode(hdc,TRANSPARENT);
		while(LineY <= rect.bottom )
		{
			int i=0;
			if(ARow <= m_pSheetSvr->GetRowCount())
			{
				ARect.bottom = ARect.top + pAxisSvr->GetRowHeight(ARow);
				wsprintf(values,_T("%d\0"),ARow);

				if(pStyle)
					XUtil::Css::DrawCssFrame(hdc,&ARect,pStyle);
				else
					this->DrawDefaultFrame(hdc,ARow,ARect);

				if(!this->SholudDrawRowNo())
				{
					if(ARow == m_pSheetSvr->GetRow())
					{

					}
				}else
				{
					if(ARow == m_pSheetSvr->GetRow())
					{
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
						::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
					}
					else
					{
						::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
					}
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

	TGridState XRowSelectorSvr::CalcSizingState(POINT point,RECT &rect)
	{
		int ARow;
		int nPos;
		return CalcSizingState(point,rect,ARow,nPos);
	}

	TGridState XRowSelectorSvr::CalcSizingState(POINT point,RECT &rect,int &ARow, int &nPos)
	{
		if(!m_pSheetSvr) return gsNormal;
		XReportAxisSvr * pAxisSvr = m_pAxisSvr;
		if(!pAxisSvr) return gsNormal;

		int startCell = m_pSheetSvr->FTopLeft.y;
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
			if(ARow < m_pSheetSvr->GetReportDataSvr()->m_pContentSvr->GetRowCount())
			{
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
	
	bool XRowSelectorSvr::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND XRowSelectorSvr::GetHWND()
	{
		return this->m_pSheetSvr->GetHWND();
	}
}}}}
