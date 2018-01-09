#pragma once

#include "uxtheme.h"
#include "Tmschema.h"
#include "xframe.h"

#define	ID_TIMERHOT  201             // ID for timer to track hot button activation/deactivation (CM_MouseLeave unreliable)
#define	ID_TIMERAUTOSCROLL  202      // ID for auto scroll timer
#define	ID_TIMERAUTOSCROLLDELAY  203 // ID for auto scroll delay timer

#define	TIME_AUTOSCROLLDELAY  500 // milliseconds before autoscroll starts
#define	TIMER_HOT  50            // milliseconds between checking mouse postions for mouse exit or hot button activation

#define	FScrollBarState	int
#define	ssDraggingThumb 0x0001       // The mouse is dragging the Thumb knob
#define	ssDownPressed 0x0002           // Down Button is pressed
#define	ssHotDown 0x0004              // Mouse is over the Down button
#define	ssHotThumb 0x0008             // Mouse is over the ThumbButton
#define	ssHotUp 0x0010                // Mouse is over the Up button
#define	ssMouseCaptured 0x0020        // The control has captured the mouse
#define	ssPageDownPressed 0x0040      // The Thumb client area is pressed to the down side of the thumb button
#define	ssPageScrollPause 0x0080      // The cursor has hit the Thumbbutton during the scroll and the scroll has paused
#define	ssPageUpPressed 0x0100        // The Thumb client area is pressed to the up side of the thumb button
#define	ssScrollDown 0x0200           // The control is in the middle of a Scroll down
#define	ssScrollUp 0x0400             // The control is in the middle of a Scroll up
#define	ssUpPressed 0x0800            // The Up button is pressed
#define	ssThemesAvailable 0x1000      // Caches the UsesTheme call in the JwaUxTheme.pas file
 
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFFICE_API CXScrollBar : public xfControl
	{
	public:
		enum TScrollArea {
			saBackground,          // The client area of the scrollbar
			saScrollUp,            // The scroll up/left button
			saScrollDown,          // The scroll down/right button
			saThumb,               // The scroll dragable thumbbox
			saThumbClient,         // The "client" area that the Thumb is contained in
			saThumbPageDownClient, // The "client" area between the down button and the thumb button
			saThumbPageUpClient    // The "client" area between the up button and the thumb button
		};
		enum  TScrollDimensions {
			sbVArrowWidth,       // Arrow button width of vertical scrollbar
			sbVArrowHeight,      // Arrow button height of vertical scrollbar
			sbVThumbHeight,      // Minimum thumb box height of vertical scrollbar
			sbVThumbClientHeight // Height of the client area where the Thumb is constrained
		};
		enum TScrollPaintCycle {
			pcBackground,       // Paint the background
			pcScrollUp,         // Paint the Up/Left button
			pcScrollDown,       // Paint the Down/Right button
			pcThumb             // Paint the ThumbBox
		};

		enum TScrollPaintState {
			psNormal,
			psHot,
			psPressed,
			psScrolling,
			psDisabled
		};
 
	public:
		CXScrollBar(int nBar);
		~CXScrollBar(void);
	public:
		int fnBar;
		int	nTrackPos;
		int	nPos;
		int	nPage;
		int	nMin;
		int	nMax;
		int nNewPos;
	public:

		//HWND	Handle;

		RECT	FRect;
		FScrollBarState	State;

		POINT	ptCurMouse;	
		bool	Themed;
		HTHEME	ThemeScrollbar;
		bool	Enabled;
		bool	Flat;
		int		SmallChange;
		int		LargeChange;
		xfAreaObject *m_pOwner;
	public:
		void OnDraw(HDC hdc);
		virtual bool EvtPaint(TEvent* pEvent,LPARAM lParam);
		virtual bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		virtual bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
		virtual bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
		virtual bool EvtTimer(TEvent* pEvent,LPARAM lParam);
	public:
		static int GetHScrollBarHeight();
		static int GetVScrollBarWidth();
	public:
		RECT GetScrollArea(TScrollArea emScrollArea);
		int Metric( TScrollDimensions Dimension);
		int CalcuatePositionByPixel(int APixel);
		int CalculateThumbCenterPixel();
		BOOL CanScrollPage();
		void  DrawScrollBkgnd();
		void  DrawScrollBkgnd(HDC hdc);
		void DrawScrollButton( TScrollPaintCycle Cycle, HDC hdc);
		void DrawScrollButton( TScrollPaintCycle Cycle);
		void FreeThemes();
		int GetMax();
		void OpenThemes();
		void ScrollClick(POINT Pos);
		bool SendScrollMsg(DWORD ScrollCode, int NewPos = 0);
	public:
		void KillDelayTimer();
		void KillScrollTimer();
		void CXScrollBar::HandleAutoScrollTimer(bool Enable, bool DelayTimer);
		void HandleHotTimer(bool Enable);
		void HandleHotTracking(POINT MousePos,bool ForceClearAll);
		void SetState(FScrollBarState	NewState);
		void SetPosition(int NewPos);
	public:
		UINT_PTR TimerAutoScrollDelay;
		UINT_PTR TimerAutoScroll;
		UINT_PTR TimerHot;
		UINT AutoScrollTime;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	public:
		virtual HWND GetHWND();
		virtual bool GetClientRect(RECT *pRect);
	};
}}}
