#pragma once

#define XFrameWndClass _T("XFrameWndClass")

namespace Hxsoft
{
	namespace XFrame
	{
		class XTimer;
		class XEditTemplate;
		class XFRAME_API XApp
		{
		public:
			XApp();
			~XApp();
		public:
			HINSTANCE m_hInst;
			HWND m_hWnd;
			HWND m_hMdiClient;
			HMODULE m_hSEditDll;
			//HMODULE m_hHtmlDll;
		public:
			int XFrameStartUp(HINSTANCE hInstance);
			int XFrameShutDown(HINSTANCE hInstance);
			static LRESULT CALLBACK XFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			HINSTANCE GetInstance();
		public:
			static XApp * GetXApp();
		public:
			static DWORD WINAPI TimerThreadProc( LPVOID lpParam );
			static DWORD WINAPI TimerEventThreadProc( LPVOID lpParam );
			static bool m_bShutDown;
			static HANDLE m_hThreadTimer;
			
			XTimer **	m_pTimers;
			int	m_timerCount; 
			int m_timeAllocSize;
			
			int AddTimer(XTimer * pTimer);
			int RemoveTimer(XTimer * pTimer);
		public:
			XEditTemplate*	m_pXEditTemplate;
			XEditTemplate* GetXEditTemplate();
		public:
			LPTSTR	m_pBrowerModulaPath;
			LPTSTR	m_pXConfigPath;
		public:
			static 	int CreateXmlDoc(IXMLDOMDocument * &pXmlDocument);
		};
	}
}