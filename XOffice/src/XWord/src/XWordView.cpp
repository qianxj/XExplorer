#include "StdAfx.h"
#include "XWordView.hpp"
#include "XWordLayerDocument.hpp"
#include "XWCodeInter.hpp"
#include "XWCodeContext.hpp"
#include "XWordLayerPara.hpp"
#include "xuXUtil.h"
#include <assert.h>
#include "xcell.h"

using namespace Hxsoft;
using namespace Hxsoft::XFrame;

XWordView::XWordView(void):FOptions(goThumbTracking),m_pages(-1),
m_pSelectionBlocks(new std::vector<vord_t>),m_hitPos(HitPosition::none),
m_State(TWordState_Normal)
{
	::SetRect(&FRect,0,0,0,0);
	
	m_pSerializeSvr = new XWordSerializeSvr;
	m_pDataSvr = new XWordDataSvr;

	FTopLeft.x = 0;
	FTopLeft.y = 0;

	m_ptHittest.x = -1;
	m_ptHittest.y = -1;

	m_ptScroll.x = 0;
	m_ptScroll.y = 0;

	m_ptObj.x = 0;
	m_ptObj.y = 0;

	m_Selection.startCol = 1;
	m_Selection.startRow = 1;
	m_Selection.endCol = 1;
	m_Selection.endRow = 1;


	InitCursorInfo(m_CursorInfo);

	SetEventListons();

	
}

XWordView::~XWordView(void)
{
	if(m_pSerializeSvr) delete m_pSerializeSvr;
	if(m_pDataSvr) delete m_pDataSvr ;

	if(m_pSelectionBlocks)delete m_pSelectionBlocks;


	UnSetEventListons();
}

bool XWordView::SetEventListons()
{
	
	this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XWordView::EvtLButtonDown);
	this->AttachListon(WM_LBUTTONUP,(FNEvent)&XWordView::EvtLButtonUp);
	this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XWordView::EvtMouseMove);
	
	this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XWordView::EvtMouseWheel);
	this->AttachListon(WM_VSCROLL,(FNEvent)&XWordView::EvtVScroll);
	this->AttachListon(WM_HSCROLL,(FNEvent)&XWordView::EvtHScroll);
	this->AttachListon(WM_SIZE,(FNEvent)&XWordView::EvtSize);

	this->AttachListon(WM_CHAR,(FNEvent)&XWordView::EvtChar);
	this->AttachListon(WM_KEYDOWN,(FNEvent)&XWordView::EvtKeyDown);
	this->AttachListon(WM_SETFOCUS,(FNEvent)&XWordView::EvtSetFocus);
	this->AttachListon(WM_KILLFOCUS,(FNEvent)&XWordView::EvtKillFocus);
	this->AttachListon(WM_XCOMMAND,(FNEvent)&XWordView::EvtXCommand);
	/*this->AttachListon(WM_TIMER,(FNEvent)&XWordView::EvtTimer);
	*/
	this->AttachListon(WM_SETCURSOR,(FNEvent)&XWordView::EvtSetCursor);
	this->AttachListon(WM_NCHITTEST,(FNEvent)&XWordView::EvtNCHitTest);


	return true;
}

bool XWordView::UnSetEventListons()
{
	this->ClearAllListonsTo();
	return true;
}

int XWordView::DrawPageMarker(HDC hPaintDC,RECT &rcPage,RECT &rcPageMargin)
{
	HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(192,192,192));
	hPen = (HPEN)::SelectObject(hPaintDC,hPen);

	//topleft
	::MoveToEx(hPaintDC,rcPage.left + rcPageMargin.left - 20,
		rcPage.top + rcPageMargin.top - 1,NULL);
	::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
		rcPage.top + rcPageMargin.top - 1);
	::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
		rcPage.top + rcPageMargin.top - 20); 

	//bottomleft
	::MoveToEx(hPaintDC,rcPage.left + rcPageMargin.left - 20,
		rcPage.bottom - rcPageMargin.bottom - 1,NULL);
	::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
		rcPage.bottom - rcPageMargin.bottom - 1);
	::LineTo(hPaintDC,rcPage.left + rcPageMargin.left - 1,
		rcPage.bottom - rcPageMargin.bottom + 20); 

	//topright
	::MoveToEx(hPaintDC,rcPage.right - rcPageMargin.right + 20,
		rcPage.top + rcPageMargin.top - 1,NULL);
	::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
		rcPage.top + rcPageMargin.top - 1);
	::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
		rcPage.top + rcPageMargin.top - 20); 

	//bottomright
	::MoveToEx(hPaintDC,rcPage.right - rcPageMargin.right + 20,
		rcPage.bottom - rcPageMargin.bottom - 1,NULL);
	::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
		rcPage.bottom - rcPageMargin.bottom - 1);
	::LineTo(hPaintDC,rcPage.right - rcPageMargin.right - 1,
		rcPage.bottom - rcPageMargin.bottom + 20); 

	hPen = (HPEN)::SelectObject(hPaintDC,hPen);
	::DeleteObject(hPen);

	return 1;
}

int XWordView::DoDraw(HDC hPaintDC,RECT * pDrawRect)
{
	int nSaveDC = ::SaveDC(hPaintDC);

	RECT rc;
	if(pDrawRect)
	{
		SetViewportOrgEx(hPaintDC,pDrawRect->left + 1,pDrawRect->top + 1,NULL);
		::IntersectClipRect(hPaintDC,1,1,
			pDrawRect->right - pDrawRect->left - 1 ,
			pDrawRect->bottom - pDrawRect->top -1);
		SetMetaRgn(hPaintDC);
		rc = *pDrawRect;
		::SetRect(&rc,1,1,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
	}
	else
	{
		::GetClientRect(GetHWND(),&rc);
	}

	IntersectClipRect(hPaintDC,rc.left,rc.top,rc.right,rc.bottom);

	HBRUSH hBrush = ::CreateSolidBrush(RGB(144,153,174));
	::FillRect(hPaintDC,&rc,hBrush);
	::DeleteObject(hBrush);

	OffsetViewportOrgEx(hPaintDC,- m_ptScroll.x, - m_ptScroll.y,NULL);

	RECT rcPage;
	if( rc.right - rc.left < m_Page.GetWidth())
		rcPage.left = rc.left + 5;
	else
		rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_Page.GetWidth())/2;
	rcPage.top = rc.top + 20;
	rcPage.right = rcPage.left + m_Page.GetWidth();
	rcPage.bottom = rcPage.top + m_Page.GetHeight();

	//::Rectangle(hPaintDC,rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);
	//DrawPageMarker(hPaintDC,rcPage,m_Page.m_rcPageMargin);

	int pages = m_pages==-1?CalcPages():m_pages;

	OffsetRect(&rc,m_ptScroll.x,m_ptScroll.y);
	for(int i=1;i<=pages;i++)
	{
		if(i>1)
			::OffsetRect(&rcPage, 0,rcPage.bottom - rcPage.top /*+ 1*/ + 20);
		if(rcPage.bottom < rc.top) continue;
		if(rcPage.top > rc.bottom) break;

		::Rectangle(hPaintDC,rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);
		DrawPageMarker(hPaintDC,rcPage,m_Page.m_rcPageMargin);

		POINT	pt;
		pt.y = rcPage.top + m_Page.m_rcPageMargin.top;
		pt.x = rcPage.left + m_Page.m_rcPageMargin.left;
		DoDrawPage(hPaintDC,i,pt,&rc,&rc);
	}
	/*
	if(m_pHRuler && ViewHRuler())
		m_pHRuler->DoDraw(hPaintDC);

	if(m_pVRuler && ViewVRuler())
		m_pVRuler->DoDraw(hPaintDC);

	if(m_pConer && ViewConer())
		m_pConer->DoDraw(hPaintDC);
	*/

	::RestoreDC(hPaintDC,nSaveDC);

	return 0;
}

int XWordView::SetFRect(RECT rc)
{
	return 	XOfficeSheetSvr::SetFRect(rc);

}
int XWordView::ShowSheet(bool bShow)
{
	return -1;
}

CXScrollBar * XWordView::GetHScrollBar()
{
	return m_pSheetCtrl->m_pHScrollBar;
}
CXScrollBar * XWordView::GetVScrollBar()
{
	return m_pSheetCtrl->m_pVScrollBar;
}

void XWordView::WMVScroll(UINT nSBCode, UINT nPos, HWND hWndCtl)
{
	ModifyScrollBar(SB_VERT, nSBCode, nPos);
}

void XWordView::WMHScroll(UINT nSBCode, UINT nPos, HWND hWndCtl)
{
	ModifyScrollBar(SB_HORZ, nSBCode, nPos);
}

void XWordView::WMWheel(POINT point,UINT_PTR	fwKeys,	short zDelta)
{
	if(zDelta > 0)
		ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
	else
		ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
}

int XWordView::CalcScrollBar(int Value, int ScrollCode, int ScrollBar,int Pos)
{
	int Result;
	Result = Value;
	switch(ScrollCode) 
	{
	case SB_LINEUP:
		Result = Value - 15;
		break;
	case SB_LINEDOWN:
		Result = Value + 15;
		break;
	case SB_PAGEUP:
		Result = Value - PageUp(ScrollBar);
		break;
	case SB_PAGEDOWN:
		Result = Value + PageDown(ScrollBar);
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		if((goThumbTracking & FOptions) || (ScrollCode == SB_THUMBPOSITION))
		{
			Result = ScrollBarMin(ScrollBar) + 
				MulDiv(Pos, ScrollBarMax(ScrollBar)
				- ScrollBarMin(ScrollBar), 100);
			break;
		}
	case SB_BOTTOM:
		Result = ScrollBarMax(ScrollBar);
		break;
	case SB_TOP:
		Result = ScrollBarMin(ScrollBar);
		break;
	}
	return Result;
}

void XWordView::ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos)
{
	if(ScrollBar==SB_VERT)
	{
		m_ptScroll.y = this->CalcScrollBar(m_ptScroll.y,ScrollCode,ScrollBar,Pos);
		if(m_ptScroll.y < ScrollBarMin(ScrollBar)) m_ptScroll.y = ScrollBarMin(ScrollBar);
		if(m_ptScroll.y > ScrollBarMax(ScrollBar)) m_ptScroll.y = ScrollBarMax(ScrollBar);
	}else
	{
		m_ptScroll.x = this->CalcScrollBar(m_ptScroll.x,ScrollCode,ScrollBar,Pos);
		if(m_ptScroll.x < ScrollBarMin(ScrollBar)) m_ptScroll.x = ScrollBarMin(ScrollBar);
		if(m_ptScroll.x > ScrollBarMax(ScrollBar)) m_ptScroll.x = ScrollBarMax(ScrollBar);
	}
	this->UpdateScrollPos(ScrollBar);
	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	ShowCurrentCaret(true);
}

void XWordView::UpdateScrollPos(int ScrollBar)
{
	if(ScrollBar==SB_VERT)
	{
		if(GetVScrollBar())GetVScrollBar()->SetPosition(MulDiv(m_ptScroll.y , 100 , GetMaxScrollExtent(ScrollBar)));
	}else
		if(GetHScrollBar())GetHScrollBar()->SetPosition(MulDiv(m_ptScroll.x, 100 , GetMaxScrollExtent(ScrollBar)));
}

int XWordView::PageUp(int ScrollBar)
{
	if(ScrollBar==SB_VERT)
	{
		if(m_ptScroll.y > FRect.bottom - FRect.top + 1)
			return FRect.bottom - FRect.top + 1;
		else
			return m_ptScroll.y;
	}else
	{
		if(m_ptScroll.x > FRect.right - FRect.left + 1)
			return FRect.right - FRect.left + 1;
		else
			return m_ptScroll.x;

	}
	return 0;

}

int XWordView::PageDown(int ScrollBar)
{
	if(ScrollBar==SB_VERT)
	{
		int nMaxScrollExtent = GetMaxScrollExtent(ScrollBar);
		if(m_ptScroll.y +( FRect.bottom - FRect.top + 1) > nMaxScrollExtent)
			return nMaxScrollExtent;
		else
			return m_ptScroll.y + FRect.bottom - FRect.top + 1;
	}else
	{
		int nMaxScrollExtent = GetMaxScrollExtent(ScrollBar);
		if(m_ptScroll.x +( FRect.right - FRect.left + 1) > nMaxScrollExtent)
			return nMaxScrollExtent;
		else
			return m_ptScroll.x + FRect.right - FRect.left + 1;
	}
	return 0;
}

int XWordView::ScrollBarMin(int ScrollBar)
{
	return 0;
}

int XWordView::ScrollBarMax(int ScrollBar)
{
	return GetMaxScrollExtent( ScrollBar);
}

int XWordView::GetTotalHeight()
{
	int pages = m_pages==-1?CalcPages():m_pages;
	return  50 + pages * m_Page.GetHeight() + (pages +1) * 20;

	/*
	HDC hDC = GetDC(GetHWND());
	//return  m_nPage *(m_rcPage.bottom - m_rcPage.top + 1) + (m_nPage +1) * 20;

	XWordLayerDocument doc;
	doc.set_block(&((XWordDataSvr*)m_pDataSvr)->m_pContentSvr->document);
	doc.FRect = m_Page.m_rcPageContent;
	doc.FContentRect = m_Page.m_rcPageContent;

	SIZE sz =  doc.GetExtent();
	if(!sz.cy)
		sz = doc.CalcExtent((XWordDataSvr *)m_pDataSvr,doc.get_block(),NULL,hDC);

	::ReleaseDC(GetHWND(),hDC);

	sz.cy += FRect.top + m_Page.m_rcPageMargin.top;
	if(sz.cy < m_Page.m_rcPage.bottom - m_Page.m_rcPage.top + 50 + FRect.top)
		sz.cy = FRect.top + 50 + m_Page.m_rcPage.bottom - m_Page.m_rcPage.top;

	//sz.cy += sz.cy % m_Page.GetHeight() + 15;
	
	return sz.cy ;*/
}

