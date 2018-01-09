#pragma once

#define XFrameWndClass _T("XFrameWndClass")

#include "xutil.hpp"
#include <vector>
#include "xnode.hpp"
#include <commctrl.h>

using namespace std;

#include "xwinbase.hpp"
namespace Hxsoft
{
	namespace XFrame
	{
		class XNode;
		class XSpliterBar;
		class XFRAME_API XWin:public XWinbase
		{
		public:
			// window handle
			HMODULE m_hDLL;
			IXMLDOMDocumentPtr m_pXmlDoc;
			XNode *	m_pNode;
			POINT	m_MousePt;
			vector<HIMAGELIST> m_hImageLists;

		public:
			XSpliterBar *	m_pSpliterBar;
		public:
			XWin(void):m_hDLL(NULL),m_pNode(NULL),m_pSpliterBar(NULL){m_MousePt.x=0;m_MousePt.y=0;};
		public:
			~XWin(void);
		public:
			virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
			virtual int OnLButtonDown( UINT nFlags, POINT point ) ;
			virtual int  OnMouseMove( UINT nFlags, POINT point );
			virtual int OnLButtonUp( UINT nFlags, POINT point );
			virtual BOOL OnSetCursor( HWND hWnd, UINT nHitTest, UINT message );
			virtual int OnSize(UINT nFlags, POINT point);
			virtual bool OnEraseBkgnd(HDC hdc);
			virtual int OnPaint(HDC hDC);
		public:
			static XWin* CreateByXPage(LPTSTR xmlTemplate,bool bFile,HWND hParent,HINSTANCE hInst);
		private:
			virtual int LoadTemplate(LPTSTR xmlTemplate,bool bFile,HWND hParent,HINSTANCE hInst);
		};
	}
}