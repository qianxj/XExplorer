#include "StdAfx.h"
#include "xscrollbar.hpp"
#include <atltrace.h>

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	CXScrollBar::CXScrollBar(int nBar):m_pOwner(NULL),fnBar(nBar),nTrackPos(1),nPos(1),nPage(30),nMin(1),nMax(100),State(0),Themed(true),ThemeScrollbar(NULL),Enabled(true),Flat(false),SmallChange(1),LargeChange(70),AutoScrollTime(100),TimerHot(0),TimerAutoScrollDelay(0)
	{
		::SetRect(&FRect,0,0,0,0);
		SetEventListons();
	}

	CXScrollBar::~CXScrollBar(void)
	{
		UnSetEventListons();
	}

	
	int CXScrollBar::GetHScrollBarHeight()
	{
		return GetSystemMetrics(SM_CYHSCROLL);
	}

	int CXScrollBar::GetVScrollBarWidth()
	{
		return GetSystemMetrics(SM_CXVSCROLL);
	}

	bool CXScrollBar::EvtPaint(TEvent* pEvent,LPARAM lParam)
	{
		OnDraw(pEvent->paint.hdc);
		return true;
	}

	void CXScrollBar::OnDraw(HDC hdc)
	{
		if(FRect.right - this->FRect.left < 2) return ;

		this->ThemeScrollbar = ::OpenThemeData(GetDesktopWindow(),_T("scrollbar"));
		this->DrawScrollBkgnd(hdc);
		this->DrawScrollButton(pcScrollUp,hdc);
		this->DrawScrollButton(pcScrollDown,hdc);
		this->DrawScrollButton(pcThumb,hdc);
		::CloseThemeData(ThemeScrollbar);
		return ;

		RECT	rect;
		int swidth,sheight;
		int hwidth,hheight;
		
		swidth  = GetSystemMetrics(SM_CXVSCROLL);
		sheight = GetSystemMetrics(SM_CYVSCROLL);
		hwidth  = GetSystemMetrics(SM_CXHSCROLL);
		hheight = GetSystemMetrics(SM_CYHSCROLL);

		//draw scrollbar
		HTHEME theme = ::OpenThemeData(GetDesktopWindow(),_T("scrollbar"));

		//draw vscrollbar
		if(this->fnBar==SB_VERT)
		{
			/*
			rect = this->FRect;
			rect.left = rect.right - swidth;
			rect.bottom = rect.top + 6;
			::DrawFrameControl(hdc,&rect,DFC_BUTTON,DFCS_BUTTONPUSH);
			*/

			rect = this->FRect;
			rect.bottom = rect.top + sheight;
			DrawThemeBackground (theme, hdc, SBP_ARROWBTN, ABS_UPNORMAL, &rect, 0);
			
			rect.bottom = FRect.bottom ;
			rect.top = FRect.top + sheight + 6;
			DrawThemeBackground (theme, hdc, SBP_LOWERTRACKVERT, SCRBS_NORMAL, &rect, 0);
			
			int barValidHeight = FRect.bottom - FRect.top - 2*sheight;
			int ThumbHeight = ::MulDiv(nPage, barValidHeight,(nMax - nMin + 1));
			
			if(ThumbHeight < 1) ThumbHeight = 6;
			
			rect.top = FRect.top +  sheight + ::MulDiv(nPos,barValidHeight,(nMax - nMin + 1));
			rect.bottom = rect.top + ThumbHeight;
			DrawThemeBackground (theme, hdc, SBP_THUMBBTNVERT, SCRBS_NORMAL, &rect, 0);
			if( ThumbHeight > sheight) //draw grip
				DrawThemeBackground (theme, hdc, SBP_GRIPPERVERT,0 , &rect, 0);

			//draw 
			rect = this->FRect;
			rect.top = rect.bottom - sheight;
			DrawThemeBackground (theme, hdc, SBP_ARROWBTN, ABS_DOWNNORMAL, &rect, 0);

			/*
			rect.top += hheight;
			rect.bottom += hheight;
			DrawThemeBackground (theme, hdc, SBP_SIZEBOX, SZB_RIGHTALIGN, &rect, 0);
			*/
		}else
		{
			//draw horz scrollbar
			rect = this->FRect;
			rect.right = rect.left + hwidth;
			DrawThemeBackground (theme, hdc, SBP_ARROWBTN, ABS_LEFTNORMAL, &rect, 0);
			
			rect = this->FRect;
			rect.left = rect.left + hwidth;
			DrawThemeBackground (theme, hdc, SBP_LOWERTRACKHORZ, SCRBS_NORMAL, &rect, 0);

			int barValidWidth = FRect.right - FRect.left - 2*hwidth;
			int ThumbWidth = ::MulDiv(nPage, barValidWidth,(nMax - nMin + 1));
			
			if(ThumbWidth < 1) ThumbWidth = 6;
			
			rect.left = FRect.left +  hwidth + ::MulDiv(nPos,barValidWidth,(nMax - nMin + 1));
			rect.right = rect.left + ThumbWidth;
			DrawThemeBackground (theme, hdc, SBP_THUMBBTNHORZ, SCRBS_NORMAL, &rect, 0);
			if( ThumbWidth > hwidth) //draw grip
				DrawThemeBackground (theme, hdc, SBP_GRIPPERHORZ,0 , &rect, 0);


			rect = this->FRect;
			rect.left = rect.right - hwidth;
			DrawThemeBackground (theme, hdc, SBP_ARROWBTN, ABS_RIGHTNORMAL, &rect, 0);
		}

		::CloseThemeData(theme);

	}

	int CXScrollBar::Metric( TScrollDimensions Dimension)
	{
		int Result;
		int Temp;
		switch(Dimension)
		{
		case sbVArrowWidth:
			if(fnBar ==SB_VERT)
				Result = GetSystemMetrics(SM_CXVSCROLL);
			else
			{
				Temp = GetSystemMetrics(SM_CXHSCROLL);
				if(FRect.right - FRect.left > 2 * Temp)
					Result = Temp;
				else
					Result = Temp >> 1;
			}
			break;
		case sbVArrowHeight:
			if(fnBar ==SB_HORZ)
				Result = GetSystemMetrics(SM_CYVSCROLL);
			else
			{
				Temp = GetSystemMetrics(SM_CYVSCROLL);
				if(FRect.bottom - FRect.top > 2 * Temp)
					Result = Temp;
				else
					Result = Temp >> 1;
			}
			break;
		case sbVThumbHeight:
			Temp = Metric(sbVThumbClientHeight);
			Result = ::MulDiv(Temp,nPage,nMax - nMin);
			if(fnBar ==SB_VERT)
			{
				if(Result < GetSystemMetrics(SM_CYVTHUMB) /2 )
					Result = GetSystemMetrics(SM_CYVTHUMB) /2;
				if(Temp - Result < 0) Result = 0 ;
			}else
			{
				if(Result < GetSystemMetrics(SM_CXHTHUMB) /2 )
					Result = GetSystemMetrics(SM_CXHTHUMB) /2;
				if(Temp - Result < 0) Result = 0 ;
			}
			break;
		case sbVThumbClientHeight:
			if(fnBar == SB_VERT)
			{
				Temp = GetSystemMetrics(SM_CYVSCROLL);;
				if(FRect.bottom - FRect.top > 2 * Temp)
					Result = FRect.bottom - FRect.top - 2 * Temp;
				else
					Result = 0;
			}else
			{
				Temp = GetSystemMetrics(SM_CXHSCROLL);;
				if(FRect.right - FRect.left > 2 * Temp)
					Result = FRect.right - FRect.left - 2 * Temp;
				else
					Result = 0;
			}
			break;
		}
		return Result;
	}
	RECT CXScrollBar::GetScrollArea(TScrollArea emScrollArea)
	{
		RECT Result = FRect;
		switch(emScrollArea)
		{
		case saBackground:
			Result = FRect;
			if(fnBar == SB_VERT)
				InflateRect(&Result, 0, -Metric(sbVArrowWidth));
			else
				InflateRect(&Result,  -Metric(sbVArrowHeight) ,0);
			break;
		case saScrollUp:
			SetRect(&Result, Result.left, Result.top, Result.left + Metric(sbVArrowWidth), Result.top + Metric(sbVArrowHeight));
			break;
		case saScrollDown:
			SetRect(&Result, Result.right - Metric(sbVArrowWidth) , Result.bottom - Metric(sbVArrowHeight), Result.right, Result.bottom);

			break;
		 case saThumb:
			Result = GetScrollArea(saThumbClient);
			 if(fnBar == SB_VERT)
			 {
				Result.top = CalculateThumbCenterPixel() - Metric(sbVThumbHeight) /2;
				Result.bottom = Result.top + Metric(sbVThumbHeight);
			}else
			{
				Result.left = CalculateThumbCenterPixel() - Metric(sbVThumbHeight) /2;
				Result.right = Result.left + Metric(sbVThumbHeight);
			}
			break;
		case saThumbClient:
			if(fnBar == SB_VERT)
				InflateRect(&Result, 0, - Metric(sbVArrowHeight));
			else
				InflateRect(&Result, - Metric(sbVArrowWidth) , 0);
			break;
		case saThumbPageDownClient:
          Result = GetScrollArea(saThumbClient);
		  if(fnBar == SB_VERT)
			Result.top = GetScrollArea(saThumb).bottom;
		  else
			Result.left = GetScrollArea(saThumb).right;
		  break;
	    case saThumbPageUpClient:
          Result = GetScrollArea(saThumbClient);
		  if(fnBar == SB_VERT)
			 Result.bottom = GetScrollArea(saThumb).top;
		  else
			 Result.right = GetScrollArea(saThumb).left;
		  break;
		default:
			::SetRect(&Result,0,0,0,0);
		}
		return Result;
	}

	int CXScrollBar::CalcuatePositionByPixel(int APixel)
	{
		RECT R;
		int Result;
		R = GetScrollArea(saThumbClient);
		if(fnBar == SB_VERT)
		{
			//InflateRect(&R, 0, -Metric(sbVThumbHeight) /2);
			if(APixel < R.top) 
				Result = 0;
			else
			{
				if(APixel > R.bottom) 
					Result = nMax;
				else
					Result = ::MulDiv(nMax - nMin, APixel - R.top ,R.bottom - R.top);
			}
		}else
		{
			//InflateRect(&R,- Metric(sbVThumbHeight) /2,0);
			if(APixel < R.left) 
				Result = 0;
			else
			{
				if(APixel > R.right) 
					Result = nMax;
				else
					Result = ::MulDiv(nMax - nMin, APixel - R.left ,R.right - R.left);
			}
		}
		return Result;
	}

	int CXScrollBar::CalculateThumbCenterPixel()
	{
		RECT	R;
		int Result;
		R = GetScrollArea(saThumbClient); // Get the thumbs client area
		if(fnBar == SB_VERT)
		{
			if(nPos ==nMin)
				Result = R.top + Metric(sbVThumbHeight) /2;
			else 
			{
				if (nPos == nMax)
					Result = R.bottom - Metric(sbVThumbHeight) / 2;
				else
				{
					InflateRect(&R, 0, - Metric(sbVThumbHeight) /2 );
					Result = R.top + MulDiv(nPos,R.bottom - R.top,nMax - nMin);
				}
			}
		}else
		{
			if(nPos ==nMin)
				Result = R.left + Metric(sbVThumbHeight) /2;
			else 
			{
				if (nPos == nMax)
					Result = R.right - Metric(sbVThumbHeight) / 2;
				else
				{
					InflateRect(&R,  - Metric(sbVThumbHeight) /2 ,0 );
					Result = R.left + MulDiv(nPos,R.right - R.left,nMax - nMin);
				}
			}
		}
		return Result;

	}
	BOOL CXScrollBar::CanScrollPage()
	{
		RECT R;
		BOOL Result;
		if(State & ssPageDownPressed)
			R =  GetScrollArea(saThumbPageDownClient);
		else if ( State & ssPageUpPressed)
			R = GetScrollArea(saThumbPageUpClient);
		Result = PtInRect(&R,ptCurMouse);
		if(Result)
			State |= ssPageScrollPause;
		else
			State &= ~ssPageScrollPause;
		return Result;
	}

	void CXScrollBar::DrawScrollBkgnd()
	{
		HDC hdc = GetDC(GetHWND());
		DrawScrollBkgnd(hdc);
		ReleaseDC(GetHWND(),hdc);
	}

	void CXScrollBar::DrawScrollBkgnd(HDC hdc)
	{
		RECT	RUp,RDown;
		DWORD	PartType, PartState;
		
		RUp = GetScrollArea(saThumbPageUpClient);
		RDown = GetScrollArea(saThumbPageDownClient);
		if(ssPageDownPressed & State)
		{
			if(Themed)
			{
				PartType = SBP_UPPERTRACKVERT;
				PartState = SCRBS_PRESSED;
				DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &RDown, NULL);
				PartState = SCRBS_NORMAL;
				DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &RUp, NULL);
			}else
			{
				/*
				Windows.FillRect(DC, RUp, BkGndBrush.GetHWND());
				Canvas.Brush.Color = Colors.PageScrollHot;
				Windows.FillRect(DC, RDown, Canvas.Brush.GetHWND());
				*/
			}
		}else if(ssPageUpPressed & State)
		{
			if(Themed)
			{
				PartType = SBP_LOWERTRACKVERT;
				PartState = SCRBS_PRESSED;
				DrawThemeBackground(ThemeScrollbar,hdc, PartType, PartState, &RUp, NULL);
				PartState = SCRBS_NORMAL;
				DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &RDown, NULL);
			}else
			{
				/*
				Windows.FillRect(DC, RDown, BkGndBrush.GetHWND());
				Canvas.Brush.Color = Colors.PageScrollHot;
				Windows.FillRect(DC, RUp, Canvas.Brush.GetHWND());*/
			}
		}else
		{
			if(Themed)
			{
				PartType = SBP_UPPERTRACKVERT;
				PartState = SCRBS_NORMAL;
				DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &RUp, NULL);

				PartType = SBP_LOWERTRACKVERT;
				DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &RDown, NULL);
			}else
			{
				/*
				Windows.FillRect(DC, RUp, BkGndBrush.GetHWND());
				Windows.FillRect(DC, RDown, BkGndBrush.GetHWND());
				*/
			}
		}
	}
	
	void CXScrollBar::DrawScrollButton( TScrollPaintCycle Cycle)
	{
		HDC hdc = GetDC(GetHWND());
		DrawScrollButton(Cycle,hdc);
		ReleaseDC(GetHWND(),hdc);
	}
	void CXScrollBar::DrawScrollButton( TScrollPaintCycle Cycle, HDC hdc)
	{
		DWORD Flags;
		RECT R,ContentR;
		DWORD  PartType, PartState;

		if(Cycle == pcScrollUp)
		{
			R = GetScrollArea(saScrollUp);
			if(Themed)
			{
				PartType = SBP_ARROWBTN;
				PartState = fnBar==SB_VERT?ABS_UPNORMAL:ABS_LEFTNORMAL;

				if(!Enabled) 
					PartState = fnBar==SB_VERT?ABS_UPDISABLED:ABS_LEFTDISABLED;
				else if (State & ssUpPressed)
					PartState = fnBar==SB_VERT?ABS_UPPRESSED:ABS_LEFTPRESSED;
				else if(ssHotUp & State)
					PartState = fnBar==SB_VERT?ABS_UPHOT:ABS_LEFTHOT;
				DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &R, NULL);
			}else
			{
			   Flags = DFCS_SCROLLUP;
			   if(!Enabled)
				   Flags = DFCS_INACTIVE;
			   else
			   {
				   if(Flat)
					   Flags = Flags | DFCS_FLAT;
				   if(ssUpPressed & State )
				   {
					   if(Flat)
						   Flags = Flags | DFCS_PUSHED;
					   else
						Flags = Flags | DFCS_FLAT;
				   }
				   DrawFrameControl(hdc, &R, DFC_SCROLL, Flags);
			   }
			}
		}
		else if(Cycle == pcScrollDown)
		{
			R = GetScrollArea(saScrollDown);
			if(Themed)
			{
				PartType = SBP_ARROWBTN;
				PartState = fnBar==SB_VERT?ABS_DOWNNORMAL:ABS_RIGHTNORMAL;

				if(!Enabled) 
					PartState = fnBar==SB_VERT?ABS_DOWNDISABLED:ABS_RIGHTDISABLED;
				else if (State & ssDownPressed)
					PartState = fnBar==SB_VERT?ABS_DOWNPRESSED:ABS_RIGHTPRESSED;
				else if(ssHotDown & State)
					PartState = fnBar==SB_VERT?ABS_DOWNHOT:ABS_RIGHTHOT;
				DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &R, NULL);
			}else
			{
			   Flags = DFCS_SCROLLDOWN;
			   if(!Enabled)
				   Flags = DFCS_INACTIVE;
			   else
			   {
				   if(Flat)
					   Flags = Flags | DFCS_FLAT;
				   if(ssUpPressed & State )
				   {
					   if(Flat)
						   Flags = Flags | DFCS_PUSHED;
					   else
						Flags = Flags | DFCS_FLAT;
				   }
				   DrawFrameControl(hdc, &R, DFC_SCROLL, Flags);
			   }
			}
		}else if(Cycle == pcThumb)
		{
			R = GetScrollArea(saThumb);
			if(Themed)
			{
				if(R.bottom - R.top >0)
				{
					PartType = SBP_THUMBBTNVERT;
					PartState = SCRBS_NORMAL;
					if(!Enabled)
						PartState = SCRBS_DISABLED;
					else if(State & ssDraggingThumb)
						 PartState = SCRBS_PRESSED;
					else if (ssHotThumb & State)
						PartState = SCRBS_HOT;

					DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &R, NULL);
					GetThemeBackgroundContentRect(ThemeScrollbar, hdc, PartType, PartState, &R, &ContentR);

					PartType = fnBar==SB_VERT?SBP_GRIPPERVERT:SBP_GRIPPERHORZ;
					DrawThemeBackground(ThemeScrollbar, hdc, PartType, PartState, &ContentR, NULL);
				}
			}else
			{
				/*
				Canvas.Brush.Color = Colors.ThumbButton;
				FillRect(DC, R, Canvas.Brush.GetHWND());
				if not Flat then
				begin
				  Flags = EDGE_RAISED;
				  DrawEdge(DC, R, Flags, BF_RECT)
				end else
				begin
				  Canvas.Brush.Color = clBtnShadow;
				  Canvas.FrameRect(R);
				end;
				DrawEdge(DC, R, Flags, BF_RECT)
				*/
			}
		}
	}
	void CXScrollBar::FreeThemes()
	{
		if(ThemeScrollbar)
			CloseThemeData(ThemeScrollbar);
			ThemeScrollbar = NULL;
	}
	int CXScrollBar::GetMax()
	{
		return nMax - nPage;
	}
	void CXScrollBar::OpenThemes()
	{
		ThemeScrollbar = OpenThemeData(::GetDesktopWindow(), _T("scrollbar"));
	}

	void CXScrollBar::ScrollClick(POINT Pos)
	{
		if(PtInRect(&GetScrollArea(saScrollUp), Pos))
		{
			 SetState(State | ssUpPressed);
			if(!SendScrollMsg(SB_LINEUP))
			{
				this->SetPosition(nPos - SmallChange);
			}
		}else if(PtInRect(&GetScrollArea(saScrollDown), Pos))
		{
			 SetState(State | ssDownPressed);
			if(!SendScrollMsg(SB_LINEDOWN))
			{
				this->SetPosition(nPos + SmallChange);
			}
		}else if(PtInRect(&GetScrollArea(saThumb), Pos))
		{
			SetState(State | ssDraggingThumb);
		}else if(PtInRect(&GetScrollArea(saThumbPageDownClient), Pos))
		{
			 SetState(State | ssPageDownPressed);
			 if(!SendScrollMsg(SB_PAGEDOWN))
			 {
				this->SetPosition(nPos + LargeChange);
			 }
		}else if(PtInRect(&GetScrollArea(saThumbPageUpClient), Pos))
		{
			 SetState(State | ssPageUpPressed);
			 if(!SendScrollMsg(SB_PAGEUP))
			 {
				this->SetPosition(nPos - LargeChange);
			 }
		}
	}
	bool CXScrollBar::SendScrollMsg(DWORD ScrollCode, int NewPos )
	{
		UINT Msg;
		bool	Result = false;

		if(ScrollCode == SB_THUMBPOSITION || ScrollCode == SB_THUMBTRACK)
			if(NewPos>>16) ScrollCode = ScrollCode | ( NewPos>>16<<8);

		
		if(fnBar ==SB_VERT)
			Msg = WM_VSCROLL;
		else
			Msg = WM_HSCROLL;

		this->nNewPos = NewPos;
		// Send the Scrollbar message
		SendMessage(GetHWND(), Msg, MAKELONG(ScrollCode, NewPos) , ( LPARAM)this);
		// All code message are followed by EndScroll except Thumbtrack
		if(ScrollCode != SB_THUMBTRACK)
			SendMessage(GetHWND(), Msg, MAKELONG(SB_ENDSCROLL, 0), ( LPARAM)this);
		Result = true;
		return Result;
	}


	bool CXScrollBar::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		ptCurMouse = point;
		this->SetCapture();
		State |= ssMouseCaptured;
		ScrollClick(point);
		HandleAutoScrollTimer(true, true);
		return true;

	}
	bool CXScrollBar::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		ptCurMouse = point;
		this->ReleaseCapture();
		HandleAutoScrollTimer(false, false);
		if(ssDraggingThumb|State)
			SendScrollMsg(SB_ENDSCROLL);
		this->SetState(State & ~(ssDownPressed |ssUpPressed|ssMouseCaptured|ssDraggingThumb|ssPageUpPressed|ssPageDownPressed));

		return true;

	}
	bool CXScrollBar::EvtMouseMove(TEvent* pEvent,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		ptCurMouse = point;
		if(TimerHot == 0)HandleHotTimer(true);

		HandleHotTracking(point, false);

		if(State & ssMouseCaptured && State & ssDraggingThumb)
		{
			int Pos = CalcuatePositionByPixel(fnBar==SB_VERT?point.y:point.x);
			this->SetPosition(Pos);
			SendScrollMsg(SB_THUMBTRACK, Pos);
		}
		return true;
	}
	void CXScrollBar::SetPosition(int NewPos)
	{
		if(nPos != NewPos)
		{
			if(NewPos < nMin) NewPos = nMin;
			if(NewPos > nMax) NewPos = nMax;
			nPos = NewPos;
			this->DrawScrollBkgnd();
			this->DrawScrollButton(pcThumb);
		}
	}
	bool CXScrollBar::EvtTimer(TEvent* pEvent,LPARAM lParam)
	{
		int TimerID = (int)pEvent->wParam;
		//RECT R;
		switch(TimerID)
		{
		case ID_TIMERHOT:
			{
				POINT pt;
				::GetCursorPos(&pt);
				::ScreenToClient(GetHWND(),&pt);
				ptCurMouse = pt; 
				if(!PtInRect(&FRect, ptCurMouse))
				{
					HandleHotTracking(ptCurMouse, true);
					HandleHotTimer(false);
				}else
				{
					HandleHotTracking(ptCurMouse, false); 
				}
			}
			break;
		case ID_TIMERAUTOSCROLLDELAY:
			{
				HandleAutoScrollTimer(true, false);
			}
			break;
		case ID_TIMERAUTOSCROLL:
			{
				if(State & ssUpPressed)
				{
					if(!SendScrollMsg(SB_LINEUP))  SetPosition(nPos - SmallChange);
				}else if(State & ssDownPressed)
				{
					if(!SendScrollMsg(SB_LINEDOWN)) SetPosition(nPos + SmallChange);
				}else if(CanScrollPage())
				{
					if(ssPageDownPressed & State)
					{
						if(!SendScrollMsg(SB_PAGEDOWN)) SetPosition(nPos +  LargeChange);
					}else if(ssPageUpPressed & State)
					{
						if(!SendScrollMsg(SB_PAGEUP))	SetPosition(nPos -  LargeChange);
					}
				}
			}
			break;
		}
		return true;
	}
	void CXScrollBar::KillDelayTimer()
	{
		if(TimerAutoScrollDelay)
		{
			KillTimer(GetHWND(), TimerAutoScrollDelay);
			TimerAutoScrollDelay = 0;
		}
	}
	void CXScrollBar::KillScrollTimer()
	{
		if(TimerAutoScroll)
		{
			KillTimer(GetHWND(), TimerAutoScroll);
			TimerAutoScroll = 0;
		}
	}
	void CXScrollBar::HandleAutoScrollTimer(bool Enable, bool DelayTimer)
	{
		if(Enable)
		{
			if(Enable && (! DelayTimer ) && (TimerAutoScroll==NULL))
			{
				KillDelayTimer();
				TimerAutoScroll = ::SetTimer(GetHWND(),ID_TIMERAUTOSCROLL, AutoScrollTime,NULL);
			}
			else if(Enable && DelayTimer && (TimerAutoScrollDelay == 0))
			{
				KillScrollTimer();
				TimerAutoScrollDelay = ::SetTimer(GetHWND(), ID_TIMERAUTOSCROLLDELAY, TIME_AUTOSCROLLDELAY,NULL);
			}
		}else
		{
			KillDelayTimer();
			KillScrollTimer();
		}
	}

	void CXScrollBar::HandleHotTimer(bool Enable)
	{
		if(Enable && (TimerHot==0))
			TimerHot = ::SetTimer(GetHWND(),ID_TIMERHOT, TIMER_HOT,NULL);
		if((!Enable) && TimerHot)
		{
			KillTimer(GetHWND(),TimerHot);
			TimerHot = 0 ;
		}
	}

	void CXScrollBar::SetState(FScrollBarState	NewState)
	{

		if(NewState != State)
		{
			FScrollBarState OldState;
			OldState  = State;
			State = NewState;
			{
				if((OldState | State) & ssHotUp) this->DrawScrollButton(pcScrollUp);
				if((OldState | State) & ssHotDown) this->DrawScrollButton(pcScrollDown);
				if((OldState | State) & (ssHotThumb |ssDraggingThumb)) this->DrawScrollButton(pcThumb);
			}
		}
	}

	void CXScrollBar::HandleHotTracking(POINT MousePos,bool ForceClearAll)
	{
		  if(PtInRect(&GetScrollArea(saScrollUp), MousePos))
		  {
			if (!(ssHotUp & State))
			{
			  SetState((State | ssHotUp) & ~(ssHotDown | ssHotThumb));
			}
		  }else if(PtInRect(&GetScrollArea(saScrollDown), MousePos))
		  {
			if(!(ssHotDown & State))
			{
			  SetState((State | ssHotDown) & ~(ssHotUp | ssHotThumb));
			}
		  }else if(PtInRect(&GetScrollArea(saThumb), MousePos))
		  {
			if( !(ssHotThumb & State))
			{
				SetState ( (State |ssHotThumb) & ~(ssHotUp | ssHotDown));
			}
		  }else
		  if(ForceClearAll || ((State & ssHotUp && State & ssHotDown && State & ssHotThumb) && !(ssDraggingThumb & State)))
		  {
				SetState(State &  ~(ssHotUp | ssHotDown |ssHotThumb));
		  }
	}
	bool CXScrollBar::SetEventListons()
	{
		this->AttachListon(WM_PAINT,(FNEvent)&CXScrollBar::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&CXScrollBar::EvtLButtonDown);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&CXScrollBar::EvtMouseMove);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&CXScrollBar::EvtLButtonUp);
		this->AttachListon(WM_TIMER,(FNEvent)&CXScrollBar::EvtTimer);
		return true;
	}
	bool CXScrollBar::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	HWND CXScrollBar::GetHWND()
	{
		return m_pOwner->GetHWND();
	}

	bool CXScrollBar::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
}}}