int XWordView::GetMaxScrollExtent(int ScrollBar)
{
	if(ScrollBar==SB_VERT)
	{
		return GetTotalHeight() - (FRect.bottom - FRect.top + 1);

	}else
	{
		RECT rc = FRect;
		/*if( rc.right - rc.left < m_rcPage.right - m_rcPage.left)
		return (m_rcPage.right - m_rcPage.left) - (rc.right - rc.left) + 10;
		else*/
		return 0; //not need scroll
	}
}

bool XWordView::EvtVScroll(TEvent *pEvent ,LPARAM lParam)
{
	WMVScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
	return true;
}

bool XWordView::EvtHScroll(TEvent *pEvent ,LPARAM lParam)
{
	WMHScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
	return true;
}

bool XWordView::EvtMouseWheel(TEvent *pEvent ,LPARAM lParam)
{
	/*
	if(m_pObj)
	{
		XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
		pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
		pEvent->mouse.point.x -= m_ptObj.x;
		pEvent->mouse.point.y -= m_ptObj.y;
		pCellSvr->m_pGridFrameSvrs->EvtMouseWheel(pEvent,lParam);
		return true;
	}*/
	WMWheel(pEvent->wheel.point,pEvent->wheel.fwKeys,pEvent->wheel.zDelta);
	return true;
}

bool XWordView::EvtSize(TEvent *pEvent ,LPARAM lParam)
{
	return true;
}

bool XWordView::EvtLButtonDown(TEvent *pEvent ,LPARAM lParam)
{
	if(::GetFocus() != m_pSheetCtrl->m_hWnd)::SetFocus(m_pSheetCtrl->m_hWnd);

	POINT point = pEvent->mouse.point;
	XWCursorInfo curInfo;
	InitCursorInfo(curInfo);
	if(!PtInRect(&FRect,point)) return true;

	point.x += m_ptScroll.x;
	point.y += m_ptScroll.y;

	if(CalcCursorInfoAtPoint(point,curInfo))
	{
		m_CursorInfo.pBlock= vord_t();
		m_pSelectionBlocks->clear();

		m_pSelectionBlocks->clear();
		AdjustSelection(curInfo);
		m_CursorInfo = curInfo;
		ShowCurrentCaret(true);
	}else if(curInfo.pControl)
	{	
		vord_t pBlock = curInfo.pControl;
		if(IsControl(pBlock,L"xcell"))
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)pBlock[control::object];
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;

			RECT rt = {pBlock[draw::left],pBlock[draw::top],pBlock[draw::right],pBlock[draw::bottom]};
			pEvent->mouse.point.x -= rt.left;
			pEvent->mouse.point.y -= rt.top;
			pCellSvr->m_pGridFrameSvrs->EvtLButtonDown(pEvent,lParam);
			pCellSvr->m_pGridFrameSvrs->ReleaseCapture();

			m_State = (m_State & 0xffff0000) | TWordState_Selecting;
		}else
		{
			bool bFound = false;
			int i = 0;
			for(i=0;i<(int)m_pSelectionBlocks->size();i++)
			{
				if((*m_pSelectionBlocks)[i]==pBlock)
				{
					bFound = true;
					break;
				}
			}
			if(!(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE))
			{
				m_pSelectionBlocks->clear();
				m_pSelectionBlocks->push_back(pBlock);
			}else
			{
				if(!bFound)
					m_pSelectionBlocks->push_back(pBlock);
				else
				{
					m_pSelectionBlocks->erase(m_pSelectionBlocks->begin() + i);
					m_State = (m_State & 0xffff0000) | TWordState_Normal;
					return true;
				}
			}

			m_hitPos = HitPosition::none;
			if(bFound)
			{
				RECT rt = {pBlock[draw::left],
					pBlock[draw::top],
					pBlock[draw::right],
					pBlock[draw::bottom]};
				m_hitPos = GetHitResizePat(point,rt,2);
			}
			::InvalidateRect(GetHWND(),NULL,false);
			if(m_hitPos == HitPosition::none)
				m_State = (m_State & 0xffff0000) | TWordState_ObjectMoving;
			else
				m_State = (m_State & 0xffff0000) | TWordState_ObjectResizing;
			m_ptObj = point;
			return true;
		}
	}
	m_State = (m_State & 0xffff0000) | TWordState_Selecting;

	return true;
}

int	XWordView::GetHitResizePat(POINT pt, RECT& rt, int cx)
{
	int swidth = (rt.right - rt.left + 1)/2;
	int sheight = (rt.bottom - rt.top + 1)/2;

	if(pt.x>=rt.left - cx && pt.x<= rt.left + cx && pt.y >=rt.top - cx && pt.y<= rt.top + cx) return HitPosition::lefttop;
	if(pt.x>=rt.right - cx && pt.x<= rt.right + cx && pt.y >=rt.top - cx && pt.y<= rt.top + cx) return HitPosition::righttop;
	if(pt.x>=rt.right - cx && pt.x<= rt.right + cx && pt.y >=rt.bottom - cx && pt.y<= rt.bottom + cx) return HitPosition::rightbottom;
	if(pt.x>=rt.left - cx && pt.x<= rt.left + cx && pt.y >=rt.bottom - cx && pt.y<= rt.bottom + cx) return HitPosition::leftbottom;

	if(pt.x>=rt.left - cx + swidth && pt.x<= rt.left + cx +swidth && pt.y >=rt.top - cx && pt.y<= rt.top + cx) return HitPosition::topcenter;
	if(pt.x>=rt.right - cx && pt.x<= rt.right + cx && pt.y >=rt.top - cx + sheight && pt.y<= rt.top + cx + sheight) return HitPosition::rightcenter;
	if(pt.x>=rt.left - cx + swidth && pt.x<= rt.left + cx + swidth && pt.y >=rt.bottom - cx && pt.y<= rt.bottom + cx) return HitPosition::bottomcenter;
	if(pt.x>=rt.left - cx && pt.x<= rt.left + cx && pt.y >=rt.top - cx +sheight&& pt.y<= rt.top + cx +sheight) return HitPosition::leftcenter;

	return HitPosition::none;
}

bool XWordView::EvtLButtonUp(TEvent *pEvent ,LPARAM lParam)
{
	vord_t pBlock = m_CursorInfo.pControl;
	if(m_pSelectionBlocks->size() > 0 && pBlock && IsControl(pBlock,L"xcell"))
	{
		XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)pBlock[control::object];
		pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;

		RECT rt = {pBlock[draw::left],pBlock[draw::top],pBlock[draw::right],pBlock[draw::bottom]};
		pEvent->mouse.point.x -= rt.left;
		pEvent->mouse.point.y -= rt.top;
		pCellSvr->m_pGridFrameSvrs->EvtLButtonUp(pEvent,lParam);

		m_State = (m_State & 0xffff0000) | TWordState_Normal;
		return true;
	}	

	m_State = (m_State & 0xffff0000) | TWordState_Normal;
	return true;
}

bool XWordView::EvtMouseMove(TEvent *pEvent ,LPARAM lParam)
{
	POINT point = pEvent->mouse.point;
	point.x += m_ptScroll.x;
	point.y += m_ptScroll.y;

	if((MK_LBUTTON & pEvent->mouse.nFlags) && ((m_State & 0xffff) == TWordState_Normal ||
		(m_State & 0xffff) == TWordState_Selecting) && m_pSelectionBlocks->size()<1)
	{
		POINT point = pEvent->mouse.point;
		XWCursorInfo curInfo;
		InitCursorInfo(curInfo);
		if(!PtInRect(&FRect,point)) return true;
		
		RECT rt;
		GetClientRect(&rt);
		if(point.x < rt.right - 10)
		{
			point.x += m_ptScroll.x;
			point.y += m_ptScroll.y;

			if(CalcCursorInfoAtPoint(point,curInfo))
			{
				if(!(m_CursorInfo.nRow == curInfo.nRow &&
					m_CursorInfo.nCol == curInfo.nCol && 
					!(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE)))
				{
					TShiftState Shift =  ssShift;
					m_pSelectionBlocks->clear();
					AdjustSelection(curInfo,Shift);

					m_CursorInfo = curInfo;
					ShowCurrentCaret(true);
				}
			}
		}
	}

	if((m_State & 0xffff) == TWordState_ObjectMoving)
	{
		int cx = m_ptObj.x - point.x;
		int cy = m_ptObj.y - point.y;

		for(int i=0;i<(int)(*m_pSelectionBlocks).size();i++)
		{
			if(((*m_pSelectionBlocks)[i])[control::left] - cx >0 &&
				((*m_pSelectionBlocks)[i])[control::top] - cy > 0  )
			{
				((*m_pSelectionBlocks)[i])[control::left] -= cx;
				((*m_pSelectionBlocks)[i])[control::top]  -= cy;
				((*m_pSelectionBlocks)[i])[control::right] -= cx;
				((*m_pSelectionBlocks)[i])[control::bottom]  -= cy;

				((*m_pSelectionBlocks)[i])[draw::left] -= cx;
				((*m_pSelectionBlocks)[i])[draw::top]  -= cy;
				((*m_pSelectionBlocks)[i])[draw::right] -= cx;
				((*m_pSelectionBlocks)[i])[draw::bottom]  -= cy;

				((*m_pSelectionBlocks)[i])[extent_c::cy] -=cy;
				((*m_pSelectionBlocks)[i]).get_parent()[extent_c::cy] -=cy;
			}
		}
		m_ptObj = point;
		::InvalidateRect(GetHWND(),NULL,false);
	}
	else if((m_State & 0xffff) == TWordState_ObjectResizing && m_hitPos!=HitPosition::none)
	{
		int cx = m_ptObj.x - point.x;
		int cy = m_ptObj.y - point.y;

		for(int i=0;i<(int)(*m_pSelectionBlocks).size();i++)
		{
			if(m_hitPos==HitPosition::righttop /*|| m_hitPos==HitPosition::leftbottom*/)
			{
				((*m_pSelectionBlocks)[i])[control::right] -= cx;
				((*m_pSelectionBlocks)[i])[control::bottom]  -= cy;
				((*m_pSelectionBlocks)[i])[draw::right] -= cx;
				((*m_pSelectionBlocks)[i])[draw::bottom]  -= cy;

				((*m_pSelectionBlocks)[i])[extent_c::cy] -=cy;
				((*m_pSelectionBlocks)[i]).get_parent()[extent_c::cy] -=cy;
			}
			else if(m_hitPos==HitPosition::rightcenter)
			{
				((*m_pSelectionBlocks)[i])[control::right] -= cx;
				((*m_pSelectionBlocks)[i])[draw::right] -= cx;
			}
			else if(/*m_hitPos==HitPosition::lefttop ||*/ m_hitPos==HitPosition::rightbottom)
			{
				((*m_pSelectionBlocks)[i])[control::right] -= cx;
				((*m_pSelectionBlocks)[i])[control::bottom]  -= cy;
				((*m_pSelectionBlocks)[i])[draw::right] -= cx;
				((*m_pSelectionBlocks)[i])[draw::bottom]  -= cy;

				((*m_pSelectionBlocks)[i])[extent_c::cy] -=cy;
				((*m_pSelectionBlocks)[i]).get_parent()[extent_c::cy] -=cy;
			}
			else if(m_hitPos==HitPosition::bottomcenter)
			{
				((*m_pSelectionBlocks)[i])[control::bottom]  -= cy;
				((*m_pSelectionBlocks)[i])[draw::bottom]  -= cy;

				((*m_pSelectionBlocks)[i])[extent_c::cy] -=cy;
				((*m_pSelectionBlocks)[i]).get_parent()[extent_c::cy] -=cy;
			}
		}

		m_ptObj = point;
		::InvalidateRect(GetHWND(),NULL,false);
	}

	return true;
}


bool XWordView::EvtSetFocus(TEvent *pEvent ,LPARAM lParam)
{
	if(m_CursorInfo.nRow < 1)
		CalcCursorInfo(m_CursorInfo,1,1);
	ShowCurrentCaret(true);

	/*
	if(m_pObj)
	{
		XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
		pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
		pCellSvr->m_pGridFrameSvrs->EvtSetFocus(pEvent,lParam);
		return true;
	}

	if(this->m_pFlowDocument)
	{
		if(this->m_pFlowDocument->m_FDrawState & WDSSELECTION)
		{
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = true;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
		}
		else if(this->m_pFlowDocument->m_FDrawState & WDSIMAGESELECTION)
		{
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = true;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
		}
		else if(this->m_pFlowDocument->m_FDrawState & WDSSHAPESELECTION)
		{
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = true;
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
		}
	}*/

	return true;
}

bool XWordView::EvtKillFocus(TEvent *pEvent ,LPARAM lParam)
{
	return true;
}

