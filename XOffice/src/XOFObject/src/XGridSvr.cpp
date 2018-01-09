#include "StdAfx.h"
#include "..\include\XGridSvr.hpp"
#include "idwview.hpp"
#include "IDwViewObServer.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	RECT    LRect(int left,int top,int right,int bottom)
	{
		RECT    Rect;
		Rect.left = left;
		Rect.top = top;
		Rect.right = right;
		Rect.bottom  = bottom;

		return Rect;
	}


	TPoint  Point(int x,int y) 
	{
		TPoint  P;
		P.x = x;
		P.y = y;
		return P;
	}

	TGridRect GridRect(TGridCoord Coord1, TGridCoord Coord2){ 
		TGridRect   Result;
		Result.left = Coord2.x;
		if(Coord1.x < Coord2.x) Result.left = Coord1.x;
		Result.right = Coord1.x;
		if(Coord1.x < Coord2.x) Result.right = Coord2.x;
		Result.top = Coord2.y;
		if(Coord1.y < Coord2.y) Result.top = Coord1.y;
		Result.bottom = Coord1.y;
		if(Coord1.y < Coord2.y)Result.bottom = Coord2.y;
		return Result;
	}

	BOOL Includes(TPoint P1,TPoint *P2,RECT R1,RECT R2)
	{
		BOOL Result;
		Result = PtInRect(P1.x, P1.y, R1) || PtInRect(P1.x, P1.y, R2);
		if(Result){P2->x = P1.x;P2->y = P1.y;}
		return Result;
	}

	BOOL Build(TGridRect *R,TPoint P1, TPoint P2, TPoint P3,RECT R1,RECT R2)
	{
		BOOL Result = TRUE;
		if(Includes(P1, &(R->TopLeft),R1,R2))
		{
			if(!Includes(P3, &(R->BottomRight),R1,R2)) R->BottomRight = P2;
		}
		else if(Includes(P2, &(R->TopLeft),R1,R2)) R->BottomRight = P3;
		else Result = FALSE;
		return Result;
	};

	void XorRects(RECT R1, RECT R2,  RECT XorRects[4])
	{
		RECT Intersect, Union;

		FillMemory(XorRects, sizeof(RECT) * 4, 0);
		if(!IntersectRect(&Intersect, &R1, &R2))
		{
			// Don't intersect so its simple 
			XorRects[0] = R1;
			XorRects[1] = R2;
		}
		else
		{
			UnionRect(&Union, &R1, &R2);
			if(Build((TGridRect *)&XorRects[0],
				Point(Union.left, Union.top),
				Point(Union.left, Intersect.top),
				Point(Union.left, Intersect.bottom),R1,R2))
				XorRects[0].right = Intersect.left;
			if(Build((TGridRect *)&XorRects[1],
				Point(Intersect.left, Union.top),
				Point(Intersect.right, Union.top),
				Point(Union.right, Union.top),R1,R2))
				XorRects[1].bottom = Intersect.top;
			if(Build((TGridRect *)&XorRects[2],
				Point(Union.right, Intersect.top),
				Point(Union.right, Intersect.bottom),
				Point(Union.right, Union.bottom),R1,R2)) 
				XorRects[2].left = Intersect.right;
			if(Build((TGridRect *)&XorRects[3],
				Point(Union.left, Union.bottom),
				Point(Intersect.left, Union.bottom),
				Point(Intersect.right, Union.bottom),R1,R2))
				XorRects[3].top = Intersect.bottom;
		};
	};
	
	XGridSvr::XGridSvr(void):FRowCount(20),FColCount(25)
	{
		::SetRect(&FRect,0,0,0,0);
		Initialize();
		SetEventListons();
	}

	XGridSvr::~XGridSvr(void)
	{
		UnSetEventListons();
	}

	void XGridSvr::Initialize()
	{
		FGridLineWidth = 1;
		FOptions = goFixedVertLine|goFixedHorzLine|goVertLine|goHorzLine|goRangeSelect|goColSizing|goRowSizing|goDrawFixed|goDrawBound;
		FOptions = FOptions|goRowMoving|goColMoving;
		FScrollBars = ssBoth;
		FBorderStyle = bsSingle;
		FGridState = gsNormal;

		FTopLeft.x = 1;
		FTopLeft.y = 1;
		FCurrent = FTopLeft;
		FAnchor = FCurrent;
		FFixedCols = 1;
		FFixedRows = 1;
		if(goRowSelect & FOptions)FAnchor.x = GetColCount() ;
	}

	bool XGridSvr::SetEventListons()
	{
		//this->AttachListonTo(pDest,WM_PAINT,(FNEvent)&XGridSvr::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XGridSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XGridSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XGridSvr::EvtMouseMove);

		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XGridSvr::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&XGridSvr::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&XGridSvr::EvtHScroll);

		this->AttachListon(WM_SIZE,(FNEvent)&XGridSvr::EvtSize);
		this->AttachListon(WM_CHAR,(FNEvent)&XGridSvr::EvtChar);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XGridSvr::EvtKeyDown);
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XGridSvr::EvtSetFocus);
		this->AttachListon(WM_KILLFOCUS,(FNEvent)&XGridSvr::EvtKillFocus);
		this->AttachListon(WM_XCOMMAND,(FNEvent)&XGridSvr::EvtXCommand);
		this->AttachListon(WM_TIMER,(FNEvent)&XGridSvr::EvtTimer);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XGridSvr::EvtSetCursor);
		return true;
	}

	bool XGridSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	XAPI void XGridSvr::SetGridDataSvr(XGridDataSvr * pGridDataSvr)
	{
		m_pGridDataSvr = pGridDataSvr;
	}
	XAPI XGridDataSvr * XGridSvr::GetGridDataSvr()
	{
		return m_pGridDataSvr;
	}

	void XGridSvr::CalcDrawInfo(TGridDrawInfo1 &DrawInfo)
	{
		CalcDrawInfoXY(DrawInfo, FRect.right - FRect.left, FRect.bottom - FRect.top);
	}
	void XGridSvr::CalcDrawInfoXY(TGridDrawInfo1 &DrawInfo,int  UseWidth,int  UseHeight)
	{
		DrawInfo.Horz.GetExtent = &XGridSvr::GetColWidth;
		DrawInfo.Vert.GetExtent = &XGridSvr::GetRowHeight;
		CalcFixedInfo(DrawInfo);
		CalcAxis(DrawInfo.Horz, UseWidth);
		CalcAxis(DrawInfo.Vert, UseHeight);
	}

	void XGridSvr::CalcAxis(TGridAxisDrawInfo1 &AxisInfo, int UseExtent)
	{
		AxisInfo.GridExtent = UseExtent;
		AxisInfo.GridBoundary = AxisInfo.FixedBoundary;
		AxisInfo.FullVisBoundary = AxisInfo.FixedBoundary;
		AxisInfo.LastFullVisibleCell = AxisInfo.FirstGridCell;
		for(int i=AxisInfo.FirstGridCell;i<=AxisInfo.GridCellCount - 1;i++){
			AxisInfo.GridBoundary += (this->*AxisInfo.GetExtent)(i) + AxisInfo.EffectiveLineWidth;
			if(AxisInfo.GridBoundary > AxisInfo.GridExtent + AxisInfo.EffectiveLineWidth)
			{
				AxisInfo.GridBoundary = AxisInfo.GridExtent;
				break;
			}
			AxisInfo.LastFullVisibleCell = i;
			AxisInfo.FullVisBoundary = AxisInfo.GridBoundary;
		}
	}


	void XGridSvr::CalcFixedInfo(TGridDrawInfo1 &DrawInfo)
	{
		CalcFixedAxis(DrawInfo.Horz, goFixedVertLine|goVertLine, GetFixedCols(),
			GetLeftCol(), GetColCount());
		CalcFixedAxis(DrawInfo.Vert, goFixedHorzLine|goHorzLine, GetFixedRows(),
			GetTopRow(), GetRowCount());
	}

	void XGridSvr::CalcFixedAxis(TGridAxisDrawInfo1 &Axis ,TGridOptions LineOptions,
		int FixedCount,int  FirstCell,int  CellCount)
	{
		if(!(LineOptions & FOptions))
			Axis.EffectiveLineWidth = 0;
		else
			Axis.EffectiveLineWidth = GetGridLineWidth();

		Axis.FixedBoundary = 0;
		for(int i= 0 ;i<=FixedCount - 1;i++)
			Axis.FixedBoundary +=  (this->*Axis.GetExtent)(i) + Axis.EffectiveLineWidth;

		Axis.FixedCellCount = FixedCount;
		Axis.FirstGridCell =  FirstCell;
		Axis.GridCellCount =  CellCount;
	}



	XAPI void XGridSvr::SetCurrent(TGridCoord Value)
	{
		MoveCurrent(Value);
	}
	XAPI void XGridSvr::SetAnchor(TGridCoord Value)
	{
		MoveAnchor(Value);	
	}
	XAPI void XGridSvr::SetTopLeft(int ALeft, int ATop)
	{
		MoveTopLeft(ALeft,ATop);
	}
	XAPI void XGridSvr::ClampInView(TGridCoord Coord)
	{
		TGridDrawInfo1 DrawInfo ;
		TGridCoord MaxTopLeft ;
		TGridCoord OldTopLeft ;

		this->CalcDrawInfo(DrawInfo);

		if((Coord.x > DrawInfo.Horz.LastFullVisibleCell) ||
			(Coord.y > DrawInfo.Vert.LastFullVisibleCell) || (Coord.x < GetLeftCol()) || (Coord.y < GetTopRow()))
		{
			OldTopLeft = FTopLeft;
			MaxTopLeft = CalcMaxTopLeft(Coord, DrawInfo);
			Update();
			if(Coord.x < GetLeftCol()) FTopLeft.x = Coord.x;
			else if(Coord.x > DrawInfo.Horz.LastFullVisibleCell) FTopLeft.x = MaxTopLeft.x;
			if(Coord.y < GetTopRow()) FTopLeft.y = Coord.y;
			else if(Coord.y > DrawInfo.Vert.LastFullVisibleCell) FTopLeft.y = MaxTopLeft.y;
			TopLeftMoved(OldTopLeft);
		}
	}

	XAPI TGridRect XGridSvr::GetSelection()
	{
		TGridRect SRect(FCurrent.x,FCurrent.y,FAnchor.x,FAnchor.y);
		return  SRect;

	}

	int XGridSvr::MoveCurrent(TGridCoord Coord)
	{
		MoveCurrent(Coord.x,Coord.y,true,true);
		return 1;
	}

	BOOL XGridSvr::SelectCell(int ACol,int ARow)
	{
		return TRUE;
	}
	void XGridSvr::MoveCurrent(int ACol,int ARow, BOOL MoveAnchor,BOOL Show)
	{
		TGridRect OldSel ;
		TGridCoord OldCurrent;
		
		if( (ACol < 0) || (ARow < 0) || (ACol >= GetColCount()) || (ARow >= GetRowCount())) return;
		if( SelectCell(ACol, ARow))
		{
			OldSel = GetSelection();
			OldCurrent = FCurrent;
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
	}
	void XGridSvr::MoveAnchor(TGridCoord NewAnchor)
	{
		TGridRect OldSel;
		if(goRangeSelect & FOptions)
		{
			OldSel = GetSelection();
			FAnchor = NewAnchor;
			if(goRowSelect & FOptions) FAnchor.x = GetColCount() - 1;
			ClampInView(NewAnchor);
			SelectionMoved(OldSel);
		}
		else MoveCurrent(NewAnchor.x, NewAnchor.y, TRUE, TRUE);
	}

	void XGridSvr::AreaMoved(TGridRect OldSel,TGridRect NewSel)
	{
		TRect OldRect, NewRect;
		TGridRect SelRect;

		SelRect = NewSel;

		TGridDrawInfo1 DrawInfo;
		CalcDrawInfo(DrawInfo);
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

	void XGridSvr::SelectionMoved(TGridRect OldSel)
	{
		TGridRect SelRect = GetSelection();
		this->AreaMoved(OldSel,SelRect);
	}


	void XGridSvr::MoveTopLeft(int ALeft, int ATop)
	{
		TGridCoord OldTopLeft;

		if( (ALeft == FTopLeft.x) && (ATop == FTopLeft.y)) return;
		Update();
		OldTopLeft = FTopLeft;
		FTopLeft.x = ALeft;
		FTopLeft.y = ATop;
		TopLeftMoved(OldTopLeft);
	}

	void XGridSvr::TopLeftMoved(TGridCoord OldTopLeft)
	{
		TGridDrawInfo1 DrawInfo;
		TGridCoord Delta;
		
		UpdateScrollPos();
		CalcDrawInfo(DrawInfo);
		if(CalcScroll(DrawInfo.Horz, OldTopLeft.x, FTopLeft.x, Delta.x) &&
			CalcScroll(DrawInfo.Vert, OldTopLeft.y, FTopLeft.y, Delta.y))
			ScrollDataInfo(Delta.x, Delta.y, DrawInfo);
		TopLeftChanged();
			TopLeftChanged();
	}

	void XGridSvr::TopLeftChanged()
	{

	}

	XAPI void XGridSvr::SetFRect(RECT &rect)
	{
		FRect = rect;
	}

	int XGridSvr::DoCalc(const TGridAxisDrawInfo1 &AxisInfo, int N)
	{
		int  Start, Stop;
		int   Line;

		if(N < AxisInfo.FixedBoundary)
		{
			Start = 0;
			Stop =  AxisInfo.FixedCellCount - 1;
			Line = 0;
		}
		else
		{
			Start = AxisInfo.FirstGridCell;
			Stop = AxisInfo.GridCellCount - 1;
			Line = AxisInfo.FixedBoundary;
		}

		int Result;
		Result = -1;
		int i;
		for(i = Start;i<=Stop;i++)
		{
			Line += (this->*AxisInfo.GetExtent)(i) +  AxisInfo.EffectiveLineWidth;
			if(N < Line){
				Result = i;
				break;
			}
		}
		if(i>Stop) return Stop;

		return Result;
	}

	XAPI TGridCoord XGridSvr::CalcCoordFromPoint(POINT pt)
	{
		TGridDrawInfo1 DrawInfo;
		CalcDrawInfo(DrawInfo);
		return CalcCoordFromPoint(pt,DrawInfo);
	}

	XAPI TGridCoord XGridSvr::CalcCoordFromPoint(POINT pt,const TGridDrawInfo1 & DrawInfo)
	{
		TGridCoord Result;
		Result.x = DoCalc(DrawInfo.Horz, pt.x);
		Result.y = DoCalc(DrawInfo.Vert, pt.y);
		
		return Result;
	}

	XAPI int XGridSvr::CalcRowFromPoint(POINT pt)
	{
		TGridDrawInfo1 DrawInfo;
		CalcDrawInfo(DrawInfo);
		return DoCalc(DrawInfo.Vert, pt.y);
	}
	XAPI int XGridSvr::CalcColFromPoint(POINT pt)
	{
		TGridDrawInfo1 DrawInfo;
		CalcDrawInfo(DrawInfo);
		return DoCalc(DrawInfo.Horz, pt.x);
	}

	XAPI void XGridSvr::SetGridState(TGridState Value)
	{
		FGridState = Value;
	}
	XAPI void XGridSvr::SetGridOptions(TGridOptions Value)
	{
		FOptions = Value;
	}

	int XGridSvr::GetMinHeight(int ARow)
	{
		return 0;
	}
	int XGridSvr::GetMinWidth(int ACol)
	{
		return 0;
	}
	void XGridSvr::SetRowHeight(int Index,int value)
	{
		GetGridDataSvr()->SetRowHeight(Index,value);
	}

	void XGridSvr::SetColWidth(int Index,int value)
	{
		GetGridDataSvr()->SetColWidth(Index,value);
	}

	int XGridSvr::GetRowHeight(int Index)
	{
		return GetGridDataSvr()->GetRowHeight(Index);
	}

	int XGridSvr::GetColWidth(int Index)
	{
		return GetGridDataSvr()->GetColWidth(Index);
	}
	
	XAPI int XGridSvr::GetHeaderHeight()
	{
		return GetGridDataSvr()->GetRowHeight(0);
	}
	XAPI int XGridSvr::GetHeaderWidth()
	{
		return GetGridDataSvr()->GetColWidth(0);
	}

	void XGridSvr::OnDraw(HDC hdc,RECT &rect)
	{
		RECT drect = GetFRect();
		DrawBackGround(hdc,drect);
		DrawGridLine(hdc,drect,FTopLeft.x,FTopLeft.y);
		DrawBorderLines(hdc,drect,FTopLeft.x,FTopLeft.y);
		DrawCells(hdc,drect,FTopLeft.x,FTopLeft.y);
		DrawSelectionBorder(hdc,rect);
	}
	void XGridSvr::DrawBackGround(HDC,RECT &rect)
	{
		return ;
	}
	void XGridSvr::DrawCells(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth)
	{
		RECT  wRect;
		RECT FocRect;
		int ARow,ACol;
		int StartX,StartY;
		int StopX,StopY;

		StartX = rect.left + this->FGridLineWidth;
		StartY = rect.top + this->FGridLineWidth;
		StopX = rect.right;
		StopY = rect.bottom;

		::SetRect(&FocRect,0,0,0,0);
		TGridDrawInfo1 AxisInfo;
		CalcDrawInfo(AxisInfo);

		ARow=StartRow;
		wRect.top = StartY;
		wRect.left = StartX;
		while (wRect.top < StopY && ARow <= GetRowCount())
		{
			ACol = StartCol;
			wRect.left = StartX;
			wRect.bottom = wRect.top + GetGridDataSvr()->GetRowHeight(ARow);
			if(wRect.bottom > StopY)break;
			while (wRect.left < StopX && ACol <= GetColCount())
			{
				TGridRect gRect;
				wRect.right = wRect.left + GetGridDataSvr()->GetColWidth(ACol);
				DrawCell(hdc,ACol, ARow, wRect);

				if(wRect.right > StopX)break;
				wRect.left = wRect.right + 1;
				ACol++;
			}
			wRect.top = wRect.bottom + 1;
			ARow++;
		}
	}
	void XGridSvr::DrawCell(HDC hdc,int ACol,int ARow, RECT &ARect)
	{
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
	}
	void XGridSvr::DrawGridLine(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth, COLORREF color)
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

		HPEN	hPen = ::CreatePen(PS_SOLID,this->FGridLineWidth,XGridOption::stdRowSelecterColor0);
		hPen = (HPEN)::SelectObject(hdc,hPen);
		//get vert line count;
		ARow= StartRow ;
		while(LineY <= rect.bottom)
		{
			cy++;
			if(ARow > this->GetRowCount()) break;
			LineY +=  GetGridDataSvr()->GetRowHeight(ARow) + GridLineWidth;
			ARow++;
		}
		EndY = LineY > rect.bottom ?rect.bottom : LineY;

		//get horz line count
		ACol=StartCol;
		while(LineX <= rect.right)
		{
			cx++;
			if(ACol > this->GetColCount()) break;
			LineX +=  GetGridDataSvr()->GetColWidth(ACol) + GridLineWidth;
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
			if(ARow > this->GetRowCount()) break;
			LineY +=  GetGridDataSvr()->GetRowHeight(ARow) + 1;
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
			LineX +=  GetGridDataSvr()->GetColWidth(ACol) + 1; 
			ACol++;
		}

		int nSavedDC = SaveDC(hdc);
		::BeginPath(hdc);
		::PolyPolyline(hdc,(LPPOINT)Points, StrokeList, (cx + cy));
		::EndPath(hdc);


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
		pVertex[1].Blue = (GetGValue(color)>230?255:GetGValue(color) - 25)*256;
		pGRect[0].UpperLeft = 0;
		pGRect[0].LowerRight = 1;

		HRGN hRgn = ::CreateRectRgn(0,0,0,0);
		TGridDrawInfo1 AxisInfo;
		CalcDrawInfo(AxisInfo);
		
		//::SelectClipPath(hdc,RGN_COPY);
		//GradientFill(hdc, pVertex,2,pGRect , 1,GRADIENT_FILL_RECT_H);
		StrokePath(hdc);

		::DeleteObject(hRgn);
		RestoreDC(hdc,nSavedDC);


		hPen = (HPEN)::SelectObject(hdc,hPen);
		::DeleteObject(hPen);

		delete Points;
		delete StrokeList;
	}

	const unsigned int FlatPenStyle = PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_FLAT|PS_JOIN_MITER;
	void XGridSvr::DrawBorderLines(HDC hdc,RECT &rect,int StartCol,int StartRow,int GridLineWidth, COLORREF color)
	{
		/*
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
		TGridDrawInfo1 AxisInfo;
		CalcDrawInfo(&AxisInfo);

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
			wRect.bottom = wRect.top + GetGridDataSvr()->GetRowHeight(ARow);
			if(wRect.bottom > StopY)break;
			while (wRect.left < StopX && ACol <= GetColCount())
			{
				TGridRect gRect;
				bDrawRect = false;
				AColEx = ACol;

				wRect.right = wRect.left + GetGridDataSvr()->GetColWidth(ACol);
				{
					sRect = wRect;
					bDrawRect = true;
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
		}*/
	}

	void XGridSvr::DrawSelectionBorder(HDC hdc,RECT &rect)
	{
		int width,height;
		RECT FocRect;
		TGridDrawInfo1 AxisInfo;
		CalcDrawInfo(AxisInfo);
		BoxRect(AxisInfo,GetSelection(),FocRect);
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

	XAPI void XGridSvr::SetScrollStyle(TScrollStyle ScrollBars)
	{
		FScrollBars = ScrollBars;
	}
	XAPI  void XGridSvr::SetBorderStyle(TBorderStyle BorderStyle)
	{
		FBorderStyle = BorderStyle;
	}

	void XGridSvr::CalcPageExtents(TGridDrawInfo1 &DrawInfo, int &PageWidth, int &PageHeight)
	{
		CalcDrawInfo(DrawInfo);
		PageWidth = DrawInfo.Horz.LastFullVisibleCell - this->GetLeftCol();
		if(PageWidth < 1) PageWidth = 1;
		PageHeight = DrawInfo.Vert.LastFullVisibleCell - this->GetTopRow();
		if(PageHeight < 1)PageHeight = 1;
	}

	void XGridSvr::TimedScroll(TGridScrollDirection Direction)
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

	bool XGridSvr::CanEditAcceptKey(char Key)
	{
		return true;
	}
	bool XGridSvr::CanGridAcceptKey(WORD Key, TShiftState Shift)
	{
		return true;
	}

	void Restrict(TGridCoord &Coord, long MinX, long MinY, long MaxX, long MaxY)
	{
		if( Coord.x > MaxX) Coord.x  = MaxX;
		else if( Coord.x  < MinX) Coord.x  = MinX;
		if( Coord.y > MaxY) Coord.y = MaxY;
		else if( Coord.y < MinY) Coord.y = MinY;
	}

	bool XGridSvr::EvtLButtonDown(TEvent *pEvent ,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		UINT nFlags = pEvent->mouse.nFlags;
		if(::GetFocus() != GetHWND())::SetFocus(GetHWND());
		TGridCoord CellHit;
		CellHit = CalcCoordFromPoint(point);

		if((CellHit.x == FCurrent.x) && (CellHit.y == FCurrent.y)) return true;
		MoveCurrent(CellHit.x, CellHit.y, True, True);
		Click();
		return true;
	}
	bool XGridSvr::EvtLButtonUp(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtMouseMove(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtMouseWheel(TEvent *pEvent ,LPARAM lParam)
	{
		if(pEvent->wheel.zDelta > 0)
			ModifyScrollBar(SB_VERT, SB_LINEUP, 0);
		else
			ModifyScrollBar(SB_VERT, SB_LINEDOWN, 0);
		return true;
	}
	bool XGridSvr::EvtVScroll(TEvent *pEvent ,LPARAM lParam)
	{
		ModifyScrollBar(SB_VERT, pEvent->scroll.nSBCode, pEvent->scroll.nPos);
		return true;
	}
	bool XGridSvr::EvtHScroll(TEvent *pEvent ,LPARAM lParam)
	{
		ModifyScrollBar(SB_HORZ, pEvent->scroll.nSBCode, pEvent->scroll.nPos);
		return true;
	}
	bool XGridSvr::EvtSize(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtChar(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		TGridCoord NewTopLeft, NewCurrent, MaxTopLeft;
		TGridDrawInfo1 DrawInfo;
		int PageWidth, PageHeight;
		UINT Key;

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
			case VK_NEXT: NewCurrent.y = DrawInfo.Vert.LastFullVisibleCell;break;
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

					break;
			//case VK_F2: FEditorMode = TRUE;break;
			}
		}

		MaxTopLeft.x = GetColCount() - 1;
		MaxTopLeft.y = GetRowCount() - 1;
		MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
		Restrict(NewTopLeft, GetFixedCols(), GetFixedRows(), MaxTopLeft.x, MaxTopLeft.y);
		if( (NewTopLeft.x != GetLeftCol()) || (NewTopLeft.y != GetTopRow()))
			MoveTopLeft(NewTopLeft.x, NewTopLeft.y);
		Restrict(NewCurrent, GetFixedCols(), GetFixedRows(), GetColCount() - 1,  GetRowCount() - 1);
		if( (NewCurrent.x != GetCol()) || (NewCurrent.y != GetRow()))
			FocusCell(NewCurrent.x, NewCurrent.y, !(ssShift & Shift));

		return true;
	}
	bool XGridSvr::EvtSetFocus(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtKillFocus(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtXCommand(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtTimer(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XGridSvr::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	int XGridSvr::CalcScrollBar(int Value, Cardinal ScrollCode, Cardinal ScrollBar, TGridDrawInfo1& DrawInfo, int Pos)
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
			if(/*(goThumbTracking && FOptions) || */(ScrollCode == SB_THUMBPOSITION))
			{
				TGridCoord MaxTopLeft;
				MaxTopLeft.x = GetColCount() ;
				MaxTopLeft.y = GetRowCount() ;
				MaxTopLeft = CalcMaxTopLeft(MaxTopLeft, DrawInfo);
				Result = ScrollBarMin(ScrollBar) + 
					MulDiv(Pos, ScrollBarMax(ScrollBar,MaxTopLeft)
					- ScrollBarMin(ScrollBar), MaxShortInt);
				break;
			}
		case SB_BOTTOM:
			Result = ScrollBarMin(ScrollBar);
			break;
		case SB_TOP:
			Result = ScrollBarMin(ScrollBar);
			break;
		}
		return Result;
	}

	void XGridSvr::ModifyScrollBar(Cardinal ScrollBar, Cardinal ScrollCode, Cardinal Pos)
	{
		TGridCoord NewTopLeft, MaxTopLeft;
		TGridDrawInfo1 DrawInfo;

		//	if(/*Visible && CanFocus && */TabStop)
		//		m_pView->SendMessage(WM_SETFOCUS,0,0);
		CalcDrawInfo(DrawInfo);

		if((ScrollBar == SB_HORZ) && (GetColCount() == 1))
		{
			//ModifyPixelScrollBar(ScrollCode, Pos,DrawInfo);
			return;
		}

		MaxTopLeft.x = GetColCount() ;
		MaxTopLeft.y = GetRowCount() ;
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
	int XGridSvr::ScrollBarMin(Cardinal ScrollBar)
	{
		return 1;
	}
	int XGridSvr::ScrollBarMax(Cardinal ScrollBar,TGridCoord MaxTopLeft)
	{
		return ScrollBar == SB_HORZ? MaxTopLeft.x:MaxTopLeft.y;
	}
	int XGridSvr::PageUp(Cardinal ScrollBar,TGridDrawInfo1& DrawInfo)
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
	int  XGridSvr::PageDown(Cardinal ScrollBar)
	{
		TGridDrawInfo1 DrawInfo;
		CalcDrawInfo(DrawInfo);

		int Result;
		if(ScrollBar == SB_HORZ)
			Result =  DrawInfo.Horz.LastFullVisibleCell - FTopLeft.x;
		else
			Result =  DrawInfo.Vert.LastFullVisibleCell - FTopLeft.y;

		if(Result < 1)Result = 1;

		return Result;
	}

	TGridCoord  XGridSvr::CalcMaxTopLeft(TGridCoord Coord, TGridDrawInfo1& DrawInfo)
	{
		TGridCoord Result;
		Result.x = CalcMaxCell(DrawInfo.Horz, Coord.x);
		Result.y = CalcMaxCell(DrawInfo.Vert, Coord.y);

		return Result;
	}

	int XGridSvr::CalcMaxCell(TGridAxisDrawInfo1 &Axis, int Start)
	{
		int Line;
		int Result;
	    
		Result = Start;
		Line = Axis.GridExtent + Axis.EffectiveLineWidth;
		for(int i= Start;i>=Axis.FixedCellCount;i--)
		{
			Line -= (this->*Axis.GetExtent)(i);
			Line -= Axis.EffectiveLineWidth;
			if(Line < Axis.FixedBoundary)
				break;
			Result = i;
		}
		return Result;
	}

	void XGridSvr::UpdateScrollPos()
	{
		TGridDrawInfo1 DrawInfo;
		TGridCoord MaxTopLeft;

		//int GridSpace, ColWidth;

		if(FScrollBars == ssNone) return;
		CalcDrawInfo(DrawInfo);
		//MaxTopLeft.x = GetGridDataSvr()->GetValidCol() ;
		//MaxTopLeft.y = GetGridDataSvr()->GetValidRow() ;//GetRowCount() ;
		MaxTopLeft.x = GetColCount();// - 1;
		MaxTopLeft.y = GetRowCount();// - 1;

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
				GetHScrollBar()->SetPosition(MulDiv(FTopLeft.x - 1, 100 /*MaxShortInt*/, MaxTopLeft.x - 1));
			}
		}
		if(FScrollBars & ssVertical || FScrollBars & ssBoth)
			GetVScrollBar()->SetPosition(MulDiv(FTopLeft.y - 1, 100 /*MaxShortInt*/, MaxTopLeft.y - 1));
	}

	void XGridSvr::ScrollData(int DX, int DY)
	{
		TGridDrawInfo1 DrawInfo;

		CalcDrawInfo(DrawInfo);
		ScrollDataInfo(DX, DY, DrawInfo);
	}
	void XGridSvr::ScrollDataInfo(int DX, int DY, TGridDrawInfo1 &DrawInfo)
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
			SetRect(&ScrollArea, GetGridSheetSvr()->GetHeaderSelector()->GetFRect().left, GetGridSheetSvr()->GetHeaderSelector()->GetFRect().top, FRect.right, FRect.bottom);
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);
		}
		else if( DX == 0)
		{
			//{ Scroll both the row titles && data area at the same time }
			SetRect(&ScrollArea, GetGridSheetSvr()->GetRowSelector()->GetFRect().left, GetGridSheetSvr()->GetRowSelector()->GetFRect().top, FRect.right, FRect.bottom);
			::ScrollWindowEx(GetHWND(),0, DY, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);
		}
		else
		{
			//{ Scroll titles && data area separately }
			//{ Column titles }
			ScrollArea = GetGridSheetSvr()->GetHeaderSelector()->GetFRect();
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);

			//{ Row titles }
			ScrollArea =  GetGridSheetSvr()->GetRowSelector()->GetFRect();
			::ScrollWindowEx(GetHWND(),DX, 0, &ScrollArea, &ScrollArea, 0, NULL, ScrollFlags);

			//{data Area}
			ScrollArea = GetFRect();
			::ScrollWindowEx(GetHWND(),DX, DY, &ScrollArea, &ScrollArea, 0,NULL, ScrollFlags);
		}
		UpdateWindow(GetHWND());		
	}

	BOOL XGridSvr::CalcScroll(TGridAxisDrawInfo1 &AxisInfo, int OldPos, int CurrentPos, int &Amount)
	{
		int Start, Stop;
		BOOL Result;

		Result = FALSE;

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
			Amount += (this->*AxisInfo.GetExtent)(i) + AxisInfo.EffectiveLineWidth;
			if(Amount > (AxisInfo.GridBoundary - AxisInfo.FixedBoundary))
			{
				//{ Scroll amount too big, redraw the whole thing }
				InvalidateGrid();
				return Result;
			}
		}
		if(OldPos < CurrentPos)Amount = -Amount;

		return TRUE;
	}
	
	CXScrollBar * XGridSvr::GetHScrollBar()
	{
		return dynamic_cast<CXScrollBar *>(GetGridSheetSvr()->GetHScrollbar());
	}
	CXScrollBar * XGridSvr::GetVScrollBar()
	{
		return dynamic_cast<CXScrollBar *>(GetGridSheetSvr()->GetVScrollbar());
	}

	int XGridSvr::LinePos(TGridAxisDrawInfo1 & AxisInfo ,int  Line)
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
			Result += (this->*AxisInfo.GetExtent)(i) + AxisInfo.EffectiveLineWidth;
			if(Result > AxisInfo.GridExtent)
			{
				Result = 0;
				return Result;
			}
		}
		return Result;
	}
	bool XGridSvr::GetCellRect(int ACol,int ARow,RECT &rect,bool bIncludeSpan )
	{
		TGridDrawInfo1 AxisInfo;
		CalcDrawInfo(AxisInfo);
		return GetCellRect(AxisInfo,ACol,ARow,rect,bIncludeSpan);

	}
	bool XGridSvr::GetCellRect(TGridDrawInfo1& AxisInfo,int ACol,int ARow,RECT &rect,bool bIncludeSpan)
	{
		::SetRect(&rect,0,0,0,0);

		rect.left = LinePos(AxisInfo.Horz,ACol);
		rect.top =  LinePos(AxisInfo.Vert,ARow);

		rect.right  = rect.left + GetGridDataSvr()->GetColWidth(ACol);
		rect.bottom = rect.top  + GetGridDataSvr()->GetRowHeight(ARow);

		return true;
	}
	bool XGridSvr::BoxRect(TGridDrawInfo1& AxisInfo,TGridRect gridRect,RECT &screenRect)
	{
		RECT	sRect,	eRect;

		::SetRect(&screenRect,0,0,0,0);
		NormalizeRect(*(RECT *)&gridRect);
		GetCellRect(AxisInfo,gridRect.left,gridRect.top,sRect,false);
		GetCellRect(AxisInfo,gridRect.right,gridRect.bottom,eRect,false);

		::SetRect(&screenRect,sRect.left,sRect.top,eRect.right,eRect.bottom);
		return true;
	}

	void XGridSvr::Update()
	{

	}
	int XGridSvr::GetVisibleRowCount()
	{
		TGridDrawInfo1  DrawInfo;
		CalcDrawInfo(DrawInfo);
		return DrawInfo.Vert.LastFullVisibleCell - this->FTopLeft.y + 1;
	}
	int XGridSvr::GetVisibleColCount()
	{
		TGridDrawInfo1  DrawInfo;
		CalcDrawInfo(DrawInfo);
		return DrawInfo.Horz.LastFullVisibleCell - this->FTopLeft.x + 1;
	}
	void XGridSvr::InvalidateGrid()
	{
		::InvalidateRect(GetHWND(),&GetGridSheetSvr()->GetFRect(),false);
	}
	void XGridSvr::InvalidateRect( TGridRect ARect)
	{
		RECT rect,rectX,rectY;

		TGridDrawInfo1  DrawInfo;
		CalcDrawInfo(DrawInfo);
		BoxRect(DrawInfo,ARect, rect);

		rect.left -= FGridLineWidth *2;
		rect.right += FGridLineWidth *2;
		rect.top -= FGridLineWidth *2;
		rect.bottom += FGridLineWidth *2;

		::InvalidateRect(GetHWND(),&rect, FALSE);

		rectX = rect;
		rectX.top = FGridLineWidth;
		rectX.bottom = GetGridDataSvr()->GetRowHeight(0) + FGridLineWidth;
		::InvalidateRect(GetHWND(),&rectX, FALSE);

		rectY = rect;
		rectY.left = FGridLineWidth;
		rectY.right = GetGridDataSvr()->GetColWidth(0) + FGridLineWidth;
		::InvalidateRect(GetHWND(),&rectY, FALSE);
	}

	void XGridSvr::InvalidateRow(int ARow)
	{
		TGridRect Rect;

		Rect.top = ARow;
		Rect.left = 0;
		Rect.bottom = ARow;
		Rect.right = GetVisibleColCount()+1;
		InvalidateRect(Rect);

	}
	void XGridSvr::InvalidateCol(int ACol)
	{
		TGridRect Rect;

		Rect.top = 0;
		Rect.left = ACol;
		Rect.bottom = GetVisibleRowCount()+1;
		Rect.right = ACol;
		InvalidateRect(Rect);
	}
	void XGridSvr::InvalidateCell(int ACol, int ARow)
	{
		TGridRect Rect;

		Rect.top = ARow;
		Rect.left = ACol;
		Rect.bottom = ARow;
		Rect.right = ACol;

		InvalidateRect(Rect);
	}

	void XGridSvr::ShowGrid()
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
	void XGridSvr::ShowGrid(bool bShow)
	{
		if(bShow){
			FOptions = FOptions|goVertLine|goHorzLine;
		}
		else
		{
			FOptions = FOptions&(0xffffffff^goVertLine)&(0xffffffff^goHorzLine);
		}
		InvalidateGrid();
	}

	int XGridSvr::InsertCol(int ACol)
	{
		return -1;
	}
	int XGridSvr::InsertRow(int ARow)
	{
		return -1;
	}
	int XGridSvr::InsertRow(int ARow,int Number,bool copyMode)
	{
		return -1;
	}
	void XGridSvr::DeleteColumn(int ACol)
	{
	}
	void XGridSvr::DeleteRow(int ARow)
	{
	}
	int XGridSvr::DeleteRow(int ARow,int Number)
	{
		return -1;
	}

	int XGridSvr::DoSizeAdjust(int Count, XUtil::xuSummableCsArray< int, int>& Extents, int DefaultExtent, int Current ,int Amount,int Index)
	{
		return -1;
	}
	void XGridSvr::SizeChanged(int OldColCount, int OldRowCount)
	{
	}
	void XGridSvr::AdjustSize(int Index, int Amount, BOOL Rows)
	{
	}

	void XGridSvr::MoveRowData(int FromIndex, int ToIndex)
	{
	}
	void XGridSvr::MoveRowData(int FromIndex, int ToIndex,int Number)
	{
	}
	void XGridSvr::MoveColData(int FromIndex ,int ToIndex)
	{
	}

	void XGridSvr::ResizeCol(int Index, int OldSize, int NewSize)
	{
	}
	void XGridSvr::ResizeRow(int Index, int OldSize, int NewSize)
	{
	}
	void XGridSvr::RowHeightChanged()
	{
	}
	void XGridSvr::ColWidthChanged()
	{
	}
	void XGridSvr::RowMoved(int FromIndex, int ToIndex)
	{
	}
	void XGridSvr::RowMoved(int FromIndex, int ToIndex,int Number)
	{
	}
	void XGridSvr::ColumnMoved(int FromIndex,int  ToIndex)
	{
	}
	void XGridSvr::MoveAdjust(int  &CellPos,int  FromIndex,int  ToIndex)
	{
	}
	void XGridSvr::MoveRow(int FromIndex, int ToIndex)
	{
	}
	void XGridSvr::MoveColumn(int FromIndex, int ToIndex)
	{
	}
	BOOL XGridSvr::GetTabStops(int Index)
	{
		return TRUE;
	}
	void XGridSvr::Click()
	{
	}
	void XGridSvr::FocusCell(int ACol, int ARow, BOOL MoveAnchor)
	{
		MoveCurrent(ACol, ARow, MoveAnchor, TRUE);
		Click();
	}
	
	bool XGridSvr::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
	HWND XGridSvr::GetHWND()
	{
		return GetGridSheetSvr()->GetHWND();
	}

	XAPI void XGridSvr::SetGridSheetSvr(IGridSheetSvr * pGridSheetSvr)
	{
		m_pGridSheetSvr = pGridSheetSvr;
	}
	XAPI IGridSheetSvr * XGridSvr::GetGridSheetSvr()
	{
		return m_pGridSheetSvr;
	}

}}}
