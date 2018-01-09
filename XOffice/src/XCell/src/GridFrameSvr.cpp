#include "stdafx.h"
#include <gdiplus.h>
#include "gridframesvr.hpp"
#include "XCellSheetSvr.hpp"
#include "XScrollBar.hpp"
#include "colheadersvr.hpp"
#include "rowselectorsvr.hpp"
#include "xcellserializeSvr.hpp"
#include "xbxbase.h"
#include "DwTable.hpp"
#include "xcellsheetsvr.hpp"
#include "cellprop.hpp"
#include "xuxutil.h"
#include "XCellQueryDlg.hpp"
#include "HxComplier.h"
#include "XGridComplier.hpp"

using namespace Gdiplus;

extern HMODULE hCellModula;
#define ID_TRACETIMER 10018
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CGridFrameSvr::CGridFrameSvr(void):m_RowCount(32000),m_ColCount(255),m_pSheetSvr(NULL)

	{
		::SetRect(&FRect,0,0,0,0);

		m_pEditSvr = new CEditSvr();
		m_pEditSvr->m_pGridSvr = this;

		Initialize();
		SetEventListons();
	}

	int CGridFrameSvr::SetFRect(RECT &rect)
	{
		FRect = rect;
		return 1;
	}

	CGridDataSvr*	CGridFrameSvr::GetGridDataSvr()
	{
		return dynamic_cast<CGridDataSvr*>(this->m_pSheetSvr->m_pDataSvr);
	}


	void CGridFrameSvr::Initialize()
	{
		HandleAllocated = true;
		FEditorMode = false;

		FCanEditModify = true;
		FInplaceCol = -1;
		FInplaceRow = -1;

		FShowOrigin = false;

		FGridLineWidth = 1;
		FOptions = /*goRowSelect|goEditing|*/goThumbTracking|goVisibleSelection|goAllowEdit|goFixedVertLine|goFixedHorzLine|goVertLine|goHorzLine|goRangeSelect|goColSizing|goRowSizing|goDrawFixed|goDrawBound/*|goAlwaysShowEditor*/;
		FOptions = FOptions|goRowMoving|goColMoving;
		FScrollBars =ssBoth;
		FBorderStyle = bsSingle;
		FGridState = gsNormal;

		FTopLeft.x = 1;
		FTopLeft.y = 1;
		FCurrent = FTopLeft;
		FAnchor = FCurrent;
		FFixedCols = 1;
		FFixedRows = 1;
		if(goRowSelect & FOptions)FAnchor.x = GetColCount() ;

		m_ptStart.x = -1;
		m_ptStart.y = -1;

		FMouseTrace = false;
		m_ptTrace.x = -1;
		m_ptTrace.y = -1;
		m_nDelay = 500;
		m_TipHWND = NULL;

		m_ptPosition.x = -1;
		m_ptPosition.y = -1;

		m_EditUpperMode = false;
	}

	CGridFrameSvr::~CGridFrameSvr(void)
	{
		if(m_pEditSvr) delete m_pEditSvr;
		UnSetEventListons();
	}

	bool CGridFrameSvr::SetEventListons()
	{
		//this->AttachListonTo(pDest,WM_PAINT,(FNEvent)&CGridFrameSvr::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&CGridFrameSvr::EvtLButtonDown);
		this->AttachListon(WM_RBUTTONDOWN,(FNEvent)&CGridFrameSvr::EvtRButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&CGridFrameSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&CGridFrameSvr::EvtMouseMove);

		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&CGridFrameSvr::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&CGridFrameSvr::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&CGridFrameSvr::EvtHScroll);

		this->AttachListon(WM_SIZE,(FNEvent)&CGridFrameSvr::EvtSize);
		this->AttachListon(WM_CHAR,(FNEvent)&CGridFrameSvr::EvtChar);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&CGridFrameSvr::EvtKeyDown);
		this->AttachListon(WM_SETFOCUS,(FNEvent)&CGridFrameSvr::EvtSetFocus);
		this->AttachListon(WM_KILLFOCUS,(FNEvent)&CGridFrameSvr::EvtKillFocus);
		this->AttachListon(WM_XCOMMAND,(FNEvent)&CGridFrameSvr::EvtXCommand);
		this->AttachListon(WM_TIMER,(FNEvent)&CGridFrameSvr::EvtTimer);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&CGridFrameSvr::EvtSetCursor);
		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&CGridFrameSvr::EvtDoubleClicked);
		return true;
	}

	bool CGridFrameSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}


	bool CGridFrameSvr::EvtDoubleClicked(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		UINT nFlags = pEvent->mouse.nFlags;
		if(::GetFocus() != this->GetHWND())::SetFocus(GetHWND());
		TGridCoord CellHit;
		CellHit = CalcCoordFromPoint(point.x,point.y);
		if(GetGridDataSvr()->m_enView==CGridDataSvr::envGridReportDesign || GetGridDataSvr()->m_enView==CGridDataSvr::envPrintDesign)
		{
			CReportBandSvr::TReportBandItem* pBandItem;
			int nGroup;
			if(GetGridDataSvr()->m_ReportBand->GetReportBand(CellHit.y,pBandItem,nGroup))
			{
				if(pBandItem->endRow == CellHit.y) return true;
			}
		}
		GetGridDataSvr()->m_pSpanSvr->GetSpanedRowCol(CellHit.x,CellHit.y,CellHit.x,CellHit.y);

		if(this->FOptions & goRunState)
		{
			if(!GetGridDataSvr()->GetDwTableFromCell(CellHit.x,CellHit.y))
			{
				HideEditor();
				::InvalidateRect(this->m_pSheetSvr->m_pSheetCtrl->GetHWND(),NULL,false);
				return true;
			}
		}

		FOldAnchor = FAnchor;

		this->SetCapture();
		if(this->FGridState  == gsDrawLining || this->FGridState == gsDeleteLining)
		{
			this->m_ptStart = point;
			this->m_ptMouse = point;
			return true;
		}

		if(FEditorMode)
			HideEditor();

		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_DOUBLECLICKED;
		hdr.DwTable = NULL;
		hdr.row = CellHit.y;
		hdr.col = CellHit.x;	
		PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(CellHit.x,CellHit.y);
		if(pItem && pItem->pStrColumn)
			hdr.colname = pItem->pStrColumn;
		if(pItem && pItem->pStrContent)
			hdr.data = pItem->pStrContent;
		else
			hdr.data = _T("");
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		return true;
	}
	bool CGridFrameSvr::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		HCURSOR hCursor = NULL;
		if(FGridState == gsDrawLining)
			hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_PEN));
		else if(FGridState == gsDeleteLining)
			hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_CLEARLINE));
		else
		{
			POINT point = pEvent->pt;
			ScreenToClient(GetHWND(),&point);

			TGridState State;
			if(FGridState == gsNormal )
			{
				State = CalcSizingState(point,FRect);
				TGridRect sRect = this->GetSelection();
				TGridRect sRectEx;
				if(State != gsCellsMoving)
				{
					if(sRect.left==sRect.right && sRect.top==sRect.bottom)State = gsNormal;
					this->GetGridDataSvr()->m_pSpanSvr->isSpaned(sRect.left,sRect.top,sRectEx);
					if(sRectEx.right >=sRect.right && sRectEx.bottom>=sRect.bottom)State = gsNormal;
				}
			}
			else 
				State = FGridState;
			if(State == gsCellsMoving)
				hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_MOVING));
			if(State == gsColsSizing)
				hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_ARROWWE));
			else if(State == gsRowsSizing)
				hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_ARROWNS));
			else if(State == gsFilling)
				hCursor = LoadCursor(hCellModula,MAKEINTRESOURCE(IDC_FILLING));
		}
		if(hCursor)
		{
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}
		
		return true;
	}

	bool CGridFrameSvr::EvtSize(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool CGridFrameSvr::EvtSetFocus(TEvent *pEvent ,LPARAM lParam)
	{
		if(!m_pSheetSvr->m_pSheetCtrl || !m_pSheetSvr->m_pSheetCtrl->m_pxfNode) return true;
		xfNode* pNode = this->m_pSheetSvr->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->GetAgentNode();

		if(pNode && pNode->GetXPage()->m_pFocusNodeControl)
		{
			if(pNode->GetXPage()->m_pFocusNodeControl->GetHWND() != m_pSheetSvr->m_pSheetCtrl->GetHWND() ||
				!pNode->GetNodeByName(_T("xcellagent")))
				this->m_pSheetSvr->m_pSheetCtrl->FocusedSheet();
		}

		return true;
	}
	bool CGridFrameSvr::EvtKillFocus(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}

	TGridState	CGridFrameSvr::CalcSizingState(POINT point,RECT &rect)
	{
		TGridState State = gsNormal;
		if(this->FOptions & gsDragColumn)
		{
			State = gsDragColumn;
			return State;
		}
		int X = point.x;
		int Y = point.y;
		if (X > rect.left && Y >= rect.top)
		{
			TGridRect SelectionCell;
			RECT	  SelectionRect;	
			TGridDrawInfo GridInfo;
			InitDrawInfo(&GridInfo);
			SelectionCell =this->GetSelection();
			BoxRect(GridInfo,SelectionCell,SelectionRect);

			if((FOptions & goAllowEdit) &&( SelectionRect.right - X > -2 &&  SelectionRect.right - X <=5 && SelectionRect.bottom - Y > -2 && SelectionRect.bottom - Y <= 5))
			{
				State = gsFilling;
			}
			else if(abs(X - SelectionRect.left) < 5 && Y <=SelectionRect.bottom && Y >=SelectionRect.top)
			{
				State = gsColsSizing;
			}
			else if(abs(X - SelectionRect.right) < 5 && Y <=SelectionRect.bottom && Y >=SelectionRect.top)
			{
				State = gsColsSizing;
			}
			else if(abs(Y - SelectionRect.top) < 5 && X <=SelectionRect.right && X >=SelectionRect.left)
			{
				State = gsRowsSizing;
			}
			else if(abs(Y - SelectionRect.bottom) < 5 && X <=SelectionRect.right && X >=SelectionRect.left)
			{
				State = gsRowsSizing;
			}
			else
				State = gsNormal;
			if((FOptions & goAllowEdit) &&(State == gsColsSizing || State == gsRowsSizing))
				State = gsCellsMoving;
			if(FOptions & goRunState)State = gsNormal;
		}
		return State;
	}

	void CGridFrameSvr::OnDraw(HDC hdc,RECT &rect)
	{
		RECT drect = FRect;	
		if(FRect.right ==0 && FRect.bottom==0 && FRect.top==0 && FRect.left==0)
			drect = rect;
		else if(FRect.right<=FRect.left &&  FRect.bottom<=FRect.top)
			drect = rect;
		else
			::OffsetRect(&drect,rect.left, rect.top);
		
		DrawBackGround(hdc,drect);
		DrawGridLine(hdc,drect,FTopLeft.x,FTopLeft.y);
		DrawBorderLines(hdc,drect,FTopLeft.x,FTopLeft.y);
		DrawCells(hdc,drect,FTopLeft.x,FTopLeft.y);
		if(FRect.right ==0 && FRect.bottom==0 && FRect.top==0 && FRect.left==0)return ;
		if(FOptions & goVisibleSelection) DrawSelectionBorder(hdc,drect);

		if(this->FGridState ==gsFilling)
		{
			RECT   FocRect;
			//TPoint Point;
			//TGridCoord CellHit;
			TGridRect rect;
			bool	bDrawFill = false;

			if( this->FFillingAnchor.y >= (FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y) &&
				this->FFillingAnchor.y <= (FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y))
			{
				if( this->FFillingAnchor.x < (FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x))
				{
					rect.left = this->FFillingAnchor.x;
					rect.right = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
					rect.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
					rect.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
					bDrawFill = true;
				}else if( this->FFillingAnchor.x > (FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x))
				{
					rect.right = this->FFillingAnchor.x;
					rect.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
					rect.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
					rect.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
					bDrawFill = true;
				}
			}
			if( this->FFillingAnchor.x >= (FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x) &&
				this->FFillingAnchor.x <= (FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x))
			{
				if( this->FFillingAnchor.y < (FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y))
				{
					rect.top = this->FFillingAnchor.y;
					rect.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
					rect.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
					rect.right = FCurrent.x>=FAnchor.x?FCurrent.y:FAnchor.x;
					bDrawFill = true;
				}else if( this->FFillingAnchor.y > (FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y))
				{
					rect.bottom = this->FFillingAnchor.y;
					rect.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
					rect.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
					rect.right = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
					bDrawFill = true;
				} 
			}
			if (bDrawFill)
			{
				TGridDrawInfo DrawInfo;
				InitDrawInfo(&DrawInfo);
				//GridRectToScreenRect(rect, FocRect, FALSE);
				this->BoxRect(DrawInfo,rect,FocRect);
				//FocRect -= this->GetFBound().TopLeft;

				int width,height;
				HBRUSH hbrush = ::CreateSolidBrush(RGB(127,127,127));
				height = FocRect.bottom - FocRect.top;
				width = FocRect.right - FocRect.left;
				hbrush = (HBRUSH)::SelectObject(hdc,hbrush);
				
				::PatBlt(hdc,FocRect.left - 2,FocRect.top - 2,3,height + 4,PATCOPY);
				if(FocRect.bottom <= DrawInfo.FullVisBoundaryCol)
					::PatBlt(hdc,FocRect.right -1,FocRect.top - 2,3,height +4 ,PATCOPY);
				::PatBlt(hdc,FocRect.left - 2,FocRect.top - 2 ,width + 4,3,PATCOPY);
				
				if(FocRect.bottom <= DrawInfo.FullVisBoundaryRow)
					::PatBlt(hdc,FocRect.left - 2,FocRect.bottom - 1,width +4 , 3,PATCOPY);
				
				 hbrush =(HBRUSH)::SelectObject(hdc,hbrush);
				::DeleteObject(hbrush);
			}
		}
	}

	void CGridFrameSvr::DrawBackGround(HDC hdc, RECT &rect)
	{
		/*if(!m_pSheetSvr->m_pSheetCtrl || ! m_pSheetSvr->m_pSheetCtrl->m_pxfNode) return ;
		xfNode * pNode = m_pSheetSvr->m_pSheetCtrl->m_pxfNode;
		LPCTSTR pStyle = pNode->GetCssStyle();

		Hxsoft::XUtil::Css::TBoxStyle* pBoxStyle = Hxsoft::XUtil::xuCCssStyle::ParseCssStyle(pStyle);
		if(pBoxStyle && pBoxStyle->mask &cssMaskBackGround)
		{
			//XCellSheetSvr is draw
		}
		else
		{
			HBRUSH hBrush = ::CreateSolidBrush(RGB(225,229,238));
			::FillRect(hdc,&rect,hBrush);
			::DeleteObject(hBrush);
		}*/
		return ;
	}

	TGridRect CGridFrameSvr::GetSelection()
	{ 
		RECT SRect;
		::SetRect(&SRect,FCurrent.x,FCurrent.y,FAnchor.x,FAnchor.y);
		::NormalizeRect(SRect);
		return  GetGridDataSvr()->m_pSpanSvr->GetFullRect(*(TGridRect *)&SRect);
	}

	void CGridFrameSvr::DrawSelectionBorder(HDC hdc,RECT &rect)
	{
		int width,height;
		RECT FocRect;
		TGridDrawInfo AxisInfo;
		InitDrawInfo(&AxisInfo);
		BoxRect(AxisInfo,GetSelection(),FocRect);

		::OffsetRect(&FocRect,rect.left - FRect.left ,rect.top - FRect.top);

		HBRUSH hbrush = ::CreateSolidBrush(RGB(0,0,0));
		height = FocRect.bottom - FocRect.top;
		width = FocRect.right - FocRect.left;
		hbrush = (HBRUSH)::SelectObject(hdc,hbrush);

		::PatBlt(hdc,FocRect.left - 2,FocRect.top - 2,3,height + 4,PATCOPY);
		::PatBlt(hdc,FocRect.right -1,FocRect.top - 2,3,height - 1 ,PATCOPY);
		::PatBlt(hdc,FocRect.left - 2,FocRect.top - 2 ,width + 4,3,PATCOPY);
		::PatBlt(hdc,FocRect.left - 2,FocRect.bottom - 1,width - 1, 3,PATCOPY);
		::PatBlt(hdc,FocRect.right - 2, FocRect.bottom - 2, 5 , 5,PATCOPY);

		hbrush =(HBRUSH)::SelectObject(hdc,hbrush);
		::DeleteObject(hbrush);
	}

	void CGridFrameSvr::DrawCells(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineExtent )
	{
		RECT  wRect,sRect;
		RECT FocRect;
		int ARow,ACol;
		int StartX,StartY;
		int StopX,StopY;

		StartX = rect.left + this->FGridLineWidth;
		StartY = rect.top + this->FGridLineWidth;
		StopX = rect.right;
		StopY = rect.bottom;

		::SetRect(&FocRect,0,0,0,0);
		TGridDrawInfo AxisInfo;
		InitDrawInfo(&AxisInfo);

		int OffsetX,OffsetY;

		TGridRect gRect;
		gRect.left = StartCol;
		gRect.top = StartRow;
		gRect.right = StartCol;
		gRect.bottom = StartRow;
		BoxRect(AxisInfo,gRect,sRect);
		OffsetX = sRect.left - rect.left - 1;
		OffsetY = sRect.top - rect.top - 1;

		ARow=StartRow;
		wRect.top = StartY;
		wRect.left = StartX;
		while (wRect.top < StopY && ARow <= GetRowCount())
		{
			int RowHeight = GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow);
			if(RowHeight < 1)
			{
				ARow++;
				//wRect.bottom = wRect.top - 1;
				continue;
			}
			ACol = StartCol;
			wRect.left = StartX;
			wRect.bottom = wRect.top + GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow);

			if(wRect.bottom > StopY)break;

			if(GetGridDataSvr()->m_enView==CGridDataSvr::envGridReportDesign || GetGridDataSvr()->m_enView==CGridDataSvr::envPrintDesign)
			{
				CReportBandSvr::TReportBandItem	* pBandItem;
				int nGroup;
				if(GetGridDataSvr()->m_ReportBand->GetReportBand(ARow,pBandItem,nGroup))
				{
					if(ARow == pBandItem->endRow)
					{
						LPTSTR pStr = GetGridDataSvr()->m_ReportBand->GetReportBandTitle(pBandItem);
						
						LOGFONT	logFont;
						XUtil::Css::GetDefaultFont(logFont);
						HFONT hFont = ::CreateFontIndirect(&logFont);
						hFont = (HFONT)::SelectObject(hdc,hFont);
						HBRUSH hBrush = ::CreateSolidBrush(RGB(237,240,237));
						
						RECT bRect = rect;
						bRect.top = wRect.top;
						bRect.bottom = wRect.bottom;
						::InflateRect(&bRect,-1,-1);
						::FillRect(hdc,&bRect,hBrush);
						::DeleteObject(hBrush);
						bRect.top += 3;
						bRect.left += 4;
						int hbr = SetBkMode(hdc,TRANSPARENT);
						HPEN hPen = CreatePen(PS_SOLID,1,RGB(0,0,255));
						hPen = (HPEN)SelectObject(hdc,hPen);

						::MoveToEx(hdc,bRect.left + 7,bRect.top + 4,NULL);
						::LineTo(hdc,bRect.left + 10,bRect.top + 1);
						::LineTo(hdc,bRect.left + 14,bRect.top + 5);
						::MoveToEx(hdc,bRect.left + 10,bRect.top + 3,NULL);
						::LineTo(hdc,bRect.left + 10,bRect.top + 12);

						hPen = (HPEN)SelectObject(hdc,hPen);
						::DeleteObject(hPen);

						bRect.left += 20;
						::DrawText(hdc,pStr,(int)_tcslen(pStr),&bRect,DT_LEFT|DT_NOPREFIX);
						SetBkMode(hdc,hbr);
						hFont = (HFONT)::SelectObject(hdc,hFont);
						::DeleteObject(hFont);
						if(pStr) delete pStr;
					}
				}
			}
			while (wRect.left < StopX && ACol <= GetColCount())
			{
				TGridRect gRect;
				if(GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
				{
					BoxRect(AxisInfo,gRect,sRect);
					if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
					{
						::OffsetRect(&sRect,- OffsetX,- OffsetY);
						DrawCell(hdc,ACol, ARow, sRect);
					}
					ACol = gRect.right;
					wRect.right = sRect.right;
				}else
				{
					wRect.right = wRect.left + GetGridDataSvr()->GetAxisSvr(ARow)->GetColWidth(ACol);
					DrawCell(hdc,ACol, ARow, wRect);
				}
				if(wRect.right > StopX)break;
				wRect.left = wRect.right + 1;
				ACol++;
			}
			wRect.top = wRect.bottom + 1;
			ARow++;
		}
	}

	void CGridFrameSvr::DrawCell(HDC hdc,int ACol,int ARow, RECT &ARect)
	{
		bool bVisible = true;
		LPTSTR visible = this->GetItemProp(ACol,ARow,_T("cellvisible"));
		if(visible && _tcsicmp(visible,_T(""))!=0)
		{
			if(_tcsicmp(visible,_T("否"))==0 || _tcsicmp(visible,_T("false"))==0 ||
				_tcsicmp(visible,_T("不可见"))==0 || _tcsicmp(visible,_T("0"))==0) 
			{
				if(visible) delete visible;
				if(FOptions & goRunState)return;
				bVisible = false;
			}
			if(bVisible && visible)delete visible;
		}

		bool bProtect = false;
		if(FOptions & goRunState)
		{
			LPTSTR protect = this->GetItemProp(ACol,ARow,_T("cellprotect"));
			if(protect && _tcsicmp(protect,_T(""))!=0)
			{
				if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
					_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
				{
					bProtect = true;
				}
				if(protect) delete protect;
			}
		}

#if 0		
		CInplaceEdit::eEditType	editType;
		LPTSTR pXClass;
		if(ACol==2 && ARow==2)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xoffice";// L"xflowchart";
		}else if(ACol==2 && ARow==3)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xflowchart";
		}else if(ACol==2 && ARow==4)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xwordsheet";
		}else
			editType=CInplaceEdit::_edit;

		CInplaceEdit * pEdit = NULL;
		for(int i=0;i< m_pEditSvr->m_EditBuffer.size();i++)
		{
			if(m_pEditSvr->m_EditBuffer[i]->editType == editType) 
			{
				if(editType !=CInplaceEdit::_control)
				{
					pEdit = m_pEditSvr->m_EditBuffer[i]->pEdit;
					break;
				}
				else
				{
					if(::_wcsicmp(m_pEditSvr->m_EditBuffer[i]->pEdit->m_szCotrolClass,pXClass)==0)
					{
						pEdit = m_pEditSvr->m_EditBuffer[i]->pEdit;
						break;
					}
				}
			}
		}

		if(editType==CInplaceEdit::_control)
		{
			if(!pEdit)
			{
				pEdit = new CControlEditer();
				((CControlEditer *)pEdit)->InstanceControl((HWND)this->GetHWND(),pXClass);
				if(pEdit->GetHWND())::MoveWindow(pEdit->GetHWND(),ARect.left,ARect.top,ARect.right - ARect.left - 1,ARect.bottom - ARect.top -1,false);
				CEditSvr::tagEditCtlBuff	* pBuffer = new CEditSvr::tagEditCtlBuff();
				pBuffer->editType = editType;
				pBuffer->pEdit = pEdit;
				m_pEditSvr->m_EditBuffer.push_back(pBuffer);
			}
			if(pEdit)
			{
				if(pEdit->GetHWND())
				{
					if(!::EqualRect(&ARect,&pEdit->m_DrawRect))
						::MoveWindow(pEdit->GetHWND(),ARect.left,ARect.top,ARect.right - ARect.left - 1,ARect.bottom - ARect.top -1,false);
				}
				if(((CControlEditer *)pEdit)->m_pxfControl) ((CControlEditer *)pEdit)->m_pxfControl->DoDraw(hdc,&ARect);
			}
			return ;
		}
