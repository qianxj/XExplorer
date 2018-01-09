#include "StdAfx.h"
#include "..\include\XRowSelector.hpp"

extern HMODULE	g_DllModule;
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XRowSelector::XRowSelector(xfAreaObject* pOwner,IAxisSvr * pAxisSvr,XOfficeStyleSvr * pStyleSvr):IRowSelector(),
		m_pOwner(pOwner),m_pAxisSvr(pAxisSvr),m_pStyleSvr(pStyleSvr),FGridState(gsNormal),
		m_nStyleIndex(-1),m_nSelectionStyleIndex(-1),m_hotStyleIndex(-1),m_nStartRow(1),m_nRowCount(25)

	{
		::SetRect(&FRect,0,0,0,0);
		m_ptMouse.x=-1;
		m_ptMouse.y=-1;
		SetEventListons();
	}

	XRowSelector::~XRowSelector(void)
	{
		UnSetEventListons();
	}

	bool XRowSelector::SetEventListons()
	{
		AttachListon(WM_LBUTTONDOWN,(FNEvent)&XRowSelector::EvtLButtonDown);
		AttachListon(WM_LBUTTONUP,(FNEvent)&XRowSelector::EvtLButtonUp);
		AttachListon(WM_MOUSEMOVE,(FNEvent)&XRowSelector::EvtMouseMove);
		AttachListon(WM_SETCURSOR,(FNEvent)&XRowSelector::EvtSetCursor);
		AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&XRowSelector::EvtLDoubleClick);

		return true;
	}

	bool XRowSelector::UnSetEventListons()
	{
		ClearAllListonsTo();
		return true;
	}

	XAPI STDMETHODIMP_(int) XRowSelector::SetFRect(TRect &rc)
	{
		FRect = rc;
		return 1;
	}

	void XRowSelector::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;
		RECT	rc;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );
		m_pOwner->GetClientRect(&rc);

		if(FGridState == gsRowSizing)
		{
			::MoveToEx(hDC,0, pt.y,NULL);
			::LineTo(hDC,rc.right , pt.y);
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	TGridState XRowSelector::CalcSizingState(POINT point,RECT &rect)
	{
		int ARow;
		int nPos;
		return CalcSizingState(point,rect,ARow,nPos);
	}

	TGridState XRowSelector::CalcSizingState(POINT point,RECT &rect,int &ARow,int &nPos)
	{
		int LineY;
		int EndY;

		LineY = rect.top  + 1;
		EndY = rect.bottom;

		if(point.x < rect.left || point.x > rect.right) return gsNormal;
		if(point.y < LineY || point.y > EndY) return gsNormal;

		ARow = GetStartRow();
		while(LineY <= EndY )
		{
			int i=0;
			if(ARow < GetRowCount())
			{
				LineY += m_pAxisSvr->GetRowHeight(ARow) + 1;
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

	bool XRowSelector::EvtLDoubleClick(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XRowSelector::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;
		POINT point = pEvent->mouse.point;
		FGridState = CalcSizingState(point,FRect,ARow,nPos);
		if(FGridState == gsRowSizing)
		{
			m_nSRow = ARow;
			m_nSPos = nPos;
			DrawSizingLine(point);
			this->SetCapture();
		}
		return true;
	}

	bool XRowSelector::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(FGridState == gsRowSizing)
		{
			DrawSizingLine(point);
			FGridState = gsNormal;
			int height = m_pAxisSvr->GetRowHeight(m_nSRow);
			height += point.y - m_nSPos;
			if(height > 0) 
			{
				m_pAxisSvr->SetRowHeight(m_nSRow, height);
				int k = m_pAxisSvr->GetRowHeight(m_nSRow);
				int j=1;
			}
			ReleaseCapture();
			::InvalidateRect(m_pOwner->GetHWND(),NULL,FALSE);
		}
		return true;
	}

	bool XRowSelector::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(FGridState == gsRowSizing)
		{
			DrawSizingLine(m_ptMouse);
			DrawSizingLine(point);
		}
		this->m_ptMouse = point;
		return true;
	}

	bool XRowSelector::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		int ARow;
		int nPos;

		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);
		TGridState State = CalcSizingState(point,FRect,ARow,nPos);
		if(State == gsRowSizing)
		{
			HCURSOR hCursor = LoadCursor(g_DllModule,MAKEINTRESOURCE(IDC_ARROWNS));
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}
		return true;
	}

	bool XRowSelector::IsRowSelected(int nRow)
	{
		return false;
	}

	bool XRowSelector::IsRowMarked(int nRow)
	{
		return false;
	}


	STDMETHODIMP_(void)XRowSelector::OnDraw(HDC hdc,RECT &rect)
	{
		int width,height;
		width = m_pAxisSvr->GetColWidth(0);
		height = rect.bottom - rect.top;

		int LineY = 0;
		int ARow= GetStartRow();

		RECT	ARect;
		TCHAR	values[40];
		ARect.top = rect.top + m_pAxisSvr->GetRowHeight(0) + 1;
		ARect.left = rect.left;
		ARect.right = rect.left + m_pAxisSvr->GetColWidth(0);
		LineY =  ARect.top;

		LOGFONT	logFont;
		XUtil::Css::GetDefaultFont(logFont);

		HFONT	hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);

		logFont.lfWeight = FW_BOLD;
		HFONT	hFontB =::CreateFontIndirect(&logFont);

		int hbr;
		hbr = ::SetBkMode(hdc,TRANSPARENT);

		while(LineY <= rect.bottom )
		{
			int i=0;
			if(ARow < GetRowCount())
			{
				ARect.bottom = ARect.top + m_pAxisSvr->GetRowHeight(ARow);
				wsprintf(values,_T("%d\0"),ARow);

				//draw background
				COLORREF color;
				if(IsRowSelected(ARow))
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
					
				if(IsRowSelected(ARow))
				{
					hFontB = (HFONT)::SelectObject(hdc,hFontB);
					::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					hFontB = (HFONT)::SelectObject(hdc,hFontB);
				}
				else
				{
					::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
				}

				if(IsRowMarked(ARow))
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

	int XRowSelector::Invalidate(HWND hWnd)
	{
		::InvalidateRect(hWnd,&FRect,false);
		return 1;
	}

	HWND XRowSelector::GetHWND()
	{
		return m_pOwner->GetHWND();
	}

	bool XRowSelector::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
}}}