bool XWordView::ClampInView(const XWCursorInfo & CursorInfo)
{
	/*
	XWordParagraphRowInfo::RowBlock * pRowBlock;
	XWordParagraphBlock * pTextParagraphBlock = dynamic_cast<XWordParagraphBlock *>(this->GetParagraphByRow(CursorInfo.nRow));
	pRowBlock = this->GetRowBlock(CursorInfo.nRow,CursorInfo.nCol);

	if(CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y < pRowBlock->nRowheight + 40 )
	{
		this->ShowCurrentCaret(false);
		m_ptScroll.y -= pRowBlock->nRowheight - (CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y) + 40;
		if(m_ptScroll.y <= 40)m_ptScroll.y = 0;
		UpdateScrollPos(SB_VERT);
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		this->ShowCurrentCaret(true);
		return true;
	}else if(CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y > FRect.bottom - FRect.top + 1 - 40)
	{
		this->ShowCurrentCaret(false);
		m_ptScroll.y += (CursorInfo.YPosCursur + CursorInfo.CursorHeight - m_ptScroll.y) - (FRect.bottom - FRect.top + 1) + 40;
		if(m_ptScroll.y <= 40)m_ptScroll.y = 0;
		UpdateScrollPos(SB_VERT);
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		this->ShowCurrentCaret(true);
		return true;
	}else
	{
		this->ShowCurrentCaret(true);
		return false;
	}
	*/
	return true;
}
bool XWordView::CalcCursorInfo(XWCursorInfo & CursorInfo,int nRow,int nCol)
{
	int paraRow;
	int row;
	vord_t  item;

	int nPage = GetPageAtRow(nRow);
	if(nPage < 1) return false;

	if(!GetFirstRowOfPage(nPage,row,paraRow,item)) return false;
	

	int rRow = nRow - row + 1;
	int sRow = paraRow;

	RECT rc;
	GetPageRect(nPage,rc);
	POINT pt={rc.left + m_Page.m_rcPageContent.left,
		rc.top + m_Page.m_rcPageContent.top};

	for(;item;item = item.get_next())
	{
		int pc = item[render::pc];
		unsigned char * code = item.get_parent()[render::code];
		if(item[control::type])
		{
			rRow--;
			if(rRow < 1) return false;
			pt.y += item[extent_c::cy] + 6;
		}else
		{
			bool found = false;
			if(paraRow > 1)
			{
				if(rRow + paraRow - 1 <= item[render::rowcount])
					found=true;
				else
				{
					XWCodeInter machine;
					XWCodeContentExtent context;
					machine.Inter(&context,item.get_parent()[render::code] +
						item[render::pc]);
					for(int i= paraRow - 1; i < (int)context.rowHeights.size();i++)
					{
						pt.y += context.rowHeights[i];
						
					}
					rRow -= (int)context.rowHeights.size() - (paraRow - 1);
					pt.y += 6;
				}
			}else
			{
				if(item[render::rowcount]>= rRow )
					found = true;
				else
				{
					rRow -= item[render::rowcount];
					pt.y += item[extent_c::cy] + 6;
				}
			}

			if(found)
			{
				XWCodeInter machine;
				CalcPointAtRowCol xt(rRow,nCol);
				XWCodeContextInfo context;
				xt.startRow = paraRow;
				machine.Inter(xt,&context,item.get_parent()[render::code] +
					item[render::pc]);

				CursorInfo.XPosCursur = xt.pt_.x + pt.x;
				CursorInfo.YPosCursur = xt.pt_.y + pt.y + xt.rowheight_/6 ;
				CursorInfo.nCol = nCol;
				CursorInfo.nRow = nRow;
				CursorInfo.CursorHeight = xt.rowheight_*2/3 + 1;
				CursorInfo.nIndex = xt.index_;
				CursorInfo.pBlock = xt.block_;

				return true;
			}
			paraRow = 0;
		}
	}
	return false;
}

bool XWordView::CalcCursorInfoEx(XWCursorInfo & CursorInfo,vord_t pBlock,int nPosChar)
{
	return false;
}

bool XWordView::CalcCursorInfoAtPoint(POINT pt,XWCursorInfo & CursorInfo)
{
	//clear
	CursorInfo.pControl = vord_t();

	int nPage = this->GetPageAtPoint(pt);
	if(nPage < 1) return false;

	int nRow;
	int paraRow;
	vord_t  item;
	if(!GetFirstRowOfPage(nPage,nRow,paraRow,item)) return false;
	
	RECT rc;
	GetPageRect(nPage,rc);
	POINT rPoint ={pt.x - rc.left - m_Page.m_rcPageContent.left,
		pt.y - rc.top - m_Page.m_rcPageContent.top};

	if(pt.x < rc.left) return false;
	if(pt.x > rc.right) return false;
	if(pt.y < rc.top + m_Page.m_rcPageContent.top) return false;
	if(pt.y > rc.top + m_Page.m_rcPageContent.bottom) return false;

	for(;item;item = item.get_next())
	{
		int pc = item[render::pc];
		unsigned char * code = item.get_parent()[render::code];
		if(item[control::type])
		{
			CursorInfo.pControl = item;
			if(rPoint.y < item[extent_c::cy]) return false;
			nRow++;
			rPoint.y -= item[extent_c::cy] + 6; 
		}else
		{
			bool found = false;
			if(paraRow > 1)
			{
				XWCodeInter machine;
				XWCodeContentExtent context;
				machine.Inter(&context,item.get_parent()[render::code] +
					item[render::pc]);
				int ext = 0;
				int rows = 0;
				for(int i= paraRow - 1; i < (int)context.rowHeights.size();i++)
				{
					rows++;
					ext += context.rowHeights[i];
					if(ext > rPoint.y) break;
				}
				if(ext > rPoint.y)
					found = true;
				else
				{
					nRow += rows;
					rPoint.y -= ext + 6;
				}
			}else
			{
				if(item[extent_c::cy] > rPoint.y)
					found = true;
				else
				{
					rPoint.y -= item[extent_c::cy] + 6;
					nRow += item[render::rowcount];
				}
			}

			if(found)
			{
				XWCodeInter machine;
				CalcRowColAtPoint xt(rPoint);
				XWCodeContextInfo context;
				xt.startRow = paraRow;
				machine.Inter(xt,&context,item.get_parent()[render::code] +
					item[render::pc]);
				
				CalcPointAtRowCol xt1(xt.row_,xt.col_);
				xt1.startRow = paraRow;
				machine.Inter(xt1,&context,item.get_parent()[render::code] +
					item[render::pc]);
				CursorInfo.XPosCursur = xt1.pt_.x + pt.x - rPoint.x;
				CursorInfo.YPosCursur = xt1.pt_.y + pt.y - rPoint.y  + xt1.rowheight_/6 ;
				CursorInfo.nCol = xt.col_;
				CursorInfo.nRow = nRow + xt.row_ - 1;
				CursorInfo.CursorHeight = xt1.rowheight_*2/3 + 1;
				CursorInfo.nIndex = xt1.index_;
				CursorInfo.pBlock = xt1.block_;

				return true;				
			}
			paraRow = 0;

		}
	}
	return false;
}

void XWordView::ShowCurrentCaret(bool bShow)
{
	if(!bShow)
		::HideCaret(m_pSheetCtrl->m_hWnd);
	else
	{
		CreateCaret(m_pSheetCtrl->m_hWnd, NULL, 1, m_CursorInfo.CursorHeight);
		SetCaretPos(m_CursorInfo.XPosCursur - m_ptScroll.x,m_CursorInfo.YPosCursur - m_ptScroll.y);  
		ShowCaret(m_pSheetCtrl->m_hWnd);
	}
}

vord_t XWordView::GetParaAtRow(int & nRow)
{
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;
	if(pDataSvr->m_pContentSvr)
	{
		vord_t doc = pDataSvr->m_pContentSvr->document;
		if(doc)
		{
			//chapter
			for(vord_t item = doc.get_child();item;item = item.get_next())
			{
				if(item(block::type)== blocktype::chapter)
				{
					//para
					for(vord_t pitem = item.get_child();pitem;pitem = pitem.get_next())
					{
						if(pitem[render::rowcount] >= nRow) return pitem;
						nRow -= pitem[render::rowcount];
					}
				}
			}
		}
	}
	return NULL;
}

vord_t XWordView::GetFirstBlockAtRow(int nRow)
{
	vord_t para = GetParaAtRow(nRow);
	if(!para) return NULL;

	XWCodeInter machine;
	XWCodeContextInfo context;
	CalcBlockAtRow cit(nRow);
	machine.Inter(cit,&context,
		para.get_parent()[render::code] + para[render::pc]);
	return cit.block_;
}

vord_t XWordView::GetBlockAtRowColumn(int nRow,int nCol)
{
	int index = 0;
	return GetBlockAtRowColumn(nRow,nCol,index);
}

vord_t XWordView::GetBlockAtRowColumn(int nRow,int nCol, int &index)
{
	vord_t para = GetParaAtRow(nRow); 
	if(!para) return NULL;
	
	XWCodeInter machine;
	XWCodeContextInfo context;
	CalcBlockAtRowCol cit(nRow,nCol);
	machine.Inter(cit,&context,
		para.get_parent()[render::code] + para[render::pc]);
	index = cit.index;
	return cit.block_;
}

int XWordView::GetColCount(int nRow)
{
	vord_t para = GetParaAtRow(nRow);
	if(!para)
		return -1;
	XWCodeInter machine;
	XWCodeContextInfo context;
	CalcColCount cit(nRow);
	if(!para.get_parent()) return -1;
	machine.Inter(cit,&context,
		para.get_parent()[render::code] + para[render::pc]);
	return cit.count_;
}

int XWordView::GetRowCount()
{
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;
	int nRow = 0;
	if(pDataSvr->m_pContentSvr)
	{
		vord_t doc =  pDataSvr->m_pContentSvr->document;
		if(doc)
		{
			//chapter
			for(vord_t item = doc.get_child();item;item = item.get_next())
			{
				if(item(block::type)== blocktype::chapter)
				{
					//para
					for(vord_t pitem = item.get_child();pitem;pitem = pitem.get_next())
					{
						nRow += pitem[render::rowcount];
					}

				}
			}
		}
	}
	return nRow;
}

bool XWordView::CalcLeftCursorInfo(XWCursorInfo & CursorInfo)
{
	if(CursorInfo.nCol > 1)
	{
		CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol - 1);
		return true;
	}
	else
	{
		if(CursorInfo.nRow > 1)
		{
			wchar_t* pStr = CursorInfo.pBlock[text::content];
			if(CursorInfo.nIndex >= 0 && CursorInfo.pBlock && !pStr[CursorInfo.nIndex] && pStr[CursorInfo.nIndex - 1]=='\n')
				CalcCursorInfo(CursorInfo,CursorInfo.nRow - 1,this->GetColCount(CursorInfo.nRow - 1) + 1);
			else if(CursorInfo.nIndex >= 0 && CursorInfo.pBlock && CursorInfo.pBlock[text::content][CursorInfo.nIndex]!='\n')
				CalcCursorInfo(CursorInfo,CursorInfo.nRow - 1,this->GetColCount(CursorInfo.nRow - 1));
			else
				CalcCursorInfo(CursorInfo,CursorInfo.nRow - 1,this->GetColCount(CursorInfo.nRow - 1) + 1);
			return true;
		}
		else
			return false;
	}
}

bool XWordView::CalcRightCursorInfo(XWCursorInfo & CursorInfo)
{
	if(CursorInfo.nCol < this->GetColCount(CursorInfo.nRow) + 1 )
	{
		CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol + 1);
		return true;
	}
	else
	{
		if(CursorInfo.nRow < GetRowCount())
		{
			CalcCursorInfo(CursorInfo,CursorInfo.nRow + 1,1);
			return true;
		}
		else
			return false;
	}
	return false;
}

bool XWordView::CalcUpCursorInfo(XWCursorInfo & CursorInfo)
{
	/*
	XWordParagraphBlock * pTextParagraphBlock ;
	int nParaRow;
	if(CursorInfo.nRow < 2) return false;
	CalcParagraphRow(CursorInfo.nRow - 1,pTextParagraphBlock,nParaRow);

	int dx = 0;
	if(pTextParagraphBlock->m_pContent->align & DT_CENTER)
		dx = pTextParagraphBlock->m_rowInfos[nParaRow].nBala/2;
	if(pTextParagraphBlock->m_pContent->align & DT_RIGHT)
		dx = pTextParagraphBlock->m_rowInfos[nParaRow].nBala;

	RECT rcContent;
	int x;
	int nPosChar,posX, nCol;
	this->CalcContentPageRect(0,&this->FRect,rcContent);
	x = CursorInfo.XPosCursur + m_ptScroll.x - rcContent.left - dx;
	this->PosCharIndexEx(x,&pTextParagraphBlock->m_rowInfos[nParaRow],nPosChar,posX, nCol);
	return this->CalcCursorInfo(CursorInfo,CursorInfo.nRow - 1,nCol);
	*/
	return false;
}

bool XWordView::CalcDownCursorInfo(XWCursorInfo & CursorInfo)
{
	return false;
}

bool XWordView::EvtChar(TEvent *pEvent ,LPARAM lParam)
{
	UINT nChar = pEvent->key.nChar; 
	UINT nRepCnt = pEvent->key.nRepCnt;
	UINT nFlags = pEvent->key.nFlags;

	if(m_pSelectionBlocks->size()>0) return true;
	switch (nChar) 
	{
	case 0x08:          // Backspace 
		BackDeleteChar(m_CursorInfo);
		ClampInView(m_CursorInfo);
		break; 
	case 0x09:          // Tab 
		// Tab stops exist every four spaces, so add 
		// spaces until the user hits the next tab. 

		SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
		SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
		SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
		SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
		break; 

	case 0x0D: 
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) nChar=0x0B;
		InsertChar(m_CursorInfo,nChar);
		ClampInView(m_CursorInfo); 
		break; 
	case 0x0B:   
		InsertChar(m_CursorInfo,nChar);
		ClampInView(m_CursorInfo);
		break; 

	case 0x1B:        // Escape 
	case 0x0A:        // Linefeed 
		MessageBeep((UINT) -1); 
		break; 
	default:
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
			break;
		InsertChar(m_CursorInfo,nChar);
		ClampInView(m_CursorInfo);
		break; 
	}

/*
	if(m_pObj)
	{
		XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
		pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
		pCellSvr->m_pGridFrameSvrs->EvtChar(pEvent,lParam);
		return true;
	}
*/
	return true;
}

void XWordView::CalcBlockExtent(vord_t pBlock)
{
	XWordLayerPara layer;
	layer.SetFRect(m_Page.m_rcPage);
	layer.SetContentRect(m_Page.m_rcPageContent);
	layer.set_block(pBlock.get_parent());
	HDC hDC = GetDC(GetHWND());
	layer.CalcExtent((XWordDataSvr*)m_pDataSvr ,pBlock.get_parent(),
		0,hDC);
	ReleaseDC(GetHWND(),hDC);
}

