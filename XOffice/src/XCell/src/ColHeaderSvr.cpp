#include "stdafx.h"
#include "xofbase.h"
#include "ColHeadersvr.hpp"

extern HMODULE hCellModula;
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CColHeaderSvr::CColHeaderSvr(void)
	{
		SetEventListons();
	}
	CColHeaderSvr::~CColHeaderSvr(void)
	{
		UnSetEventListons();
	}

	bool CColHeaderSvr::SetEventListons()
	{
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&CColHeaderSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&CColHeaderSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&CColHeaderSvr::EvtMouseMove);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&CColHeaderSvr::EvtSetCursor);

		return true;
	}

	bool CColHeaderSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool CColHeaderSvr::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		int ACol;
		int nPos;

		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);
		if(m_pGridFrameSvr->FGridState != gsDrawLining && m_pGridFrameSvr->FGridState != gsDeleteLining)
		{
			TGridState State = CalcSizingState(point,FRect,ACol,nPos);
			if(State == gsColSizing)
			{
				HCURSOR hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_ARROWWE));
				hCursor = ::SetCursor(hCursor);
				pEvent->dwRet = 1;
			}
		}
		return true;
	}

	bool CColHeaderSvr::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		int ACol;
		int nPos;
		POINT point = pEvent->mouse.point;
		m_pGridFrameSvr->FGridState = CalcSizingState(point,FRect,ACol,nPos);
		if(m_pGridFrameSvr->FGridState == gsColSizing)
		{
			m_nSCol = ACol;
			m_nSPos = nPos;
			DrawSizingLine(point);
			this->m_ptMouse = point;
			this->SetCapture();
		}
		return true;
	}
	bool CColHeaderSvr::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(m_pGridFrameSvr->FGridState == gsColSizing)
		{
			DrawSizingLine(point);
			m_pGridFrameSvr->FGridState = gsNormal;
			CAxisSvr * pAxisSvr = m_pGridFrameSvr->GetGridDataSvr()->GetAxisSvr(  m_pGridFrameSvr->GetRow());

			int width =  pAxisSvr->GetColWidth(m_nSCol);
			width += point.x - m_nSPos;
			if(width > 0) 
				 pAxisSvr->SetColWidth(m_nSCol, width);
			this->ReleaseCapture();
			if(GetHWND())::InvalidateRect(GetHWND(),NULL,FALSE);
		}
		return true;
	}

	bool CColHeaderSvr::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(m_pGridFrameSvr->FGridState == gsColSizing)
		{
			DrawSizingLine(m_ptMouse);
			DrawSizingLine(point);
		}
		this->m_ptMouse = point;
		return true;
	}

	void CColHeaderSvr::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		if(m_pGridFrameSvr->FGridState == gsColSizing)
		{
			::MoveToEx(hDC,pt.x, 0,NULL);
			::LineTo(hDC,pt.x, m_pGridFrameSvr->FRect.bottom );
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	TGridState CColHeaderSvr::CalcSizingState(POINT point,RECT &rect)
	{
		int ACol;
		int nPos;
		return CalcSizingState(point,rect,ACol,nPos);
	}

	TGridState CColHeaderSvr::CalcSizingState(POINT point,RECT &rect,int &ACol, int &nPos)
	{
		if(!m_pGridFrameSvr) return gsNormal;
		CAxisSvr * pAxisSvr = m_pGridFrameSvr->GetGridDataSvr()->GetAxisSvr(  m_pGridFrameSvr->GetRow());
		if(!pAxisSvr) return gsNormal;

		int startCell = m_pGridFrameSvr->FTopLeft.x;
		int LineX;
		int EndX;

		LineX = rect.left + 1;
		EndX = rect.right;

		if(point.y < rect.top || point.y > rect.bottom) return gsNormal;
		if(point.x < LineX || point.x > EndX) return gsNormal;

		ACol = startCell;
		while(LineX <= EndX )
		{
			int i=0;
			if(ACol < m_pGridFrameSvr->GetColCount())
			{
				LineX += pAxisSvr->GetColWidth(ACol) + 1;
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

	void CColHeaderSvr::OnDraw(HDC hdc,RECT &rect)
	{
		if(!m_pGridFrameSvr) return;

		CAxisSvr * pAxisSvr = m_pGridFrameSvr->GetGridDataSvr()->GetAxisSvr(  m_pGridFrameSvr->GetRow());
		if(!pAxisSvr) return;
		if(pAxisSvr->GetRowHeight(0) < 2)return;

		int width,height;
		width = rect.right- rect.left;
		height = pAxisSvr->GetRowHeight(0);

		int startCell = m_pGridFrameSvr->FTopLeft.x;
		int LineX = 0;
		int ACol = startCell;

		TCHAR Letters[27] =_T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		TCHAR  colName[3];

		RECT	ARect;
		ARect.top = rect.top  + 1;
		ARect.left = rect.left + pAxisSvr->GetColWidth(0) + 1;
		ARect.bottom  = rect.top + pAxisSvr->GetRowHeight(0);
		LineX =  ARect.left;

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
		while(LineX <= rect.right )
		{
			int i=0;
			if(ACol < m_pGridFrameSvr->GetColCount())
			{
				ARect.right = ARect.left + pAxisSvr->GetColWidth(ACol);

				::memset(colName,0, 3*sizeof(TCHAR));
				if( ACol/26/26 < 1)
				{
					if( (ACol - 1)/26 > 0)
					{
						colName[0] = Letters[ (ACol - 1)/26 - 1];
						colName[1] = Letters[(ACol -1) % 26];
					}else
					{
						colName[0] = Letters[(ACol -1)% 26];
					}
				}
				
				//draw background
				COLORREF color;
				if(ACol >=Selection.left && ACol<=Selection.right)
					color= RGB(0xc8,0xd7,0xec);//RGB(0xcc,0xd8,0xcc);
				else
					color= RGB(0xd8,0xe7,0xfc);//RGB(0xcc,0xd8,0xcc);

				TRIVERTEX	vertex[2];
				GRADIENT_RECT grect[1];

				vertex[0].x = ARect.left;
				vertex[0].y = rect.top + 1;
				vertex[0].Red  =  GetRValue(color)*256;
				vertex[0].Green = GetGValue(color)*256;
				vertex[0].Blue =  GetBValue(color)*256;
				//vertex[0].Red  = (GetRValue(color)>25? GetRValue(color) - 5:0)*256;
				//vertex[0].Green = (GetGValue(color)>25? GetGValue(color) - 5:0)*256;
				//vertex[0].Blue = (GetBValue(color)>25? GetBValue(color) - 5:0)*256;

				vertex[1].x = ARect.right;
				vertex[1].y = rect.top + 1 + height ;
				vertex[1].Red  =  GetRValue(color)*256;
				vertex[1].Green = GetGValue(color)*256;
				vertex[1].Blue =  GetBValue(color)*256;
				//vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
				//vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
				//vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;

				grect[0].UpperLeft = 0;
				grect[0].LowerRight = 1;

				GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);

				if(ARect.bottom - ARect.top > 12)
				{
					if(ACol >=Selection.left && ACol<=Selection.right)
					{
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
						::DrawText(hdc,colName,(int)_tcslen(colName),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
						hFontB = (HFONT)::SelectObject(hdc,hFontB);
					}
					else
						::DrawText(hdc,colName,(int)::_tcslen(colName),&ARect,DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX);
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
	bool CColHeaderSvr::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND CColHeaderSvr::GetHWND()
	{
		return this->m_pGridFrameSvr->GetHWND();
	}
}}}}
