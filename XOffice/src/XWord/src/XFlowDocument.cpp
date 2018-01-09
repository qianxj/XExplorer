#include "StdAfx.h"
#include "..\include\XFlowDocument.hpp"
#include "..\include\XFlowChapter.hpp"
#include "..\include\XFlowPara.hpp"
#include "xcell.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XUtil::Css::tagCssData 	_CssData;
	XFlowDocument::XFlowDocument():m_pContent(NULL),m_pSheetSvr(NULL),m_FState(TWordState_Normal),m_FDrawState(WDSSELECTION),m_pStrShape(NULL),FOptions(goThumbTracking),m_nPage(1),m_nView(WVIEW_PAGE)
	{
		m_ptScroll.x = 0;
		m_ptScroll.y = 0;

		::SetRect(&m_rcPage,0,0, MulDiv(210,100*_CssData.nXPixelPerInch,2540),
		MulDiv(297,100*_CssData.nXPixelPerInch,2540));

		::SetRect(&m_rcPageMargin,60,60,60,60);
		m_rcPageContent.left = m_rcPage.left + m_rcPageMargin.left;
		m_rcPageContent.right = m_rcPage.right - m_rcPageMargin.right;
		m_rcPageContent.top = m_rcPage.top + m_rcPageMargin.top;
		m_rcPageContent.bottom = m_rcPage.bottom - m_rcPageMargin.bottom;

		InitCursorInfo(m_CursorInfo);
		InitSelection(m_Selection);
	}

	XFlowDocument::~XFlowDocument()
	{
		if(m_pStrShape)delete m_pStrShape;
	}

	//获取设定高度和宽度信息
	POINT XFlowDocument::GetExtent()
	{
		POINT pt={0,0};
		return pt;
	}
	//获取实际占用的高度和宽度信息
	POINT XFlowDocument::GetRealExtent()
	{
		POINT pt={0,0};
		return pt;
	}

	int XFlowDocument::SetFRect(RECT &rc)
	{
		FRect = rc;
		FContentRect = rc;
		return 1;
	}
	const RECT & XFlowDocument::GetFRect()
	{
		return FRect;
	}

	int XFlowDocument::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		return 1;
	}

	int XFlowDocument::DrawPageMarker(HDC hPaintDC,RECT &rcPage,RECT &rcPageMargin)
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

	int XFlowDocument::DoDraw(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,RECT * pDrawRect,RECT * pUpdateRect)
	{
		RECT	rc;
		//if(!pDrawRect) 
			rc = FRect;
		//else
		//	rc = *pDrawRect;

		int nSave = ::SaveDC(hPaintDC);
		IntersectClipRect(hPaintDC,rc.left,rc.top,rc.right,rc.bottom);

		HBRUSH hBrush = ::CreateSolidBrush(RGB(144,153,174));
		::FillRect(hPaintDC,&rc,hBrush);
		::DeleteObject(hBrush);

		OffsetViewportOrgEx(hPaintDC,- m_ptScroll.x, - m_ptScroll.y,NULL);

		RECT rcPage;
		if( rc.right - rc.left < m_rcPage.right - m_rcPage.left)
			rcPage.left = rc.left + 5;
		else
			rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_rcPage.right - m_rcPage.left)/2;
		rcPage.top = rc.top + 20;
		rcPage.right = rcPage.left + m_rcPage.right - m_rcPage.left;
		rcPage.bottom = rcPage.top + m_rcPage.bottom - m_rcPage.top;

		::Rectangle(hPaintDC,rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);
		DrawPageMarker(hPaintDC,rcPage,m_rcPageMargin);

		/*
		for(int i=1;i<=m_nPage;i++)
		{
			POINT	pt;
			pt.y = rcPage.top + m_rcPageMargin.top;
			pt.x = rcPage.left + m_rcPageMargin.left;
			DoDrawPage(hPaintDC,i,pt);

			::OffsetRect(&rcPage, 0,rcPage.bottom - rcPage.top + 1 + 20);
			::Rectangle(hPaintDC,rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);
			DrawPageMarker(hPaintDC,rcPage,m_rcPageMargin);
		}*/

		if(m_pContent)
		{
			POINT	pt = { rcPage.left + m_rcPageMargin.left,rcPage.top + m_rcPageMargin.top};
			int		nWidth = rcPage.right - rcPage.left - m_rcPageMargin.right - m_rcPageMargin.left;  
			POINT   ptStart = pt;

			XFlowChapter *pChapter = new XFlowChapter;
			for(int i=0;i<this->m_pContent->m_pChilds->size();i++)
			{
				pChapter->m_pContent = (XWordDataChapter *)(*m_pContent->m_pChilds)[i];
				
				::SetRect(&pChapter->FRect,ptStart.x,ptStart.y,ptStart.x + nWidth ,ptStart.y);
				SIZE extent = pChapter->CalcExtent(pDataSvr,pParentBoxStyle,hPaintDC);
				/*if(ptStart.y + extent.cy < pDrawRect->top + m_ptScroll.y)
				{
					//非可视区，继续下一段
					ptStart.y += extent.cy;
					continue;
				}*/
				pChapter->DoDraw(pDataSvr,this,pParentBoxStyle,hPaintDC,ptStart,pDrawRect,pUpdateRect);
				ptStart.y += extent.cy;

				//超出视区,则退出
				//if(ptStart.y > pDrawRect->bottom  + m_ptScroll.y ) break;
			}
			if(pChapter) delete pChapter;
		}

		OffsetViewportOrgEx(hPaintDC,m_ptScroll.x,m_ptScroll.y,NULL);
		::RestoreDC(hPaintDC,nSave);

		return 1;
	}

	//重新计算位置信息
	SIZE XFlowDocument::CalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC)
	{
		SIZE extent={0,0};
		if(!m_pContent) return extent;

		int		nHeight = 0;
		RECT	rc = FRect;
		RECT rcPage;
		if( rc.right - rc.left < m_rcPage.right - m_rcPage.left)
			rcPage.left = rc.left + 5;
		else
			rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_rcPage.right - m_rcPage.left)/2;
		rcPage.top = rc.top + 20;
		rcPage.right = rcPage.left + m_rcPage.right - m_rcPage.left;
		rcPage.bottom = rcPage.top + m_rcPage.bottom - m_rcPage.top;
		int		nWidth = rcPage.right - rcPage.left - m_rcPageMargin.right - m_rcPageMargin.left;  
	
		XFlowChapter *pChapter = new XFlowChapter;
		for(int i=0;i<this->m_pContent->m_pChilds->size();i++)
		{
			pChapter->m_pContent = (XWordDataChapter *)(*m_pContent->m_pChilds)[i];
			
			RECT rc ;
			::SetRect(&rc,0,0,nWidth ,0);
			pChapter->FRect = rc;
			SIZE extent = pChapter->CalcExtent(pDataSvr,pParentBoxStyle,hDC);
			nHeight += extent.cy;
		}
		if(pChapter) delete pChapter;

		extent.cx = nWidth;
		extent.cy = nHeight;
		return extent;
	}

	XFlowChapter * XFlowDocument::GetChapterAtPointer(XWordDataSvr* pDataSvr,POINT pt)
	{
		SIZE extent={0,0};
		if(!m_pContent) return NULL;

		int		nHeight = 0;
		RECT	rc = FRect;
		RECT rcPage;
		if( rc.right - rc.left < m_rcPage.right - m_rcPage.left)
			rcPage.left = rc.left + 5;
		else
			rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_rcPage.right - m_rcPage.left)/2;
		rcPage.top = rc.top + 20;
		rcPage.right = rcPage.left + m_rcPage.right - m_rcPage.left;
		rcPage.bottom = rcPage.top + m_rcPage.bottom - m_rcPage.top;
		int		nWidth = rcPage.right - rcPage.left - m_rcPageMargin.right - m_rcPageMargin.left;  
		int	i=0;

		POINT   ptStart ={ rcPage.left + m_rcPageMargin.left,rcPage.top + m_rcPageMargin.top};

		HDC hDC = GetDC(NULL);
		XFlowChapter *pChapter = new XFlowChapter;
		for(i=0;i<this->m_pContent->m_pChilds->size();i++)
		{
			pChapter->m_pContent = (XWordDataChapter *)(*m_pContent->m_pChilds)[i];
			
			::SetRect(&pChapter->FRect,ptStart.x,ptStart.y,ptStart.x + nWidth ,ptStart.y);
			SIZE extent = pChapter->CalcExtent(pDataSvr,NULL,hDC);
			nHeight += extent.cy;
			if(pt.y < nHeight)break; 
		}
		::ReleaseDC(NULL,hDC);
		return pChapter;
	}

	bool XFlowDocument::CalcCursorInfoAtPoint(XWordDataSvr* pDataSvr,POINT pt,XWordCursorInfoEx & CursorInfo)
	{
		if(!m_pContent) return false;
		if(m_pContent->m_pChilds->size()==1 &&
			(*m_pContent->m_pChilds)[0]->m_pChilds->size()==1 &&
			(*(*m_pContent->m_pChilds)[0]->m_pChilds)[0]->m_pChilds->size()==1 )
		{
			CursorInfo.CursorHeight = 17;
			return CalcCursorPosition( pDataSvr,NULL,
				(XWordDataTextBlock *)(*(*(*m_pContent->m_pChilds)[0]->m_pChilds)[0]->m_pChilds)[0],0,CursorInfo);
		}
		::memset(&CursorInfo,0,sizeof(XWordCursorInfoEx));
		XFlowChapter * pChater = this->GetChapterAtPointer(pDataSvr,pt);
		if(!pChater) return false;

		XFlowPara * pPara = pChater->GetParaAtPointer(pDataSvr,pt);
		if(!pPara ||pPara->m_pContent->eWordDataType==_XWordDataControlParagraph)
		{
			if(pChater) delete pChater;
			return false;
		}

		HDC hDC = ::GetDC(NULL);
		bool ret = pPara->CalcCursorPositionAtPoint(pDataSvr,NULL,hDC,pt,CursorInfo);
		::ReleaseDC(NULL,hDC);
		
		if(pChater) delete pChater;
		if(pPara) delete pPara;

		return ret;
	}
	
	XWordDataBase* XFlowDocument::GetControlBlockAtPoint(XWordDataSvr* pDataSvr,POINT pt)
	{
		XFlowChapter * pChater = this->GetChapterAtPointer(pDataSvr,pt);
		if(!pChater) return NULL;

		XFlowPara * pPara = pChater->GetParaAtPointer(pDataSvr,pt);
		if(!pPara)
		{
			if(pChater) delete pChater;
			return NULL;
		}
		
		XWordDataBase* pBlock;
		if(pPara->m_pContent->eWordDataType==_XWordDataControlParagraph)
			pBlock = pPara->m_pContent;
		else
			pBlock = pPara->GetControlBlockAtPoint(pDataSvr, pt);
		if(pChater) delete pChater;
		if(pPara) delete pPara;

		return pBlock;
	}

	XFlowPara* XFlowDocument::GetControlParaAtPoint(XWordDataSvr* pDataSvr,POINT pt)
	{
		XFlowChapter * pChater = this->GetChapterAtPointer(pDataSvr,pt);
		if(!pChater) return NULL;

		XFlowPara * pPara = pChater->GetParaAtPointer(pDataSvr,pt);
		if(!pPara)
		{
			if(pChater) delete pChater;
			return NULL;
		}
		
		if(pPara->m_pContent->eWordDataType==_XWordDataControlParagraph)
			return pPara;
		else
			return NULL;	
	}


	bool XFlowDocument::CalcCursorPosition(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle,XWordDataTextBlock *pBlock,int nTextIndex,XWordCursorInfoEx & CursorInfo)
	{
		int nIndex = -1;
		XFlowChapter * pChapter = new XFlowChapter;
		XFlowPara * pPara = NULL;

		SIZE extent={0,0};
		if(!m_pContent) return NULL;

		int		nHeight = 0;
		RECT	rc = FRect;
		RECT rcPage;
		if( rc.right - rc.left < m_rcPage.right - m_rcPage.left)
			rcPage.left = rc.left + 5;
		else
			rcPage.left = rc.left + (rc.right - rc.left)/2 - (m_rcPage.right - m_rcPage.left)/2;
		rcPage.top = rc.top + 20;
		rcPage.right = rcPage.left + m_rcPage.right - m_rcPage.left;
		rcPage.bottom = rcPage.top + m_rcPage.bottom - m_rcPage.top;
		int		nWidth = rcPage.right - rcPage.left - m_rcPageMargin.right - m_rcPageMargin.left;  

		POINT   ptStart ={ rcPage.left + m_rcPageMargin.left,rcPage.top + m_rcPageMargin.top};

		HDC hDC = GetDC(NULL);
		for(int i=0;i<m_pContent->m_pChilds->size();i++)
		{
			//RECT rc ;
			//::SetRect(&rc,0,FRect.top + nHeight,nWidth ,FRect.top + nHeight);
			//pChapter->FRect = rc;
			::SetRect(&pChapter->FRect,ptStart.x,ptStart.y,ptStart.x + nWidth ,ptStart.y);
		
			pChapter->m_pContent = (XWordDataChapter *)(*m_pContent->m_pChilds)[i];
			pPara = pChapter->GetParaByBlock(pDataSvr,pBlock);
			if(pPara)break;

			SIZE extent = pChapter->CalcExtent(pDataSvr,NULL,hDC);
			nHeight += extent.cy;
		}
		if(!pPara)
		{
			if(pChapter) delete pChapter;
			return false;
		}

		bool ret = pPara->CalcCursorPosition(pDataSvr,NULL,hDC,pBlock,nTextIndex,CursorInfo);
		::ReleaseDC(NULL,hDC);

		delete pPara;
		delete pChapter;

		return ret;
	}


	void XFlowDocument::ShowCurrentCaret(bool bShow)
	{
		if(!bShow)
			::HideCaret(m_pSheetSvr->m_pSheetCtrl->m_hWnd);
		else
		{
			CreateCaret(m_pSheetSvr->m_pSheetCtrl->m_hWnd, NULL, 1, m_CursorInfo.CursorHeight + 4);
			SetCaretPos(m_CursorInfo.XPosCursur - m_ptScroll.x - 2,m_CursorInfo.YPosCursur - m_ptScroll.y - 2);  
			ShowCaret(m_pSheetSvr->m_pSheetCtrl->m_hWnd);
		}
	}

	bool XFlowDocument::CalcLeftCursorInfo(XWordCursorInfoEx & CursorInfo)
	{
		if(!CursorInfo.pBlock) return false;

		if(CursorInfo.nIndex >0)
			CursorInfo.nIndex--;
		else
		{
			XWordDataTextBlock * pBlock = this->GetPirorTextBlock(CursorInfo.pBlock);
			if(!pBlock) return false;
			CursorInfo.pBlock = pBlock;
			CursorInfo.nIndex = (int)_tcslen(pBlock->m_pStrText);
		}
		CalcCursorPosition((XWordDataSvr *)m_pSheetSvr->m_pDataSvr,NULL,CursorInfo.pBlock,CursorInfo.nIndex,CursorInfo);
		return true;
	}
	bool XFlowDocument::CalcRightCursorInfo(XWordCursorInfoEx & CursorInfo)
	{
		if(!CursorInfo.pBlock) return false;
		if(CursorInfo.nIndex <= (int)_tcslen(CursorInfo.pBlock->m_pStrText) - 1)
			CursorInfo.nIndex++;
		else
		{
			XWordDataTextBlock * pBlock = this->GetNextTextBlock(CursorInfo.pBlock);
			if(!pBlock) return false;
			CursorInfo.pBlock = pBlock;
			CursorInfo.nIndex = 0;
		}
		CalcCursorPosition((XWordDataSvr *)m_pSheetSvr->m_pDataSvr,NULL,CursorInfo.pBlock,CursorInfo.nIndex,CursorInfo);
		return true;
	}
	bool XFlowDocument::CalcUpCursorInfo(XWordCursorInfoEx & CursorInfo)
	{
		return true;
	}
	bool XFlowDocument::CalcDownCursorInfo(XWordCursorInfoEx & CursorInfo)
	{
		return true;
	}

	CXScrollBar * XFlowDocument::GetHScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pHScrollBar;
	}
	CXScrollBar * XFlowDocument::GetVScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pVScrollBar;
	}

	void XFlowDocument::WMVScroll(UINT nSBCode, UINT nPos, HWND hWndCtl)
	{
		ModifyScrollBar(SB_VERT, nSBCode, nPos);
	}

	void XFlowDocument::WMHScroll(UINT nSBCode, UINT nPos, HWND hWndCtl)
	{
		ModifyScrollBar(SB_HORZ, nSBCode, nPos);
	}

	void XFlowDocument::WMWheel(POINT point,UINT_PTR	fwKeys,	short zDelta)
	{
		if(zDelta > 0)
			ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
		else
			ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
	}

	int XFlowDocument::CalcScrollBar(int Value, int ScrollCode, int ScrollBar,int Pos)
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

	void XFlowDocument::ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos)
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
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		ShowCurrentCaret(true);
	}

	void XFlowDocument::UpdateScrollPos(int ScrollBar)
	{
		if(ScrollBar==SB_VERT)
		{
			if(GetVScrollBar())GetVScrollBar()->SetPosition(MulDiv(m_ptScroll.y , 100 , GetMaxScrollExtent(ScrollBar)));
		}else
			if(GetHScrollBar())GetHScrollBar()->SetPosition(MulDiv(m_ptScroll.x, 100 , GetMaxScrollExtent(ScrollBar)));
	}

	int XFlowDocument::PageUp(int ScrollBar)
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

	int XFlowDocument::PageDown(int ScrollBar)
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

	int XFlowDocument::ScrollBarMin(int ScrollBar)
	{
		return 0;
	}

	int XFlowDocument::ScrollBarMax(int ScrollBar)
	{
		return GetMaxScrollExtent( ScrollBar);
	}

	int XFlowDocument::GetTotalHeight()
	{
		HDC hDC = GetDC(m_pSheetSvr->GetHWND());
		//return  m_nPage *(m_rcPage.bottom - m_rcPage.top + 1) + (m_nPage +1) * 20;
		SIZE sz = this->CalcExtent((XWordDataSvr *)m_pSheetSvr->m_pDataSvr,NULL,hDC);
		::ReleaseDC(m_pSheetSvr->GetHWND(),hDC);
		
		sz.cy += FRect.top + this->m_rcPageMargin.top;
		if(sz.cy < this->m_rcPage.bottom - this->m_rcPage.top + 50 + FRect.top)
			sz.cy = FRect.top + 50 + this->m_rcPage.bottom - this->m_rcPage.top;

		return sz.cy ;
	}

	int XFlowDocument::GetMaxScrollExtent(int ScrollBar)
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

	bool XFlowDocument::ClampInView(const XWordCursorInfoEx & CursorInfo)
	{
		return false;
	}

	
	bool XFlowDocument::InsertChar(XWordCursorInfoEx & CursorInfo,TCHAR ch)
	{
		m_Selection.bSelection = false;

		if(ch=='\r')
		{
			StandOnlyToPara(CursorInfo.nRow,CursorInfo.nCol);
			//m_nPage = CalcPages();
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
			this->CalcRightCursorInfo(CursorInfo);
			ShowCurrentCaret(true);
			return true;
		}
		

		int nPosChar = CursorInfo.nIndex;
		XWordDataTextBlock * pTextBlock = CursorInfo.pBlock;
		if(!pTextBlock) return false;
		
		LPTSTR pStr = pTextBlock->m_pStrText;
		int nLimit = pTextBlock->nStrLimit;

		LPTSTR pStrTemp = NULL;
		int nStrLen = (int)(pStr?_tcslen(pStr):0);
		if(!pStr)
		{
			nLimit = 35;
			pStr = new TCHAR[nLimit];
			pStr[0]='\0';
			pTextBlock->nStrLimit = nLimit;
			pTextBlock->m_pStrText = pStr;
		}else
		{
			if(nStrLen + 3 > nLimit)
			{
				if(nLimit == 0)
					nLimit =(int) nStrLen + 3 + 32;
				else
					nLimit += 32;
				pStrTemp = new TCHAR[nLimit];
				pTextBlock->nStrLimit = nLimit;
			}
			
			if(pStrTemp)
			{
				::_tcscpy_s(pStrTemp,nLimit,pStr);
				delete pStr;
				pStr = pStrTemp;
				pTextBlock->m_pStrText = pStr;
			}
		}
		if(ch!=0x0B)
		{
			int nLen = (int)::_tcslen(pStr + nPosChar) + 1;
			while(nLen> 0)
			{
				pStr[nPosChar + nLen] = pStr[nPosChar + nLen - 1];
				nLen--;
			}
			pStr[nPosChar] = ch;
		}
		else
		{
			int nLen = (int)::_tcslen(pStr + nPosChar) + 2;
			while(nLen> 0)
			{
				pStr[nPosChar + nLen + 1] = pStr[nPosChar + nLen - 1];
				nLen--;
			}
			pStr[nPosChar] = '\r';
			pStr[nPosChar + 1] = '\n';
		}
		//pTextBlock->m_pDataParent->m_pDataParent->AdjustExtent();
		//m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		this->CalcRightCursorInfo(CursorInfo);
		ShowCurrentCaret(true);
		return true;
	}
	
	bool XFlowDocument::BackDeleteChar(XWordCursorInfoEx & CursorInfo)
	{
		//m_Selection.bSelection = false;

		int nPosChar = CursorInfo.nIndex;
		XWordDataTextBlock * pTextBlock = CursorInfo.pBlock;
		if(!pTextBlock) return false;
		
		LPTSTR pStr = pTextBlock->m_pStrText;
		int nLimit = pTextBlock->nStrLimit;

		LPTSTR pStrTemp = NULL;
		int nStrLen = (int)(pStr?_tcslen(pStr):0);

		if(nPosChar == 0)
		{
			if(GetIndexFromParent(pTextBlock)==-1) return false;
			if(GetIndexFromParent(pTextBlock)==0)
			{
				XWordDataTextBlock * pBlockEx = GetPirorTextBlock(pTextBlock);
				if(!pBlockEx) return false;

				if(!pBlockEx->m_pStrText||pBlockEx->m_pStrText[0]=='\0')
				{
					this->RemoveBlock(pBlockEx);
				}else
					this->MerginPara((XWordDataContentParagraph *)pBlockEx->m_pDataParent,
							(XWordDataContentParagraph *)pTextBlock->m_pDataParent);
				//pBlock->m_pDataParent->m_pDataParent->AdjustExtent();
				//m_nPage = CalcPages();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
				AdjustExtent();
				CalcCursorPosition((XWordDataSvr *)m_pSheetSvr->m_pDataSvr,NULL,CursorInfo.pBlock,CursorInfo.nIndex,CursorInfo);
				ShowCurrentCaret(true);

				return true;
			}
		}
		XWordDataTextBlock * pBlock = pTextBlock;
		while(nPosChar==0)
		{
			pTextBlock = GetPirorTextBlock(pBlock);
			if(!pTextBlock) return false;
			pStr = pTextBlock->m_pStrText;
			nPosChar = 0;
			if(pStr) nPosChar = (int)::_tcslen(pStr);
			if(!pStr ||pStr[0]=='\0')
			{
				this->RemoveBlock(pTextBlock);
				nPosChar = 0;
			}
		}
		
		if(pStr[nPosChar - 1]=='\n' && nPosChar - 2 >=0 && pStr[nPosChar - 2]=='\r')
		{
			::_tcscpy_s(pStr + nPosChar - 2, _tcslen(pStr) - nPosChar + 1, pStr + nPosChar );
			nPosChar -= 2;
		}else
		{
			::_tcscpy_s(pStr + nPosChar - 1, _tcslen(pStr) - nPosChar + 1, pStr + nPosChar );
			nPosChar--;
		}
		if(pStr[0]=='\0' /*&& nCol - 1>1*/)
		{
			this->RemoveBlock(pTextBlock);
			pTextBlock = NULL;
		}else
		{
			//((XWordDataContentParagraph*)pTextBlock->m_pDataParent)->AdjustExtent();
		}
		//m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
		this->CalcLeftCursorInfo(CursorInfo);
		ShowCurrentCaret(true);
		return true;
	}

	bool XFlowDocument::DeleteChar(XWordCursorInfoEx & CursorInfo)
	{
		//m_Selection.bSelection = false;

		int nPosChar = CursorInfo.nIndex;
		XWordDataTextBlock * pTextBlock = CursorInfo.pBlock;
		XWordDataTextBlock * pBlock = pTextBlock;
		XWordDataTextBlock * pBlockEx = NULL;
		if(!pTextBlock) return false;
		
		LPTSTR pStr = pTextBlock->m_pStrText;
		int nLimit = pTextBlock->nStrLimit;

		LPTSTR pStrTemp = NULL;
		int nStrLen = (int)(pStr?_tcslen(pStr):0);

		
		if(!pStr ||(int)::_tcslen(pStr)== nPosChar)
		{
			if(GetIndexFromParent(pBlock)==pBlock->m_pDataParent->m_pChilds->size() - 1)
			{
				pBlockEx = GetNextTextBlock(pBlock);
				if(!pBlockEx) return false;
				if(!pBlockEx->m_pStrText||pBlockEx->m_pStrText[0]=='\0')
				{
					this->RemoveBlock(pBlockEx);
				}else
					this->MerginPara((XWordDataContentParagraph *)pBlock->m_pDataParent,(XWordDataContentParagraph *)pBlockEx->m_pDataParent);
				//pBlock->m_pDataParent->m_pDataParent->AdjustExtent();
				//m_nPage = CalcPages();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
				AdjustExtent();
				CalcCursorPosition((XWordDataSvr *)m_pSheetSvr->m_pDataSvr,NULL,CursorInfo.pBlock,CursorInfo.nIndex,CursorInfo);
				ShowCurrentCaret(true);
				return true;
			}
		}
		
		if(pStr && (int)::_tcslen(pStr)> nPosChar)
		{
			if(pStr[nPosChar]=='\r' && (int)::_tcslen(pStr)> nPosChar + 1 && pStr[nPosChar + 1]=='\n')
				::_tcscpy_s(pStr + nPosChar, _tcslen(pStr) - nPosChar, pStr + nPosChar + 2 );
			else
				::_tcscpy_s(pStr + nPosChar, _tcslen(pStr) - nPosChar, pStr + nPosChar + 1);
		}else
		{
			while(!pStr || (int)::_tcslen(pStr)<=nPosChar)
			{
				pBlock = GetNextTextBlock(pTextBlock);
				if(!pBlock) return false;
				pTextBlock = pBlock;
				pStr = pTextBlock->m_pStrText;
				nPosChar = 0;
			}
			::_tcscpy_s(pStr + nPosChar, _tcslen(pStr) - nPosChar, pStr + nPosChar + 1 );
		}

		if(pStr[0]=='\0' /*&& nCol < nColCount - 1*/)
		{
			this->RemoveBlock(pTextBlock);
			pTextBlock = NULL;
		}else
		{
			//((XWordContentParagraph *)pTextBlock->m_pDataParent)->AdjustExtent();
		}

		//m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);

		return true;
	}

	int XFlowDocument::GetIndexFromParent(XWordDataBase   * pBlock)
	{
		for(int i=0;i<(int)pBlock->m_pDataParent->m_pChilds->size();i++)
		{
			if((*pBlock->m_pDataParent->m_pChilds)[i]==pBlock) return i;
		}
		return -1;
	}
	
	int XFlowDocument::GetIndex(XWordDataBase  * pBlock)
	{
		int nIndex = 0;
		for(int i=0;i<m_pContent->m_pChilds->size();i++)
		{
			for(int j=0;j<(*m_pContent->m_pChilds)[i]->m_pChilds->size();j++)
			{
				for(int m=0;m<(*(*m_pContent->m_pChilds)[i]->m_pChilds)[j]->m_pChilds->size();m++)
				{
					if((*(*(*m_pContent->m_pChilds)[i]->m_pChilds)[j]->m_pChilds)[m]==pBlock) return nIndex;
					nIndex++;
				}
			}
		}
		return -1;
	}


	XWordDataTextBlock* XFlowDocument::GetPirorTextBlock(XWordDataTextBlock  * pBlock)
	{
		int index = this->GetIndexFromParent(pBlock);
		for(int i=index - 1;i>=0;i--)
		{
			if((*pBlock->m_pDataParent->m_pChilds)[i]->eWordDataType==_XWordDataTextBlock)
				return (XWordDataTextBlock *)(*pBlock->m_pDataParent->m_pChilds)[i];
		}
		index = GetIndexFromParent(pBlock->m_pDataParent);
		XWordDataContentParagraph * pParagraph;
		XWordDataChapter * pChapter;
		for(int i=index - 1;i>=0;i--)
		{
			if((*pBlock->m_pDataParent->m_pDataParent->m_pChilds)[i]->eWordDataType!=_XWordDataParagraph)continue;
			pParagraph = (XWordDataContentParagraph*)(*pBlock->m_pDataParent->m_pDataParent->m_pChilds)[i];
			for(int k=(int)pParagraph->m_pChilds->size() - 1 ;k>=0;k--)
			{
				if((*pParagraph->m_pChilds)[k]->eWordDataType==_XWordDataTextBlock)
				return (XWordDataTextBlock *)(*pParagraph->m_pChilds)[k];
			}
		}
		index = GetIndexFromParent(pBlock->m_pDataParent->m_pDataParent);
		for(int i=index - 1;i>=0;i--)
		{
			if((*pBlock->m_pDataParent->m_pDataParent->m_pDataParent->m_pChilds)[i]->eWordDataType!=_XWordDataChapter)continue;
			pChapter = (XWordDataChapter*)(*pBlock->m_pDataParent->m_pDataParent->m_pDataParent->m_pChilds)[i];
			for(int c=(int)pChapter->m_pChilds->size() - 1;c>=0;c--)
			{
				if((*pChapter->m_pChilds)[c]->eWordDataType!=_XWordDataParagraph)continue;
				pParagraph = (XWordDataContentParagraph*)(*pChapter->m_pChilds)[c];
				for(int k=(int)pParagraph->m_pChilds->size() - 1 ;k>=0;k--)
				{
					if((*pParagraph->m_pChilds)[k]->eWordDataType==_XWordDataTextBlock)
					return (XWordDataTextBlock *)(*pParagraph->m_pChilds)[k];
				}
			}
		}
		return NULL;
	}
	
	XWordDataTextBlock* XFlowDocument::GetNextTextBlock(XWordDataTextBlock  * pBlock)
	{
		int index = this->GetIndexFromParent(pBlock);
		for(int i=index + 1;i<(int)pBlock->m_pDataParent->m_pChilds->size();i++)
		{
			if((*pBlock->m_pDataParent->m_pChilds)[i]->eWordDataType==_XWordDataTextBlock)
				return (XWordDataTextBlock *)(*pBlock->m_pDataParent->m_pChilds)[i];
		}

		index = GetIndexFromParent(pBlock->m_pDataParent);
		XWordDataContentParagraph * pParagraph;
		XWordDataChapter * pChapter;
		for(int i=index+1;i<(int)pBlock->m_pDataParent->m_pDataParent->m_pChilds->size();i++)
		{
			if((*pBlock->m_pDataParent->m_pDataParent->m_pChilds)[i]->eWordDataType!=_XWordDataParagraph)continue;
			pParagraph = (XWordDataContentParagraph*)(*pBlock->m_pDataParent->m_pDataParent->m_pChilds)[i];
			for(int k=0;k<=(int)pParagraph->m_pChilds->size() - 1;k++)
			{
				if((*pParagraph->m_pChilds)[k]->eWordDataType==_XWordDataTextBlock)
				return (XWordDataTextBlock *)(*pParagraph->m_pChilds)[k];
			}
		}

		index = GetIndexFromParent(pBlock->m_pDataParent->m_pDataParent);
		for(int i=index+1;i<(int)pBlock->m_pDataParent->m_pDataParent->m_pDataParent->m_pChilds->size();i++)
		{
			if((*pBlock->m_pDataParent->m_pDataParent->m_pDataParent->m_pChilds)[i]->eWordDataType!=_XWordDataChapter)continue;
			pChapter = (XWordDataChapter*)(*pBlock->m_pDataParent->m_pDataParent->m_pDataParent->m_pChilds)[i];
			for(int c=0;c<=(int)pChapter->m_pChilds->size() - 1;c++)
			{
				if((*pChapter->m_pChilds)[c]->eWordDataType!=_XWordDataParagraph)continue;
				pParagraph = (XWordDataContentParagraph*)(*pChapter->m_pChilds)[c];
				for(int k=0;k<=(int)pParagraph->m_pChilds->size() - 1 ;k++)
				{
					if((*pParagraph->m_pChilds)[k]->eWordDataType==_XWordDataTextBlock)
					return (XWordDataTextBlock *)(*pParagraph->m_pChilds)[k];
				}
			}
		}

		return NULL;
	}
	
	int XFlowDocument::RemoveBlock(XWordDataBase   * pBlock)
	{
		if(m_pContent->m_pChilds->size()==1 &&
			(*m_pContent->m_pChilds)[0]->m_pChilds->size()==1 && 
			(*(*m_pContent->m_pChilds)[0]->m_pChilds)[0]->m_pChilds->size()==1) return 1;
		int nIndex = GetIndexFromParent( pBlock);
	
		XWordDataContentParagraph * pPara;
		XWordDataChapter * pChapter;
		pPara = (XWordDataContentParagraph *)pBlock->m_pDataParent;

		delete pBlock;

		pPara->m_pChilds->erase(nIndex);
		pChapter = (XWordDataChapter*)pPara->m_pDataParent;
		if(pPara->m_pChilds->size()==0)
		{
			nIndex = GetIndexFromParent(pPara);
			delete pPara;
			pChapter->m_pChilds->erase(nIndex);
		}
		if(pChapter->m_pChilds->size()==0)
		{
			nIndex = GetIndexFromParent(pChapter);
			delete pChapter;
			m_pContent->m_pChilds->erase(nIndex);
		}else
		{
			//pChapter->AdjustExtent();
		}
		return 1;
	}

	bool XFlowDocument::MerginPara(XWordDataContentParagraph * pDestPara,XWordDataContentParagraph * pSrcPara)
	{
		for(int i=0;i<(int)pSrcPara->m_pChilds->size();i++)
		{
			(*pSrcPara->m_pChilds)[i]->m_pDataParent = pDestPara;
			pDestPara->m_pChilds->push_back((*pSrcPara->m_pChilds)[i]);
		}
		
		int nIndex = GetIndexFromParent(pSrcPara);
		pSrcPara->m_pDataParent->m_pChilds->erase(	nIndex);	
		
		pSrcPara->m_pChilds->clear();
		delete pSrcPara;
		
		return true;
	}
	
	int XFlowDocument::AdjustExtent()
	{
		HDC hDC = GetDC(m_pSheetSvr->GetHWND());
		this->CalcExtent((XWordDataSvr *)m_pSheetSvr->m_pDataSvr,NULL,hDC);
		::ReleaseDC(m_pSheetSvr->GetHWND(),hDC);
		return 1;
	}

	bool XFlowDocument::AdjustSelection(XWordCursorInfoEx & CursorInfo,TShiftState state)
	{
		bool bRedraw = false;
		if(state & ssShift)
		{
			m_Selection.bSelection = true;
			int nResult = -1;
			if(!m_Selection.bLast)
			{
				//if(m_Selection.nStartRow > CursorInfo.nRow || (m_Selection.nStartRow == CursorInfo.nRow && m_Selection.nStartCol > CursorInfo.nCol))
				//	nResult = 1;
				if(GetIndex(m_Selection.pStartBlock) > GetIndex(CursorInfo.pBlock)
					|| (GetIndex(m_Selection.pStartBlock) == GetIndex(CursorInfo.pBlock) && m_Selection.nStartPosChar > CursorInfo.nIndex))
					nResult = 1;
			}else
			{
				//if(m_Selection.nEndRow > CursorInfo.nRow || (m_Selection.nEndRow == CursorInfo.nRow && m_Selection.nEndCol > CursorInfo.nCol))
				//	nResult = 1;
				if(GetIndex(m_Selection.pEndBlock) > GetIndex(CursorInfo.pBlock)
					|| (GetIndex(m_Selection.pEndBlock) == GetIndex(CursorInfo.pBlock) && m_Selection.nEndPosChar > CursorInfo.nIndex))
					nResult = 1;
			}
			if(nResult < 0)
			{
				if(m_Selection.bLast)
				{
					m_Selection.pStartBlock = m_Selection.pEndBlock;
					m_Selection.nStartPosChar = m_Selection.nEndPosChar;
					m_Selection.bLast = false;
				}
				m_Selection.pEndBlock =  CursorInfo.pBlock;
				m_Selection.nEndPosChar = CursorInfo.nIndex;
			}
			if(nResult > 0)
			{
				if(!m_Selection.bLast)
				{
					m_Selection.pEndBlock	= m_Selection.pStartBlock;
					m_Selection.nEndPosChar = m_Selection.nStartPosChar;
					m_Selection.bLast = true;
				}
				m_Selection.pStartBlock =  CursorInfo.pBlock;
				m_Selection.nStartPosChar = CursorInfo.nIndex;
			}
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		}else
		{
			if(m_Selection.bSelection)bRedraw =true;
			m_Selection.pStartBlock =  CursorInfo.pBlock;
			m_Selection.nStartPosChar = CursorInfo.nIndex;
			m_Selection.pEndBlock	= m_Selection.pStartBlock;
			m_Selection.nEndPosChar = m_Selection.nStartPosChar;
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		}

		if(m_Selection.pStartBlock == m_Selection.pEndBlock && m_Selection.nStartPosChar == m_Selection.nEndPosChar)m_Selection.bSelection = false;

		return true;
	}

	bool XFlowDocument::BlockIsSelected(XWordDataTextBlock * pBlock,XWordDataTextBlock * pStartBlock,XWordDataTextBlock * pEndBlock)
	{
		if(!m_Selection.bSelection)return false;
		if(GetIndex(pBlock)<GetIndex(pStartBlock))return false;
		if(GetIndex(pBlock)>GetIndex(pEndBlock))return false;
		return true;
	}

	bool XFlowDocument::CursorInfoInSelection(XWordCursorInfoEx & CursorInfo)
	{
		bool bInSelection = false;
		/**if(CursorInfo.bControlParagraph) return false;
		XWordBlock * pBlock = ((XWordParagraphBlock *)CursorInfo.m_pParagraphBlock)->m_rowInfos[CursorInfo.nRow].m_rowBlocks[CursorInfo.nBlock].pBlock;
		if(m_Selection.bSelection && BlockIsSelected(pBlock))
		{
			bInSelection = true;
			if(pBlock==m_Selection.pStartBlock && CursorInfo.nPosChar < m_Selection.nStartPosChar) bInSelection = false;
			if(pBlock==m_Selection.pEndBlock && CursorInfo.nPosChar > m_Selection.nEndPosChar) bInSelection = false;
		}
		**/
		return bInSelection;
	}
	
	void XFlowDocument::SetFontBold()
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		
		XWordDataTextBlock * pStartBlock;
		XWordDataTextBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordDataTextBlock * pTextBlock = pStartBlock;
		XWordDataTextBlock * pEndTextBlock = pEndBlock;

		LOGFONT logFont;
		bool bFirst = true;
		bool bBold = false;
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
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
			
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle) delete pBoxStyle;
			pTextBlock->m_nIndexStyle = nStyleIndex;
			//pTextBlock->m_nCssStyle = nStyleIndex;
			
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = GetNextTextBlock(pTextBlock);
		}
		//pTextBlock->GetDocumentBlock()->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);

		if(m_Selection.bLast)
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	void XFlowDocument::SetFontItalic()
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		
		XWordDataTextBlock * pStartBlock;
		XWordDataTextBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordDataTextBlock * pTextBlock = pStartBlock;
		XWordDataTextBlock * pEndTextBlock = pEndBlock;

		LOGFONT logFont;
		bool bFirst = true;
		bool bItalic = false;
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
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
			
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle) delete pBoxStyle;
			pTextBlock->m_nIndexStyle = nStyleIndex;
			//pTextBlock->m_nCssStyle = nStyleIndex;
			
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = GetNextTextBlock(pTextBlock);
		}
		//pTextBlock->GetDocumentBlock()->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);

		if(m_Selection.bLast)
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	void XFlowDocument::SetFontUnderline()
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		
		XWordDataTextBlock * pStartBlock;
		XWordDataTextBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordDataTextBlock * pTextBlock = pStartBlock;
		XWordDataTextBlock * pEndTextBlock = pEndBlock;

		LOGFONT logFont;
		bool bFirst = true;
		bool bUnderline = false;
		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
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
			
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle) delete pBoxStyle;
			pTextBlock->m_nIndexStyle = nStyleIndex;
			//pTextBlock->m_nCssStyle = nStyleIndex;
			
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = GetNextTextBlock(pTextBlock);
		}
		//pTextBlock->GetDocumentBlock()->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);

		if(m_Selection.bLast)
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	void XFlowDocument::SetFont(LOGFONT &logFont,COLORREF color)
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		
		XWordDataTextBlock * pStartBlock;
		XWordDataTextBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordDataTextBlock * pTextBlock = pStartBlock;
		XWordDataTextBlock * pEndTextBlock = pEndBlock;

		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);
			
			XUtil::Css::SetFont(pBoxStyle,logFont);
			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle)delete pBoxStyle;
			pTextBlock->m_nIndexStyle = nStyleIndex;
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = GetNextTextBlock(pTextBlock);
		}
		//pTextBlock->GetDocumentBlock()->AdjustExtent();
		//m_nPage = CalcPages();
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);

		if(m_Selection.bLast)
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	bool XFlowDocument::ChooseFontDlg()
	{
		CHOOSEFONT cf; 
		LOGFONT	   logFont;
		COLORREF	Color = -1;

		XWordDataTextBlock * pTextBlock = (XWordDataTextBlock *)m_Selection.pStartBlock;
		const XUtil::Css::TBoxStyle * pBoxStyle = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle);
		ZeroMemory(&cf,sizeof(CHOOSEFONT));

		XUtil::Css::GetFont(pBoxStyle,logFont);
		if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
		{
			Color = pBoxStyle->pColor->color[0];
		}

		cf.lStructSize = sizeof(CHOOSEFONT); 
		cf.hwndOwner = GetParent(m_pSheetSvr->m_pSheetCtrl->m_hWnd); 
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

			return TRUE;
		}
		return false;
	}

	bool XFlowDocument::ChooseColorDlg(DWORD flag)
	{
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
		cc.hwndOwner = GetParent(m_pSheetSvr->m_pSheetCtrl->m_hWnd); 
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
			return TRUE;
		}
		return FALSE;
	}
	void XFlowDocument::SetColor(COLORREF color, DWORD flag)
	{
		if(!m_Selection.bSelection) return; //should todo add new block
		
		XWordDataTextBlock * pStartBlock;
		XWordDataTextBlock * pEndBlock;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordDataTextBlock * pTextBlock = pStartBlock;
		XWordDataTextBlock * pEndTextBlock = pEndBlock;

		while(pTextBlock)
		{
			XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
			if(m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(pTextBlock->m_nIndexStyle); 
			if(!pBoxStyle &&  m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0))
				*pBoxStyle = *m_pSheetSvr->m_pDataSvr->m_pStyleSvr->GetBoxStyle(0);

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

			int nStyleIndex = m_pSheetSvr->m_pDataSvr->m_pStyleSvr->SetCssStyle(pBoxStyle);
			if(pBoxStyle)delete pBoxStyle;
			pTextBlock->m_nIndexStyle = nStyleIndex;
			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = GetNextTextBlock(pTextBlock);
		}
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		ShowCurrentCaret(true);
	}
	void XFlowDocument::SetAlign(DWORD flag)
	{
		if(!m_Selection.bSelection)
		{
			if(m_CursorInfo.pBlock) m_CursorInfo.pBlock->m_pDataParent->align = flag;
		}else
		{
			m_Selection.pStartBlock->m_pDataParent->align = flag;
			if(m_Selection.pStartBlock!=m_Selection.pEndBlock)
			{
				XWordDataTextBlock * pBlock = (XWordDataTextBlock *)m_Selection.pStartBlock; 
				pBlock = GetNextTextBlock(pBlock);
				while(pBlock!= m_Selection.pEndBlock && pBlock)
				{
					pBlock->m_pDataParent->align = flag;
					pBlock = GetNextTextBlock( pBlock);
				}
				m_Selection.pEndBlock->m_pDataParent->align = flag;
			}
		}

		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
		if(m_Selection.bLast)
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nStartRow ,m_Selection.nStartCol);
		}else
		{
			//CalcCursorInfo(m_CursorInfo,m_Selection.nEndRow ,m_Selection.nEndCol);
		}
		ShowCurrentCaret(true);
	}

	bool XFlowDocument::StandOnlySelection(XWordSelectionEx &Selection)
	{
		XWordDataTextBlock * pStartBlock =NULL,* pEndBlock = NULL;
		return StandOnlySelection(Selection,pStartBlock,pEndBlock);
	}
	bool XFlowDocument::StandOnlySelection(XWordSelectionEx &Selection,XWordDataTextBlock * &pStartBlockEx,XWordDataTextBlock * &pEndBlockEx)
	{
		//sparse start block;
		int nStartPosChar,nEndPosChar;
		XWordDataTextBlock * pStartBlock ,* pEndBlock;
		XWordDataTextBlock * pContentBlock;

		nStartPosChar = Selection.nStartPosChar;
		pStartBlock = (XWordDataTextBlock *)Selection.pStartBlock;
		pContentBlock = pStartBlock;
		if(nStartPosChar !=0)
		{
			XWordDataTextBlock *  pNewDataTextBlock = new XWordDataTextBlock();
			pNewDataTextBlock->m_pStrText = new TCHAR[::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1];
			::_tcscpy_s(pNewDataTextBlock->m_pStrText,::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1,pContentBlock->m_pStrText + nStartPosChar);
			pNewDataTextBlock->m_nIndexStyle = pContentBlock->m_nIndexStyle;
			pNewDataTextBlock->m_pDataParent = pContentBlock->m_pDataParent;
			//pNewDataTextBlock->m_nCssStyle = pContentBlock->m_nCssStyle;

			int nLenEx = (int)::_tcslen(pContentBlock->m_pStrText) - nStartPosChar;
			pContentBlock->m_pStrText[nStartPosChar]='\0';
			if(nLenEx > 64)
			{
				LPTSTR pStrTemp = new TCHAR[ ::_tcslen(pContentBlock->m_pStrText) + 1];
				::_tcscpy_s(pStrTemp,::_tcslen(pContentBlock->m_pStrText) + 1,pContentBlock->m_pStrText);
				delete pContentBlock->m_pStrText;
				pContentBlock->m_pStrText = pStrTemp;
				pContentBlock->nStrLimit = 0;
			}
			pContentBlock->m_pDataParent->m_pChilds->insert(GetIndexFromParent(pContentBlock) + 1,pNewDataTextBlock);

			//pTextBlock->m_pBlockParent->AdjustExtent();
			pStartBlockEx = pNewDataTextBlock;

			if(Selection.pEndBlock == pStartBlock)
			{
				Selection.pEndBlock = pStartBlockEx;
				Selection.nEndPosChar -= Selection.nStartPosChar;
			}
			Selection.nStartPosChar = 0;
			Selection.pStartBlock = pStartBlockEx;
		}else
			pStartBlockEx = pStartBlock;

		//sparse end block;
		nEndPosChar = Selection.nEndPosChar;
		pEndBlock = (XWordDataTextBlock *)Selection.pEndBlock;
		pContentBlock = pEndBlock;
		if(nEndPosChar != ::_tcslen(pContentBlock->m_pStrText))
		{
			XWordDataTextBlock *  pNewDataTextBlock = new XWordDataTextBlock();
			pNewDataTextBlock->m_pStrText = new TCHAR[::_tcslen(pContentBlock->m_pStrText + nEndPosChar) + 1];
			::_tcscpy_s(pNewDataTextBlock->m_pStrText,::_tcslen(pContentBlock->m_pStrText + nEndPosChar) + 1,pContentBlock->m_pStrText + nEndPosChar);
			pNewDataTextBlock->m_nIndexStyle = pContentBlock->m_nIndexStyle;
			pNewDataTextBlock->m_pDataParent = pContentBlock->m_pDataParent;
			//pNewDataTextBlock->m_nCssStyle = pContentBlock->m_nCssStyle;

			int nLenEx = (int)::_tcslen(pContentBlock->m_pStrText) - nEndPosChar;
			pContentBlock->m_pStrText[nEndPosChar]='\0';
			if(nLenEx > 64)
			{
				LPTSTR pStrTemp = new TCHAR[ ::_tcslen(pContentBlock->m_pStrText) + 1];
				::_tcscpy_s(pStrTemp,::_tcslen(pContentBlock->m_pStrText) + 1,pContentBlock->m_pStrText);
				delete pContentBlock->m_pStrText;
				pContentBlock->m_pStrText = pStrTemp;
				pContentBlock->nStrLimit = 0;
			}
			pContentBlock->m_pDataParent->m_pChilds->insert(GetIndexFromParent(pContentBlock) + 1,pNewDataTextBlock);
			//pTextBlock->m_pBlockParent->AdjustExtent();
		}
		pEndBlockEx = pEndBlock;
		return true;
	}

	bool XFlowDocument::StandOnlyToPara(int nRow,int nCol)
	{
		int nStartPosChar;
		XWordDataTextBlock * pStartBlock;
		XWordDataTextBlock * pContentBlock;

		nStartPosChar = m_CursorInfo.nIndex;
		pStartBlock = (XWordDataTextBlock *)m_CursorInfo.pBlock;
		pContentBlock = pStartBlock;
		if(nStartPosChar !=0)
		{
			XWordDataTextBlock *  pNewDataTextBlock = new XWordDataTextBlock();
			pNewDataTextBlock->m_pStrText = new TCHAR[::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1];
			::_tcscpy_s(pNewDataTextBlock->m_pStrText,::_tcslen(pContentBlock->m_pStrText + nStartPosChar) + 1,pContentBlock->m_pStrText + nStartPosChar);
			pNewDataTextBlock->m_nIndexStyle = pContentBlock->m_nIndexStyle;
			pNewDataTextBlock->m_pDataParent = pContentBlock->m_pDataParent;
			//pNewDataTextBlock->m_nCssStyle = pContentBlock->m_nCssStyle;

			int nLenEx = (int)::_tcslen(pContentBlock->m_pStrText) - nStartPosChar;
			pContentBlock->m_pStrText[nStartPosChar]='\0';
			if(nLenEx > 64)
			{
				LPTSTR pStrTemp = new TCHAR[ ::_tcslen(pContentBlock->m_pStrText) + 1];
				::_tcscpy_s(pStrTemp,::_tcslen(pContentBlock->m_pStrText) + 1,pContentBlock->m_pStrText);
				delete pContentBlock->m_pStrText;
				pContentBlock->m_pStrText = pStrTemp;
				pContentBlock->nStrLimit = 0;
			}

			pContentBlock->m_pDataParent->m_pChilds->insert(GetIndexFromParent(pContentBlock) + 1,pNewDataTextBlock);
			pStartBlock = pNewDataTextBlock;
		}
		
		int nIndex = GetIndexFromParent(pStartBlock);
		if(nIndex==0)
		{
			XWordDataTextBlock * pTextBlock = new XWordDataTextBlock();
			pTextBlock->m_nIndexStyle = pStartBlock->m_nIndexStyle;

			XWordDataParagraph * pPara = new XWordDataParagraph();

			pTextBlock->m_pDataParent = pPara;
			pPara->m_pChilds->push_back(pContentBlock);

			pPara->m_pDataParent = pStartBlock->m_pDataParent->m_pDataParent;

			int nParaIndex = GetIndexFromParent( pContentBlock->m_pDataParent);
			pStartBlock->m_pDataParent->m_pDataParent->m_pChilds->insert(nParaIndex,pPara);
			//pStartBlock->m_pDataParent->m_pDataParent->AdjustExtent();
		}
		else if(nIndex>0)
		{
			XWordDataParagraph * pPrevPara = (XWordDataParagraph *)pStartBlock->m_pDataParent;
			XWordDataParagraph * pPara = new XWordDataParagraph();
			pPara->m_pDataParent = pPrevPara->m_pDataParent;
			for(int i=nIndex;i<(int)pPrevPara->m_pChilds->size();i++)
			{
				pPara->m_pChilds->push_back((*pPrevPara->m_pChilds)[i]);
				(*pPrevPara->m_pChilds)[i]->m_pDataParent = pPara;
			}

			while((int)pPrevPara->m_pChilds->size()> nIndex)
			{
				pPrevPara->m_pChilds->erase(nIndex);
			}

			int nParaIndex = GetIndexFromParent(pPrevPara);
			pPrevPara->m_pDataParent->m_pChilds->insert(nParaIndex + 1,pPara);
			//pPrevPara->m_pBlockParent->AdjustExtent();
		}
		
		return true;
	}

	int XFlowDocument::DoCopy()
	{
		if(!m_Selection.bSelection) return 0; //should todo add new block

		TCHAR* lpdata;
		HGLOBAL        hClipGlobal;
		hClipGlobal = GlobalAlloc(0,128000);
		lpdata = (TCHAR *)GlobalLock(hClipGlobal);
		ZeroMemory(lpdata,128000);


		XWordDataTextBlock * pStartBlock = (XWordDataTextBlock *)m_Selection.pStartBlock;
		XWordDataTextBlock * pEndBlock = (XWordDataTextBlock *)m_Selection.pEndBlock;
		//StandOnlySelection(m_Selection,pStartBlock,pEndBlock);
		XWordDataTextBlock * pTextBlock = pStartBlock;
		XWordDataTextBlock * pEndTextBlock = pEndBlock;

		bool bFirst = true;
		bool bBold = false;

		XWordDataBase* pPrevParentBlock = NULL;
		while(pTextBlock)
		{
			if(!pPrevParentBlock)pPrevParentBlock = pTextBlock->m_pDataParent;
			if(pPrevParentBlock != pTextBlock->m_pDataParent)
				_tcscat_s(lpdata,64000,_T("\r\n"));
			if(pTextBlock->m_pStrText)
			{
				if(pTextBlock==pStartBlock)
				{
					if(pEndBlock!=pStartBlock)
						_tcscat_s(lpdata,64000,pTextBlock->m_pStrText+ m_Selection.nStartPosChar);
					else
						_tcsncpy_s(lpdata + _tcslen((TCHAR*)lpdata),64000 - _tcslen((TCHAR*)lpdata),
							pTextBlock->m_pStrText+ m_Selection.nStartPosChar,
							m_Selection.nEndPosChar - m_Selection.nStartPosChar);
				}else
				{
					if(pTextBlock!=pEndBlock)
					{
						_tcscat_s(lpdata,64000,pTextBlock->m_pStrText);
					}else
						_tcsncpy_s(lpdata + _tcslen((TCHAR*)lpdata),64000 - _tcslen((TCHAR*)lpdata),
							pTextBlock->m_pStrText,	m_Selection.nEndPosChar);
				}
			}

			if(pTextBlock == pEndTextBlock)break;
			pTextBlock = GetNextTextBlock(pTextBlock);
		}

		GlobalUnlock(hClipGlobal);
		
		if(OpenClipboard(m_pSheetSvr->GetHWND()))
		{
			SetClipboardData(CF_UNICODETEXT,hClipGlobal);
			CloseClipboard();
		}
		
		return 1;
	}

	int XFlowDocument::DoCut()
	{
		
		DoCopy();
		if(!m_Selection.bSelection) return 0; //should todo add new block

		XWordDataTextBlock * pStartBlock;
		XWordDataTextBlock * pEndBlock;
		XWordDataTextBlock * pBlock = NULL;
		StandOnlySelection(m_Selection,pStartBlock,pEndBlock);

		if(m_pContent->m_pChilds->size()==1 &&
			(*m_pContent->m_pChilds)[0]->m_pChilds->size()==1 && 
			(*(*m_pContent->m_pChilds)[0]->m_pChilds)[0]->m_pChilds->size()==1)
		{
			XWordDataTextBlock * pTextBlock = pStartBlock;
			if(pTextBlock)
			{
				if(pTextBlock->m_pStrText)
					delete pTextBlock->m_pStrText;
				pTextBlock->m_pStrText = NULL;
				//pTextBlock->AdjustExtent();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&FRect,false);
				//CalcCursorInfo(m_CursorInfo,1,1);
				ShowCurrentCaret(true);
			}
			return 1;
		}

		bool bbreak = true;
		while(bbreak && pStartBlock)
		{
			if(pStartBlock == pEndBlock)bbreak = false;
			pBlock = pStartBlock;
			int nIndex = GetIndexFromParent(pBlock);
			XWordDataParagraph * pPara;
			XWordDataChapter * pChater;
			pPara = (XWordDataParagraph *)pBlock->m_pDataParent;
			delete pBlock;
			pPara->m_pChilds->erase(nIndex);
			pChater = (XWordDataChapter *)pPara->m_pDataParent;

			int nPara = GetIndexFromParent(pPara);
			bool bPara = true;
			if(pPara->m_pChilds->size()==0)
			{
				nIndex = GetIndexFromParent(pPara);
				delete pPara;
				pChater->m_pChilds->erase(nIndex);
				bPara  = false;
			}else if(nIndex == pPara->m_pChilds->size())
				bPara  = false;
			int nChater = GetIndexFromParent(pChater);
			bool bChater = true;
			if(pChater->m_pChilds->size()==0)
			{
				nIndex =GetIndexFromParent(pChater);
				delete pChater;
				m_pContent->m_pChilds->erase(nIndex);
				bChater =false;
			}else if(nPara == pChater->m_pChilds->size())
			{
				bChater =false;
			}

			if(!pEndBlock) break;
			if(bPara)
				pStartBlock = (XWordDataTextBlock *)(*pPara->m_pChilds)[nIndex];
			else if(bChater)
			{
				pStartBlock =  (XWordDataTextBlock *)(*((*pChater->m_pChilds)[nPara]->m_pChilds))[0];
			}else
				(*((*(*m_pContent->m_pChilds)[0]->m_pChilds)[0]->m_pChilds))[0];
		}
		m_Selection.bSelection = false;
		//m_pWordDocumentBlock->AdjustExtent();
		::InvalidateRect(m_pSheetSvr->GetHWND(),&FRect,false);
		//CalcCursorInfo(m_CursorInfo,1,1);
		ShowCurrentCaret(true);
		
		return 1;
	}

	int XFlowDocument::DoPaste()
	{
		
		HANDLE        hClipGlobal;
		void *       lpbi;
		int         length;

		OpenClipboard(this->m_pSheetSvr->m_pSheetCtrl->m_hWnd);
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
				
				int nCharPos = m_CursorInfo.nIndex;
				XWordDataTextBlock* pBlock = m_CursorInfo.pBlock ;

				int nLimit;
				int nStrLen;
				LPTSTR pStrTemp;
				LPTSTR pStr = pBlock->m_pStrText;
				if(!pStr)
				{
					pStr = new TCHAR[length/2];
					::_tcscpy_s(pStr,length/2,(LPTSTR)lpdata);
					pBlock->nStrLimit = length/2 + 1;
					pBlock->m_pStrText = pStr;
				}else
				{
					nStrLen = (int)::_tcslen(pStr) + length/2;
					nLimit = pBlock->nStrLimit;
					if(nLimit == 0)
						nLimit =(int) nStrLen + 3 + 32;
					else
						nLimit += length/2 + 32;
					pStrTemp = new TCHAR[nLimit];
					pBlock->nStrLimit = nLimit;

					if(pStrTemp)
					{
						::_tcsncpy_s(pStrTemp,nLimit,pStr,nCharPos);
						::_tcscpy_s(pStrTemp + nCharPos,length/2 ,(LPTSTR)lpdata);

						int nLen = (int)::_tcslen(pStr);
						::_tcscpy_s(pStrTemp + nCharPos + length/2 - 1,::_tcslen(pStr) - nCharPos + 1,pStr + nCharPos);
						delete pStr;
						pStr = pStrTemp;
						pBlock->m_pStrText = pStr;
					}
				}

				GlobalUnlock(hData);
				GlobalFree(hData);

				GlobalUnlock( hClipGlobal );
				
				//pTextBlock->m_pBlockParent->m_pBlockParent->AdjustExtent();
				//m_nPage = CalcPages();
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->m_hWnd,&m_pSheetSvr->FRect,false);
				this->CalcRightCursorInfo(m_CursorInfo);
				ShowCurrentCaret(true);
			}
		}
		CloseClipboard();
		
		return 1;
	}

	int XFlowDocument::CalcContentPageRect(int nPage,RECT * pDrawRect,RECT &rc)
	{
		if(!pDrawRect) return 0;
		RECT rect = *pDrawRect;

		RECT rcPage;
		if( rect.right - rect.left < m_rcPage.right - m_rcPage.left)
			rcPage.left = rect.left + 5;
		else
			rcPage.left = rect.left + (rect.right - rect.left)/2 - (m_rcPage.right - m_rcPage.left)/2;
		rcPage.top = rect.top + 20;
		rcPage.right = rcPage.left + m_rcPage.right - m_rcPage.left;
		rcPage.bottom = rcPage.top + m_rcPage.bottom - m_rcPage.top;

		rc = rcPage;
		rc.top += (rcPage.bottom - rcPage.top + 1)* nPage + nPage * 20;
		rc.bottom = rc.top + rcPage.bottom - rcPage.top;

		rc.left += m_rcPageMargin.left;
		rc.top += m_rcPageMargin.top;
		rc.right -= m_rcPageMargin.right;
		rc.bottom -= m_rcPageMargin.bottom;

		return 1;
	}
	
	bool XFlowDocument::ViewConer()
	{
		if(m_nView & WVIEW_NODRAWRULER)return false;
		if(m_nView==WVIEW_READER) return false;
		return true;
	}
	bool XFlowDocument::ViewHRuler()
	{
		if(m_nView & WVIEW_NODRAWRULER)return false;
		if(m_nView==WVIEW_READER) return false;
		return true;

	}
	bool XFlowDocument::ViewVRuler()
	{
		if(m_nView & WVIEW_NODRAWRULER)return false;
		if(m_nView==WVIEW_READER) return false;
		if(m_nView==WVIEW_PAGE) return true;
		return false;

	}

}}}}