bool XWordView::DeleteChar(XWCursorInfo & CursorInfo)
{
	if(GetReadOnly()) return 1;

	m_Selection.bSelection = false;
	vord_t pBlock = CursorInfo.pBlock;
	if(!pBlock) return false;
	int nPosChar;
	nPosChar = CursorInfo.nIndex;

	int nCol = CursorInfo.nCol; 
	int nRow = CursorInfo.nRow;
	int nColCount = GetColCount(nRow);

	if(m_Selection.endCol!=m_Selection.startCol ||
		m_Selection.startRow != m_Selection.endRow)
	{
		return DeleteSelection(CursorInfo);
	}

	LPTSTR pStr =pBlock[text::content];
	if(!pStr ||(int)::_tcslen(pStr)== nPosChar + 1)
	{
		if(!pBlock.get_next())
		{
			vord_t p = pBlock.get_parent();
			if(p.get_next() && !p.get_next()[control::type])
			{
				vord_t s = p.get_parent();
				vord_t w = p.get_next();
				vord_t::mergeChild(w,p);
				s[extent_c::cy] -= w[extent_c::cy] + 6;
				s.removeChild(w);
				CalcBlockExtent(pBlock);
			}

			CalcBlockExtent(pBlock);
			m_pages = CalcPages(); 
			CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol);
			::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
			ClampInView(CursorInfo);
			ShowCurrentCaret(true);
			return true;
		}
	}

	
	if(pStr && (int)::_tcslen(pStr) > nPosChar +1)
	{
		if(pStr[nPosChar + 1]=='\r' && (int)::_tcslen(pStr)> nPosChar + 2 && pStr[nPosChar + 2]=='\n')
			::_tcscpy_s(pStr + nPosChar + 1, _tcslen(pStr) - (nPosChar + 1), pStr + nPosChar + 3 );
		else
			::_tcscpy_s(pStr + nPosChar + 1, _tcslen(pStr) - (nPosChar + 1), pStr + nPosChar + 2);
	}else
	{
		while(!pStr || (int)::_tcslen(pStr)<= nPosChar + 1)
		{
			pBlock = pBlock.get_next();
			if(!pBlock) return false;
			pStr = pBlock[text::content];
			nPosChar = -1;
		}
		::_tcscpy_s(pStr + nPosChar + 1, _tcslen(pStr) - (nPosChar + 1),
			pStr + nPosChar + 2 );
	}

	CalcBlockExtent(pBlock);
	if(pStr[0]=='\0' && (pBlock.get_piror() || pBlock.get_next()))
	{
		vord_t p = pBlock.get_parent();
		p.removeChild(pBlock);
		//when block is last, remove para block
		if(!p.get_child())
		{
			vord_t s = p.get_parent();
			if(!(s.get_child()==p && !p.get_next())) //not delete last block
			{
				s[extent_c::cy] -= p[extent_c::cy] + 6;
				s.removeChild(p);
			}
		}
	}
	m_pages = CalcPages(); 

	CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol);
	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	ClampInView(CursorInfo);
	ShowCurrentCaret(true);

	return true;
}

bool XWordView::DeleteSelection(XWCursorInfo & CursorInfo)
{
	if(GetReadOnly()) return 1;

	if(m_Selection.endCol==m_Selection.startCol && 
		m_Selection.startRow == m_Selection.endRow) return true;
	
	vord_t start;
	vord_t last;
	StandOnlySelection(m_Selection,start,last);

	bool bmerge = false;
	vord_t from;
	vord_t to;
	if(start.get_parent()!=last.get_parent()
		&& start.get_piror() 
		&& last.get_next())
	{
		from = start.get_piror();
		to = last.get_next();
		bmerge = true;
	}
	
	bool blast = false; 
	if(start == last) blast = true;
	while(start && (start!= last || blast) )
	{
		vord_t p = start.get_parent();
		vord_t t = start.get_next();
		if(start[control::type])
		{
			start.get_parent()[extent_c::cy] -= start[extent_c::cy];
			p = vord_t();
		}
		else
		{
			start.get_parent().removeChild(start);
		}
		if(!p)
		{
			continue;
			start = t;
		}

		//when is last not to continue
		if(blast)
		{
			if(p.get_child())
			{
				CalcBlockExtent(p.get_child());
				p.get_parent()[extent_c::cy] -= p[extent_c::cy];
			}else
			{
				p.get_parent()[extent_c::cy] -= p[extent_c::cy];
				p.get_parent().removeChild(p);
			}
			break;
		}

		if(!t)
		{
			if(p.get_child())
			{
				CalcBlockExtent(p.get_child());
				p.get_parent()[extent_c::cy] -= p[extent_c::cy];
				p = p.get_next();
			}else
			{
				p.get_parent()[extent_c::cy] -= p[extent_c::cy];
				vord_t q = p.get_next();
				p.get_parent().removeChild(p);
				p = q;
			}
			while(p && p!=last.get_parent())
			{
				p.get_parent()[extent_c::cy] -= p[extent_c::cy];
				vord_t q = p.get_next();
				p.get_parent().removeChild(p);
				p = q;
			}
			start = p.get_child(); 
		}else
			start = t;
		if(start==last) blast = true;
	}
	if(bmerge)
	{
		vord_t::mergeChild(from,to);
		CalcBlockExtent(from);
	}

	CursorInfo.nRow = m_Selection.startRow;
	CursorInfo.nCol = m_Selection.startCol;
	AdjustSelection(CursorInfo);

	CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol);

	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	ClampInView(CursorInfo);
	ShowCurrentCaret(true);

	return true;
}

bool XWordView::BackDeleteChar(XWCursorInfo & CursorInfo)
{
	if(GetReadOnly()) return 1;

	//m_Selection.bSelection = false;
	vord_t pBlock = CursorInfo.pBlock;
	if(!pBlock) return false;

	if(m_Selection.endCol!=m_Selection.startCol ||
		m_Selection.startRow != m_Selection.endRow)
	{
		return DeleteSelection(CursorInfo);
	}else
	{
		int nPosChar;
		nPosChar = CursorInfo.nIndex;
		if(nPosChar==-1)
		{
			while(pBlock.get_piror())
			{
				pBlock = pBlock.get_piror();
				wchar_t* pStr = pBlock[text::content];
				if(pStr && pStr[0])
				{
					nPosChar = (int)wcslen(pStr) - 1;
					break;
				}
			}
		}
		CalcLeftCursorInfo(CursorInfo);

		if(nPosChar >= 0)
		{
			wchar_t* pStr = pBlock[text::content];
			if(pStr && nPosChar >=0)
			{
				if(!pStr[nPosChar] && nPosChar > 0) nPosChar--;

				if(pStr[nPosChar]=='\n' && nPosChar - 1 >=0 && pStr[nPosChar - 1]=='\r')
				{
					::_tcscpy_s(pStr + nPosChar - 1, _tcslen(pStr + nPosChar + 1) + 1, pStr + nPosChar + 1 );
					nPosChar -= 2;
				}else
				{
					::_tcscpy_s(pStr + nPosChar , _tcslen(pStr + nPosChar + 1) + 1, pStr + nPosChar + 1 );
					nPosChar--;
				}
				CalcBlockExtent(pBlock);
				m_pages = CalcPages();
			}
		}else
		{
			if(!pBlock[text::content][0])
			{
				int ext = pBlock[extent_c::cy];
				vord_t p = pBlock.get_parent();
				p.removeChild(pBlock);
				if(!p.get_child())
				{
					p.get_parent()[extent_c::cy] -= ext + 6;
					p.get_parent().removeChild(p);
					m_pages = CalcPages();
				}else
				{
					CalcBlockExtent(p.get_child());
					m_pages = CalcPages();
				}
			}else //merge
			{
				vord_t p = pBlock.get_parent();
				vord_t s = p.get_parent();

				vord_t p1 = pBlock.get_parent().get_child();
				wchar_t* pStr = p1[text::content];

				vord_t pr = pBlock.get_piror();
				while(pr && !pr[text::content][0])
				{
					pr.get_parent().removeChild(pr);
					pr = pBlock.get_piror();
				}
				if(pBlock.get_parent().get_child().equal(pBlock))
				{
					if(p.get_piror() && !p.get_piror()[control::type])
					{
						vord_t w = p.get_piror();
						vord_t::mergeChild(p,p.get_piror());
						s[extent_c::cy] -= p[extent_c::cy] + 6;
						s.removeChild(p);
						CalcBlockExtent(w.get_child());
					}
				}
			}
		}
	}
	CalcCursorInfo(CursorInfo,CursorInfo.nRow,CursorInfo.nCol);

	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	ClampInView(CursorInfo);
	ShowCurrentCaret(true);

	return true;
}

bool XWordView::InsertChar(XWCursorInfo & CursorInfo,TCHAR ch)
{
	if(GetReadOnly()) return 1;

	//!m_Selection.bSelection = false;

	//if(ch=='\r')ch=0xB;
	if(ch=='\r')
	{
		StandOnlyToPara(CursorInfo.nRow,CursorInfo.nCol);
		m_pages = CalcPages();
		::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
		this->CalcRightCursorInfo(CursorInfo);
		AdjustSelection(CursorInfo);
		ShowCurrentCaret(true);
		return true;
	}
	
	int nPosChar = CursorInfo.nIndex;
	vord_t pTextBlock = CursorInfo.pBlock;
	if(!pTextBlock) return false;
	
	LPTSTR pStr = pTextBlock[text::content];
	int nLimit = pTextBlock[text::limit];

	LPTSTR pStrTemp = NULL;
	int nStrLen = (int)(pStr?_tcslen(pStr):0);
	if(!pStr)
	{
		nLimit = 35;
		pStr = new TCHAR[nLimit];
		pStr[0]='\0';
		pTextBlock[text::limit] = nLimit;
		pTextBlock[text::content] = pStr;
	}else
	{
		if(nStrLen + 3 >= nLimit)
		{
			if(nLimit == 0)
				nLimit =(int) nStrLen + 3 + 32;
			else
				nLimit += 32;
			pStrTemp = new TCHAR[nLimit];
			pTextBlock[text::limit] = nLimit;
		}
		
		if(pStrTemp)
		{
			::_tcscpy_s(pStrTemp,nLimit,pStr);
			delete pStr;
			pStr = pStrTemp;
			pTextBlock[text::content] = pStr;
		}
	}
	if(ch!=0x0B)
	{
		if(!pStr[nPosChar])
		{
			pStr[nPosChar + 1] = 0;
			pStr[nPosChar] = ch;
		}else
		{
			int nLen = (int)::_tcslen(pStr + nPosChar + 1);
			while(nLen >= 0)
			{
				if(nLen + nPosChar + 2 >= nLimit)
				{
					int k = 1;
				}
				pStr[nLen + nPosChar + 2] = pStr[nLen + nPosChar +1];
				nLen--;
			}
			pStr[nPosChar + 1] = ch;
		}
	}
	else
	{
		if(!pStr[nPosChar])
		{
			pStr[nPosChar] = '\r';
			pStr[nPosChar + 1] = '\n';
			pStr[nPosChar + 2] = 0;
		}
		else
		{
			int nLen = (int)::_tcslen(pStr + nPosChar);
			while(nLen>= 0)
			{
				if(nLen + nPosChar + 3 >= nLimit)
				{
					int k = 1;
				}
				pStr[nLen + nPosChar + 3] = pStr[nLen + nPosChar +1];
				nLen--;
			}
			pStr[nPosChar + 1] = '\r';
			pStr[nPosChar + 2] = '\n';
		}
	}
	CalcBlockExtent(pTextBlock); 
	m_pages = CalcPages();
	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	CalcRightCursorInfo(CursorInfo);
	ClampInView(CursorInfo);
	ShowCurrentCaret(true);
	return true;
}

bool XWordView::AdjustSelection(XWCursorInfo & CursorInfo)
{
	TShiftState Shift = 0;
	if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
	if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
	if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;
	return AdjustSelection(CursorInfo,Shift);
}

bool XWordView::AdjustSelection(XWCursorInfo& CursorInfo,TShiftState state)
{
	bool bRedraw = false;
	if(state & ssShift)
	{
		m_Selection.bSelection = true;
		int nResult = -1;
		if(!m_Selection.bLast)
		{
			if(m_Selection.startRow > CursorInfo.nRow ||
				(m_Selection.startRow == CursorInfo.nRow && m_Selection.startCol > CursorInfo.nCol))
			nResult = 1;
		}else
		{
			if(m_Selection.endRow > CursorInfo.nRow ||
				(m_Selection.endRow == CursorInfo.nRow && m_Selection.endCol > CursorInfo.nCol))
				nResult = 1;
		}
		if(nResult < 0)
		{
			if(m_Selection.bLast)
			{
				m_Selection.startRow = m_Selection.endRow;
				m_Selection.startCol = m_Selection.endCol;
				m_Selection.bLast = false;
			}
			m_Selection.endRow =  CursorInfo.nRow;
			m_Selection.endCol = CursorInfo.nCol;
		}
		if(nResult > 0)
		{
			if(!m_Selection.bLast)
			{
				m_Selection.endRow = m_Selection.startRow;
				m_Selection.endCol = m_Selection.startCol;
				m_Selection.bLast  = true;
			}
			m_Selection.startRow =  CursorInfo.nRow;
			m_Selection.startCol = CursorInfo.nCol;
		}
		::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	}else
	{
		if(m_Selection.bSelection)bRedraw =true;
		m_Selection.startRow =  CursorInfo.nRow;
		m_Selection.startCol = CursorInfo.nCol;
		m_Selection.endRow	= m_Selection.startRow;
		m_Selection.endCol = m_Selection.startCol;
		::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	}

	if(m_Selection.startRow == m_Selection.endRow && m_Selection.startCol == m_Selection.endCol)m_Selection.bSelection = false;
	
	return true;
}

