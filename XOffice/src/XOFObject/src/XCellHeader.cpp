#include "StdAfx.h"
#include "..\include\XCellHeader.hpp"

extern HMODULE	g_DllModule;
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XCellHeader::XCellHeader(xfAreaObject* pOwner,IAxisSvr * pAxisSvr,XOfficeStyleSvr * pStyleSvr):IHeaderSelector(),
		m_pOwner(pOwner),m_pAxisSvr(pAxisSvr),m_pStyleSvr(pStyleSvr),FGridState(gsNormal),
		m_nStyleIndex(-1),m_nSelectionStyleIndex(-1),m_hotStyleIndex(-1),m_nStartCol(1),m_nColCount(16)

	{
		::SetRect(&FRect,0,0,0,0);
		m_ptMouse.x=-1;
		m_ptMouse.y=-1;
		SetEventListons();
	}

	XCellHeader::~XCellHeader(void) 
	{
		UnSetEventListons();
	}

	bool XCellHeader::SetEventListons()
	{
		AttachListon(WM_LBUTTONDOWN,(FNEvent)&XCellHeader::EvtLButtonDown);
		AttachListon(WM_LBUTTONUP,(FNEvent)&XCellHeader::EvtLButtonUp);
		AttachListon(WM_MOUSEMOVE,(FNEvent)&XCellHeader::EvtMouseMove);
		AttachListon(WM_SETCURSOR,(FNEvent)&XCellHeader::EvtSetCursor);
		AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&XCellHeader::EvtLDoubleClick);

		return true;
	}

	bool XCellHeader::UnSetEventListons()
	{
		ClearAllListonsTo();
		return true;
	}

	XAPI STDMETHODIMP_(int) XCellHeader::SetFRect(TRect &rc)
	{
		FRect = rc;
		return 1;
	}

	void XCellHeader::DrawSizingLine(POINT pt)
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

		if(FGridState == gsColSizing)
		{
			::MoveToEx(hDC,pt.x, 0,NULL);
			::LineTo(hDC,pt.x , rc.bottom);
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	TGridState XCellHeader::CalcSizingState(POINT point,RECT &rect)
	{
		int ACol;
		int nPos;
		return CalcSizingState(point,rect,ACol,nPos);
	}

	TGridState XCellHeader::CalcSizingState(POINT point,RECT &rect,int &ACol,int &nPos)
	{
		int LineX;
		int EndX;

		LineX = rect.left  + 1;
		EndX = rect.right;

		if(point.y < rect.top || point.y > rect.bottom) return gsNormal;
		if(point.x < LineX || point.x > EndX) return gsNormal;

		ACol = GetStartCol();
		while(LineX <= EndX )
		{
			int i=0;
			if(ACol < GetColCount())
			{
				LineX += m_pAxisSvr->GetColWidth(ACol) + 1;
				if(abs(LineX - point.x)<=3)
				{
					nPos = LineX +  1;
					return gsColSizing;
				}
				ACol++;
				if(LineX > point.x) return gsNormal;
			}else
				return gsNormal;
		}
	
		return gsNormal;
	}

	bool XCellHeader::EvtLDoubleClick(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XCellHeader::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		int ACol;
		int nPos;
		POINT point = pEvent->mouse.point;
		FGridState = CalcSizingState(point,FRect,ACol,nPos);
		if(FGridState == gsColSizing)
		{
			m_nSCol = ACol;
			m_nSPos = nPos;
			DrawSizingLine(point);
			this->SetCapture();
		}
		return true;
	}

	bool XCellHeader::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(FGridState == gsColSizing)
		{
			DrawSizingLine(point);
			FGridState = gsNormal;
			int width = m_pAxisSvr->GetColWidth(m_nSCol);
			width += point.x - m_nSPos;
			if(width > 0) 
				m_pAxisSvr->SetColWidth(m_nSCol, width);
			ReleaseCapture();
			::InvalidateRect(m_pOwner->GetHWND(),NULL,FALSE);
		}
		return true;
	}

	bool XCellHeader::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(FGridState == gsColSizing)
		{
			DrawSizingLine(m_ptMouse);
			DrawSizingLine(point);
		}
		this->m_ptMouse = point;
		return true;
	}

	bool XCellHeader::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		int ACol;
		int nPos;

		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);
		TGridState State = CalcSizingState(point,FRect,ACol,nPos);
		if(State == gsColSizing)
		{
			HCURSOR hCursor = LoadCursor(g_DllModule,MAKEINTRESOURCE(IDC_ARROWWE));
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}
		return true;
	}

	bool XCellHeader::IsColSelected(int nCol)
	{
		return false;
	}

	bool XCellHeader::IsColMarked(int nCol)
	{
		return false;
	}


	STDMETHODIMP_(void)XCellHeader::OnDraw(HDC hdc,RECT &rect)
	{
		int width,height;
		height = m_pAxisSvr->GetRowHeight(0); 
		width = rect.right - rect.left;

		int LineX = 0;
		int ACol= GetStartCol();

		RECT	ARect;
		TCHAR	values[40];
		ARect.left = rect.left + m_pAxisSvr->GetColWidth(0) + 1;
		ARect.top = rect.top;
		ARect.bottom = rect.top + m_pAxisSvr->GetColWidth(0);
		LineX =  ARect.left;

		LOGFONT	logFont;
		XUtil::Css::GetDefaultFont(logFont);

		HFONT	hFont =::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hdc, hFont);

		logFont.lfWeight = FW_BOLD;
		HFONT	hFontB =::CreateFontIndirect(&logFont);

		int hbr;
		hbr = ::SetBkMode(hdc,TRANSPARENT);

		while(LineX <= rect.right )
		{
			int i=0;
			if(ACol < GetColCount())
			{
				ARect.right = ARect.left + m_pAxisSvr->GetColWidth(ACol);
				wsprintf(values,_T("%d\0"),ACol);

				//draw background
				COLORREF color;
				if(IsColSelected(ACol))
					color= RGB(0xc8,0xd7,0xec);//RGB(0xcc,0xd8,0xcc);
				else
					color= RGB(0xd8,0xe7,0xfc);//RGB(0xcc,0xd8,0xcc);

				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = ARect.left;
				vertex[0].y = rect.top ;
				vertex[0].Red  =  GetRValue(color)*256;
				vertex[0].Green = GetGValue(color)*256;
				vertex[0].Blue =  GetBValue(color)*256;
				//vertex[0].Red  = (GetRValue(color)>25? GetRValue(color) - 5:0)*256;
				//vertex[0].Green = (GetGValue(color)>25? GetGValue(color) - 5:0)*256;
				//vertex[0].Blue = (GetBValue(color)>25? GetBValue(color) - 5:0)*256;

				vertex[1].x = ARect.right;
				vertex[1].y = rect.top + height;
				vertex[1].Red  =  GetRValue(color)*256;
				vertex[1].Green = GetGValue(color)*256;
				vertex[1].Blue =  GetBValue(color)*256;
				//vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
				//vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
				//vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;

				GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);
					
				if(IsColSelected(ACol))
				{
					hFontB = (HFONT)::SelectObject(hdc,hFontB);
					::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					hFontB = (HFONT)::SelectObject(hdc,hFontB);
				}
				else
				{
					::DrawText(hdc,values,(int)::_tcslen(values),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
				}

				if(IsColMarked(ACol))
				{
					HBRUSH hbrush = ::CreateSolidBrush(RGB(128,0,0));
					hbrush = (HBRUSH)::SelectObject(hdc,hbrush);
					PatBlt(hdc,ARect.left+1,ARect.bottom - 4,4,4,PATCOPY);
					hbrush = (HBRUSH)::SelectObject(hdc,hbrush);
					::DeleteObject(hbrush);
				}
					
				ACol++;
				ARect.left = ARect.right + 1;
			}else
				break;
			LineX = ARect.left ;
		}
		hFont = (HFONT)::SelectObject(hdc, hFont);
		::DeleteObject(hFont);
		::DeleteObject(hFontB);
		::SetBkMode(hdc,hbr);
	}

	int XCellHeader::Invalidate(HWND hWnd)
	{
		::InvalidateRect(hWnd,&FRect,false);
		return 1;
	}

	HWND XCellHeader::GetHWND()
	{
		return m_pOwner->GetHWND();
	}

	bool XCellHeader::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
}}}
