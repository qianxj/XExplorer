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
		class XWinDesign:public XControl
		{
		public: 
			// window handle
			IXMLDOMDocumentPtr m_pXmlDoc;
			XNode *	m_pOldNode;
			POINT	m_MousePt;

		public:
			XSpliterBar *	m_pSpliterBar;
		public:
			XWinDesign(void):m_pOldNode(NULL),m_pSpliterBar(NULL){m_MousePt.x=0;m_MousePt.y=0;};
		public:
			~XWinDesign(void);
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
			virtual int LoadDesignFrame(LPTSTR xmlTemplate,bool bFile);
		};
	}
}