#endif		

		HBRUSH brush;
		if(!(ACol==FCurrent.x && ARow ==FCurrent.y))
		{
			if(PointInGridRect(ACol, ARow, GetSelection()))
			{
				brush = ::CreateSolidBrush(RGB(169,178,202));
				::FillRect(hdc,&ARect,brush);
				::DeleteObject(brush);
			}
		}


		RECT rect;
		LPTSTR	pItemStr = NULL;
		PStrItem pItem;
		pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		
		const XUtil::Css::TBoxStyle* pBoxStyle = this->GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(ACol,ARow);
		LPTSTR fontstyle = this->GetItemProp(ACol,ARow,_T("fontstyle"));
		if(fontstyle && _tcscmp(fontstyle,L"") != 0)
		{
			if(fontstyle[0]=='=')
			{
				CXGridComplier x;
				if(x.SetSource(fontstyle, this, 1, ARow, ACol))
					fontstyle = wcsdup(x.GetString());
			}

			XUtil::Css::TBoxStyle* pBoxStyle1 =  XUtil::xuCCssStyle::ParseCssStyle(fontstyle);
			LOGFONT	logFont1;
			XUtil::Css::GetFont(pBoxStyle1,logFont1);
			GetGridDataSvr()->GetStyleSvr()->SetFont(ACol,ARow,logFont1);
			*pBoxStyle1 = (*pBoxStyle1) + (*pBoxStyle); 
			pBoxStyle = pBoxStyle1;
			delete fontstyle;
		}

		bool bBackGround = false;
		if(pBoxStyle && pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
		{
			HBRUSH hBrush;
			hBrush = ::CreateSolidBrush(pBoxStyle->pBackGround->color.color[0]);
			::FillRect(hdc,&ARect,hBrush);
			::DeleteObject(hBrush);
			bBackGround  = true;
		}

		if(!(pItemStr && pItemStr[0])) pItemStr = (LPTSTR)GetItemDisplayString(ARow, ACol);
		if(!(pItemStr && pItemStr[0])) pItemStr = (LPTSTR)GetItemString(ARow, ACol);

		/*if(pItem)
		{
			if(pItem->pStrText)
				pItemStr = (LPTSTR)GetItemDisplayString(ARow, ACol);//pItem->pStrText;
			else
				pItemStr = (LPTSTR)GetItemString(ARow, ACol);//pItem->pStrContent;
		}*/

		if(pItem && pItem->pStrXml)
		{
			IXMLDOMElement * pElement1 = NULL;
			IXMLDOMNode * pNode;
			_variant_t var;

			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
				pElement1->getAttribute(L"cellsource",&var);
				if(var.bstrVal && ::_tcscmp(var.bstrVal, L"") != 0)
					pItemStr = _tcsdup(var.bstrVal);
				pNode->Release();
				pElement1->Release();
			}
		}
		bool bDelete = false;
		if(pItemStr)
		{
			if(pItemStr[0]=='=')
			{
				/*CHxComplier::var value;
				CHxComplier complier;
				complier.m_pGrid = this;
				complier.FRow = ARow;
				complier.FCol = ACol;
				complier.of_calcexpression(pItemStr + 1,value);
				pItemStr = value.toString();
				bDelete = value.bStrNeedDeleted;*/
				CXGridComplier x;
				if(x.SetSource(pItemStr, this, 1, ARow, ACol))
					pItemStr = x.GetString();
				
			}
		}
		if(pItemStr || (pItem && pItem->pStrColumn) || (pItem && pItem->bAdvance))
		{
			LOGFONT	LogFont;
			HFONT hFont = NULL;
			if(GetGridDataSvr()->GetStyleSvr()->GetFont(ACol,ARow,LogFont) == 1)
			{
				hFont = ::CreateFontIndirect(&LogFont);
				hFont = (HFONT)::SelectObject(hdc,hFont);
			}else
			{
				XUtil::Css::GetDefaultFont(LogFont);
				hFont = ::CreateFontIndirect(&LogFont);
				hFont = (HFONT)::SelectObject(hdc,hFont);
			}

			COLORREF	TextColor = ::GetTextColor(hdc);
			UINT nAlign = DT_LEFT;
			if(pBoxStyle)
			{
				if(pBoxStyle->pColor && pBoxStyle->mask & cssMaskColor)
				{
					::SetTextColor(hdc,pBoxStyle->pColor->color[0]);
				}
				if(pBoxStyle->pText && pBoxStyle->mask & cssMaskText &&
					pBoxStyle->pText->mask & cssMaskTextAlign)
				{
					if(pBoxStyle->pText->textalign == XUtil::Css::_left)
						nAlign = DT_LEFT;
					else if(pBoxStyle->pText->textalign == XUtil::Css::_right)
						nAlign = DT_RIGHT;
					else if(pBoxStyle->pText->textalign == XUtil::Css::_center)
						nAlign = DT_CENTER;
				}
			}
			if(!bVisible)
				::SetTextColor(hdc,RGB(237,237,237)); 

			rect = ARect;
			::SetRect(&rect,ARect.left + 2,ARect.top + 2,ARect.right - 2,ARect.bottom - 2);
			int hbr = SetBkMode(hdc,TRANSPARENT);

			if(pItemStr && pItemStr[0]=='!' && pItemStr[1]=='#' && pItemStr[2]=='!' && _tcsstr(pItemStr + 3,L"!#!"))
			{
				LPTSTR pStr1;
				LPTSTR pStr2;
				int nLen1;
				int nLen2;

				pStr1 = pItemStr + 3;
				nLen1 = (int)(_tcsstr(pItemStr + 3,L"!#!") - pStr1);
				pStr2 = _tcsstr(pItemStr + 3,L"!#!") + 3;
				nLen2 = (int)_tcslen(pStr2);

				::MoveToEx(hdc,ARect.left,ARect.top,NULL);
				::LineTo(hdc,ARect.right,ARect.bottom);

				RECT rt = ARect;
				::DrawText(hdc,pStr1,nLen1,&rt,DT_CALCRECT|DT_NOPREFIX);
				int h = rt.bottom - rt.top;
				rt = ARect;
				rt.bottom -= 2;
				rt.top = rt.bottom - h;
				rt.left += 3;
				::DrawTextW(hdc,pStr1,nLen1,&rt,DT_LEFT|DT_NOPREFIX);

				rt = ARect;
				rt.top += 2;
				rt.right -= 3;
				::DrawTextW(hdc,pStr2,nLen2,&rt,DT_RIGHT|DT_NOPREFIX);

			}else
			{

				IXMLDOMElement * pElement = NULL;
				IXMLDOMElement * pElement1 = NULL;
				IXMLDOMNode * pNode;
				LPTSTR pEditType = NULL;
				LPTSTR pEditFormat = NULL;
				LPTSTR pDataType = NULL;
				_variant_t var;
				if(pItem->pStrXml)
				{
					xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pElement->getAttribute(L"edittype",&var);
						if(var.bstrVal)
							pEditType = _tcsdup(var.bstrVal);
						pNode->Release();
					}

					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement1);
						pElement1->getAttribute(L"celltype",&var);
						if(var.bstrVal)
							pDataType = _tcsdup(var.bstrVal);
						pElement1->getAttribute(L"cellformat",&var);
						if(var.bstrVal)
							pEditFormat = _tcsdup(var.bstrVal);	
						pNode->Release();
						pElement1->Release();
					}
				}
				bool bDrawText = true;
				bool bBarcode = false;
				if(!pItem->pStrColumn )
				{
					if(pEditType && _tcsicmp(pEditType,_T("checkbox"))==0) //checkbox
					{
						//if(ACol == GetCol() && ARow==GetRow())
						//{
							LPTSTR pCheckText = NULL;
							_variant_t vCheckText;
							
							if(pElement)
							{
								pElement->getAttribute(L"editchecked",&vCheckText);
								if(vCheckText.bstrVal)
									pCheckText = _tcsdup(vCheckText.bstrVal);
							}

							HBITMAP hbp = NULL;
							TCHAR path[MAX_PATH];
							::GetModuleFileName(NULL,path,MAX_PATH);
							::PathRemoveFileSpec(path);
							TCHAR buf[MAX_PATH];

							if(pCheckText && pItemStr/*pItem->pStrContent*/ && _tcsicmp(pCheckText,pItemStr/*pItem->pStrContent*/)==0)
								::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\true.bmp"),path);
							else
								::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\false.bmp"),path);

							hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
							HDC hMemDC = CreateCompatibleDC(hdc);
							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
							
							int dTop = (rect.bottom - rect.top)/2 - 8;
							BitBlt(hdc,rect.left,rect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
							::DeleteObject(hbp);
							::DeleteDC(hMemDC);

							rect.left += 16;

							if(pCheckText) delete pCheckText;
						//}
					}
					else if(pEditType && _tcsicmp(pEditType,_T("ddlb"))==0) //combobox
					{
						//if(ACol == GetCol() && ARow==GetRow())
						//{
							HBITMAP hbp = NULL;
							TCHAR path[MAX_PATH];
							::GetModuleFileName(NULL,path,MAX_PATH);
							::PathRemoveFileSpec(path);
							TCHAR buf[MAX_PATH];
							::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\down.bmp"),path);

							hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
							HDC hMemDC = CreateCompatibleDC(hdc);
							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

							int dTop = (rect.bottom - rect.top)/2 - 8;
							BitBlt(hdc,rect.right - 16,rect.top+dTop,16,16,hMemDC,0,0,SRCCOPY);

							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
							::DeleteObject(hbp);
							::DeleteDC(hMemDC);

							rect.right -= 16;
						//}
					}
					else if(pEditType && _tcsicmp(pEditType,_T("query"))==0) //QUery
					{
						pElement->getAttribute(L"dddwshowbutton",&var);

						if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0 || 
							(ACol == GetCol() && ARow==GetRow()))
						{
							HBITMAP hbp = NULL;
							TCHAR path[MAX_PATH];
							::GetModuleFileName(NULL,path,MAX_PATH);
							::PathRemoveFileSpec(path);
							TCHAR buf[MAX_PATH];
							::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\query.bmp"),path);

							hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
							HDC hMemDC = CreateCompatibleDC(hdc);
							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

							BitBlt(hdc,rect.right - 16,rect.top,16,16,hMemDC,0,0,SRCCOPY);

							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
							::DeleteObject(hbp);
							::DeleteDC(hMemDC);

							rect.right -= 16;
						}
					}
					else if(pEditType && _tcsicmp(pEditType,_T("image"))==0) //image
					{
						if(pItem && pItem->pStrContent)
						{
							TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
							DWORD dwSize = INTERNET_MAX_URL_LENGTH;
							LPTSTR pServer = m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl;
							xfUriLoader::UrlCombineW(pServer,pItem->pStrContent,strUrl,&dwSize,0);

							TCHAR Path[MAX_PATH];
							bool reload = 
								(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
							if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
							{
								Graphics graph(hdc);
								graph.SetPageUnit(UnitPixel);
								Image image(Path);
								int nWidth = rect.right - rect.left;
								int nHeight = rect.bottom - rect.top;
								graph.DrawImage(&image,rect.left,rect.top,nWidth,nHeight);
								bDrawText = false;
							}
						}
					}
					if(pItemStr)
					{

						SIZE cz;
						::GetTextExtentPoint32(hdc,pItemStr,(int)_tcslen(pItemStr),&cz);

						if(pEditType && _tcsicmp(pEditType,_T("barcode"))==0) //条码
						{
							LPTSTR pBarType = NULL;
							BarcodeBase * pBarcode = NULL;
							pElement->getAttribute(L"bartype",&var);
							if(var.bstrVal) pBarType = _tcsdup(var.bstrVal);
							if(pBarType)
							{
								LPSTR pBuf = XUtil::strdupFromBstr((BSTR)pItemStr);
								if(_tcsicmp(pBarType,_T("CODE39"))==0)
								{
									pBarcode = new Barcode39();
									((Barcode39 *)pBarcode)-> Encode39(pBuf);
								}
								else if(_tcsicmp(pBarType,_T("CODE93"))==0)
								{
									pBarcode = new Barcode93();
									((Barcode93 *)pBarcode)-> Encode93(pBuf);
								}
								else if(_tcsicmp(pBarType,_T("CODE128A"))==0)
								{
									pBarcode = new Barcode128();
									((Barcode128 *)pBarcode)-> Encode128A(pBuf);
								}
								else if(_tcsicmp(pBarType,_T("CODE128B"))==0)
								{
									pBarcode = new Barcode128();
									((Barcode128 *)pBarcode)-> Encode128B(pBuf);
								}
								else if(_tcsicmp(pBarType,_T("CODE128C"))==0)
								{
									pBarcode = new Barcode128();
									((Barcode128 *)pBarcode)-> Encode128C(pBuf);
								}
								else if(_tcsicmp(pBarType,_T("CODEI2OF5"))==0)
								{
									pBarcode = new BarcodeI2of5();
									((BarcodeI2of5 *)pBarcode)-> EncodeI2of5(pBuf);
								}
								else if(_tcsicmp(pBarType,_T("CODEEAN13"))==0)
								{
									pBarcode = new BarcodeEan13();
									((BarcodeEan13 *)pBarcode)-> EncodeEan13(pBuf);
								}
								if(pBarcode)
								{
									pElement->getAttribute(L"barheight",&var);

									XUtil::Css::TMeasure mess;
									if(var.bstrVal)
										XUtil::Css::ProcessCssMeasure(var.bstrVal,&mess);

									int ty = rect.bottom - 2;
									if(mess.measureRef ==XUtil::Css::_amount)
									{
										int hy = (int)XUtil::Css::GetPixelExtent(&mess);
										ty =  rect.top + 2 + hy;
									}else if(mess.measureRef ==XUtil::Css::_percentage)
									{
										int hy = (int)((rect.bottom - rect.top - 4)*mess.percentage /100.0);
										ty =  rect.top + 2 + hy;
									}
									if(ty > rect.bottom - 2) ty = rect.bottom - 2;
									if(ty < rect.top + 10)
									{
										ty = rect.bottom - 2;
										bDrawText = false;
									}

									if(bDrawText)
									{
										pElement->getAttribute(L"bartext",&var);
										if(var.bstrVal && _tcsicmp(var.bstrVal,_T("不显示"))==0) bDrawText = false;
										if(bDrawText)
											if(ty > rect.bottom - cz.cy - 4 ) ty =  rect.bottom - cz.cy - 4;
									}

									pBarcode->DrawBarcode(hdc,rect.left + 2,rect.top + 2, ty,ty ,RGB(0,0,0),RGB(255,255,255),2);
									bBarcode = true;
									delete pBarcode;
									
									if(bDrawText) rect.top = ty + 2;
								}
								if(pBuf)delete pBuf;
							}
						}

						if(bDrawText)
						{
							if(_tcschr(pItemStr,'\r'))
								::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,nAlign|(bBarcode?0x0:DT_VCENTER)|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
							else
							{
								SIZE sz;
								GetTextExtentPoint32(hdc,pItemStr,(int)::_tcslen(pItemStr),&sz);
								if(sz.cx < rect.right - rect.left)
									::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,DT_SINGLELINE |nAlign|(bBarcode?0x0:DT_VCENTER)|DT_EDITCONTROL|DT_NOPREFIX);
								else
									::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,nAlign|(bBarcode?0x0:DT_VCENTER)|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
							}
						}
					}
				}
				else
				{
					if(FOptions & goRunState)
					{
						RECT rc;
						HBRUSH hBrush ;
						if(!bBackGround)
						{
							if(!bProtect)
								hBrush = ::CreateSolidBrush(RGB(245,255,245));
							else
								hBrush = ::CreateSolidBrush(RGB(247,250,247));
							::SetRect(&rc,rect.left -1 ,rect.top - 1,rect.right + 1 ,rect.bottom + 1);
							::FillRect(hdc,&rc,hBrush);
							::DeleteObject(hBrush);
						}
						if(pEditType && _tcsicmp(pEditType,_T("checkbox"))==0) //checkbox
						{
							//if(ACol == GetCol() && ARow==GetRow())
							//{
								LPTSTR pCheckText = NULL;
								_variant_t vCheckText;
								
								if(pElement)
								{
									pElement->getAttribute(L"editchecked",&vCheckText);
									if(vCheckText.bstrVal)
										pCheckText = _tcsdup(vCheckText.bstrVal);
								}

								HBITMAP hbp = NULL;
								TCHAR path[MAX_PATH];
								::GetModuleFileName(NULL,path,MAX_PATH);
								::PathRemoveFileSpec(path);
								TCHAR buf[MAX_PATH];

								if(pCheckText && pItemStr/*pItem->pStrContent*/ && _tcsicmp(pCheckText,pItemStr/*pItem->pStrContent*/)==0)
									::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\true.bmp"),path);
								else
									::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\false.bmp"),path);

								hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
								HDC hMemDC = CreateCompatibleDC(hdc);
								hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

								int dTop = (rect.bottom - rect.top)/2 - 8;
								BitBlt(hdc,rect.left,rect.top + dTop,16,16,hMemDC,0,0,SRCCOPY);

								hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
								::DeleteObject(hbp);
								::DeleteDC(hMemDC);

								rect.left += 16;
								if(!pItem->pStrText) bDrawText = false;
								if(pCheckText) delete pCheckText;
							//}
						}
						else if(pEditType && _tcsicmp(pEditType,_T("ddlb"))==0) //combobox
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

							int dTop = (rect.bottom - rect.top)/2 - 8;
							BitBlt(hdc,rect.right - 16,rect.top+dTop,16,16,hMemDC,0,0,SRCCOPY);

							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
							::DeleteObject(hbp);
							::DeleteDC(hMemDC);

							rect.right -= 16;
						}
						else if(pEditType && _tcsicmp(pEditType,_T("query"))==0) //QUery
						{
							pElement->getAttribute(L"dddwshowbutton",&var);

							if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0 ||
								(ACol == GetCol() && ARow==GetRow()))
							{
								HBITMAP hbp = NULL;
								TCHAR path[MAX_PATH];
								::GetModuleFileName(NULL,path,MAX_PATH);
								::PathRemoveFileSpec(path);
								TCHAR buf[MAX_PATH];
								::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\query.bmp"),path);

								hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
								HDC hMemDC = CreateCompatibleDC(hdc);
								hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

								BitBlt(hdc,rect.right - 16,rect.top,16,16,hMemDC,0,0,SRCCOPY);

								hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
								::DeleteObject(hbp);
								::DeleteDC(hMemDC);

								rect.right -= 16;
							}
						}
						else if(pEditType && _tcsicmp(pEditType,_T("image"))==0) //image
						{
							if(pItem && pItem->pStrContent)
							{
								TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
								DWORD dwSize = INTERNET_MAX_URL_LENGTH;
								LPTSTR pServer = m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl;
								xfUriLoader::UrlCombineW(pServer,pItem->pStrContent,strUrl,&dwSize,0);

								TCHAR Path[MAX_PATH];
								bool reload = 
									(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
								if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
								{
									Graphics graph(hdc);
									graph.SetPageUnit(UnitPixel);
									Image image(Path);
									int nWidth = rect.right - rect.left;
									int nHeight = rect.bottom - rect.top;
									graph.DrawImage(&image,rect.left,rect.top,nWidth,nHeight);
									bDrawText = false;
								}
							}
						}
						if(pItemStr)
						{
							
							SIZE cz;
							::GetTextExtentPoint32(hdc,pItemStr,(int)_tcslen(pItemStr),&cz);

							if(pEditType && _tcsicmp(pEditType,_T("barcode"))==0) //条码
							{
								LPTSTR pBarType = NULL;
								BarcodeBase * pBarcode = NULL;
								pElement->getAttribute(L"bartype",&var);
								if(var.bstrVal) pBarType = _tcsdup(var.bstrVal);
								if(pBarType)
								{
									LPSTR pBuf = XUtil::strdupFromBstr((BSTR)pItemStr);
									if(_tcsicmp(pBarType,_T("CODE39"))==0)
									{
										pBarcode = new Barcode39();
										((Barcode39 *)pBarcode)-> Encode39(pBuf);
									}
									else if(_tcsicmp(pBarType,_T("CODE93"))==0)
									{
										pBarcode = new Barcode93();
										((Barcode93 *)pBarcode)-> Encode93(pBuf);
									}
									else if(_tcsicmp(pBarType,_T("CODE128A"))==0)
									{
										pBarcode = new Barcode128();
										((Barcode128 *)pBarcode)-> Encode128A(pBuf);
									}
									else if(_tcsicmp(pBarType,_T("CODE128B"))==0)
									{
										pBarcode = new Barcode128();
										((Barcode128 *)pBarcode)-> Encode128B(pBuf);
									}
									else if(_tcsicmp(pBarType,_T("CODE128C"))==0)
									{
										pBarcode = new Barcode128();
										((Barcode128 *)pBarcode)-> Encode128C(pBuf);
									}
									else if(_tcsicmp(pBarType,_T("CODEI2OF5"))==0)
									{
										pBarcode = new BarcodeI2of5();
										((BarcodeI2of5 *)pBarcode)-> EncodeI2of5(pBuf);
									}
									else if(_tcsicmp(pBarType,_T("CODEEAN13"))==0)
									{
										pBarcode = new BarcodeEan13();
										((BarcodeEan13 *)pBarcode)-> EncodeEan13(pBuf);
									}
									if(pBarcode)
									{
										pElement->getAttribute(L"barheight",&var);

										XUtil::Css::TMeasure mess;
										if(var.bstrVal)
											XUtil::Css::ProcessCssMeasure(var.bstrVal,&mess);

										int ty = rect.bottom - 2;
										if(mess.measureRef ==XUtil::Css::_amount)
										{
											int hy = (int)XUtil::Css::GetPixelExtent(&mess);
											ty =  rect.top + 2 + hy;
										}else if(mess.measureRef ==XUtil::Css::_percentage)
										{
											int hy = (int)((rect.bottom - rect.top - 4)*mess.percentage /100.0);
											ty =  rect.top + 2 + hy;
										}
										if(ty > rect.bottom - 2) ty = rect.bottom - 2;
										if(ty < rect.top + 10)
										{
											ty = rect.bottom - 2;
											bDrawText = false;
										}

										if(bDrawText)
										{
											pElement->getAttribute(L"bartext",&var);
											if(var.bstrVal && _tcsicmp(var.bstrVal,_T("不显示"))==0) bDrawText = false;
											if(bDrawText)
												if(ty > rect.bottom - cz.cy - 4 ) ty =  rect.bottom - cz.cy - 4;
										}

										pBarcode->DrawBarcode(hdc,rect.left + 2,rect.top + 2, ty,ty ,RGB(0,0,0),RGB(255,255,255),2);
										bBarcode = true;
										delete pBarcode;
										
										if(bDrawText) rect.top = ty + 2;
									}
									if(pBuf)delete pBuf;
								}
							}

							if(bDrawText)
							{
								if(pDataType && _tcscmp(pDataType,L"xs:decimal")==0)
								{
									if(pItemStr[0]=='.')
									{
										int nlen = _tcslen(pItemStr);
										LPTSTR pTemp = new TCHAR[nlen + 2];
										pTemp[0]='0';
										wcsncpy_s(pTemp + 1,nlen + 1,pItemStr,nlen);
										if(bDelete) delete pItemStr;
										bDelete = true;
										pItemStr = pTemp;
									}
									if(pEditFormat && _tcscmp(pEditFormat,L"")!=0)
									{
										pItemStr = XUtil::FormatDecimalString(pItemStr,pEditFormat,bDelete);
									}
								}
								else if(pDataType && _tcscmp(pDataType,L"xs:date")==0)
								{
									if(pEditFormat && (_tcscmp(pEditFormat,L"yyyy-mm-dd")==0 || _tcscmp(pEditFormat,L"yyyy/mm/dd")==0))
									{
										int len = 0;
										while(pItemStr[len] != ' ' && pItemStr[len] != 'T'&& pItemStr[len] != '\0') len++;
										pItemStr[len--]='\0';
									}
									else if(pEditFormat && _tcscmp(pEditFormat,L"")!=0)
									{
										int len = _tcslen(pEditFormat);
										pItemStr[len++] = '\0';
									}
								}
								COLORREF cr;
								if(pItem->bAdvance)
								{
									if(pItem->pStrAdvance)
									{
										pItemStr = new TCHAR[_tcslen(pItem->pStrAdvance)+2];
										_tcscpy(pItemStr,pItem->pStrAdvance);
										pItemStr[_tcslen(pItem->pStrAdvance)]='~';
										pItemStr[_tcslen(pItem->pStrAdvance)+1]=0;
									}else
									{
										pItemStr = new TCHAR[2];
										pItemStr[0] = '~';
										pItemStr[1] = 0;
									}
									bDelete = true;
									cr = ::SetTextColor(hdc,this->m_pSheetSvr->crAdvance);
								}else if(pItem->bDirect)
								{
									cr = ::SetTextColor(hdc,this->m_pSheetSvr->crDirect);
								}

								if(_tcschr(pItemStr,'\n'))
								{
									::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,nAlign|(bBarcode?0x0:DT_VCENTER)|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
								}
								else
								{
									SIZE sz;
									GetTextExtentPoint32(hdc,pItemStr,(int)::_tcslen(pItemStr),&sz);
									if(sz.cx < rect.right - rect.left)
									{
										::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,DT_SINGLELINE |nAlign|(bBarcode?0x0:DT_VCENTER)|DT_EDITCONTROL|DT_NOPREFIX);
										if(FShowOrigin && pItem->pOrigin)
										{
											RECT rc = rect;
											rc.left = rect.left + sz.cx + 4;
											COLORREF rf = ::SetTextColor(hdc,RGB(128,0,0));
											LOGFONT LogFont1 = LogFont;
											LogFont1.lfStrikeOut = true;
											HFONT hFont1 = ::CreateFontIndirect(&LogFont1);
											hFont1 = (HFONT)::SelectObject(hdc,hFont1);
											::DrawTextW(hdc,pItem->pOrigin,(int)::_tcslen(pItem->pOrigin),&rc,DT_SINGLELINE |nAlign|(bBarcode?0x0:DT_VCENTER)|DT_EDITCONTROL|DT_NOPREFIX|DT_WORD_ELLIPSIS);
											hFont1 = (HFONT)::SelectObject(hdc,hFont1);
											::DeleteObject(hFont1);
											::SetTextColor(hdc,rf);

											/*rc.bottom = rc.top + sz.cy;
											HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(255,0,0));
											hPen = (HPEN)::SelectObject(hdc,hPen);
											::MoveToEx(hdc,rc.left,(rc.bottom + rc.top)/2 - 1,NULL);
											::LineTo(hdc,rc.right,(rc.bottom + rc.top)/2 - 1);
											::MoveToEx(hdc,rc.left,(rc.bottom + rc.top)/2 + 1,NULL);
											::LineTo(hdc,rc.right,(rc.bottom + rc.top)/2 + 1);
											hPen = (HPEN)::SelectObject(hdc,hPen);
											::DeleteObject(hPen);
											*/
										}
									}
									else
										::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,nAlign|(bBarcode?0x0:DT_VCENTER)|DT_EDITCONTROL | DT_WORDBREAK|DT_NOPREFIX);
								}

								if(pItem->bAdvance)
									::SetTextColor(hdc,cr);
								else if(pItem->bDirect)
									::SetTextColor(hdc,cr);
							}
						}
						CDwTable * pTable = GetGridDataSvr()->GetDwTableFromCell(ACol,ARow);
						if(pTable)
						{
							int nRow, nCol;
							nRow = pTable->getRowColumnFromCell(ACol,ARow,nRow,nCol);
							if(nRow>0 && pTable->GetItemStatus(nRow,0)== rs_deleted)
							{
								HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(255,0,0));
								hPen = (HPEN)::SelectObject(hdc,hPen);
								::MoveToEx(hdc,ARect.left,(ARect.bottom + ARect.top)/2 - 1,NULL);
								::LineTo(hdc,ARect.right,(ARect.bottom + ARect.top)/2 - 1);
								::MoveToEx(hdc,ARect.left,(ARect.bottom + ARect.top)/2 + 1,NULL);
								::LineTo(hdc,ARect.right,(ARect.bottom + ARect.top)/2 + 1);
								hPen = (HPEN)::SelectObject(hdc,hPen);
								::DeleteObject(hPen);
							}
						}
					}else
					{
						if(pEditType && _tcsicmp(pEditType,_T("checkbox"))==0) //checkbox
						{
							LPTSTR pCheckText = NULL;
							_variant_t vCheckText;
							
							if(pElement)
							{
								pElement->getAttribute(L"editchecked",&vCheckText);
								if(vCheckText.bstrVal)
									pCheckText = _tcsdup(vCheckText.bstrVal);
							}

							HBITMAP hbp = NULL;
							TCHAR path[MAX_PATH];
							::GetModuleFileName(NULL,path,MAX_PATH);
							::PathRemoveFileSpec(path);
							TCHAR buf[MAX_PATH];

							if(pCheckText && pItemStr/*pItem->pStrContent*/ && _tcsicmp(pCheckText,pItemStr/*pItem->pStrContent*/)==0)
								::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\true.bmp"),path);
							else
								::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\false.bmp"),path);

							hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
							HDC hMemDC = CreateCompatibleDC(hdc);
							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

							int dTop = (rect.bottom - rect.top)/2 - 8;
							BitBlt(hdc,rect.left,rect.top+dTop,16,16,hMemDC,0,0,SRCCOPY);

							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
							::DeleteObject(hbp);
							::DeleteDC(hMemDC);

							rect.left += 16;
							if(pCheckText) delete pCheckText;
						}
						else if(pEditType && _tcsicmp(pEditType,_T("ddlb"))==0) //combobox
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

							int dTop = (rect.bottom - rect.top)/2 - 8;
							BitBlt(hdc,rect.right - 16,rect.top+dTop,16,16,hMemDC,0,0,SRCCOPY);

							hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
							::DeleteObject(hbp);
							::DeleteDC(hMemDC);

							rect.right -= 16;
						}
						else if(pEditType && _tcsicmp(pEditType,_T("query"))==0) //QUery
						{
							pElement->getAttribute(L"dddwshowbutton",&var);
							
							if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0)
							{
								HBITMAP hbp = NULL;
								TCHAR path[MAX_PATH];
								::GetModuleFileName(NULL,path,MAX_PATH);
								::PathRemoveFileSpec(path);
								TCHAR buf[MAX_PATH];
								::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\query.bmp"),path);

								hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
								HDC hMemDC = CreateCompatibleDC(hdc);
								hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

								BitBlt(hdc,rect.right - 16,rect.top,16,16,hMemDC,0,0,SRCCOPY);

								hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
								::DeleteObject(hbp);
								::DeleteDC(hMemDC);

								rect.right -= 16;
							}
						}
						/*else if(pEditType && _tcsicmp(pEditType,_T("image"))==0) //image
						{
							if(pItem && pItem->pStrContent)
							{
								TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
								DWORD dwSize = INTERNET_MAX_URL_LENGTH;
								LPTSTR pServer = m_pSheetSvr->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl;
								xfUriLoader::UrlCombineW(pServer,pItem->pStrContent,strUrl,&dwSize,0);

								TCHAR Path[MAX_PATH];
								bool reload = 
									(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
								if(xfUriLoader::GetCacheFileName(strUrl,Path,MAX_PATH,reload)==1)
								{
									Graphics graph(hdc);
									graph.SetPageUnit(UnitPixel);
									Image image(Path);
									int nWidth = rect.right - rect.left;
									int nHeight = rect.bottom - rect.top;
									graph.DrawImage(&image,rect.left,rect.top,nWidth,nHeight);
									bDrawText = false;
								}
							}
						}*/

						COLORREF TextColorEx = ::SetTextColor(hdc,RGB(192,192,192));
						TCHAR buf[120];
						::_stprintf_s(buf,120,_T(" [%s]"),pItem->pStrColumn);
						::DrawTextW(hdc,buf,(int)::_tcslen(buf),&rect,DT_LEFT|DT_VCENTER|DT_NOPREFIX);
						::SetTextColor(hdc,TextColorEx);
					}
					if(pEditType)delete pEditType;
					if(pEditFormat)delete pEditFormat;
					if(pDataType)delete pDataType;
				}
				if(pElement)pElement->Release();
			}
			::SetBkMode(hdc,hbr);
			if(hFont)
			{
				hFont = (HFONT)::SelectObject(hdc,hFont);
				::DeleteObject(hFont);
			}
			::SetTextColor(hdc,TextColor);
			if(pItem->pStrColumn)
			{
				if(!(FOptions & goRunState))
				{
					HPEN hPen = CreatePen(PS_SOLID,1,RGB(0,0,255));
					hPen = (HPEN)::SelectObject(hdc,hPen);
					HBRUSH hBrush = ::CreateSolidBrush(RGB(192,228,192));
					RECT rc;
					::SetRect(&rc,rect.left -2 ,rect.top - 2,rect.left + 4,rect.bottom + 2);
					::FillRect(hdc,&rc,hBrush);
					hPen = (HPEN)::SelectObject(hdc,hPen);
					::DeleteObject(hPen);
					::DeleteObject(hBrush);
				}
			}
		}

		if(pItem && pItem->pNote)
		{
			HBRUSH hBrush = ::CreateSolidBrush(RGB(255,0,0));
			hBrush = (HBRUSH)::SelectObject(hdc,hBrush);
			::PatBlt(hdc,ARect.right - 6,ARect.top+2,5,5,PATCOPY);
			hBrush = (HBRUSH)::SelectObject(hdc,hBrush);
			::DeleteObject(hBrush);
		}

		if(bDelete) delete pItemStr;
	}

	void CGridFrameSvr::DrawGridLine(HDC hdc,RECT &rect, int StartCol,int StartRow,int GridLineWidth, COLORREF color)
	{
		if(!(FOptions & goVertLine && FOptions & goHorzLine)) return ;

		//draw grid line
		int startX,startY;
		int EndX,EndY;
		int cx = 0,cy = 0;
		int LineX,LineY;
		int ARow,ACol;
		startX = rect.left;
		startY = rect.top;
		LineY = startY;
		LineX = startX;

		ARow= StartRow ;
		int nEndRow = -1;
		int nIndex = this->GetGridDataSvr()->GetChildIndex(ARow);
		if(nIndex >=0)
			nEndRow = (*GetGridDataSvr()->m_pChilds)[nIndex]->endRow;
		else
		{
			nIndex = this->GetGridDataSvr()->GetChildIndexFollow(ARow);
			if(nIndex >=0)
				nEndRow = (*GetGridDataSvr()->m_pChilds)[nIndex]->startRow - 1;
		}

		HPEN	hPen = ::CreatePen(PS_SOLID,this->FGridLineWidth,COptionSvr::stdRowSelecterColor0);
		hPen = (HPEN)::SelectObject(hdc,hPen);
		//get vert line count;
		while(LineY <= rect.bottom)
		{
			cy++;
			if(ARow > this->GetRowCount() || (nEndRow >0 && ARow > nEndRow) ) break;
			if(GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) > 0)
				LineY +=  GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) + GridLineWidth;
			ARow++;
		}
		EndY = LineY > rect.bottom ?rect.bottom : LineY;

		//get horz line count
		ACol=StartCol;
		while(LineX <= rect.right)
		{
			cx++;
			if(ACol > this->GetColCount()) break;
			LineX +=  GetGridDataSvr()->GetAxisSvr(StartRow)->GetColWidth(ACol) + GridLineWidth;
			ACol++;
		}
		EndX = LineX > rect.right ?rect.right : LineX;

		POINT *	 Points = new POINT[(cx + cy)*2];
		DWORD *  StrokeList = new DWORD[cx + cy];

		cy =0;
		LineY = startY;
		ARow = StartRow;
		while(LineY <= rect.bottom)
		{
			if(ARow > this->GetRowCount() || (nEndRow >0 && ARow > nEndRow) ) break;
			if(GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) < 1)
			{
				ARow++;
				continue;
			}
			Points[cy << 1].x = startX;
			Points[cy << 1].y = LineY;
			Points[(cy << 1) + 1].x = EndX;
			Points[(cy << 1) + 1].y = LineY ;
			StrokeList[cy] = 2;

			cy++;
			LineY +=  GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) + 1;
			ARow++;
		}

		cx =0;
		LineX = startX;
		ACol = StartCol;
		while(LineX <= rect.right)
		{
			Points[(cx + cy) << 1].x = LineX;
			Points[(cx + cy) << 1].y = startY;
			Points[((cx + cy) << 1) + 1].x = LineX ;
			Points[((cx + cy) << 1) + 1].y = EndY;
			StrokeList[cx + cy] = 2;

			cx++;
			if(ACol > this->GetColCount()) break;
			LineX +=  GetGridDataSvr()->GetAxisSvr(StartRow)->GetColWidth(ACol) + 1; 
			ACol++;
		}

		int nSavedDC = SaveDC(hdc);
		::BeginPath(hdc);
		::PolyPolyline(hdc,(LPPOINT)Points, StrokeList, (cx + cy));
		::EndPath(hdc);
		//::WidenPath(hdc);


		TRIVERTEX	pVertex[2];
		GRADIENT_RECT pGRect[1];

		pVertex[0].x = FRect.left;
		pVertex[0].y = FRect.top;
		pVertex[0].Red  = (GetRValue(color)>25? GetRValue(color) + 25:0)*256;
		pVertex[0].Green = (GetGValue(color)>25? GetGValue(color) + 25:0)*256;
		pVertex[0].Blue = (GetBValue(color)>25? GetBValue(color) + 25:0)*256;

		pVertex[1].x = FRect.right;
		pVertex[1].y = FRect.bottom ;
		pVertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) - 25)*256;
		pVertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) - 25)*256;
		pVertex[1].Blue = (GetBValue(color)>230?255:GetBValue(color) - 25)*256;
		pGRect[0].UpperLeft = 0;
		pGRect[0].LowerRight = 1;

		HRGN hRgn = ::CreateRectRgn(0,0,0,0);
		TGridDrawInfo AxisInfo;
		InitDrawInfo(&AxisInfo);
		if(this->CalcSpanRgn(hRgn,AxisInfo))
		{
			::ExtSelectClipRgn(hdc,hRgn,RGN_DIFF);
			//::SelectClipPath(hdc,RGN_AND);
			//GradientFill(hdc, pVertex,2,pGRect , 1,GRADIENT_FILL_RECT_H);
			StrokePath(hdc);
		}else
		{
			//::SelectClipPath(hdc,RGN_COPY);
			//GradientFill(hdc, pVertex,2,pGRect , 1,GRADIENT_FILL_RECT_H);
			StrokePath(hdc);
		}
		::DeleteObject(hRgn);
		RestoreDC(hdc,nSavedDC);


		hPen = (HPEN)::SelectObject(hdc,hPen);
		::DeleteObject(hPen);

		delete Points;
		delete StrokeList;

		if(EndY < rect.bottom &&  (nEndRow >0 && ARow > nEndRow) && nEndRow  < GetRowCount() &&
			EndY + this->GetRowHeight(ARow)< rect.bottom)
		{
			RECT rt=rect;
			rt.top = EndY ;  
			DrawGridLine( hdc,rt, StartCol,ARow, GridLineWidth, color);//bug startcol should trans
		}
	}

	const unsigned int FlatPenStyle = PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_FLAT|PS_JOIN_MITER;
	void CGridFrameSvr::DrawBorderLines(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth , COLORREF color)
	{
		RECT  wRect,sRect;
		RECT FocRect;
		int ARow,ACol;
		int AColEx;
		int StartX,StartY;
		int StopX,StopY;
		bool bDrawRect = false;

		StartX = rect.left + GridLineWidth;
		StartY = rect.top + GridLineWidth;
		StopX = rect.right;
		StopY = rect.bottom;

		::SetRect(&FocRect,0,0,0,0);
		TGridDrawInfo AxisInfo;
		InitDrawInfo(&AxisInfo);

		int OffsetX,OffsetY;

		TGridRect gRect;
		gRect.left = StartCol;
		gRect.top = StartRow;
		gRect.right = StartCol;
		gRect.bottom = StartRow;
		BoxRect(AxisInfo,gRect,sRect);
		OffsetX = sRect.left - rect.left - 1;
		OffsetY = sRect.top - rect.top - 1;

		std::vector<RECT> rects;
		RECT trect;
		const XUtil::Css::TBoxStyle * pBoxStyle;

		ARow=StartRow;
		wRect.top = StartY;
		wRect.left = StartX;
		while (wRect.top < StopY && ARow <= GetRowCount())
		{
			if(GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) < 1)
			{
				ARow ++;
				//wRect.top = wRect.bottom;
				//wRect.bottom = wRect.top;
				continue;
			}
			ACol = StartCol;
			wRect.left = StartX;
			wRect.bottom = wRect.top + GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow);
			if(wRect.bottom > StopY)break;
			while (wRect.left < StopX && ACol <= GetColCount())
			{
				TGridRect gRect;
				bDrawRect = false;
				AColEx = ACol;
				if(GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,gRect))
				{
					BoxRect(AxisInfo,gRect,sRect);
					if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
						bDrawRect = true;

					::OffsetRect(&sRect,- OffsetX,- OffsetY);
					
					ACol = gRect.right;
					wRect.right = sRect.right;
				}else
				{
					wRect.right = wRect.left + GetGridDataSvr()->GetAxisSvr(ARow)->GetColWidth(ACol);
					{
						sRect = wRect;
						bDrawRect = true;
					}
				}
				if(bDrawRect)
				{
					pBoxStyle = this->GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(AColEx,ARow);
					if(pBoxStyle && pBoxStyle->mask & cssMaskBorder)
					{
						if( pBoxStyle->pBorder[0] && pBoxStyle->pBorder[0]->bordertype != XUtil::Css::_none)
						{
							::SetRect(&trect,sRect.left - GridLineWidth,sRect.top - GridLineWidth,sRect.right,sRect.top - GridLineWidth);
							rects.push_back(trect);
							::SetRect(&trect,sRect.left - GridLineWidth,sRect.bottom,sRect.right,sRect.bottom);
							rects.push_back(trect);
							::SetRect(&trect,sRect.left - GridLineWidth,sRect.top - GridLineWidth,sRect.left - GridLineWidth,sRect.bottom);
							rects.push_back(trect);
							::SetRect(&trect,sRect.right,sRect.top - GridLineWidth,sRect.right,sRect.bottom);
							rects.push_back(trect);
						}
						if( pBoxStyle->pBorder[1] && pBoxStyle->pBorder[1]->bordertype != XUtil::Css::_none)
						{
							::SetRect(&trect,sRect.left - GridLineWidth,sRect.top - GridLineWidth,sRect.left - GridLineWidth,sRect.bottom);
							rects.push_back(trect);
						}
						if( pBoxStyle->pBorder[2] && pBoxStyle->pBorder[2]->bordertype != XUtil::Css::_none)
						{
							::SetRect(&trect,sRect.left - GridLineWidth,sRect.top - GridLineWidth,sRect.right,sRect.top - GridLineWidth);
							rects.push_back(trect);
						}
						if( pBoxStyle->pBorder[3] && pBoxStyle->pBorder[3]->bordertype != XUtil::Css::_none)
						{
							::SetRect(&trect,sRect.right,sRect.top - GridLineWidth,sRect.right,sRect.bottom);
							rects.push_back(trect);
						}
						if( pBoxStyle->pBorder[4] && pBoxStyle->pBorder[4]->bordertype != XUtil::Css::_none)
						{
							::SetRect(&trect,sRect.left - GridLineWidth,sRect.bottom,sRect.right,sRect.bottom);
							rects.push_back(trect);
						}
					}
				}
				if(wRect.right > StopX)break;
				wRect.left = wRect.right + 1;
				ACol++;
			}
			wRect.top = wRect.bottom + 1;
			ARow++;
		}

		if(rects.size()>0)
		{
			LOGBRUSH	LogBrush;
			HPEN		hPen = NULL;
			unsigned int LineWidth;
			LineWidth = GridLineWidth;
			if(LineWidth >= 1){
				LogBrush.lbStyle = BS_SOLID;
				LogBrush.lbColor = color;
				LogBrush.lbHatch = 0;
				hPen = (HPEN)ExtCreatePen(FlatPenStyle, LineWidth, &LogBrush, 0L, NULL);
				hPen = (HPEN)::SelectObject(hdc,hPen);
			}

			POINT *         Points;
			DWORD *         StrokeList;
			int				MaxStroke;

			MaxStroke = (int)rects.size();
			Points = (POINT * )LocalAlloc(LMEM_FIXED,(MaxStroke  + 1) * sizeof(POINT) * 2);
			StrokeList = (DWORD * )LocalAlloc( LMEM_FIXED,(MaxStroke + 1) * sizeof(DWORD));
			for(int i=0;i<MaxStroke;i++)
			{
				StrokeList[i] = 2;
				Points[i<<1].x  = rects[i].left; Points[i<<1].y  = rects[i].top;
				Points[(i<<1) + 1].x  = rects[i].right; Points[(i<<1) + 1].y  = rects[i].bottom;
			}

			// 2 integers per point, 2 points per line -> Index div 4
			::PolyPolyline(hdc,(LPPOINT)Points, StrokeList, MaxStroke);

			LocalFree(Points);
			LocalFree(StrokeList);
			if(hPen){
				hPen = (HPEN)::SelectObject(hdc,hPen);
				DeleteObject(hPen);
			}
		}
	}

	bool	CGridFrameSvr::EvtXCommand(TEvent *pEvent ,LPARAM lParam)
	{
		LPTSTR	pStrID = pEvent->xcommand.pStrID;

		if(pStrID && _tcscmp(pStrID,_T("xmUnMergeHeader"))==0)
		{
			TGridRect rect = this->GetSelection();
			int nRet = GetGridDataSvr()->AddChild(rect.top,rect.bottom);
			if(GetHWND())::InvalidateRect(GetHWND(),NULL,false);
			return nRet>=0?true:false;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmMergeHeader"))==0)
		{
			return GetGridDataSvr()->RemoveChild(FCurrent.y)>=0?true:false;
		}

		else if(pStrID && _tcscmp(pStrID,_T("xmCellProp"))==0)
		{
			CCellProp * pWin = new CCellProp();
			xfXPage* pXPage = new xfXPage();

			TCHAR path[MAX_PATH];
			::GetModuleFileName(NULL,path,MAX_PATH);
			::PathRemoveFileSpec(path);
			TCHAR buf[MAX_PATH];
			::_stprintf_s(buf,MAX_PATH,_T("%s\\agent\\xcell\\cellprop.xml"),path);
			pXPage->Load(buf); 

			HWND hParent = this->GetHWND();
			while(::GetParent(hParent))hParent =GetParent(hParent);
			pWin->m_pParam = this;
			pWin->Open(pXPage,hParent);
			
			return 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFilePrint"))==0)
		{
			Print();
			return 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmEditCopy"))==0)
		{
			Copy();
			return 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmEditCut"))==0)
		{
			Cut();
			return 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmEditPaste"))==0)
		{
			Paste();
			return 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmInsertRow"))==0)
		{
			CDwTable * pTable = this->GetCurDwTable();
			if(pTable) pTable->AppendRow();
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmDeleteRow"))==0)
		{
			CDwTable * pTable = this->GetCurDwTable();
			if(pTable) pTable->DeleteRow(0);
		}

		else if(pStrID && (_tcscmp(pStrID,_T("xmDevDatasource"))==0))
		{
			xfNode* pNode = this->m_pSheetSvr->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->GetAgentNode();
			if(pNode)
			{
				XFrame::xfControl * pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("datasource"));
				TCHAR pStrGroup[255];
				pStrGroup[0]  = '\0';
				::GetWindowText(pControl->m_hWnd,pStrGroup,255);
				this->GetGridDataSvr()->SetSchmaSource(pStrGroup);
			}
		}

		else if(pStrID && _tcscmp(pStrID,_T("xmDevSchema"))==0)
		{
			xfNode* pNode = this->m_pSheetSvr->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->GetAgentNode();
			if(pNode)
			{
				XFrame::xfControl * pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("xmDevSchema"));
				LPTSTR pSchema = ((XOffice::XCell::CGridDataSvr *)m_pSheetSvr->m_pDataSvr)->m_pSchemaSource;
				if(pSchema)
				{
					TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
					DWORD dwSize = INTERNET_MAX_URL_LENGTH;
					xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("dataschemadlg.xpage"),strUrl,&dwSize,0);

					RECT rt = pControl->m_pxfNode->m_AreaRect;
					MapWindowPoints(pControl->GetWin()->GetFrameWindow()->GetHWND(),NULL,(LPPOINT)&rt,2);
					xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE, WS_CAPTION|WS_DLGFRAME|WS_POPUP,&rt,pControl->GetWin()->GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),m_pSheetSvr);
				}
			}
		}
		else if(pStrID && _tcscmp(pStrID,_T("groupok"))==0)
		{
			xfNode* pNode = this->m_pSheetSvr->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->GetAgentNode();
			if(pNode)
			{
				XFrame::xfControl * pGroupControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("groupitem"));
				XFrame::xfControl * pColumnControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("columnitem"));
				if(!pGroupControl) 
					pGroupControl = (XFrame::xfControl *)this->m_pSheetSvr->m_pSheetCtrl->GetWin()->GetControl(_T("groupitem"));
				if(!pColumnControl)
					pColumnControl = (XFrame::xfControl *)this->m_pSheetSvr->m_pSheetCtrl->GetWin()->GetControl(_T("columnitem"));
				PStrItem pItem;
				pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(this->GetCol(),this->GetRow());
				if(!pItem)
				{
					pItem = new StrItem;
					this->GetGridDataSvr()->m_pContentSvr->SetItemData(this->GetCol(),this->GetRow(),pItem);
				}
				TCHAR pStrGroup[255];
				TCHAR pStrColumn[255];
				
				pStrGroup[0]  = '\0';
				pStrColumn[0] = '\0';
				if(pGroupControl)
					::GetWindowText(pGroupControl->m_hWnd,pStrGroup,255);
				if(pColumnControl)
					::GetWindowText(pColumnControl->m_hWnd,pStrColumn,255);
				if(pItem->pStrGroup)delete pItem->pStrGroup;
				if(pStrGroup[0]!='\0')
					pItem->pStrGroup = _tcsdup(pStrGroup);
				else
					pItem->pStrGroup = NULL;
				if(pItem->pStrColumn)delete pItem->pStrColumn;
				if(pStrColumn[0]!='\0')
					pItem->pStrColumn = _tcsdup(pStrColumn);
				else
					pItem->pStrColumn = NULL;
				this->InvalidateCell(this->GetCol(),this->GetRow());
			}
		}

		else if(pStrID && _tcscmp(pStrID,_T("xmFormatBold"))==0)
		{
			TGridRect rect = GetSelection();
			bool bBlod = true;
			for(int i=rect.left;i<=rect.right;i++)
			{
				for(int j=rect.top;j<=rect.bottom;j++)
				{
					XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
					if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
						*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
					if(!(pBoxStyle->mask & cssMaskFont))
					{
						pBoxStyle->pFont = new XUtil::Css::TFont;
						pBoxStyle->mask |= cssMaskFont;
					}
					if(i== rect.left && j==rect.top)
					{
						if(pBoxStyle->mask & cssMaskFont && pBoxStyle->pFont->mask & cssMaskFontWeight && pBoxStyle->pFont->fontweight == XUtil::Css::_bold)
							bBlod =false;
					}
					if(!bBlod)
					{
						pBoxStyle->pFont->fontweight = XUtil::Css::_normal;
						GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
					}else
					{
						pBoxStyle->pFont->mask |= cssMaskFontWeight;
						pBoxStyle->pFont->fontweight = XUtil::Css::_bold;
						GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
					}
					if(pBoxStyle)delete pBoxStyle;
				}
			}
			this->InvalidateRect(rect);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatItalic"))==0)
		{
			TGridRect rect = GetSelection();
			bool bItalic = true;
			for(int i=rect.left;i<=rect.right;i++)
			{
				for(int j=rect.top;j<=rect.bottom;j++)
				{
					XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
					if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
						*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
					if(!(pBoxStyle->mask & cssMaskFont))
					{
						pBoxStyle->pFont = new XUtil::Css::TFont;
						pBoxStyle->mask |= cssMaskFont;
					}
					if(i== rect.left && j==rect.top)
					{
						if(pBoxStyle->mask & cssMaskFont && pBoxStyle->pFont->mask & cssMaskFontStyle && pBoxStyle->pFont->fontstyle == XUtil::Css::_italic)
							bItalic =false;
					}
					if(!bItalic)
					{
						pBoxStyle->pFont->fontstyle = XUtil::Css::_normal;
						GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
					}else
					{
						pBoxStyle->pFont->mask |= cssMaskFontStyle;
						pBoxStyle->pFont->fontstyle = XUtil::Css::_italic;
						GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
					}
					if(pBoxStyle)delete pBoxStyle;
				}
			}
			this->InvalidateRect(rect);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatUnderline"))==0)
		{
			TGridRect rect = GetSelection();
			bool bUnderline = true;
			for(int i=rect.left;i<=rect.right;i++)
			{
				for(int j=rect.top;j<=rect.bottom;j++)
				{
					XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();;
					if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
						*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
					if(!(pBoxStyle->mask & cssMaskText))
					{
						pBoxStyle->pText = new XUtil::Css::TText;
						pBoxStyle->mask |= cssMaskText;
					}
					if(i== rect.left && j==rect.top)
					{
						if(pBoxStyle->mask & cssMaskText && pBoxStyle->pText->mask & cssMaskTextDecoration && pBoxStyle->pText->textdecoration == XUtil::Css::_underline)
							bUnderline =false;
					}
					if(!bUnderline)
					{
						pBoxStyle->pText->textdecoration = XUtil::Css::_normal;
						GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
					}else
					{
						pBoxStyle->pText->mask |= cssMaskTextDecoration;
						pBoxStyle->pText->textdecoration = XUtil::Css::_underline;
						GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
					}
					if(pBoxStyle)delete pBoxStyle;
				}
			}
			this->InvalidateRect(rect);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatFont"))==0)
		{
			CellChooseFont();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatTextColor"))==0)
		{
			CellChooseColor(STI_TEXTCOLOR);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatBkColor"))==0)
		{
			CellChooseColor(STI_BKCOLOR);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmClearBkColor"))==0)
		{
			ClearColor(GetSelection(),STI_BKCOLOR);
			ClearColor(GetSelection(),STI_TEXTCOLOR);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignLeft"))==0)
		{
			this->SetAlign(XUtil::Css::_left);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignRight"))==0)
		{
			this->SetAlign(XUtil::Css::_right);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignCenter"))==0)
		{
			this->SetAlign(XUtil::Css::_center);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatMerge"))==0)
		{
			this->GetGridDataSvr()->m_pSpanSvr->Merge(this->GetSelection());
			this->InvalidateRect(this->GetSelection());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatUnMerge"))==0)
		{
			this->GetGridDataSvr()->m_pSpanSvr->UnMerge(this->GetSelection());
			this->InvalidateRect(this->GetSelection());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatMergeRow"))==0)
		{
			this->GetGridDataSvr()->m_pSpanSvr->RMerge(this->GetSelection());
			this->InvalidateRect(this->GetSelection());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatMergeCol"))==0)
		{
			this->GetGridDataSvr()->m_pSpanSvr->CMerge(this->GetSelection());
			this->InvalidateRect(this->GetSelection());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsstr(pStrID,_T("xmFormatBorder"))==pStrID)
		{
			if(_tcscmp(pStrID,_T("xmFormatBorderAll"))==0)this->SetBorder(BORDER_ALL);
			if(_tcscmp(pStrID,_T("xmFormatBorderLeft"))==0)this->SetBorder(BORDER_LEFT);
			if(_tcscmp(pStrID,_T("xmFormatBorderTop"))==0)this->SetBorder(BORDER_TOP);
			if(_tcscmp(pStrID,_T("xmFormatBorderRight"))==0)this->SetBorder(BORDER_RIGHT);
			if(_tcscmp(pStrID,_T("xmFormatBorderBottom"))==0)this->SetBorder(BORDER_BOTTOM);
			if(_tcscmp(pStrID,_T("xmFormatBorderOuter"))==0)this->SetBorder(BORDER_OUTER);
			if(_tcscmp(pStrID,_T("xmFormatBorderInner"))==0)this->SetBorder(BORDER_INNER);
			if(_tcscmp(pStrID,_T("xmFormatBorderNone"))==0)this->SetBorder(BORDER_NONE);
			this->InvalidateRect(this->GetSelection());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsicmp(pStrID,_T("xmFormatShowGrid"))==0)
		{
			this->ShowGrid();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsicmp(pStrID,_T("xmFormatCellInsertRow"))==0)
		{
			this->InsertRow(this->GetRow());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsicmp(pStrID,_T("xmFormatCellDeleteRow"))==0)
		{
			this->DeleteRow(this->GetRow());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsicmp(pStrID,_T("xmFormatCellInsertCol"))==0)
		{
			this->InsertCol(this->GetCol());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsicmp(pStrID,_T("xmFormatCellDeleteCol"))==0)
		{
			this->DeleteColumn(this->GetCol());
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsicmp(pStrID,_T("xmFormatDrawLine"))==0)
		{
			FGridState = gsDrawLining;
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcsicmp(pStrID,_T("xmFormatDeleteLine"))==0)
		{
			FGridState = gsDeleteLining;
			pEvent->dwRet = 1;
		}
		pEvent->dwRet = 1;
		return true;
	}

	bool CGridFrameSvr::EvtRButtonDown(TEvent *pEvent ,LPARAM lParam)
	{
		if(this->FGridState ==gsDrawLining || this->FGridState ==gsDeleteLining)
		{
			this->FGridState = gsNormal;
		}

		POINT point = pEvent->mouse.point;
		UINT nFlags = pEvent->mouse.nFlags;
		if(::GetFocus() != this->GetHWND())::SetFocus(GetHWND());
		TGridCoord CellHit;
		CellHit = CalcCoordFromPoint(point.x,point.y);		
		int aRow,aCol;
		TGridRect	rect;
		GetGridDataSvr()->m_pSpanSvr->isSpaned(CellHit.x,CellHit.y,rect);
		if(GetGridDataSvr()->GetDwTableFromCell(rect.left,rect.top))
		{
			GetGridDataSvr()->GetDwTableFromCell(rect.left,rect.top)->getRowColumnFromCell(rect.left,rect.top,aRow,aCol);
			DWNMHDR hdr;
			memset(&hdr,0,sizeof(DWNMHDR));
			hdr.hwndFrom = GetHWND();
			hdr.idFrom = (UINT_PTR)GetGridDataSvr()->GetDwTableFromCell(rect.left,rect.top)->GetInterface();
			hdr.code = DWV_RCLICKED;
			hdr.DwTable = this;
			hdr.row = aRow;
			hdr.col = aCol;	
			PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(rect.left,rect.top);
			if(pItem && pItem->pStrColumn)
				hdr.colname = pItem->pStrColumn;
			if(pItem && pItem->pStrContent)
				hdr.data = pItem->pStrContent;
			else if(pItem && pItem->pStrText)
				hdr.data = pItem->pStrText;
			::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

			if(hdr.idFrom > 0)
			{
				HMENU M = CreatePopupMenu();
				AppendMenu(M,MF_STRING, 0x502, L"复制");

				xbXml xml;
				IXMLDOMNodeList * pList = NULL;
				if(hdr.data && _tcscmp(hdr.data, L"") != 0)
				{
					if(xml.LoadXml(hdr.data))
					{
						xml.GetXmlDoc()->selectNodes(L"/node()/node()", &pList);
						long len;
						pList->get_length(&len);
						if(len > 0) AppendMenu(M,MF_STRING, 0x0, NULL);
						for(int i=0; i<len; i++)
						{
							IXMLDOMNode *pNode = NULL;
							IXMLDOMElement * pElement = NULL;
							pList->get_item(i,&pNode);
							if(pNode)
							{
								pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
								pNode->Release();	
							}
							if(pElement)
							{
								_variant_t caption = NULL;
								pElement->getAttribute(L"value", &caption);
								if(caption.bstrVal)
								{
									LPTSTR pStr = _tcsdup(caption.bstrVal);
									if(_tcscmp(pStr, L"") == 0 || _tcscmp(pStr, L"-") == 0)
										AppendMenu(M,MF_STRING, 0x0, NULL);
									else
										AppendMenu(M,MF_STRING, 0x1000 + i, pStr);
								}
								pElement->Release();	
							}
						}
					}
				}
				RECT xr;
				POINT pt = pEvent->pt;
				int ret = TrackPopupMenu(M, TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, 0, ::GetParent(GetHWND()), &xr);
				if(ret == 0x502)
				{
					LPCTSTR DataStr = L"";
					if(pItem && pItem->pStrContent)
						DataStr = pItem->pStrContent;
					else if(pItem && pItem->pStrText)
						DataStr = pItem->pStrText;
					if(!DataStr) DataStr = L"";
					char* szText = XUtil::strdupFromBstr((BSTR)DataStr);
					if (::OpenClipboard (NULL)) 
					{
						::EmptyClipboard ();
						HANDLE hData = ::GlobalAlloc (GMEM_MOVEABLE, ::strlen(szText) + 1);
						LPWSTR pData = (LPWSTR)::GlobalLock (hData);
						memcpy(pData, szText, ::strlen(szText) + 1); 
						::GlobalUnlock(hData);
						::SetClipboardData(CF_TEXT, hData);
						::CloseClipboard ();
					}
				}
				else if(ret >= 0x1000 && pList)
				{
					ret -= 0x1000;
					IXMLDOMNode *pNode = NULL;
					IXMLDOMElement * pElement = NULL;
					pList->get_item(ret,&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();	
					}
					if(pElement)
					{
						_variant_t comdid;
						pElement->getAttribute(L"name", &comdid);
						if(comdid.bstrVal) 
							::SendMessage(::GetParent(GetHWND()),0x401,(WPARAM)comdid.bstrVal,NULL);
						pElement->Release();
					}
				}				
				if(pList) pList->Release();			
			}
		}

		/*
		TGridCoord CellHit;
		CellHit = CalcCoordFromPoint(point.x,point.y);
		GetGridDataSvr()->m_pSpanSvr->GetSpanedRowCol(CellHit.x,CellHit.y,CellHit.x,CellHit.y);

		if(this->FOptions & goRunState)
		{
			if(!GetGridDataSvr()->GetDwTableFromCell(CellHit.x,CellHit.y))
				return true;
		}
		*/
		return true;
	}

	bool CGridFrameSvr::EvtLButtonDown(TEvent *pEvent ,LPARAM lParam)
	{
		if(GetGridDataSvr()->GetDwTableFromCell(FCurrent.x,FCurrent.y))
		{
			if(GetGridDataSvr()->GetDwTableFromCell(FCurrent.x,FCurrent.y)->event_EditorLoseFocus() < 0) return true;
		}

		POINT point = pEvent->mouse.point;
		UINT nFlags = pEvent->mouse.nFlags;
		if(::GetFocus() != this->GetHWND())::SetFocus(GetHWND());
		TGridCoord CellHit;
		CellHit = CalcCoordFromPoint(point.x,point.y);
		if(GetGridDataSvr()->m_enView==CGridDataSvr::envGridReportDesign || GetGridDataSvr()->m_enView==CGridDataSvr::envPrintDesign)
		{
			CReportBandSvr::TReportBandItem* pBandItem;
			int nGroup;
			if(GetGridDataSvr()->m_ReportBand->GetReportBand(CellHit.y,pBandItem,nGroup))
			{
				if(pBandItem->endRow == CellHit.y) return true;
			}
		}
		GetGridDataSvr()->m_pSpanSvr->GetSpanedRowCol(CellHit.x,CellHit.y,CellHit.x,CellHit.y);


		TGridRect	rect;
		GetGridDataSvr()->m_pSpanSvr->isSpaned(CellHit.x,CellHit.y,rect);
		PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(rect.left,rect.top);
		//if(GetGridDataSvr()->GetDwTableFromCell(rect.left,rect.top))
		//{
			DWNMHDR hdr;
			memset(&hdr,0,sizeof(DWNMHDR));
			hdr.hwndFrom = GetHWND();
			hdr.idFrom = (UINT_PTR)this->GetInterface();
			hdr.code = DWV_CLICKED;
			hdr.DwTable = this;
			hdr.row = CellHit.y;
			hdr.col = CellHit.x;	
			if(pItem && pItem->pStrColumn)
				hdr.colname = pItem->pStrColumn;
			if(pItem && pItem->pStrContent)
				hdr.data = pItem->pStrContent;
			else if(pItem && pItem->pStrText)
				hdr.data = pItem->pStrText;
			else
				hdr.data = _T("");
			int ret = ::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
			if((int)hdr.idFrom < 0 ) return true;
		//}

		/*if(GetGridDataSvr()->GetDwTableFromCell(rect.left,rect.top))
		{
			LPTSTR cellprotect = this->GetColumnProp(GetGridDataSvr()->GetDwTableFromCell(rect.left,rect.top), CellHit.x, L"cellprotect");
			if(cellprotect && (_tcsicmp(cellprotect,_T("是"))==0 || _tcsicmp(cellprotect,_T("true"))==0 ||
				_tcsicmp(cellprotect,_T("保护"))==0 || _tcsicmp(cellprotect,_T("1"))==0))
				return true;
		}*/

		if(pItem && pItem->pStrXml && FOptions & goRunState && FOptions & goAllowEdit)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop/edit",&pNode);
			if(pNode)
			{
				_variant_t var;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					pElement->getAttribute(L"edittype",&var);
					if(var.bstrVal)
					{
						if(_tcscmp(var.bstrVal, L"checkbox") == 0)
						{
							MoveCurrent(CellHit.x, CellHit.y, true, true);
							LPTSTR protect = this->GetItemProp(CellHit.x,CellHit.y,_T("cellprotect"));
							if(protect)
							{
								if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
									_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
								return true;
							}
							int aRow,aCol;
							GetCurDwTable()->getRowColumnFromCell(CellHit.x,CellHit.y,aRow,aCol);
							GetCurDwTable()->event_ItemChanging(aRow,aCol,pItem->pStrContent);
							_variant_t ck;
							_variant_t unck;
							pElement->getAttribute(L"editchecked",&ck);
							pElement->getAttribute(L"uneditchecked",&unck);
							if(pItem->pStrContent)
							{
								if(ck.bstrVal && _tcscmp(ck.bstrVal, pItem->pStrContent) == 0)
								{
									if(unck.bstrVal) pItem->pStrContent = _tcsdup(unck.bstrVal);
								}
								else /*if(unck.bstrVal && _tcscmp(unck.bstrVal, pItem->pStrContent) == 0)*/
								{
									if(ck.bstrVal) pItem->pStrContent = _tcsdup(ck.bstrVal);
								}
							}
							else
							{
								if(ck.bstrVal) pItem->pStrContent = _tcsdup(ck.bstrVal);
							}
							if(this->FOptions & goRunState)
							{
								if(!GetGridDataSvr()->GetDwTableFromCell(CellHit.x,CellHit.y))
								{
									HideEditor();
								}
							}
							GetCurDwTable()->SetItemModified(aRow,aCol);
							GetCurDwTable()->event_ItemChanged(aRow,aCol,pItem->pStrContent);
							if(!pItem->pStrText) pItem->pStrText = L"";
							::InvalidateRect(this->m_pSheetSvr->m_pSheetCtrl->GetHWND(),NULL,false);
							return true;
						}
						if(_tcscmp(var.bstrVal, L"ddlb") == 0)
						{
							MoveCurrent(CellHit.x, CellHit.y, true, true);
							LPTSTR protect = this->GetItemProp(CellHit.x,CellHit.y,_T("cellprotect"));
							if(protect)
							{
								if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
									_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
								return true;
							}
							if(!GetGridDataSvr()->GetDwTableFromCell(CellHit.x,CellHit.y))
							{
								HideEditor();
							}
							ShowEditor();
							this->m_pEditSvr->UpdateEditContent(CellHit.x,CellHit.y);
							if(this->m_pEditSvr->m_pEdit)
								::SendMessage(this->m_pEditSvr->m_pEdit->GetHWND(), CB_SHOWDROPDOWN,true,NULL);
							return true;
						}
					}
					pElement->Release();
					//if(var.bstrVal)return _tcsdup(var.bstrVal);
				}
				pNode->Release();
			}
		}

		if(this->FOptions & goRunState)
		{
			if(!GetGridDataSvr()->GetDwTableFromCell(CellHit.x,CellHit.y))
			{
				HideEditor();
				::InvalidateRect(this->m_pSheetSvr->m_pSheetCtrl->GetHWND(),NULL,false);
				return true;
			}
		}

		FOldAnchor = FAnchor;

		if(this->FGridState  == gsDrawLining || this->FGridState == gsDeleteLining)
		{
			this->SetCapture();
			this->m_ptStart = point;
			this->m_ptMouse = point;
			return true;
		}

		if(goEditing & FOptions)
		{
			if((CellHit.x == FCurrent.x) && (CellHit.y == FCurrent.y))
			{
				ShowEditor();
				MoveAnchor(CellHit);

				bool bShowButton;
				if(IsEditQuery(CellHit.x,CellHit.y,bShowButton))
				{
					TGridDrawInfo GridInfo;
					InitDrawInfo(&GridInfo);

					RECT rc;
					this->GetCellRect(GridInfo,CellHit.x,CellHit.y,rc);
					
					if(point.x > rc.right - 16 && point.y < rc.top + 16)
					{
						bool bProtect = false;
						if(!(FOptions & goAllowEdit)) bProtect = true;
		
						if(FOptions & goRunState)
						{
							LPTSTR protect = this->GetItemProp(CellHit.x,CellHit.y,_T("cellprotect"));
							if(protect)
							{
								if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
									_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
								{
									bProtect = true;
								}
								delete protect;
							}
						}
						if(!bProtect) ShowQueryDlg();
					}
				}
			}
			else
			{
				if(m_pEditSvr && m_pEditSvr->TipHWMD && ::IsWindow(m_pEditSvr->TipHWMD)) ::DestroyWindow(m_pEditSvr->TipHWMD);
				/*MoveCurrent(CellHit.x, CellHit.y, True, True);
				UpdateEdit();
				*/
				this->SetCapture();
				FGridState = gsSelecting;
				::SetTimer(GetHWND(),ID_TIMER, 60, NULL);
				if(MK_SHIFT & nFlags)
					MoveAnchor(CellHit);	//选中
				else
				{
					MoveCurrent(CellHit.x, CellHit.y, True, True);
				}
			}
			Click();
		}
		else
		{
			if(m_pEditSvr && m_pEditSvr->TipHWMD && ::IsWindow(m_pEditSvr->TipHWMD)) ::DestroyWindow(m_pEditSvr->TipHWMD);
			TGridState state;
			state = CalcSizingState(point,FRect);

			TGridRect sRect = this->GetSelection();
			TGridRect sRectEx;
			if(sRect.left==sRect.right && sRect.top==sRect.bottom && state!= gsCellsMoving)state = gsNormal;
			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(sRect.left,sRect.top,sRectEx);
			if(sRectEx.right >=sRect.right && sRectEx.bottom>=sRect.bottom && state!= gsCellsMoving)state = gsNormal;

			if( state != gsNormal)
			{
				this->SetCapture();
				FGridState = state;
				this->m_ptStart = point;
				this->m_ptMouse = point;

				if(state==gsCellsMoving)
				{
					m_ptPosition = CellHit;
					TGridRect rs = GetSelection();
					if(m_ptPosition.x <rs.left)m_ptPosition.x  = rs.left; 
					if(m_ptPosition.x >rs.right)m_ptPosition.x = rs.right; 
					if(m_ptPosition.y <rs.top)m_ptPosition.y = rs.top; 
					if(m_ptPosition.y >rs.bottom)m_ptPosition.y = rs.bottom; 

					m_ptPositionStart.x = rs.left;
					m_ptPositionStart.y = rs.top;
				}else
				{
					DrawSizingLine(point);
					if(state==gsFilling)
					{
						FFillingAnchor = FAnchor;
						::SetTimer(GetHWND(),ID_TIMER, 60, NULL);
					}
					this->m_ptMouse = point;
				}
			}else
			{
				if((CellHit.x == FCurrent.x) && (CellHit.y == FCurrent.y))
				{
					ShowEditor();
					MoveAnchor(CellHit);

					bool bShowButton;
					if(IsEditQuery(CellHit.x,CellHit.y,bShowButton))
					{
						TGridDrawInfo GridInfo;
						InitDrawInfo(&GridInfo);

						RECT rc;
						this->GetCellRect(GridInfo,CellHit.x,CellHit.y,rc);
						if(point.x > rc.right - 16 && point.y < rc.top + 16)
						{
							bool bProtect = false;
							if(!(FOptions & goAllowEdit)) bProtect = true;
			
							if(FOptions & goRunState)
							{
								LPTSTR protect = this->GetItemProp(CellHit.x,CellHit.y,_T("cellprotect"));
								if(protect)
								{
									if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
										_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
									{
										bProtect = true;
									}
									delete protect;
								}
							}
							if(!bProtect) ShowQueryDlg();
						}						
					}
				}
				else
				{
					/*MoveCurrent(CellHit.x, CellHit.y, True, True);
					UpdateEdit();
					*/
					if(this->FOptions & goRunState && GetGridDataSvr()->GetDwTableFromCell(CellHit.x,CellHit.y))
					{
						CDwTable * pDwTable = GetGridDataSvr()->GetDwTableFromCell(CellHit.x,CellHit.y);
						bool bProtect = false;
						//if(!(FOptions & goAllowEdit)) bProtect = true;
						LPTSTR protect = this->GetItemProp(CellHit.x,CellHit.y,_T("cellvisible"));
						if(protect)
						{
							if(_tcsicmp(protect,_T("不可见"))==0) bProtect = true;
							delete protect;
						}
						if(bProtect)return true;
						MoveCurrent(CellHit.x, CellHit.y, True, True);
					}else
					{
						this->SetCapture();
						FGridState = gsSelecting;
						::SetTimer(GetHWND(),ID_TIMER, 60, NULL);
						if(MK_SHIFT & nFlags)
							MoveAnchor(CellHit);	//选中
						else
						{
							MoveCurrent(CellHit.x, CellHit.y, True, True);
						}
					}
				}
				Click();
			}
		}

		return true;
	}

	void CGridFrameSvr::DrawMovingLine()
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,2,RGB(128,128,128));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		TGridDrawInfo GridInfo;
		InitDrawInfo(&GridInfo);
		TGridRect grect;
		grect.left	 = m_ptPositionStart.x;
		grect.top	 = m_ptPositionStart.y;
		grect.right  = grect.left + FAnchor.x - FCurrent.x;
		grect.bottom = grect.top + FAnchor.y - FCurrent.y;

		RECT rt;
		this->BoxRect(GridInfo,grect,rt);
		::InflateRect(&rt,2,2);
		MoveToEx(hDC,rt.left, rt.top,NULL);
		::LineTo(hDC,rt.right, rt.top );
		::LineTo(hDC,rt.right, rt.bottom );
		::LineTo(hDC,rt.left, rt.bottom );
		::LineTo(hDC,rt.left, rt.top );

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		if(OldPen)::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	void CGridFrameSvr::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		TGridRect SelectionCell;
		RECT	  SelectionRect;	
		TGridDrawInfo GridInfo;
		InitDrawInfo(&GridInfo);
		SelectionCell =this->GetSelection();
		BoxRect(GridInfo,SelectionCell,SelectionRect);
		switch(FGridState)
		{
		case gsCellsMoving:
			{
				TGridCoord CellHit = CalcCoordFromPoint(pt.x, pt.y);
				if(CellHit.x != m_ptPosition.x || 
					CellHit.y != m_ptPosition.y)
				{
					TGridCoord newPos = m_ptPositionStart;
					newPos.x =  newPos.x + CellHit.x - m_ptPosition.x < 1?
						1: newPos.x + CellHit.x - m_ptPosition.x;
					newPos.y =  newPos.y + CellHit.y - m_ptPosition.y < 1?
						1: newPos.y + CellHit.y - m_ptPosition.y;
					if(newPos.x != m_ptPositionStart.x ||
						newPos.y != m_ptPositionStart.y)
					{
						TGridDrawInfo AxisInfo;
						InitDrawInfo(&AxisInfo);

						TGridRect grect;
						for(int i=0;i<=1;i++)
						{
							grect.left = m_ptPositionStart.x;
							grect.top = m_ptPositionStart.y;
							grect.right  = grect.left + FAnchor.x - FCurrent.x;
							grect.bottom = grect.top + FAnchor.y - FCurrent.y;

							RECT rt;
							this->BoxRect(AxisInfo,grect,rt);
							::InflateRect(&rt,2,2);
							MoveToEx(hDC,rt.left, rt.top,NULL);
							::LineTo(hDC,rt.right, rt.top );
							::LineTo(hDC,rt.right, rt.bottom );
							::LineTo(hDC,rt.left, rt.bottom );
							::LineTo(hDC,rt.left, rt.top );

							m_ptPositionStart = newPos; 
						}
					}
				}
				break;
			}
		case gsColsSizing:
			{
				int startY,endY;
				startY = SelectionRect.top>FRect.top?SelectionRect.top:FRect.top;
				endY = SelectionRect.bottom <FRect.bottom?SelectionRect.bottom:FRect.bottom;

				::MoveToEx(hDC,pt.x, startY,NULL);
				::LineTo(hDC,pt.x, endY );
				break;
			}
		case gsRowsSizing:
			{
				int startX,endX;
				startX = SelectionRect.left>FRect.left?SelectionRect.left:FRect.left;
				endX = SelectionRect.right <FRect.right?SelectionRect.right:FRect.right;

				::MoveToEx(hDC,startX,pt.y, NULL);
				::LineTo(hDC, endX, pt.y);
				break;
			}
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		if(OldPen)::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}
	void CGridFrameSvr::DrawInsLine(POINT EndPt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;
	    
		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = SetROP2(hDC,R2_XORPEN );
		
		TGridCoord CellHit1 = CalcCoordFromPoint(m_ptStart.x, m_ptStart.y);
		TGridCoord CellHit2 = CalcCoordFromPoint(EndPt.x, EndPt.y);
		
		RECT	rect1,rect2;
		this->GetCellRect(CellHit1.x,CellHit1.y,rect1);
		this->GetCellRect(CellHit2.x,CellHit2.y,rect2);
		
		//::OffsetRect(&rect1,FRect.left,FRect.top);
		//::OffsetRect(&rect2,FRect.left,FRect.top);
		
		RECT	sRect;
		POINT	sResult;
		sResult.x = CalcColFromPoint(m_ptStart.x, m_ptStart.y);
		sResult.y = CalcRowFromPoint(m_ptStart.x, m_ptStart.y);
		GetCellRect(sResult.x,sResult.y,sRect);
		//::OffsetRect(&sRect,FRect.left,FRect.top);
		
		RECT	rect;
		::SetRect(&rect,m_ptStart.x, m_ptStart.y,EndPt.x,EndPt.y);
		::NormalizeRect(rect);
		if(FGridState == gsDrawLining)
		{
			if(rect.right - rect.left < 10 && rect.bottom - rect.top < 10)
			{
				::MoveToEx(hDC,m_ptStart.x,m_ptStart.y,NULL);
				::LineTo(hDC,EndPt.x,EndPt.y);
			}else if(rect.right - rect.left < 10 && rect.bottom - rect.top >= 0.5 * (rect1.bottom - rect1.top))
			{
				int startx;
				startx = m_ptStart.x;
				if( m_ptStart.x - sRect.left < 5)startx = sRect.left;
				if( sRect.right - m_ptStart.x < 5)startx = sRect.right;
				if(rect1.top <= rect2.top)
				{
					::MoveToEx(hDC,startx,rect1.top,NULL);
					::LineTo(hDC,startx,rect2.bottom);
				}else
				{
					::MoveToEx(hDC,startx,rect1.bottom,NULL);
					::LineTo(hDC,startx,rect2.top);
				}
			}else if(rect.bottom - rect.top < 10 && (rect.right - rect.left) >= 0.5 * (rect1.right - rect1.left))
			{
				int starty;
				starty = m_ptStart.y;
				if( m_ptStart.y - sRect.top< 5)starty = sRect.top;
				if( sRect.bottom - m_ptStart.y < 5)starty = sRect.bottom;
				if(rect1.left <= rect2.left)
				{
					::MoveToEx(hDC,rect1.left,starty,NULL);
					::LineTo(hDC,rect2.right,starty);
				}else
				{
					::MoveToEx(hDC,rect1.right,starty,NULL);
					::LineTo(hDC,rect2.left,starty);
				}
			}else
			{
				::MoveToEx(hDC,m_ptStart.x,m_ptStart.y,NULL);
				::LineTo(hDC,EndPt.x,EndPt.y);
			}
		}
		else if(FGridState == gsDeleteLining)
		{
			if(rect.right - rect.left < 10 && rect.bottom - rect.top < 10)
			{
				return;
			}else if(rect.right - rect.left < 10 && rect.bottom - rect.top >= 0.5 * (rect1.bottom - rect1.top))
			{
				int startx;
				if( m_ptStart.x - sRect.left < 8)
					startx = sRect.left;
				else if( sRect.right - m_ptStart.x < 8)
					startx = sRect.right;
				else 
					goto end;
				
				HPEN	pPen = (HPEN)::CreatePen(PS_SOLID,7,RGB(128^255,255,255));
				pPen = (HPEN)::SelectObject(hDC,pPen);
				if(rect1.top <= rect2.top)
				{
					::MoveToEx(hDC,startx,rect1.top,NULL);
					::LineTo(hDC,startx,rect2.bottom);
				}else
				{
					goto end;
				}
				pPen = (HPEN)::SelectObject(hDC,pPen);
				if(pPen)::DeleteObject(pPen);

			}else if(rect.bottom - rect.top < 10 && rect.right - rect.left >= 0.5 * (rect1.right - rect1.left))
			{
				int starty;
				if( m_ptStart.y - sRect.top< 5)
					starty = sRect.top;
				else if( sRect.bottom - m_ptStart.y < 5)
					starty = sRect.bottom;
				else
					goto end;

				HPEN	pPen = ::CreatePen(PS_SOLID,7,RGB(128^255,255,255));
				pPen = (HPEN)::SelectObject(hDC,pPen);
				if(rect1.left <= rect2.left)
				{
					::MoveToEx(hDC,rect1.left,starty,NULL);
					::LineTo(hDC,rect2.right,starty);
				}else
				{
					::MoveToEx(hDC,rect1.right,starty,NULL);
					::LineTo(hDC,rect2.left,starty);
				}
				pPen = (HPEN)::SelectObject(hDC,pPen);
				if(pPen)::DeleteObject(pPen);
			}else
			{
				goto end;
			}
		}
	end:   
		::SetROP2(hDC,rop);
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		if(OldPen)::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);	
	}

	void CGridFrameSvr::DoMoveCells()
	{
		TGridRect rs = GetSelection();
		int rx = this->m_ptPositionStart.x - rs.left;
		int ry = this->m_ptPositionStart.y - rs.top;
		int sCol,eCol;
		int sRow,eRow;
		int dx,dy;

		if(rx ==0 && ry==0) return;

		if(rx > 0)
		{
			sCol = rs.right;
			eCol = rs.left;
			dx = -1;
			sRow = rs.top;
			eRow = rs.bottom;
			dy = 1;
		}else if(rx < 0)
		{
			sCol = rs.left;
			eCol = rs.right;
			dx = 1;
			sRow = rs.top;
			eRow = rs.bottom;
			dy = 1;
		}else if(ry > 0)
		{
			sCol = rs.left;
			eCol = rs.right;
			dx = 1;
			sRow = rs.bottom;
			eRow = rs.top;
			dy = -1;
		}else if(ry < 0)
		{
			sCol = rs.left;
			eCol = rs.right;
			dx = 1;
			sRow = rs.top;
			eRow = rs.bottom;
			dy = 1;
		}

		for(int i=sCol;dx>0?i<=eCol:i>=eCol; i+=dx)
		{
			for(int j=sRow;dy>0?j<=eRow:j>=eRow;j+=dy)
			{
				PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(i,j);
				if(pItem)
				{
					(*(*GetGridDataSvr()->m_pContentSvr->m_pData)[j])[i + CContentSvr::count] = NULL;
					GetGridDataSvr()->m_pContentSvr->SetItemData(i + rx,j + ry,pItem);
				}
			}
		}
	}
	
	bool CGridFrameSvr::GetFillingSize(int &Cols,int &Rows)
	{
		int srow,erow;
		int scol,ecol;

		scol = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
		srow = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
		ecol = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
		erow = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;

		Rows = 1;
		Cols = 1;

		TGridRect srect;
		for(int i=srow;i<=erow;i++)
		{
			for(int j=scol;j<=ecol;j++)
			{
				if(this->GetGridDataSvr()->m_pSpanSvr->isSpaned(j,i,srect))
				{
					if(srect.top != srect.bottom)
					{
						Rows = erow - srow + 1;
						return true;
					}
					if(srect.left != srect.right)
					{
						Cols = ecol - scol + 1;
						return true;
					}
				}
			}
		}
		return true;
	}

	void CGridFrameSvr::MoveFillingAnchor(TGridDrawInfo DrawInfo,TGridCoord NewFillingAnchor)
	{
		TGridRect rect,rectOld;

		rectOld.left=-1;rectOld.top=-1;rectOld.right=-1;rectOld.bottom=-1;
		rect.left=-1;rect.top=-1;rect.right=-1;rect.bottom=-1;
		if( this->FFillingAnchor.y >= (FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y) && 
			 this->FFillingAnchor.y <= (FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y))
		{
			if( this->FFillingAnchor.x < (FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x))
			{
				rectOld.left = this->FFillingAnchor.x;
				rectOld.right = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
				rectOld.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
				rectOld.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
			}else if( this->FFillingAnchor.x > (FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x))
			{
				rectOld.right = this->FFillingAnchor.x;
				rectOld.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
				rectOld.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
				rectOld.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
			}
		}
		if( this->FFillingAnchor.x >= (FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x) && 
			this->FFillingAnchor.x <= (FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x))
		{
			if( this->FFillingAnchor.y < (FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y))
			{
				rectOld.top = this->FFillingAnchor.y;
				rectOld.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
				rectOld.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
				rectOld.right = FCurrent.x>=FAnchor.x?FCurrent.y:FAnchor.x;
			}else if( this->FFillingAnchor.y > (FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y))
			{
				rectOld.bottom = this->FFillingAnchor.y;
				rectOld.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
				rectOld.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
				rectOld.right = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
			}
		}

		int rows,cols;
		this->GetFillingSize(cols,rows);

		if( NewFillingAnchor.y >= (FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y) && 
			NewFillingAnchor.y <= (FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y))
		{
			if( NewFillingAnchor.x < (FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x))
			{
				rect.left = NewFillingAnchor.x;
				rect.right = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
				rect.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
				rect.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
			}else if( NewFillingAnchor.x > (FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x))
			{
				rect.right = NewFillingAnchor.x;
				rect.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
				rect.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
				rect.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
			}
		}

		if( NewFillingAnchor.x >= (FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x) && 
			NewFillingAnchor.x <= (FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x))
		{
			if( NewFillingAnchor.y < (FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y))
			{
				rect.top = NewFillingAnchor.y;
				rect.bottom = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
				rect.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
				rect.right = FCurrent.x>=FAnchor.x?FCurrent.y:FAnchor.x;
			}else if( NewFillingAnchor.y > (FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y))
			{
				rect.bottom = NewFillingAnchor.y;
				rect.top = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
				rect.left = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
				rect.right = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
			}
		}

		this->AreaMoved(rectOld,rect);
		FFillingAnchor = NewFillingAnchor;
	}

	bool CGridFrameSvr::DoAutoFilling()
	{
		int srow,scol;
		int erow,ecol;
		int trow=-1,tcol=-1;

		scol = FCurrent.x<=FAnchor.x?FCurrent.x:FAnchor.x;
		srow = FCurrent.y<=FAnchor.y?FCurrent.y:FAnchor.y;
		ecol = FCurrent.x>=FAnchor.x?FCurrent.x:FAnchor.x;
		erow = FCurrent.y>=FAnchor.y?FCurrent.y:FAnchor.y;
		
		if(this->FFillingAnchor.x>=scol && this->FFillingAnchor.x<=ecol)

		{
			trow = FFillingAnchor.y < srow ? FFillingAnchor.y:-1;
			if(trow==-1)trow = FFillingAnchor.y > erow ? FFillingAnchor.y:-1;
		}
		if(this->FFillingAnchor.y>=srow && this->FFillingAnchor.y<=erow)
		{
			tcol= FFillingAnchor.x < scol ? FFillingAnchor.x:-1;
			if(tcol==-1)tcol = FFillingAnchor.x > ecol? FFillingAnchor.x:-1;
		}

		if(trow==-1 && tcol==-1) return false; //not need filling

		

		//判断合并的单元格是否与填充区域较差
		RECT	srect;
		bool bCross = false;
		Hxsoft::XUtil::xuVector<RECT>  &spRects = this->GetGridDataSvr()->m_pSpanSvr->SpanRects;
		if(trow !=-1)
		{
			if(trow <srow)
			{
				for(int i=0;i<spRects.size();i++)
				{
					srect = spRects[i];
					if((srect.left <= ecol && srect.right >ecol)||
						(srect.left <scol && srect.right >=ecol))
					{
						if(!(srect.top > erow || srect.bottom <trow))
						{
							bCross = true;
							break;
						}
					}else if((srect.top <= erow && srect.bottom >erow)||
							(srect.top <trow && srect.bottom >=trow))
					{
						if(!(srect.left > ecol || srect.right <ecol))
						{
							bCross = true;
							break;
						}
					}
				}
			}else
			{
				for(int i=0;i<spRects.size();i++)
				{
					srect = spRects[i];
					if((srect.left <= ecol && srect.right >ecol)||
						(srect.left <scol && srect.right >=scol))
					{
						if(!(srect.top > trow || srect.bottom <srow))
						{
							bCross = true;
							break;
						}
					}else if((srect.top <= trow && srect.bottom >trow)||
							(srect.top <srow && srect.bottom >=srow))
					{
						if(!(srect.left > ecol || srect.right <ecol))
						{
							bCross = true;
							break;
						}
					}
				}
			}
		}else
		{
			if(tcol <scol)
			{
				for(int i=0;i<spRects.size();i++)
				{
					srect = spRects[i];
					if((srect.top <= erow && srect.bottom >erow)||
						(srect.top <srow && srect.bottom >=srow))
					{
						if(!(srect.left > ecol || srect.right <tcol))
						{
							bCross = true;
							break;
						}

					}else if((srect.left <= ecol && srect.right >ecol)||
							(srect.left <tcol && srect.right >=tcol))
					{

						if(!(srect.top > erow || srect.bottom <srow))
						{
							bCross = true;
							break;
						}
					}

				}
			}else
			{
				for(int i=0;i<spRects.size();i++)
				{
					srect = spRects[i];
					if((srect.top <= erow && srect.bottom >erow)||
						(srect.top <erow && srect.bottom >=erow))
					{
						if(!(srect.left > tcol || srect.right <scol))
						{
							bCross = true;
							break;
						}

					}else if((srect.left <= tcol && srect.right >tcol)||
							(srect.left <scol && srect.right >=scol))
					{

						if(!(srect.top > erow || srect.bottom <srow))
						{
							bCross = true;
							break;
						}
					}
				}
			}
		}

		if(bCross)
		{
			MessageBox(this->GetHWND(),_T("合并的单元格与要求填充的单元格不一致"),_T("单元格填充"),MB_OK);
			return false;
		}
		
		struct tagCellV
		{
			bool breal;
			int ainum;
			int linum;
			float arnum;
			float lrnum;
		};

		float rnum;
		bool breal = false;
		CHxComplier* pComplier = new CHxComplier();
		pComplier->m_pGrid = this;
		TCHAR vbuf[80];
		TGridRect mrect;
		std::vector<tagCellV> vals;
		std::vector<POINT> cells;
		TGridRect grect;
		
		int rowIndex; //row index;
		if(trow !=-1)
		{
			mrect.left =scol;
			mrect.right = ecol;
			mrect.top = srow <trow?erow+1:trow;
			mrect.bottom = erow >trow?srow - 1:trow;
			this->GetGridDataSvr()->m_pSpanSvr->UnMerge(mrect);
			for(int i=scol;i<=ecol;i++)
			{
				vals.clear();
				cells.clear();
				if(trow > erow)
				{
					for(int j=srow;j<=erow;j++)
					{
						this->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,j,grect);
						if(grect.left!=i || grect.top !=j)
						{
							j += grect.bottom - grect.top;
							continue;
						}
						PStrItem pItem;
						pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(i,j);
						if(pItem && !pItem->pStrFormula && pItem->pStrContent && pItem->pStrContent[0]!='\0')
						{
							int inum0 = 0;
							float rnum0 = 0;
							POINT point;
							rnum = 0;
							int count=0;
							float lrnum0=0;
							CHxComplier::keyw typ = CHxComplier::undef;
							int start = -1;
							breal = false;
							if(pComplier->isNumber(pItem->pStrContent ,typ,inum0,rnum0))
							{
								rnum = 0;
								start = j;
								point.x = j;
								point.y = j;
								count = 0; //count number
								lrnum0 = rnum0;
								if(typ==CHxComplier::reals)breal = true;
								j += grect.bottom - grect.top + 1; 
								while(j <=erow)
								{
									this->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,j,grect);
									if(grect.left==i || grect.top ==j)
									{
										pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(i,j);
										if(pItem)
										{
											if(pItem->pStrFormula)break;
											if(pItem->pStrContent && pItem->pStrContent[0]!='\0')
											{
												if(pComplier->isNumber(pItem->pStrContent ,typ,inum0,rnum0))
												{
													point.y = j;
													rnum += rnum0 - lrnum0;
													lrnum0 = rnum0;
													count++;
													if(typ==CHxComplier::reals)breal = true;
												}else
													break;
											}
										}
									}
									j += grect.bottom - grect.top + 1;
								}
								cells.push_back(point);
								if(count==0){ count=1;rnum=1.0;}
								if(!breal && abs(rnum/count - int(rnum/count))>=0.000001)breal = true;
								tagCellV cv;
								if(!breal)
								{
									cv.breal = breal;
									cv.ainum = int(rnum/count);
									cv.linum = int(lrnum0);
								}else
								{
									cv.breal = breal;
									cv.arnum = rnum/count;
									cv.lrnum = lrnum0;
								}
								vals.push_back(cv);
							}
						}
					}
					bool bexit;
					bexit = false;
					rowIndex = erow + 1;
					while(!bexit && rowIndex<=trow)
					{
						for(int j=srow;j<=erow;j++)
						{
							this->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,j,grect);
							if(grect.bottom - grect.top > trow - rowIndex  )
							{
								bexit = true;
								break;//not enough to fill
							}
							if (grect.left ==i && grect.top==j && (grect.top !=grect.bottom || grect.left !=grect.right))
							{
								mrect.left = grect.left;
								mrect.right = grect.right;
								mrect.top = rowIndex;
								mrect.bottom = rowIndex +  (grect.bottom - grect.top);
								this->GetGridDataSvr()->m_pSpanSvr->Merge(mrect);
							}
							if(!(grect.left ==i && grect.top==j))
							{
								rowIndex += grect.bottom - grect.top + 1;
								j += grect.bottom - grect.top;
								continue;
							}
							PStrItem pItem;
							TCHAR * pNewFormula = NULL;
							pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(i,j);
							if(pItem)
							{
								PStrItem pItemEx = new StrItem;
								pItemEx->CopyFrom(pItem);
								pItem = pItemEx;
								this->GetGridDataSvr()->m_pContentSvr->SetItemData(i,rowIndex,pItem);
								if(pItem->pStrFormula)
								{
									pComplier->GetRCellFormula(pItem->pStrFormula,0,rowIndex - j ,pNewFormula);
									if(pNewFormula)
									{
										GetGridDataSvr()->m_pContentSvr->SetItemString(i,rowIndex,(TCHAR *)((std::wstring)_T("=")+pNewFormula).c_str());
										if(pNewFormula)delete pNewFormula;
									}
								}else
								{
									if(pItem->pStrContent && pItem->pStrContent[0]!='\0')
									{
										int k=0;
										for(k=0;k<(int)cells.size();k++)
										{
											if(cells[k].x<= j && cells[k].y >=j)break;
										}
										if(k<(int)cells.size())
										{
											if(!vals[k].breal)
											{

												vals[k].linum =  vals[k].linum + vals[k].ainum;
												::_stprintf_s(vbuf,80,L"%i",vals[k].linum);
												this->GetGridDataSvr()->m_pContentSvr->SetItemString(i,rowIndex,vbuf);

											}else
											{
												vals[k].lrnum = vals[k].lrnum + vals[k].arnum;
												::_stprintf_s(vbuf,80,L"%f",vals[k].lrnum);
												this->GetGridDataSvr()->m_pContentSvr->SetItemString(i,rowIndex,vbuf);
											}
										}
									}
								}
							}
							rowIndex = rowIndex + grect.bottom - grect.top + 1;
							if(rowIndex>trow) {bexit = true;break;}
							if(grect.top != grect.bottom) j += grect.bottom - grect.top;
						}
					}
				}
			}
		}
		if(pComplier)delete pComplier; 
		return true;
	}
	
	bool CGridFrameSvr::EvtLButtonUp(TEvent *pEvent ,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		this->ReleaseCapture();
		switch(FGridState)
		{
		case gsFilling:
			{
				TGridCoord NewAnchor;
				TGridCoord NewFillingAnchor;
				this->EvtMouseMove(pEvent,lParam);
				KillTimer(GetHWND(),ID_TIMER);
				TGridDrawInfo DrawInfo;
				this->InitDrawInfo(&DrawInfo);
				if(FFillingAnchor.x >= (FCurrent.x <= FAnchor.x? FCurrent.x:FAnchor.x) &&
					FFillingAnchor.x <= (FCurrent.x >= FAnchor.x? FCurrent.x:FAnchor.x))
				{
					NewAnchor.y = FFillingAnchor.y;
					NewAnchor.x = FAnchor.x;
					if(DoAutoFilling())this->MoveAnchor(NewAnchor);
					NewFillingAnchor.x =-1;
					NewFillingAnchor.y =-1;
					this->MoveFillingAnchor(DrawInfo,NewFillingAnchor);
					
					//redraw selection border
					TGridRect	rect;
					rect.left = FCurrent.x < FAnchor.x?FCurrent.x:FAnchor.x;
					rect.right = FCurrent.x > FAnchor.x?FCurrent.x:FAnchor.x;
					rect.top = FCurrent.y < FAnchor.y?FCurrent.y:FAnchor.y;
					rect.bottom = FCurrent.y > FAnchor.y?FCurrent.y:FAnchor.y;

					RECT srect;
					this->BoxRect(DrawInfo,rect,srect);
					::InflateRect(&srect,-2,-2);
					::ValidateRect(GetHWND(),&srect);
					::InflateRect(&srect,4,4);
					::InvalidateRect(GetHWND(),&srect,false);
				}else if(FFillingAnchor.y >= (FCurrent.y <= FAnchor.y? FCurrent.y:FAnchor.y) &&
					FFillingAnchor.y <= (FCurrent.y >= FAnchor.y? FCurrent.y:FAnchor.y))
				{
					NewAnchor.x = FFillingAnchor.x;
					NewAnchor.y = FAnchor.y;
					if(DoAutoFilling())	this->MoveAnchor(NewAnchor);
					NewFillingAnchor.x =-1;
					NewFillingAnchor.y =-1;
					this->MoveFillingAnchor(DrawInfo,NewFillingAnchor);

					//redraw selection border
					TGridRect	rect;
					rect.left = FCurrent.x < FAnchor.x?FCurrent.x:FAnchor.x;
					rect.right = FCurrent.x > FAnchor.x?FCurrent.x:FAnchor.x;
					rect.top = FCurrent.y < FAnchor.y?FCurrent.y:FAnchor.y;
					rect.bottom = FCurrent.y > FAnchor.y?FCurrent.y:FAnchor.y;

					RECT srect;
					this->BoxRect(DrawInfo,rect,srect);
					::InflateRect(&srect,-2,-2);
					::ValidateRect(GetHWND(),&srect);
					::InflateRect(&srect,4,4);
					::InvalidateRect(GetHWND(),&srect,false);
				}
				UpdateEdit();
				Click();
				break;
			}
		case gsCellsMoving:
			{
				TGridRect rs = GetSelection();
				if(m_ptPositionStart.x != rs.left || m_ptPositionStart.y!=rs.top)
				{
					this->DrawMovingLine();
					DoMoveCells();
					this->FAnchor.x = FAnchor.x + m_ptPositionStart.x - FCurrent.x; 
					this->FAnchor.y = FAnchor.y + m_ptPositionStart.y - FCurrent.y; 
					this->FCurrent  = m_ptPositionStart;
					InvalidateGrid();
				}
				break;
			}
		case gsSelecting:
			{
				::KillTimer(GetHWND(),ID_TIMER);
				if(FOldAnchor.x !=FAnchor.x || FOldAnchor.y !=FAnchor.y)
				{
					CELLNMHDR hdr;
					memset(&hdr,0,sizeof(CELLNMHDR));
					hdr.hwndFrom = GetHWND();
					hdr.idFrom = 0;
					hdr.code = CELL_SELECTCHANGED;
					hdr.currentrow = FCurrent.y;
					hdr.currentcol = FCurrent.x;
					hdr.anchorrow  = FAnchor.y;
					hdr.anchorcol  = FAnchor.x;

					HWND hWnd = GetHWND();

					::SendMessage(::GetParent(::GetParent(GetHWND())),WM_NOTIFY,0,(LPARAM)&hdr);
				}

				break;
			}
		case gsRowsSizing:
		case gsColsSizing:
			{
				DrawSizingLine(m_ptMouse); //{ XOR it out }
				DoMoveCells(m_ptStart,point);
				break;
			}
 		case gsDrawLining:
		case gsDeleteLining:
			{
				if(m_ptStart.x == -1 && m_ptStart.y == -1)break;
				//CalcDrawInfo(DrawInfo);
				DrawInsLine(m_ptMouse);
				if(FGridState == gsDrawLining)
					this->DoInsLine(m_ptStart,m_ptMouse);
				else
					this->DoDeleteLine(m_ptStart,m_ptMouse);
				m_ptStart.x = -1;
				m_ptStart.y = -1;
				return true;		
			}
		}
		this->FGridState = gsNormal;
		return true;
	}

	bool CGridFrameSvr::EvtMouseMove(TEvent *pEvent ,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		TGridCoord Coord;
		if(::PtInRect(&FRect,point))
		{
			Coord = CalcCoordFromPoint(point.x,point.y);
			switch(FGridState)
			{
			case gsSelecting:
				{
					GetGridDataSvr()->m_pSpanSvr->GetSpanedRowCol(Coord.x,Coord.y,Coord.x,Coord.y);
					this->FAnchor = Coord;
					InvalidateGrid();
					break;
				}
			case gsRowsSizing:
			case gsColsSizing:
				{
					DrawSizingLine(m_ptMouse); //{ XOR it out }
					DrawSizingLine(point); //{ XOR it back in }
					break;
				}
			case gsCellsMoving:
				{
					TGridRect rs = GetSelection();
					TGridCoord CellHit = Coord;
					if(CellHit.x != m_ptPosition.x || 
						CellHit.y != m_ptPosition.y)
					{
						TGridCoord newPos = m_ptPositionStart;
						newPos.x =  newPos.x + CellHit.x - m_ptPosition.x < 1?
							1: newPos.x + CellHit.x - m_ptPosition.x;
						newPos.y =  newPos.y + CellHit.y - m_ptPosition.y < 1?
							1: newPos.y + CellHit.y - m_ptPosition.y;
						if(newPos.x != m_ptPositionStart.x ||
							newPos.y != m_ptPositionStart.y)
						{
							if(m_ptPositionStart.x != rs.left || m_ptPositionStart.y!=rs.top)
								DrawMovingLine();
							m_ptPositionStart = newPos;
							if(m_ptPositionStart.x != rs.left || m_ptPositionStart.y!=rs.top)
								DrawMovingLine();
							m_ptPosition = CellHit;
						}
					}
					break;
				}
			case gsDrawLining:
			case gsDeleteLining:
				{
					if(m_ptStart.x == -1 && m_ptStart.y == -1)break;
					if(m_ptMouse.x !=-1 && m_ptMouse.y != -1)
						DrawInsLine(m_ptMouse);
					DrawInsLine(point);
					m_ptMouse = point;
					break;
				}
			case gsFilling:
				{
					//this->FHitTest = point;
					TGridDrawInfo DrawInfo;
					this->InitDrawInfo(&DrawInfo);
					TGridCoord CellHit = CalcCoordFromPoint(point.x, point.y);
					MoveFillingAnchor(DrawInfo,CellHit);
					break;
				}
			default:
				{
					if(abs(point.x - m_ptTrace.x) > 0 || abs(point.y - m_ptTrace.y) > 0)
					{
						if(m_TipHWND && ::IsWindow(m_TipHWND))
						{
							::DestroyWindow(m_TipHWND);
							m_TipHWND = NULL;
						}
						if(FMouseTrace) ::KillTimer(GetHWND(),ID_TRACETIMER);
						FMouseTrace = true;
						m_nDelayed = 500;
						::SetTimer(GetHWND(),ID_TRACETIMER,m_nDelayed,0);
					}
					m_ptTrace = point;
					//if(!FMouseTrace)
					//{
						/*if(m_TipHWND && ::IsWindow(m_TipHWND))
						{
							::DestroyWindow(m_TipHWND);
							m_TipHWND = NULL;
						}
						if(!(m_TipHWND && ::IsWindow(m_TipHWND)))
						{*/
							/*
							TRACKMOUSEEVENT	tve;
							tve.cbSize = sizeof(TRACKMOUSEEVENT);
							tve.dwFlags = TME_LEAVE;
							tve.hwndTrack = pEvent->hWnd;
							tve.dwHoverTime = HOVER_DEFAULT;

							TrackMouseEvent(&tve);
							*/

							//FMouseTrace = true;
							//m_ptTrace = point;
							//m_nDelayed = 0;
							//::SetTimer(GetHWND(),ID_TRACETIMER,500,0);
						//}
					//}
				}
			}
		}
		this->m_ptMouse = point;
		return true;
	}

	int CGridFrameSvr::CalcRowFromPoint(int X, int Y)
	{
		if(Y < FRect.top + 1) return -1;
		int LineY = FRect.top + 1; 
		int ARow = this->FTopLeft.y;
		LineY += (this->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow)<1?0:GetRowHeight(ARow));
		while(LineY < Y) 
		{
			ARow++;
			if(ARow > this->GetRowCount())break;
			if(GetRowHeight(ARow) < 1) continue;
			LineY += this->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(ARow) + this->FGridLineWidth;
		}
		return ARow;
	}
	int CGridFrameSvr::CalcColFromPoint(int X, int Y)
	{
		if(X < FRect.left + 1) return -1;
		int LineX = FRect.left + 1;
		int ACol = this->FTopLeft.x;

		int ARow = CalcRowFromPoint(X,Y);
		LineX += GetGridDataSvr()->GetAxisSvr(ARow)->GetColWidth(ACol);
		while(LineX < X )
		{
			ACol++;
			if(ACol > this->GetColCount())break;
			LineX += GetGridDataSvr()->GetAxisSvr(ARow)->GetColWidth(ACol)  + this->FGridLineWidth;
		}
		return ACol;
	}

	TGridCoord CGridFrameSvr::CalcCoordFromPoint(int X, int Y)
	{
		TGridCoord Coord;

		Coord.y = CalcRowFromPoint(X,Y);
		Coord.x = CalcColFromPoint(X,Y);

		return Coord;
	}

	BOOL CGridFrameSvr::DoInsLine(POINT start, POINT end)
	{
		int offset = 10;
		enum Arrow {vert,horz} arrow; //draw arrow

		//Get line rect
		RECT	LineRect;
		::SetRect(&LineRect,start.x,start.y,end.x,end.y);
		NormalizeRect(LineRect);

		TGridDrawInfo DrawInfo;
		RECT	StartSpanRect,EndSpanRect;

		//get hit cell
		this->InitDrawInfo(&DrawInfo);
		TGridCoord StartCellHit = CalcCoordFromPoint(start.x, start.y);
		//get cell span && get cell axis
		this->GetCellRect(StartCellHit.x,StartCellHit.y,StartSpanRect);

		if(LineRect.right - LineRect.left < offset + FGridLineWidth && LineRect.bottom - LineRect.top < offset + FGridLineWidth)
		{
			//unable to get arrow
			return FALSE;
		}else if(LineRect.right - LineRect.left < offset + FGridLineWidth && LineRect.bottom - LineRect.top >= 0.4 * (StartSpanRect.bottom - StartSpanRect.top) )
		{
			//should vert arrow 
			arrow = vert;
			end.x = start.x;	//adjust x axis
		}else if(LineRect.bottom - LineRect.top < offset + FGridLineWidth && LineRect.right - LineRect.left >= 0.4 * (StartSpanRect.right - StartSpanRect.left))
		{
			//should horz arrow
			arrow = horz;
			end.y = start.y;	//adjust y axis
		}else
		{
			//not operator
			return FALSE;
		}

		//get end point hit cell,the cell is spaned
		TGridCoord EndCellHit = CalcCoordFromPoint(end.x, end.y);

		//get end cell span && get end cell axis
		 this->GetCellRect(EndCellHit.x,EndCellHit.y,EndSpanRect);

		TGridCoord	unSpanStartCell	;//not span
		RECT   unSpanStartRect;	//not span
		//get start cell info
		unSpanStartCell = CalcCoordFromPoint(start.x, start.y);
		GetCellRect(unSpanStartCell.x,unSpanStartCell.y,unSpanStartRect,false);

		//get cell pos
		int CellPosX,CellPosY;
		CellPosX = unSpanStartRect.left;
		CellPosY = unSpanStartRect.top;

		//get split col and row
		int SplitCol,SplitRow;
		SplitCol = unSpanStartCell.x;
		SplitRow = unSpanStartCell.y;

		TGridRect	StartCellRect,EndCellRect;	//cell span Rect
		this->GetGridDataSvr()->m_pSpanSvr->isSpaned(StartCellHit.x,StartCellHit.y,StartCellRect);
		this->GetGridDataSvr()->m_pSpanSvr->isSpaned(EndCellHit.x,EndCellHit.y,EndCellRect);


		if(arrow == vert)
		{
			//vert split
			BOOL	bShouldNewCol = FALSE;
			int		width;

			//get unspan cell width
			width = this->GetColWidth(SplitCol);
			if(start.x - CellPosX >= (offset >> 1) && width - start.x + CellPosX >= (offset >> 1))
			{
				int FromIndex,ToIndex;
				m_ColCount++;
				FromIndex = GetColCount() - 1;
				ToIndex = SplitCol + 1;
				//GetGridDataSvr()->m_pSpanSvr->InsertCol(ToIndex);
				GetGridDataSvr()->m_pAxisSvr->m_pColWidths->Insert(ToIndex,1);

				MoveAdjust(FCurrent.x, FromIndex, ToIndex);
				MoveAdjust(FAnchor.x, FromIndex, ToIndex);
				MoveAdjust(FInplaceCol, FromIndex, ToIndex);
				MoveColData(FromIndex,ToIndex);
				this->GetGridDataSvr()->GetStyleSvr()->InsertCol(ToIndex,1);

		
				for(int i=1;i<GetRowCount();i++)
				{
					PStrItem	pItem;
					pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(GetColCount() - 1,i);
					if(pItem)
					{
						delete pItem;
						this->GetGridDataSvr()->m_pContentSvr->SetItemData(GetColCount() - 1,i,NULL);
					}
				}

				//set new column width
				this->SetColWidth(SplitCol + 1, width - start.x + CellPosX - DrawInfo.EffectiveLineWidth);

				//adjust old column width
				this->SetColWidth(SplitCol, start.x - CellPosX);

				bShouldNewCol = TRUE;
			}else
			{
				//not need insert newcolumn
				bShouldNewCol = FALSE;
				if(start.x - CellPosX < (offset >> 1))SplitCol--;
			}
			if(SplitCol < 1 ) return FALSE;

			//get valid row and col
			int row,col;
			this->GetGridDataSvr()->m_pContentSvr->GetValidRowCol(col,row);
			if(row > 30 && row >= this->GetRowCount() - 1)row = 30;
			if(col > 12 && col >= this->GetColCount() - 1) col = 12;

			int i;
			int CellTop,CellBottom;

			//get top 
			CellTop = StartCellRect.top < EndCellRect.top ?StartCellRect.top:EndCellRect.top;
			//get bottom
			CellBottom = StartCellRect.bottom > EndCellRect.bottom ?StartCellRect.bottom:EndCellRect.bottom;
			if(CellBottom > row)row = CellBottom ;

			TGridRect sRect;
			RECT tRect;
			int index;
			if(bShouldNewCol)
			{
				//span right
				for(i=0;i<GetGridDataSvr()->m_pSpanSvr->SpanRects.size();i++)
				{
					if(GetGridDataSvr()->m_pSpanSvr->SpanRects[i].left > SplitCol)
					{
						GetGridDataSvr()->m_pSpanSvr->SpanRects[i].left ++;
						GetGridDataSvr()->m_pSpanSvr->SpanRects[i].right ++;
					}else if(GetGridDataSvr()->m_pSpanSvr->SpanRects[i].right >= SplitCol)
					{
						GetGridDataSvr()->m_pSpanSvr->SpanRects[i].right ++;
					}
				}

				//span top
				for(i=1; i< CellTop;i++)
				{
					if(GetGridDataSvr()->m_pSpanSvr->isSpaned(SplitCol,i,sRect,index))
					{
						i = sRect.bottom;
					}else
					{
						::SetRect(&tRect,SplitCol,i,SplitCol + 1,i);
						GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
					}
				}

				//span bottom
				for(i=CellBottom + 1;i<=row;i++)
				{
					if(GetGridDataSvr()->m_pSpanSvr->isSpaned(SplitCol,i,sRect,index))
					{
						i = sRect.bottom;
					}else
					{
						::SetRect(&tRect,SplitCol,i,SplitCol + 1,i);
						GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
					}
				}
			}
		

			//split process
			PStrItem	pItem;
			for(i=CellTop;i<=CellBottom;i++)
			{
				if(this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SplitCol,i,sRect,index))
				{
					if(GetGridDataSvr()->m_pSpanSvr->SpanRects[index].right <=SplitCol) continue;

					//split span
					RECT	spRect = GetGridDataSvr()->m_pSpanSvr->SpanRects[index];
					int pos = GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(spRect);
					GetGridDataSvr()->m_pSpanSvr->SpanRects[pos].left =SplitCol + 1;

					//adjust owner right
					GetGridDataSvr()->m_pSpanSvr->SpanRects[index].right = SplitCol;

					pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(GetGridDataSvr()->m_pSpanSvr->SpanRects[index].left,GetGridDataSvr()->m_pSpanSvr->SpanRects[index].top);
					if(pItem)
					{
						//pItem->mask = STI_STYLE;
						//this->SetCellsItem(GetSpans()[pos].left,GetSpans()[pos].top,pItem);
					}
					i = sRect.bottom;
				}else
				{
					if(!bShouldNewCol) continue;
					::SetRect(&tRect,SplitCol + 1,i,SplitCol + 1,i);
					int pos = GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
					pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(SplitCol,i);
					if(pItem)
					{
						//pItem->mask = STI_STYLE;
						//this->SetCellsItem(SplitCol + 1,i,pItem);
					}
				}
			}
		}else if(arrow == horz)
		{
			//horz split
			BOOL	bShouldNewRow = FALSE;
			int		height;

			//get unspan cell height
			height = this->GetRowHeight(SplitRow);
			if(height < 1) return false;
			if(start.y - CellPosY >= (offset >> 1) && height - start.y + CellPosY >= (offset >> 1))
			{
				//adjust data
				int FromIndex,ToIndex;
				m_RowCount++;
				FromIndex = GetRowCount() - 1;
				ToIndex = SplitRow + 1;

				//GetGridDataSvr()->m_pSpanSvr->InsertRow(ToIndex);
				GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Insert(ToIndex,1);

				MoveAdjust(FCurrent.y, FromIndex, ToIndex);
				MoveAdjust(FAnchor.y, FromIndex, ToIndex);
				MoveAdjust(FInplaceRow, FromIndex, ToIndex);
				MoveRowData(FromIndex,ToIndex);
				this->GetGridDataSvr()->GetStyleSvr()->InsertRow(ToIndex,1);

				//set new row height
				this->SetRowHeight(SplitRow + 1, height - start.y + CellPosY - DrawInfo.EffectiveLineWidth);

				//adjust old column width
				this->SetRowHeight(SplitRow, start.y - CellPosY);

				bShouldNewRow = TRUE;
			}else
			{
				//not need insert newcolumn
				bShouldNewRow = FALSE;
				if(start.y - CellPosY < (offset >> 1))SplitRow--;
			}
			if(SplitRow < 1 ) return FALSE;

			//get valid row and col
			int row,col;
			this->GetGridDataSvr()->m_pContentSvr->GetValidRowCol(col,row);
			if(row > 30 && row >= this->GetRowCount() - 1)row = 30;
			if(col > 12 && col >= this->GetColCount() - 1) col = 12;

			int i;
			int CellLeft,CellRight;

			//get left 
			CellLeft = StartCellRect.left < EndCellRect.left ?StartCellRect.left:EndCellRect.left;

			//get right
			CellRight = StartCellRect.right > EndCellRect.right ?StartCellRect.right:EndCellRect.right;
			if(CellRight > col)col = CellRight;

			TGridRect sRect;
			RECT	tRect;
			int index;

			if(bShouldNewRow)
			{
				//span bottom
				for(i=0;i<GetGridDataSvr()->m_pSpanSvr->SpanRects.size();i++)
				{
					if(GetGridDataSvr()->m_pSpanSvr->SpanRects[i].top > SplitRow)
					{
						GetGridDataSvr()->m_pSpanSvr->SpanRects[i].top ++;
						GetGridDataSvr()->m_pSpanSvr->SpanRects[i].bottom ++;
					}else if(GetGridDataSvr()->m_pSpanSvr->SpanRects[i].bottom >= SplitRow)
					{
						GetGridDataSvr()->m_pSpanSvr->SpanRects[i].bottom ++;
					}
				}


				//span left
				for(i=1; i< CellLeft;i++)
				{
					if(GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SplitRow,sRect,index))
					{
						i = sRect.right;
					}else
					{
						::SetRect(&tRect,i,SplitRow,i,SplitRow + 1);
						GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
					}
				}

				//span right
				for(i=CellRight + 1;i<=col;i++)
				{
					if(GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SplitRow,sRect,index))
					{
						i = sRect.right;
					}else
					{
						::SetRect(&tRect,i,SplitRow,i,SplitRow + 1);
						GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
					}
				}
			}

			//split process
			//PStrItem	pItem;
			for(i=CellLeft;i<=CellRight;i++)
			{
				if(GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SplitRow,sRect,index))
				{
					//split span
					if(GetGridDataSvr()->m_pSpanSvr->SpanRects[index].bottom <=SplitRow) continue;
					RECT	spRect = GetGridDataSvr()->m_pSpanSvr->SpanRects[index];
					int pos = GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(spRect);
					GetGridDataSvr()->m_pSpanSvr->SpanRects[pos].top = SplitRow + 1;

					//adjust owner bottom
					GetGridDataSvr()->m_pSpanSvr->SpanRects[index].bottom = SplitRow;

					/*pItem = this->GetCellsItem(GetSpans()[index].left,GetSpans()[index].top);
					if(pItem)
					{
						pItem->mask = STI_STYLE;
						this->SetCellsItem(GetSpans()[pos].left,GetSpans()[pos].top,pItem);
					}*/
					i = sRect.right;
				}else
				{
					if(!bShouldNewRow) continue;
					::SetRect(&tRect,i,SplitRow+1,i,SplitRow + 1);
					int pos = GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
					/*pItem = this->GetCellsItem(i,SplitRow);
					if(pItem)
					{
						pItem->mask = STI_STYLE;
						this->SetCellsItem(i,SplitRow + 1,pItem);
					}*/
				}
			}
		}

		this->InvalidateGrid();
		return TRUE;
	}

	BOOL CGridFrameSvr::DoDeleteLine(POINT start, POINT end)
	{
		int offset = 10;
		enum Arrow {vert,horz} arrow; //draw arrow

		//Get line rect
		RECT	LineRect;
		::SetRect(&LineRect,start.x,start.y,end.x,end.y);
		NormalizeRect(LineRect);

		TGridDrawInfo DrawInfo;
		RECT	StartSpanRect,EndSpanRect;

		//get hit cell
		this->InitDrawInfo(&DrawInfo);
		TGridCoord StartCellHit = CalcCoordFromPoint(start.x, start.y);
		//get cell span && get cell axis
		this->GetCellRect(StartCellHit.x,StartCellHit.y,StartSpanRect);

		if(LineRect.right - LineRect.left < offset + FGridLineWidth && LineRect.bottom - LineRect.top < offset + FGridLineWidth)
		{
			//unable to get arrow
			return FALSE;
		}else if(LineRect.right - LineRect.left < offset + FGridLineWidth && LineRect.bottom - LineRect.top >= 0.4 * (StartSpanRect.bottom - StartSpanRect.top) )
		{
			//should vert arrow 
			arrow = vert;
			end.x = start.x;	//adjust x axis
		}else if(LineRect.bottom - LineRect.top < offset + FGridLineWidth && LineRect.right - LineRect.left >= 0.4 * (StartSpanRect.right - StartSpanRect.left))
		{
			//should horz arrow
			arrow = horz;
			end.y = start.y;	//adjust y axis
		}else
		{
			//not operator
			return FALSE;
		}

		//get end point hit cell,the cell is spaned
		TGridCoord EndCellHit = CalcCoordFromPoint(end.x, end.y);

		//get end cell span && get end cell axis
		 this->GetCellRect(EndCellHit.x,EndCellHit.y,EndSpanRect);

		TGridCoord	unSpanStartCell	;//not span
		RECT   unSpanStartRect;	//not span
		//get start cell info
		unSpanStartCell = CalcCoordFromPoint(start.x, start.y);
		GetCellRect(unSpanStartCell.x,unSpanStartCell.y,unSpanStartRect,false);

		//get cell pos
		int CellPosX,CellPosY;
		CellPosX = unSpanStartRect.left;
		CellPosY = unSpanStartRect.top;

		//get split col and row
		int SplitCol,SplitRow;
		SplitCol = unSpanStartCell.x;
		SplitRow = unSpanStartCell.y;

		TGridRect	StartCellRect,EndCellRect;	//cell span Rect
		this->GetGridDataSvr()->m_pSpanSvr->isSpaned(StartCellHit.x,StartCellHit.y,StartCellRect);
		this->GetGridDataSvr()->m_pSpanSvr->isSpaned(EndCellHit.x,EndCellHit.y,EndCellRect);


		if(arrow == vert)
		{
			if(start.x - CellPosX < (offset >> 1))SplitCol--;
			if(SplitCol < 1 ) return FALSE;

			//get valid row and col
			int row,col;
			this->GetGridDataSvr()->m_pContentSvr->GetValidRowCol(col,row);
			if(row > 30 && row >= this->GetRowCount() - 1)row = 30;
			if(col > 12 && col >= this->GetColCount() - 1) col = 12;

			int i;
			int CellTop,CellBottom;

			//get top 
			CellTop = StartCellRect.top < EndCellRect.top ?StartCellRect.top:EndCellRect.top;

			//get bottom
			CellBottom = StartCellRect.bottom > EndCellRect.bottom ?StartCellRect.bottom:EndCellRect.bottom;
			if(CellBottom > row)row = CellBottom;

			int indexLeft,indexRight;
			TGridRect	spRectLeft,spRectRight;
			RECT tRect;
			for(i = CellTop;i<=CellBottom;i++)
			{
				GetGridDataSvr()->m_pSpanSvr->isSpaned(SplitCol,i,spRectLeft,indexLeft);
				GetGridDataSvr()->m_pSpanSvr->isSpaned(SplitCol + 1,i,spRectRight,indexRight);
				if(spRectLeft.right ==SplitCol && spRectRight.left ==SplitCol + 1 &&
					spRectLeft.top == spRectLeft.top && spRectLeft.bottom == spRectLeft.bottom)
				{
					if(indexLeft >=0)
					{
						GetGridDataSvr()->m_pSpanSvr->SpanRects[indexLeft].right = spRectRight.right;
						::SetRect(&tRect,0,0,0,0);
						if(indexRight >=0)GetGridDataSvr()->m_pSpanSvr->SpanRects[indexRight] = tRect;
					}else
					{
						if(indexRight >=0)
							GetGridDataSvr()->m_pSpanSvr->SpanRects[indexRight].left = SplitCol;
						else
						{
							::SetRect(&tRect,SplitCol,i,SplitCol + 1,i);
							GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
						}
					}
				}
			}
		}else if(arrow = horz)
		{
			if(start.y - CellPosY < (offset >> 1))SplitRow--;
			if(SplitRow < 1 ) return FALSE;

			//get valid row and col
			int row,col;
			this->GetGridDataSvr()->m_pContentSvr->GetValidRowCol(col,row);
			if(row > 30 && row >= this->GetRowCount() - 1)row = 30;
			if(col > 12 && col >= this->GetColCount() - 1) col = 12;

			int i;
			int CellLeft,CellRight;

			//get top 
			CellLeft = StartCellRect.left < EndCellRect.left ?StartCellRect.left:EndCellRect.left;

			//get bottom
			CellRight = StartCellRect.right > EndCellRect.right ?StartCellRect.right:EndCellRect.right;
			if(CellRight > col)col = CellRight;

			int indexTop,indexBottom;
			TGridRect	spRectTop,spRectBottom;
			RECT tRect;
			for(i = CellLeft;i<=CellRight;i++)
			{
				GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SplitRow,spRectTop,indexTop);
				GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SplitRow + 1,spRectBottom,indexBottom);
				if(spRectTop.bottom ==SplitRow && spRectBottom.top ==SplitRow + 1 &&
					spRectTop.left == spRectTop.left && spRectTop.right == spRectTop.right)
				{
					if(indexTop >=0)
					{
						GetGridDataSvr()->m_pSpanSvr->SpanRects[indexTop].bottom = spRectBottom.bottom;
						::SetRect(&tRect,0,0,0,0);
						if(indexBottom >=0)GetGridDataSvr()->m_pSpanSvr->SpanRects[indexBottom] = tRect;
					}else
					{
						if(indexBottom >=0)
							GetGridDataSvr()->m_pSpanSvr->SpanRects[indexBottom].top = SplitRow;
						else
						{
							::SetRect(&tRect,i,SplitRow,i,SplitRow + 1);
							GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(tRect);
						}
					}
				}
			}
		}
		this->InvalidateGrid();
		return TRUE;
	}

	BOOL CGridFrameSvr::DoMoveCells(POINT start, POINT end)
	{
		TGridRect	SelectionCell;
		RECT SelectionRect;
		TGridRect	TempCell;
		RECT	TempRect;
		int		From,To;
		TGridRect	mRect;
		enum Arrow{left,right,top,bottom,none} arrow;

		SelectionCell = GetSelection();
		TGridDrawInfo DrawInfo;
		this->InitDrawInfo(& DrawInfo);
		BoxRect(DrawInfo,SelectionCell,SelectionRect );

		arrow = none;
		if(this->FGridState == gsColsSizing)
		{
			if(abs(end.x - start.x) < 5)return FALSE;

			if(abs(SelectionRect.left - start.x) < 5 && end.x < SelectionRect.left)
				arrow = left;
			if(abs(SelectionRect.right - start.x) < 5 && end.x > SelectionRect.right)
				arrow = right;
		}
		else
		{
			if(abs(end.y - start.y) < 5)return FALSE;

			if(abs(SelectionRect.top - start.y) < 5 && end.y < SelectionRect.top)
				arrow = top;
			if(abs(SelectionRect.bottom - start.y) < 5 && end.y > SelectionRect.bottom)
				arrow = bottom;
		}
		if(arrow == left)
		{
			//left
			if(SelectionCell.left <= 0)
				return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left - 1,SelectionCell.top,TempCell);	//top
			if(TempCell.top != SelectionCell.top) return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left - 1,SelectionCell.bottom,TempCell);	//bottom
			if(TempCell.bottom != SelectionCell.bottom) return FALSE;

			int i;
			for(i = SelectionCell.top ;i<=SelectionCell.bottom;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left - 1,i,TempCell);
				this->GetCellRect(TempCell.left,TempCell.top,TempRect);
				if(TempRect.left > end.x)return FALSE;
				i = TempCell.bottom;
			}
			this->GetCellRect(SelectionCell.left,SelectionCell.top,TempRect);
			From = TempRect.top;

			this->GetCellRect(SelectionCell.left,SelectionCell.bottom,TempRect);
			To = TempRect.bottom;

			POINT ptStart,ptEnd;
			ptStart.x = end.x; ptStart.y = From;
			ptEnd.x = end.x; ptEnd.y = To;
			this->DoInsLine(ptStart,ptEnd);
			SelectionCell = GetSelection();

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left - 1,SelectionCell.top,TempCell);	//top
			mRect.top = TempCell.top;
			mRect.left = TempCell.left;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left - 1,SelectionCell.bottom,TempCell);	//bottom
			mRect.bottom = TempCell.bottom;
			mRect.right = TempCell.right;
			this->GetGridDataSvr()->m_pSpanSvr->UnMerge(mRect);

			TGridRect tRect;
			PStrItem	pItem;
			for(i = SelectionCell.top;i<=SelectionCell.bottom;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left,i,TempCell);
				::SetRect((RECT *)&tRect,mRect.left,i,TempCell.right,TempCell.bottom);
				this->GetGridDataSvr()->m_pSpanSvr->Merge(tRect);

				pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(TempCell.left,TempCell.top);

				this->GetGridDataSvr()->m_pContentSvr->SetItemData(TempCell.left,TempCell.top,NULL);
				this->GetGridDataSvr()->m_pContentSvr->SetItemData(mRect.left,TempCell.top,pItem);
				i = TempCell.bottom;
			}
		}
		else if(arrow == top)
		{
			//left
			if(SelectionCell.top <= 0)
				return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left,SelectionCell.top - 1,TempCell);	//top
			if(TempCell.left != SelectionCell.left) return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right,SelectionCell.top - 1,TempCell);	//bottom
			if(TempCell.right != SelectionCell.right) return FALSE;

			int i;
			for(i = SelectionCell.left ;i<=SelectionCell.right;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SelectionCell.top - 1,TempCell);
				this->GetCellRect(TempCell.left,TempCell.top,TempRect);
				if(TempRect.top > end.y)return FALSE;
				i = TempCell.right;
			}
			this->GetCellRect(SelectionCell.left,SelectionCell.top,TempRect);
			From = TempRect.left;

			 this->GetCellRect(SelectionCell.right,SelectionCell.top,TempRect);
			To = TempRect.right ;

			POINT ptStart,ptEnd;
			ptStart.x = From; ptStart.y = end.y;
			ptEnd.x = To; ptEnd.y = end.y;
			this->DoInsLine(ptStart,ptEnd);
			SelectionCell = GetSelection();

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right,SelectionCell.top - 1,TempCell);	//left
			mRect.top = TempCell.top;
			mRect.left = TempCell.left;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right,SelectionCell.top - 1,TempCell);	//right
			mRect.bottom = TempCell.bottom;
			mRect.right = TempCell.right;
			this->GetGridDataSvr()->m_pSpanSvr->UnMerge(mRect);

			TGridRect tRect;
			PStrItem	pItem;
			for(i = SelectionCell.left;i<=SelectionCell.right;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SelectionCell.top,TempCell);
				::SetRect((RECT *)&tRect,i,mRect.top,TempCell.right,TempCell.bottom);
				this->GetGridDataSvr()->m_pSpanSvr->Merge(tRect);

				pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(TempCell.left,TempCell.top);

				this->GetGridDataSvr()->m_pContentSvr->SetItemData(TempCell.left,TempCell.top,NULL);
				this->GetGridDataSvr()->m_pContentSvr->SetItemData(mRect.left,TempCell.top,pItem);
				i = TempCell.right;
			}
		}
		else if(arrow == right)
		{
			//left
			if(SelectionCell.right >= this->GetColCount() - 1)
				return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right + 1,SelectionCell.top,TempCell);	//top
			if(TempCell.top != SelectionCell.top) return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right + 1,SelectionCell.bottom,TempCell);	//bottom
			if(TempCell.bottom != SelectionCell.bottom) return FALSE;

			int i;
			for(i = SelectionCell.top ;i<=SelectionCell.bottom;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right + 1,i,TempCell);
				this->GetCellRect(TempCell.left,TempCell.top,TempRect);
				if(TempRect.right < end.x)return FALSE;
				i = TempCell.bottom;
			}
			this->GetCellRect(SelectionCell.right,SelectionCell.top,TempRect);
			From = TempRect.top ;

			this->GetCellRect(SelectionCell.right,SelectionCell.bottom,TempRect);
			To = TempRect.bottom;

			POINT ptStart,ptEnd;
			ptStart.x = end.x; ptStart.y = From;
			ptEnd.x = end.x; ptEnd.y = To;
			this->DoInsLine(ptStart,ptEnd);
			SelectionCell = this->GetSelection();

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right + 1,SelectionCell.top,TempCell);	//top
			mRect.top = TempCell.top;
			mRect.left = TempCell.left;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right + 1,SelectionCell.bottom,TempCell);	//bottom
			mRect.bottom = TempCell.bottom;
			mRect.right = TempCell.right;
			this->GetGridDataSvr()->m_pSpanSvr->UnMerge(mRect);

			TGridRect tRect;
			//PStrItem	pItem;
			for(i = SelectionCell.top;i<=SelectionCell.bottom;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right,i,TempCell);
				::SetRect((RECT *)&tRect,TempCell.left,i,mRect.right,TempCell.bottom);
				this->GetGridDataSvr()->m_pSpanSvr->Merge(tRect);
				i = TempCell.bottom;
			}
		}
		else if(arrow == bottom)
		{
			//left
			if(SelectionCell.bottom >= GetRowCount() - 1)
				return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.left,SelectionCell.bottom + 1,TempCell);	//top
			if(TempCell.left != SelectionCell.left) return FALSE;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right,SelectionCell.bottom + 1,TempCell);	//bottom
			if(TempCell.right != SelectionCell.right) return FALSE;

			int i;
			for(i = SelectionCell.left ;i<=SelectionCell.right;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SelectionCell.bottom + 1,TempCell);
				this->GetCellRect(TempCell.left,TempCell.top,TempRect);
				if(TempRect.bottom < end.y)return FALSE;
				i = TempCell.right;
			}
			this->GetCellRect(SelectionCell.left,SelectionCell.top,TempRect);
			From = TempRect.left;

			this->GetCellRect(SelectionCell.right,SelectionCell.top,TempRect);
			To = TempRect.right;

			POINT ptStart,ptEnd;
			ptStart.x = From; ptStart.y = end.y;
			ptEnd.x = To; ptEnd.y = end.y;
			this->DoInsLine(ptStart,ptEnd);
			SelectionCell = GetSelection();

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right,SelectionCell.bottom + 1,TempCell);	//left
			mRect.top = TempCell.top;
			mRect.left = TempCell.left;

			this->GetGridDataSvr()->m_pSpanSvr->isSpaned(SelectionCell.right,SelectionCell.bottom + 1,TempCell);	//right
			mRect.bottom = TempCell.bottom;
			mRect.right = TempCell.right;
			this->GetGridDataSvr()->m_pSpanSvr->UnMerge(mRect);

			TGridRect tRect;
			//PStrItem	pItem;
			for(i = SelectionCell.left;i<=SelectionCell.right;i++)
			{
				this->GetGridDataSvr()->m_pSpanSvr->isSpaned(i,SelectionCell.bottom,TempCell);
				::SetRect((RECT *)&tRect,i,TempCell.top,TempCell.right,mRect.bottom);
				this->GetGridDataSvr()->m_pSpanSvr->Merge(tRect);
				i = TempCell.right;
			}
		}
		return TRUE;
	}

	int CGridFrameSvr::MoveCurrent(TGridCoord Coord)
	{
		FCurrent = Coord;
		if(GetHWND())::InvalidateRect(GetHWND(),NULL,FALSE);
		return 1;
	}

	bool CGridFrameSvr::CanEditAcceptKey(char Key)
	{
		return true;
	}
	bool CGridFrameSvr::CanGridAcceptKey(WORD Key, TShiftState Shift)
	{
		return true;
	}
	bool CGridFrameSvr::CanEditModify()
	{
		return FCanEditModify;
	}

	bool CGridFrameSvr::CanEditShow()
	{
		if(FOptions & goRunState)
		{
			LPTSTR protect = this->GetItemProp(FCurrent.x,FCurrent.y,_T("cellprotect"));
			if(protect)
			{
				if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
					_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) 
				{
					delete protect;
					return false;
				}
				delete protect;
			}
		}

		return  (!(goRowSelect &FOptions) && goAllowEdit&FOptions) &&
			FEditorMode &&  HandleAllocated &&
			((goAlwaysShowEditor & FOptions) || IsActiveControl());
	}

	bool CGridFrameSvr::IsActiveControl()
	{
		/*  H: Hwnd;
		ParentForm: TCustomForm;

		Result = False;
		ParentForm = GetParentForm(Self);
		if Assigned(ParentForm) then
		begin
		if (ParentForm.ActiveControl = Self) then
		Result = TRUE
		end
		else
		begin
		H = GetFocus;
		while IsWindow(H)&&(Result = False) do
		begin
		if H = WindowGetHWND() then
		Result = TRUE
		else
		H = GetParent(H);
		end;
		end;
		*/
		return true;
	}

	void CGridFrameSvr::InvalidateEditor()
	{
		FInplaceCol = -1;
		FInplaceRow = -1;
		UpdateEdit();
	}

	void CGridFrameSvr::UpdateEditor()
	{
		/*
		if(FInplaceEdit == NULL) return;
		FInplaceCol = GetCol();
		FInplaceRow = GetRow();
		if(this->m_bEditFloat)
		FInplaceEdit->UpdateFloatContent();
		else
		FInplaceEdit->UpdateContents();
		if(FInplaceEdit->MaxLength == -1) FCanEditModify = FALSE;
		else FCanEditModify = TRUE;
		FInplaceEdit->SelectAll();*/
		m_pEditSvr->SelectCell(GetCol(), GetRow());
	}

	void CGridFrameSvr::UpdateEdit()
	{
		if(CanEditShow())
		{
			m_pEditSvr->SelectCell(GetCol(), GetRow());
			FInplaceCol = GetCol();
			FInplaceRow = GetRow();
		}
	}
	void CGridFrameSvr::ShowEditor()
	{
		FEditorMode = TRUE;
		UpdateEdit();
	}

	void CGridFrameSvr::HideEdit()
	{
		this->m_pEditSvr->HiddenEditor();
		FInplaceCol = -1;
		FInplaceRow = -1;
	}

	void CGridFrameSvr::HideEditor()
	{
		FEditorMode = FALSE;
		HideEdit();
	}

	bool CGridFrameSvr::SelectCell(int ACol, int ARow)
	{
		return true;
	}

	void CGridFrameSvr::ClampInView(TGridCoord Coord)
	{
		TGridDrawInfo DrawInfo ;
		TGridCoord MaxTopLeft ;
		TGridCoord OldTopLeft ;

		if( !HandleAllocated) return;
		this->InitDrawInfo(&DrawInfo);
	
		if((Coord.x > DrawInfo.LastFullVisibleCol) ||
			(Coord.y > DrawInfo.LastFullVisibleRow) || (Coord.x < GetLeftCol()) || (Coord.y < GetTopRow()))
		{
			OldTopLeft = FTopLeft;
			MaxTopLeft = CalcMaxTopLeft(Coord, DrawInfo);
			Update();
			if(Coord.x < GetLeftCol()) FTopLeft.x = Coord.x;
			else if(DrawInfo.LastFullVisibleCol && Coord.x > DrawInfo.LastFullVisibleCol) FTopLeft.x = MaxTopLeft.x;
			if(Coord.y < GetTopRow()) FTopLeft.y = Coord.y;
			else if(Coord.y > DrawInfo.LastFullVisibleRow) FTopLeft.y = MaxTopLeft.y;
			TopLeftMoved(OldTopLeft);
		}
	}

	void CGridFrameSvr::MoveCurrent(int ACol,int ARow, BOOL MoveAnchor,BOOL Show)
	{
		TGridRect OldSel ;
		TGridCoord OldCurrent= FCurrent;

		if( (ACol < 0) || (ARow < 0) || (ACol >= GetColCount()) || (ARow >= GetRowCount())) return;
		if( SelectCell(ACol, ARow))
		{
			if( ! (goAlwaysShowEditor & FOptions)) HideEditor();
			OldSel = GetSelection();
			FCurrent.x = ACol;
			FCurrent.y = ARow;
			if( MoveAnchor || ! (goRangeSelect & FOptions))
			{
				FAnchor = FCurrent;
				if( goRowSelect & FOptions) FAnchor.x = GetColCount() - 1;
			}
			if( goRowSelect & FOptions) FCurrent.x = FFixedCols;
			if( Show) ClampInView(FCurrent);
			SelectionMoved(OldSel);
			InvalidateCell(OldCurrent.x, OldCurrent.y);
			InvalidateCell(ACol, ARow);
		}
		m_pSheetSvr->initialAgent(false);
		if(this->FOptions & goRunState)
		{
			CDwTable * pOldDwTable = GetGridDataSvr()->GetDwTableFromCell(OldCurrent.x,OldCurrent.y);
			CDwTable * pDwTable = GetGridDataSvr()->GetDwTableFromCell(ACol,ARow);
			if(pDwTable)
			{
				int nRow,nCol;
				pDwTable->getRowColumnFromCell(ACol,ARow,nRow,nCol);
				pDwTable->event_ItemFocusChanged(nRow,nCol);

				if(pOldDwTable ==pDwTable)
				{
					int nOldRow = pDwTable->GetRowFromCell(OldCurrent.y);
					if( nOldRow!= nRow)
						pDwTable->event_RowFocusChanged(nRow,nOldRow);
				}else
				{
					pDwTable->event_GetFocus(pOldDwTable);
					if(pOldDwTable)pOldDwTable->event_LoseFocus(pDwTable);
				}
			}
		}
		if(GetGridDataSvr()->GetChildIndex(OldCurrent.y)!= GetGridDataSvr()->GetChildIndex(ARow))
			::InvalidateRect(this->m_pSheetSvr->m_pColHeaderSvr->GetHWND(),NULL,false);
	}

	void CGridFrameSvr::AreaMoved(TGridRect OldSel,TGridRect NewSel)
	{
		TRect OldRect, NewRect;
		TGridRect SelRect;

		SelRect = NewSel;

		if( ! HandleAllocated) return;

		TGridDrawInfo DrawInfo;
		InitDrawInfo(&DrawInfo);
		BoxRect(DrawInfo,OldSel, OldRect);//, FALSE);
		BoxRect(DrawInfo,SelRect, NewRect);//, FALSE);

		//(A|B|C|D) - (A&C)|(B&D)
		RECT rectAB,rectCD,rectA,rectC;
		HRGN  rgnB,rgnD;
		HRGN	rgn,rgnTemp,rgnTemp1;

		rgn = CreateRectRgn(0,0,0,0);
		rgnTemp = CreateRectRgn(0,0,0,0);
		rgnTemp1 = CreateRectRgn(0,0,0,0);
		rgnB = CreateRectRgn(0,0,0,0);
		rgnD = CreateRectRgn(0,0,0,0);


		//A|B
		rectAB = NewRect;
		InflateRect(&rectAB,2,2);
		SetRectRgn(rgn,rectAB.left,rectAB.top,rectAB.right,rectAB.bottom);

		//C|D
		rectCD = OldRect;
		InflateRect(&rectCD,2,2);
		SetRectRgn(rgnTemp,rectCD.left,rectCD.top,rectCD.right,rectCD.bottom);

		//A|B|C|D
		CombineRgn(rgn,rgn, rgnTemp,RGN_OR);

		//A
		rectA = NewRect;
		InflateRect(&rectA,-1,-1);

		//C
		rectC = OldRect;
		InflateRect(&rectC,-1,-1);

		//A&C
		RECT rect;
		IntersectRect(&rect,&rectC,&rectA);
		SetRectRgn(rgnTemp,rect.left,rect.top,rect.right,rect.bottom);

		//B = A|B - A
		SetRectRgn(rgnB,rectAB.left,rectAB.top,rectAB.right,rectAB.bottom);
		SetRectRgn(rgnTemp1,rectA.left,rectA.top,rectA.right,rectA.bottom);
		CombineRgn(rgnB,rgnB, rgnTemp1,RGN_XOR);

		//D = C|D - C 
		SetRectRgn(rgnD,rectCD.left,rectCD.top,rectCD.right,rectCD.bottom);
		SetRectRgn(rgnTemp1,rectC.left,rectC.top,rectC.right,rectC.bottom);
		CombineRgn(rgnD,rgnD, rgnTemp1,RGN_XOR);

		//B&D
		CombineRgn(rgnTemp1,rgnB,rgnD,RGN_AND);

		//A&C |B&D
		CombineRgn(rgnTemp,rgnTemp,rgnTemp1,RGN_OR);

		////(A|B|C|D) - (A&C)|(B&D)
		CombineRgn(rgn,rgn,rgnTemp,RGN_DIFF);

		//patbit
		SetRectRgn(rgnTemp,rectA.right - 2,rectA.bottom - 2,rectA.right + 5,rectA.bottom + 5);
		SetRectRgn(rgnTemp1,rectC.right - 2,rectC.bottom - 2,rectC.right + 5,rectC.bottom + 5);
		CombineRgn(rgnTemp,rgnTemp,rgnTemp1,RGN_OR);
		CombineRgn(rgn,rgn,rgnTemp,RGN_OR);

		//title
		RECT	rect1;
		rect = OldRect;
		rect.left =0;
		rect.right = this->GetHeaderWidth();
		SetRectRgn(rgnTemp,rect.left,rect.top,rect.right,rect.bottom);
		rect1 = NewRect;
		rect1.left = 0;
		rect1.right = this->GetHeaderWidth();
		SetRectRgn(rgnTemp1,rect1.left,rect1.top,rect1.right,rect1.bottom);
		CombineRgn(rgnTemp,rgnTemp,rgnTemp1,RGN_XOR);
		CombineRgn(rgn,rgn,rgnTemp,RGN_OR);

		rect = OldRect;
		rect.top = 0;
		rect.bottom = this->GetHeaderHeight();
		SetRectRgn(rgnTemp,rect.left,rect.top,rect.right,rect.bottom);
		rect1 = NewRect;
		rect1.top = 0;
		rect1.bottom = this->GetHeaderHeight();
		SetRectRgn(rgnTemp1,rect1.left,rect1.top,rect1.right,rect1.bottom);
		CombineRgn(rgnTemp,rgnTemp,rgnTemp1,RGN_XOR);
		CombineRgn(rgn,rgn,rgnTemp,RGN_OR);
		::InvalidateRgn(GetHWND(),rgn, FALSE);

		DeleteObject(rgnTemp);
		DeleteObject(rgnTemp1);
		DeleteObject(rgnB);
		DeleteObject(rgnD);
		DeleteObject(rgn);
	}

	void CGridFrameSvr::SelectionMoved(TGridRect OldSel)
	{
		TGridRect SelRect = GetSelection();
		this->AreaMoved(OldSel,SelRect);
	}

	int CGridFrameSvr::InitDrawInfo(TGridDrawInfo * pDrawInfo)
	{
		return m_pSheetSvr->InitDrawInfo(pDrawInfo);
	}


	int CGridFrameSvr::CalcScrollBar(int Value, Cardinal ScrollCode, Cardinal ScrollBar, TGridDrawInfo &DrawInfo, int Pos)
	{
		int Result;
		Result = Value;
		switch(ScrollCode) 
		{
		case SB_LINEUP:
			Result = Value - 1;
			break;
		case SB_LINEDOWN:
			Result = Value + 1;
			break;
		case SB_PAGEUP:
			Result = Value - PageUp(ScrollBar,DrawInfo);
			break;
		case SB_PAGEDOWN:
			Result = Value + PageDown(ScrollBar);
			break;
		case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
			if((goThumbTracking && FOptions) || (ScrollCode == SB_THUMBPOSITION))
			{
				TGridCoord MaxTopLeft;
				MaxTopLeft.x = GetColCount() ;
				MaxTopLeft.y = GetRowCount() ;
				MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
				/*Result = ScrollBarMin(ScrollBar) + 
					MulDiv(Pos, ScrollBarMax(ScrollBar,MaxTopLeft)
					- ScrollBarMin(ScrollBar), MaxShortInt);*/
				int nMax = ScrollBar == SB_HORZ? GetHScrollBar()->nMax:GetVScrollBar()->nMax;
				Result = ScrollBar == SB_HORZ? MulDiv(Pos, GetGridDataSvr()->m_pContentSvr->GetValidCol(), nMax):MulDiv(Pos, GetGridDataSvr()->m_pContentSvr->GetValidRow(), nMax);
				break;
			}
		case SB_BOTTOM:
			//Result = ScrollBarMax(ScrollBar);
			Result = ScrollBarMin(ScrollBar);
			break;
		case SB_TOP:
			Result = ScrollBarMin(ScrollBar);
			break;
		}
		return Result;
	}

	void CGridFrameSvr::ModifyScrollBar(Cardinal ScrollBar, Cardinal ScrollCode, Cardinal Pos)
	{
		TGridCoord NewTopLeft, MaxTopLeft;
		TGridDrawInfo DrawInfo;

		if(FScrollBars == ssNone) return;
		if(!(FScrollBars & ssHorizontal) && ScrollBar == SB_HORZ) return;
		if(!(FScrollBars & ssVertical)   && ScrollBar == SB_VERT) return;

		//	if(/*Visible && CanFocus && */TabStop)
		//		m_pView->SendMessage(WM_SETFOCUS,0,0);
		InitDrawInfo(&DrawInfo);
		if(ScrollBar == SB_HORZ)
			DrawInfo.bCalcVert = false;
		else
			DrawInfo.bCalcVert = true;

		if((ScrollBar == SB_HORZ) && (GetColCount() == 1))
		{
			//ModifyPixelScrollBar(ScrollCode, Pos,DrawInfo);
			return;
		}

		MaxTopLeft.x = GetColCount() ;
		MaxTopLeft.y = GetRowCount() ;
		if(MaxTopLeft.y < 1) MaxTopLeft.y=1;
		
		MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
		NewTopLeft = FTopLeft;
		if(ScrollBar == SB_HORZ) 
			NewTopLeft.x = CalcScrollBar(NewTopLeft.x,ScrollCode, ScrollBar, DrawInfo,Pos);
		else
			NewTopLeft.y = CalcScrollBar(NewTopLeft.y,ScrollCode, ScrollBar, DrawInfo,Pos);
		if(NewTopLeft.x < 1)
			NewTopLeft.x =1;
		else if(NewTopLeft.x > MaxTopLeft.x )
			NewTopLeft.x = MaxTopLeft.x;
		if(NewTopLeft.y < 1)
			NewTopLeft.y = 1;
		else if(NewTopLeft.y > MaxTopLeft.y)
			NewTopLeft.y = MaxTopLeft.y;
		if((NewTopLeft.x != FTopLeft.x) || (NewTopLeft.y != FTopLeft.y))
			MoveTopLeft(NewTopLeft.x, NewTopLeft.y);
	}

	void CGridFrameSvr::MoveTopLeft(int ALeft, int ATop)
	{
		TGridCoord OldTopLeft;

		if( (ALeft == FTopLeft.x) && (ATop == FTopLeft.y)) return;
		Update();
		OldTopLeft = FTopLeft;
		FTopLeft.x = ALeft;
		FTopLeft.y = ATop;
		TopLeftMoved(OldTopLeft);
	}

	void CGridFrameSvr::TopLeftMoved(TGridCoord OldTopLeft)
	{

		UpdateScrollPos();

		InvalidateGrid();
		/*
		TGridDrawInfo DrawInfo;
		TGridCoord Delta;
		InitDrawInfo(&DrawInfo);

		bool bcs0,bcs1;
		DrawInfo.bCalcVert=false;
		bcs0 = CalcScroll(DrawInfo, OldTopLeft.x, FTopLeft.x, Delta.x);
		DrawInfo.bCalcVert=true;
		bcs1 = CalcScroll(DrawInfo, OldTopLeft.y, FTopLeft.y, Delta.y);
		if(bcs0 && bcs1) ScrollDataInfo(Delta.x, Delta.y, DrawInfo);
		*/
		TopLeftChanged();
	}

	void CGridFrameSvr::TopLeftChanged()
	{
		//if(FEditorMode && FInplaceEdit) FInplaceEdit->UpdateLoc(CellRect(GetCol(), GetRow()));
	}
	int CGridFrameSvr::CalcMaxCell(TGridDrawInfo &Axis, int Start)
	{
		int Line;
		int Result;

		Result = Start;
		Line = Axis.bCalcVert?Axis.GridRect.bottom:Axis.GridRect.right;
		Line -= Axis.EffectiveLineWidth;
		for(int i= Start;i>=1;i--)
		{
			Line -= Axis.bCalcVert?(Axis.pAxisSvr->GetRowHeight(i)<1?0:Axis.pAxisSvr->GetRowHeight(i)):Axis.pAxisSvr->GetColWidth(i);
			Line -= Axis.EffectiveLineWidth;
			if(!Axis.bCalcVert && Line < Axis.GridRect.left)
				break;
			else if(Axis.bCalcVert && Line < Axis.GridRect.top)
				break;
			Result = i;
		}
		return Result;
	}

	int CGridFrameSvr::ScrollBarMin(Cardinal ScrollBar)
	{
		return 1;
	}

	int CGridFrameSvr::ScrollBarMax(Cardinal ScrollBar,TGridCoord MaxTopLeft)
	{
		return ScrollBar == SB_HORZ? MaxTopLeft.x:MaxTopLeft.y;
	}

	TGridCoord  CGridFrameSvr::CalcMaxTopLeft(TGridCoord Coord, TGridDrawInfo& DrawInfo)
	{
		TGridCoord Result;

		DrawInfo.bCalcVert = false;
		Result.x = CalcMaxCell(DrawInfo, Coord.x);
		DrawInfo.bCalcVert = true;
		Result.y = CalcMaxCell(DrawInfo, Coord.y);

		return Result;
	}

	int CGridFrameSvr::PageUp(Cardinal ScrollBar,TGridDrawInfo& DrawInfo)
	{
		TGridCoord MaxTopLeft;

		int Result;
		MaxTopLeft = CalcMaxTopLeft(FTopLeft, DrawInfo);
		if(ScrollBar == SB_HORZ)
			Result = FTopLeft.x - MaxTopLeft.x; else
			Result = FTopLeft.y - MaxTopLeft.y;
		if(Result < 1) Result = 1;

		return Result;
	}

	int  CGridFrameSvr::PageDown(Cardinal ScrollBar)
	{
		TGridDrawInfo DrawInfo;
		this->InitDrawInfo(&DrawInfo);

		int Result;
		if(ScrollBar == SB_HORZ)
			Result =  DrawInfo.LastFullVisibleCol - FTopLeft.x;
		else
			Result =  DrawInfo.LastFullVisibleRow - FTopLeft.y;

		if(Result < 1)Result = 1;

		return Result;
	}

	bool CGridFrameSvr::EvtVScroll(TEvent *pEvent ,LPARAM lParam)
	{
		//this->HideEdit();
		ModifyScrollBar(SB_VERT, pEvent->scroll.nSBCode, pEvent->scroll.nPos);
		return true;
	}

	bool CGridFrameSvr::EvtHScroll(TEvent *pEvent ,LPARAM lParam)
	{
		//this->HideEdit();
		ModifyScrollBar(SB_HORZ, pEvent->scroll.nSBCode, pEvent->scroll.nPos);
		return true;
	}

	void CGridFrameSvr::UpdateScrollPos()
	{
		TGridDrawInfo DrawInfo;
		TGridCoord MaxTopLeft;

		//int GridSpace, ColWidth;

		if(FScrollBars == ssNone) return;
		InitDrawInfo(&DrawInfo);
		MaxTopLeft.x = GetGridDataSvr()->m_pContentSvr->GetValidCol();
		MaxTopLeft.y = GetGridDataSvr()->m_pContentSvr->GetValidRow() ;//GetRowCount() ;
		MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
		if(FScrollBars & ssHorizontal || FScrollBars &  ssBoth)
		{
			if(GetColCount() == 1) 
			{
				/*ColWidth = GetFColWidths()[DrawInfo.Horz.FirstGridCell];
				GridSpace = GetClientWidth() - DrawInfo.Horz.FixedBoundary;
				if((FColOffset > 0) && (GridSpace > (ColWidth - FColOffset)))
				ModifyScrollBar(SB_HORZ, SB_THUMBPOSITION, ColWidth - GridSpace);
				else
				SetScroll(SB_HORZ, FColOffset);*/
			}
			else
			{
				if(GetHScrollBar())GetHScrollBar()->SetPosition(MulDiv(FTopLeft.x - 1, 100 /*MaxShortInt*/, MaxTopLeft.x - 1));
			}
		}
		else if(FScrollBars & ssVertical || FScrollBars & ssBoth)
			if(GetVScrollBar())GetVScrollBar()->SetPosition(MulDiv(FTopLeft.y - 1, 100 /*MaxShortInt*/, MaxTopLeft.y - 1));
	}

	void CGridFrameSvr::ScrollData(int DX, int DY)
	{
		TGridDrawInfo DrawInfo;

		InitDrawInfo(&DrawInfo);
		ScrollDataInfo(DX, DY, DrawInfo);
	}

	void CGridFrameSvr::ScrollDataInfo(int DX, int DY, TGridDrawInfo &DrawInfo)
	{
		RECT ScrollArea;
		int ScrollFlags;

		//TRect	UpdateRect;
		ScrollFlags = SW_INVALIDATE;
		ScrollFlags = ScrollFlags | SW_ERASE;
		//{ Scroll the area }
		if( DY == 0)
		{
			//   { Scroll both the column titles && data area at the same time }
			SetRect(&ScrollArea, m_pSheetSvr->m_pColHeaderSvr->FRect.left, m_pSheetSvr->m_pColHeaderSvr->FRect.top, FRect.right, FRect.bottom);
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);
		}
		else if( DX == 0)
		{
			//{ Scroll both the row titles && data area at the same time }
			SetRect(&ScrollArea, m_pSheetSvr->m_pSelectorSvr->FRect.left, m_pSheetSvr->m_pSelectorSvr->FRect.top, FRect.right, FRect.bottom);
			::ScrollWindowEx(GetHWND(),0, DY, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);
		}
		else
		{
			//{ Scroll titles && data area separately }
			//{ Column titles }
			ScrollArea =  m_pSheetSvr->m_pColHeaderSvr->FRect;
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);

			//{ Row titles }
			ScrollArea =  m_pSheetSvr->m_pSelectorSvr->FRect;
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);

			//{data Area}
			ScrollArea = FRect;
			::ScrollWindowEx(GetHWND(),DX, DY, &ScrollArea, &ScrollArea, 0,NULL, ScrollFlags);
		}
		UpdateWindow(GetHWND());
	}

	bool CGridFrameSvr::CalcScroll(TGridDrawInfo &AxisInfo, int OldPos, int CurrentPos, int &Amount)
	{
		int Start, Stop;
		bool Result;

		Result = false;

		if(OldPos < CurrentPos)
		{
			Start = OldPos;
			Stop = CurrentPos;
		}
		else
		{
			Start = CurrentPos;
			Stop = OldPos;
		}
		Amount = 0;
		for(int i = Start;i<=Stop - 1;i++)
		{
			Amount += (AxisInfo.bCalcVert?(AxisInfo.pAxisSvr->GetRowHeight(i)>0?AxisInfo.pAxisSvr->GetRowHeight(i):0):AxisInfo.pAxisSvr->GetColWidth(i)) + AxisInfo.EffectiveLineWidth;
			if(Amount > (AxisInfo.bCalcVert? AxisInfo.GridRect.right - AxisInfo.GridRect.left : AxisInfo.GridRect.bottom - AxisInfo.GridRect.top))
			{
				//{ Scroll amount too big, redraw the whole thing }
				InvalidateGrid();
				return Result;
			}
		}
		if(OldPos < CurrentPos)Amount = -Amount;

		return true;
	}

	CXScrollBar * CGridFrameSvr::GetHScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pHScrollBar;
	}
	CXScrollBar * CGridFrameSvr::GetVScrollBar()
	{
		return m_pSheetSvr->m_pSheetCtrl->m_pVScrollBar;
	}

	int CGridFrameSvr::GetVisibleRowCount()
	{
		TGridDrawInfo  DrawInfo;
		this->InitDrawInfo(&DrawInfo);
		return DrawInfo.LastFullVisibleRow - this->FTopLeft.y + 1;
	}
	int CGridFrameSvr::GetVisibleColCount()
	{
		TGridDrawInfo  DrawInfo;
		this->InitDrawInfo(&DrawInfo);
		return DrawInfo.LastFullVisibleCol - this->FTopLeft.x + 1;
	}

	void CGridFrameSvr::InvalidateGrid()
	{
		/*::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->GetHWND(),&m_pSheetSvr->m_pColHeaderSvr->FRect,false);
		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->GetHWND(),&m_pSheetSvr->m_pSelectorSvr->FRect,false);

		::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->GetHWND(),&FRect,false);*/
		if(m_pSheetSvr->m_pSheetCtrl && m_pSheetSvr->m_pSheetCtrl->GetHWND() )
		{
			::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->GetHWND(),NULL,false);
			if(m_pEditSvr->m_pEdit)m_pEditSvr->m_pEdit->Invalidate();
		}
	}

	void CGridFrameSvr::InvalidateCell(int ACol, int ARow)
	{
		TGridRect Rect;

		Rect.top = ARow;
		Rect.left = ACol;
		Rect.bottom = ARow;
		Rect.right = ACol;

		GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,Rect);
		InvalidateRect(Rect);
	}

	void CGridFrameSvr::InvalidateCol(int ACol)
	{
		TGridRect Rect;

		Rect.top = 0;
		Rect.left = ACol;
		Rect.bottom = GetVisibleRowCount()+1;
		Rect.right = ACol;
		InvalidateRect(Rect);
	}

	void CGridFrameSvr::InvalidateRow(int ARow)
	{
		TGridRect Rect;

		Rect.top = ARow;
		Rect.left = 0;
		Rect.bottom = ARow;
		Rect.right = GetVisibleColCount()+1;
		InvalidateRect(Rect);

	}

	void CGridFrameSvr::InvalidateRect(TGridRect ARect)
	{
		RECT rect,rectX,rectY;

		TGridDrawInfo  DrawInfo;
		InitDrawInfo(&DrawInfo);
		BoxRect(DrawInfo,ARect, rect);

		rect.left -= FGridLineWidth *2;
		rect.right += FGridLineWidth *2;
		rect.top -= FGridLineWidth *2;
		rect.bottom += FGridLineWidth *2;

		if(m_pSheetSvr->m_pSheetCtrl)
		{
			RECT rt = rect;
			if(m_pSheetSvr->m_pSheetCtrl->GetHWND() && ::IsWindowVisible(m_pSheetSvr->m_pSheetCtrl->GetHWND()))
			{
				::OffsetRect(&rt,m_pSheetSvr->GetOffset().x,m_pSheetSvr->GetOffset().y);
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->GetHWND(),&rt, FALSE);
			}

			rectX = rect;
			rectX.top = FGridLineWidth;
			rectX.bottom = GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0) + FGridLineWidth;
			if(m_pSheetSvr->m_pSheetCtrl->GetHWND() && ::IsWindowVisible(m_pSheetSvr->m_pSheetCtrl->GetHWND()))
			{
				::OffsetRect(&rectX,m_pSheetSvr->GetOffset().x,m_pSheetSvr->GetOffset().y);
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->GetHWND(),&rectX, FALSE);
			}

			rectY = rect;
			rectY.left = FGridLineWidth;
			rectY.right = GetGridDataSvr()->m_pAxisSvr->GetColWidth(0) + FGridLineWidth;
			if(m_pSheetSvr->m_pSheetCtrl->GetHWND() && ::IsWindowVisible(m_pSheetSvr->m_pSheetCtrl->GetHWND()))
			{
				::OffsetRect(&rectY,m_pSheetSvr->GetOffset().x,m_pSheetSvr->GetOffset().y);
				::InvalidateRect(m_pSheetSvr->m_pSheetCtrl->GetHWND(),&rectY, FALSE);
			}

			if(m_pEditSvr->m_pEdit)m_pEditSvr->m_pEdit->Invalidate();
		}
	}

	int CGridFrameSvr::LinePos(TGridDrawInfo& AxisInfo ,int  Line)
	{
		int Start;
		int End;

		int Result  ;

		if(!AxisInfo.bCalcVert)
		{
			Start = Line > AxisInfo.FTopLeft.x? AxisInfo.FTopLeft.x:Line;
			End = (Line < AxisInfo.FTopLeft.x? AxisInfo.FTopLeft.x:Line) ;
		}else
		{
			Start = Line > AxisInfo.FTopLeft.y? AxisInfo.FTopLeft.y:Line;
			End = (Line < AxisInfo.FTopLeft.y? AxisInfo.FTopLeft.y:Line);
		}
		//Result = !AxisInfo.bCalcVert ?AxisInfo.pAxisSvr->m_pColWidths->SumValues(Start,End - 1) :AxisInfo.pAxisSvr->m_pRowHeights->SumValues(Start,End - 1);
		//Result += AxisInfo.EffectiveLineWidth *( End - Start);
		Result = !AxisInfo.bCalcVert ?AxisInfo.pAxisSvr->m_pColWidths->SumValues(Start,End - 1) :AxisInfo.pAxisSvr->GetSumRowHeight(Start,End - 1);

		if(AxisInfo.bCalcVert)
		{
			int i;
			for(i=Start; i<End; i++)
			{
				int RowHeight = GetRowHeight(i);
				if(RowHeight > 0)
					Result += AxisInfo.EffectiveLineWidth;
			}
		}
		else
			Result += AxisInfo.EffectiveLineWidth *( End - Start);

		if(Start == Line)
		{
			Result = (AxisInfo.bCalcVert?AxisInfo.GridRect.top:AxisInfo.GridRect.left) + AxisInfo.EffectiveLineWidth - Result;
		}else
		{
			Result = (AxisInfo.bCalcVert?AxisInfo.GridRect.top:AxisInfo.GridRect.left) + AxisInfo.EffectiveLineWidth + Result;
		}

		return Result;
	}

	bool CGridFrameSvr::GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan)
	{
		TGridDrawInfo AxisInfo;
		InitDrawInfo(&AxisInfo);
		return GetCellRect(AxisInfo,ACol,ARow,rect,bIncludeSpan);
	}

	bool CGridFrameSvr::GetCellRect(TGridDrawInfo& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan)
	{
		::SetRect(&rect,0,0,0,0);

		CAxisSvr * pOldAxisSvr = AxisInfo.pAxisSvr;
		CAxisSvr * pNewAxisSvr = GetGridDataSvr()->GetAxisSvr(ARow);
		AxisInfo.pAxisSvr = pNewAxisSvr;

		AxisInfo.bCalcVert = false;
		rect.left = this->LinePos(AxisInfo,ACol);
		AxisInfo.bCalcVert = true;
		rect.top = this->LinePos(AxisInfo,ARow);

		rect.right  = rect.left + pNewAxisSvr->GetColWidth(ACol);
		rect.bottom = rect.top  + (pNewAxisSvr->GetRowHeight(ARow)>0?pNewAxisSvr->GetRowHeight(ARow):0);

		TGridRect	tRect;
		if(bIncludeSpan && GetGridDataSvr()->m_pSpanSvr->isSpaned(ACol,ARow,tRect))
		{
			RECT eRect;
			GetCellRect(AxisInfo,tRect.right,tRect.bottom,eRect,false);
			rect.right = eRect.right;
			rect.bottom = eRect.bottom;
		}
		AxisInfo.pAxisSvr = pNewAxisSvr = pOldAxisSvr;

		return true;
	}

	bool CGridFrameSvr::BoxRect(TGridDrawInfo& AxisInfo,TGridRect gridRect,RECT &screenRect)
	{
		RECT	sRect,	eRect;

		::SetRect(&screenRect,0,0,0,0);
		NormalizeRect(*(RECT *)&gridRect);
		GetCellRect(AxisInfo,gridRect.left,gridRect.top,sRect,false);
		GetCellRect(AxisInfo,gridRect.right,gridRect.bottom,eRect,false);

		::SetRect(&screenRect,sRect.left,sRect.top,eRect.right,eRect.bottom);
		return true;

	}
	bool CGridFrameSvr::CalcSpanRgn(HRGN & hRgn,TGridDrawInfo& AxisInfo)
	{
		int LineX,LineY;
		bool InitRgn = true;
		TGridRect sRect;
		RECT rect;
		LineY = AxisInfo.GridRect.top;
		for(int i=AxisInfo.FTopLeft.y ;i<=AxisInfo.LastFullVisibleRow;i++)
		{
			LineX = AxisInfo.GridRect.left;
			for(int j=AxisInfo.FTopLeft.x ;j<=AxisInfo.LastFullVisibleCol;j++)
			{
				//calc span Rect;
				if(GetGridDataSvr()->m_pSpanSvr->isSpaned(j,i,sRect))
				{
					BoxRect(AxisInfo,sRect,rect);
					if(InitRgn) 
					{
						SetRectRgn(hRgn,rect.left,rect.top,rect.right,rect.bottom);
						InitRgn = false;
					}else
					{
						HRGN hRgnRect = ::CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
						CombineRgn(hRgn,hRgn,hRgnRect,RGN_OR);
						DeleteObject(hRgnRect);
					}
				}
			}
		}
		return InitRgn?false:true;
	}

	bool CGridFrameSvr::CellChooseFont()
	{
		CHOOSEFONT cf; 
		LOGFONT	   logFont;
		COLORREF	Color = -1;

		TGridRect rect = GetSelection();
		const XUtil::Css::TBoxStyle * pBoxStyle =  GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(FCurrent.x,FCurrent.y);
		ZeroMemory(&cf,sizeof(CHOOSEFONT));

		XUtil::Css::GetFont(pBoxStyle,logFont);
		if(pBoxStyle && pBoxStyle->mask & cssMaskColor)
		{
			Color = pBoxStyle->pColor->color[0];
		}

		cf.lStructSize = sizeof(CHOOSEFONT); 
		cf.hwndOwner = GetParent(m_pSheetSvr->m_pSheetCtrl->GetHWND()); 
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
				(Color == -1 && cf.rgbColors != COptionSvr::stdTextColor))
				this->SetFont(GetSelection(),*cf.lpLogFont,cf.rgbColors);
			else
				this->SetFont(GetSelection(),*cf.lpLogFont,-1);

			return TRUE;
		}
		return FALSE;
	}

	void CGridFrameSvr::SetFont(TGridRect cellRect,LOGFONT &logFont)
	{
		SetFont(cellRect,logFont,-1);
	}

	void CGridFrameSvr::SetFont(TGridRect cellRect,LOGFONT &logFont,COLORREF Color)
	{
		bool bCurrent=false;
		for(int i = cellRect.left;i<=cellRect.right;i++)
		{
			for(int j = cellRect.top;j<=cellRect.bottom;j++)
			{
				XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle;
				if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
					*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
				XUtil::Css::SetFont(pBoxStyle,logFont);
				if((int)Color !=-1)
				{
					if(!(pBoxStyle->pColor))
					{
						pBoxStyle->mask |= cssMaskColor;
						pBoxStyle->pColor = new XUtil::Css::TColor;
					}
					pBoxStyle->pColor->colorRef[0] = XUtil::Css::_rgbcolor;
					pBoxStyle->pColor->color[0] = Color;
				}
				GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);

				if(i == FCurrent.x && j ==FCurrent.y)bCurrent = TRUE;
				if(pBoxStyle)delete pBoxStyle;
			}
		}
		if(m_pEditSvr->m_pEdit && bCurrent)
		{
			m_pEditSvr->m_pEdit->SetFont(logFont,TRUE);
		}
		InvalidateRect(cellRect);
	}

	void CGridFrameSvr::ClearColor(TGridRect cellRect, DWORD flag)
	{
		for(int i = cellRect.left;i<=cellRect.right;i++)
		{
			for(int j = cellRect.top;j<=cellRect.bottom;j++)
			{
				XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle;
				if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
					*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
				if(flag & STI_TEXTCOLOR)
				{
					if(pBoxStyle->mask & cssMaskColor)
					{
						pBoxStyle->mask = pBoxStyle->mask &~ cssMaskColor;
					}
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}else if(flag & STI_BKCOLOR)
				{
					if(pBoxStyle->mask & cssMaskBackGround)
					{
						pBoxStyle->mask = pBoxStyle->mask &~ cssMaskBackGround;
					}
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}
				if(pBoxStyle) delete pBoxStyle;
			}
		}
		this->InvalidateRect(cellRect);
	}

	void CGridFrameSvr::SetColor(TGridRect cellRect, COLORREF color, DWORD flag)
	{
		for(int i = cellRect.left;i<=cellRect.right;i++)
		{
			for(int j = cellRect.top;j<=cellRect.bottom;j++)
			{
				XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle;
				if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
					*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
				if(flag & STI_TEXTCOLOR)
				{
					if(!(pBoxStyle->pColor))
					{
						pBoxStyle->mask |= cssMaskColor;
						pBoxStyle->pColor = new XUtil::Css::TColor;
					}
					pBoxStyle->pColor->colorRef[0] = XUtil::Css::_rgbcolor;
					pBoxStyle->pColor->color[0] = color;
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
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
					pBoxStyle->pBackGround->color.mask = cssMaskColor;
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}
				if(pBoxStyle) delete pBoxStyle;
				//if(flag & STI_TEXTCOLOR)pItem->FTextColor = color;
				//if(flag & STI_TEXTBKCOLOR)pItem->FTextBkColor = color;
			}
		}
		this->InvalidateRect(cellRect);
	}
	bool CGridFrameSvr::CellChooseColor()
	{
		if(::GetKeyState(VK_SHIFT)<0) return CellChooseColor(STI_TEXTBKCOLOR);
		if(::GetKeyState(VK_CONTROL)<0) return CellChooseColor(STI_BKCOLOR);
		return CellChooseColor(STI_TEXTCOLOR);
	}

	bool CGridFrameSvr::CellChooseColor(DWORD flag)
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
		cc.hwndOwner = GetParent(m_pSheetSvr->m_pSheetCtrl->GetHWND()); 
		cc.hInstance = 0; 
		cc.rgbResult = 0; 
		cc.lpCustColors = colors; 
		cc.Flags = CC_RGBINIT; 
		cc.lCustData = 0; 
		cc.lpfnHook = NULL; 
		cc.lpTemplateName = NULL;

		if(::ChooseColor(&cc))
		{
			this->SetColor(GetSelection(),cc.rgbResult,flag);
			return TRUE;
		}
		return FALSE;
	}

	void CGridFrameSvr::SetAlign(XUtil::Css::TextAlign align)
	{
		TGridRect rect = GetSelection();
		SetAlign(rect,align);
	}

	void CGridFrameSvr::SetAlign(TGridRect rect, XUtil::Css::TextAlign align)
	{
		for(int i=rect.left;i<=rect.right;i++)
		{
			for(int j=rect.top;j<=rect.bottom;j++)
			{
				XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
				if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
					*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
				if(!(pBoxStyle->mask & cssMaskText))
				{
					pBoxStyle->pText = new XUtil::Css::TText;
					pBoxStyle->mask |= cssMaskText;
				}

				pBoxStyle->pText->mask |= cssMaskTextAlign;
				pBoxStyle->pText->textalign = align;
				GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				if(pBoxStyle)delete pBoxStyle;
			}
		}
		this->InvalidateRect(rect);
	}

	void CGridFrameSvr::SetBorder(DWORD nBorder)
	{
		TGridRect rect = GetSelection();
		this->SetBorder(rect,nBorder);		
	}

	void CGridFrameSvr::SetBorder(TGridRect ARect,DWORD nBorder)
	{
		TGridRect rect = ARect;
		for(int i=rect.left;i<=rect.right;i++)
		{
			for(int j=rect.top;j<=rect.bottom;j++)
			{
				XUtil::Css::TBoxStyle * pBoxStyle = new XUtil::Css::TBoxStyle();
				if(GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j))
					*pBoxStyle = *GetGridDataSvr()->GetStyleSvr()->GetCellCssStyle(i,j);
				if(nBorder==BORDER_ALL)
				{
					pBoxStyle->mask |= cssMaskBorder;
					if(!pBoxStyle->pBorder[0])
						pBoxStyle->pBorder[0] = new XUtil::Css::TBorder;
					pBoxStyle->pBorder[0]->mask = cssMaskBorderType;
					pBoxStyle->pBorder[0]->bordertype = XUtil::Css::_solid;
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}
				if(nBorder==BORDER_NONE)
				{
					pBoxStyle->mask &= ~cssMaskBorder;
					for(int k=0;k<5;k++)
					{
						if(!pBoxStyle->pBorder[k])
							delete pBoxStyle->pBorder[k];
						pBoxStyle->pBorder[k] = NULL;
						GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
					}
				}
				if(nBorder==BORDER_LEFT)
				{
					pBoxStyle->mask |= cssMaskBorder;
					if(!pBoxStyle->pBorder[1])
						pBoxStyle->pBorder[1] = new XUtil::Css::TBorder;
					pBoxStyle->pBorder[1]->mask = cssMaskBorderType;
					pBoxStyle->pBorder[1]->bordertype = XUtil::Css::_solid;
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}
				if(nBorder==BORDER_TOP)
				{
					pBoxStyle->mask |= cssMaskBorder;
					if(!pBoxStyle->pBorder[2])
						pBoxStyle->pBorder[2] = new XUtil::Css::TBorder;
					pBoxStyle->pBorder[2]->mask = cssMaskBorderType;
					pBoxStyle->pBorder[2]->bordertype = XUtil::Css::_solid;
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}

				if(nBorder==BORDER_RIGHT)
				{
					pBoxStyle->mask |= cssMaskBorder;
					if(!pBoxStyle->pBorder[3])
						pBoxStyle->pBorder[3] = new XUtil::Css::TBorder;
					pBoxStyle->pBorder[3]->mask = cssMaskBorderType;
					pBoxStyle->pBorder[3]->bordertype = XUtil::Css::_solid;
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}
				if(nBorder==BORDER_BOTTOM)
				{
					pBoxStyle->mask |= cssMaskBorder;
					if(!pBoxStyle->pBorder[4])
						pBoxStyle->pBorder[4] = new XUtil::Css::TBorder;
					pBoxStyle->pBorder[4]->mask = cssMaskBorderType;
					pBoxStyle->pBorder[4]->bordertype = XUtil::Css::_solid;
					GetGridDataSvr()->GetStyleSvr()->SetCellCssStyle(i,j,pBoxStyle);
				}
				if(nBorder==BORDER_OUTER)
				{

				}

				if(pBoxStyle)delete pBoxStyle ;
			}
		}
		this->InvalidateRect(rect);
	}

	void CGridFrameSvr::ShowGrid()
	{
		if(FOptions & goVertLine || FOptions & goHorzLine)
		{
			FOptions = FOptions&(0xffffffff^goVertLine)&(0xffffffff^goHorzLine);
		}
		else
		{
			FOptions = FOptions|goVertLine|goHorzLine;
		}
		InvalidateGrid();
	}

	void CGridFrameSvr::ShowGrid(bool abShow)
	{
		if(abShow){
			FOptions = FOptions|goVertLine|goHorzLine;
		}
		else
		{
			FOptions = FOptions&(0xffffffff^goVertLine)&(0xffffffff^goHorzLine);
		}
		InvalidateGrid();
	}

	void CGridFrameSvr::ResizeRow(int Index, int OldSize, int NewSize)
	{
		InvalidateGrid();
	}

	void CGridFrameSvr::ResizeCol(int Index, int OldSize, int NewSize)
	{
		InvalidateGrid();
	}

	void CGridFrameSvr::RowHeightsChanged()
	{
		
		//UpdateScrollRange();
		UpdateEdit();
		/**this->UpdateObject();**/
	}

	void CGridFrameSvr::ColWidthsChanged()
	{
		//UpdateScrollRange();
		UpdateEdit();
		/**this->UpdateObject();**/
	}

	int CGridFrameSvr::InsertRow(int ARow)
	{	
		if(GetGridDataSvr()->m_enView==CGridDataSvr::envGridReportDesign || GetGridDataSvr()->m_enView==CGridDataSvr::envPrintDesign)
		{
			if(!GetGridDataSvr()->m_ReportBand->InsertRow(ARow)) return -1;
		}
		this->AdjustSize(GetRowCount(),1,TRUE);
		BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);
		MoveRow(GetRowCount() - 1,ARow);
		if(ARow > 1)
		{
			this->GetGridDataSvr()->GetStyleSvr()->InsertRow(ARow,1);
			if(bCtrl && this->GetGridDataSvr()->m_pContentSvr) this->GetGridDataSvr()->m_pContentSvr->CopyTo(ARow - 1,ARow);
			this->SetRowHeight(ARow,this->GetRowHeight(ARow - 1));
		}else
		{
			this->GetGridDataSvr()->GetStyleSvr()->InsertRow(0,1);
		}
		CDwTable::AdjustDwTables(this,ARow,1);
		int nIndex = this->GetGridDataSvr()->GetChildIndex(ARow);
		if(nIndex>=0)
			(*GetGridDataSvr()->m_pChilds)[nIndex]->endRow += 1;
		return ARow;
	}

	void CGridFrameSvr::DeleteRow(int ARow)
	{
		if(GetGridDataSvr()->m_enView==CGridDataSvr::envGridReportDesign || GetGridDataSvr()->m_enView==CGridDataSvr::envPrintDesign)
		{
			if(!GetGridDataSvr()->m_ReportBand->DeleteRow(ARow)) return;
		}
		MoveRow(ARow, GetRowCount() - 1);
		m_RowCount = GetRowCount() - 1;
		MoveCurrent(this->FCurrent.x,ARow,TRUE,TRUE);
		if(ARow >=1)
			this->GetGridDataSvr()->GetStyleSvr()->RemoveRow(ARow,1);

		CDwTable::AdjustDwTables(this,ARow,-1);

		int nIndex = this->GetGridDataSvr()->GetChildIndex(ARow);
		if(nIndex>=0)
		{
			(*GetGridDataSvr()->m_pChilds)[nIndex]->endRow -= 1;
			if((*GetGridDataSvr()->m_pChilds)[nIndex]->endRow < 
				(*GetGridDataSvr()->m_pChilds)[nIndex]->startRow)
				GetGridDataSvr()->m_pChilds->erase(GetGridDataSvr()->m_pChilds->begin() + nIndex);
		}
	}

	int CGridFrameSvr::GetMinHeight(int ARow)
	{
		return 0;
	}

	int CGridFrameSvr::GetMinWidth(int ACol)
	{
		return 0;
	}

	void	CGridFrameSvr::SetRowHeight(int Index,int value)
	{
		if(value == GetRowHeight(Index)) return;
		if(value < this->GetMinHeight(Index)) value = this->GetMinHeight(Index);

		ResizeRow(Index, GetRowHeight(Index), value);
		this->GetGridDataSvr()->m_pAxisSvr->SetRowHeight(Index,value);
		RowHeightsChanged();
	}

	void	CGridFrameSvr::SetColWidth(int Index,int value)
	{
		if(value == GetColWidth(Index)) return;
		if(value < this->GetMinWidth(Index)) value = this->GetMinWidth(Index);

		ResizeCol(Index, GetColWidth(Index), value);
		this->GetGridDataSvr()->m_pAxisSvr->SetColWidth(Index,value);
		ColWidthsChanged();

	}

	int	CGridFrameSvr::GetRowHeight(int Index)
	{
		return GetGridDataSvr()->m_pAxisSvr->GetRowHeight(Index);
	}

	int	CGridFrameSvr::GetColWidth(int Index)
	{
		return GetGridDataSvr()->m_pAxisSvr->GetColWidth(Index);
	}

	void CGridFrameSvr::MoveRow(int FromIndex, int ToIndex)
	{
		if(FromIndex==ToIndex) return;
		RECT rect;
		if(ToIndex != GetRowCount() - 1 && FromIndex != GetRowCount() - 1)
		{
			for(int i=0;i<GetGridDataSvr()->m_pSpanSvr->SpanRects.size();i++)
			{
				rect = GetGridDataSvr()->m_pSpanSvr->SpanRects[i];
				if(rect.top !=rect.bottom &&  rect.top<=FromIndex && rect.bottom>=FromIndex)
				{
					FGridState = gsNormal;
					MessageBox(NULL,_T("不能对合并单元格作部分改动"),_T("提示!"),MB_OK);
					return;
				}
				if(FromIndex < ToIndex)
				{
					if(rect.top<=ToIndex && rect.bottom>ToIndex)
					{
						FGridState = gsNormal;
						MessageBox(NULL,_T("不能对合并单元格作部分改动"),_T("提示!"),MB_OK);
						return;
					}
				}else
				{
					if(rect.top<ToIndex && rect.bottom>=ToIndex)
					{
						FGridState = gsNormal;
						MessageBox(NULL,_T("不能对合并单元格作部分改动"),_T("提示!"),MB_OK);
						return;
					}
				}
			}
		}
		if(FromIndex == GetRowCount() - 1)	//INSERTROW
		{
			GetGridDataSvr()->m_pSpanSvr->InsertRow(ToIndex,1,true);
		}
		if(ToIndex == GetRowCount() - 1)	//DELETEROW
		{
			GetGridDataSvr()->m_pSpanSvr->DeleteRow(FromIndex);
		}

		if(ToIndex != GetRowCount() - 1 && FromIndex != GetRowCount() - 1) //move
		{
			GetGridDataSvr()->m_pSpanSvr->MoveRow(FromIndex,ToIndex);
		}
		GetGridDataSvr()->m_pSpanSvr->RefreshSpans();

		TGridRect Rect;
		if(ToIndex > FromIndex)
		{
			GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Insert(ToIndex,1);
			GetGridDataSvr()->m_pAxisSvr->SetRowHeight(ToIndex, GetGridDataSvr()->m_pAxisSvr->GetRowHeight(FromIndex));
			GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Remove(FromIndex,1);
		}else
		{
			int extent = GetGridDataSvr()->m_pAxisSvr->GetRowHeight(FromIndex);
			GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Remove(FromIndex,1);
			GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Insert(ToIndex,1);
			GetGridDataSvr()->m_pAxisSvr->SetRowHeight(ToIndex,extent );
		}

		MoveAdjust(FCurrent.y, FromIndex, ToIndex);
		MoveAdjust(FAnchor.y, FromIndex, ToIndex);
		MoveAdjust(FInplaceRow, FromIndex, ToIndex); //move edit
		MoveRowData(FromIndex, ToIndex);
		RowMoved(FromIndex, ToIndex);


		if(ToIndex == GetRowCount() - 1) //deleterow
		{
			//move delete buffer
			/**
			if(this->GetGridDataSvr()->bDwGrid && this->GetCellsItem(SC_IDCOLUMN,GetRowCount() - 1))
			{
			TSparseList *ssRow;
			ssRow = (TSparseList*)this->GetGridDataSvr()->FDeleteBuffer;
			ssRow->Put(ssRow->GetHighBound()>=0?ssRow->GetHighBound():1 ,((TSparseList *)GetFData())->Get(GetRowCount() - 1));
			((TSparseList *)GetFData())->Put(GetRowCount() - 1,NULL);
			}else
			{**/
			this->GetGridDataSvr()->m_pContentSvr->ClearRowData(GetRowCount() - 1);/**
																			  }
																			  */
		}

		TGridDrawInfo  DrawInfo;
		InitDrawInfo(&DrawInfo);

		Rect.left = FTopLeft.x;
		Rect.right = DrawInfo.LastFullVisibleCol + 1;
		if(FromIndex < ToIndex )
		{
			Rect.top = FromIndex;
			Rect.bottom = ToIndex;
		}
		else
		{
			Rect.top = ToIndex;
			Rect.bottom = FromIndex;
		}
		InvalidateRect(Rect);

		RowHeightsChanged();
		UpdateEdit();
		/**this->UpdateObject();**/
	}

	void CGridFrameSvr::MoveAdjust(int &CellPos, int FromIndex, int ToIndex)
	{
		int Min, Max;

		if(CellPos == FromIndex) CellPos = ToIndex;
		else
		{
			Min = FromIndex;
			Max = ToIndex;
			if(FromIndex > ToIndex)
			{
				Min = ToIndex;
				Max = FromIndex;
			}
			if((CellPos >= Min) && (CellPos <= Max))
				if(FromIndex > ToIndex)
					CellPos++ ; else
					CellPos --;
		}
	}

	void CGridFrameSvr::ColumnMoved(int FromIndex, int ToIndex)
	{
	}

	void CGridFrameSvr::RowMoved(int FromIndex, int ToIndex)
	{

	}

	void CGridFrameSvr::RowMoved(int FromIndex, int ToIndex,int Number)
	{
	}


	void CGridFrameSvr::MoveRowData(int FromIndex, int ToIndex)
	{
		this->GetGridDataSvr()->m_pContentSvr->MoveRow(FromIndex,ToIndex);
	}

	void CGridFrameSvr::MoveRowData(int FromIndex, int ToIndex,int Number)
	{
		this->GetGridDataSvr()->m_pContentSvr->MoveRow(FromIndex,ToIndex,Number);
	}


	void CGridFrameSvr::AdjustSize(int Index, int Amount, BOOL Rows)
	{
		TGridCoord NewCur;
		int OldRows, OldCols;
		int MovementX, MovementY;
		TGridRect MoveRect;
		RECT ScrollArea;
		int AbsAmount;

		if(Amount == 0) return ;
		NewCur  = FCurrent;
		OldCols = GetColCount();
		OldRows = GetRowCount();
		MoveRect.left = FFixedCols;
		MoveRect.right = GetColCount() - 1;
		MoveRect.top = FFixedRows;
		MoveRect.bottom = GetRowCount() - 1;
		MovementX = 0;
		MovementY = 0;
		AbsAmount = Amount;
		if(AbsAmount < 0) AbsAmount = -AbsAmount;
		if(Rows)
		{
			MovementY = DoSizeAdjust(GetRowCount(), *GetGridDataSvr()->m_pAxisSvr->m_pRowHeights, COptionSvr::stdRowHeight/*FDefaultRowHeight*/, NewCur.y,Amount,Index);
			MoveRect.top = Index;
			if(Index + AbsAmount <= FTopLeft.y) MoveRect.bottom = FTopLeft.y - 1;
		}
		else
		{
			MovementX = DoSizeAdjust(GetColCount(), *GetGridDataSvr()->m_pAxisSvr->m_pColWidths , COptionSvr::stdColumnWidth/*FDefaultColWidth*/, NewCur.x,Amount,Index);
			MoveRect.left = Index;
			if(Index + AbsAmount <= FTopLeft.x)  MoveRect.right = FTopLeft.x - 1;
		}

		TGridDrawInfo  DrawInfo;
		InitDrawInfo(&DrawInfo);
		this->BoxRect(DrawInfo,MoveRect, ScrollArea);
		if(!IsRectEmpty(&ScrollArea)) 
		{
			::ScrollWindow(GetHWND(),MovementX, MovementY, &ScrollArea, &ScrollArea);
			::UpdateWindow(GetHWND());
		}
		SizeChanged(OldCols, OldRows);
		if((NewCur.x != FCurrent.x) || (NewCur.y != FCurrent.x))
			MoveCurrent(NewCur.x, NewCur.y, TRUE, TRUE);
	}

	void CGridFrameSvr::SizeChanged(int OldColCount, int OldRowCount)
	{

	}

	int CGridFrameSvr::DoSizeAdjust(int Count, XUtil::xuSummableCsArray< XCROW, USHORT> &Extents, int DefaultExtent, int Current, int Amount, int Index)
	{
		int NewCount;
		int Result;

		NewCount = Count + Amount;
		if(Amount < 0 )
		{
			Result = 0;
			for(int i = Index;i<= Index - Amount - 1;i++)
				Result += Extents.GetValue(i);
		}
		else
			Result = Amount * DefaultExtent;
		Extents.SetValue(Index, Index + Amount, DefaultExtent);
		Count = NewCount;
		if(Current >= Index)
		{
			if((Amount < 0) && (Current < Index - Amount)) Current = Index;
		}
		else 
			Current += Amount;

		return Result;
	}

	int CGridFrameSvr::GetHeaderHeight()
	{
		return this->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0);
	}

	int CGridFrameSvr::GetHeaderWidth()
	{
		return this->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
	}

	void CGridFrameSvr::CalcPageExtents(TGridDrawInfo &DrawInfo, int &PageWidth, int &PageHeight)
	{
		InitDrawInfo(&DrawInfo);
		PageWidth = DrawInfo.LastFullVisibleCol - this->GetLeftCol();
		if(PageWidth < 1) PageWidth = 1;
		PageHeight = DrawInfo.LastFullVisibleRow - this->GetTopRow();
		if(PageHeight < 1)PageHeight = 1;
	}

	void Restrict(TGridCoord &Coord, long MinX, long MinY, long MaxX, long MaxY)
	{
		if( Coord.x > MaxX) Coord.x  = MaxX;
		else if( Coord.x  < MinX) Coord.x  = MinX;
		if( Coord.y > MaxY) Coord.y = MaxY;
		else if( Coord.y < MinY) Coord.y = MinY;
	}

	bool CGridFrameSvr::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		TGridCoord NewTopLeft, NewCurrent, MaxTopLeft;
		TGridDrawInfo DrawInfo;
		int PageWidth, PageHeight;
		UINT Key;

		/**
		if(nChar == VK_DELETE && this->m_selection.GetCount() > 0)
		{
		DeleteSelection();
		return;
		}**/

		Key = nChar;
		TShiftState Shift = 0;
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
		if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;

		if( ! CanGridAcceptKey(Key, Shift)) Key = 0;
		NewCurrent = FCurrent;
		NewTopLeft = FTopLeft;

		CalcPageExtents(DrawInfo,PageWidth,PageHeight);
		if(ssCtrl & Shift)
		{
			switch(Key)
			{
			case VK_UP: NewTopLeft.y --;break;
			case VK_DOWN: NewTopLeft.y++;break;
			case VK_LEFT:
				if( ! (goRowSelect & FOptions))
				{
					NewCurrent.x -= PageWidth;
					NewTopLeft.x -= PageWidth;
				}
				break;
			case VK_RIGHT:
				if( ! (goRowSelect & FOptions))
				{
					NewCurrent.x +=PageWidth;
					NewTopLeft.x +=PageWidth;
				}
				break;
			case VK_PRIOR: NewCurrent.y = GetTopRow();break;
			case VK_NEXT: NewCurrent.y = DrawInfo.LastFullVisibleRow;break;
			case VK_HOME:
				{
					NewCurrent.x = FFixedCols;
					NewCurrent.y = FFixedRows;
				}
				break;
			case VK_END:
				{
					NewCurrent.x = GetColCount() - 1;
					NewCurrent.y = GetRowCount() - 1;
				}
				break;
			}
		}
		else
		{
			switch(Key)
			{
			case VK_UP: NewCurrent.y--;break;
			case VK_DOWN: NewCurrent.y++;break;
			case VK_LEFT:
				{
					if( goRowSelect & FOptions)
						NewCurrent.y--; else
						NewCurrent.x--;break;
				}
			case VK_RIGHT:
				if( goRowSelect & FOptions)
					NewCurrent.y++; else
					NewCurrent.x++;break;
			case VK_NEXT:
				{
					NewCurrent.y+=PageHeight;
					NewTopLeft.y +=PageHeight;
					break;}
			case VK_PRIOR:
				{
					NewCurrent.y -= PageHeight;
					NewTopLeft.y -= PageHeight;
					if(NewTopLeft.y < 1) NewTopLeft.y=1;
					break;
				}
			case VK_HOME:
				if( goRowSelect & FOptions)
					NewCurrent.y = FFixedRows; else
					NewCurrent.x = FFixedCols;
				break;
			case VK_END:
				if( goRowSelect & FOptions)
					NewCurrent.y = GetRowCount() - 1; else
					NewCurrent.x = GetColCount() - 1;
				break;
			case VK_TAB:
				{
				int dX = NewCurrent.x + 1;
				int dY = NewCurrent.y;
				PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(dX, dY);
				while(true)
				{
					if(pItem && pItem->pStrColumn && pItem->pStrGroup)
					{
						if(!pItem->pStrXml) break;
						xbXml xml;
						xml.LoadXml(pItem->pStrXml);
						IXMLDOMNode * pNode;
						IXMLDOMElement * pElement;
						_variant_t var;
						xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
						if(pNode)
						{
							pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
							pElement->getAttribute(L"cellprotect",&var);
							if(var.bstrVal)
							{
								if(_tcscmp(var.bstrVal,_T("保护"))!=0)									
								{
									pElement->getAttribute(L"cellvisible",&var);
									if(var.bstrVal)
										if(_tcscmp(var.bstrVal,_T("不可见"))!=0)
											break;
								}
							}
						}
					}
					if(dX <= this->GetVisibleColCount()) 
						dX++;
					else
					{
						dY++;
						dX = 1;
					}
					if(dY > this->GetVisibleRowCount())
					{
						dX = 1;
						dY = 1;	
					}
					if(dX == NewCurrent.x && dY == NewCurrent.y)
						break;
					if(pItem) pItem = NULL;
					pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(dX, dY);
				}
				NewCurrent.y = dY;
				NewCurrent.x = dX;
/*
				if( ! (ssAlt & Shift))
					do
					{
						if( ssShift && Shift)
						{
							NewCurrent.x --;
							if( NewCurrent.x < GetFixedCols())
							{
								NewCurrent.x = GetColCount() - 1;
								NewCurrent.y --;
								if( NewCurrent.y < GetFixedRows()) NewCurrent.y =  GetRowCount() - 1;
							}
							Shift = 0;
						}
						else
						{
							NewCurrent.x++;
							if( NewCurrent.x >= GetColCount())
							{
								NewCurrent.x = GetFixedCols();
								NewCurrent.y++;
								if( NewCurrent.y >=  GetRowCount()) NewCurrent.y = GetFixedRows();
							}
						}
					}while(!(GetTabStops(NewCurrent.x) || NewCurrent.x == FCurrent.x));
*/
				break;}
			case VK_F2: FEditorMode = TRUE;break;
			}
		}

		if(FOptions & goRunState)
		{
			CDwTable * pCurTable = this->GetCurDwTable();
			if(!pCurTable) return true;

			if(pCurTable)
			{
				int nRow,nCol;
				pCurTable->getRowColumnFromCell(FCurrent.x,FCurrent.y,nRow,nCol);
				if(Key==VK_LEFT)
				{
					int nNewCol = pCurTable->getPirorAllowEditColumn(nRow,nCol);
					if(nNewCol < 1)
					{
						if(nRow > 1)nNewCol = pCurTable->getPirorAllowEditColumn(nRow - 1,pCurTable->getColumnCount() + 1);
						if(nNewCol < 1)return true;
						nRow--;
					}
					pCurTable->getCellFromRowColumn(nRow,nNewCol,NewCurrent.x,NewCurrent.y);
				}else if(Key==VK_RIGHT)
				{
					int nNewCol = pCurTable->getNextAllowEditColumn(nRow,nCol);
					if(nNewCol < 1)
					{
						if(nRow < pCurTable->GetRowCount())nNewCol = pCurTable->getNextAllowEditColumn(nRow + 1,0);
						if(nNewCol < 1)return true;
						nRow++;
					}
					pCurTable->getCellFromRowColumn(nRow,nNewCol,NewCurrent.x,NewCurrent.y);
				}else if(Key==VK_UP)
				{
					int nNewCol = pCurTable->getUpAllowEditColumn(nRow,nCol);
					if(nNewCol < 1)
					{
						if(nRow > 1)
						{
							nNewCol = nCol;
							nRow--;
						}
						if(nNewCol < 1)return true;
					}
					pCurTable->getCellFromRowColumn(nRow,nNewCol,NewCurrent.x,NewCurrent.y);
				}else if(Key==VK_DOWN)
				{
					int nNewCol = pCurTable->getDownAllowEditColumn(nRow,nCol);
					if(nNewCol < 1)
					{
						if(nRow < pCurTable->GetRowCount())
						{
							nNewCol = nCol;
							nRow++;
						}
						if(nNewCol < 1)return true;
					}
					pCurTable->getCellFromRowColumn(nRow,nNewCol,NewCurrent.x,NewCurrent.y);
				}
				else if(Key==VK_RETURN)
				{
					bool bShowButton;
					bool bQuery = this->IsEditQuery(FCurrent.x,FCurrent.y,bShowButton);
					if(bQuery && this->m_pEditSvr && this->m_pEditSvr->m_pEdit)
					{
						LPTSTR pStr = this->m_pEditSvr->m_pEdit->GetText();
						CDwTable * pDwTable =  GetGridDataSvr()->GetDwTableFromCell(GetCol(),GetRow());
						if(!pStr && pDwTable)
						{
							int nRow,nCol;
							pDwTable->getRowColumnFromCell(GetCol(),GetRow(),nRow,nCol);
							pDwTable->setDwItemText(nRow,nCol,L"");
							pDwTable->setDwItemDisplayText(nRow,nCol,L"");	
							ShowQueryDlg();
							return true;
						}
						//ShowQueryDlg();
						PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(NewCurrent.x, NewCurrent.y);
						if(pItem && pItem->pStrXml)
						{
							xbXml xml;
							xml.LoadXml(pItem->pStrXml);
							IXMLDOMNode * pNode;
							IXMLDOMElement * pElement;
							_variant_t var;
							xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
							if(pNode)
							{
								pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
								pElement->getAttribute(L"dddwdataurl",&var);
								if(var.bstrVal && var.bstrVal[0]!=0)
								{
									TCHAR buf[512];
									if(_tcsstr(var.bstrVal,_T("xquery:"))==var.bstrVal)
										::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),var.bstrVal);
									else
										::_stprintf_s(buf,512,_T("%s"),var.bstrVal);  
																					
									TCHAR WKURL[512];
									::_stprintf_s(WKURL,512,_T("<args><arg name='__wkurl' value='%s'/><arg name='querytxt' value='%s'/></args>"),var.bstrVal, pStr);
									LPTSTR pArg= WKURL;
									xbXml xdata;
									if(xfUriLoader::FetchXml(xfApp::GetApp()->m_pFrame->m_pXPage->m_pStrUrl,buf,pArg,xdata)==1)
									{
										IXMLDOMNodeList * pNodeList;
										xdata.GetXmlDoc()->selectNodes(L"*/*", &pNodeList);
										long len;
										pNodeList->get_length(&len);
										pNodeList->Release();
										if(len == 1)
										{
											IXMLDOMNode * pNodeEx;
											xdata.GetXmlDoc()->selectSingleNode(L"*/*[1]",&pNodeEx);

											LPTSTR pDataColumn = NULL;
											LPTSTR pDisplayColumn = NULL;

											pElement->getAttribute(L"dddwdatacolumn",&var);
											if(var.bstrVal) pDataColumn = _tcsdup(var.bstrVal);

											pElement->getAttribute(L"dddwdisplaycolumn",&var);
											if(var.bstrVal) pDisplayColumn = _tcsdup(var.bstrVal);

											if(pDataColumn)
											{
												LPCTSTR pData = NULL;
												IXMLDOMNode * pNodeRow;
												pNodeEx->selectSingleNode(pDataColumn,&pNodeRow);
												BSTR v;
												if(pNodeRow)
												{
													pNodeRow->get_text(&v);
													pData = _tcsdup(v);
													pNodeRow->Release();
												}
												LPCTSTR pDisplay = NULL; 
												pNodeEx->selectSingleNode(pDisplayColumn,&pNodeRow);
												if(pNodeRow)
												{
													pNodeRow->get_text(&v);
													pDisplay = _tcsdup(v);
													pNodeRow->Release();
												}
												::SysFreeString(v);
												if(pData)
												{
													GetGridDataSvr()->m_pContentSvr->SetItemString(GetCol(),GetRow(),(TCHAR *)pData);
													if(pItem && pItem->pStrText)delete pItem->pStrText;
													if(pDisplay) pItem->pStrText =  _tcsdup(pDisplay);

													if(m_pEditSvr && m_pEditSvr->m_pEdit)
													{
														if( m_pEditSvr->m_ACol == GetCol() && 
																m_pEditSvr->m_ARow == GetRow())
														{
															::SetWindowText(m_pEditSvr->m_pEdit->m_hWnd,pData);
														}
													}
													if(pDwTable)
													{
														int nRow,nCol;
														pDwTable->getRowColumnFromCell(GetCol(),GetRow(),nRow,nCol);
														pDwTable->setDwItemText(nRow,nCol,(LPTSTR)pData);
														pDwTable->event_ItemChanged(nRow,nCol,(LPTSTR)pData);
													}
													this->m_pEditSvr->HiddenEditor();
												}
											}
											if(pDataColumn)delete pDataColumn;
											if(pDisplayColumn)delete pDisplayColumn;
											pNodeEx->Release();
										}
										else if(len == 0)
										{
											ShowQueryDlg();
										}
										else
										{
											BSTR bstrXml;
											xdata.m_pXmlDoc->get_xml(&bstrXml);
											ShowQueryDlg(bstrXml);
										}
									}
								}
								pElement->getAttribute(L"dddwxpageurl",&var);
								if(var.bstrVal && var.bstrVal[0]!=0)
								{
									if(pDwTable)
									{
										int nRow,nCol;
										pDwTable->getRowColumnFromCell(GetCol(),GetRow(),nRow,nCol);
										LPTSTR szStr = pDwTable->getDwItemDisplayText(nRow,nCol);
										if(!szStr || (szStr && this->m_pEditSvr->m_pEdit->GetText() && _tcscmp(szStr, this->m_pEditSvr->m_pEdit->GetText()) != 0))
										{
											pDwTable->setDwItemText(nRow,nCol,this->m_pEditSvr->m_pEdit->GetText());
											pDwTable->setDwItemDisplayText(nRow,nCol,this->m_pEditSvr->m_pEdit->GetText());
										}
										else if(!this->m_pEditSvr->m_pEdit->GetText())
										{
											pDwTable->setDwItemText(nRow,nCol,L"");
											pDwTable->setDwItemDisplayText(nRow,nCol,L"");
										}
									}
									ShowQueryDlg(L"  ");
								}
								pElement->Release();
								pNode->Release();
							}
						}

					}
					return true;
				}
			}
		}

		MaxTopLeft.x = GetColCount() - 1;
		MaxTopLeft.y = GetRowCount() - 1;
		if(MaxTopLeft.y < 1) MaxTopLeft.y=1;
		MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
		Restrict(NewTopLeft, GetFixedCols(), GetFixedRows(), MaxTopLeft.x, MaxTopLeft.y);
		if( (NewTopLeft.x != GetLeftCol()) || (NewTopLeft.y != GetTopRow()))
			MoveTopLeft(NewTopLeft.x, NewTopLeft.y);
		Restrict(NewCurrent, GetFixedCols(), GetFixedRows(), GetColCount() - 1,  GetRowCount() - 1);
		if( (NewCurrent.x != GetCol()) || (NewCurrent.y != GetRow()))
		{
			if(m_pEditSvr && m_pEditSvr->TipHWMD && ::IsWindow(m_pEditSvr->TipHWMD)) ::DestroyWindow(m_pEditSvr->TipHWMD);
			FocusCell(NewCurrent.x, NewCurrent.y, !(ssShift & Shift));
		}

		if(!FEditorMode) 
		{
			if(nChar > 127) 
			{
				ShowEditor();
				//if(m_pEditSvr && m_pEditSvr->m_pEdit)
				//	::SendMessage(m_pEditSvr->m_pEdit->m_hWnd,WM_CHAR,nChar,0);
			}
		}
		return true;
	}

	bool CGridFrameSvr::EvtChar(TEvent *pEvent ,LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		if(!(goAlwaysShowEditor & FOptions) && nChar == VK_RETURN )
		{
			if(FEditorMode)
				HideEditor();
			else
				ShowEditor();
			nChar = 0;
		}
		if(FEditorMode)
		{
			switch(nChar)
			case VK_UP:
			case VK_DOWN:
			case VK_PRIOR:
			case VK_NEXT:
			case VK_ESCAPE:
			case VK_INSERT:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_HOME:
			case VK_END: 
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
			case VK_F9:
			case VK_F10:
			case VK_F11:
			case VK_F12:
			case VK_TAB:
			case VK_DELETE:
				return true;
		}
		ShowEditor();
		if(nChar >=32)
		if(m_pEditSvr && m_pEditSvr->m_pEdit)
			::SendMessage(m_pEditSvr->m_pEdit->m_hWnd,WM_CHAR,nChar,0);
		return true;
	}

	BOOL CGridFrameSvr::GetTabStops(int Index)
	{
		return TRUE;
		/**if(GetFTabStops() == NULL)
		return True;
		else
		return  GetFTabStops()[Index + 1];**/
	}

	void CGridFrameSvr::Click()
	{

	}

	void CGridFrameSvr::FocusCell(int ACol, int ARow, BOOL MoveAnchor)
	{
		MoveCurrent(ACol, ARow, MoveAnchor, TRUE);
		UpdateEdit();
		Click();
	}

	void CGridFrameSvr::MoveAnchor(TGridCoord NewAnchor)
	{
		TGridRect OldSel;
		if(goRangeSelect & FOptions /*&& !(goEditing & FOptions)*/)
		{
			OldSel = GetSelection();
			FAnchor = NewAnchor;
			if(goRowSelect & FOptions) FAnchor.x = GetColCount() - 1;
			ClampInView(NewAnchor);
			SelectionMoved(OldSel);
		}
		else MoveCurrent(NewAnchor.x, NewAnchor.y, TRUE, TRUE);

		/*
		CELLNMHDR hdr;
		memset(&hdr,0,sizeof(CELLNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = CELL_SELECTCHANGED;
		hdr.currentrow = FCurrent.y;
		hdr.currentcol = FCurrent.x;
		hdr.anchorrow  = FAnchor.y;
		hdr.anchorcol  = FAnchor.x;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);*/
	}

	bool CGridFrameSvr::EvtTimer(TEvent *pEvent ,LPARAM lParam)
	{
		int nIDEvent=(int) pEvent->wParam;
		TPoint Point;
		TGridDrawInfo DrawInfo;
		TGridScrollDirection ScrollDirection;
		TGridCoord CellHit;

		if(nIDEvent ==ID_TIMER)
		{

			if( !(FGridState && (gsSelecting|gsRowMoving|gsColMoving |gsFilling))) return true;
			GetCursorPos(&Point);
			::ScreenToClient(GetHWND(),&Point);

			//Point.x -= FRect.left;
			//Point.y -= FRect.top;

			InitDrawInfo(&DrawInfo);
			ScrollDirection = 0;

			CellHit = CalcCoordFromPoint(Point.x, Point.y/**, DrawInfo**/);
			switch(FGridState)
			{
			case gsColMoving:
				/**MoveAndScroll(Point.x, CellHit.x, DrawInfo, DrawInfo.Horz, SB_HORZ);**/
				break;
			case gsRowMoving:
				/**MoveAndScroll(Point.y, CellHit.y, DrawInfo, DrawInfo.Vert, SB_VERT);**/
				break;
			case gsFilling:
				{
					TGridCoord newFillingAnchor;
					if( Point.x < DrawInfo.BandWidth)//Horz.FixedBoundary)
					{
						if(this->FTopLeft.x > this->FFixedCols)
						{
							CellHit.x = this->FTopLeft.x - 1;
							ClampInView(CellHit);
							if(CellHit.y >= (FCurrent.y <= FAnchor.y?FCurrent.y:FAnchor.y) && 
								CellHit.y <= (FCurrent.y >= FAnchor.y?FCurrent.y:FAnchor.y))
							{
								newFillingAnchor.y = FFillingAnchor.y;
								newFillingAnchor.x = FFillingAnchor.x - 1;
								MoveFillingAnchor(DrawInfo,newFillingAnchor);
							}
						}
					}else if( Point.x > DrawInfo.FullVisBoundaryCol)//Horz.FullVisBoundary
					{
						if(DrawInfo.LastFullVisibleCol/*Horz.LastFullVisibleCell*/ < this->GetColCount() - 1)
						{
							CellHit.x = DrawInfo.LastFullVisibleCol/*Horz.LastFullVisibleCell*/ + 1;
							ClampInView(CellHit);
							if(CellHit.y >= (FCurrent.y <= FAnchor.y?FCurrent.y:FAnchor.y) && 
								CellHit.y <= (FCurrent.y >= FAnchor.y?FCurrent.y:FAnchor.y))
							{
								newFillingAnchor.y = FFillingAnchor.y;
								newFillingAnchor.x = FFillingAnchor.x + 1;
								MoveFillingAnchor(DrawInfo,newFillingAnchor);
							}
						}
					}
					if( Point.y < DrawInfo.ColHeaderHeight)//(Vert.FixedBoundary)
					{
						if(this->FTopLeft.y > this->FFixedRows)
						{
							CellHit.y = this->FTopLeft.y - 1;
							ClampInView(CellHit);
							if(CellHit.x >= (FCurrent.x <= FAnchor.x?FCurrent.x:FAnchor.x) && 
								CellHit.x <= (FCurrent.x >= FAnchor.x?FCurrent.x:FAnchor.x))
							{
								newFillingAnchor.x = FFillingAnchor.x;
								newFillingAnchor.y = FFillingAnchor.y - 1;
								MoveFillingAnchor(DrawInfo,newFillingAnchor);
							}
						}
					}
					else if( Point.y > DrawInfo.FullVisBoundaryCol/*Vert.FullVisBoundary*/) 
					{
						if(DrawInfo.LastFullVisibleRow/*Vert.LastFullVisibleCell*/ < this->GetRowCount() - 1)
						{
							CellHit.y = DrawInfo.LastFullVisibleRow/*Vert.LastFullVisibleCell*/ + 1;
							ClampInView(CellHit);
							if(CellHit.x >= (FCurrent.x <= FAnchor.x?FCurrent.x:FAnchor.x) && 
								CellHit.x <= (FCurrent.x >= FAnchor.x?FCurrent.x:FAnchor.x))
							{
								newFillingAnchor.x = FFillingAnchor.x;
								newFillingAnchor.y = FFillingAnchor.y + 1;
								MoveFillingAnchor(DrawInfo,newFillingAnchor);
							}
						}
					}
				}
				break;
			case gsSelecting:
				{
					if( Point.x < FRect.left)
						ScrollDirection = ScrollDirection|sdLeft;
					else if( Point.x > DrawInfo.FullVisBoundaryCol) ScrollDirection = ScrollDirection|sdRight;
					if( Point.y < FRect.top)ScrollDirection = ScrollDirection|sdUp;
					else if( Point.y > DrawInfo.FullVisBoundaryRow) ScrollDirection = ScrollDirection|sdDown;
					if(ScrollDirection)  TimedScroll(ScrollDirection);
				}
			}
		}else if(nIDEvent == ID_TRACETIMER )
		{
			::KillTimer(GetHWND(),ID_TRACETIMER);

			FMouseTrace = false;
			if(m_TipHWND && ::IsWindow(m_TipHWND))
			{
				::DestroyWindow(m_TipHWND);
				m_TipHWND = NULL;
			}
			if(this->FOptions & goRunState)
			{
				HWND cHwnd = ::GetFocus();
				if(cHwnd != this->m_pSheetSvr->m_pSheetCtrl->GetHWND()) return true;

				GetCursorPos(&Point);
				::ScreenToClient(GetHWND(),&Point);
				CellHit = CalcCoordFromPoint(Point.x, Point.y); 
				TGridRect gRect;
				GetGridDataSvr()->m_pSpanSvr->isSpaned(CellHit.x,CellHit.y,gRect);
				if(gRect.top < 0 || gRect.left<0) return true;

				LPTSTR visible = this->GetItemProp(gRect.left,gRect.top,_T("cellvisible"));
				if(visible && _tcsicmp(visible,_T(""))!=0)
				{
					if(_tcsicmp(visible,_T("否"))==0 || _tcsicmp(visible,_T("false"))==0 ||
						_tcsicmp(visible,_T("不可见"))==0 || _tcsicmp(visible,_T("0"))==0) 
					{
						if(visible) delete visible;
						return true;
					}
					if(visible)delete visible;
				}

				PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(gRect.left,gRect.top);
				if(pItem && (pItem->pStrContent || pItem->pStrText))
				{
					TCHAR * pBuf = new TCHAR[1024];
					LPTSTR pTxt;
					if(pItem->pStrText)
						pTxt = pItem->pStrText;
					else
						pTxt = pItem->pStrContent;
					if(pItem->pOriginStr)
						::_stprintf(pBuf,_T("%s\n%s"),pTxt,pItem->pOriginStr);
					else if(pItem->pOrigin)
						::_stprintf(pBuf,_T("%s\n%s"),pTxt,pItem->pOrigin);
					else
						pBuf = _tcsdup(pTxt);
					SIZE si;
					::GetTextExtentPoint32(::GetDC(GetHWND()), pBuf, _tcslen(pBuf), &si);
					RECT re;
					this->GetCellRect(gRect.left,gRect.top, re);
					if(si.cx < (re.right - re.left)) return true;

					HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
											  WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
											  //WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
											  CW_USEDEFAULT, CW_USEDEFAULT,
											  CW_USEDEFAULT, CW_USEDEFAULT,
											  this->m_pSheetSvr->m_pSheetCtrl->GetHWND(), NULL, 
											  xfApp::GetApp()->GetInstance(), NULL);
					if(hwndTip)
					{
						m_TipHWND = hwndTip;
						SetWindowPos(hwndTip, HWND_TOPMOST,0,0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
						// Associate the tooltip with the tool.
						TOOLINFO toolInfo;
						ZeroMemory(&toolInfo,sizeof(TOOLINFO));
						toolInfo.cbSize = sizeof(toolInfo);
						toolInfo.hwnd = this->m_pSheetSvr->m_pSheetCtrl->GetHWND();
						toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
						toolInfo.uId = (UINT_PTR)this->m_pSheetSvr->m_pSheetCtrl->GetHWND();
						toolInfo.lpszText = pBuf;
						::SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
						::SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)30000);
						::SendMessage(hwndTip, TTM_SETMAXTIPWIDTH,0,500);
					}
				}
			}
