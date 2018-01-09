#pragma once

#include "xbxbase.h"
#include <vector>

using namespace std;
namespace Hxsoft{ namespace XFrame
{
	//event
	#define XFRAME_EVENT_(x) bool x(TEvent* pEvent,LPARAM lParam)
	#define XFRAME_FNEVENT_(x) (FNEvent)&x
	#define XFRAME_EVENT_CLEAR ClearAllListonsTo()
	#define XFRAME_EVENT_CLEAR_(x) x->ClearAllListonsTo()
	
	#define	WM_XCOMMAND				WM_USER + 0x01
	#define	WM_FOCUSNEXT			WM_USER + 0x02
	#define	WM_FOCUSPREV			WM_USER + 0x03
	#define	WM_PRESWITCHLAYER		WM_USER + 0x04
	#define	WM_SWITCHLAYERED		WM_USER + 0x05
	#define	WM_READNOTE				WM_USER + 0x06
	#define	WM_POSTDESTROYWINDOW	WM_USER + 0x07
	#define	WM_BINDERNOTIFY			WM_USER + 0x32
	#define	WM_NOTIFYBINDER			WM_USER + 0x33

	typedef struct tagEvent
	{
		HWND	hWnd;
		DWORD	message;
		WPARAM	wParam;
		LPARAM	lParam;
		DWORD	time;
		POINT	pt;
		union
		{
			struct {
				int wmId    ;
				int wmEvent ;
				HWND hCtrl ;
			}command;
			struct
			{
				LPTSTR pStrID;
				class xfAreaObject * pCmdFrom;
			}xcommand;
			struct {
				UINT nFlags;
				POINT point;
			} mouse;
			struct {
				UINT nFlags;
				POINT point;
			} size;
			struct {
				UINT nChar;
				UINT nRepCnt;
				UINT nFlags;
			}key;
			struct {
				HDC hdc;
			}paint;
			struct {
				HWND	hWnd ;
				UINT	nHitTest;
				UINT	mouseMsg;
			}cursor;
			struct {
				UINT nSBCode;
				UINT nPos;
				HWND hWndCtl;
			}scroll;
			struct {
				POINT point;
				UINT_PTR fwKeys;
				short zDelta;
			}wheel;
			struct {
				int idCtrl;
				LPNMHDR pnmh;
			}notify;
		};
		xfAreaObject * pSrcObject;
		LPARAM	dwRet;
	}TEvent;

	typedef int	(* FEvent)(TEvent * pEvent,LPARAM  lParam);
	typedef struct tagTXvmEvent
	{
		void * pvm;
		TCHAR * fctname;
		int nobj;
		void * pThis;
		int	(*FEvent)(TEvent * pEvent,LPARAM  lParam);
		int fn;
		int fx;
		tagTXvmEvent():pvm(0),fctname(0),nobj(0),pThis(0),FEvent(0),fn(0),fx(0){}
	}TXvmEvent;

	typedef struct tagSpecRouter 
	{
		xfAreaObject * pDestObj;
		vector<UINT> Msgs;
	}TSpecRouter;

	class XFRAME_API xfAreaObject :public xbObject
	{
	public:
		xfAreaObject(void);
		~xfAreaObject(void);
	public:
		typedef bool (xfAreaObject::*FNEvent)(TEvent *pEvent,LPARAM lparam);


		typedef struct tagTListonEvent
		{
			FNEvent  fnEvent;
			LPARAM   lparam;
			xfAreaObject* pFromObject;
			union
			{
				LPTSTR	pCtlName;
				LPTSTR	pXCmdID;
			};
			union
			{
				DWORD	dwCtlEvent;
				DWORD	nStartCmdID;
			};
			DWORD nEndCmdID;
			HWND hCtrl;
			TXvmEvent* pXvmEvent;
			tagTListonEvent::tagTListonEvent(){::ZeroMemory(this,sizeof(tagTListonEvent));}
		}TListonEvent;

		multimap<DWORD,TListonEvent>* m_pEventListons;
		vector<xfAreaObject *>* m_pListonsTo;
		vector<xfAreaObject *>* m_pEventRouters;
		vector<TSpecRouter *> * m_pEventSpecRouters;

		//vector<xfAreaObject *>* m_pEventRouters;
		bool DetachListon();
		bool DetachListon(DWORD dwMessageID);
		bool DetachListonTo(xfAreaObject * pDestObj );
		bool DetachListonTo(xfAreaObject * pDestObj, DWORD dwMessageID);
		bool DetachListonFrom(xfAreaObject * pFromObj );
		bool DetachListonFrom(xfAreaObject * pFromObj, DWORD dwMessageID);

