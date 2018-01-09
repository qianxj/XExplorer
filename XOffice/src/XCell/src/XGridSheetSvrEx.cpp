#include "StdAfx.h"
#include "..\include\XGridSheetSvrEx.hpp"
#include "XGridHeaderEx.hpp"
#include "XGridSummury.hpp"
#include "XGridDataSvrEx.hpp"
#include "XGridReport.hpp"
#include "XGridRowSelector.hpp"
#include "XCellSerializeSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridSheetSvrEx::XGridSheetSvrEx(void)
	{
		m_pDataSvr = new XGridDataSvrEx();
		m_pSerializeSvr = new XCellSerializeSvr;
		m_pColHeaderSvr = new XGridHeaderEx();
		m_pXGridSummurySvr = new XGridSummury();
		m_pSelectorSvr  = new XGridRowSelector();
		m_pGridReport   = new XGridReport();
		m_pGridReport->m_pSheetSvr = this;

		m_pColHeaderSvr->m_pGridReportSvr = m_pGridReport;
		m_pXGridSummurySvr->m_pGridReportSvr = m_pGridReport;
		m_pSelectorSvr->m_pGridReportSvr  = m_pGridReport;
		
		m_pColHeaderSvr->AddRouterMessageFrom(this);
		m_pXGridSummurySvr->AddRouterMessageFrom(this);
		m_pSelectorSvr->AddRouterMessageFrom(this);
		m_pGridReport->AddRouterMessageFrom(this);

		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XGridSheetSvrEx::EvtLButtonDown);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XGridSheetSvrEx::EvtKeyDown);
		this->AttachListon(0x400+80,(FNEvent)&XGridSheetSvrEx::EvtRetrieveRow);
		this->AttachListon(0x400+81,(FNEvent)&XGridSheetSvrEx::EvtRetrievFinish);
		m_state =  goRunState;
	}

	bool XGridSheetSvrEx::EvtRetrieveRow(TEvent * pEvent, LPARAM lParam)
	{
		if(m_pSheetCtrl->hasBindData())
		{
			wchar_t buf[32];
			wcscpy(buf,L"retrievedata:");
			*(WPARAM *)(buf + wcslen(L"retrievedata:")+1) = pEvent->wParam;
			m_pSheetCtrl->bindGetData(m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, buf);

			m_pGridReport->GetVScrollBar()->nMax = m_pGridReport->GetRowCount();
			if(m_pGridReport->GetVScrollBar()->nMax<=1 )
				m_pGridReport->GetVScrollBar()->nMax = 10;
			::InvalidateRect(this->GetHWND(),&FRect,false);
			m_pGridReport->Redraw();
		}
		return true;
	}
	bool XGridSheetSvrEx::EvtRetrievFinish(TEvent *pEvent ,LPARAM lParam)
	{
		if(m_pSheetCtrl->hasBindData())
		{
			wchar_t buf[32];
			wcscpy(buf,L"retrievefinish:");
			m_pSheetCtrl->bindGetData(m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, buf);

			DWNMHDR hdr;
			memset(&hdr,0,sizeof(DWNMHDR));
			hdr.hwndFrom = GetHWND();
			hdr.idFrom = 0;
			hdr.code = 0x400+81;
			hdr.DwTable = m_pSheetCtrl->GetInterface();
			hdr.oldrow = -1;
			hdr.row = -1;
			::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
		}
		return true;
	}

	bool XGridSheetSvrEx::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		switch(nChar)
		{
			case 'M':
				if(::GetKeyState(VK_CONTROL)<0)
				{
					ImportClipBoard();
					return true;
				}
		}
		return true;
	}

	int XGridSheetSvrEx::ImportString(LPCTSTR pStr,LPCTSTR pType,bool Header)
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
		
		colCount = m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetColCount();
		while(!finish)
		{
			//insert data row
			m_pGridReport->GetGridDataSvr()->m_pContentSvr->InsertRow(0);
			row = m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetRowCount();
			int ncol;
			for(col=1;col<=colCount;col++)
			{
				ncol = col;//GetVColumn(col);
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
				m_pGridReport->GetGridDataSvr()->m_pContentSvr->SetItemString(ncol,row,buffer);
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

	int XGridSheetSvrEx::ImportClipBoard()
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

	bool XGridSheetSvrEx::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		if(point.x < this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0) && 
			point.y < this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0)) 
		{
			m_pGridReport->m_bWheelVert = !m_pGridReport->m_bWheelVert;
			::InvalidateRect(GetHWND(),NULL,false);
			return true;
		}
		return false;
	}

	XGridSheetSvrEx::~XGridSheetSvrEx(void)
	{
	}

	int XGridSheetSvrEx::TemplateLoaded(IXMLDOMElement * pElement,int state)
	{
		/*this->m_pGridReport->ShowGrid(false);
		this->m_pGridReport->SetRunState(goRunState);
		this->m_state = goRunState;*/

		int nTitleheight = ((XGridHeaderEx*)m_pColHeaderSvr)->GetTitleHeight();
		if( nTitleheight >0)
		{
			((XGridDataSvrEx *)m_pDataSvr)->m_pAxisSvr->SetRowHeight(0,nTitleheight);
			m_pColHeaderSvr->FRect.bottom = m_pColHeaderSvr->FRect.top + nTitleheight;
		}

		int nDetailheight = ((CGridDataSvr *)m_pGridReport->m_pDataTempl)->m_ReportBand->GetDetailHeight(m_pGridReport->m_pDataTempl->m_pAxisSvr);
		((XGridDataSvrEx *)m_pDataSvr)->m_pAxisSvr->SetRowHeight(1,1<<30,nDetailheight);
		SetFRect(FRect);
		return 1;
	}

	int XGridSheetSvrEx::LoadTemplate(IXMLDOMElement * pElement,int state)
	{
		CGridDataSvr * pDataSvr = new CGridDataSvr();
		this->m_pSerializeSvr->LoadSheetData(NULL,pElement,pDataSvr);
		((XGridHeaderEx*)m_pColHeaderSvr)->m_pDataTempl = pDataSvr;
		((XGridSummury*)m_pXGridSummurySvr)->m_pDataTempl = pDataSvr;
		m_pGridReport->m_pDataTempl = pDataSvr;
		
		int mCol,mRow;
		m_pGridReport->m_pDataTempl->m_pContentSvr->GetValidRowCol(mCol,mRow);
		for(int i=1;i<=mCol ;i++)
		{
			((XGridDataSvrEx *)m_pDataSvr)->m_pAxisSvr->SetColWidth(i,pDataSvr->m_pAxisSvr->GetColWidth(i));
		}
		TemplateLoaded(pElement,state);

		CReportBandSvr::TReportBand * pReportBand = & pDataSvr->m_ReportBand->m_ReportBand;
		int startRow,endRow;
		int vCol;
		startRow = pReportBand->Detail.startRow;
		endRow = pReportBand->Detail.endRow - 1;
		PStrItem pItem;

		xbXml xml;
		IXMLDOMElement * pDsElement;
		IXMLDOMElement * pColElement;
		xml.LoadXml(_T("<datasource></datasource>"));
		xml.m_pXmlDoc->get_documentElement(&pDsElement);
		pItem = pDataSvr->m_pContentSvr->GetItemData(0,1);
		if(pItem && pItem->pStrPre)
		{
			pDsElement->setAttribute(L"xroot",(_variant_t)pItem->pStrPre);
		}

		xml.m_pXmlDoc->createElement(L"col",&pColElement);
		pColElement->setAttribute(L"xpath",(_variant_t)L"@guid");
		pColElement->setAttribute(L"name",(_variant_t)L"guid");
		pDsElement->appendChild(pColElement,NULL);
		pColElement->Release();

		for(int row = startRow;row <= endRow ;row++)
		{
			vCol = pDataSvr->m_pContentSvr->GetValidCol(row);
			for(int col = 1;col <= vCol;col++)
			{
				pItem = pDataSvr->m_pContentSvr->GetItemData(col,row);
				if(pItem && /*pItem->pStrGroup &&*/ pItem->pStrColumn)
				{
					xml.m_pXmlDoc->createElement(L"col",&pColElement);
					pColElement->setAttribute(L"xpath",(_variant_t)pItem->pStrColumn);
					pColElement->setAttribute(L"name",(_variant_t)pItem->pStrColumn);
					pDsElement->appendChild(pColElement,NULL);
					pColElement->Release();
				}
			}
		}
		((XXmlContentSvr *)((CGridDataSvr *)m_pDataSvr)->m_pContentSvr)->Initial(pDsElement);
		pDsElement->Release();
		SetFRect(FRect);
		return 1;
	}
	int XGridSheetSvrEx::SetFRect(RECT rect)
	{
		XOfficeSheetSvr::SetFRect(rect);

		RECT rt = rect;

		if(m_pSheetCtrl->m_pVScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rect.bottom -= CXScrollBar::GetHScrollBarHeight();
		}
		if(m_pSheetCtrl->m_pVScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rect.right  -= CXScrollBar::GetVScrollBarWidth();
		}

		int titleHeight =  m_pColHeaderSvr->GetTitleHeight();
		RECT	rectex;
		rectex = rect;
		rectex.top	 = rect.top + titleHeight + 2;
		rectex.right = rect.left + this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
		this->m_pSelectorSvr->FRect = rectex;

		
		rectex = rect;
		rectex.bottom	 = rect.top + titleHeight;
		rectex.left = rect.left + this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0) + 2;
		this->m_pColHeaderSvr->FRect = rectex;

		rectex = rect;
		rectex.top	 = rect.top + titleHeight + 3 ;
		rectex.left = rect.left + this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0) + 2; 
		this->m_pGridReport->FRect = rectex;

		this->FRect = rt;

		return 1;
	}

	HWND XGridSheetSvrEx::GetHWND()
	{
		return this->m_pSheetCtrl->GetHWND();
	}

	int XGridSheetSvrEx::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{

		RECT rect;
		if(pDrawRect)
			rect = *pDrawRect;
		else
			rect = FRect;

		RECT rt = rect;
		if(m_pSheetCtrl->m_pVScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rect.bottom -= CXScrollBar::GetHScrollBarHeight();
		}
		if(m_pSheetCtrl->m_pVScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rect.right  -= CXScrollBar::GetVScrollBarWidth();
		}

		DrawConer(hPaintDC,rect);
		DrawBackGround(hPaintDC,m_pGridReport->FRect);

		if(m_pGridReport)
			m_pGridReport->OnDraw(hPaintDC,rect);
		if(m_pColHeaderSvr)
			m_pColHeaderSvr->OnDraw(hPaintDC,rect);
		if(m_pSelectorSvr)
			m_pSelectorSvr->OnDraw(hPaintDC,rect);
		int nSummuryBottom = rect.bottom;
		if(m_pXGridSummurySvr)
			m_pXGridSummurySvr->OnDraw(hPaintDC,rect);

		RECT rectex;
		if(m_pSheetCtrl->m_pVScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rectex = rt;
			rectex.left = rt.right - CXScrollBar::GetVScrollBarWidth();
			rectex.bottom = rt.bottom - CXScrollBar::GetHScrollBarHeight();

			m_pSheetCtrl->m_pVScrollBar->FRect = rectex;
			m_pSheetCtrl->m_pVScrollBar->OnDraw(hPaintDC);
		}

		if(m_pSheetCtrl->m_pVScrollBar && m_pSheetCtrl->m_Standalone)
		{
			rectex = rt;
			rectex.top = rt.bottom - CXScrollBar::GetHScrollBarHeight();
			rectex.right = rt.right - CXScrollBar::GetVScrollBarWidth();

			m_pSheetCtrl->m_pHScrollBar->fnBar = SB_HORZ;
			m_pSheetCtrl->m_pHScrollBar->FRect = rectex;
			m_pSheetCtrl->m_pHScrollBar->OnDraw(hPaintDC);
		}

		//树形
		int hCols=m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
		int sCol = m_pGridReport->CalcColFromPoint(m_pGridReport->FTopLeft.x + hCols + 2, 0);
		if(sCol<1) sCol=1;
		vector<XXmlContentSvr::TTreeItem> * pTreeItems = NULL;
		int treeret = m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetTreeData(pTreeItems);
		if(treeret >= 0 && (int)pTreeItems->size() > 0 && (*pTreeItems)[0].nContentCol == sCol)
		//if(m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetTreeData(pTreeItems)>=0 && sCol==1)
		{
			::SetBkMode(hPaintDC,TRANSPARENT);

			//::SetROP2(hPaintDC,R2_NOT); 
			RECT rc = rect;

			int startX = rc.left;
			int startY = rc.top + 1;
			int i=1,nIndex=-1;
			int ARow=0;
			int ACol=0;
			int Rows=m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetRowCount();
			int hRows=m_pColHeaderSvr->GetTitleHeight();//m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0);			
			int hRow=0;
			int sRow = m_pGridReport->CalcRowFromPoint(0,m_pGridReport->FRect.top + hCols) - 2;// m_pGridReport->FTopLeft.y);

			int m_nCurRow = m_pGridReport->GetRow();

			if(sRow<0) sRow=0;
			hCols += 16;
			int LineY = startY;
			int j=sRow - 1;
			int sRow2 = -1;
			int k;
			for(k=1; k<=Rows;k++)
			{
				if(!m_pGridReport->IsTreeRowShow(k)) continue;
				sRow2++;
				if(sRow2 == sRow) break;
			}
			sRow = k - 1;
			for(i=sRow;i<Rows;i++)
			{
				nIndex = (int)pTreeItems->size() - Rows + i;

				if(!m_pGridReport->IsTreeRowShow(1 + i)) continue;
				j++;

				ARow = (*pTreeItems)[nIndex].nContentRow;
				ACol = (*pTreeItems)[nIndex].nContentCol;

				hRow=m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(i) - 2;// + 1;
				LineY += hRow;

				startX = rc.left;
				RECT	trect;
				trect.left = startX + hCols;
				trect.top = startY + hRows;
				trect.bottom = trect.top + hRow; 
				trect.right = trect.left + 16;
				hRows += hRow;
				if(trect.bottom > rect.bottom/* + hRow*/) break;

				int nIndent=0;
				int nIndentEx=0;
				int nLevelIndent = 16;

				const XUtil::Css::TBoxStyle * pStyleEx = m_pGridReport->GetGridDataSvr()->m_pStyleSvr->GetBoxStyle(i + 1);
				XUtil::Css::TBoxStyle* pStyle = (XUtil::Css::TBoxStyle*)pStyleEx;

				if(m_pGridReport->FAskMask)
				{
					DWASKSTYLENMHDR hdr;
					memset(&hdr,0,sizeof(DWASKSTYLENMHDR));
					hdr.hwndFrom = m_pGridReport->GetHWND();
					hdr.idFrom = 0;
					hdr.code = DWV_ASKSTYLE;
					hdr.row = j + 1;
					hdr.col = ACol;
					hdr.colname = (LPTSTR)m_pGridReport->GetDwColumnAtCellColumn(ACol);
					hdr.rt = trect;
					::SendMessage(::GetParent(GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
					if(hdr.mask & DMS_CSSSTYLE)
					{
						XUtil::Css::TBoxStyle* pStyle1 =  m_pGridReport->m_pDataTempl->GetStyleSvr()->m_pCssStyle->ParseCssStyle(hdr.pCssStyle);
						*pStyle1 = (*pStyle1) + (*pStyle);
						pStyle = pStyle1;
					}
				}

				HBRUSH hBrush = ::CreateSolidBrush(RGB(242,248,242));
				//::InflateRect(&trect,-1,-1);
				RECT trectEx = trect;
				trectEx.left -= nLevelIndent + 1;
				trectEx.right = trectEx.left + m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(ACol);
				//::FillRect(hPaintDC,&trectEx,(HBRUSH(RGB(249,0,0))));
				::FillRect(hPaintDC,&trectEx,hBrush);
				DeleteObject(hBrush);

				if(pStyle && (( pStyle->mask & cssMaskBackGround && pStyle->pBackGround)||(pStyle->mask&cssMaskBorder && pStyle->pBorder)))
				{
					XUtil::Css::DrawCssFrame(hPaintDC,&trectEx,pStyle);
				}
				else if(j % 2 == 1)
				{
					HBRUSH  hBrush = ::CreateSolidBrush(RGB(226,229,222));
					RECT frc;
					::SetRect(&frc,trectEx.left ,trectEx.top ,trectEx.right,trectEx.bottom );
					::FillRect(hPaintDC,&frc,hBrush);
					::DeleteObject(hBrush);
				}
				if(m_pGridReport->IsRowSelected(j + 1))
				{
					RECT frc;
					::SetRect(&frc,trectEx.left+1 ,trectEx.top ,trectEx.right+1,trectEx.bottom );
					HBRUSH hBrush = ::CreateSolidBrush(m_pGridReport->m_SelectBkColor);
					::InflateRect(&frc,-1,-1);
					::FillRect(hPaintDC,&frc,hBrush);
					DeleteObject(hBrush);
				}
				nIndent = ((*pTreeItems)[nIndex].nLevel)*nLevelIndent;
				if((*pTreeItems)[nIndex].nImage>=0)
				{
					if((*pTreeItems)[nIndex].nImageList >=0 && (*pTreeItems)[nIndex].nImageList < m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->m_pResourceMgr->m_hImageLists.size())
					{
						HIMAGELIST hImageList = m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->m_pResourceMgr->m_hImageLists[(*pTreeItems)[nIndex].nImageList];
						int cx=16,cy=16;
						ImageList_GetIconSize(hImageList,&cx,&cy);
						::ImageList_Draw(hImageList,(*pTreeItems)[nIndex].nImage,hPaintDC,trect.left + nIndent,trect.top + (trect.bottom - trect.top)/2 - cy/2,ILD_NORMAL);
						nIndentEx = cx + 2;
					}
					int py = trect.top + (trect.bottom - trect.top)/2;
					int px = trect.left + 7 + nIndent - nLevelIndent - 3;
					if((*pTreeItems)[nIndex].children)
					{
						HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(120,152,181));
						if((trect.bottom - trect.top) %2 ==1)py ++;

						//draw border
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						::MoveToEx(hPaintDC,px,py - 4,NULL);
						::LineTo(hPaintDC,px + 8,py - 4);
						::LineTo(hPaintDC,px + 8,py + 4);
						::LineTo(hPaintDC,px,py + 4);
						::LineTo(hPaintDC,px,py - 4);
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						::DeleteObject(hPen);

						hPen = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						if((*pTreeItems)[nIndex].expanded)
						{
							//draw sub
							::MoveToEx(hPaintDC,px + 2,py,NULL);
							::LineTo(hPaintDC,px + 7,py);
						}else
						{
							//draw plus
							::MoveToEx(hPaintDC,px + 2,py,NULL);
							::LineTo(hPaintDC,px + 7,py);
							::MoveToEx(hPaintDC,px + 4,py - 2,NULL);
							::LineTo(hPaintDC,px + 4,py + 3);
							/*
							if(XXmlContentSvr::GetTreeNextItem(pTreeItems,nIndex)>=0)
							{
								int px1 = trect.left + 4;
								::MoveToEx(hPaintDC,px1 + 4,trect.top + py + 9,NULL);
								::LineTo(hPaintDC,px1 + 4,9);
							}
							*/
						}
						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						::DeleteObject(hPen);

						LOGBRUSH   lb;   
						DWORD   nPen[2];   

						lb.lbStyle   =   BS_SOLID;   
						lb.lbColor   =   RGB(0,0,0);   
						nPen[0]   =   0;                             //这两个参数的设置与msdn中不一致，msdn中说的是一个点的宽度，   
						nPen[1]   =   2;                             //后面再接一个间隔的宽度，如果这两个值都设为1，不能满足要求   
						hPen = ExtCreatePen(PS_GEOMETRIC   |   PS_USERSTYLE,   1,   &lb,   2,   nPen); 

						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						//draw hline
						::MoveToEx(hPaintDC,px + 9,py,NULL);
						::LineTo(hPaintDC,px + 18,py);

						//draw top vline
						if(i!=0)
						{
							::MoveToEx(hPaintDC,px + 4,trect.top,NULL);
							::LineTo(hPaintDC,px + 4,py - 5);
						}

						//draw bottom vline
						if(XXmlContentSvr::GetTreeNextItem(pTreeItems,nIndex)>=0)
						{
							::MoveToEx(hPaintDC,px + 4,py + 5,NULL);
							::LineTo(hPaintDC,px + 4,trect.bottom + 1);
						}

						int pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,nIndex);
						int nIndent1,px1;
						while(pIndex >= 0)
						{
							if(XXmlContentSvr::GetTreeNextItem(pTreeItems,pIndex)>=0)
							{
								nIndent1 = ((*pTreeItems)[pIndex].nLevel)*nLevelIndent;
								px1 = trect.left + 7 + nIndent1 - nLevelIndent - 3;
								::MoveToEx(hPaintDC,px1 + 4,trect.top,NULL);
								::LineTo(hPaintDC,px1 + 4,trect.bottom);
							}
							pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,pIndex);
						}

						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						::DeleteObject(hPen);
					}
					else
					{
						if((trect.bottom - trect.top) %2 ==1)py ++;


						LOGBRUSH   lb;   
						DWORD   nPen[2];   

						lb.lbStyle   =   BS_SOLID;   
						lb.lbColor   =   RGB(0,0,0);   
						nPen[0]   =   0;                             //这两个参数的设置与msdn中不一致，msdn中说的是一个点的宽度，   
						nPen[1]   =   2;                             //后面再接一个间隔的宽度，如果这两个值都设为1，不能满足要求   
						HPEN hPen = ExtCreatePen(PS_GEOMETRIC   |   PS_USERSTYLE,   1,   &lb,   2,   nPen); 

						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						int nNextItem = XXmlContentSvr::GetTreeNextItem(pTreeItems,nIndex);
						if(nNextItem < 0)
						{
							if(i!=0)
							{
								::MoveToEx(hPaintDC,px + 4,trect.top,NULL); 
								::LineTo(hPaintDC,px + 4,py);
							}
						}else
						{
							if(i!=0)
								::MoveToEx(hPaintDC,px + 4,trect.top,NULL);
							else
								::MoveToEx(hPaintDC,px + 4,py,NULL);
							::LineTo(hPaintDC,px + 4,trect.bottom + 1);
						}
						::MoveToEx(hPaintDC,px + 5,py,NULL);
						::LineTo(hPaintDC,px + 5 + 10,py);

						int pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,nIndex);
						int nIndent1,px1;
						if(i>0)
						while(pIndex >= 0)
						{
							if(XXmlContentSvr::GetTreeNextItem(pTreeItems,pIndex)>=0)
							{
								nIndent1 = ((*pTreeItems)[pIndex].nLevel)*nLevelIndent;
								px1 = trect.left + 7 + nIndent1 - nLevelIndent - 3;
								::MoveToEx(hPaintDC,px1 + 4,trect.top + 1,NULL);
								::LineTo(hPaintDC,px1 + 4,trect.bottom);
							}
							pIndex = XXmlContentSvr::GetTreeParentItem(pTreeItems,pIndex);
						}

						hPen = (HPEN)::SelectObject(hPaintDC,hPen);
						::DeleteObject(hPen);
					}
					//2017.8.23 LPCTSTR pStr = m_pGridReport->GetItemDisplayString(j + 1, ACol + 1);
					LPCTSTR pStr = m_pGridReport->GetItemDisplayString(j + 1, ACol);
					//pStr = m_pDataSvr->m_pContentSvr->GetItemString((*pTreeItems)[nIndex].nContentCol,i);
					//2017.8.23 if(!pStr) pStr = m_pGridReport->GetItemString(j + 1, ACol + 1);
					if(!pStr) pStr = m_pGridReport->GetItemString(j + 1, ACol);
					if(pStr)
					{
						COLORREF color  = ::GetTextColor(hPaintDC);
						COLORREF pcolor  = color;

						LOGFONT	LogFont;
						HFONT hFont = NULL;
						if(pStyle && pStyle->mask & cssMaskFont)
						{
							XUtil::Css::GetFont(pStyle,LogFont);
							hFont = ::CreateFontIndirect(&LogFont);
							hFont = (HFONT)::SelectObject(hPaintDC,hFont);
						}else
						{
							XUtil::Css::GetDefaultFont(LogFont);					
							hFont = ::CreateFontIndirect(&LogFont);
							hFont = (HFONT)::SelectObject(hPaintDC,hFont);
						}

						if(pStyle)
						{
							if(pStyle->pColor && pStyle->mask & cssMaskColor)
							{
								if(!m_pGridReport->IsRowSelected(j + 1) && (j + 1)!=m_nCurRow)
									::SetTextColor(hPaintDC,pStyle->pColor->color[0]);
							}
						}

						/*LOGFONT	LogFont;
						XUtil::Css::GetFont(pStyle,LogFont);
						HFONT	hFont = ::CreateFontIndirect(&LogFont);
						hFont = (HFONT)SelectObject(hPaintDC,hFont);*/

						if(m_pGridReport->IsRowSelected(j + 1))
							color = ::SetTextColor(hPaintDC,RGB(255,255,255));
						else if(j == m_nCurRow - 1)
							::SetTextColor(hPaintDC,RGB(0,0,255));


						int pLev = ((*pTreeItems)[nIndex].nLevel + 1) * nLevelIndent + 4;
						trect.right = trect.left - 16
							+ m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth((*pTreeItems)[nIndex].nContentCol);
						trect.left += pLev;
						::DrawText(hPaintDC,pStr,(int)_tcslen(pStr),&trect,DT_WORDBREAK|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX);

						::SetTextColor(hPaintDC,pcolor);
						hFont = (HFONT)SelectObject(hPaintDC,hFont);
						::DeleteObject(hFont);
					}
				}
			}
		}

		//line
		HPEN hRPen1 = CreatePen(PS_SOLID,1,RGB(159,171,187));
		hRPen1 = (HPEN)SelectObject(hPaintDC,hRPen1);
		hCols=m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
		int Rows=m_pGridReport->GetRowCount();
		int hRow=m_pColHeaderSvr->GetTitleHeight();
		int h = rect.bottom - rect.top;
		int w = rect.right - rect.left;
		int fw = 0;
		//冻结
		if(m_pColHeaderSvr->m_nFECol > 0)
		{
			for(int i=m_pGridReport->FTopLeft.y; i<=Rows; i++)
			{
				if(!m_pGridReport->IsTreeRowShow(i)) continue;

				int Row = m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(i) - 2;

				int Col = m_pColHeaderSvr->m_nFSCol;
				int Y = m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
				while(Y <= rt.right && Col <= m_pColHeaderSvr->m_nFECol)
				{
					int Y1 = m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(Col) + 1;
					Y += Y1;
					Col++;
					if(Y1 > 1)
					{
						::MoveToEx(hPaintDC,Y,hRow, NULL);
						if(hRow + Row > h)
							::LineTo(hPaintDC,Y,h);
						else
							::LineTo(hPaintDC,Y,hRow + Row);
					}
					if(Col >= m_pGridReport->GetColCount()) break;
				}
				hRow += Row;
				if(hRow > h) break;
				if(Y > w) Y = w;
				::MoveToEx(hPaintDC,hCols,hRow, NULL);
				::LineTo(hPaintDC,Y,hRow);

				fw = Y;
			}
		}
		hRow=m_pColHeaderSvr->GetTitleHeight();
		int sRow=m_pGridReport->FTopLeft.y;
		if(m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetTreeData(pTreeItems)>=0 && sCol==1)
		{
			int sRow2 = 0;
			int k;
			for(k=1; k<=Rows;k++)
			{
				if(!m_pGridReport->IsTreeRowShow(k)) continue;
				sRow2++;
				if(sRow2 == sRow) break;
			}
			sRow = k;
		}
		for(int i=sRow; i<=Rows; i++)
		{
			if(!m_pGridReport->IsTreeRowShow(i)) continue;

			int Row = m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(i) - 2;

			int Col = m_pGridReport->FTopLeft.x;
			int Y = (fw > 0)?fw : m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
			while(Y <= rt.right)
			{
				int Y1 = m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(Col) + 1;
				Y += Y1;
				Col++;
				if(Y1 > 1)
				{
					::MoveToEx(hPaintDC,Y,hRow, NULL);
					if(hRow + Row > h)
						::LineTo(hPaintDC,Y,h);
					else
						::LineTo(hPaintDC,Y,hRow + Row);
				}
				if(Col > m_pGridReport->GetColCount()) break;
			}
			hRow += Row;
			if(hRow > h) break;
			if(Y > w) Y = w;
			::MoveToEx(hPaintDC,hCols,hRow, NULL);
			::LineTo(hPaintDC,Y,hRow);
		}
		hRPen1 = (HPEN)SelectObject(hPaintDC,hRPen1);
		::DeleteObject(hRPen1);

		//冻结黑线
		if(fw > 0)
		{
			HPEN hRPen2 = CreatePen(PS_SOLID,1,RGB(0,0,0));
			hRPen2 = (HPEN)SelectObject(hPaintDC,hRPen2);
			::MoveToEx(hPaintDC,fw,0, NULL);
			::LineTo(hPaintDC,fw,hRow);
			hRPen2 = (HPEN)SelectObject(hPaintDC,hRPen2);
			::DeleteObject(hRPen2);
		}
		rect.bottom = nSummuryBottom;

		return 1;
	}

	void XGridSheetSvrEx::DrawBackGround(HDC hdc, RECT &rect)
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
			rect.top -= 2;
			HBRUSH hBrush = ::CreateSolidBrush(RGB(225,229,238));
			::FillRect(hdc,&rect,hBrush);
			::DeleteObject(hBrush);
			rect.top += 2;
		}
		/*
		TRIVERTEX        vert [4] ;
		GRADIENT_TRIANGLE    gTRi[2];
		vert [0] .x       =  rect.left;
		vert [0] .y       =  rect.top ;
		vert [0] .Red     =  0xff00;
		vert [0] .Green   =  0xff00;
		vert [0] .Blue    =  0xff00;
		vert [0] .Alpha   =  0x0000;

		vert [1] .x       =  rect.right;
		vert [1] .y       =  rect.top;
		vert [1] .Red     =  0x5b00;
		vert [1] .Green   =  0x6d00;
		vert [1] .Blue    =  0x7700;
		vert [1] .Alpha   =  0x0000;

		vert [2] .x       =  rect.right;
		vert [2] .y       =  rect.bottom; 
		vert [2] .Red     =  0xff00;
		vert [2] .Green   =  0xff00;
		vert [2] .Blue    =  0xff00;
		vert [2] .Alpha   =  0x0000;

		vert [3] .x       =  rect.left;
		vert [3] .y       =  rect.bottom;
		vert [3] .Red     =  0x5b00;
		vert [3] .Green   =  0x6d00;
		vert [3] .Blue    =  0x7700;
		vert [3] .Alpha   =  0x0000;

		gTRi[0].Vertex1   = 0;
		gTRi[0].Vertex2   = 1;
		gTRi[0].Vertex3   = 3;

		gTRi[1].Vertex1   = 1;
		gTRi[1].Vertex2   = 2;
		gTRi[1].Vertex3   = 3;
		GradientFill(hdc,vert,4,&gTRi,2,GRADIENT_FILL_TRIANGLE);*/
	}
		
	void XGridSheetSvrEx::DrawConer(HDC hdc,RECT &rect,COLORREF color)
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

		vertex[1].x = rect.left + this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
		vertex[1].y = rect.top + this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0) ;
		//vertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) + 25)*256;
		//vertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) + 25)*256;
		//vertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color)+25)*256;
		vertex[1].Red  =  GetRValue(color)*256;
		vertex[1].Green = GetGValue(color)*256;
		vertex[1].Blue =  GetBValue(color)*256;

		grect[0].UpperLeft = 0;
		grect[0].LowerRight = 1;

		GradientFill(hdc, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);

		POINT points[3];
		HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(128,128,128));
		hPen = (HPEN)::SelectObject(hdc,hPen);
		HBRUSH hBrush = ::CreateSolidBrush(RGB(0,0,128));
		hBrush = (HBRUSH)::SelectObject(hdc,hBrush);

		RECT ARect = rect;
		ARect.right  =  rect.left + this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetColWidth(0);
		ARect.bottom = rect.top + this->m_pGridReport->GetGridDataSvr()->m_pAxisSvr->GetRowHeight(0);
		if(m_pGridReport->m_bWheelVert)
		{
			points[0].y = ARect.bottom - 3;
			points[0].x = ARect.right - 7;
			points[1].y = ARect.bottom - 8;
			points[1].x = ARect.right - 9;
			points[2].y = ARect.bottom - 8;
			points[2].x = ARect.right - 4;
		}else
		{
			points[0].y = ARect.bottom - 7;
			points[0].x = ARect.right - 3;
			points[1].y = ARect.bottom - 9;
			points[1].x = ARect.right - 8;
			points[2].y = ARect.bottom - 4;
			points[2].x = ARect.right - 8;
		}

		Polygon(hdc, points,3);
		hPen = (HPEN)::SelectObject(hdc,hPen);
		::DeleteObject(hPen);
		hBrush = (HBRUSH)::SelectObject(hdc,hBrush);
		::DeleteObject(hBrush);
	}
	
	CXScrollBar * XGridSheetSvrEx::GetHScrollBar()
	{
		return m_pSheetCtrl->m_pHScrollBar;
	}
	CXScrollBar * XGridSheetSvrEx::GetVScrollBar()
	{
		return m_pSheetCtrl->m_pVScrollBar;
	}

	int XGridSheetSvrEx::InitDrawInfo(TGridDrawInfoEx * pDrawInfo)
	{
		::memset(pDrawInfo,0,sizeof(TGridDrawInfo));

		//set HWND
		if(m_pSheetCtrl)
			pDrawInfo->hWnd = m_pSheetCtrl->m_hWnd;
		
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

		pDrawInfo->HSizeBarPos = -1;
		pDrawInfo->VSizeBarPos = -1;
		pDrawInfo->VScrollbarWidth = GetSystemMetrics(SM_CXVSCROLL);
		pDrawInfo->hScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);

		pDrawInfo->pSheetSvr = this;
		pDrawInfo->pAxisSvr = m_pGridReport->GetGridDataSvr()->m_pAxisSvr;

		pDrawInfo->ColHeaderHeight = pDrawInfo->pAxisSvr->GetRowHeight(0);
		pDrawInfo->RowSelectorWidth = pDrawInfo->pAxisSvr->GetColWidth(0);