bool XWordView::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
{
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;

	UINT nChar = pEvent->key.nChar; 
	if(m_pSelectionBlocks->size()>0)
	{
		if(nChar==VK_DELETE)
		{
			for(int i=0;i<(int)(*m_pSelectionBlocks).size();i++)
			{
				int ext  = (*m_pSelectionBlocks)[i][extent_c::cy];
				vord_t p = (*m_pSelectionBlocks)[i].get_parent();
				p[extent_c::cy] -= ext + 6;
				p.removeChild((*m_pSelectionBlocks)[i]);
			}
			m_pages = CalcPages();
			::InvalidateRect(GetHWND(),NULL,false);
			return true;
		}

		int cy = 0;
		int cx = 0;
		int my = 0;
		int mx = 0;

		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE)
		{
			if(nChar==VK_LEFT) cx = -2;
			else if(nChar==VK_RIGHT) cx = 2;
			else if(nChar==VK_UP) cy = -2;
			else if(nChar==VK_DOWN) cy = 2;
		}else
		{
			if(nChar==VK_LEFT) mx = -2;
			else if(nChar==VK_RIGHT) mx = 2;
			else if(nChar==VK_UP) my = -2;
			else if(nChar==VK_DOWN) my = 2;
		}
		if(cy)
		{
			for(int i=0;i<(int)(*m_pSelectionBlocks).size();i++)
			{
				(*m_pSelectionBlocks)[i][control::bottom]  += cy;
				(*m_pSelectionBlocks)[i][extent_c::cy] += cy;
				(*m_pSelectionBlocks)[i].get_parent()[extent_c::cy] +=cy;
			}

			::InvalidateRect(GetHWND(),NULL,false);
		}
		if(cx)
		{
			for(int i=0;i<(int)(*m_pSelectionBlocks).size();i++)
			{
				(*m_pSelectionBlocks)[i][control::right]  += cx;
			}
			::InvalidateRect(GetHWND(),NULL,false);
		}
		if(mx)
		{
			for(int i=0;i<(int)(*m_pSelectionBlocks).size();i++)
			{
				(*m_pSelectionBlocks)[i][control::left]  += mx;
				if((*m_pSelectionBlocks)[i][control::right])
					(*m_pSelectionBlocks)[i][control::right]  += mx;
			}
			::InvalidateRect(GetHWND(),NULL,false);
		}
		if(my)
		{
			for(int i=0;i<(int)(*m_pSelectionBlocks).size();i++)
			{
				(*m_pSelectionBlocks)[i][control::top]  += my;
				if((*m_pSelectionBlocks)[i][control::bottom])
					(*m_pSelectionBlocks)[i][control::bottom]  += my;
				(*m_pSelectionBlocks)[i][extent_c::cy] += my;
				(*m_pSelectionBlocks)[i].get_parent()[extent_c::cy] +=my;
			}
			::InvalidateRect(GetHWND(),NULL,false);
		}
		return true;
	}

	switch (pEvent->key.nChar) 
	{
	case VK_LEFT:       // Left arrow
		{
			if(pDataSvr->m_pContentSvr)
			{
				if(CalcLeftCursorInfo(m_CursorInfo))
				{
					ClampInView(m_CursorInfo);
					AdjustSelection(m_CursorInfo);
					ShowCurrentCaret(true);
				}
			}
		}
		break; 
	case VK_RIGHT:      // Right arrow 
		{
			if(pDataSvr->m_pContentSvr)
			{
				if(CalcRightCursorInfo(m_CursorInfo))
				{
					ClampInView(m_CursorInfo);
					AdjustSelection(m_CursorInfo);
					ShowCurrentCaret(true);
				}
			}
		}
		break; 

	case VK_UP:         // Up arrow 
		{
			if(pDataSvr->m_pContentSvr)
			{
				if(CalcUpCursorInfo(m_CursorInfo))
				{
					ClampInView(m_CursorInfo);
					AdjustSelection(m_CursorInfo);
					ShowCurrentCaret(true);
				}
			}
		}
		break; 

	case VK_DOWN:       // Down arrow 
		{
			if(pDataSvr->m_pContentSvr)
			{
				if(CalcDownCursorInfo(m_CursorInfo))
				{
					ClampInView(m_CursorInfo);
					AdjustSelection(m_CursorInfo);
					ShowCurrentCaret(true);
				}
			}
		}
		break; 

	case 'V': //paste
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
		{
			DoPaste();
		}
		break;
	case 'C': //copy
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
		{
			DoCopy();
		}
		break;
	case 'X': //cut
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
		{
			DoCut();
		}
		break;
	case VK_HOME: // Home 
		CalcCursorInfo(m_CursorInfo,1,1);
		ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,0);
		break; 
	case VK_END:        // End 
		ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,100);
		break; 

	case VK_PRIOR:      // Page Up 
		ModifyScrollBar(SB_VERT,SB_PAGEUP,0);
		break; 

	case VK_NEXT:       // Page Down 
		ModifyScrollBar(SB_VERT,SB_PAGEDOWN,0);
		break;

	case VK_DELETE:     // Delete
		DeleteChar(m_CursorInfo);
		//ClampInView(m_pFlowDocument->m_CursorInfo);
		break; 

	default:
		break;
	}

#ifdef Vx
	switch (pEvent->key.nChar) 
	{
	case 'V': //paste
		if(m_pWordLayer)
		{
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
			{
				m_pWordLayer->DoPaste();
			}
		}
		break;
	case 'C': //paste
		if(m_pWordLayer)
		{
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
			{
				m_pWordLayer->DoCopy();
			}
		}
		break;
	case 'X': //paste
		if(m_pWordLayer)
		{
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
			{
				m_pWordLayer->DoCut();
			}
		}
		break;
	case VK_HOME:       // Home 
		if(m_pWordLayer)
		{
			m_pWordLayer->CalcCursorInfo(m_pWordLayer->m_CursorInfo,0,0);
			m_pWordLayer->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,0);
		}
		break; 
	case VK_END:        // End 
		if(m_pWordLayer)
		{
			m_pWordLayer->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,100);
		}
		break; 

	case VK_PRIOR:      // Page Up 
		if(m_pWordLayer)
		{
			m_pWordLayer->ModifyScrollBar(SB_VERT,SB_PAGEUP,0);
		}
		break; 

	case VK_NEXT:       // Page Down 
		if(m_pWordLayer)
		{
			m_pWordLayer->ModifyScrollBar(SB_VERT,SB_PAGEDOWN,0);
		}
		break; 
	case VK_LEFT:       // Left arrow
		{
			if(m_pWordLayer)
			{
				XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
				if(m_pWordLayer->CalcLeftCursorInfo(CursorInfo))
				{
					m_pWordLayer->m_CursorInfo = CursorInfo;
					m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
					AdjustSelection(m_pWordLayer->m_CursorInfo);
				}
			}
		}
		break; 
	case VK_RIGHT:      // Right arrow 
		{
			if(m_pWordLayer)
			{
				XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
				if(m_pWordLayer->CalcRightCursorInfo(CursorInfo))
				{
					m_pWordLayer->m_CursorInfo = CursorInfo;
					m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
					AdjustSelection(m_pWordLayer->m_CursorInfo);
				}
			}
		}
		break; 

	case VK_UP:         // Up arrow 
		{
			if(m_pWordLayer)
			{
				XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
				if(m_pWordLayer->CalcUpCursorInfo(CursorInfo))
				{
					m_pWordLayer->m_CursorInfo = CursorInfo;
					m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
					AdjustSelection(m_pWordLayer->m_CursorInfo);
				}
			}
		}
		break; 

	case VK_DOWN:       // Down arrow 
		{
			if(m_pWordLayer)
			{
				XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
				if(m_pWordLayer->CalcDownCursorInfo(CursorInfo))
				{
					m_pWordLayer->m_CursorInfo = CursorInfo;
					m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
					AdjustSelection(m_pWordLayer->m_CursorInfo);
				}
			}
		}
		break; 

	case VK_DELETE:     // Delete
		m_pWordLayer->DeleteChar(m_pWordLayer->m_CursorInfo);
		m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
		break; 
	}
#elif V3 
	if(m_pObj)
	{
		XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
		pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
		pCellSvr->m_pGridFrameSvrs->EvtKeyDown(pEvent,lParam);
		return true;
	}
	switch (pEvent->key.nChar) 
	{
	case 'V': //paste
		if(m_pFlowDocument)
		{
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
			{
				m_pFlowDocument->DoPaste();
			}
		}
		break;
	case 'C': //paste
		if(m_pFlowDocument)
		{
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
			{
				m_pFlowDocument->DoCopy();
			}
		}
		break;
	case 'X': //paste
		if(m_pFlowDocument)
		{
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
			{
				m_pFlowDocument->DoCut();
			}
		}
		break;
	case VK_HOME:       // Home 
		if(this->m_pFlowDocument)
		{
			//m_pWordLayer->CalcCursorInfo(m_pWordLayer->m_CursorInfo,0,0);
			this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,0);
		}
		break; 
	case VK_END:        // End 
		if(this->m_pFlowDocument)
		{
			this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,100);
		}
		break; 

	case VK_PRIOR:      // Page Up 
		if(this->m_pFlowDocument)
		{
			this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_PAGEUP,0);
		}
		break; 

	case VK_NEXT:       // Page Down 
		if(this->m_pFlowDocument)
		{
			this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_PAGEDOWN,0);
		}
		break; 
	case VK_LEFT:       // Left arrow
		{
			if(m_pFlowDocument)
			{
				if(m_pFlowDocument->CalcLeftCursorInfo(m_pFlowDocument->m_CursorInfo))
				{
					m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
					AdjustSelection(m_pFlowDocument->m_CursorInfo);
					m_pFlowDocument->ShowCurrentCaret(true);
				}
			}
		}
		break; 
	case VK_RIGHT:      // Right arrow 
		{
			if(m_pFlowDocument)
			{
				if(m_pFlowDocument->CalcRightCursorInfo(m_pFlowDocument->m_CursorInfo))
				{
					m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
					AdjustSelection(m_pFlowDocument->m_CursorInfo);
					m_pFlowDocument->ShowCurrentCaret(true);
				}
			}
		}
		break; 

	case VK_UP:         // Up arrow 
		{
			/*
			if(m_pWordLayer)
			{
			XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
			if(m_pWordLayer->CalcUpCursorInfo(CursorInfo))
			{
			m_pWordLayer->m_CursorInfo = CursorInfo;
			m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
			AdjustSelection(m_pWordLayer->m_CursorInfo);
			}
			}*/
		}
		break; 

	case VK_DOWN:       // Down arrow 
		{
			/*
			if(m_pWordLayer)
			{
			XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
			if(m_pWordLayer->CalcDownCursorInfo(CursorInfo))
			{
			m_pWordLayer->m_CursorInfo = CursorInfo;
			m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
			AdjustSelection(m_pWordLayer->m_CursorInfo);
			}
			}*/
		}
		break; 

	case VK_DELETE:     // Delete
		if(m_pFlowDocument)
		{
			m_pFlowDocument->DeleteChar(m_pFlowDocument->m_CursorInfo);
			m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
		}

		break; 
	} 
#endif
	return true;
}

int XWordView::GetPageAtRow(int nRow)
{
	int row;
	int paraRow;
	vord_t  block;

	int pages = m_pages==-1?CalcPages():m_pages;
	for(int i=2;i<=pages;i++)
	{
		if(GetFirstRowOfPage(i,row,paraRow,block))
		{
			if(row>nRow) return i-1;
		}
	}
	return pages;
}

int XWordView::GetPageRect(int nPage,RECT &rt)
{
	RECT rc;
	::GetClientRect(GetHWND(),&rc);

	RECT rcPage;
	if( rc.right - rc.left < m_Page.GetWidth())
		rcPage.left = rc.left + 5;
	else
		rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_Page.GetWidth())/2;
	
	rcPage.top = rc.top + 20;
	rcPage.right = rcPage.left + m_Page.GetWidth();
	rcPage.bottom = rcPage.top + m_Page.GetHeight();

	rt = rcPage;
	::OffsetRect(&rt,0,(nPage - 1)*(m_Page.GetHeight()+20));

	return 1;
}

int XWordView::GetPageAtPoint(POINT pt)
{
	int nPage = 1;
	int height = m_Page.GetHeight() + 20;
	while(pt.y > height)
	{
		pt.y -= height;
		nPage++;
	}
	return nPage;
}

int XWordView::GetRowAtPoint(POINT pt)
{
	int nPage = GetPageAtPoint(pt);
	if(nPage < 1) return -1;

	int nRow;
	int nParaRow;
	vord_t  item;
	if(GetFirstRowOfPage(nPage,nRow,nParaRow,item)) return nRow;

	return -1;
}

int XWordView::CalcPages()
{
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;
	int height = 0;
	if(pDataSvr->m_pContentSvr)
		height = pDataSvr->m_pContentSvr->document[extent_c::cy];
	int hpage = m_Page.m_rcPageContent.bottom - m_Page.m_rcPageContent.top;
	int pages = height/hpage ;
	if(pages < 1) pages = 1;
	if(pages * hpage < height) pages++;

	int  nRow;
	int  paraRow;
	vord_t  pParaBlock;

	if(GetFirstRowOfPage(pages + 1,nRow,paraRow, pParaBlock))pages++;

	return pages;
}

bool XWordView::DoDrawPage(HDC hPaintDC,int nPage,POINT ptOffset,RECT * pDrawRect,RECT * pUpdateRect)
{
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;
	if(pDataSvr->m_pContentSvr)
	{
		XWordLayerDocument doc;
		doc.set_block(pDataSvr->m_pContentSvr->document);
		::SetRect(&doc.FRect,ptOffset.x,ptOffset.y,
			m_Page.m_rcPageContent.right - m_Page.m_rcPageContent.left + ptOffset.x,
			m_Page.m_rcPageContent.bottom - m_Page.m_rcPageContent.top + ptOffset.y);
		doc.FContentRect = m_Page.m_rcPageContent;

		if(!doc.GetExtent().cy)
			doc.CalcExtent((XWordDataSvr* )m_pDataSvr,doc.get_block(),NULL,hPaintDC);

		POINT ptPageOffset = {0,0};

		//draw page
		DrawPage(hPaintDC,nPage,ptPageOffset,& doc.FRect,pDrawRect,pUpdateRect);
		//doc.DoDraw((XWordDataSvr* )m_pDataSvr,NULL,hPaintDC,ptPageOffset,pDrawRect,pUpdateRect); 
	}
	return true;
}

bool XWordView::IsSelected(vord_t pBlock)
{
	for(int i=0;i < (int)m_pSelectionBlocks->size();i++)
	{
		if((*m_pSelectionBlocks)[i].equal(pBlock)) return true;
	}
	return false;
}