		/*xvm attach*/
		bool AttachListon(LPTSTR pCtlName,LPTSTR pMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachListonEx(HWND hWndControl,LPTSTR pMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachListon(LPTSTR pMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);

		bool AttachListonEx(HWND hWndControl,DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachListon(LPTSTR pCtlName,DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);

		bool AttachListon(DWORD dwMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachListon(DWORD dwMessageID,LPTSTR pCtlName,DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachCtrlListon(DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		
		bool AttachListonFrom(xfAreaObject * pFromObj, DWORD dwMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachListonTo(xfAreaObject * pDest, DWORD dwMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);

		bool AttachXCommand(LPTSTR pXCmdID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachCommand(DWORD nCmdID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		bool AttachCommand(DWORD nStartCmdID,DWORD nEndCmdID,TXvmEvent* pXvmEvent,LPARAM   lParam = NULL);
		/*end xvm attach*/

		bool AttachListon(LPTSTR pCtlName,DWORD dwCtlEvent,FNEvent fnEvent,LPARAM   lParam = NULL);
		bool AttachListon(DWORD dwMessageID,FNEvent fnEvent,LPARAM   lParam = NULL);
		bool AttachListon(DWORD dwMessageID,LPTSTR pCtlName,DWORD dwCtlEvent,FNEvent fnEvent,LPARAM   lParam = NULL);
		bool AttachCtrlListon(DWORD dwCtlEvent,FNEvent fnEvent,LPARAM   lParam = NULL);
	
		bool AttachListonFrom(xfAreaObject * pFromObj, DWORD dwMessageID,FNEvent fnEvent,LPARAM   lParam = NULL);
		bool AttachListonTo(xfAreaObject * pDest, DWORD dwMessageID,FNEvent fnEvent,LPARAM   lParam = NULL);

		bool AttachXCommand(LPTSTR pXCmdID,FNEvent fnEvent,LPARAM   lParam = NULL);
		bool AttachCommand(DWORD nCmdID,FNEvent fnEvent,LPARAM   lParam = NULL);
		bool AttachCommand(DWORD nStartCmdID,DWORD nEndCmdID,FNEvent fnEvent,LPARAM   lParam = NULL);

		bool ClearAllListonsTo(bool bIncludeOwner = true);

		bool AddRouterMessageTo(TSpecRouter * pSpecRouter);

		bool AddRouterMessageTo(xfAreaObject * pDestObj);
		bool AddRouterMessageFrom(xfAreaObject * pFromObj);
		bool RemoveRouterMessageTo(xfAreaObject * pDestObj);
		bool RemoveRouterMessageFrom(xfAreaObject * pFromObj);
	public:
		virtual LPARAM ProcessMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam,bool bContinue = true);
	public:
		virtual HWND GetHWND();
		virtual bool GetClientRect(RECT *pRect);
	public:
		static xfAreaObject * m_CaptureObject;
		int  SetCapture();
		static int  SetCapture(xfAreaObject * pObj,HWND hWnd);
		static int  ReleaseCapture();
		bool IsCaptured();
		bool HasCaptured();
		bool IsNotMeCaptured();
		xfAreaObject *  GetCaptured();
	public:
		bool AttachXvmEvent(LPTSTR msgid,int pObj, LPTSTR fctname);
		bool AttachXvmEvent(LPTSTR pCtlName,LPTSTR msgid,int pObj, LPTSTR fctname);
		bool AttachXvmEvent(HWND hWndControl,LPTSTR msgid,int pObj, LPTSTR fctname);
		bool AttachEvent(LPTSTR msgid,FEvent pEvt,void * pThis);
		bool AttachEvent(LPTSTR pCtlName, LPTSTR msgid,FEvent pEvt,void * pThis);
		bool AttachVmEvent(LPTSTR msgid,int pEvt,void * pThis);
		bool AttachVmEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis);
		bool AttachWppEvent(LPTSTR msgid,int pEvt,void * pThis);
		bool AttachWppEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis);

		bool GetMssageID(LPTSTR pMsgID,DWORD &dwMsg);
		bool xvmEventRecall(TXvmEvent * pXvmEvent,TEvent * pEvent,LPARAM lParam = NULL );
	};
}}
