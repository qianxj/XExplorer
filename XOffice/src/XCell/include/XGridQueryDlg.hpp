#pragma once
#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XGridQueryDlg :
		public xfWin
	{
	public:
		XGridQueryDlg(void);
		~XGridQueryDlg(void);
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
		virtual int OnOpen();
	public:
		bool EvtActive(TEvent* pEvent,LPARAM lParam);
		bool EvtGridDBClick(TEvent* pEvent,LPARAM lParam);
		bool EvtGridKeyDown(TEvent* pEvent,LPARAM lParam);
		bool m_bCanDestroy;
		bool m_bCloseFlag;

	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	public:
		int OnOpenQuery(LPTSTR pData);
		int OnOpenQueryEx(LPTSTR pQueryStr);
		bool OnOK();
	public: 
		WNDPROC m_PrevWndFunc;
		static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	};
}}}}
