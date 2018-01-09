#include "stdafx.h"
#include "XCellSheetSvr.hpp"
#include "gridframesvr.hpp"
#include "colheadersvr.hpp"
#include "rowselectorsvr.hpp"
#include "xcellSerializeSvr.hpp"
#include "dwtable.hpp"
#include "XCellSheet.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XCellSheetSvr::XCellSheetSvr(void):crDirect(RGB(255,0,0)),crAdvance(RGB(128,0,0))
	{
		::SetRect(&FRect,0,0,0,0);

		m_ptOffset.x = 0;
		m_ptOffset.y = 0;

		m_pDataSvr = new CGridDataSvr();
		m_pSerializeSvr = new XCellSerializeSvr;
		m_pColHeaderSvr = new CColHeaderSvr();
		m_pSelectorSvr  = new CRowSelectorSvr();
		m_pGridFrameSvrs = new CGridFrameSvr();
		m_pGridFrameSvrs->m_pSheetSvr = this;

		m_pColHeaderSvr->m_pGridFrameSvr  = m_pGridFrameSvrs;
		m_pSelectorSvr->m_pGridFrameSvr  = m_pGridFrameSvrs;
		
		m_pColHeaderSvr->AddRouterMessageFrom(this);
		m_pSelectorSvr->AddRouterMessageFrom(this);
		m_pGridFrameSvrs->AddRouterMessageFrom(this);
	}


	XCellSheetSvr::~XCellSheetSvr(void)
	{
		if(m_pDataSvr) delete m_pDataSvr ;
		if(m_pColHeaderSvr)delete m_pColHeaderSvr;
		if(m_pSelectorSvr)delete m_pSelectorSvr;
		if(m_pGridFrameSvrs) delete m_pGridFrameSvrs;
		if(m_pSerializeSvr)delete m_pSerializeSvr;
	}

	int XCellSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{

		RECT rect;
		if(pDrawRect)
			rect = *pDrawRect;
		else
		{
			rect = FRect;
			//::SetRect(&rect,0,0,FRect.right - FRect.left,FRect.bottom - FRect.top);
		}

		DrawBackGround(hPaintDC,rect);

		if(!(FRect.right ==0 && FRect.bottom==0 && FRect.top==0 && FRect.left==0))
			DrawConer(hPaintDC,rect);
		RECT rt = rect;

		if(m_pSheetCtrl)
		{
			if(m_pSheetCtrl->m_pHScrollBar && (this->m_pGridFrameSvrs->FScrollBars & ssHorizontal) && m_pSheetCtrl->m_Standalone)
			{
				rect.bottom -= CXScrollBar::GetHScrollBarHeight();
			}
			if(m_pSheetCtrl->m_pVScrollBar && (this->m_pGridFrameSvrs->FScrollBars & ssVertical) && m_pSheetCtrl->m_Standalone)
			{
				rect.right  -= CXScrollBar::GetVScrollBarWidth();
			}
		}

		if(m_pGridFrameSvrs)
			m_pGridFrameSvrs->OnDraw(hPaintDC,rect);
		
		if(FRect.right ==0 && FRect.bottom==0 && FRect.top==0 && FRect.left==0)
			return 1;

		if(m_pColHeaderSvr)
			m_pColHeaderSvr->OnDraw(hPaintDC,rect);
		if(m_pSelectorSvr)
			m_pSelectorSvr->OnDraw(hPaintDC,rect);

		if(this->m_pGridFrameSvrs->FScrollBars ==ssNone)return 1;

		RECT rectex;
		if(m_pSheetCtrl && m_pGridFrameSvrs->FScrollBars & ssVertical && m_pSheetCtrl->m_pVScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rectex = rt;
			rectex.left = rt.right - CXScrollBar::GetVScrollBarWidth();
			rectex.bottom = rt.bottom - CXScrollBar::GetHScrollBarHeight();

			m_pSheetCtrl->m_pVScrollBar->FRect = rectex;
			m_pSheetCtrl->m_pVScrollBar->OnDraw(hPaintDC);
		}

		if(m_pSheetCtrl && m_pGridFrameSvrs->FScrollBars & ssHorizontal && m_pSheetCtrl->m_pHScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rectex = rt;
			rectex.top = rt.bottom - CXScrollBar::GetHScrollBarHeight();
			rectex.right = rt.right - CXScrollBar::GetVScrollBarWidth();

			m_pSheetCtrl->m_pHScrollBar->fnBar = SB_HORZ;
			m_pSheetCtrl->m_pHScrollBar->FRect = rectex;
			m_pSheetCtrl->m_pHScrollBar->OnDraw(hPaintDC);
		}

		return 1;
	}

	int XCellSheetSvr::SetFRect(RECT rect)
	{

		XOfficeSheetSvr::SetFRect(rect);
		RECT rt = rect;

		if(m_pSheetCtrl && m_pSheetCtrl->m_pVScrollBar && m_pGridFrameSvrs->FScrollBars & ssHorizontal && m_pSheetCtrl->m_Standalone)
		{
			rect.bottom -= CXScrollBar::GetHScrollBarHeight();
		}
		if(m_pSheetCtrl && m_pSheetCtrl->m_pVScrollBar && m_pGridFrameSvrs->FScrollBars & ssVertical && m_pSheetCtrl->m_Standalone)
		{
			rect.right  -= CXScrollBar::GetVScrollBarWidth();
		}

		RECT	rectex;
		rectex = rect;
		rectex.top	 = rect.top + this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0) + 2;
		rectex.right = rect.left + this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
		this->m_pSelectorSvr->FRect = rectex;

		rectex = rect;
		rectex.bottom	 = rect.top + this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0)  ;
		rectex.left = rect.left + this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0) + 2;
		this->m_pColHeaderSvr->FRect = rectex;

		rectex = rect;
		rectex.top	 = rect.top + this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0) + 3 ;
		rectex.left = rect.left + this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0) + 2; 
		this->m_pGridFrameSvrs->FRect = rectex;


		this->FRect = rt;

		return 1;
	}
	
	void XCellSheetSvr::DrawConer(HDC hdc,RECT &rect,COLORREF color)
	{
		//draw coner
		TRIVERTEX	vertex[4];
		GRADIENT_RECT grect[2];

		vertex[0].x = rect.left;
		vertex[0].y = rect.top ;
		//vertex[0].Red  = (GetRValue(color)>25? GetRValue(color) - 5:0)*256;
		//vertex[0].Green = (GetGValue(color)>25? GetGValue(color) - 5:0)*256;
		//vertex[0].Blue = (GetBValue(color)>25? GetBValue(color) - 5:0)*256;
		vertex[0].Red  =  GetRValue(color)*256;
		vertex[0].Green = GetGValue(color)*256;
		vertex[0].Blue =  GetBValue(color)*256;

		vertex[1].x = rect.left + this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
		vertex[1].y = rect.top + (this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0)>0?this->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0):0) ;
		//vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
		//vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
		//vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;
		vertex[1].Red  =  GetRValue(color)*256;
		vertex[1].Green = GetGValue(color)*256;
		vertex[1].Blue =  GetBValue(color)*256;

		grect[0].UpperLeft = 0;
		grect[0].LowerRight = 1;

		GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
	}

	void XCellSheetSvr::DrawBackGround(HDC hdc, RECT &rect)
	{
		if(!m_pSheetCtrl || ! m_pSheetCtrl->m_pxfNode) return ;
		xfNode * pNode = m_pSheetCtrl->m_pxfNode;
		LPCTSTR pStyle = pNode->GetCssStyle();

		Hxsoft::XUtil::Css::TBoxStyle* pBoxStyle = Hxsoft::XUtil::xuCCssStyle::ParseCssStyle(pStyle);
		if(pBoxStyle && (( pBoxStyle->mask & cssMaskBackGround && pBoxStyle->pBackGround)||(pBoxStyle->mask&cssMaskBorder && pBoxStyle->pBorder)))
		{
			Hxsoft::XUtil::Css::DrawCssFrame(hdc, &rect,pBoxStyle);
			delete pBoxStyle;
		}
		else
		{
			HBRUSH hBrush = ::CreateSolidBrush(RGB(225,229,238));
			::FillRect(hdc,&rect,hBrush);
			::DeleteObject(hBrush);
		}
	}

	int XCellSheetSvr::InitDrawInfo(TGridDrawInfo * pDrawInfo)
	{
		::memset(pDrawInfo,0,sizeof(TGridDrawInfo));

		//set HWND
		if(m_pSheetCtrl)
			pDrawInfo->hWnd = this->m_pSheetCtrl->m_hWnd;
		
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

		//pDrawInfo->pSheets = m_pTabSheet->m_pSheets;
		//pDrawInfo->SheetsSize = m_pTabSheet->m_SheetSize;
		//pDrawInfo->SheetCurrent = m_pTabSheet->m_iCurSheet;
		//pDrawInfo->SheetStart =  m_pTabSheet->m_iStartSheet;

		pDrawInfo->HSizeBarPos = -1;
		pDrawInfo->VSizeBarPos = -1;
		pDrawInfo->VScrollbarWidth = GetSystemMetrics(SM_CXVSCROLL);
		pDrawInfo->hScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);

		pDrawInfo->pGridSheet = this;
		pDrawInfo->pAxisSvr = pDrawInfo->pGridSheet->m_pGridFrameSvrs->GetGridDataSvr()->m_pAxisSvr;

		pDrawInfo->ColHeaderHeight = (pDrawInfo->pAxisSvr->GetRowHeight(0)>0?pDrawInfo->pAxisSvr->GetRowHeight(0):0);
		pDrawInfo->RowSelectorWidth = pDrawInfo->pAxisSvr->GetColWidth(0);

		pDrawInfo->FAnchor = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FAnchor;
		pDrawInfo->FCurrent = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FCurrent;
		
		pDrawInfo->EffectiveLineWidth =  pDrawInfo->pGridSheet->m_pGridFrameSvrs->FGridLineWidth;
		pDrawInfo->FGridState = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FGridState;
		pDrawInfo->FOptions = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FOptions;
		pDrawInfo->FSelections = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FSelections.GetBuffer();
		pDrawInfo->FSelectionSize = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FSelections.size();

		pDrawInfo->FColCount = pDrawInfo->pGridSheet->m_pGridFrameSvrs->GetColCount();
		pDrawInfo->FRowCount = pDrawInfo->pGridSheet->m_pGridFrameSvrs->GetRowCount();
		pDrawInfo->FTopLeft = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FTopLeft;
		pDrawInfo->FTopLefts = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FTopLefts;

		//pDrawInfo->FrameRect = m_AreaRect;
		pDrawInfo->GridRect = pDrawInfo->pGridSheet->m_pGridFrameSvrs->FRect;
		//pDrawInfo->GridSheetPageNavWidth = 4*GetSystemMetrics(SM_CYHSCROLL);
		//pDrawInfo->GridSheetPageHeight = GetSystemMetrics(SM_CYHSCROLL);
		//pDrawInfo->GridSheetPageWidth = (m_AreaRect.right - m_AreaRect.left) /2 -  pDrawInfo->GridSheetPageNavWidth;


		int Line = pDrawInfo->GridRect.left;
		int i;
		for(i=pDrawInfo->FTopLeft.x;i<=pDrawInfo->FColCount;i++)
		{
			Line += pDrawInfo->EffectiveLineWidth + pDrawInfo->pAxisSvr->GetColWidth(i);
			if(Line > pDrawInfo->GridRect.right)break;
			pDrawInfo->FullVisBoundaryCol = Line;
		}
        pDrawInfo->LastFullVisibleCol = i - 1;

		Line = pDrawInfo->GridRect.top;
		for(i=pDrawInfo->FTopLeft.y;i<=pDrawInfo->FRowCount;i++)
		{
			Line += pDrawInfo->EffectiveLineWidth + (pDrawInfo->pAxisSvr->GetRowHeight(i)>0?pDrawInfo->pAxisSvr->GetRowHeight(i):0);
			if(Line > pDrawInfo->GridRect.bottom)break;
			pDrawInfo->FullVisBoundaryRow = Line;
		}
        pDrawInfo->LastFullVisibleRow = i - 1;

		return 1;
	}

	HWND XCellSheetSvr::GetHWND()
	{
		return this->m_pSheetCtrl->GetHWND();
	}
	
	int XCellSheetSvr::initialAgent(bool bInit)
	{
		if(m_state & goRunState) return 1;
		if(!m_pSheetCtrl ||!m_pSheetCtrl->m_pxfNode) return 1;

		xfNode* pNode = m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->GetAgentNode();
		if(pNode)
		{
			XFrame::xfControl * pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("groupitem"),m_pSheetCtrl->m_pAgentNodeTool);
			if(!pControl)pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("groupitem"));
			if(pControl)
			{
				if(bInit)
				{
					::SendMessage(pControl->m_hWnd,CB_RESETCONTENT,0,0);
					for(int i=0;i< (int)m_pGridFrameSvrs->GetGridDataSvr()->FBandDwTables.size();i++)
					{
						::SendMessage(pControl->m_hWnd,CB_ADDSTRING,0,(LPARAM)m_pGridFrameSvrs->GetGridDataSvr()->FBandDwTables[i]->m_strBoundGroup);
					}
				}
				PStrItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(m_pGridFrameSvrs->GetCol(),m_pGridFrameSvrs->GetRow());
				if(pItem)
				{
					SetWindowText(pControl->m_hWnd,pItem->pStrGroup);
					pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("columnitem"),m_pSheetCtrl->m_pAgentNodeTool);
					if(!pControl)pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("columnitem"));
					SetWindowText(pControl->m_hWnd,pItem->pStrColumn);
				}else
				{
					pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("columnitem"),m_pSheetCtrl->m_pAgentNodeTool);
					if(!pControl)pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("columnitem"));
					SetWindowText(pControl->m_hWnd,NULL);
				}

				pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("datasource"),m_pSheetCtrl->m_pAgentNodeTool);
				if(!pControl)pControl = (XFrame::xfControl *)pNode->GetXPage()->m_pWin->GetControl(_T("datasource"));
				if(((CGridDataSvr*)m_pDataSvr)->m_pSchemaSource)
					SetWindowText(pControl->m_hWnd,((CGridDataSvr*)m_pDataSvr)->m_pSchemaSource);
				else
					SetWindowText(pControl->m_hWnd,NULL);
			}
		}
		return 1;
	}
	
	int XCellSheetSvr::TemplateLoaded(IXMLDOMElement * pElement,int state)
	{
		if(state & goRunState)
		{
			CGridDataSvr * pDataSvr = new CGridDataSvr();
			if(m_pSheetCtrl && m_pSheetCtrl->m_pxfNode)
			{
				this->m_pSerializeSvr->LoadSheetData(this->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl,pElement,pDataSvr);
			}else
				this->m_pSerializeSvr->LoadSheetData(NULL,pElement,pDataSvr);

			
			CGridDataSvr * pGridDataSvr = this->m_pGridFrameSvrs->GetGridDataSvr();
			pGridDataSvr->AddTemplData(pDataSvr,true);
			pGridDataSvr->CalcRptBands();
			for(int i=0;i<(int)pGridDataSvr->FBandDwTables.size();i++)
			{
				pGridDataSvr->FBandDwTables[i]->m_pGrid = this->m_pGridFrameSvrs;
				if(pGridDataSvr->FBandDwTables[i]->m_pParent)continue;
				pGridDataSvr->FBandDwTables[i]->insertGroup(1);
			}
			this->m_pGridFrameSvrs->ShowGrid(false);
			this->m_pGridFrameSvrs->SetRunState(goRunState);
			this->m_state = goRunState;
		}else
		{
			m_pGridFrameSvrs->GetGridDataSvr()->AddTemplData(m_pGridFrameSvrs->GetGridDataSvr(),false);
			m_pGridFrameSvrs->GetGridDataSvr()->CalcRptBands();
		}
		return 1;
	}

	int XCellSheetSvr::SetRowSelectorWidth(int nWidth)
	{
			CGridDataSvr * pDataSvr =m_pGridFrameSvrs->GetGridDataSvr();
			int width = pDataSvr->m_pAxisSvr->GetColWidth(0);
			pDataSvr->m_pAxisSvr->SetColWidth(0,nWidth);

			this->SetFRect(FRect);
	
			return width;
	}
	int XCellSheetSvr::ResetRowSelector()
	{
			CGridDataSvr * pDataSvr =m_pGridFrameSvrs->GetGridDataSvr();
			int width = pDataSvr->m_pAxisSvr->GetColWidth(0);
			pDataSvr->m_pAxisSvr->SetColWidth(0,width);

			this->SetFRect(FRect);

			return width;
	}
	int XCellSheetSvr::GetRowSelectorWidth()
	{
			CGridDataSvr * pDataSvr =m_pGridFrameSvrs->GetGridDataSvr();
		return pDataSvr->m_pAxisSvr->GetColWidth(0);
	}
	int XCellSheetSvr::GetRowSelectorDefaultWidth()
	{
		return COptionSvr::stdRowSelectorWidth;
	}

	int XCellSheetSvr::SetColHeaderHeight(int nHeight)
	{
		CGridDataSvr * pDataSvr =m_pGridFrameSvrs->GetGridDataSvr();
		int Height = pDataSvr->m_pAxisSvr->GetRowHeight(0);
		pDataSvr->m_pAxisSvr->SetRowHeight(0,nHeight);

		this->SetFRect(FRect);
		return Height;
	}
	int XCellSheetSvr::GetColHeaderDefaultHeight()
	{
		return COptionSvr::stdColHeaderHeight;
	}
	int XCellSheetSvr::ResetColHeaderHeight()
	{
		CGridDataSvr * pDataSvr =m_pGridFrameSvrs->GetGridDataSvr();
		int Height = pDataSvr->m_pAxisSvr->GetRowHeight(0);
		pDataSvr->m_pAxisSvr->SetRowHeight(0,Height);
		this->SetFRect(FRect);
		return Height;
	}
	int XCellSheetSvr::GetColHeaderHeight()
	{
		CGridDataSvr * pDataSvr =m_pGridFrameSvrs->GetGridDataSvr();
		return pDataSvr->m_pAxisSvr->GetRowHeight(COptionSvr::stdColHeaderHeight);
	}

	POINT XCellSheetSvr::GetOffset()
	{
		return m_ptOffset;
	}

	bool XCellSheetSvr::SetOffset(POINT &pt)
	{
		m_ptOffset = pt;
		return true;
	}

	LPCTSTR	XCellSheetSvr::GetNoteString(int ACol,int ARow)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		return (pItem && pItem->pNote)?pItem->pNote:NULL;
	}
	LPCTSTR	XCellSheetSvr::GetOriginString(int ACol,int ARow)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		return (pItem && pItem->pOrigin)?pItem->pOrigin:NULL;
	}
	LPCTSTR	XCellSheetSvr::GetAdvanceString(int ACol,int ARow)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		return (pItem && pItem->pStrAdvance)?pItem->pStrAdvance:NULL;
	}
	
	void	XCellSheetSvr::SetNoteString(int ACol,int ARow,LPTSTR pStr)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(!pItem)
		{
			pItem = new CContentSvr::TContentItem;
			m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->SetItemData(ACol,ARow,pItem);
		}
		if(pItem->pNote)delete pItem->pNote;
		if(pStr)
			pItem->pNote = _tcsdup(pStr);
		else
			pItem->pNote = NULL;
	}
	void	XCellSheetSvr::SetOriginString(int ACol,int ARow,LPTSTR pStr)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(!pItem)
		{
			pItem = new CContentSvr::TContentItem;
			m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->SetItemData(ACol,ARow,pItem);
		}
		pItem->bDirect = true;
		if(pItem->pOrigin)delete pItem->pOrigin;
		if(pStr)
			pItem->pOrigin = _tcsdup(pStr);
		else
			pItem->pOrigin = NULL;
	}
	void	XCellSheetSvr::SetAdvanceString(int ACol,int ARow,LPTSTR pStr)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(!pItem)
		{
			pItem = new CContentSvr::TContentItem;
			m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->SetItemData(ACol,ARow,pItem);
		}
		pItem->bAdvance = true;
		if(pItem->pStrAdvance)delete pItem->pStrAdvance;
		if(pStr)
			pItem->pStrAdvance = _tcsdup(pStr);
		else
			pItem->pStrAdvance = NULL;

	}

	void	XCellSheetSvr::ClearNoteString(int ACol,int ARow)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(pItem && pItem->pNote)
		{
			delete pItem->pNote;
			pItem->pNote = NULL;
		}
	}
	void	XCellSheetSvr::ClearOriginString(int ACol,int ARow)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(pItem && pItem->pOrigin)
		{
			delete pItem->pOrigin;
			pItem->pOrigin = NULL;
		}
	}
	void	XCellSheetSvr::ClearAdvanceString(int ACol,int ARow)
	{
		CContentSvr::LPContentItem pItem = m_pGridFrameSvrs->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(pItem && pItem->pOrigin)
		{
			delete pItem->pStrAdvance;
			pItem->pStrAdvance = NULL;
		}
	}
}}}}
