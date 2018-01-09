#include "HtTableView.hpp"
#include "httabledata.hpp"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	CHtTableView::CHtTableView(void)
	{
		Initial();
	}

	CHtTableView::~CHtTableView(void)
	{
	}

	int	CHtTableView::Initial()
	{
		FTopLeft.x = 0;
		FTopLeft.y = 0;

		FCurrent.x = 1;
		FCurrent.y = 1;

		FAnchor.x = 1;
		FAnchor.y = 1;

		FFixed.x = 0;
		FFixed.y = 0;

		FGridLineWidth = 1;

		return 1;
	}

	IHtTableData * CHtTableView::SetTableData(IHtTableData * pData)
	{
		IHtTableData * pOldData = m_pTableData;
		m_pTableData = pData;
		return pOldData;
	}

	IHtTableData * CHtTableView::GetTableData()
	{
		return m_pTableData;
	}

	int CHtTableView::CalcRowFromPoint(int X, int Y)
	{
		RECT FRect ={0,0,0,0};

		if(Y < FRect.top + 1) return -1;
		int LineY = FRect.top + 1; 
		int ARow = this->FTopLeft.y;
		LineY += (GetTableData()->GetAxis()->GetRowHeight(ARow)<1?0:GetRowHeight(ARow));
		while(LineY < Y) 
		{
			ARow++;
			if(ARow > this->GetRowCount())break;
			if(GetRowHeight(ARow) < 1) continue;
			LineY += GetTableData()->GetAxis()->GetRowHeight(ARow) + FGridLineWidth;
		}
		return ARow;
	}
	int CHtTableView::CalcColFromPoint(int X, int Y)
	{
		RECT FRect ={0,0,0,0};

		if(X < FRect.left + 1) return -1;
		int LineX = FRect.left + 1;
		int ACol = this->FTopLeft.x;

		int ARow = CalcRowFromPoint(X,Y);
		LineX += GetTableData()->GetAxis()->GetColWidth(ACol);
		while(LineX < X )
		{
			ACol++;
			if(ACol > this->GetColCount())break;
			LineX += GetTableData()->GetAxis()->GetColWidth(ACol)  + this->FGridLineWidth;
		}
		return ACol;
	}

	THtGridCoord CHtTableView::CalcCoordFromPoint(int X, int Y)
	{
		THtGridCoord Coord;

		Coord.y = CalcRowFromPoint(X,Y);
		Coord.x = CalcColFromPoint(X,Y);

		return Coord;
	}

	int CHtTableView::GetHeight()
	{
		int nRow = m_pTableData->GetContent()->GetRowCount();
		return m_pTableData->GetAxis()->GetSumRowHeight(1,nRow);
	}

	int CHtTableView::GetWidth()
	{
		int nCol = m_pTableData->GetContent()->GetColCount();
		return m_pTableData->GetAxis()->GetSumColWidth(1,nCol);
	}

	int CHtTableView::GetRowHeight(int nRow)
	{
		return m_pTableData->GetAxis()->GetRowHeight(nRow);
	}

	int CHtTableView::GetColWidth(int nCol)
	{
		return m_pTableData->GetAxis()->GetColWidth(nCol);
	}

	LPCTSTR CHtTableView::GetItemString(int nCol,int nRow)
	{
		return m_pTableData->GetContent()->GetItemString(nCol,nRow);
	}

	void CHtTableView::SetItemString(int nCol,int nRow,LPTSTR pStr)
	{
		m_pTableData->GetContent()->SetItemString(nCol,nRow,pStr);
	}

	LPIHtContentItem CHtTableView::GetItem(int nCol,int nRow)
	{
		return m_pTableData->GetContent()->GetItemData(nCol,nRow);
	}

	void CHtTableView::SetItem(int nCol,int nRow,LPIHtContentItem pItem)
	{
		m_pTableData->GetContent()->SetItemData(nCol,nRow,pItem);
	}


	RECT CHtTableView::GetCellRect(int nCol,int nRow)
	{
		RECT rt;
		GetCellRect(nCol,nRow,rt);
		return rt;
	}

	int  CHtTableView::GetClientWidth()
	{
		return 1200;
	}

	int CHtTableView::GetClientHeight()
	{
		return 2400;
	}

	void CHtTableView::CalcDrawInfo(THtDrawInfo &DrawInfo)
	{
		CalcDrawInfoXY(DrawInfo, GetClientWidth(), GetClientHeight());
	}

	void CHtTableView::CalcDrawInfoXY(THtDrawInfo &DrawInfo,int  UseWidth,int  UseHeight)
	{
		DrawInfo.Horz.pAxis = m_pTableData->GetAxis()->GetColAxis();
		DrawInfo.Vert.pAxis = m_pTableData->GetAxis()->GetRowAxis();
		CalcFixedInfo(DrawInfo);
		CalcAxis(DrawInfo.Horz, UseWidth);
		CalcAxis(DrawInfo.Vert, UseHeight);
	}

	void CHtTableView::CalcAxis(THtAxisInfo &AxisInfo, int UseExtent)
	{
		AxisInfo.GridExtent = UseExtent;
		AxisInfo.GridBoundary = AxisInfo.FixedBoundary;
		AxisInfo.FullVisBoundary = AxisInfo.FixedBoundary;
		AxisInfo.LastFullVisibleCell = AxisInfo.FirstGridCell;
		for(int i=AxisInfo.FirstGridCell;i<=AxisInfo.GridCellCount - 1;i++){
			AxisInfo.GridBoundary += AxisInfo.GetExtent(i) + AxisInfo.EffectiveLineWidth;
			if(AxisInfo.GridBoundary > AxisInfo.GridExtent + AxisInfo.EffectiveLineWidth)
			{
				AxisInfo.GridBoundary = AxisInfo.GridExtent;
				break;
			}
			AxisInfo.LastFullVisibleCell = i;
			AxisInfo.FullVisBoundary = AxisInfo.GridBoundary;
		}
	}

	void CHtTableView::CalcFixedInfo(THtDrawInfo &DrawInfo)
	{
		CalcFixedAxis(DrawInfo.Horz, GetFixedCols(),GetLeftCol(), GetColCount());
		CalcFixedAxis(DrawInfo.Vert, GetFixedRows(),GetTopRow(),  GetRowCount());
	}

	void CHtTableView::CalcFixedAxis(THtAxisInfo &Axis ,int FixedCount,int  FirstCell,int  CellCount)
	{
		Axis.EffectiveLineWidth = GetGridLineWidth();

		Axis.FixedBoundary = 0;
		for(int i= 0 ;i<= FixedCount - 1;i++)
			Axis.FixedBoundary +=  Axis.pAxis->GetExtent(i) + Axis.EffectiveLineWidth;

		Axis.FixedCellCount = FixedCount;
		Axis.FirstGridCell =  FirstCell;
		Axis.GridCellCount =  CellCount;
	}
	

	THtGridRect CHtTableView::GetSelection()
	{
		THtGridRect rect;
		::SetRectEmpty(&rect);
		return rect;
	}


	int CHtTableView::DoDrawTo(HDC hdc,RECT &rc, int startCol,int startRow)
	{
		//DrawBackGround(hdc,rc);
		//DrawGridLine(hdc,rc,startCol,startRow,GetGridLineWidth(),RGB(217,217,217));
		DrawBorderLines(hdc,rc,startCol,startRow,GetGridLineWidth(),RGB(0,0,0));
		DrawCells(hdc,rc,startCol,startRow,GetGridLineWidth());
		//if(FOptions &goVisibleSelection) DrawSelectionBorder(hdc,rc);
		return 1;
	}

	void CHtTableView::DrawCells(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth)
	{
		IHtTableAxis * pAxis = m_pTableData->GetAxis();
		IHtTableSpan * pSpan = m_pTableData->GetSpan();

		RECT  wRect,sRect;
		RECT FocRect;
		int ARow,ACol;
		int StartX,StartY;
		int StopX,StopY;
		int EndX, EndY;

		StartX = rect.left + GridLineWidth;
		StartY = rect.top  +  GridLineWidth;
		StopX = rect.right;
		StopY = rect.bottom;
		EndX = StartX;
		EndY = StartY;

		::SetRect(&FocRect,0,0,0,0);
		THtDrawInfo AxisInfo;
		CalcDrawInfo(AxisInfo);

		int OffsetX,OffsetY;

		THtGridRect gRect;
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
		HPEN hPen = ::CreatePen(PS_SOLID,this->FGridLineWidth,RGB(217,217,217));//COptionSvr::stdRowSelecterColor0);
		hPen = (HPEN)::SelectObject(hdc,hPen);
		while (wRect.top < StopY && ARow <= GetRowCount())
		{
			ACol = StartCol;
			wRect.left = StartX;
			wRect.bottom = wRect.top + pAxis->GetRowHeight(ARow);
			if(wRect.bottom > StopY)break;

			while (wRect.left < StopX && ACol <= GetColCount())
			{
				THtGridRect gRect;
				if(pSpan->isSpaned(ACol,ARow,gRect))
				{
					BoxRect(AxisInfo,gRect,sRect);
					::OffsetRect(&sRect,- OffsetX,- OffsetY);
					if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
					{
						DrawCell(hdc,ACol, ARow, sRect);
						::MoveToEx(hdc,sRect.left,sRect.bottom,NULL);
						::LineTo(hdc,sRect.right,sRect.bottom);
						::LineTo(hdc,sRect.right,sRect.top - 1);
					}
					ACol = gRect.right;
					wRect.right = sRect.right;
				}else
				{
					wRect.right = wRect.left + pAxis->GetColWidth(ACol);
					DrawCell(hdc,ACol, ARow, wRect);
					::MoveToEx(hdc,wRect.left,wRect.bottom,NULL);
					::LineTo(hdc,wRect.right,wRect.bottom);
					::LineTo(hdc,wRect.right,wRect.top - 1);
				}
				if(wRect.right > StopX)break;
				wRect.left = wRect.right + 1;
				ACol++;
				EndX = EndX > wRect.right ? EndX : wRect.right;
			}
			wRect.top = wRect.bottom + 1;
			ARow++;
			EndY = EndY > wRect.bottom ? EndY : wRect.bottom;
		}
		::MoveToEx(hdc,StartX,EndY,NULL);
		::LineTo(hdc,StartX,StartY);
		::LineTo(hdc,EndX,StartY);
		hPen = (HPEN)::SelectObject(hdc,hPen);
		::DeleteObject(hPen);
	}

	void CHtTableView::DrawCell(HDC hdc,int ACol,int ARow, RECT &ARect)
	{
		IHtTableContent * pContent = m_pTableData->GetContent();
		IHtTableStyle   * pStyle   = m_pTableData->GetStyle();

		HBRUSH brush;
		if(!(ACol==FCurrent.x && ARow ==FCurrent.y))
		{
			if(HtPointInGridRect(ACol, ARow, GetSelection()))
			{
				brush = ::CreateSolidBrush(RGB(169,178,202));
				::FillRect(hdc,&ARect,brush);
				::DeleteObject(brush);
			}
		}

		RECT rect;
		LPTSTR	pItemStr = NULL;
		pItemStr = pContent->GetItemString(ACol,ARow);
		
		const XUtil::Css::TBoxStyle* pBoxStyle = pStyle->GetCellCssStyle(ACol,ARow);
		if(pBoxStyle && pBoxStyle->pBackGround && pBoxStyle->mask & cssMaskBackGround)
		{
			HBRUSH hBrush;
			hBrush = ::CreateSolidBrush(pBoxStyle->pBackGround->color.color[0]);
			::FillRect(hdc,&ARect,hBrush);
			::DeleteObject(hBrush);
		}

		if(pItemStr)
		{
			LOGFONT	LogFont;
			HFONT hFont = NULL;
			if(pStyle->GetFont(ACol,ARow,LogFont) == 1)
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
				::DrawText(hdc,pStr1,nLen1,&rt,DT_CALCRECT);
				int h = rt.bottom - rt.top;
				rt = ARect;
				rt.bottom -= 2;
				rt.top = rt.bottom - h;
				rt.left += 3;
				::DrawTextW(hdc,pStr1,nLen1,&rt,DT_LEFT);

				rt = ARect;
				rt.top += 2;
				rt.right -= 3;
				::DrawTextW(hdc,pStr2,nLen2,&rt,DT_RIGHT);
			}else
			{
				if(_tcschr(pItemStr,'\r'))
					::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,nAlign|DT_EDITCONTROL | DT_WORDBREAK);
				else
				{
					SIZE sz;
					GetTextExtentPoint32(hdc,pItemStr,(int)::_tcslen(pItemStr),&sz);
					if(sz.cx < rect.right - rect.left)
						::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,DT_SINGLELINE |nAlign|DT_VCENTER|DT_EDITCONTROL);
					else
						::DrawTextW(hdc,pItemStr,(int)::_tcslen(pItemStr),&rect,nAlign|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK);
				}
			}
			::SetBkMode(hdc,hbr);
			if(hFont)
			{
				hFont = (HFONT)::SelectObject(hdc,hFont);
				::DeleteObject(hFont);
			}
			::SetTextColor(hdc,TextColor); 
		}
	}
	
	int	 CHtTableView::LinePos(THtAxisInfo &AxisInfo ,int  Line)
	{
		int Start;
		int Result  ;

		Result = 0;
		if(Line < AxisInfo.FixedCellCount)
			Start = 0;
		else
		{
			if(Line >= AxisInfo.FirstGridCell)
				Result = AxisInfo.FixedBoundary;
			Start = AxisInfo.FirstGridCell;
		}
		for(int i= Start;i<= Line - 1;i++)
		{
			Result += AxisInfo.GetExtent(i) + AxisInfo.EffectiveLineWidth;
			if(Result > AxisInfo.GridExtent)
			{
				Result = 0;
				return Result;
			}
		}
		return Result;
	}

	bool CHtTableView::GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan)
	{
		THtDrawInfo AxisInfo;
		CalcDrawInfo(AxisInfo);
		return GetCellRect(AxisInfo,ACol,ARow,rect,bIncludeSpan);
	}

	bool CHtTableView::GetCellRect(THtDrawInfo& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan)
	{
		IHtTableAxis * pAxis = m_pTableData->GetAxis();
		IHtTableSpan * pSpan = m_pTableData->GetSpan();
		
		::SetRect(&rect,0,0,0,0);
		rect.left = LinePos(AxisInfo.Horz,ACol);
		rect.top =  LinePos(AxisInfo.Vert,ARow);

		rect.right  = rect.left + pAxis->GetColWidth(ACol);
		rect.bottom = rect.top  + pAxis->GetRowHeight(ARow);

		THtGridRect	tRect;
		if(bIncludeSpan && pSpan->isSpaned(ACol,ARow,tRect))
		{
			RECT eRect;
			GetCellRect(AxisInfo,tRect.right,tRect.bottom,eRect,false);
			rect.right = eRect.right;
			rect.bottom = eRect.bottom;
		}
		return true;
	}

	bool CHtTableView::BoxRect(THtDrawInfo& AxisInfo,THtGridRect gridRect,RECT &screenRect)
	{
		RECT	sRect,	eRect;

		::SetRect(&screenRect,0,0,0,0);
		HtNormalizeRect(*(RECT *)&gridRect);
		GetCellRect(AxisInfo,gridRect.left,gridRect.top,sRect,false);
		GetCellRect(AxisInfo,gridRect.right,gridRect.bottom,eRect,false);

		::SetRect(&screenRect,sRect.left,sRect.top,eRect.right,eRect.bottom);
		return true;

	}

	bool CHtTableView::CalcSpanRgn(HRGN & hRgn,THtDrawInfo& AxisInfo)
	{
		/*
		int LineX,LineY;
		bool InitRgn = true;
		RECT sRect;
		RECT rect;
		LineY = AxisInfo.Vert.GridRect.top;
		for(int i=AxisInfo.FTopLeft.y ;i<=AxisInfo.LastFullVisibleRow;i++)
		{
			LineX = AxisInfo.GridRect.left;
			for(int j=AxisInfo.FTopLeft.y ;j<=AxisInfo.LastFullVisibleCol;j++)
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
		return InitRgn?false:true;*/
		return true;
	}

	void CHtTableView::DrawGridLine(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth, COLORREF color)
	{
		//if(!(FOptions & goVertLine && FOptions & goHorzLine)) return ;

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
		/*int nIndex = this->GetGridDataSvr()->GetChildIndex(ARow);
		if(nIndex >=0)
			nEndRow = (*GetGridDataSvr()->m_pChilds)[nIndex]->endRow;
		else
		{
			nIndex = this->GetGridDataSvr()->GetChildIndexFollow(ARow);
			if(nIndex >=0)
				nEndRow = (*GetGridDataSvr()->m_pChilds)[nIndex]->startRow - 1;
		}*/

		HPEN	hPen = ::CreatePen(PS_SOLID,this->FGridLineWidth,RGB(217,217,217));//COptionSvr::stdRowSelecterColor0);
		hPen = (HPEN)::SelectObject(hdc,hPen);
		//get vert line count;
		while(LineY <= rect.bottom)
		{
			cy++;
			if(ARow > this->GetRowCount() || (nEndRow >0 && ARow > nEndRow) ) break;
			LineY +=  GetTableData()->GetAxis()->GetRowHeight(ARow) + GridLineWidth;
			ARow++;
		}
		EndY = LineY > rect.bottom ?rect.bottom : LineY;

		//get horz line count
		ACol=StartCol;
		while(LineX <= rect.right)
		{
			cx++;
			if(ACol > this->GetColCount()) break;
			LineX +=  GetTableData()->GetAxis()->GetColWidth(ACol) + GridLineWidth;
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
			Points[cy << 1].x = startX;
			Points[cy << 1].y = LineY;
			Points[(cy << 1) + 1].x = EndX;
			Points[(cy << 1) + 1].y = LineY ;
			StrokeList[cy] = 2;

			cy++;
			if(ARow > this->GetRowCount() || (nEndRow >0 && ARow > nEndRow) ) break;
			LineY +=  GetTableData()->GetAxis()->GetRowHeight(ARow) + 1;
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
			LineX +=  GetTableData()->GetAxis()->GetColWidth(ACol) + 1; 
			ACol++;
		}

		int nSavedDC = SaveDC(hdc);
		::BeginPath(hdc);
		::PolyPolyline(hdc,(LPPOINT)Points, StrokeList, (cx + cy));
		::EndPath(hdc);
		//::WidenPath(hdc);


		TRIVERTEX	pVertex[2];
		GRADIENT_RECT pGRect[1];

		RECT FRect = rect;

		pVertex[0].x = FRect.left;
		pVertex[0].y = FRect.top;
		pVertex[0].Red  = (GetRValue(color)>25? GetRValue(color) + 25:0)*256;
		pVertex[0].Green = (GetGValue(color)>25? GetGValue(color) + 25:0)*256;
		pVertex[0].Blue = (GetBValue(color)>25? GetBValue(color) + 25:0)*256;

		pVertex[1].x = FRect.right;
		pVertex[1].y = FRect.bottom ;
		pVertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) - 25)*256;
		pVertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) - 25)*256;
		pVertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color) - 25)*256;
		pGRect[0].UpperLeft = 0;
		pGRect[0].LowerRight = 1;

		/*
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

		*/
		StrokePath(hdc);
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
	void CHtTableView::DrawBorderLines(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth, COLORREF color)
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
		THtDrawInfo AxisInfo;
		CalcDrawInfo(AxisInfo);

		int OffsetX,OffsetY;

		RECT gRect;
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
			ACol = StartCol;
			wRect.left = StartX;
			wRect.bottom = wRect.top + GetTableData()->GetAxis()->GetRowHeight(ARow);
			if(wRect.bottom > StopY)break;
			while (wRect.left < StopX && ACol <= GetColCount())
			{
				RECT gRect;
				bDrawRect = false;
				AColEx = ACol;
				if(GetTableData()->GetSpan()->isSpaned(ACol,ARow,gRect))
				{
					BoxRect(AxisInfo,gRect,sRect);
					if((ACol==gRect.left && ARow==gRect.top)|| (ACol ==StartCol && gRect.left < StartCol)||(ARow==StartRow && gRect.top > StartRow))
						bDrawRect = true;

					::OffsetRect(&sRect,- OffsetX,- OffsetY);
					
					ACol = gRect.right;
					wRect.right = sRect.right;
				}else
				{
					wRect.right = wRect.left + GetTableData()->GetAxis()->GetColWidth(ACol);
					{
						sRect = wRect;
						bDrawRect = true;
					}
				}
				if(bDrawRect)
				{
					pBoxStyle = GetTableData()->GetStyle()->GetCellCssStyle(AColEx,ARow);
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

		unsigned int FlatPenStyle = PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_FLAT|PS_JOIN_MITER;
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

	int GetRowColAtPoint(int cx, int cy, int &Row, int &Col)
	{
		return 0;
	}
}}}