RECT XWordView::GetImageBoundRect(SIZE sz,RECT &rt)
{
	RECT rcImage = GetImageRect(sz,rt);
	RECT rc = rt;
	if(rc.right - rc.left < rcImage.right - rcImage.left)
	{
		rc.left = rcImage.left;
		rc.right = rcImage.right;
	}
	if(rc.bottom - rc.top < rcImage.bottom - rcImage.top)
	{
		rc.top = rcImage.top;
		rc.bottom = rcImage.bottom;
	}
	return rc;
}

RECT XWordView::GetImageRect(SIZE sz,RECT &rt)
{
	int nWidth  = sz.cx;
	int nHeight = sz.cy;

	RECT rc;
	if(rt.right  - rt.left < 1) rt.right = rt.left + nWidth;
	if(rt.bottom - rt.top < 1) rt.bottom = rt.top + nHeight;

	if(nWidth > 0)
	{
		if(1.0*(rt.right - rt.left)/nWidth*nHeight > rt.bottom - rt.top)
		{
			SetRect(&rc,rt.left,rt.top,
				rt.left + (int)1.0 *(rt.bottom - rt.top) * nWidth /nHeight ,
				rt.bottom);
		}else
			SetRect(&rc,rt.left,rt.top,rt.right ,
				rt.top + (int)1.0 *(rt.right - rt.left) * nHeight /nWidth);
	}
	return rc;
}

bool XWordView::EvtNCHitTest(TEvent *pEvent ,LPARAM lParam)
{
	POINT pt = pEvent->mouse.point;
	ScreenToClient(GetHWND(),&pt);
	m_ptHittest = pt;
	return true;
}

bool XWordView::EvtSetCursor(TEvent *pEvent ,LPARAM lParam)
{
	HCURSOR Cur = NULL;
	if (pEvent->cursor.hWnd == GetHWND() && pEvent->cursor.nHitTest==HTCLIENT )
	{
		int htPos = HitPosition::none;
		if((m_State & 0xffff) == TWordState_ObjectResizing)
			htPos = this->m_hitPos;
		else
		{
			POINT pt = m_ptHittest;
			pt.x += m_ptScroll.x;
			pt.y += m_ptScroll.y;
			for(int i=0;i< (int)(*m_pSelectionBlocks).size();i++)
			{
				RECT rt = {(*m_pSelectionBlocks)[i][draw::left],
					(*m_pSelectionBlocks)[i][draw::top],
					(*m_pSelectionBlocks)[i][draw::right],
					(*m_pSelectionBlocks)[i][draw::bottom]};
				htPos = GetHitResizePat(pt,rt,2);
				if(htPos!=HitPosition::none)break;
			}
		}
		if(htPos!=HitPosition::none)
		{
			if(htPos==HitPosition::lefttop || htPos==HitPosition::rightbottom)
				Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENWSE));//crVSplit
			if(htPos==HitPosition::leftbottom || htPos==HitPosition::righttop)
				Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENESW));//crVSplit
			if(htPos==HitPosition::leftcenter || htPos==HitPosition::rightcenter)
				Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));//crVSplit
			if(htPos==HitPosition::bottomcenter || htPos==HitPosition::topcenter)
				Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));//crVSplit
		}
	}
	if(Cur != NULL)
	{
		SetCursor(Cur);
		pEvent->dwRet = TRUE;
	}else
	{
		Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW));
		SetCursor(Cur);
		pEvent->dwRet = TRUE;
	}
	return true;
}

int	 XWordView::DrawParaControl(HDC hPaintDC,vord_t item,POINT ptOffset,RECT * contentRect, RECT * pDrawRect,RECT * pUpdateRect)
{
	if(item[control::type])
	{
		RECT rt ={item[control::left],item[control::top],
			item[control::right],item[control::bottom]};
		::OffsetRect(&rt,contentRect->left,contentRect->top);
		::OffsetRect(&rt,ptOffset.x, ptOffset.y);


		if(!item[control::object])
		{
			xbObject *	pObject;
			Hxsoft::XFrame::xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,item[control::type],pObject);
			Hxsoft::XFrame::xfControl*  pxfControl = (Hxsoft::XFrame::xfControl *)pObject;
			if(pxfControl)
			{
				/*
				RECT rect;
				::SetRect(&rect,0,0,0,0);
				pxfControl->m_pxfNode= NULL;
				pxfControl->m_dwStyle &= ~WS_VISIBLE;
				pxfControl->CreateControl(NULL,rect,GetDesktopWindow(),0,xfApp::GetApp()->GetInstance());
				::SetProp(pxfControl->m_hWnd,_T("this"),pxfControl);

				XOfficeSheet * pSheet = dynamic_cast<XOffice::XOfficeSheet *>(pxfControl);
				if(pSheet)
				{
					pSheet->m_Standalone = true;
					_variant_t v;
					IXMLDOMElement * pElement = (IXMLDOMElement*)item[control::element];
					pElement->getAttribute(L"src",&v);
					if(v.bstrVal && v.bstrVal[0])
					{
						xbXml x;
						IXMLDOMElement * pObjElement = NULL;
						if(xfUriLoader::FetchXml(NULL,v.bstrVal,NULL,x)==1)
						{
							x.GetXmlDoc()->get_documentElement(&pObjElement);
							pSheet->m_pSheetSvr->LoadTemplate(pObjElement,goRunState);
							pObjElement->Release();
						}
					}else
					{
						pSheet->m_pSheetSvr->LoadTemplate(pElement,goRunState);
					}
				}
				*/

				if(_tcsicmp(item[control::type],L"ximage")==0)
				{
					pxfControl->Initial((IXMLDOMElement*)item[control::element]);
				}
				else
					pxfControl->Initial(); 

				item[control::object] = pxfControl;
			}
		}
		SIZE sz = {0,0};
		if(IsControl(item,L"xcell"))
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)item[control::object];
			if(pCellSvr)
			{
				RECT rc;
				RECT ert;
				::SetRect(&rc,0,0,
					item[control::right] - item[control::left] - 10,
					item[control::bottom] - item[control::top] - 10);
				pCellSvr->SetFRect(rc);
				::SetRect(&ert,item[control::left],
					item[control::top],
					item[control::right] - 10,
					item[control::bottom] - 10);
				::OffsetRect(&ert,rt.left,rt.top);
				pCellSvr->SetOffset(*(POINT *)&ert);
				pCellSvr->DoDraw(hPaintDC,&ert);
			}
		}else
		{
			sz = ((Hxsoft::XFrame::xfControl *)item[control::object])->GetContentExtent();
			((Hxsoft::XFrame::xfControl*)item[control::object])->DoDraw(hPaintDC,&rt);
		}

		
		RECT rc = rt;
		if(IsControl(item,L"ximage")) 
			rc = GetImageBoundRect(sz,rt);
		item[draw::x] = rc.left - item[control::left];
		item[draw::y] = rc.top - item[control::top];
		item[draw::left] = rc.left ;
		item[draw::top]  = rc.top ;
		item[draw::right] = rc.right;
		item[draw::bottom]  = rc.bottom;

		if(IsSelected(item)) 
		{
			HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
			hPen = (HPEN)SelectObject(hPaintDC,hPen);
			
			POINT pt;
			//topleft
			pt.x = rc.left,	pt.y = rc.top;
			::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
			//topright
			pt.x = rc.right,	pt.y = rc.top;
			::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
			//bottomleft
			pt.x = rc.left,	pt.y = rc.bottom;
			::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);
			//bottomright
			pt.x = rc.right,	pt.y = rc.bottom;
			::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 3,pt.y + 3);

			hPen = (HPEN)SelectObject(hPaintDC,hPen);
			::DeleteObject(hPen);
		}
		if(rt.bottom - rt.top > 1)
			return  item[control::bottom] + 6;
		else
			return sz.cy + 6;
	}else
		return 0;
}

bool XWordView::DrawPage(HDC hPaintDC,int nPage,POINT ptOffset,RECT * contentRect,RECT * pDrawRect,RECT * pUpdateRect)
{
	int nParaRow;
	int nRow;
	vord_t  item;

	//bool nextPage
	if(!GetFirstRowOfPage(nPage,nRow,nParaRow,item)) return false;

	int hPage = contentRect->bottom - contentRect->top;
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;

	XWordDevice device;
	device.SetPaintDC(hPaintDC);
	XWCodeInter machine;
	XWCodePaintContext context(&device);
	context.pDataSvr = pDataSvr;

	int startX = contentRect->left;
	int startY = contentRect->top;

	context.pSelection = &m_Selection;
	
	context.ptPoint.x = startX;
	context.ptPoint.y = startY;
	//render
	context.GetDevice()->rcContentPage = contentRect;
	context.GetDevice()->startRow = nParaRow;
	context.GetDevice()->endRow = -1;

	context.nRow = nRow ;
	for(;item;item = item.get_next())
	{
		int pc = item[render::pc];
		unsigned char * code = item.get_parent()[render::code];
		if(item[control::type])
		{
			if(startY + item[extent_c::cy] > contentRect->bottom) return true;
			POINT pt = {context.ptPoint.x - contentRect->left,
				context.ptPoint.y - contentRect->top};

			startY +=  DrawParaControl(hPaintDC,item,pt, contentRect,pDrawRect,pUpdateRect);
		}else
		{

			if(code + pc)
			{
				machine.Inter(&context,code + pc);
				if(context.fullpage)break;
			}
			startY += item[extent_c::cy] + 6 - context.piror_ext;
		}

		context.nRow += item[render::rowcount];
		if(context.GetDevice()->startRow > 0)
			context.nRow -= context.GetDevice()->startRow - 1 ;

		context.ptPoint.x = startX;
		context.ptPoint.y = startY;
		context.GetDevice()->startRow = -1;
	}
	
	return true;
}

bool XWordView::GetFirstRowOfPage(int nPage,int & nRow,int & paraRow,vord_t & pParaBlock)
{
	int hPage = m_Page.m_rcPageContent.bottom - m_Page.m_rcPageContent.top;
	//int startP = hPage * (nPage - 1);
	int startY = 0;
	
	nRow = 1;
	paraRow = 0 ;
	pParaBlock = vord_t();
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;

	vord_t doc = pDataSvr->m_pContentSvr->document;
	vord_t item = doc.get_child().get_child();

	if(nPage<=1)
	{
		pParaBlock = item;
		nRow = 1;
		paraRow = 1;
		return true;
	}
	while(item)
	{
		int pc = item[render::pc];
		unsigned char * code = item.get_parent()[render::code];
		if(item[control::type])
		{
			nRow++;
			if(startY + item[extent_c::cy] > hPage )
			{
				nPage--;
				if(nPage==1)
				{
					pParaBlock = item;
					paraRow = 0;
					nRow--;
					return true;
				}else
					startY = item[extent_c::cy] + 6; 
			}else
			{
				startY += item[extent_c::cy] + 6; 
			}
			item = item.get_next();
		}else
		{
			bool found = false;
			if(paraRow > 1)
			{
				XWCodeInter machine;
				XWCodeContentExtent context;
				machine.Inter(&context,item.get_parent()[render::code] +
					item[render::pc]);
				int ext = 0;
				int rows = 0;
				for(int i = paraRow - 1; i < (int)context.rowHeights.size();i++)
				{
					if(ext + context.rowHeights[i] + startY < hPage)
					{
						rows++;
						ext += context.rowHeights[i];
					}
					else
					{
						found = true;
						break;
					}
				}
				nRow += rows;
				if(found)
				{
					paraRow = paraRow > 0 ? paraRow + rows  : rows+1 ;
					nPage--;
					if(nPage==1)
					{
						pParaBlock = item;
						return true;
					}else
						startY = 0;
				}
				else
				{
					paraRow = 0;
					startY +=  ext + 6;

					item = item.get_next();
				}
			}else
			{
				if(item[extent_c::cy] + startY > hPage)
				{
					XWCodeInter machine;
					XWCodeContentExtent context;
					machine.Inter(&context,item.get_parent()[render::code] +
						item[render::pc]);
					int ext = 0;
					int rows = 0;
					for(int i=0; i < (int)context.rowHeights.size();i++)
					{
						if(ext + context.rowHeights[i] + startY < hPage)
						{
							rows++;
							ext += context.rowHeights[i];
						}
						else
							break;
					}
					nRow += rows;
					paraRow = rows + 1;
					nPage--;
					if(nPage==1)
					{
						pParaBlock = item;
						return true;
					}else
						startY = 0;
				}else
				{
					startY += item[extent_c::cy] + 6;
					nRow += item[render::rowcount];
					paraRow = 0;

					item = item.get_next();
				}
			}
		}
	}
	return false;
}

int XWordView::DoPaste()
{
	if(GetReadOnly()) return 1;

	HANDLE        hClipGlobal;
	void *       lpbi;
	int         length;

	OpenClipboard(m_pSheetCtrl->m_hWnd);
	if( (hClipGlobal = GetClipboardData( CF_UNICODETEXT )) != NULL )
	{
		length = (int)GlobalSize(hClipGlobal);                         
		// Lock it down
		if( (lpbi=GlobalLock(hClipGlobal)) != NULL )
		{
			HANDLE hData;
			TCHAR* lpdata;
			hData = GlobalAlloc(0,length);
			lpdata = (TCHAR *)GlobalLock(hData);
			CopyMemory(lpdata,lpbi,length);
			int nDataLen = (LPTSTR)lpdata ?(int)wcslen((LPTSTR)lpdata):0;
			
			int nRow = m_Selection.startRow;
			int nCol = m_Selection.startCol;

			int index;
			vord_t block  = GetBlockAtRowColumn(nRow,nCol,index);

			int nLimit;
			int nStrLen;
			LPTSTR pStrTemp;
			LPTSTR pStr = block[text::content];
			if(!pStr)
			{
				pStr = new TCHAR[nDataLen + 1];
				::_tcscpy_s(pStr,nDataLen + 1,(LPTSTR)lpdata);
				block[text::content] = pStr;
			}else
			{
				nStrLen = (int)::_tcslen(pStr) + nDataLen + 1;
				nLimit = block[text::limit];
				if(nLimit == 0)
					nLimit =(int) nStrLen + 3 + 32;
				else
					nLimit += nDataLen + 1 + 32;
				pStrTemp = new TCHAR[nLimit];
				block[text::limit] = nLimit;

				if(pStrTemp)
				{
					::_tcsncpy_s(pStrTemp,nLimit,pStr,index+1);
					if(nDataLen)::_tcscpy_s(pStrTemp + index + 1,nDataLen + 1 ,(LPTSTR)lpdata);

					int nLen = (int)::_tcslen(pStr);
					::_tcscpy_s(pStrTemp + _tcslen(pStrTemp),::_tcslen(pStr) - index + 1 ,pStr + index + 1);
					delete pStr;
					pStr = pStrTemp;
					block[text::content] = pStr;
				}
			}
			if(block[text::pdx])
			{
				delete block[text::pdx];
				block[text::pdx] = 0;
			}
			

			GlobalUnlock(hData);
			GlobalFree(hData);

			GlobalUnlock( hClipGlobal );
			
			CalcBlockExtent(block); 
			m_pages = CalcPages();
			::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
			this->CalcRightCursorInfo(m_CursorInfo);
			ShowCurrentCaret(true);
		}
	}
	CloseClipboard();
	return 1;
}

