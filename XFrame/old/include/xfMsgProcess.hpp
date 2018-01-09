#pragma once

#include "xbxbase.h"
namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API xfMsgProcess :public xbObject
	{
	public:
		xfMsgProcess(void);
		~xfMsgProcess(void);
	public:
		virtual int ProcessMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
		virtual int ProcessCmdMessage(UINT message, int wmId, int wmEvent);
	public:
		virtual int HandleCmdMessage(UINT message, int wmId, int wmEvent){return 0;}
		virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam){return 0;}
	public:
		//mouse
		virtual void OnLButtonDown( UINT nFlags, POINT point ){};
		virtual void OnRButtonDown( UINT nFlags, POINT point ){};
		virtual void OnMouseMove( UINT nFlags, POINT point ){};
		virtual void OnLButtonUp( UINT nFlags, POINT point ){};
		virtual void OnRButtonUp( UINT nFlags, POINT point ){};
		virtual BOOL OnSetCursor( HWND hWnd, UINT nHitTest, UINT message ){return NULL;}
	public:
		//scroll
		virtual void OnVScroll(UINT nSBCode, UINT nPos, HWND hWndCtl){};
		virtual void OnHScroll(UINT nSBCode, UINT nPos, HWND hWndCtl){};
		virtual void OnMouseWheel(POINT point,UINT_PTR	fwKeys,	short zDelta){};
	public:
		//position
		virtual void  OnSize(UINT nFlags, POINT point){};
	public:
		//keyborad
		virtual void OnChar(UINT nChar,UINT nRepCnt,UINT nFlags){};
		virtual void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags){};
		virtual void OnKeyUp(UINT nChar,UINT nRepCnt,UINT nFlags){};

	public:
		//paint
		virtual bool OnEraseBkgnd(HDC hdc){return true;}
		virtual void  OnPaint(HDC hDC){};
	public:
		//create
		virtual int OnWinCreated(UINT message,WPARAM wParam, LPARAM lParam){return 0;}
	public:
		//fouse
		virtual void OnSetFocus(HWND hHwndLost){};
		virtual void OnKillFocus(HWND hHwndFocus){};
	public:
		//active
		virtual void OnActivate(UINT nState,HWND pWndOther,BOOL bMinimized ){};
		virtual int OnMouseActivate(HWND pDesktopWnd,UINT nHitTest,UINT message){return 0;};
		virtual void OnActivateApp(BOOL bActive,DWORD dwThreadID){};

	public:
		static xbObject * m_CaptureObject;
		int  SetCapture();
		int  ReleaseCapture();
		bool IsCaptured();
		bool HasCaptured();
		bool IsNotMeCaptured();
		xbObject *  GetCaptured();
	public:
		virtual HWND GetHWnd()=0;
		virtual bool GetClientRect(RECT *pRect)=0;
	};
}}
