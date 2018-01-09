#pragma once

#include "xbxbase.h"
#include "xfareaobject.hpp"
#include "vector"
#include "itrans.hpp"

#ifdef JS_SUPPER
/* 
 * This define is for Windows only, it is a work-around for bug 661663. 
 */  
#ifdef _MSC_VER  
#define XP_WIN  
#endif  
  
/* Include the JSAPI header file to get access to SpiderMonkey. */  
#include "jsapi.h"

#endif

#define WM_IDLEUPDATECMDUI  0x0363  
#define XFrameWndClass _T("XFrameWndClass")

namespace Hxsoft{ namespace XFrame
{
	class ITrans;
	class XFRAME_API xfApp :
		public xbObject
	{
	public:
		xfApp(void);
		~xfApp(void);
	public:
		bool m_bIdling ;
		int OnIdle(int nLevel);
		int Idle(DWORD dwTime);
	public:
		int InitApp();
		int ExitApp();
	public:
		std::vector<HWND>* m_pWins;
		int AddhWnd(HWND hWnd);
		int RemovehWnd(HWND hWnd);
	public:
		HINSTANCE m_hInst;
		HWND m_hWnd;
		HWND m_hMdiClient;
	public:
		int XFrameStartUp(HINSTANCE hInstance,DWORD dwIcon,DWORD dwIconSm);
		int XFrameShutDown(HINSTANCE hInstance);
		static LRESULT CALLBACK XFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK XFrameDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		HINSTANCE GetInstance();
	public:
		static xfApp * GetApp();
	public:
		const wchar_t* GetServerUrl();
	public:
		static bool m_bShutDown;

	public:
		LPTSTR	m_pBrowerModulaPath;
		LPTSTR	m_pXConfigPath;
	public:
		static 	int CreateXmlDoc(IXMLDOMDocument * &pXmlDocument);
	public:
		class xfNetwork * m_pNetwork;
	public:
		xbObjectMgr	m_ObjectMgr;
		class XShapeMgr* m_pShapeMgr;
		void InitObjectMgr();
	public:
		class xfConnect * m_pConnect;
		GUID		guid;
	public:
		CRITICAL_SECTION m_csCapture;
	public:
		class xfWin* m_pFrame;

	public:
		class xfCacheMgr * m_pCacheMgr;
	public:
		LPTSTR m_WorkEnv;
		LPTSTR m_pUser;
		LPTSTR m_pPassWord;
		LPTSTR m_pUID;
		bool m_bDev;
		xfWin * m_pDefaultView;
	public:
		LPCTSTR GetUser();
		LPCTSTR GetPassWord();
	public:
		ITrans * m_pTrans;
		ITrans * GetTrans(LPCTSTR pServerUrl,LPCTSTR pUser,LPCTSTR pAuthor);
	public:
		void * m_xsscp;
		void * m_xsmparser;
		void * m_vmlang;
		void * m_wpp;
		void * m_wpp5;
		class xfWin* m_pDevFrame;
		class xfControl* m_pOutPut;
	public:
		int m_tipcount;
	public:
		static int trace(LPCTSTR pstr);
#ifdef JS_SUPPER
	public:
		JSRuntime *m_jsrt;  
		JSContext *m_jscx;  
		JSObject  *m_jsglobal;  
#endif
	};
}}