int XWordView::DoCopy()
{
	if(GetReadOnly()) return 1;

	if(!m_Selection.bSelection) return 0; //should todo add new block

	TCHAR* lpdata;
	HGLOBAL        hClipGlobal;
	hClipGlobal = GlobalAlloc(0,128000);
	lpdata = (TCHAR *)GlobalLock(hClipGlobal);
	ZeroMemory(lpdata,128000);

	int nRow = m_Selection.startRow;
	int nCol = m_Selection.startCol;
	
	int nRow1 = m_Selection.endRow;
	int nCol1 = m_Selection.endCol;
	
	int index,index1;
	vord_t block  = GetBlockAtRowColumn(nRow,nCol,index);
	vord_t block1 = GetBlockAtRowColumn(nRow1,nCol1,index1);

	bool start = true;
	while(block && block1 && !block1.equal(block))
	{
		_tcscat_s(lpdata,64000,block[text::content] + index + 1);
		index = -1;

		//next block
		if(!block.get_next())
		{
			block = block.get_parent().get_next();
			if(block)
			{
				block = block.get_child();
				_tcscat_s(lpdata,64000,_T("\r\n"));
			}
		}else
			block = block.get_next();
	}
	if(block1.equal(block))
	{
		if(index1 >=0)
		{
			_tcsncat_s(lpdata+ _tcslen((TCHAR*)lpdata),64000 - _tcslen((TCHAR*)lpdata),
					block[text::content] + index + 1, index1 - index);
		}
	}

	GlobalUnlock(hClipGlobal);
	
	if(OpenClipboard(GetHWND()))
	{
		SetClipboardData(CF_UNICODETEXT,hClipGlobal);
		CloseClipboard();
	}
	
	return 1;
}

int XWordView::DoCut()
{
	if(GetReadOnly()) return 1;

	DoCopy();
	DeleteSelection(m_CursorInfo);

	return -1;
}

