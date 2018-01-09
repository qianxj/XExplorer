#pragma once

#include "xfWinbase.hpp"
namespace Hxsoft{ namespace XFrame
{
	class xfXPage;
	class XFRAME_API xfWin :
		public xfWinbase
	{
	public:
		xfWin(void);
		~xfWin(void); 


	public:
		// window handle
		HMODULE m_hDLL;
		xfXPage *	m_pXPage;
		POINT	m_MousePt;

	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
		virtual xfControl * CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *);
	public:
		virtual int DefCmdProcess(UINT message, WPARAM wParam, LPARAM lParam);
		virtual int OnXCommandProcess(WPARAM wParam, LPARAM lParam);
	public:
		xfWin * m_pOwnerWin;
		xfWin * m_pCmdWin;
		xfWin * m_pParentWin;
	public:
		static HWND hHookMenu;
		static HHOOK hHook;

	public:
		int SetEventListions();
		int UnSetEventListions();
	public:
		virtual bool EvtSize(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtPaint(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtXCommand(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtDestroy(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtKillFocus(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtActivate(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtMdiActivate(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtFocusNext(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtFocusPrev(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtUpdateCmdUI(TEvent *pEvent,LPARAM lparam);
		virtual bool EvtEraseBkgn(TEvent* pEvent,LPARAM lParam);
		virtual bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);

		virtual bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
		virtual bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
	public:
		virtual int Construct();
	public:
		virtual int OnPreOpen();
		virtual int OnOpen();
		virtual int OnPostOpen();
		virtual int OnOpen(xfWin * pWin);
		virtual int OnClose();
		virtual int OnCloseQuery();
	public:
		static xfWin* LoadXPage(LPCTSTR pStrUri,LPCTSTR pStrUrl,DWORD dwExStyle,DWORD dwStyle, RECT *pBoundRect,HWND hParent,HINSTANCE hInst,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
		static xfWin* LoadXPage(LPCTSTR pStrUri,LPCTSTR pStrUrl,HWND hParent,HINSTANCE hInst,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
		static xfWin* LoadXPage(LPCTSTR pXPageFile,HWND hParent,HINSTANCE hInst,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
		static xfWin* LoadXPageXml(LPCTSTR pXPageXml,HWND hParent,HINSTANCE hInst,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
		static xfWin* LoadXPageXml(xfXPage * pXPage,HWND hParent,HINSTANCE hInst,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
		static xfWin* LoadXPage(IXMLDOMElement * pElement,HWND hParent,HINSTANCE hInst,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
		static xfWin * CreateWinObject(IXMLDOMElement * pElementEx,LPCTSTR pStrUrl);
	public:
		virtual bool Open(xfXPage * pXPage,HWND hParent = NULL,HINSTANCE hInst = NULL,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
		virtual bool Open(xfXPage * pXPage,DWORD dwExStyle,DWORD dwStyle, RECT *pBoundRect ,HWND hParent = NULL,HINSTANCE hInst = NULL,LPVOID pParam=NULL,xfWin* pCmdWin=NULL);
	public:
		LPVOID m_pParam;
		LPVOID m_pFrameParam;
	public:
		xfControl * GetControl(LPCTSTR pStrName,bool bInsUnlayered);
		xfControl * GetControl(LPCTSTR pStrName);
		xfControl * GetControl(LPCTSTR pStrName,xfNode * pNode);
		void * GetIControl(LPCTSTR pStrName);
	public:
		virtual xfWin * GetFrameWindow();
	public:
		virtual int PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame);
	public:
		bool m_bAutoDeleted ;
	public:
		virtual xbObject * GetInterface(); 
	public:
		enum {st_normal = 0, st_move,st_roll,st_size};
		enum {roll_left,roll_top,roll_right,roll_bottom};
		struct Paper
		{
			POINT pt[4];
			COLORREF crBack;
			COLORREF crLine;
			DWORD state;
			HRGN rgn;
			int nStyle;
			~Paper(){::DeleteObject(rgn);}
		} * m_pPaper ;
		int SetPaperState(int nStyle = 0);
	};
}}
