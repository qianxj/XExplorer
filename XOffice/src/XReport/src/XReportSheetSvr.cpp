#include "StdAfx.h"
#include "..\include\XReportSheetSvr.hpp"
#include "xreportserializesvr.hpp"
#include "xframe.h"
#include "XReportSheetSvr.hpp"
#include "IReportEvent.hpp"
#include "IReportObserver.hpp"
#include "IXReport.hpp"

#define	DWV_DOUBLECLICKED	 0x0008
#define DWV_RCLICKED		 0x0019
#define	DWV_CLICKED			 0x0009

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	XReportSheetSvr::XReportSheetSvr(void):m_bTreeDoubleClick(false),m_SelectBkColor(RGB(128,128,255))
	{
		this->Initialize();

		m_pSerializeSvr = new XReportSerializeSvr;
		m_pDataSvr = new XReportDataSvr;


		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		pDataSvr->m_pAixsSvr = new XReportAxisSvr;
		pDataSvr->m_pRowSelectorSvr = new XRowSelectorSvr ;
		pDataSvr->m_pRowSelectorSvr->m_pAxisSvr = pDataSvr->m_pAixsSvr;
		pDataSvr->m_pRowSelectorSvr->m_pSheetSvr = this;
		pDataSvr->m_pRightRowSelectorSvr = new XRowSelectorSvr ;
		pDataSvr->m_pRightRowSelectorSvr->m_pAxisSvr = pDataSvr->m_pAixsSvr;
		pDataSvr->m_pRightRowSelectorSvr->m_pSheetSvr = this;
		pDataSvr->m_pRightRowSelectorSvr->m_nWidth = 5;
		pDataSvr->m_pRightRowSelectorSvr->m_bDrawRowNo = false;
		SetEventListons();

		m_pEditSvr = new CEditSvr;
		m_pEditSvr->m_pGetCellContent = &XReportSheetSvr::GetCellContent;
		m_pEditSvr->m_pSetCellContent = &XReportSheetSvr::SetCellContent;
		m_pEditSvr->m_pGetCellRect = &XReportSheetSvr::GetCellRect;
		m_pEditSvr->m_pGetEditType = &XReportSheetSvr::GetEditType;
		m_pEditSvr->m_pOwner =this;
		m_pEditSvr->m_pGetFont =  &XReportSheetSvr::GetEditFont;

		m_pRowSelects = new XUtil::xuCsArray< int, BYTE>( 1<<30,0);
		m_nSelectionLastRow = 0;
		m_SelectionMode = 0;

	}

	XReportSheetSvr::~XReportSheetSvr(void)
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
		if(m_pRowSelects) delete m_pRowSelects;
		UnSetEventListons();
	}

	bool XReportSheetSvr::SetEventListons()
	{
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XReportSheetSvr::EvtSetFocus);
		this->AttachListon(WM_KILLFOCUS,(FNEvent)&XReportSheetSvr::EvtKillFocus);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XReportSheetSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XReportSheetSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XReportSheetSvr::EvtMouseMove);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XReportSheetSvr::EvtKeyDown);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XReportSheetSvr::EvtSetCursor);

	
		this->AttachListon(WM_COMMAND,(FNEvent)&XReportSheetSvr::EvtCommand);

		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XReportSheetSvr::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&XReportSheetSvr::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&XReportSheetSvr::EvtHScroll);

		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&XReportSheetSvr::EvtDoubleClicked);
		this->AttachListon(WM_RBUTTONDOWN,(FNEvent)&XReportSheetSvr::EvtRButtonDown);

		this->AddRouterMessageTo(this->GetReportDataSvr()->m_pRowSelectorSvr);
		this->AddRouterMessageTo(this->GetReportDataSvr()->m_pRightRowSelectorSvr);

		return true;
	}

	bool XReportSheetSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}


	bool XReportSheetSvr::EvtCommand(TEvent * pEvent, LPARAM lParam)
	{
		if(HIWORD(pEvent->wParam)==CBN_KILLFOCUS)
			this->HideEdit();
		else if(HIWORD(pEvent->wParam)==EN_KILLFOCUS)
			this->HideEdit();
		return true;
	}

	bool XReportSheetSvr::EvtMouseWheel(TEvent * pEvent, LPARAM lParam)
	{
		if(pEvent->wheel.zDelta < 0)
			if(this->FTopLeft.y <this->GetRowCount()) FTopLeft.y++;
		if(pEvent->wheel.zDelta > 0)
			if(this->FTopLeft.y > 1) FTopLeft.y--;
		::InvalidateRect(GetHWND(),&FRect,false);
		return true;
	}
	bool XReportSheetSvr::EvtHScroll(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}
	bool XReportSheetSvr::EvtVScroll(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	void XReportSheetSvr::Initialize()
	{
		FCurrent.x = 1;
		FCurrent.y = 1;
		FTopLeft.x = 1;
		FTopLeft.y = 1;

		FAnchor.x = 1;
		FAnchor.y = 1;

		HandleAllocated = true;
		FEditorMode = false;

		FCanEditModify = true;
		FInplaceCol = -1;
		FInplaceRow = -1;


		FGridLineWidth = 1;
		FOptions = /*goRowSelect|*/goEditing|goThumbTracking|/*goAllowEdit|*/goFixedVertLine|goFixedHorzLine|goVertLine|goHorzLine|goRangeSelect|goColSizing|goRowSizing|goDrawFixed|goDrawBound/*|goAlwaysShowEditor*/;
		FOptions = FOptions|goRowMoving|goColMoving;
		FScrollBars = ssBoth;
		FBorderStyle = bsSingle;
		FGridState = gsNormal;
	}


	int XReportSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRec)
	{
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pHeaderSvr) return 0;

		RECT rc;
		if(pDrawRec)
			rc = *pDrawRec;
		else
			rc = FRect;
		
		HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
		::FillRect(hPaintDC,&rc,hBrush);
		::DeleteObject(hBrush);

		if(pDataSvr->FOption & troRowSelector || pDataSvr->FOption & troRowSelectorEx)
		{
			pDataSvr->m_pRowSelectorSvr->m_pAxisSvr->m_pRowHeights->SetValue(0,3200*3200,pDataSvr->m_pDetailSvr->m_nHeight);
			pDataSvr->m_pRowSelectorSvr->OnDraw(hPaintDC,pDataSvr->m_pRowSelectorSvr->FRect);
		}

		if(pDataSvr->FOption & troRightSelector)
		{
			RECT rect = pDataSvr->m_pRightRowSelectorSvr->FRect;
			pDataSvr->m_pRightRowSelectorSvr->OnDraw(hPaintDC,rect);
		}

		RECT rcHeader=rc;
		GetDwRect(&rcHeader);
		if(pDataSvr->FOption & troRowSelector || pDataSvr->FOption & troRowSelectorEx)
		{
			RECT cRect;
			::SetRect(&cRect,FRect.left,FRect.top,pDataSvr->m_pRowSelectorSvr->FRect.right, pDataSvr->m_pRowSelectorSvr->FRect.top);
			pDataSvr->m_pRowSelectorSvr->DrawCorner(hPaintDC,cRect);
		}
		if(pDataSvr->FOption & troRightSelector)
		{
			RECT cRect;
			::SetRect(&cRect,pDataSvr->m_pRightRowSelectorSvr->FRect.left,
				FRect.top,pDataSvr->m_pRightRowSelectorSvr->FRect.right, pDataSvr->m_pRightRowSelectorSvr->FRect.top);
			pDataSvr->m_pRightRowSelectorSvr->DrawCorner(hPaintDC,cRect);
		}

		rcHeader.right = FRect.right;
		pDataSvr->m_pHeaderSvr->OnDraw(hPaintDC,rcHeader);
		GetDwRect(&rcHeader);

		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(0xce,0xce,0xce));
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);

		int hHeight = pDataSvr->m_pHeaderSvr->GetTotalLevel()*pDataSvr->m_pHeaderSvr->m_nRowHeight;
		::MoveToEx(hPaintDC,FRect.left,rcHeader.top + hHeight,NULL);
		::LineTo(hPaintDC,FRect.right,rcHeader.top + hHeight);
		hPen = (HPEN)::SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		
		rcHeader.top = rcHeader.top + hHeight + 1;
		pDataSvr->m_pDetailSvr->OnDraw(hPaintDC,rcHeader,this,FTopLeft.y);

		return 0;
	}
	int XReportSheetSvr::SetFRect(RECT rc)
	{
		XOfficeSheetSvr::SetFRect(rc);
		RECT rect;
		if(GetReportDataSvr()->m_pHeaderSvr)
		{
			int hHeight = GetReportDataSvr()->m_pHeaderSvr->GetTotalLevel()*GetReportDataSvr()->m_pHeaderSvr->m_nRowHeight;
			GetReportDataSvr()->m_pAixsSvr->SetRowHeight(0,hHeight);
			if(GetReportDataSvr()->FOption & troRowSelector || this->GetReportDataSvr()->FOption & troRowSelectorEx)
			{
				rect = FRect;
				rect.top += GetReportDataSvr()->m_pAixsSvr->GetRowHeight(0) + 2;
				rect.right = FRect.left + GetReportDataSvr()->m_pAixsSvr->GetColWidth(0);
				GetReportDataSvr()->m_pRowSelectorSvr->FRect = rect;
			}
			if(GetReportDataSvr()->FOption & troRowSelector || this->GetReportDataSvr()->FOption & troRowSelectorEx)
			{
				rect = FRect;
				rect.top += GetReportDataSvr()->m_pAixsSvr->GetRowHeight(0) + 2;
				rect.left = rect.right - GetReportDataSvr()->m_pRightRowSelectorSvr->m_nWidth - 1;
				GetReportDataSvr()->m_pRightRowSelectorSvr->FRect = rect; 
			}
		}

		return 0;
	}

	TGridState XReportSheetSvr::CalcSizingState(POINT point,RECT &rect)
	{
		int ACol;
		int nPos;
		return CalcSizingState(point,rect,ACol,nPos);
	}

	TGridState XReportSheetSvr::CalcSizingState(POINT point,RECT &rect,int &ACol, int &nPos)
	{
		int startCell =FTopLeft.x;
		int LineX;
		int EndX;

		LineX = rect.left + 1;
		EndX = rect.right;

		if(point.y < rect.top || point.y > rect.bottom) return gsNormal;
		if(point.x < LineX || point.x > EndX) return gsNormal;

		ACol = startCell;
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		while(LineX <= EndX )
		{
			int i=0;
			if(ACol <= pDataSvr->m_pHeaderSvr->m_HeaderItems.size())
			{
				LineX += pDataSvr->m_pHeaderSvr->m_HeaderItems[ACol - 1]->m_nWidth  + 1;
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

	void XReportSheetSvr::DrawSizingLine(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,1,RGB(0,0,0));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		if(FGridState == gsColSizing)
		{
			::MoveToEx(hDC,pt.x, 0,NULL);
			::LineTo(hDC,pt.x, FRect.bottom );
		}

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	bool XReportSheetSvr::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pDetailSvr) return true;

		switch(nChar)
		{
			case 'M':
				if(::GetKeyState(VK_CONTROL)<0)
				{
					ImportClipBoard();
					return true;
				}

			case VK_UP:
				{
					if(pDataSvr->m_pDetailSvr->m_nCurRow>1)
					{
						pDataSvr->m_pDetailSvr->m_nCurRow--;
						this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow + 1);
					}
					::InvalidateRect(this->GetHWND(),&FRect,false);
					break;
				}
			case VK_DOWN:
				{
					if(pDataSvr->m_pDetailSvr->m_nCurRow < pDataSvr->m_pContentSvr->GetRowCount())
					{
						pDataSvr->m_pDetailSvr->m_nCurRow++;
						this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow - 1);
					}
					::InvalidateRect(this->GetHWND(),&FRect,false);
					break;
				}
				break;
			case VK_TAB:
				{
					if(!(::GetKeyState(VK_SHIFT)<0))
					{
							if(pDataSvr->m_pDetailSvr->m_nCurCol < pDataSvr->m_pContentSvr->GetColCount())
						{
							pDataSvr->m_pDetailSvr->m_nCurCol++;
							//this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow + 1);
							
							FCurrent.x = pDataSvr->m_pDetailSvr->m_nCurCol;
							FCurrent.y = pDataSvr->m_pDetailSvr->m_nCurRow;
							::InvalidateRect(this->GetHWND(),&FRect,false);

						}else if(pDataSvr->m_pDetailSvr->m_nCurRow < pDataSvr->m_pContentSvr->GetRowCount())
						{
							pDataSvr->m_pDetailSvr->m_nCurCol = 1;
							pDataSvr->m_pDetailSvr->m_nCurRow++;
							this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow - 1);

							FCurrent.x = pDataSvr->m_pDetailSvr->m_nCurCol;
							FCurrent.y = pDataSvr->m_pDetailSvr->m_nCurRow;
							::InvalidateRect(this->GetHWND(),&FRect,false);
						}
					}else
					{
						if(pDataSvr->m_pDetailSvr->m_nCurCol > 1)
						{
							pDataSvr->m_pDetailSvr->m_nCurCol--;
							//this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow + 1);
							
							FCurrent.x = pDataSvr->m_pDetailSvr->m_nCurCol;
							FCurrent.y = pDataSvr->m_pDetailSvr->m_nCurRow;
							::InvalidateRect(this->GetHWND(),&FRect,false);

						}else if(pDataSvr->m_pDetailSvr->m_nCurRow > 1)
						{
							pDataSvr->m_pDetailSvr->m_nCurCol = pDataSvr->m_pContentSvr->GetColCount() ;
							FCurrent.x = pDataSvr->m_pDetailSvr->m_nCurCol;
							pDataSvr->m_pDetailSvr->m_nCurRow--;
							this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow - 1);

							FCurrent.x = pDataSvr->m_pDetailSvr->m_nCurCol;
							FCurrent.y = pDataSvr->m_pDetailSvr->m_nCurRow;
							::InvalidateRect(this->GetHWND(),&FRect,false);
						}

					}
					break;
				}
			case VK_LEFT:
				{
					if(pDataSvr->m_pDetailSvr->m_nCurCol > 1)
					{
						pDataSvr->m_pDetailSvr->m_nCurCol--;
						//this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow + 1);
					}
					::InvalidateRect(this->GetHWND(),&FRect,false);
					break;
				}
			case VK_RIGHT:
				{
					if(pDataSvr->m_pDetailSvr->m_nCurCol < pDataSvr->m_pContentSvr->GetColCount())
					{
						pDataSvr->m_pDetailSvr->m_nCurCol++;
						//this->OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,pDataSvr->m_pDetailSvr->m_nCurRow - 1);
					}
					::InvalidateRect(this->GetHWND(),&FRect,false);
					break;
				}
				break;
		}
		return true;
	}

	bool XReportSheetSvr::EvtKillFocus(TEvent * pEvent, LPARAM lParam)
	{
		HWND hFocusWnd = (HWND)pEvent->wParam;
		if(GetParent(hFocusWnd) == GetHWND()) return true;
		HideEdit();
		return true;
	}

	bool XReportSheetSvr::EvtSetFocus(TEvent * pEvent, LPARAM lParam)
	{
		if((HWND)pEvent->wParam != this->GetHWND())
		{
			XReportDataSvr * pReportDataSvr = (XReportDataSvr *)this->m_pDataSvr;
			if(!pReportDataSvr->m_pQueryXml && pReportDataSvr->m_pQueryUrl)
			{
				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombineW(m_pSheetCtrl->m_pxfNode->GetXPage()->m_pStrUrl,pReportDataSvr->m_pQueryUrl,strUrl,&dwSize,0);
				
				int len;
				LPBYTE pData;
				if(xfUriLoader::Load(strUrl,NULL,pData,len,false)==1)
				{
					LPBYTE pStr = new BYTE[len +sizeof(TCHAR)];
					::memcpy_s(pStr,len +sizeof(TCHAR),pData,len);
					pStr[len]=0;pStr[len + 1]=0;
					xfUriLoader::FreeData(pData);
					pReportDataSvr->m_pQueryXml = (LPTSTR)pStr;
				}
			}
			xfNode* pNode = m_pSheetCtrl->GetWin()->m_pXPage->GetAgentNode();
			if(pNode && pReportDataSvr->m_pQueryXml && pNode->GetXPage()->m_pFocusNodeControl !=dynamic_cast<xfNodeControl *>(m_pSheetCtrl->m_pxfNode))
			{
				pNode->GetXPage()->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(m_pSheetCtrl->m_pxfNode);
				
				xbXml xml;
				xml.LoadXml(pReportDataSvr->m_pQueryXml);
				IXMLDOMElement * pElement;
				xml.m_pXmlDoc->get_documentElement(&pElement);
				if(pElement)
				{
					 m_pSheetCtrl->m_pxfNode->GetXPage()->SetAgentNodeContent(pNode,pElement,m_pSheetCtrl->m_pxfNode);
					pElement->Release();
				}
				m_pSheetCtrl->GetWin()->m_pXPage->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(m_pSheetCtrl->m_pxfNode);
			}
		}
		return true;
	}

	bool XReportSheetSvr::EvtDoubleClicked(TEvent * pEvent, LPARAM lParam)
	{
		SetFocus(m_pSheetCtrl->GetHWND());

		POINT point = pEvent->mouse.point;
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pHeaderSvr) return true;

		int hHeight = pDataSvr->m_pHeaderSvr->GetTotalLevel()*pDataSvr->m_pHeaderSvr->m_nRowHeight;
		if(point.y - hHeight < 0) return true;
		point.y -= hHeight;
		int y;
		int nRow = pDataSvr->m_pDetailSvr->GetRowAtPoint(point,y) + FTopLeft.y - 1;
		if(nRow > 0)
		{
			this->OnDoubleClicked(nRow);
			if(!m_bTreeDoubleClick) return true;
			vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
			if(((XXmlContentSvr *)((XReportDataSvr *)m_pDataSvr)->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
			{
				int nIndex = XXmlContentSvr::GetTreeIndex(pTreeItems,nRow);
				if(nIndex < 0) false;
				if((*pTreeItems)[nIndex].children)
				{
					(*pTreeItems)[nIndex].expanded = ! (*pTreeItems)[nIndex].expanded;
					::InvalidateRect(this->GetHWND(),&FRect,false);
				}
			}
		}

		return true;
	}

	bool XReportSheetSvr::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		SetFocus(m_pSheetCtrl->GetHWND());
		POINT point = pEvent->mouse.point;
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pHeaderSvr) return true;

		int hHeight = pDataSvr->m_pHeaderSvr->GetTotalLevel()*pDataSvr->m_pHeaderSvr->m_nRowHeight;
		if(point.y - hHeight < 0)
		{
			RECT	tRect;
			this->GetDwRect(&tRect);

			int ACol;
			int nPos;
			tRect.bottom = tRect.top + hHeight;
			FGridState = CalcSizingState(point,tRect,ACol,nPos);
			if(FGridState==gsColSizing)
			{
				m_nSCol = ACol;
				m_nSPos = nPos;
				DrawSizingLine(point);
				this->m_ptMouse = point;
				this->SetCapture();
			}
			return true;
		}else
		{
			point.y -= hHeight;
			int y,x;
			int nRow = pDataSvr->m_pDetailSvr->GetRowAtPoint(point,y) + FTopLeft.y - 1;
			int nCol = pDataSvr->m_pDetailSvr->GetColAtPoint(point,x) + FTopLeft.x - 1;
			if(goEditing & FOptions && goAllowEdit & FOptions)
			{
				if((nCol == FCurrent.x) && (nRow == FCurrent.y))
				{
					ShowEditor();
				}else if(FEditorMode)
				{
					HideEditor();
				}
			}

			if(nRow > 0)
			{
				vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
				if(((XXmlContentSvr *)((XReportDataSvr *)m_pDataSvr)->m_pContentSvr)->GetTreeData(pTreeItems)>=0)
				{
					int nIndex = XXmlContentSvr::GetTreeIndex(pTreeItems,nRow);
					if(nIndex >= 0)
					{
						if((*pTreeItems)[nIndex].children)
						{
							RECT rect;
							this->GetDwRect(&rect);
							if(y > pDataSvr->m_pDetailSvr->m_nHeight/2 - 5 && y < pDataSvr->m_pDetailSvr->m_nHeight/2 + 5)
							{
								int px = rect.left + 7 + ((*pTreeItems)[nIndex].nLevel - 2)*16 - 3;
								if(point.x >=px && point.x <=px + 12) //hited
								{
									(*pTreeItems)[nIndex].expanded = !(*pTreeItems)[nIndex].expanded;
									::InvalidateRect(this->GetHWND(),&FRect,false);
									return true;
								}
							}
						}
					}
				}
			}
			if(nRow > 0 )
			{
				if(nRow != pDataSvr->m_pDetailSvr->m_nCurRow)
				{
					OnRowFocusChange(pDataSvr->m_pDetailSvr->m_nCurRow,nRow);
					pDataSvr->m_pDetailSvr->m_nCurRow = nRow;

					if(m_SelectionMode == 1)//single
					{
						this->SelectRow(0,false);
						this->SelectRow(nRow,true);
					}
					m_nSelectionLastRow = nRow;
					if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
				}
				if(nRow > 0 && nRow <= GetRowCount())
				{
					if(m_SelectionMode == 2)
					{
						//this->SelectRow(0,false);
						if(pEvent->mouse.nFlags & MK_SHIFT)
						{
							if(m_nSelectionLastRow > 0)
								this->SelectRow(m_nSelectionLastRow,nRow,true);
							else
								this->SelectRow(nRow,true);
						}else if(pEvent->mouse.nFlags & MK_CONTROL)
						{
							this->SelectRow(nRow,!IsRowSelected(nRow));
						}else
							this->SelectRow(nRow,true);
						m_nSelectionLastRow = nRow;
						if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
					}else if(m_SelectionMode == 3)
					{
						//this->SelectRow(0,false);
						if(pEvent->mouse.nFlags & MK_SHIFT)
						{
							if(m_nSelectionLastRow > 0)
								this->SelectRow(m_nSelectionLastRow,nRow,true);
							else
								this->SelectRow(nRow,true);
						}else 
						{
							this->SelectRow(nRow,!IsRowSelected(nRow));
						}
						m_nSelectionLastRow = nRow;
						if(this->GetHWND())::InvalidateRect(this->GetHWND(),&FRect,false);
					}
				}

				int x=0;
				int nCol = pDataSvr->m_pDetailSvr->GetColAtPoint(point,x);
				if(nCol >0)
				{
					FCurrent.x = nCol;
					pDataSvr->m_pDetailSvr->m_nCurCol = nCol;
					if(pDataSvr->m_pDetailSvr->m_DetailItems[nCol - 1]->m_nIndexEditor>=0)
					{
						int row = pDataSvr->m_pContentSvr->GetContentRow(nRow);
						LPTSTR pStr = (*pDataSvr->m_pEditor)[pDataSvr->m_pDetailSvr->m_DetailItems[nCol - 1]->m_nIndexEditor];
						LPTSTR pStart = NULL;
						TCHAR buf[255];
						TCHAR buf1[255];
						int npos = 0;
						int ipos = 0;
						if(_tcsstr(pStr,_T("editor:checkbox")) && pDataSvr->m_pDetailSvr->m_DetailItems[nCol - 1]->colid > 0)
						{
							LPCTSTR pValue = pDataSvr->m_pContentSvr->GetItemString(pDataSvr->m_pDetailSvr->m_DetailItems[nCol -1]->colid,row);
							pStart = _tcsstr(pStr,_T("editor-checkbox-checked:"));
							if(pStart)
							{
								pStart += _tcslen(_T("editor-checkbox-checked:"));
								while(pStart[npos] && (pStart[npos]==' ' || pStart[npos]=='\t' ||pStart[npos]=='\r' ||pStart[npos]=='\n'))npos++;
								while(pStart[npos] && pStart[npos]!=';')
								{
									if(ipos < 253)buf[ipos++] = pStart[npos++];
								}
								buf[ipos++]=0;
							}

							npos =0;
							ipos =0;
							buf1[0] = 0;
							pStart = _tcsstr(pStr,_T("editor-checkbox-unchecked:"));
							if(pStart)
							{
								pStart += _tcslen(_T("editor-checkbox-unchecked:"));
								while(pStart[npos] && (pStart[npos]==' ' || pStart[npos]=='\t' ||pStart[npos]=='\r' ||pStart[npos]=='\n'))npos++;
								while(pStart[npos] && pStart[npos]!=';')
								{
									if(ipos < 253)buf1[ipos++] = pStart[npos++];
								}
								buf1[ipos++]=0;
							}
							if(buf[0]==0)buf[0]='Y';
							if(buf1[0]==0)buf1[0]='N';
							
							if(pValue && _tcsicmp(buf,pValue)==0)
							{
								pDataSvr->m_pContentSvr->SetItemString(pDataSvr->m_pDetailSvr->m_DetailItems[nCol -1]->colid,row,buf1);
							}else
							{
								pDataSvr->m_pContentSvr->SetItemString(pDataSvr->m_pDetailSvr->m_DetailItems[nCol -1]->colid,row,buf);
							}
							//if(pValue)delete pValue;
						}
					}
				}
				::InvalidateRect(this->GetHWND(),&FRect,false);
				if(nRow > 0 && nCol > 0)
				{
					OnClicked(nRow,nCol);
				}

			}
		}
		return true;
	}

	bool XReportSheetSvr::EvtRButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		SetFocus(m_pSheetCtrl->GetHWND());
		POINT point = pEvent->mouse.point;
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pHeaderSvr) return true;

		int hHeight = pDataSvr->m_pHeaderSvr->GetTotalLevel()*pDataSvr->m_pHeaderSvr->m_nRowHeight;
		if(point.y - hHeight >=0)
		{
			point.y -= hHeight;
			int y,x;
			int nRow = pDataSvr->m_pDetailSvr->GetRowAtPoint(point,y) + FTopLeft.y - 1;
			int nCol = pDataSvr->m_pDetailSvr->GetColAtPoint(point,x) + FTopLeft.x - 1;
			if(goEditing & FOptions && goAllowEdit & FOptions)
			{
				HideEditor();
			}
			if(nRow > 0) OnRButtonDown(nRow,pEvent);
			if(nRow > 0 )
			{
				if(nRow != pDataSvr->m_pDetailSvr->m_nCurRow)
				{
					int nOldRow = pDataSvr->m_pDetailSvr->m_nCurRow;
					pDataSvr->m_pDetailSvr->m_nCurRow = nRow;
					OnRowFocusChange(nOldRow,nRow);
					::InvalidateRect(GetHWND(),NULL,FALSE);
				}
			}
		}
		return true;
	}

	bool XReportSheetSvr::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pHeaderSvr) return true;
		if(FGridState == gsColSizing)
		{
			DrawSizingLine(point);
			FGridState = gsNormal;
			int width = pDataSvr->m_pHeaderSvr->m_HeaderItems[m_nSCol - 1]->m_nWidth  + 1;
			width += point.x - m_nSPos;
			if(width > 0) 
			{
				if(pDataSvr->m_pHeaderSvr->m_HeaderItems.size()> m_nSCol - 1) pDataSvr->m_pHeaderSvr->m_HeaderItems[m_nSCol - 1]->m_nWidth =  width;
				if(pDataSvr->m_pDetailSvr->m_DetailItems.size()> m_nSCol - 1) pDataSvr->m_pDetailSvr->m_DetailItems[m_nSCol - 1]->m_nWidth = width;
			}
			this->ReleaseCapture();
			::InvalidateRect(GetHWND(),&FRect,FALSE);
		}
		return true;
	}

	bool XReportSheetSvr::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
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

	bool XReportSheetSvr::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);

		RECT tRect ;
		this->GetDwRect(&tRect);
		XReportDataSvr* pDataSvr = dynamic_cast<XReportDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pHeaderSvr) return true;
		int hHeight = pDataSvr->m_pHeaderSvr->GetTotalLevel()*pDataSvr->m_pHeaderSvr->m_nRowHeight;
		tRect.bottom = tRect.top + hHeight;
		TGridState State = CalcSizingState(point,tRect);
		if(State == gsColSizing)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_SIZEWE);
			hCursor = ::SetCursor(hCursor);
			pEvent->dwRet = 1;
		}
		return true;
	}

	void XReportSheetSvr::OnDoubleClicked(int nRow)
	{
		DWRNMHDR hdr;
		memset(&hdr,0,sizeof(DWRNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_DOUBLECLICKED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = GetCol();
		hdr.colname = (LPTSTR)GetColumnName(hdr.col);
		hdr.data = NULL;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		if(m_pSheetCtrl->m_pObservers)
		{
			for(int i=0;i<(int)m_pSheetCtrl->m_pObservers->size();i++)
			{
				if((*m_pSheetCtrl->m_pObservers)[i])
				{
					IReportEvent * pEvent = dynamic_cast<IReportEvent *>((*m_pSheetCtrl->m_pObservers)[i]);
					if(pEvent) pEvent->OnDoubleClicked(nRow);
				}
			}
		}
	}
	void XReportSheetSvr::OnRButtonDown(int nRow,TEvent * pEvent)
	{
		DWRNMHDR hdr;
		memset(&hdr,0,sizeof(DWRNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_RCLICKED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = GetCol();
		hdr.colname = (LPTSTR)GetColumnName(hdr.col);
		hdr.data = NULL;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		if(m_pSheetCtrl->m_pObservers)
		{
			for(int i=0;i<(int)m_pSheetCtrl->m_pObservers->size();i++)
			{
				if((*m_pSheetCtrl->m_pObservers)[i])
				{
					IReportEvent * pEvent1 = dynamic_cast<IReportEvent *>((*m_pSheetCtrl->m_pObservers)[i]);
					if(pEvent1) pEvent1->OnRButtonDown(nRow,pEvent);
				}
			}
		}
	}

	void XReportSheetSvr::OnClicked(int nRow,int nCol)
	{
		DWRNMHDR hdr;
		memset(&hdr,0,sizeof(DWRNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_CLICKED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetColumnName(hdr.col);
		hdr.data = NULL;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
	}

	void XReportSheetSvr::OnRowFocusChange(int nOldRow,int nNewRow)
	{
		DWRNMHDR hdr;
		memset(&hdr,0,sizeof(DWRNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ROWFOCUSCHANGED;
		hdr.DwTable = NULL;
		hdr.row = nNewRow;
		hdr.oldrow = nOldRow;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		FCurrent.y = nNewRow;
		if(m_pSheetCtrl->m_pObservers)
		{
			for(int i=0;i<(int)m_pSheetCtrl->m_pObservers->size();i++)
			{
				if((*m_pSheetCtrl->m_pObservers)[i])
				{
					IReportEvent * pEvent = dynamic_cast<IReportEvent *>((*m_pSheetCtrl->m_pObservers)[i]);
					if(pEvent) pEvent->OnRowFoucsChanged(nNewRow, nOldRow);
				}
			}
		}
	}
	

	bool XReportSheetSvr::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND XReportSheetSvr::GetHWND()
	{
		return this->m_pSheetCtrl->GetHWND();
	}
	XReportDataSvr * XReportSheetSvr::GetReportDataSvr()
	{
		return (XReportDataSvr *)this->m_pDataSvr;
	}
	bool XReportSheetSvr::GetDwRect(RECT *pRect)
	{
		*pRect = FRect;
		if(GetReportDataSvr()->FOption & troRowSelector || this->GetReportDataSvr()->FOption & troRowSelectorEx)
			pRect->left = GetReportDataSvr()->m_pRowSelectorSvr->FRect.right + 1;
		if(GetReportDataSvr()->FOption & troRightSelector)
			pRect->right = GetReportDataSvr()->m_pRightRowSelectorSvr->FRect.left - 1;
		return true;
	}

	int XReportSheetSvr::GetRow()
	{
		return this->GetReportDataSvr()->m_pDetailSvr->m_nCurRow;
	}

	int XReportSheetSvr::GetRowCount()
	{
		return this->GetReportDataSvr()->m_pContentSvr->GetRowCount();
	}
	int XReportSheetSvr::SetRow(int ARow)
	{
		if(ARow > GetRowCount()) return 1;
		if(ARow < 1) return 1;
		this->GetReportDataSvr()->m_pDetailSvr->m_nCurRow = ARow;
		::InvalidateRect(GetHWND(),NULL,false);
		return 1;
	}

	int XReportSheetSvr::Retrieve(LPTSTR pStrXQuery)
	{
		if(!this->GetReportDataSvr()->m_pContentSvr->m_pUrl)
			this->GetReportDataSvr()->m_pContentSvr->m_pUrl = _tcsdup(this->m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl);
		int nRows = this->GetReportDataSvr()->m_pContentSvr->Retrieve(pStrXQuery);
		::InvalidateRect(GetHWND(),&FRect,false);
		return nRows;
	}

	int XReportSheetSvr::Retrieve(LPTSTR pStrUrl,LPTSTR pStrXQuery)
	{
		return 0;
	}

	bool XReportSheetSvr::Redraw()
	{
		::InvalidateRect(this->GetHWND(),&FRect,false);
		return true;
	}

	int XReportSheetSvr::GetColumnCount()
	{
		return GetReportDataSvr()->m_pContentSvr->GetColCount();
	}

	LPCTSTR XReportSheetSvr::GetColumnName(int col)
	{
		return ((XXmlContentSvr *)GetReportDataSvr()->m_pContentSvr)->GetColumnName(col);
	}

	int XReportSheetSvr::GetColumnIndex(LPCTSTR pColumn)
	{
		return ((XXmlContentSvr *)GetReportDataSvr()->m_pContentSvr)->GetColumnIndex(pColumn);
	}


	LPCTSTR XReportSheetSvr::GetItemString(int ARow,int ACol ,IDwView::DWBuffer dwBuffer)
	{
		return this->GetReportDataSvr()->m_pContentSvr->GetItemString(ACol,ARow);
	}
	LPCTSTR XReportSheetSvr::GetItemString(int ARow,LPCTSTR pColName,IDwView::DWBuffer dwBuffer)
	{
		return this->GetReportDataSvr()->m_pContentSvr->GetItemString((LPTSTR)pColName,ARow);
	}
	
	int XReportSheetSvr::DeleteRow(int nRow)
	{
		int nRet = GetReportDataSvr()->m_pContentSvr->DeleteRow(nRow);
		Redraw();
		return nRet;

	}
	int XReportSheetSvr::InsertRow(int nRow)
	{
		int nRowex = GetReportDataSvr()->m_pContentSvr->InsertRow(nRow);
		Redraw();
		return nRowex;
	}

	bool XReportSheetSvr::SetItemString(int Row,int Col,LPCTSTR pStr ,IDwView::DWBuffer dwBuffer)
	{
		return GetReportDataSvr()->m_pContentSvr->SetItemString(Col,Row,(LPTSTR)pStr);
	}

	int XReportSheetSvr::DwUpdate(IXMLDOMDocument2 * pXmlDoc)
	{
		int nColCount = this->GetColumnCount();
		vector<LPTSTR> xslPaths;
		LPTSTR pXslPath;
		for(int nCol = 0;nCol <nColCount;nCol++)
		{
			//2017.8.23 xslPaths.push_back((LPTSTR)GetColumnName(nCol + 1));
			xslPaths.push_back((LPTSTR)GetColumnName(nCol));
		}
		pXslPath = ((XXmlContentSvr *)GetReportDataSvr()->m_pContentSvr)->GetXPath();

		long gcnt = 1;
		IXMLDOMNodeListPtr gNodes;
		IXMLDOMNodePtr gNode;
		IXMLDOMNodePtr pNode;
		IXMLDOMNodePtr Node;
		IXMLDOMNodePtr  currentNode;
		IXMLDOMNodePtr pNodeEx;
		IXMLDOMNodeListPtr pRowNodes;
		IXMLDOMElement* pNewElement;
		IXMLDOMNodeList* pTempNodes; 
		IXMLDOMNodePtr NewNode;
		IXMLDOMElementPtr tNode;
		IXMLDOMElementPtr NewElement;
		IXMLDOMElementPtr NewElement1;
		vector<IXMLDOMElement * > vcElements;
		wstring StrName;

		gcnt = 1;
		for(int group = 1;group <=gcnt;group++)
		{
			gNode = pXmlDoc;

			//if no exists pNode then create parent Node
			wstring StrPath = pXslPath;
			int nPosLast = -1;
			nPosLast = (int)StrPath.find_last_of(_T("/"));

			for(vector<IXMLDOMElement * >::const_iterator tr=vcElements.begin();tr!=vcElements.end();tr++)(*tr)->Release();
			vcElements.clear();

			wstring StrName;
			if(nPosLast >=0)
			{
				StrName = StrPath.substr(nPosLast + 1);
				StrPath = StrPath.substr(0,nPosLast);
				while(StrPath.compare(_T(""))!=0)
				{
					pNode = NULL;
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);
					if(!pNode)break;
					nPosLast = (int)StrPath.find_last_of(_T("/"));
					if(nPosLast < 0)break;
					StrName = StrPath.substr(nPosLast + 1);
					StrPath = StrPath.substr(0,nPosLast);
					if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
					if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
					pXmlDoc->createElement((LPTSTR)StrName.c_str(),&pNewElement);
					vcElements.push_back(pNewElement);
				}
				if(StrPath.compare(_T(""))!=0)
					pNode = gNode;
				else
					gNode->selectSingleNode((LPTSTR)StrPath.c_str(),&pNode);

				for(int i=(int)vcElements.size() - 1;i>1;i--)
				{
					vcElements[i]->appendChild(vcElements[i - 1],NULL);
				}
				if(vcElements.size() > 0)
				{
					pNode->appendChild(vcElements[vcElements.size() - 1],NULL);
					pNodeEx = vcElements[0];
				}else
					pNodeEx = pNode;
			}else
			{
				StrName = StrPath;
				if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
				if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
				StrPath = _T("");
				pNodeEx = gNode;
			}
			//end for if no exists pNode then create parent node

			gNode->selectNodes(pXslPath,&pRowNodes);
			int trow =0;
			long nRowLen;
			pRowNodes->get_length(&nRowLen);
			int row = 1;
			for(row=1;row <= this->GetRowCount();row++)
			{
				trow++;
				if(row <= nRowLen)
				{
					pRowNodes->get_item(row -1,&Node);
					for(int i=1 ;i<=(int)xslPaths.size();i++)
					{
						Node->selectSingleNode(xslPaths[i - 1],&currentNode);
						if(!currentNode)
						{
							//create not exits column node
							wstring path = xslPaths[i - 1];
							if(path[0]=='/' &&path[1]=='/')continue; 
							if(path[0]=='/')path = path.substr(1);
							int pos = (int)path.find(_T("/"));
							if(pos >0)
							{
								StrName = path.substr(0,pos);
								path = path.substr(pos +1);
							}else
							{
								StrName = path;
								path = _T("");
							}
							if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
							if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

							Node->selectNodes((LPTSTR)StrName.c_str(),&pTempNodes);
							long nTempLen = 0;
							pTempNodes->get_length(&nTempLen);
							if(nTempLen < 1)
							{
								if(StrName[0]=='@')
								{
									StrName = StrName.substr(1);
									((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
									continue;
								}else
								{
									pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement);
									Node->appendChild(NewElement,NULL);
								}
							}else
							{
								if(StrName[0]=='@')continue;
								 pTempNodes->get_item(0,&NewNode);
								 NewElement = NewNode;
							}
							while(path.compare(_T(""))!=0)
							{
								int pos = (int)path.find(_T("/"));
								if(pos >=0)
								{
									StrName =  path.substr(0,pos);
									path = path.substr(pos + 1);
								}else
								{
									StrName = path;
									path = _T("");
								}
								NewElement->selectNodes((LPTSTR)StrName.c_str(),&pTempNodes);
								pTempNodes->get_length(&nTempLen);
								if(nTempLen < 1)
								{
									if(StrName[0]=='@')
									{
										StrName = StrName.substr(1);
										((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
										continue;
									}else
									{
										pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement1);
										NewElement->appendChild(NewElement1,NULL);
										NewElement = NewElement1;
									}
								}else
								{
									if(StrName[0]=='@')continue;
									 pTempNodes->get_item(0,&NewNode);
									 NewElement = NewNode;
								}
							}
							//end create not exists column
						}
					}
				}else
				{
					//create data frame
					if(!tNode)
					{
						//create template data node
						wstring StrPath = pXslPath;
						int posLast = (int)StrPath.find_last_of(_T("/"));
						if(posLast>=0)
							StrPath = StrPath.substr(nPosLast + 1);
						else
							StrName = StrPath;
						
						if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
						if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));

						pXmlDoc->createElement((LPTSTR)StrName.c_str(),&tNode);

						//end create template data node
						for(int i=1 ;i<=(int)xslPaths.size();i++)
						{
							wstring path = xslPaths[i - 1];
							if(path[0]=='/' &&path[1]=='/')continue; 
							if(path[0]=='/')path = path.substr(1);
							int pos = (int)path.find(_T("/"));
							if(pos >0)
							{
								StrName = path.substr(0,pos);
								path = path.substr(pos +1);
							}else
							{
								StrName = path;
								path = _T("");
							}
							if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
							if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
							if(StrName[0]=='@')
							{
								StrName = StrName.substr(1);
								((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
								continue;
							}else
							{
								pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement);
								tNode->appendChild(NewElement,NULL);
							}
							while(path.compare(_T(""))!=0)
							{
								int pos = (int)path.find(_T("/"));
								if(pos >=0)
								{
									StrName =  path.substr(0,pos);
									path = path.substr(pos + 1);
								}else
								{
									StrName = path;
									path = _T("");
								}
								if(StrName.find(_T("("))>0)StrName = StrName.substr(0,StrName.find(_T("(")));
								if(StrName.find(_T("["))>0)StrName = StrName.substr(0,StrName.find(_T("[")));
								if(StrName[0]=='@')
								{
									StrName = StrName.substr(1);
									((IXMLDOMElementPtr)Node)->setAttribute((LPTSTR)StrName.c_str(),(_variant_t)_T(""));
									continue;
								}else
								{
									pXmlDoc->createElement((LPTSTR)StrName.c_str(),&NewElement1);
									NewElement->appendChild(NewElement1,NULL);
									NewElement = NewElement1;
								}
							}//while
						}//next
					}//end if tnod
					//end create data frame

					tNode->cloneNode(VARIANT_TRUE,&Node);
					pNodeEx->appendChild(Node,NULL);
				}//row
				
				//set data
				//set key value
				//long ll_value;
				_variant_t var;
				((XXmlContentSvr *)GetReportDataSvr()->m_pContentSvr)->GetRowElement(trow)->getAttribute(L"guid",&var);

				if(!var.bstrVal || var.bstrVal[0]=='\0')
				{

					GUID guid;
					CoCreateGuid(&guid);
					LPTSTR pStrGuid = new TCHAR[48];
					StringFromGUID2(guid,pStrGuid,48);
					((IXMLDOMElementPtr)Node)->setAttribute(L"guid",(_variant_t)pStrGuid);
				}else
				{
					((IXMLDOMElementPtr)Node)->setAttribute(L"guid",var);
				}


				LPCTSTR pStrValue;
				DOMNodeType	iNodeType;
				for(int col=1;col <= (int)xslPaths.size();col++)
				{
					pStrValue = this->GetItemString(trow,col);
					currentNode = NULL;
					Node->selectSingleNode(xslPaths[col - 1],&currentNode);
					if(currentNode)
					{
						currentNode->get_nodeType(&iNodeType);
						if(iNodeType == NODE_ELEMENT)
						{
							if(!pStrValue)
							{
								((IXMLDOMElementPtr)currentNode)->setAttribute(L"isnull",(_variant_t)_T("1"));
								currentNode->put_text(L"");
							}else
							{
								((IXMLDOMElementPtr)currentNode)->setAttribute(L"isnull",(_variant_t)_T("0"));
								((IXMLDOMElementPtr)currentNode)->removeAttribute(L"isnull");
								currentNode->put_text((BSTR)pStrValue);
							}
						}
						else //attribute
						{
							if(!pStrValue)
								currentNode->put_text(L"");
							else
								currentNode->put_text((BSTR)pStrValue);
						}//NodeType
					}//currentNode
				}//for
			}//for row

			IXMLDOMNode * pNodeItem;
			for(int k = nRowLen;k>=row;k--)
			{
				pRowNodes->get_item( k - 1,&pNodeItem);
				pNodeEx->removeChild(pNodeItem,NULL);
				pNodeItem->Release();
			}
		}
		//end update data to xml node
		return 1;
	}
	int XReportSheetSvr::SetReadOnly(BOOL bReadOnly)
	{
		if(!bReadOnly)
			FOptions |= goAllowEdit;
		else
			FOptions &= ~goAllowEdit;
		
		return 1;
			
	}

	int  XReportSheetSvr::GetXml(BSTR &bstr)
	{
		return GetReportDataSvr()->m_pContentSvr->GetContent(bstr);
	}
	int  XReportSheetSvr::GetXml(BSTR &bstr,LPTSTR pGuid)
	{
		return GetReportDataSvr()->m_pContentSvr->GetContent(bstr,pGuid);
	}
	int  XReportSheetSvr::SetXml(BSTR bstr)
	{
		return GetReportDataSvr()->m_pContentSvr->LoadXml(bstr);
	}
	
	int XReportSheetSvr::SelectRow(int nRow,bool bSelect)
	{
		if(nRow==0)
		{
			if(bSelect)
			{
				m_pRowSelects->SetValue(1,GetRowCount(),1);
				return 1;
			}else
			{
				m_pRowSelects->Reset(0);
				return 1;
			}
		}
		if(nRow > GetRowCount()) return -1;
		if(nRow < 1) return -1;

		m_pRowSelects->SetValue(nRow -1,bSelect?1:0);

		return 1;
	}
	
	int XReportSheetSvr::SelectRow(int sRow,int eRow, bool bSelect)
	{
		int nRow1 = sRow < eRow ? sRow:eRow;
		int nRow2 = sRow > eRow ? sRow:eRow;
		
		if(nRow2 < 1) return -1;
		if(nRow1 > GetRowCount()) return -1;
		if(nRow1 < 1) nRow1 = 1;
		if(nRow2 > GetRowCount()) nRow2 = GetRowCount();
		m_pRowSelects->SetValue(nRow1 -1 ,nRow2 - 1, bSelect?1:0);
		return 1;
	}

	int XReportSheetSvr::GetNextSelectRow(int nStartRow)
	{
		if(nStartRow < 1) nStartRow = 1 ;
		int Rows = GetRowCount();
		while(nStartRow <= Rows)
		{
			if(m_pRowSelects->GetValue(nStartRow - 1) ==1) return nStartRow;
			nStartRow ++;
		}
		return -1;
	}

	bool XReportSheetSvr::IsRowSelected(int nRow)
	{
		if(nRow < 1) return false;
		if(nRow > GetRowCount())return false;
		return m_pRowSelects->GetValue(nRow - 1)==1?true:false;
	}
	int XReportSheetSvr::SetSelectBkColor(COLORREF color)
	{
		m_SelectBkColor = color;
		return 1;
	}
	int XReportSheetSvr::SetSelectionMode(int nMode)
	{
		this->m_SelectionMode = nMode;
		return 1;
	}

	int XReportSheetSvr::AddObserver(xbObserver * pObserver)
	{
		m_pObServers.push_back(pObserver);
		return 1;
	}
	int XReportSheetSvr::RemoveObserver(xbObserver * pObserver)
	{
		for(int i= 0;i < (int)m_pObServers.size();i++)
		{
			if(m_pObServers[i] == pObserver)
			{
				m_pObServers.erase(m_pObServers.begin()+i);
				break;
			}
		}
		return 1;
	}
	int XReportSheetSvr::ClearObserver()
	{
		m_pObServers.clear();
		return 1;
	}
	
	int XReportSheetSvr::event_ItemFocusChanged(int nRow,int nCol)
	{
		DWRNMHDR hdr;
		memset(&hdr,0,sizeof(DWRNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMFOCUSCHANGED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetColumnName(nCol);
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IReportObserver *)m_pObServers[i])->ItemFocueChanged((IReport *)GetInterface(),nRow,nCol);
		return 1;
	}
	int XReportSheetSvr::event_RowFocusChanged(int nRow,int nOldRow)
	{
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IReportObserver *)m_pObServers[i])->RowFoucsChanged((IReport *)GetInterface(),nRow,nOldRow);
		return 1;
	}
	int XReportSheetSvr::event_GetFocus( )
	{
		for(int i=0;i<(int)m_pObServers.size();i++)
		{
			((IReportObserver *)m_pObServers[i])->GetFocus();
		}
		return 1;
	}
	int XReportSheetSvr::event_LoseFocus()
	{
		for(int i=0;i<(int)m_pObServers.size();i++)
		{
			((IReportObserver *)m_pObServers[i])->LoseFocus();
		}
		return 1;
	}
	int XReportSheetSvr::event_ItemChanged(int nRow,int nCol,LPTSTR pData)
	{
		DWRNMHDR hdr;
		memset(&hdr,0,sizeof(DWRNMHDR));
		hdr.hwndFrom = GetHWND();
		hdr.idFrom = 0;
		hdr.code = DWV_ITEMCHANGED;
		hdr.DwTable = NULL;
		hdr.row = nRow;
		hdr.col = nCol;
		hdr.colname = (LPTSTR)GetColumnName(nCol);
		hdr.data = pData;
		::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IReportObserver *)m_pObServers[i])->ItemChanged((IReport *)GetInterface(),nRow,nCol,pData);
		return 1;
	}
	int XReportSheetSvr::event_ItemChanging(int nRow,int nCol,LPTSTR pData)
	{
		for(int i=0;i<(int)m_pObServers.size();i++)
			((IReportObserver *)m_pObServers[i])->ItemChanging((IReport *)GetInterface(),nRow,nCol,pData);
		return 1;
	}

	IXMLDOMElement * XReportSheetSvr::GetRowElement(int nRow)
	{
		if(nRow < 1 || nRow > this->GetRowCount()) return NULL;
		 int row;
		 row = GetReportDataSvr()->m_pContentSvr->GetContentRow(nRow);
		 if(row > 0)
			return GetReportDataSvr()->m_pContentSvr->GetRowElement(row);
		 else
			return NULL;
	}
	
	int XReportSheetSvr::InsertCol(int nPos, LPCTSTR pCaption,LPCTSTR pColName,int nWidth,LPCTSTR pColPath,LPCTSTR pStyleClass,
				LPCTSTR pEditClass,LPCTSTR pFormatClass)
	{
		int nIndex = nPos ;
		if(nIndex > GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size() || nIndex < 1) nIndex = GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size()  + 1;

		GetReportDataSvr()->m_pContentSvr->m_pCols-> push_back(_tcsdup(pColName));
		if(!pColPath)
			GetReportDataSvr()->m_pContentSvr->m_pXPaths->push_back(_tcsdup(pColName));
		else
			GetReportDataSvr()->m_pContentSvr->m_pXPaths->push_back(_tcsdup(pColPath));

		ColHeaderItem *pItem = new ColHeaderItem;

		pItem->colid = (int)GetReportDataSvr()->m_pContentSvr->m_pCols->size();
		pItem->pStrText = _tcsdup(pCaption);
		if(pStyleClass)
		{
			pItem->m_nIndexStyle = GetReportDataSvr()->m_pStyleSvr->GetBoxStyleIndexByClass((LPTSTR)pStyleClass);
		}else
		{
			if(nPos > 1 && nPos - 1< GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems.size())
			{
				pItem->m_nIndexStyle = GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems[nPos - 2]->m_nIndexStyle;
			}else if(nPos == 1  )
			{
				pItem->m_nIndexStyle = GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems[0]->m_nIndexStyle;
			}else if(nPos <=0 && GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems.size()>0)
			{
				pItem->m_nIndexStyle = GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems[ GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems.size() - 1]->m_nIndexStyle;
			}else
				pItem->m_nIndexStyle =  -1;
		}
		pItem->m_nWidth = nWidth;
		GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems.insert(nIndex - 1,pItem);

		XReportBandItem *pItemEx = new XReportBandItem;

		pItemEx->colid = (int)GetReportDataSvr()->m_pContentSvr->m_pCols->size();
		pItemEx->pStrText = NULL;
		if(pStyleClass)
		{
			pItemEx->m_nIndexStyle = GetReportDataSvr()->m_pStyleSvr->GetBoxStyleIndexByClass((LPTSTR)pStyleClass);
		}else
		{
			if(nPos > 1 && nPos - 1< GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size())
			{
				pItemEx->m_nIndexStyle = GetReportDataSvr()->m_pDetailSvr->m_DetailItems[nPos - 2]->m_nIndexStyle;
			}else if(nPos == 1  )
			{
				pItemEx->m_nIndexStyle = GetReportDataSvr()->m_pDetailSvr->m_DetailItems[0]->m_nIndexStyle;
			}else if(nPos <=0 && GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size()>0)
			{
				pItemEx->m_nIndexStyle = GetReportDataSvr()->m_pDetailSvr->m_DetailItems[ GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size() - 1]->m_nIndexStyle;
			}else
				pItemEx->m_nIndexStyle =  -1;
		}
		pItemEx->m_nWidth = nWidth;
		GetReportDataSvr()->m_pDetailSvr->m_DetailItems.insert(nIndex - 1,pItemEx);

		return nIndex;
	}

	int XReportSheetSvr::RemoveCol(int nCol)
	{
		if(nCol < 1 || nCol >= (int)GetReportDataSvr()->m_pContentSvr->m_pCols->size()) return -1;

		nCol = GetReportDataSvr()->m_pDetailSvr->m_DetailItems[nCol -1]->colid;

		LPCTSTR pColName = this->GetColumnName(nCol);
		this->SetDDLBData(pColName,NULL);
		GetReportDataSvr()->m_pContentSvr->m_pCols->erase(GetReportDataSvr()->m_pContentSvr->m_pCols->begin()+ nCol - 1);

		int size = GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems.size();
		for(int i=size -1;i>=0;i--)
		{
			if( GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems[i]->colid ==nCol) 
				GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems.erase(i);
		}

		size = GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size();
		for(int i=size -1;i>=0;i--)
		{
			if( GetReportDataSvr()->m_pDetailSvr->m_DetailItems[i]->colid ==nCol) 
				GetReportDataSvr()->m_pDetailSvr->m_DetailItems.erase(i);
		}

		size = GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems.size();
		for(int i=size -1;i>=0;i--)
		{
			if( GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems[i]->colid > nCol) 
				GetReportDataSvr()->m_pHeaderSvr->m_HeaderItems[i]->colid--;
		}

		size = GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size();
		for(int i=size -1;i>=0;i--)
		{
			if( GetReportDataSvr()->m_pDetailSvr->m_DetailItems[i]->colid > nCol) 
				GetReportDataSvr()->m_pDetailSvr->m_DetailItems[i]->colid--;
		}

		return 1;
	}

	int XReportSheetSvr::RemoveVCol(int nCol)
	{
		if(nCol < 1 || nCol > (int)GetReportDataSvr()->m_pDetailSvr->m_DetailItems.size()) return -1;

		XReportDataSvr * pDataSvr = GetReportDataSvr();
		int nColId = pDataSvr->m_pDetailSvr->m_DetailItems[nCol - 1]->colid;
		if(nColId > 0)
			pDataSvr->m_pContentSvr->m_pCols->erase(pDataSvr->m_pContentSvr->m_pCols->begin()+ nColId - 1);

		pDataSvr->m_pHeaderSvr->m_HeaderItems.erase(nCol - 1);
		pDataSvr->m_pDetailSvr->m_DetailItems.erase(nCol - 1);

		int size = pDataSvr->m_pHeaderSvr->m_HeaderItems.size();
		for(int i=size -1;i>=0;i--)
		{
			if( pDataSvr->m_pHeaderSvr->m_HeaderItems[i]->colid > nColId) 
				pDataSvr->m_pHeaderSvr->m_HeaderItems[i]->colid--;
		}

		size = pDataSvr->m_pDetailSvr->m_DetailItems.size();
		for(int i=size -1;i>=0;i--)
		{
			if( pDataSvr->m_pDetailSvr->m_DetailItems[i]->colid > nColId) 
				pDataSvr->m_pDetailSvr->m_DetailItems[i]->colid--;
		}

		return 1;
	}

	int XReportSheetSvr::RemoveCol(LPCTSTR pColName)
	{
		int nCol;
		nCol = GetReportDataSvr()->m_pContentSvr-> GetColumnIndex(pColName);
		if(nCol < 1) return -1;

		return RemoveCol(nCol);
	}

	int XReportSheetSvr::AddEditStyle(LPCTSTR pName, LPCTSTR pStr)
	{
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		if(!pDataSvr->m_pClassEditor)
		{
			pDataSvr->m_pClassEditor = new vector<LPTSTR>;
			pDataSvr->m_pEditor = new vector<LPTSTR>;
		}
		pDataSvr->m_pClassEditor->push_back(_tcsdup(pName));
		pDataSvr->m_pEditor->push_back(_tcsdup(pStr));

		return 1;
	}
	int XReportSheetSvr::RemoveEditStyle(LPCTSTR pName, LPCTSTR pStr)
	{
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		if(!pDataSvr->m_pClassEditor) return -1;
		for(int i=0;i<(int)pDataSvr->m_pClassEditor->size();i++)
		{
			if(_tcsicmp(pName,(*pDataSvr->m_pClassEditor)[i])==0)
			{
				pDataSvr->m_pClassEditor->erase(pDataSvr->m_pClassEditor->begin() + i);
				pDataSvr->m_pEditor->erase(pDataSvr->m_pEditor->begin() + i);
				break;
			}
		}
		return 1;
	}
	int XReportSheetSvr::SetColEditStyle(LPCTSTR pColName,LPCTSTR pClassName)
	{
		int nIndex = this->GetColumnIndex(pColName);
		if(nIndex < 1) return -1;
		
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		int nClassIndex = -1;
		for(int i=0;i<(int)pDataSvr->m_pClassEditor->size();i++)
		{
			if(_tcsicmp(pClassName,(*pDataSvr->m_pClassEditor)[i])==0)
			{
				 nClassIndex = i;
				break;
			}
		}
		if(nClassIndex >=0)
		{
			for(int i=0;i<(int)pDataSvr->m_pDetailSvr->m_DetailItems.size();i++)
			{
				if(pDataSvr->m_pDetailSvr->m_DetailItems[i]->colid == nIndex)
				{
					pDataSvr->m_pDetailSvr->m_DetailItems[i]->m_nIndexEditor = nClassIndex;
					break;
				}
			}
		}

		return 1;
	}

	int XReportSheetSvr::SetDDLBData(LPCTSTR pColName,LPCTSTR pData)
	{
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		if(!pDataSvr->m_pContentSvr->m_pDDLBData)
		{
			pDataSvr->m_pContentSvr->m_pDDLBData = new map<wstring,wstring>;
		}
		map<wstring,wstring>::iterator tr;
		tr = pDataSvr->m_pContentSvr->m_pDDLBData->find(pColName);
		if(tr!= pDataSvr->m_pContentSvr->m_pDDLBData->end())
		{
			if(!pData)
				pDataSvr->m_pContentSvr->m_pDDLBData->erase(tr);
			else
				(*pDataSvr->m_pContentSvr->m_pDDLBData)[pColName] = pData;
		}else
		{
			pair<wstring,wstring> pr;
			pr.first = pColName;
			pr.second = pData;
			pDataSvr->m_pContentSvr->m_pDDLBData->insert(pr);
		}
		return 1;
	}

	LPCTSTR XReportSheetSvr::GetDDLBData(LPCTSTR pColName)
	{
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		if(!pDataSvr->m_pContentSvr->m_pDDLBData)
		{
			pDataSvr->m_pContentSvr->m_pDDLBData = new map<wstring,wstring>;
		}
		map<wstring,wstring>::const_iterator tr;
		tr = pDataSvr->m_pContentSvr->m_pDDLBData->find(pColName);
		if(tr!= pDataSvr->m_pContentSvr->m_pDDLBData->end())
		{
			return (*pDataSvr->m_pContentSvr->m_pDDLBData)[pColName].c_str();
		}else
			return NULL;
	}

	bool XReportSheetSvr::CanEditAcceptKey(char Key)
	{
		return true;
	}
	bool XReportSheetSvr::CanGridAcceptKey(WORD Key, TShiftState Shift)
	{
		return true;
	}
	bool XReportSheetSvr::CanEditModify()
	{
		return FCanEditModify;
	}

	bool XReportSheetSvr::CanEditShow()
	{
		return  (!(goRowSelect &FOptions) && goAllowEdit&FOptions) &&
			FEditorMode &&  HandleAllocated &&
			((goAlwaysShowEditor & FOptions) || IsActiveControl());
	}

	bool XReportSheetSvr::IsActiveControl()
	{
		return true;
	}

	void XReportSheetSvr::InvalidateEditor()
	{
		FInplaceCol = -1;
		FInplaceRow = -1;
		UpdateEdit();
	}

	void XReportSheetSvr::UpdateEditor()
	{
		m_pEditSvr->SelectCell(GetCol(), GetRow());
	}

	void XReportSheetSvr::UpdateEdit()
	{
		if(CanEditShow())
		{
			m_pEditSvr->SelectCell(GetCol(), GetRow());
			FInplaceCol = GetCol();
			FInplaceRow = GetRow();
		}
	}
	void XReportSheetSvr::ShowEditor()
	{
		FEditorMode = TRUE;
		UpdateEdit();
	}

	void XReportSheetSvr::HideEdit()
	{
		this->m_pEditSvr->HiddenEditor();
		FInplaceCol = -1;
		FInplaceRow = -1;
	}

	void XReportSheetSvr::HideEditor()
	{
		FEditorMode = FALSE;
		HideEdit();
	}

	LPTSTR XReportSheetSvr::GetCellContent(XOfficeSheetSvr* m_pOwner, int ACol,int ARow)
	{
		XReportDataSvr * pDataSvr = ((XReportSheetSvr *) m_pOwner)->GetReportDataSvr();
		if(pDataSvr)
		{
			int VCol =((XReportSheetSvr*) m_pOwner)->GetVColumn(ACol);
			return _tcsdup(pDataSvr->m_pContentSvr->GetItemString(VCol,ARow));
		}
		return NULL;
	}
	int XReportSheetSvr::SetCellContent(XOfficeSheetSvr* m_pOwner, int ACol,int ARow, LPTSTR pStr)
	{
		XReportDataSvr * pDataSvr = ((XReportSheetSvr *) m_pOwner)->GetReportDataSvr();
		if(pDataSvr)
		{
			int VCol =((XReportSheetSvr*) m_pOwner)->GetVColumn(ACol);
			pDataSvr->m_pContentSvr->SetItemString(VCol,ARow,pStr);
		}
		return 1;
	}
	int XReportSheetSvr::GetCellRect(XOfficeSheetSvr* m_pOwner, int ACol,int ARow,RECT &rt)
	{
		return ((XReportSheetSvr *) m_pOwner)->GetCellRect(ACol,ARow,rt);
	}
	LPTSTR XReportSheetSvr::GetEditType(XOfficeSheetSvr* m_pOwner, int ACol,int ARow)
	{
		XReportDataSvr * pDataSvr = ((XReportSheetSvr *) m_pOwner)->GetReportDataSvr();
		if(pDataSvr)
		{
			int nIndexEditor = pDataSvr->m_pDetailSvr->m_DetailItems[ACol - 1]->m_nIndexEditor;
			if(nIndexEditor>=0)
			{
				LPTSTR pStr = (*pDataSvr->m_pEditor)[nIndexEditor];
				return _tcsdup(pStr);
			}
		}

		return _tcsdup(L"edit");
	}
	HFONT XReportSheetSvr::GetEditFont(XOfficeSheetSvr* m_pOwner,int ACol,int ARow)
	{
		XReportDataSvr * pDataSvr = ((XReportSheetSvr *) m_pOwner)->GetReportDataSvr();
		if(pDataSvr)
		{
			const XUtil::Css::TBoxStyle * pStyle = pDataSvr->m_pStyleSvr->GetBoxStyle( pDataSvr->m_pDetailSvr->m_DetailItems[ACol - 1]->m_nIndexStyle);
			LOGFONT	LogFont;
			XUtil::Css::GetFont(pStyle,LogFont);
			HFONT	hFont = ::CreateFontIndirect(&LogFont);
			return hFont;
		}
		return NULL;
	}

	bool XReportSheetSvr::GetCellRect(int ACol,int ARow,RECT &rect)
	{
		RECT dRect;
		this->GetDwRect(&dRect);

		int width;
		width = dRect.right- dRect.left;

		int StartCol = FTopLeft.x;
		int StartRow = FTopLeft.y;

		int startCell = FTopLeft.x;
		int LineX = 0;
		int nCol ;

		int EndCol = this->GetColumnCount();

		if(ACol < StartCol) return false;
		if(ACol > EndCol) return false;
		if(ARow < StartRow) return false;
		if(ARow > GetRowCount()) return false;

		XReportDataSvr * pDataSvr = GetReportDataSvr();
		int hHeight = pDataSvr->m_pHeaderSvr->GetTotalLevel()*pDataSvr->m_pHeaderSvr->m_nRowHeight;
		int LineY = dRect.top  + hHeight + 1;
		int RowLineY;

		for(int row = StartRow;row<=GetRowCount();row++)
		{
			RowLineY = LineY;
			if(ARow == row)
			{
				RECT	ARect;
				::SetRect(&ARect,0,0,0,0);
				ARect.top = LineY;
				ARect.left = dRect.left + GetColWidth(0) + 1;
				ARect.bottom  = ARect.top +  GetRowHeight(row);

				LineX =  ARect.left;
				nCol = startCell;

				while(LineX <= dRect.right )
				{
					ARect.bottom  = ARect.top +  GetRowHeight(ARow);
					if(nCol <= EndCol)
					{
						ARect.right = ARect.left + GetColWidth(nCol); 

						if(nCol==ACol)
						{
							rect = ARect;
							return true;
						}

						nCol++;
						ARect.left = ARect.right + 1;
					}else
						break;
					LineX = ARect.left ;
				}
				LineY +=  GetRowHeight(row) + 1;
			}
			LineY = RowLineY + GetRowHeight(row) + 1;
			if(LineY > dRect.bottom) break;
		}
		return false;
	}

	int XReportSheetSvr::GetColWidth(int  nCol)
	{
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		if(nCol < 1 || nCol > this->GetColumnCount()) return 0;
		return pDataSvr->m_pDetailSvr->m_DetailItems[nCol - 1 ]->m_nWidth;
	}

	int XReportSheetSvr::GetRowHeight(int nRow)
	{
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		return pDataSvr->m_pDetailSvr->m_nHeight;
	}

	int XReportSheetSvr::GetVColumn(int nCol)
	{
		XReportDataSvr * pDataSvr = GetReportDataSvr();
		return pDataSvr->m_pDetailSvr->m_DetailItems[nCol -1]->colid;

	}

	int XReportSheetSvr::ImportString(LPCTSTR pStr,LPCTSTR pType,bool Header)
	{
		int row,col;
		int pos,start;
		int /*RowCount,*/colCount;
		int nLen ;
		TCHAR ch;
		TCHAR buffer[1024];
		BOOL  finish = false;
		finish = false;
		pos = 0;
		
		if(!pStr) return 0;
		if(Header)
		{
			pStr = _tcschr(pStr,'\n');
			if(!pStr) return 0;
			pStr++;
		}
		nLen = (int)_tcslen(pStr);
		if(nLen < 1) return 0;
		
		colCount = GetColumnCount();
		while(!finish)
		{
			//insert data row
			InsertRow(0);
			row = GetRowCount();
			int ncol;
			for(col=1;col<=colCount;col++)
			{
				ncol = GetVColumn(col);
				start = pos;
				do
				{
					ch = *(pStr+pos);
					buffer[pos - start]=ch;
					pos++;
				}while(pos <nLen && ch !='\0' && ch !='\t' &&ch!='\r' && ch!='\n');
				if(ch=='\t')buffer[pos-start - 1]='\0';
				if(ch=='\r')buffer[pos-start - 1]='\0';
				if(ch=='\n')buffer[pos-start - 1]='\0';
				SetItemString(row,ncol,buffer);
				if(pos == nLen ||ch=='\0' )
				{
					finish = true;
					break;
				}
				
				if(ch=='\t') continue;
				if(ch=='\r')
				{
					ch = *(pStr+pos);
					if(ch=='\n')pos++;
				}
				break;
			}
		}
		return 1;
	}
	
	int XReportSheetSvr::ImportClipBoard()
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
				
				ImportString(lpdata);
				
				GlobalUnlock(hData);
				GlobalFree(hData);
				
				GlobalUnlock( hClipGlobal );
			}
		} 
		CloseClipboard();
		return 1;
	}

}}}}