bool XWordView::EvtXCommand(TEvent *pEvent ,LPARAM lParam)
{
	LPTSTR	pStrID = pEvent->xcommand.pStrID;
	if(pStrID && _tcscmp(pStrID,_T("xmEditPaste"))==0)
	{
		DoPaste();
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmEditCopy"))==0)
	{
		DoCopy();
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmEditCut"))==0)
	{
		DoCut();
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatFont"))==0)
	{
		ChooseFontDlg();
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatItalic"))==0)
	{
		SetFontItalic();
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatUnderline"))==0)
	{
		SetFontUnderline();
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatBold"))==0)
	{
		SetFontBold();
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatTextColor"))==0)
	{
		ChooseColorDlg(STI_TEXTCOLOR);
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatTextBkColor"))==0)
	{
		ChooseColorDlg(STI_BKCOLOR);
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignCenter"))==0)
	{
		SetAlign(DT_CENTER);
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignRight"))==0)
	{
		SetAlign(DT_RIGHT);
		pEvent->dwRet = 1;
	}
	if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignLeft"))==0)
	{
		SetAlign(DT_LEFT);
		pEvent->dwRet = 1;
	}
	//if(_tcsstr(pEvent->xcommand.pStrID,_T("xmShapeImage")))//_T("xmInsertImage:")))
	if(_tcsstr(pEvent->xcommand.pStrID,_T("xmInsertImage:")))
	{
		int nRow = m_Selection.startRow;
		int nCol = m_Selection.startCol;
		int index;

		StandOnlyToPara(nRow,nCol);
		vord_t block  = GetBlockAtRowColumn(nRow + 1,1,index);
		XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;
		vord_t doc =  pDataSvr->m_pContentSvr->document;

		if(block)
		{
			vord_t paragraph(doc.groupid());
			block.get_parent().insertBefore(paragraph);

			paragraph.setItem(block::type,blocktype::control);
			paragraph.append(control::type,L"ximage");

			xbXml xml;
			xml.LoadXml(L"<p controlclass='ximage' x='10' height='300' width='600'/>");
			IXMLDOMElement * element;
			xml.GetXmlDoc()->get_documentElement(&element);
			element->setAttribute(L"src",(_variant_t)(_tcsstr(pEvent->xcommand.pStrID,_T(":"))+1));
			paragraph.append(control::element,element);

			xbObject *	pObject;
			Hxsoft::XFrame::xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,paragraph[control::type],pObject);
			Hxsoft::XFrame::xfControl*  pxfControl = (Hxsoft::XFrame::xfControl *)pObject;
			pxfControl->Initial(element);
			paragraph[control::object] = pxfControl;
			SIZE sz = pxfControl->GetContentExtent();
			RECT rc = {10,0,600,310};
			RECT rt = GetImageRect(sz,rc);

			int width =  rt.right - rt.left + 1;
			if(width < 10) width = 10;
			element->setAttribute(L"width",(_variant_t)width);
			int height = rt.bottom - rt.top + 1;
			if(height < 10) height = 10;
			element->setAttribute(L"height",(_variant_t)height);

			paragraph.append(control::left,(int)10);
			//paragraph.append(control::top,(int)10);
			paragraph.append(control::right,(int)10 + width);
			paragraph.append(control::bottom,(int)height);

			paragraph.append(extent_c::cy,height);
			paragraph.get_parent()[extent_c::cy] += height;
			paragraph.append(render::rowcount,1);

			::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
			ShowCurrentCaret(true);
		}

		pEvent->dwRet = 1;
	}
#if 0
	if(m_pObj)
	{
		XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
		pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
		pCellSvr->m_pGridFrameSvrs->EvtXCommand(pEvent,lParam);
		return true;
	}

	if(pStrID && _tcscmp(pStrID,_T("xmEditPaste"))==0)
	{
		m_pFlowDocument->DoPaste();
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmEditCopy"))==0)
	{
		m_pFlowDocument->DoCopy();
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmEditCut"))==0)
	{
		m_pFlowDocument->DoCut();
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatFont"))==0)
	{
		m_pFlowDocument->ChooseFontDlg();
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatItalic"))==0)
	{
		m_pFlowDocument->SetFontItalic();
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatUnderline"))==0)
	{
		m_pFlowDocument->SetFontUnderline();
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatBold"))==0)
	{
		m_pFlowDocument->SetFontBold();
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatTextColor"))==0)
	{
		m_pFlowDocument->ChooseColorDlg(STI_TEXTCOLOR);
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatTextBkColor"))==0)
	{
		m_pFlowDocument->ChooseColorDlg(STI_BKCOLOR);
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignCenter"))==0)
	{
		m_pFlowDocument->SetAlign(DT_CENTER);
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignRight"))==0)
	{
		m_pFlowDocument->SetAlign(DT_RIGHT);
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignLeft"))==0)
	{
		m_pFlowDocument->SetAlign(DT_LEFT);
		pEvent->dwRet = 1;
	}
	else if(pStrID && _tcscmp(pStrID,_T("xmShapeImage"))==0)
	{
		this->m_pFlowDocument->m_FDrawState=WDSIMAGESELECTION;
		pEvent->dwRet = 1;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = true;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
	}else if(pStrID && _tcscmp(pStrID,_T("xmShapeObject"))==0)
	{
		pEvent->dwRet = 1;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = true;
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
	}else if(pStrID && _tcscmp(pStrID,_T("xmPointerSelect"))==0)
	{
		this->m_pFlowDocument->m_FDrawState=WDSSELECTION;
		pEvent->dwRet = 1;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = true;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
		((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
		::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
	}
	else if(_tcsstr(pEvent->xcommand.pStrID,_T(":")))
	{
		this->m_pFlowDocument->m_FDrawState=WDSSHAPESELECTION;
		if(this->m_pFlowDocument->m_pStrShape)delete this->m_pFlowDocument->m_pStrShape;
		this->m_pFlowDocument->m_pStrShape = _tcsdup(pEvent->xcommand.pStrID);
		pEvent->dwRet = 1;
	}
#endif
	return true;
}

void XWordView::SetFontBold()
{
	if(GetReadOnly()) return ;

	vord_t block;
	vord_t block1;
	StandOnlySelection(m_Selection,block,block1);

	int nRow1 = m_Selection.endRow;
	int nCol1 = m_Selection.endCol;
	int index1;
	vord_t block2 = GetBlockAtRowColumn(nRow1,nCol1,index1);

	LOGFONT logFont;
	bool bFirst = true;
	bool bBold = false;

	bool start = true;
	while(block && block1)
	{
		XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
		if(block[css::index]> 0 && m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1); 
		else if(block.get_parent()[css::index]> 0 && m_pDataSvr->m_pStyleSvr->GetBoxStyle(block.get_parent()[css::index] - 1))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(block.get_parent()[css::index] - 1); 
		if(!pBoxStyle &&  m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
		XUtil::Css::GetFont(pBoxStyle,logFont);
		if(bFirst)
		{
			if(logFont.lfWeight == FW_BOLD)
			{
				logFont.lfWeight = FW_NORMAL;
			}else
			{
				logFont.lfWeight = FW_BOLD;
				bBold = true;
			}
		}else
		{
			if(bBold)
				logFont.lfWeight = FW_BOLD;
			else
				logFont.lfWeight = FW_NORMAL;
		}
		bFirst = false;

		
		XUtil::Css::SetFont(pBoxStyle,logFont);
		int nStyleIndex = m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
		if(pBoxStyle) delete pBoxStyle;
		block[css::index] = nStyleIndex + 1;
		CalcBlockExtent(block);

		if(block.equal(block1))break;

		//next block
		if(!block.get_next())
		{
			block = block.get_parent().get_next();
			while(block && block[control::type])block = block.get_next();
			if(block)block = block.get_child();
		}else
			block = block.get_next();
	}

	GetRowColumnByBlock(block2,index1,nRow1,nCol1);
	m_Selection.endRow = nRow1;
	m_Selection.endCol = nCol1;

	m_pages = CalcPages();
	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	if(m_Selection.bLast)
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.startRow ,m_Selection.startCol);
	}else
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.endRow ,m_Selection.endCol);
	}
	ShowCurrentCaret(true);
}
void XWordView::SetFontItalic()
{
	if(GetReadOnly()) return ;

	LOGFONT logFont;
	bool bFirst = true;
	bool bBold = false;

	vord_t block;
	vord_t block1;
	StandOnlySelection(m_Selection,block,block1);

	int nRow1 = m_Selection.endRow;
	int nCol1 = m_Selection.endCol;
	int index1;
	vord_t block2 = GetBlockAtRowColumn(nRow1,nCol1,index1);

	bool start = true;
	bool bItalic = false;
	while(block && block1 )
	{
		XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
		if(block[css::index]> 0 && m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1); 
		if(!pBoxStyle &&  m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
		XUtil::Css::GetFont(pBoxStyle,logFont);
		if(bFirst)
		{
			if(logFont.lfItalic == TRUE)
			{
				logFont.lfItalic = FALSE;
			}else
			{
				logFont.lfItalic = TRUE;
				bItalic = true;
			}
		}else
		{
			if(bItalic)
				logFont.lfItalic = TRUE;
			else
				logFont.lfItalic = FALSE;
		}
		bFirst = false;


		XUtil::Css::SetFont(pBoxStyle,logFont);
		int nStyleIndex = m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
		if(pBoxStyle) delete pBoxStyle;
		block[css::index] = nStyleIndex + 1;
		CalcBlockExtent(block);

		if(block.equal(block1))break;

		//next block
		if(!block.get_next())
		{
			block = block.get_parent().get_next();
			while(block && block[control::type])block = block.get_next();
			if(block)block = block.get_child();
		}else
			block = block.get_next();
	}

	GetRowColumnByBlock(block2,index1,nRow1,nCol1);
	m_Selection.endRow = nRow1;
	m_Selection.endCol = nCol1;

	m_pages = CalcPages();
	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	if(m_Selection.bLast)
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.startRow ,m_Selection.startCol);
	}else
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.endRow ,m_Selection.endCol);
	}
	ShowCurrentCaret(true);

}
void XWordView::SetFontUnderline()
{
	if(GetReadOnly()) return ;

	LOGFONT logFont;
	bool bFirst = true;
	bool bBold = false;

	vord_t block;
	vord_t block1;
	StandOnlySelection(m_Selection,block,block1);

	int nRow1 = m_Selection.endRow;
	int nCol1 = m_Selection.endCol;
	int index1;
	vord_t block2 = GetBlockAtRowColumn(nRow1,nCol1,index1);

	bool start = true;
	bool bUnderline = false;
	while(block && block1)
	{
		XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
		if(block[css::index]> 0 && m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1); 
		if(!pBoxStyle &&  m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
		XUtil::Css::GetFont(pBoxStyle,logFont);
	
		if(bFirst)
		{
			if(logFont.lfUnderline == TRUE)
			{
				logFont.lfUnderline = FALSE;
			}else
			{
				logFont.lfUnderline = TRUE;
				bUnderline = true;
			}

		}else
		{
			if(bUnderline)
				logFont.lfUnderline = TRUE;
			else
				logFont.lfUnderline = FALSE;
		}
		bFirst = false;

		XUtil::Css::SetFont(pBoxStyle,logFont);
		int nStyleIndex = m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
		if(pBoxStyle) delete pBoxStyle;
		block[css::index] = nStyleIndex + 1;
		CalcBlockExtent(block);

		if(block.equal(block1))break;

		//next block
		if(!block.get_next())
		{
			block = block.get_parent().get_next();
			while(block && block[control::type])block = block.get_next();
			if(block)block = block.get_child();
		}else
			block = block.get_next();
	}

	GetRowColumnByBlock(block2,index1,nRow1,nCol1);
	m_Selection.endRow = nRow1;
	m_Selection.endCol = nCol1;

	m_pages = CalcPages();
	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	if(m_Selection.bLast)
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.startRow ,m_Selection.startCol);
	}else
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.endRow ,m_Selection.endCol);
	}
	ShowCurrentCaret(true);

}

void XWordView::SetFont(LOGFONT &logFont,COLORREF color)
{
	if(GetReadOnly()) return ;

	vord_t block;
	vord_t block1;
	StandOnlySelection(m_Selection,block,block1);

	int nRow1 = m_Selection.endRow;
	int nCol1 = m_Selection.endCol;
	int index1;
	vord_t block2 = GetBlockAtRowColumn(nRow1,nCol1,index1);

	bool start = true;
	while(block && block1)
	{
		XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
		if(block[css::index]> 0 && m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1); 
		if(!pBoxStyle &&  m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
		XUtil::Css::SetFont(pBoxStyle,logFont);
		int nStyleIndex = m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
		if(pBoxStyle) delete pBoxStyle;
		block[css::index] = nStyleIndex + 1;
		CalcBlockExtent(block);

		if(block.equal(block1))break;

		//next block
		if(!block.get_next())
		{
			block = block.get_parent().get_next();
			while(block && block[control::type])block = block.get_next();
			if(block)block = block.get_child();
		}else
			block = block.get_next();
	}

	GetRowColumnByBlock(block2,index1,nRow1,nCol1);
	m_Selection.endRow = nRow1;
	m_Selection.endCol = nCol1;

	m_pages = CalcPages();
	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	if(m_Selection.bLast)
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.startRow ,m_Selection.startCol);
	}else
	{
		CalcCursorInfo(m_CursorInfo,m_Selection.endRow ,m_Selection.endCol);
	}
	ShowCurrentCaret(true);
}

bool XWordView::ChooseFontDlg()
{
	if(GetReadOnly()) return 1;

	CHOOSEFONT cf; 
	LOGFONT	   logFont;
	COLORREF   Color = -1;

	int nRow = m_Selection.startRow;
	int nCol = m_Selection.startCol;
	int index = 0;
	vord_t block  = GetBlockAtRowColumn(nRow,nCol,index);
	const XUtil::Css::TBoxStyle * pBoxStyle = m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1);
	ZeroMemory(&cf,sizeof(CHOOSEFONT));

	XUtil::Css::GetFont(pBoxStyle,logFont);
	if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
	{
		Color = pBoxStyle->pColor->color[0];
	}

	cf.lStructSize = sizeof(CHOOSEFONT); 
	cf.hwndOwner = GetParent(m_pSheetCtrl->m_hWnd); 
	cf.hDC = 0;  
	cf.lpLogFont = &logFont; 
	cf.iPointSize = 18; 
	cf.Flags = CF_BOTH|CF_EFFECTS|CF_INITTOLOGFONTSTRUCT|CF_EFFECTS; 
	cf.rgbColors = Color; 
	cf.lCustData = 0; 
	cf.lpfnHook = NULL; 
	cf.lpTemplateName = NULL; 
	cf.hInstance = NULL; 
	cf.lpszStyle = NULL; 
	cf.nFontType = 200; 
	cf.___MISSING_ALIGNMENT__ = 0; 
	cf.nSizeMin = 8; 
	cf.nSizeMax = 32;

	if(::ChooseFont(&cf))
	{
		if(cf.rgbColors !=Color && Color != -1 ||
			(Color == -1 && cf.rgbColors != RGB(0,0,0)))
			this->SetFont(*cf.lpLogFont,cf.rgbColors);
		else
			this->SetFont(*cf.lpLogFont,-1);

		return true;
	}
	return false;
}

bool XWordView::ChooseColorDlg(DWORD flag)
{
	if(GetReadOnly()) return 1;

	CHOOSECOLOR cc;
	COLORREF colors[16];

	colors[0] = RGB(255,255,255);
	colors[1] = RGB(255,204,153);
	colors[2] = RGB(255,255,153);
	colors[3] = RGB(204,255,204);
	colors[4] = RGB(204,255,255);
	colors[5] = RGB(255,153,204);
	colors[6] = RGB(255,153,0);
	colors[7] = RGB(153,204,0);
	colors[8] = RGB(153,51,0);
	colors[9] = RGB(51,153,102);

	colors[10] = 0xdda0dd;
	colors[11] = 0xee82ee;
	colors[12] = 0xe6e6fa;
	colors[13] = 0x778899;
	colors[14] = 0x00ced1;
	colors[15] = 0x7fffd4;

	::memset(&cc,0,sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR); 
	cc.hwndOwner = GetParent(m_pSheetCtrl->m_hWnd); 
	cc.hInstance = 0; 
	cc.rgbResult = 0; 
	cc.lpCustColors = colors; 
	cc.Flags = CC_RGBINIT; 
	cc.lCustData = 0; 
	cc.lpfnHook = NULL; 
	cc.lpTemplateName = NULL;

	if(::ChooseColor(&cc))
	{
		this->SetColor(cc.rgbResult,flag);
		return true;
	}
	return false;
}

void XWordView::SetColor(COLORREF color, DWORD flag)
{
	if(GetReadOnly()) return ;

	vord_t block;
	vord_t block1;
	StandOnlySelection(m_Selection,block,block1);

	bool start = true;
	while(block && block1)
	{
		XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
		if(block[css::index]> 0 && m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(block[css::index] - 1); 
		if(!pBoxStyle &&  m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
			*pBoxStyle = *m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
		
		if(flag & STI_TEXTCOLOR)
		{
			if(!(pBoxStyle->pColor))
			{
				pBoxStyle->mask |= cssMaskColor;
				pBoxStyle->pColor = new XUtil::Css::TColor;
			}
			pBoxStyle->pColor->colorRef[0] = XUtil::Css::_rgbcolor;
			pBoxStyle->pColor->color[0] = color;
		}else if(flag & STI_BKCOLOR)
		{
			if(!(pBoxStyle->pBackGround))
			{
				pBoxStyle->mask |= cssMaskBackGround;
				pBoxStyle->pBackGround = new XUtil::Css::TBackGround;
			}
			pBoxStyle->pBackGround->mask = cssMaskBackGroundColor;
			pBoxStyle->pBackGround->color.colorRef[0] = XUtil::Css::_rgbcolor;
			pBoxStyle->pBackGround->color.color[0] = color;
		}
			
		int nStyleIndex = m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
		if(pBoxStyle) delete pBoxStyle;
		block[css::index] = nStyleIndex + 1;

		if(block.equal(block1))break;

		//next block
		if(!block.get_next())
		{
			block = block.get_parent().get_next();
			while(block && block[control::type])block = block.get_next();
			if(block)block = block.get_child();
		}else
			block = block.get_next();
	}

	::InvalidateRect(m_pSheetCtrl->m_hWnd,&FRect,false);
	ShowCurrentCaret(true);
}

void XWordView::SetAlign(DWORD flag)
{
	if(GetReadOnly()) return ;
}

bool XWordView::StandOnlyToPara(int nRow,int nCol)
{
	int index;
	vord_t block  = GetBlockAtRowColumn(nRow,nCol,index);

	vord_t p(block.get_parent().groupid());
	p[css::index] = block.get_parent()[css::index];
	wchar_t * pStr = block[text::content];

	vord_t v = block;
	if(index < 0) //entity block
	{
		if(block.get_piror())
		{
			vord_t pr = block.get_piror();
			wchar_t * pStr1 = (pr)[text::content];

			vord_t::splitChild(p,block);
			CalcBlockExtent(p.get_piror().get_child());
			CalcBlockExtent(p.get_child());
		}
		else
		{
			vord_t t(block.groupid());
			t.append(css::index,block[css::index]);
			t.append(block::type,blocktype::text);
			t.append(text::content,L"");
			p.appendChild(t);
			block.get_parent().insertBefore(p);

			CalcBlockExtent(p.get_next().get_child());
			CalcBlockExtent(p.get_child());
		}
	}
	else if(index == wcslen(pStr) - 1) //next block
	{
		if(block.get_next())
		{
			vord_t::splitChild(p,block.get_next());
			CalcBlockExtent(p.get_piror().get_child());
			CalcBlockExtent(p.get_child());
		}
		else
		{
			vord_t t(block.groupid());
			t.append(css::index,block[css::index]);
			t.append(block::type,blocktype::text);
			t.append(text::content,L"");
			p.appendChild(t);
			block.get_parent().insertAfter(p);

			CalcBlockExtent(p.get_piror().get_child());
			CalcBlockExtent(p.get_child());
		}
	}else
	{
		vord_t t(block.groupid());
		t.append(css::index,block[css::index]);
		t.append(block::type,blocktype::text);
		t.append(text::content,_wcsdup(block[text::content] + index + 1));
		t.append(css::index,block[css::index]);
		block[text::content][index + 1] = 0;

		block.insertAfter(t);
		vord_t::splitChild(p,t);

		CalcBlockExtent(block);
		CalcBlockExtent(t);

	}
	v.get_parent().get_parent()[extent_c::cy] += 6;
	v.get_parent().get_parent().get_parent()[extent_c::cy] += 6;

	return false;
}

bool XWordView::StandOnlySelection(XWSelection &Selection)
{
	vord_t first;
	vord_t last;
	return StandOnlySelection(Selection, first, last);
}

bool XWordView::StandOnlySelection(XWSelection &Selection,vord_t &first,vord_t &last)
{

	int nRow = m_Selection.startRow;
	int nCol = m_Selection.startCol;
	
	int nRow1 = m_Selection.endRow;
	int nCol1 = m_Selection.endCol;
	
	int index,index1;
	vord_t block  = GetBlockAtRowColumn(nRow,nCol,index);
	vord_t block1 = GetBlockAtRowColumn(nRow1,nCol1,index1);

	if(index >= 0 && index + 1 == wcslen(block[text::content]) && block.get_next())
	{
		index = -1;
		block = block.get_next();
	}

	if(index < 0)
		first = block;
	else
	{
		vord_t t(block.groupid()); 
		t.append(block::type,blocktype::text);
		t.append(text::content,_wcsdup(block[text::content] + index + 1));
		block[text::content][index + 1] = 0;
		if(block.get_next())
			block.get_next().insertBefore(t);
		else
			block.get_parent().appendChild(t);
		t[css::index] = block[css::index];
		first = block.get_next();
		CalcBlockExtent(block);
		if(block.get_parent()!=first.get_parent())
			CalcBlockExtent(first);
	}
	if(block==block1)
	{
		block1 = first;
		if(index > 0) index1 -= index + 1;
	}
	if(index1 +1 >= (block1[text::content]?(int)wcslen(block1[text::content]):0))
		last = block1;
	else
	{
		vord_t t(block1.groupid());
		t.append(block::type,blocktype::text);

		wchar_t* pStr = block1[text::content];
		wchar_t* buf = new wchar_t[index1 + 2];
		if(index1>=0) wcsncpy_s(buf,index1 + 2,pStr,index1+1);
		buf[index1+1] = 0;
		int i = 0;
		while(pStr[i+index1+1])
		{
			pStr[i] = pStr[i+index1+1];
			i++;
		}
		pStr[i]=0;
		/*if((*block1)[text::pdx])
		{
			(*block1)[text::pdx] = 0;
			delete (*block1)[text::pdx];
		}*/
		
		t.append(text::content,buf);
		block1.insertBefore(t);
		t[css::index] = block1[css::index];
		last = block1.get_piror();
		CalcBlockExtent(last);
		if(last.get_parent()!=block1.get_parent())
			CalcBlockExtent(block1);
		if(first==block1)first =last;
	}

	return true;
}

bool XWordView::GetRowColumnByBlock(vord_t block,int charPos, int & nRow,int & nCol)
{
	vord_t para = block.get_parent();
	XWordDataSvr* pDataSvr = (XWordDataSvr*)m_pDataSvr;

	vord_t& doc =  pDataSvr->m_pContentSvr->document;
	vord_t item = doc.get_child().get_child();

	nRow = 0;
	nCol = 0;
	while(item)
	{
		int pc = item[render::pc];
		unsigned char * code = item.get_parent()[render::code];
		if(item[control::type])
		{
			nRow++;
		}else
		{
			if(!item.equal(para))
				nRow += item[render::rowcount];
			else
			{
				XWCodeInter machine;
				CalcRowColAtBlock wx(block,charPos);
				XWCodeContextInfo context;
				machine.Inter(wx,&context, item.get_parent()[render::code] +
					item[render::pc]);
				nRow += wx.row_;
				nCol = wx.col_;
				return true;
			}
		}
		item = item.get_next();
	}
	return false;
}

bool XWordView::IsControl(vord_t & item,wchar_t * typ)
{
	if(item && _wcsicmp(item[control::type],typ)==0)
		return true;
	else
		return false;
}

void XWordView::SetReadOnly(bool bReadOnly)
{
	if(bReadOnly)
	{
		if(!GetReadOnly())
		{
			m_State |= TWordState_ReadOnly;
		}
	}else
	{
		if(GetReadOnly())
		{
			m_State &= ~TWordState_ReadOnly;
		}
	}
}

bool XWordView::GetReadOnly()
{
	return m_State & TWordState_ReadOnly ? true:false;
}