/*
			if( abs(pt.x - m_ptTrace.x) > 0 || abs(pt.y - m_ptTrace.y) > 0 || !FMouseTrace )
			{
				FMouseTrace = false;
				if(m_TipHWND && ::IsWindow(m_TipHWND))
				{
					::DestroyWindow(m_TipHWND);
					m_TipHWND = NULL;
				}
			}else
			{
				m_nDelayed += 50;
				if(m_nDelayed > m_nDelay)
				{
					::KillTimer(GetHWND(),ID_TRACETIMER );
					FMouseTrace = false;

					CELLTIPNMHDR hdr;
					memset(&hdr,0,sizeof(CELLNMHDR));
					hdr.hwndFrom = GetHWND();
					hdr.idFrom = 0;
					hdr.code = CELL_NEEDTIPTEXT;
					hdr.currentrow = FCurrent.y;
					hdr.currentcol = FCurrent.x;
					hdr.anchorrow  = FAnchor.y;
					hdr.anchorcol  = FAnchor.x;

					HWND hWnd = GetHWND();

					SendMessage(::GetParent(::GetParent(GetHWND())),WM_NOTIFY,0,(LPARAM)&hdr);
					if(m_TipHWND && ::IsWindow(m_TipHWND)) 
					{
						return 1;
						//::DestroyWindow(m_TipHWND);
					}
					xfWin * pWin = xfWin::LoadXPage(NULL,L"dev:xpage[sale.order.top]",/*WS_EX_TOPMOST|WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE,WS_POPUP,NULL,this->m_pSheetSvr->m_pSheetCtrl->GetHWND(),
						xfApp::GetApp()->GetInstance());
					if(pWin)
					{
						m_TipHWND = pWin->GetHWND();
						::ShowWindow(m_TipHWND,SW_SHOWNOACTIVATE);
					}
 				}
			}*/
		}
		return true;
	}
	void CGridFrameSvr::TimedScroll(TGridScrollDirection Direction)
	{
		TGridCoord MaxAnchor, NewAnchor;

		NewAnchor = FAnchor;
		MaxAnchor.x = GetColCount() - 1;
		MaxAnchor.y = GetRowCount() - 1;
		if( (sdLeft & Direction) && (FAnchor.x > FFixedCols))
			NewAnchor.x --;
		if( (sdRight & Direction) && (FAnchor.x < MaxAnchor.x)) NewAnchor.x++;
		if( (sdUp & Direction) && (FAnchor.y > FFixedRows))NewAnchor.y--;
		if( (sdDown & Direction) && (FAnchor.y < MaxAnchor.y)) NewAnchor.y++;
		if( (FAnchor.x != NewAnchor.x) || (FAnchor.y != NewAnchor.y))
			MoveAnchor(NewAnchor);
	}
	bool CGridFrameSvr::EvtMouseWheel(TEvent *pEvent ,LPARAM lParam)
	{
		if(pEvent->wheel.zDelta > 0)
			ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
		else
			ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
		return true;
	}

	void CGridFrameSvr::MoveColData(int FromIndex, int ToIndex)
	{
		this->GetGridDataSvr()->m_pContentSvr->MoveCol(FromIndex,ToIndex);
	}

	void CGridFrameSvr::MoveColumn(int FromIndex, int ToIndex)
	{
		if(FromIndex==ToIndex) return;
		RECT rect;
		if(ToIndex != GetColCount() - 1 && FromIndex != GetColCount() - 1)
		{
			for(int i=0;i<GetGridDataSvr()->m_pSpanSvr->SpanRects.size();i++)
			{
				rect = GetGridDataSvr()->m_pSpanSvr->SpanRects[i];
				if(rect.left !=rect.right &&  rect.left<=FromIndex && rect.right>=FromIndex)
				{
					FGridState = gsNormal;
					MessageBox(NULL,_T("不能对合并单元格作部分改动"),_T("提示!"),MB_OK);
					return;
				}
				if(FromIndex < ToIndex)
				{
					if(rect.left<=ToIndex && rect.right>ToIndex)
					{
						FGridState = gsNormal;
						MessageBox(NULL,_T("不能对合并单元格作部分改动"),_T("提示!"),MB_OK);
						return;
					}
				}else
				{
					if(rect.left<ToIndex && rect.right>=ToIndex)
					{
						FGridState = gsNormal;
						MessageBox(NULL,_T("不能对合并单元格作部分改动"),_T("提示!"),MB_OK);
						return;
					}
				}
			}
		}
		if(FromIndex == GetColCount() - 1)	//INSERTCol
		{
			GetGridDataSvr()->m_pSpanSvr->InsertCol(ToIndex);
		}
		if(ToIndex == GetColCount() - 1)	//DELETECol
		{
			GetGridDataSvr()->m_pSpanSvr->DeleteCol(FromIndex);
		}
		if(ToIndex != GetColCount() - 1 && FromIndex != GetColCount() - 1) //move
		{
			GetGridDataSvr()->m_pSpanSvr->MoveCol(FromIndex,ToIndex);
		}
		GetGridDataSvr()->m_pSpanSvr->RefreshSpans();

		TGridRect Rect;
		if(ToIndex > FromIndex)
		{
			GetGridDataSvr()->m_pAxisSvr->m_pColWidths->Insert(ToIndex,1);
			GetGridDataSvr()->m_pAxisSvr->SetColWidth(ToIndex, GetGridDataSvr()->m_pAxisSvr->GetColWidth(FromIndex));
			GetGridDataSvr()->m_pAxisSvr->m_pColWidths->Remove(FromIndex,1);
		}else
		{
			int extent = GetGridDataSvr()->m_pAxisSvr->GetColWidth(FromIndex);
			GetGridDataSvr()->m_pAxisSvr->m_pColWidths->Remove(FromIndex,1);
			GetGridDataSvr()->m_pAxisSvr->m_pColWidths->Insert(ToIndex,1);
			GetGridDataSvr()->m_pAxisSvr->SetColWidth(ToIndex,extent );
		}

		MoveAdjust(FCurrent.x, FromIndex, ToIndex);
		MoveAdjust(FAnchor.x, FromIndex, ToIndex);
		MoveAdjust(FInplaceCol, FromIndex, ToIndex);

		MoveColData(FromIndex,ToIndex);
		ColumnMoved(FromIndex, ToIndex);

		if(ToIndex ==GetColCount() - 1)
		{
			for(int i=1;i<GetRowCount();i++)
			{
				PStrItem	pItem;
				pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(GetColCount() - 1,i);
				if(pItem)this->GetGridDataSvr()->m_pContentSvr->SetItemData(GetColCount() - 1,i,NULL);
			}
		}

		TGridDrawInfo  DrawInfo;
		InitDrawInfo(&DrawInfo);

		Rect.top = FTopLeft.y;
		Rect.bottom = GetTopRow() + DrawInfo.LastFullVisibleRow +1;
		if(FromIndex < ToIndex )
		{
			Rect.left = FromIndex;
			Rect.right = ToIndex;
		}
		else
		{
			Rect.left = ToIndex;
			Rect.right = FromIndex;
		}
		InvalidateRect(Rect);
		ColWidthsChanged();
		UpdateEdit();
		/**this->UpdateObject();**/
	}

	void CGridFrameSvr::DeleteColumn(int ACol)
	{
		MoveColumn(ACol, GetColCount()-1);
		if(ACol >= 1)
			this->GetGridDataSvr()->GetStyleSvr()->RemoveCol(ACol,1);
		m_ColCount = GetColCount() - 1;
		MoveCurrent(ACol,this->FCurrent.y,TRUE,TRUE);
	}

	int CGridFrameSvr::InsertCol(int ACol)
	{
		this->AdjustSize(GetColCount(),1,FALSE);
		MoveColumn(GetColCount() - 1,ACol);
		BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);
		if(ACol > 1)
		{
			this->GetGridDataSvr()->GetStyleSvr()->InsertCol(ACol,1);
			this->SetColWidth(ACol,this->GetColWidth(ACol - 1));
		}else
		{
			this->GetGridDataSvr()->GetStyleSvr()->InsertCol(0,1);
			this->SetColWidth(ACol,this->GetColWidth(ACol - 1));
		}
		return ACol;
	}

	int CGridFrameSvr::InsertRow(int ARow,int Number,bool bCopyMode)
	{
		this->AdjustSize(GetRowCount(),Number,TRUE);
		BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);

		//adjust spans data
		int cnt = GetGridDataSvr()->m_pSpanSvr->SpanRects.size();
		for(int i=0;i< cnt;i++)
		{
			RECT & rect = GetGridDataSvr()->m_pSpanSvr->SpanRects[i];
			if(rect.top >= ARow)
			{
				rect.top += Number;
				rect.bottom += Number;
			}else if(rect.bottom == ARow - 1 &&  rect.top== ARow - 1 )
			{
				if(bCopyMode)
				{
					for(int k=1;k<=Number;k++)
					{
						rect = rect;
						rect.top = rect.bottom = ARow + k - 1 ;
						GetGridDataSvr()->m_pSpanSvr->SpanRects.push_back(rect);
					}
				}
			}else if(rect.bottom == ARow - 1 &&  rect.top<= ARow - 1 )
			{
				PStrItem pItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(rect.left,rect.top);
				if(!pItem || !(pItem->pStrGroup && pItem->pStrColumn)) 
					rect.bottom += Number;
			}
			else if(rect.top < ARow && rect.bottom >= ARow)
			{
				rect.bottom += Number;
			} 
		}

		int ToIndex = GetRowCount() - Number;
		int FromIndex = ARow;

		if(ToIndex > FromIndex)
		{
			this->GetGridDataSvr()->GetStyleSvr()->InsertRow(FromIndex,Number);
			GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Insert(FromIndex,Number);
			/*
			for(int i=0;i<Number;i++)
			{
				GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Insert(ToIndex,1);
				GetGridDataSvr()->m_pAxisSvr->SetRowHeight(ToIndex, GetGridDataSvr()->m_pAxisSvr->GetRowHeight(FromIndex));
				GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Remove(FromIndex,1);
			}
			for(int i=0;i<Number;i++)
			{
				this->GetGridDataSvr()->GetStyleSvr()->InsertRow(FromIndex,1);
			}*/
		}else
		{
			for(int i=0;i<Number;i++)
			{
				int extent = GetGridDataSvr()->m_pAxisSvr->GetRowHeight(FromIndex);
				GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Remove(FromIndex,1);
				GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Insert(ToIndex,1);
				GetGridDataSvr()->m_pAxisSvr->SetRowHeight(ToIndex,extent );
			}
			for(int i=0;i<Number;i++)
			{
				this->GetGridDataSvr()->GetStyleSvr()->RemoveRow(ToIndex,1);
			}
		}

		if(FCurrent.y>=ARow)FCurrent.y += Number;
		if(FAnchor.y>=ARow)FAnchor.y += Number;
		if(FInplaceRow>=ARow)FInplaceRow += Number;

		MoveRowData(GetRowCount() - Number, ARow,Number);
		RowMoved(GetRowCount() - Number, ARow,Number);


		if(ARow > 1 && bCopyMode)
		{
			this->GetGridDataSvr()->m_pContentSvr->MoveRow(FromIndex,ToIndex,Number);
			for(int k=1;k<=Number;k++)
			{
				this->SetRowHeight(ARow + k -1,this->GetRowHeight(ARow - 1));
			}
		}

		//修改报表band 忽略
		CDwTable::AdjustDwTables(this,ARow,Number);

		int nIndex = this->GetGridDataSvr()->GetChildIndex(ARow);
		if(nIndex>=0)
			(*GetGridDataSvr()->m_pChilds)[nIndex]->endRow += Number;
		return ARow ;
	}

	int CGridFrameSvr::DeleteRow(int ARow,int Number)
	{
		BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);

		//adjust spans data
		int cnt =this->GetGridDataSvr()->m_pSpanSvr->SpanRects.size();
		for(int i=0;i< cnt;i++)
		{
			RECT &rect = GetGridDataSvr()->m_pSpanSvr->SpanRects[i];
			if(rect.bottom <= ARow + Number - 1 &&  rect.top >= ARow )
			{
				rect.bottom = 0;
				rect.top = 0;
			}
			if(rect.top < ARow && rect.bottom >= ARow)
			{
				rect.bottom -= (rect.bottom - ARow + 1)>Number?Number:(rect.bottom - ARow + 1) ;
			}
			if(rect.top >= ARow && rect.top <= ARow + Number - 1)
			{
				if(GetGridDataSvr()->m_pContentSvr->GetItemData(rect.left,rect.top))
				{
					PStrItem	pItem  = new StrItem();
					pItem->CopyFrom(GetGridDataSvr()->m_pContentSvr->GetItemData(rect.left,rect.top));
					GetGridDataSvr()->m_pContentSvr->SetItemData(rect.left,ARow + Number,pItem);
				}
				rect.top = ARow; rect.bottom -= Number;
			}
			if( rect.top > ARow + Number - 1)
			{
				rect.top -= Number; rect.bottom -= Number;
			}
		}
		GetGridDataSvr()->m_pSpanSvr->RefreshSpans();

		this->GetGridDataSvr()->m_pAxisSvr->m_pRowHeights->Remove(ARow,Number);
		this->GetGridDataSvr()->GetStyleSvr()->RemoveRow(ARow,Number);
		/*
		if(FCurrent.y>=ARow)FCurrent.y -= Number;
		if(FAnchor.y>=ARow)FAnchor.y -= Number;
		if(FInplaceRow>=ARow)FInplaceRow -= Number;
		*/

		MoveRowData(ARow,GetRowCount() - 1,Number);
		RowMoved(ARow,GetRowCount() - 1,  Number);

		for(int row= this->GetRowCount() - Number;row <= this->GetRowCount() - 1;row++)
		{
			this->GetGridDataSvr()->m_pContentSvr->ClearRowData(row);
		}
		m_RowCount = GetRowCount() - Number;

		CDwTable::AdjustDwTables(this,ARow,- Number);

		int nIndex = this->GetGridDataSvr()->GetChildIndex(ARow);
		if(nIndex>=0)
		{
			(*GetGridDataSvr()->m_pChilds)[nIndex]->endRow -= Number;
			if((*GetGridDataSvr()->m_pChilds)[nIndex]->endRow < 
				(*GetGridDataSvr()->m_pChilds)[nIndex]->startRow)
				GetGridDataSvr()->m_pChilds->erase(GetGridDataSvr()->m_pChilds->begin() + nIndex);
		}

		//修改报表band 忽略
		return ARow ;
	}

	bool CGridFrameSvr::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND CGridFrameSvr::GetHWND()
	{
		return m_pSheetSvr->GetHWND();
	}

	int CGridFrameSvr::SetRunState(int runstate)
	{
		this->FOptions &= ~goDesignState;
		this->FOptions &= ~goRunState;
		this->FOptions &= ~goNormalState;
		this->FOptions |= runstate;

		return 1;
	}
	
	CDwTable * CGridFrameSvr::GetCurDwTable()
	{
		return this->GetGridDataSvr()->GetDwTableFromCell(FCurrent.x,FCurrent.y);
	}

	int CGridFrameSvr::SetDwGrid(CDwTable * pTable)
	{
		pTable->m_pGrid = this;
		for(int i=0;i< (int)pTable->m_pChilds.size();i++)
		{
			SetDwGrid(pTable->m_pChilds[i]);
		}
		return 1;
	}

	int CGridFrameSvr::Copy()
	{
		int row,col;
		TGridRect  rect;
		TCHAR* lpdata;
		TCHAR* svalue;
		int pos = 0;
		HGLOBAL        hClipGlobal;
		
		hClipGlobal = GlobalAlloc(0,128000*sizeof(TCHAR));   
		lpdata = (TCHAR *)GlobalLock(hClipGlobal);
		ZeroMemory(lpdata,128000*sizeof(TCHAR));
		rect = GetSelection();
		for(row=rect.top;row <= rect.bottom;row++)
		{
			for(col=rect.left;col <= rect.right;col++)
			{
				
				svalue =  GetGridDataSvr()->m_pContentSvr->GetItemString(col,row);
				if(!svalue || svalue[0] =='\0')
				{
					if(col < rect.right)
					{
						lpdata[pos]='\t';
						pos ++;
						lpdata[pos]='\0';
						
					}
				}
				else
				{
					_tcscat_s(lpdata,128000,svalue);
					pos = pos+(int)_tcslen(svalue);
					if(col < rect.right)
					{
						lpdata[pos]='\t';
						pos ++;
						lpdata[pos]='\0';
					}                                     
				}
			}
			if(row < rect.bottom)
			{
				lpdata[pos]='\r';pos ++;lpdata[pos]='\0';
				lpdata[pos]='\n';pos ++;lpdata[pos]='\0';
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

	int CGridFrameSvr::Cut()
	{
		int row,col;
		TGridRect  rect;
		TCHAR* lpdata;
		TCHAR* svalue;
		int pos = 0;
		HGLOBAL        hClipGlobal;
		
		hClipGlobal = GlobalAlloc(0,128000*sizeof(TCHAR));
		lpdata = (TCHAR *)GlobalLock(hClipGlobal);
		ZeroMemory(lpdata,128000*sizeof(TCHAR));
		rect = GetSelection();
		for(row=rect.top;row <= rect.bottom;row++)
		{
			for(col=rect.left;col <= rect.right;col++)
			{
				
				svalue =  GetGridDataSvr()->m_pContentSvr->GetItemString(col,row);
				if(!svalue || svalue[0] =='\0')
				{
					if(col < rect.right)
					{
						lpdata[pos]='\t';
						pos ++;
						lpdata[pos]='\0';
					}
				}
				else
				{
					_tcscat_s(lpdata,128000,svalue);
					pos = pos+(int)_tcslen(svalue);
					if(col < rect.right)
					{
						lpdata[pos]='\t';
						pos ++;
						lpdata[pos]='\0';
					}                                     
				}
				GetGridDataSvr()->m_pContentSvr->SetItemString(col,row,NULL);
			}
			if(row < rect.bottom)
			{
				lpdata[pos]='\r';pos ++;lpdata[pos]='\0';
				lpdata[pos]='\n';pos ++;lpdata[pos]='\0';
			}
		}
		GlobalUnlock(hClipGlobal);
		
		if(OpenClipboard(GetHWND()))
		{
			SetClipboardData(CF_UNICODETEXT,hClipGlobal);
			CloseClipboard();
		}
		this->InvalidateRect(rect);
		return 1;
	}

	int CGridFrameSvr::Paste()
	{
		HANDLE        hClipGlobal;
		void *       lpbi;
		int         length;
		
		OpenClipboard(GetHWND());
		if( (hClipGlobal = GetClipboardData( CF_UNICODETEXT)) != NULL )
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
				
				int row,col;
				int pos,start;
				int currow,curcol;
				int RowCount,colCount;
				TCHAR ch;
				TCHAR buffer[1024];
				BOOL  finish;
				
				finish = false;
				
				currow =GetRow();
				curcol = GetCol();
				RowCount = GetRowCount();
				colCount = GetColCount();
				pos = 0;
				for(row=currow;row<= RowCount;row++)
				{
					if(finish) break;
					for(col=curcol;col<=colCount;col++)
					{
						start = pos;
						if(*(lpdata+pos)=='"')
						{
							start++;
							pos++;
							while(*(lpdata+pos)!='"')
							{
								ch = *(lpdata+pos);
								buffer[pos - start]=ch;
								pos++;
								if(ch==0)break;
							}
							if(ch!=0)
							{
								buffer[pos-start] = 0;
								pos++;
								ch = *(lpdata+pos);
								pos++;
							}
						}else
						{
							do
							{
								ch = *(lpdata+pos);
								buffer[pos - start]=ch;
								pos++;
							}while(pos <length && ch !='\0' && ch !='\t' &&ch!='\r' && ch!='\n');
							if(ch=='\t')buffer[pos-start - 1]='\0';
							if(ch=='\r')buffer[pos-start - 1]='\0';
							if(ch=='\n')buffer[pos-start - 1]='\0';
						}
						if(pos == length ||ch=='\0' )
						{
							//note 从excel复制时最后会有一个回车
							if(buffer && _tcscmp(buffer, _T("")) != 0)
								this->GetGridDataSvr()->m_pContentSvr->SetItemString(col,row,buffer);
							finish = true;
							break;
						}
						this->GetGridDataSvr()->m_pContentSvr->SetItemString(col,row,buffer);

						
						if(ch=='\t') continue;
						if(ch=='\r')
						{
							ch = *(lpdata+pos);
							if(ch=='\n')pos++;
						}
						break;
					}
				}
				
				GlobalUnlock(hData);
				GlobalFree(hData);
				
				GlobalUnlock( hClipGlobal );
			}
		} 
		CloseClipboard();
		this->InvalidateGrid();
		return 1;
	}

	HDC CGridFrameSvr::GetPrintDC(HWND hWnd, WORD PD_FLAG){
		/*
		PRINTDLG *pr = new PRINTDLG;

		pr->lStructSize=sizeof(PRINTDLG); 
		pr->hwndOwner = NULL; 
		pr->hDevMode = NULL; 
		pr->hDevNames = NULL; 
		pr->hDC = 0; 
		pr->Flags = Flag ; 
		pr->nFromPage = 0; 
		pr->nToPage = 0; 
		pr->nMinPage = 0; 
		pr->nMaxPage = 0; 
		pr->nCopies = 0; 
		pr->hInstance = NULL; 
		pr->lCustData = NULL; 
		pr->lpfnPrintHook = NULL; 
		pr->lpfnSetupHook = NULL; 
		pr->lpPrintTemplateName = NULL; 
		pr->lpSetupTemplateName = NULL; 
		pr->hPrintTemplate = NULL; 
		pr->hSetupTemplate = NULL;

		PrintDlg(pr);

		LPDEVNAMES lpDevNames = (LPDEVNAMES)::GlobalLock(pr->hDevNames);
		LPDEVMODE  lpDevMode = (pr->hDevMode != NULL) ?(LPDEVMODE)::GlobalLock(pr->hDevMode) : NULL;

		if (lpDevNames == NULL)
		return NULL;

		HDC hDC = ::CreateDC((LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
		(LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
		(LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
		lpDevMode);

		::GlobalUnlock(pr->hDevNames);
		if (pr->hDevMode != NULL)
		::GlobalUnlock(pr->hDevMode);

		delete pr;*/

		HRESULT hResult;
		LPPRINTDLGEX  pPDX = NULL;
		LPPRINTPAGERANGE pPageRanges = NULL;

		// Allocate the PRINTDLGEX structure.

		pPDX = (LPPRINTDLGEX)GlobalAlloc(GPTR, sizeof(PRINTDLGEX));
		if (!pPDX)
			return NULL;

		// Allocate an array of PRINTPAGERANGE structures.

		pPageRanges = (LPPRINTPAGERANGE) GlobalAlloc(GPTR, 
			10 * sizeof(PRINTPAGERANGE));
		if (!pPageRanges)
			return NULL;

		//  Initialize the PRINTDLGEX structure.

		HWND	hParent = hWnd;
		hParent = hWnd;
		while(::GetParent(hParent))
		{
			hParent = ::GetParent(hParent);
		}

		pPDX->lStructSize = sizeof(PRINTDLGEX);
		pPDX->hwndOwner = hParent;
		pPDX->hDevMode = NULL;
		pPDX->hDevNames = NULL;
		pPDX->hDC = NULL;
		pPDX->Flags = PD_RETURNDC | PD_COLLATE |PD_FLAG;
		pPDX->Flags2 = 0;
		pPDX->ExclusionFlags = 0;
		pPDX->nPageRanges = 0;
		pPDX->nMaxPageRanges = 10;
		pPDX->lpPageRanges = pPageRanges;
		pPDX->nMinPage = 1;
		pPDX->nMaxPage = 1000;
		pPDX->nCopies = 1;
		pPDX->hInstance = 0;
		pPDX->lpPrintTemplateName = NULL;
		pPDX->lpCallback = NULL;
		pPDX->nPropertyPages = 0;
		pPDX->lphPropertyPages = NULL;
		pPDX->nStartPage = START_PAGE_GENERAL;
		pPDX->dwResultAction = 0;

		//  Invoke the Print property sheet.

		hResult = PrintDlgEx(pPDX);

		/*if ( (hResult == S_OK) &&
		pPDX->dwResultAction == PD_RESULT_PRINT) {
		return pPDX->hDC;
		}*/

		//if (pPDX->hDC != NULL) 
		//	DeleteDC(pPDX->hDC);
		if (pPDX->hDevMode != NULL) 
			GlobalFree(pPDX->hDevMode); 
		if (pPDX->hDevNames != NULL) 
			GlobalFree(pPDX->hDevNames); 
		if (hResult == S_OK)
			return  pPDX->hDC;
		else
			return NULL;
	};    

	void CGridFrameSvr::Print()
	{
		HDC hPrintDC = NULL;
		hPrintDC = GetPrintDC(GetHWND(),0/*PD_PRINTSETUP|PD_RETURNDEFAULT*/);
		if(!hPrintDC) return; 
		Print(hPrintDC);
	}

	void CGridFrameSvr::Print(HDC hPrintDC)
	{
		DOCINFO docInfo;
		docInfo.cbSize=sizeof( DOCINFO);
		docInfo.lpszDocName=_T("hxent");
		docInfo.lpszOutput=NULL;
		docInfo.lpszDatatype = NULL;
		docInfo.fwType = NULL;

		int ACol,ARow;
		this->GetGridDataSvr()->m_pContentSvr->GetValidRowCol(ACol,ARow);

		HDC	dc = ::GetDC(NULL);

		int pageW,pageH;
		int OffsetW,OffsetH;
		pageH = GetDeviceCaps(hPrintDC,PHYSICALHEIGHT);
		pageW = GetDeviceCaps(hPrintDC,PHYSICALWIDTH);
		OffsetW = GetDeviceCaps(hPrintDC,PHYSICALOFFSETX);
		OffsetH = GetDeviceCaps(hPrintDC,PHYSICALOFFSETY);

		pageW = pageW * ::GetDeviceCaps(dc,LOGPIXELSX) / ::GetDeviceCaps(hPrintDC,LOGPIXELSX);
		pageH = pageH * ::GetDeviceCaps(dc,LOGPIXELSY) / ::GetDeviceCaps(hPrintDC,LOGPIXELSY);

		OffsetW = OffsetW * ::GetDeviceCaps(dc,LOGPIXELSX) / ::GetDeviceCaps(hPrintDC,LOGPIXELSX);
		OffsetH = OffsetH * ::GetDeviceCaps(dc,LOGPIXELSY) / ::GetDeviceCaps(hPrintDC,LOGPIXELSY);

		OffsetW += 

		ReleaseDC(NULL,dc);

		StartDoc(hPrintDC,&docInfo);
		pageAxis * pAxisPage = NULL;
		int iPageCount;
		RECT	pageRect;

		pageRect.left = OffsetW ;
		pageRect.top = OffsetH ;
		pageRect.right = pageW  -  OffsetW ;
		pageRect.bottom = pageH - OffsetH ;

		this->CalcPrintPageAxis(pageRect.right - pageRect.left + 1,pageRect.bottom - pageRect.top + 1,pAxisPage,iPageCount);
		for(int i = 1;i<=iPageCount;i++)
		{
			this->PrintPage(hPrintDC,pAxisPage[i - 1],&pageRect);
		}
		if(pAxisPage) delete[] pAxisPage;
		EndDoc(hPrintDC);
		DeleteDC(hPrintDC);
	}
	int CGridFrameSvr::PrintPage(HDC hPrintDC, pageAxis & pAxisPage,RECT * pPageArea)
	{
		StartPage(hPrintDC);
		HDC	dc = ::GetDC(NULL);
		SetMapMode(hPrintDC,MM_ANISOTROPIC);
		SetWindowExtEx(hPrintDC,::GetDeviceCaps(dc,LOGPIXELSX),::GetDeviceCaps(dc,LOGPIXELSY),NULL);
		SetViewportExtEx(hPrintDC,GetDeviceCaps(hPrintDC,LOGPIXELSX),GetDeviceCaps(hPrintDC,LOGPIXELSY),NULL);
		::SetViewportOrgEx(hPrintDC,0,0,NULL);

		/*
		TGridDrawInfo DrawInfo;
		DrawInfo.Horz.EffectiveLineWidth = this->FGridLineWidth;
		DrawInfo.Vert.EffectiveLineWidth = this->FGridLineWidth;
		DrawInfo.Horz.LastFullVisibleCell = pAxisPage.endCol;
		DrawInfo.Vert.LastFullVisibleCell = pAxisPage.endRow;

		DrawCells(hPrintDC,pAxisPage.startCol,pAxisPage.startRow,pPageArea->left,pPageArea->top,pPageArea->right,pPageArea->bottom,
			RGB(0,0,0),0,DrawInfo);
		DrawSpanCellsEx(hPrintDC,pAxisPage.startCol,pAxisPage.startRow,pPageArea->left,pPageArea->top,pPageArea->right,pPageArea->bottom,
			RGB(0,0,0),0,DrawInfo);

		DrawInfo.Horz.FixedBoundary = pPageArea->left;//0;
		DrawInfo.Horz.FixedCellCount = 0;
		DrawInfo.Horz.FirstGridCell = pAxisPage.startCol;
		DrawInfo.Vert.FixedBoundary = pPageArea->top;//0;
		DrawInfo.Vert.FixedCellCount = 0;
		DrawInfo.Vert.FirstGridCell = pAxisPage.startRow;*/

		pPageArea->left += this->GetPrintHeaderHeight();
		pPageArea->top += this->GetPrintFooterHeight();
		pPageArea->right -= this->GetPrintRightWidth();
		pPageArea->bottom -= this->GetPrintLeftWidth();

		RECT drect ;
		::SetRect(&drect, pPageArea->left,pPageArea->top,pPageArea->right,pPageArea->bottom);

		DrawCells(hPrintDC,drect,pAxisPage.startCol,pAxisPage.startRow);
		DrawBorderLines(hPrintDC,drect,pAxisPage.startCol,pAxisPage.startRow);

		/*
		//print object
		POINT scroll;

		scroll.x = -1*GetWidth(1,pAxisPage.startCol - 1) - GetFBound().left;
		scroll.y = -1*GetHeight(1,pAxisPage.startRow - 1) - GetFBound().top;

		scroll.x = scroll.x * GetDeviceCaps(hPrintDC,LOGPIXELSX) / ::GetDeviceCaps(dc,LOGPIXELSX);
		scroll.y = scroll.y * GetDeviceCaps(hPrintDC,LOGPIXELSY) / ::GetDeviceCaps(dc,LOGPIXELSY);
		ReleaseDC(NULL,dc);

		::SetViewportOrgEx(hPrintDC,scroll.x, scroll.y,&scroll);
		::SetWindowOrgEx(hPrintDC,0,0,NULL);
		//::SetMapMode(hPrintDC,MM_TEXT);
		POSITION pos = this->GetDrawObjects().GetHeadPosition();
		while (pos != NULL)
		{
			CDrawObj* pObj = (CDrawObj*)GetDrawObjects().GetNext(pos);
			pObj->Draw(hPrintDC);
		}*/

		EndPage(hPrintDC);
		return 1;
	}

	int CGridFrameSvr::CalcPrintPageAxis(int pageW,int pageH, pageAxis* &pPageAxis ,int &iPageCount)
	{
		int ACol,ARow;
		int AColStart,ARowStart;
		int cols = 1,rows = 1;
		int PageX,PageY;
		RECT	PrintRect;

		//this->GetValidRowCol(ACol,ARow);
		this->GetPrintArea(PrintRect);
		ACol = PrintRect.right;
		ARow = PrintRect.bottom;
		AColStart = PrintRect.left;
		ARowStart = PrintRect.top;

		pageH -= this->GetPrintHeaderHeight() + this->GetPrintFooterHeight();
		pageW -= this->GetPrintLeftWidth() + this->GetPrintRightWidth();
		PageX = pageW;

		std::vector<int> arRows;
		std::vector<int> arCols;
		arRows.push_back(1);
		arCols.push_back(1);
		int i;
		for(i=AColStart;i<=ACol;i++)
		{
			PageX -= GetGridDataSvr()->m_pAxisSvr->GetColWidth(i) +  this->FGridLineWidth;
			if(PageX < 0)
			{
				arCols.push_back(i);
				cols++;
				PageX = pageW - GetGridDataSvr()->m_pAxisSvr->GetColWidth(i)- this->FGridLineWidth;
			}
		}
		arCols.push_back(ACol);
		PageY = pageH;
		for(i=ARowStart;i<=ARow;i++)
		{
			PageY -= (GetGridDataSvr()->m_pAxisSvr->GetRowHeight(i)>0?GetRowHeight(i):0) +  this->FGridLineWidth;
			if(PageY < 0)
			{
				arRows.push_back(i);
				rows++;
				PageY = pageH - (GetGridDataSvr()->m_pAxisSvr->GetRowHeight(i)>0?GetRowHeight(i):0) - this->FGridLineWidth;
			}
		}
		arRows.push_back(ARow);

		pPageAxis = new pageAxis[(arRows.size() - 1) * (arCols.size() - 1)];

		int nPage = 1;
		for(i=0;i<(int)arRows.size() - 1; i++)
		{
			for(int j=0;j<(int)arCols.size() - 1;j++)
			{
				pPageAxis[i * (arCols.size() - 1) + j].iPage = nPage;
				nPage ++;
				pPageAxis[i * (arCols.size() - 1) + j].startRow = arRows[i];
				pPageAxis[i * (arCols.size() - 1) + j].endRow = arRows[i + 1];
				pPageAxis[i * (arCols.size() - 1) + j].startCol = arCols[j];
				pPageAxis[i * (arCols.size() - 1) + j].endCol = arCols[j + 1];
			}
		}

		iPageCount = ((int)arRows.size() - 1) * ((int)arCols.size() - 1);
		return iPageCount;
	}

	int CGridFrameSvr::GetPrintArea(RECT & PrintRect)
	{
		int ACol,ARow;
		PrintRect.left =  PrintRect.top = 1;

		this->GetGridDataSvr()->m_pContentSvr->GetValidRowCol(ACol,ARow);
		PrintRect.right = ACol;
		PrintRect.bottom = ARow;
		return 1;
	}
	int CGridFrameSvr::GetPrintHeaderHeight()
	{
		return 40;
	}
	int CGridFrameSvr::GetPrintFooterHeight()
	{
		return 40;
	}
	int CGridFrameSvr::GetPrintLeftWidth()
	{
		return 40;
	}
	int CGridFrameSvr::GetPrintRightWidth()
	{
		return 40;
	}
	
	bool CGridFrameSvr::IsEditQuery(int ACol,int ARow,bool &bShowButton)
	{
		bShowButton = false;

		PStrItem	pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(!pItem) return false;

		IXMLDOMElement * pElement = NULL;
		IXMLDOMNode * pNode;
		LPTSTR pEditType = NULL;
		_variant_t var;
		if(pItem && pItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"edittype",&var);
				if(var.bstrVal)
					pEditType = _tcsdup(var.bstrVal);
				pNode->Release();
			}
		}
		bool bQuery = false;
		if(pElement)
		{
			if(pEditType && _tcsicmp(pEditType,_T("query"))==0) bQuery = true;
			pElement->getAttribute(L"dddwshowbutton",&var);
			if(var.bstrVal && _tcsicmp(var.bstrVal,_T("是"))==0) bShowButton = true;

			pElement->Release();
		}
		return bQuery;
	}

	int CGridFrameSvr::ShowQueryDlg(LPTSTR pData)
	{
		bool bxpage = false;
		TCHAR buf[512];
		PStrItem pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(GetCol(),GetRow());
		if(pItem && pItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"dddwxpageurl",&var);
				if(var.bstrVal && var.bstrVal[0] !=0)
				{
					if(_tcsstr(var.bstrVal,_T("dev:xpage"))==var.bstrVal)
						::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),var.bstrVal);
					else
						::_stprintf_s(buf,512,_T("%s"),var.bstrVal);
					
					bxpage = true;
				}
				pElement->Release();
				pNode->Release();
			}
		}
		
		if(!bxpage)
		{
			XCellQueryDlg  * pWin = new XCellQueryDlg();
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
			xml.m_pXmlDoc->selectSingleNode(L"/xpages/xpage[@name='querywin']/xframe",&pNode);
			if(!pNode) return 1;
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
			pXPage->Load(pElement);
			pElement->Release();

			TGridDrawInfo GridInfo;
			InitDrawInfo(&GridInfo);

			RECT rc;
			GetCellRect(GridInfo,FCurrent.x,FCurrent.y,rc);

			POINT ptStart,ptEnd;
			ptStart.x = rc.left;
			ptStart.y = rc.top;
			ClientToScreen(GetHWND(),&ptStart);
			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;
			ClientToScreen(GetHWND(),&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);

			pWin->Open(pXPage,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,NULL,NULL,(LPVOID)this);
			::SetProp(pWin->GetHWND(),_T("this__"),this->m_pSheetSvr->m_pSheetCtrl->GetWin());
			pWin->OnOpenQuery(pData);
		}else
		{
			TGridDrawInfo GridInfo;
			InitDrawInfo(&GridInfo);

			RECT rc;
			GetCellRect(GridInfo,FCurrent.x,FCurrent.y,rc);

			POINT ptStart,ptEnd;
			ptStart.x = rc.left;
			ptStart.y = rc.top;
			ClientToScreen(GetHWND(),&ptStart);
			ptEnd.x = rc.right;
			ptEnd.y = rc.bottom;
			ClientToScreen(GetHWND(),&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
			
			CDwTable * pDwTable =  GetCurDwTable();
			IDwTable * pTable = NULL;
			if(pDwTable) pTable = (IDwTable *)pDwTable->GetInterface();
			this->HideEdit();
			xfWin* pWin = xfWin::LoadXPage(NULL,buf,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,NULL,NULL,(LPVOID)pTable);
			::SetProp(pWin->GetHWND(),_T("this__"),this->m_pSheetSvr->m_pSheetCtrl->GetWin());
			if(pData)
			{
				::SendMessage(pWin->GetHWND(), WM_XCOMMAND, (WPARAM)_T("xmEnter"), (LPARAM)pWin);
			}
		}
		return 1;
	}
	LPTSTR CGridFrameSvr::GetItemProp(int ACol,int ARow,LPTSTR pItem)
	{
		if(FOptions & goRunState)
		{
			CDwTable * pDwTable = GetGridDataSvr()->GetDwTableFromCell(ACol,ARow);
			if(pDwTable)
			{
				int nRow,nCol;
				pDwTable->getRowColumnFromCell(ACol,ARow,nRow,nCol);
				return GetColumnProp(pDwTable,nCol,pItem);
			}
		}
		PStrItem pStrItem = this->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(pStrItem && pStrItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pStrItem->pStrXml);
			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				_variant_t var;
				LPTSTR pItemStr = NULL;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pElement)
				{
					pElement->getAttribute(pItem,&var);
					pElement->Release();
					if(var.bstrVal)
					{
						//return var.bstrVal;
						pItemStr = _tcsdup(var.bstrVal);
						if(pItemStr[0]=='=')
						{
							CXGridComplier x;
							if(x.SetSource(pItemStr, this, 1, ARow, ACol))
								pItemStr = x.GetString();	
						}
					}
				}
				pNode->Release();
				return _tcsdup(pItemStr);
			}
		}
		return NULL;
	}
	LPTSTR CGridFrameSvr::GetColumnProp(CDwTable * pDwTable,int nCol,LPTSTR pItem)
	{
		return pDwTable->GetColumnProp(nCol,pItem);
	}
	LPTSTR CGridFrameSvr::GetColumnProp(CDwTable * pDwTable,LPTSTR pColName,LPTSTR pItem)
	{
		return pDwTable->GetColumnProp(pColName,pItem);
	}

	int CGridFrameSvr::SetReadOnly(BOOL bReadOnly)
	{
		if(!bReadOnly)
			FOptions |= goAllowEdit;
		else
			FOptions &= ~goAllowEdit;
		
		return 1;
			
	}

	LPCTSTR CGridFrameSvr::GetItemString(int Row,int Col)
	{
		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			CDwTable * pDwTable = GetGridDataSvr()->GetDwTableFromCell(Col, Row);
			if(pDwTable)
			{
				wstring path = pDwTable->m_strBoundGroup;
				CDwTable * pDwParent = pDwTable->m_pParent;
				while(pDwParent)
				{
					path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
					pDwParent = pDwParent->m_pParent;
				}
				int xRow;
				int xCol;
				pDwTable->getRowColumnFromCell(Col, Row, xRow, xCol);
				return m_pSheetSvr->m_pSheetCtrl->bindGetData((wchar_t*)path.c_str(), xRow, (wchar_t*)pDwTable->getColumnName(xCol));
			}
		}
		PStrItem pItem;
		pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(Col,Row);
		if(pItem && pItem->pStrContent)
		{
			return pItem->pStrContent;
		}
		return NULL;
	}

	LPCTSTR CGridFrameSvr::GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer)
	{
		if(m_pSheetSvr->m_pSheetCtrl->hasBindData())
		{
			CDwTable * pDwTable = GetGridDataSvr()->GetDwTableFromCell(nColumn, nRow);
			if(pDwTable)
			{
				wstring path = pDwTable->m_strBoundGroup;
				CDwTable * pDwParent = pDwTable->m_pParent;
				while(pDwParent)
				{
					path = pDwParent->m_strBoundGroup + (wstring)_T("/") + path;
					pDwParent = pDwParent->m_pParent;
				}
				int xRow;
				int xCol;
				pDwTable->getRowColumnFromCell(nColumn, nRow, xRow, xCol);
				wstring colname = (wstring)pDwTable->getColumnName(xCol) + (wstring)_T("/@_displaystring");
				return m_pSheetSvr->m_pSheetCtrl->bindGetData((wchar_t*)path.c_str(), xRow, (wchar_t*)colname.c_str());
			}
		}
		PStrItem pItem;
		pItem = GetGridDataSvr()->m_pContentSvr->GetItemData(nColumn,nRow);
		if(pItem && pItem->pStrText)
		{
			return pItem->pStrText;
		}
		return NULL;
	}

	void CGridFrameSvr::SetEditUpperMode(bool upper)
	{
		m_EditUpperMode = upper;
	}
	bool CGridFrameSvr::GetEditUpperMode()
	{
		return m_EditUpperMode;
	}


}}}}