//		pDrawInfo->FAnchor  = m_pGridReport->FAnchor;
		pDrawInfo->FCurrent = m_pGridReport->FCurrent;
		
		pDrawInfo->EffectiveLineWidth =  m_pGridReport->FGridLineWidth;
		pDrawInfo->FGridState = m_pGridReport->FGridState;
		pDrawInfo->FOptions = m_pGridReport->FOptions;
//		pDrawInfo->FSelections = m_pGridReport->FSelections.GetBuffer();
//		pDrawInfo->FSelectionSize = m_pGridReport->FSelections.size();

		pDrawInfo->FColCount = m_pGridReport->GetColCount();
		pDrawInfo->FRowCount = m_pGridReport->GetRowCount();
		pDrawInfo->FTopLeft = m_pGridReport->FTopLeft;
//		pDrawInfo->FTopLefts = m_pGridReport->FTopLefts;

		//pDrawInfo->FrameRect = m_AreaRect;
		pDrawInfo->GridRect = m_pGridReport->FRect;
		//pDrawInfo->GridSheetPageNavWidth = 4*GetSystemMetrics(SM_CYHSCROLL);
		//pDrawInfo->GridSheetPageHeight = GetSystemMetrics(SM_CYHSCROLL);
		//pDrawInfo->GridSheetPageWidth = (m_AreaRect.right - m_AreaRect.left) /2 -  pDrawInfo->GridSheetPageNavWidth;
		
		pDrawInfo->GridRect.left += m_pGridReport->m_pSheetSvr->m_pColHeaderSvr->GetFColumnWidth() + 1;

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
			Line += pDrawInfo->EffectiveLineWidth + pDrawInfo->pAxisSvr->GetRowHeight(i);
			if(Line > pDrawInfo->GridRect.bottom)break;
			pDrawInfo->FullVisBoundaryRow = Line;
		}
        pDrawInfo->LastFullVisibleRow = i - 1;

		return 1;
	}


}}}}
