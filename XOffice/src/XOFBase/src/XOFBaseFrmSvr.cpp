#include "stdafx.h"
#include "XOFBaseFrmsvr.hpp"
#include "xscrollbar.hpp"
#include "xtabsheet.hpp"
#include "xnavigate.hpp"
#include "xofficesheet.hpp"
#include "xofficesheetsvr.hpp"
#include "XOFBaseFrm.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOFBaseFrmSvr::XOFBaseFrmSvr():m_hWnd(NULL),m_pOfficeFrm(NULL),m_pSheet(NULL)
	{
		::SetRect(&m_AreaRect,0,0,0,0);
		::SetRect(&m_ClientRect,0,0,0,0);

		m_pTabSheet = new CXTabSheet;
		m_pTabSheet->AppendSheetAlloc(256);

		//FGridState = gsNormal;
		m_ptHitTest.x = -1;
		m_ptHitTest.y = -1;

		pVScrollBar[0] = new CXScrollBar(SB_VERT);
		pVScrollBar[0]->m_pOwner = this;
		pHScrollBar[0] = new CXScrollBar(SB_HORZ);
		pHScrollBar[0]->m_pOwner = this;
		//pVScrollBar[1] = new CXScrollBar(SB_VERT);
		//pHScrollBar[1] = new CXScrollBar(SB_HORZ);

		m_pNavigate = new CXNavigate();
		m_pNavigate->m_pTabSheet = m_pTabSheet;
		m_pTabSheet->m_pOfficeSvr = this;

	}
	bool XOFBaseFrmSvr::Initial(XOFBaseFrm * m_pFrame)
	{
		m_pOfficeFrm = m_pFrame;
		SetEventListons();
		return true;
	}


	bool XOFBaseFrmSvr::SetEventListons()
	{
		this->AttachListon(WM_CREATE,(FNEvent)&XOFBaseFrmSvr::EvtCreate);
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XOFBaseFrmSvr::EvtSetFocus);
		this->AttachListon(WM_SIZE,(FNEvent)&XOFBaseFrmSvr::EvtSize);
		this->AttachListon(WM_PAINT,(FNEvent)&XOFBaseFrmSvr::EvtPaint);
		
		this->AddRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		m_pTabSheet->AddRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		m_pNavigate->AddRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		pVScrollBar[0]->AddRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		pHScrollBar[0]->AddRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		return true;
	}

	bool XOFBaseFrmSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		this->RemoveRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		m_pTabSheet->RemoveRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		m_pNavigate->RemoveRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		pVScrollBar[0]->RemoveRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);
		pHScrollBar[0]->RemoveRouterMessageFrom((xfAreaObject *)m_pOfficeFrm);

		return true;
	}

	XOFBaseFrmSvr::~XOFBaseFrmSvr(void)
	{
		UnSetEventListons();

		if(pVScrollBar[0])delete pVScrollBar[0];
		//if(pVScrollBar[1])delete pVScrollBar[1];
		if(pHScrollBar[0])delete pHScrollBar[0];
		//if(pHScrollBar[1])delete pHScrollBar[1];
		if(m_pTabSheet) delete m_pTabSheet;
		if(m_pNavigate) delete m_pNavigate;

		for(int i=0;i<m_Sheets.size();i++)
		{
			if(m_Sheets[i]) delete m_Sheets[i];
		}
		m_Sheets.clear();
		for(int i=0;i<m_SheetsElement.size();i++)
		{
			if(m_SheetsElement[i]) m_SheetsElement[i]->Release();
		}
		m_SheetsElement.clear();

	}
	
	int XOFBaseFrmSvr::SelectSheet(int newSheet)
	{
		if(m_Sheets.size() < 1)return 1;
		if(m_pSheet && m_pSheet==m_Sheets[newSheet])
		{
			RECT	rc;
			rc = this->m_pOfficeFrm->m_pxfNode->m_AreaRect;
			::OffsetRect(&rc,- rc.left,- rc.top );

			rc.right -= CXScrollBar::GetVScrollBarWidth(); 
			rc.bottom -= CXScrollBar::GetHScrollBarHeight();

			m_pSheet->m_pSheetSvr->m_pSheetCtrl = m_pSheet;
			m_pSheet->m_pSheetSvr->SetFRect(rc);
			if(m_pSheet->m_pxfNode->GetXPage()->m_pFocusNodeControl == dynamic_cast<xfNodeControl *>(m_pSheet->m_pxfNode)) return 1;
			goto setagent;
		}
		if(m_pSheet)m_pSheet->RemoveRouterMessageFrom(this->m_pOfficeFrm);
		if(m_pSheet && m_pSheet->m_pSheetSvr)
		{
			m_pSheet->m_pSheetSvr->ShowSheet(false);
			::SendMessage(m_pSheet->m_hWnd,WM_KILLFOCUS,0,0);
		}

		m_pSheet =  m_Sheets[newSheet];
		if(m_pSheet && m_pSheet->m_pSheetSvr)
		{
			RECT	rc;
			rc = this->m_pOfficeFrm->m_pxfNode->m_AreaRect;
			::OffsetRect(&rc,- rc.left,- rc.top );
			rc.right -= CXScrollBar::GetVScrollBarWidth(); 
			rc.bottom -= CXScrollBar::GetHScrollBarHeight();

			m_pSheet->m_pSheetSvr->m_pSheetCtrl = m_pSheet;
			m_pSheet->m_pSheetSvr->SetFRect(rc);
		}
		if(m_pSheet && m_pSheet->m_pSheetSvr)
		{
			if(!m_pSheet->m_pSheetSvr->m_bContentLoaded)
				m_pSheet->m_pSheetSvr->LoadTemplate(m_SheetsElement[newSheet],m_state);
			m_pSheet->m_pSheetSvr->ShowSheet(true);
			::InvalidateRect(m_hWnd,&m_pSheet->m_pSheetSvr->FRect,false);
		}
		m_pSheet->AddRouterMessageFrom(this->m_pOfficeFrm);

setagent:
		if(m_pSheet)m_pSheet->FocusedSheet();
		return newSheet;
	}

	bool XOFBaseFrmSvr::EvtCreate(TEvent* pEvent,LPARAM lParam)
	{
		m_hWnd = pEvent->hWnd;
		pVScrollBar[0]->m_hWnd = m_hWnd;
		pHScrollBar[0]->m_hWnd = m_hWnd;
		m_pTabSheet->m_hWnd = m_hWnd;
		m_pNavigate->m_hWnd = m_hWnd;
		return true;
	}
	bool XOFBaseFrmSvr::EvtSetFocus(TEvent* pEvent,LPARAM lParam)
	{
		if((HWND)pEvent->wParam == this->GetHWND())
		{
			//if(m_pSheet)m_pSheet->KillFocusedSheet();
		}else
		{
			if(m_pSheet)m_pSheet->FocusedSheet();
		}
		return true;
	}

	bool XOFBaseFrmSvr::EvtSize(TEvent* pEvent,LPARAM lParam)
	{
		POINT point = pEvent->size.point;
		::SetRect(&m_AreaRect,0,0,point.x,point.y);

		point.x -= CXScrollBar::GetVScrollBarWidth(); 
		point.y -= CXScrollBar::GetHScrollBarHeight();
		lParam = MAKELPARAM(point.x,point.y);
		if(m_pSheet)m_pSheet->EvtSize(pEvent,lParam);
		return true;
	}

	bool XOFBaseFrmSvr::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		HDC hdc = pEvent->paint.hdc;

		RECT	rc;
		HDC	hPaintDC;
		HBITMAP	hBmp;

		rc= m_AreaRect;
		hPaintDC = ::CreateCompatibleDC(hdc);
		hBmp = ::CreateCompatibleBitmap(hdc,rc.right - rc.left + 1,rc.bottom - rc.top +1);
		hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

		HBRUSH	hBrush = ::CreateSolidBrush(RGB(255,255,255));
		::FillRect(hPaintDC,&rc,hBrush);
		::DeleteObject(hBrush);

		this->Draw(hPaintDC);
		::BitBlt(hdc,0,0,rc.right - rc.left + 1,rc.bottom - rc.top +1,hPaintDC,0,0,SRCCOPY);
		hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
		::DeleteObject(hBmp);
		::DeleteDC(hPaintDC); 
		return true;
	}

	int XOFBaseFrmSvr::Draw(HDC hdc, RECT * pRectClip)
	{

		RECT	rect;
		
		int swidth,sheight;
		int hwidth,hheight;
		
		swidth  = GetSystemMetrics(SM_CXVSCROLL);
		sheight = GetSystemMetrics(SM_CYVSCROLL);
		hwidth  = GetSystemMetrics(SM_CXHSCROLL);
		hheight = GetSystemMetrics(SM_CYHSCROLL);

		rect = m_AreaRect;
		rect.bottom -= hheight;
		rect.right -= swidth;
	
		if(m_pSheet)m_pSheet->DoDraw(hdc);

		rect = m_AreaRect;
		rect.left = rect.right - CXScrollBar::GetVScrollBarWidth();
		rect.bottom = rect.bottom - CXScrollBar::GetHScrollBarHeight();
		pVScrollBar[0]->FRect = rect;
		pVScrollBar[0]->OnDraw(hdc);

		rect = m_AreaRect;
		rect.top = rect.bottom - CXScrollBar::GetHScrollBarHeight();
		rect.right = rect.right - CXScrollBar::GetVScrollBarWidth();
		rect.left = rect.left + (m_AreaRect.right - m_AreaRect.left)*4/5;
		pHScrollBar[0]->fnBar = SB_HORZ;
		pHScrollBar[0]->FRect = rect;
		pHScrollBar[0]->OnDraw(hdc);

		HBRUSH	hBrush;
		hBrush = ::GetSysColorBrush(COLOR_3DLIGHT);
		rect = m_AreaRect;
		rect.top = rect.bottom - hheight;
		rect.right = rect.left + (m_AreaRect.right - m_AreaRect.left)*4/5 - 1;
		::FillRect(hdc,&rect,hBrush);

		HPEN hPen = ::CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
		hPen = (HPEN)::SelectObject(hdc,hPen);
		::MoveToEx(hdc,m_AreaRect.left,m_AreaRect.bottom - hheight,NULL);

		::LineTo(hdc,m_AreaRect.left + (m_AreaRect.right - m_AreaRect.left)/2 - 6 ,m_AreaRect.bottom - hheight);
		::LineTo(hdc,(m_AreaRect.right - m_AreaRect.left)/2 - 6 ,m_AreaRect.bottom );
		::MoveToEx(hdc,m_AreaRect.left + 4 * hheight,m_AreaRect.bottom - hheight,NULL);
		::LineTo(hdc,m_AreaRect.left + 4 * hheight ,m_AreaRect.bottom );
		hPen = (HPEN)::SelectObject(hdc,hPen);
		::DeleteObject(hPen);

		
		//draw xnavigate
		::SetRect(&rect, m_AreaRect.left ,m_AreaRect.bottom - hheight,
				 m_AreaRect.left + 4*hheight,
			    m_AreaRect.bottom -1);
		m_pNavigate->FRect = rect;
		m_pNavigate->OnDraw(hdc);

		//draw xtabsheet
		::SetRect(&rect, m_AreaRect.left + 4 * hheight,m_AreaRect.bottom - hheight,
				 m_AreaRect.left + (m_AreaRect.right - m_AreaRect.left)*4/5 - 6,
			    m_AreaRect.bottom -1);
		m_pTabSheet->FRect = rect;
		m_pTabSheet->OnDraw(hdc); 

		return 1;
	}

	BOOL XOFBaseFrmSvr::OnSetCursor(HWND hWnd ,UINT nHitTest, UINT message)
	{
		HCURSOR Cur=NULL;
/*		TGridState State = gsNormal;
		if(nHitTest == HTCLIENT)
		{
			//if(FGridState == gsDrawLining)
			//	Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_PEN));//crHSplit
			//else if(FGridState == gsDeleteLining)
			//	Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_CLEARLINE));//crHSplit
			//else
			//{
				if(FGridState == gsNormal)
				{
					State = m_Sheets[0]->m_pSelectorSvr->CalcSizingState(m_ptHitTest,m_Sheets[0]->m_pSelectorSvr->FRect);
					if(State == gsNormal)
						State =  m_Sheets[0]->m_pColHeaderSvr->CalcSizingState(m_ptHitTest,m_Sheets[0]->m_pColHeaderSvr->FRect);
				} 
				if(State==gsNormal) State = FGridState;
				if(State == gsRowSizing)
				{
					//Cur = LoadCursor(NULL,IDC_SIZENS);//crVSplit]
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));//crHSplit

				}
				if(State == gsFilling)
				{
					//Cur = LoadCursor(NULL,IDC_SIZENS);//crVSplit]
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(1250));//crHSplit

				}
				else if(State == gsColSizing)
				{
					//Cur = LoadCursor(NULL,IDC_SIZEWE);//crHSplit 
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));//crHSplit
				}
				else if(State == gsColsSizing)
				{
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));//crHSplit
				}
				else if(State == gsRowsSizing)
				{
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));//crHSplit
				}
			//}
		}
		if(Cur != NULL)
		{
			SetCursor(Cur);
			return TRUE;
		}
		else */
			return FALSE;
	}
	int XOFBaseFrmSvr::OnNcHitTest(HWND hWnd, POINT point)
	{
		ScreenToClient(hWnd,&point);
		point.x -= this->m_AreaRect.left;
		point.y -= this->m_AreaRect.top;
		m_ptHitTest.x = point.x;
		m_ptHitTest.y = point.y; 
		
		return 0;
	}

	int XOFBaseFrmSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		RECT	rect;
		int swidth,sheight;
		int hwidth,hheight;
		
		swidth  = GetSystemMetrics(SM_CXVSCROLL);
		sheight = GetSystemMetrics(SM_CYVSCROLL);
		hwidth  = GetSystemMetrics(SM_CXHSCROLL);
		hheight = GetSystemMetrics(SM_CYHSCROLL);

		int nSaveDC = -1;
		if(pDrawRect)
		{
			rect = *pDrawRect;
			int nSaveDC = ::SaveDC(hPaintDC);
			SetViewportOrgEx(hPaintDC,pDrawRect->left + 1,pDrawRect->top + 1,NULL);
			::IntersectClipRect(hPaintDC,1,1,
				pDrawRect->right - pDrawRect->left - 1 ,
				pDrawRect->bottom - pDrawRect->top -1);
			SetMetaRgn(hPaintDC);
		}
		else
			rect = m_AreaRect;
		rect.bottom -= hheight;
		rect.right -= swidth;
	
		if(m_pSheet) m_pSheet->DoDraw(hPaintDC);

		rect = m_AreaRect;
		rect.left = rect.right - CXScrollBar::GetVScrollBarWidth();
		rect.bottom = rect.bottom - CXScrollBar::GetHScrollBarHeight();
		pVScrollBar[0]->FRect = rect;
		pVScrollBar[0]->OnDraw(hPaintDC);

		rect = m_AreaRect;
		rect.top = rect.bottom - CXScrollBar::GetHScrollBarHeight();
		rect.right = rect.right - CXScrollBar::GetVScrollBarWidth();
		rect.left = rect.left + (m_AreaRect.right - m_AreaRect.left)/2;
		pHScrollBar[0]->fnBar = SB_HORZ;
		pHScrollBar[0]->FRect = rect;
		pHScrollBar[0]->OnDraw(hPaintDC);

		HBRUSH	hBrush;
		hBrush = ::GetSysColorBrush(COLOR_3DLIGHT);
		rect = m_AreaRect;
		rect.top = rect.bottom - hheight;
		rect.right = rect.left + (m_AreaRect.right - m_AreaRect.left)/2 - 1;
		::FillRect(hPaintDC,&rect,hBrush);

		HPEN hPen = ::CreatePen(PS_SOLID,1,::GetSysColor(COLOR_3DSHADOW));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::MoveToEx(hPaintDC,m_AreaRect.left,m_AreaRect.bottom - hheight,NULL);

		::LineTo(hPaintDC,m_AreaRect.left + (m_AreaRect.right - m_AreaRect.left)/2 - 6 ,m_AreaRect.bottom - hheight);
		::LineTo(hPaintDC,(m_AreaRect.right - m_AreaRect.left)/2 - 6 ,m_AreaRect.bottom );
		::MoveToEx(hPaintDC,m_AreaRect.left + 4 * hheight,m_AreaRect.bottom - hheight,NULL);
		::LineTo(hPaintDC,m_AreaRect.left + 4 * hheight ,m_AreaRect.bottom );
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);

		
		//draw xnavigate
		::SetRect(&rect, m_AreaRect.left ,m_AreaRect.bottom - hheight,
				 m_AreaRect.left + 4*hheight,
			    m_AreaRect.bottom -1);
		m_pNavigate->FRect = rect;
		m_pNavigate->OnDraw(hPaintDC);

		//draw xtabsheet
		::SetRect(&rect, m_AreaRect.left + 4 * hheight,m_AreaRect.bottom - hheight,
				 m_AreaRect.left + (m_AreaRect.right - m_AreaRect.left)/2 - 6,
			    m_AreaRect.bottom -1);
		m_pTabSheet->FRect = rect;
		m_pTabSheet->OnDraw(hPaintDC); 

		if(pDrawRect)
			::RestoreDC(hPaintDC,nSaveDC);

		return 0;
	}


	/*
	int XOFBaseFrmSvr::InitDrawInfo(TGridDrawInfo * pDrawInfo)
	{
		::memset(pDrawInfo,0,sizeof(TGridDrawInfo));

		//set HWND
		pDrawInfo->hWnd = this->GetHwnd();
		
		//set color
		//columnheader color
		pDrawInfo->DrawColor.ColHeaderColor.color0 = COptionSvr::stdColHeaderColor0;
		pDrawInfo->DrawColor.ColHeaderColor.color1 = COptionSvr::stdColHeaderColor1;

		//rowselector color
		pDrawInfo->DrawColor.RowSelectorColor.color0 = COptionSvr::stdRowSelecterColor0;
		pDrawInfo->DrawColor.RowSelectorColor.color1 = COptionSvr::stdRowSelecterColor1;

		//grid color
		pDrawInfo->DrawColor.GridColor.color0 = COptionSvr::stdGridColor0;
		pDrawInfo->DrawColor.GridColor.color1 = COptionSvr::stdGridColor1;
		pDrawInfo->DrawColor.GridColor.color2 = COptionSvr::stdGridColor2;
		pDrawInfo->DrawColor.GridColor.color3 = COptionSvr::stdGridColor3;
		//end set color

		pDrawInfo->pSheets = m_pTabSheet->m_pSheets;
		pDrawInfo->SheetsSize = m_pTabSheet->m_SheetSize;
		pDrawInfo->SheetCurrent = m_pTabSheet->m_iCurSheet;
		pDrawInfo->SheetStart =  m_pTabSheet->m_iStartSheet;

		pDrawInfo->HSizeBarPos = -1;
		pDrawInfo->VSizeBarPos = -1;
		pDrawInfo->VScrollbarWidth = GetSystemMetrics(SM_CXVSCROLL);
		pDrawInfo->hScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);

		pDrawInfo->pGridSheet = m_Sheets[m_pTabSheet->m_iCurSheet];
		pDrawInfo->pAxisSvr = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->m_pAxisSvr;

		pDrawInfo->ColHeaderHeight = pDrawInfo->pAxisSvr->GetRowHeight(0);
		pDrawInfo->RowSelectorWidth = pDrawInfo->pAxisSvr->GetColWidth(0);

		pDrawInfo->FAnchor = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FAnchor;
		pDrawInfo->FCurrent = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FCurrent;
		
		pDrawInfo->EffectiveLineWidth =  pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FGridLineWidth;
		pDrawInfo->FGridState = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FGridState;
		pDrawInfo->FOptions = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FOptions;
		pDrawInfo->FSelections = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FSelections.GetBuffer();
		pDrawInfo->FSelectionSize = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FSelections.size();

		pDrawInfo->FColCount = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->GetColCount();
		pDrawInfo->FRowCount = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->GetRowCount();
		pDrawInfo->FTopLeft = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FTopLeft;
		pDrawInfo->FTopLefts = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FTopLefts;

		pDrawInfo->FrameRect = m_AreaRect;
		pDrawInfo->GridRect = pDrawInfo->pGridSheet->m_pGridFrameSvrs[0]->FRect;
		pDrawInfo->GridSheetPageNavWidth = 4*GetSystemMetrics(SM_CYHSCROLL);
		pDrawInfo->GridSheetPageHeight = GetSystemMetrics(SM_CYHSCROLL);
		pDrawInfo->GridSheetPageWidth = (m_AreaRect.right - m_AreaRect.left) /2 -  pDrawInfo->GridSheetPageNavWidth;


		int Line = pDrawInfo->GridRect.left;
		int i;
		for(i=pDrawInfo->FTopLeft.x;i<=pDrawInfo->FColCount;i++)
		{
			Line += pDrawInfo->EffectiveLineWidth + pDrawInfo->pAxisSvr->GetColWidth(i);
			if(Line >= pDrawInfo->GridRect.right)break;
		}
        pDrawInfo->LastFullVisibleCol = i;

		Line = pDrawInfo->GridRect.top;
		for(i=pDrawInfo->FTopLeft.y;i<=pDrawInfo->FRowCount;i++)
		{
			Line += pDrawInfo->EffectiveLineWidth + pDrawInfo->pAxisSvr->GetRowHeight(i);
			if(Line >= pDrawInfo->GridRect.bottom)break;
		}
        pDrawInfo->LastFullVisibleRow = i;

		return 1;
	}*/
	HWND XOFBaseFrmSvr::GetHWND()
	{
		return m_pOfficeFrm->GetHWND();
	}

	bool XOFBaseFrmSvr::GetClientRect(RECT *pRect)
	{
		return m_pOfficeFrm->GetClientRect(pRect);
	}

	int XOFBaseFrmSvr::Reset()
	{
		if(m_pSheet)m_pSheet->RemoveRouterMessageFrom(this->m_pOfficeFrm);
		if(m_pSheet && m_pSheet->m_pSheetSvr)
		{
			m_pSheet->m_pSheetSvr->ShowSheet(false);
			::SendMessage(m_pSheet->m_hWnd,WM_KILLFOCUS,0,0);
		}
		m_pSheet = NULL;
		for(int i=0;i<m_Sheets.size();i++)
		{
			m_SheetsElement[i]->Release();
			//delete m_Sheets[i];
		}
		m_Sheets.clear();
		m_SheetsElement.clear();
		
		return 1;
	}

}}}
