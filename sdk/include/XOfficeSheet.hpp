#pragma once

#include "xframe.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFFICE_API XOfficeSheet :
		public xfControl
	{
	public:
		XOfficeSheet(void);
		~XOfficeSheet(void);
	public:
		class XOfficeSheetSvr*	m_pSheetSvr;
	public:
		bool	m_pxfNodeCreated;
		int		CreatexfNode();
	public:
		bool	m_Standalone;
	public:
		bool EvtPaint(TEvent* pEvent,LPARAM lParam);
		bool EvtSize(TEvent* pEvent,LPARAM lParam);
		bool EvtEraseBkgnd(TEvent* pEvent,LPARAM lParam);

	public:
		class CXScrollBar *	m_pHScrollBar;
		class CXScrollBar *	m_pVScrollBar;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		virtual	void Initial();
		virtual int FocusedSheet();
		virtual int KillFocusedSheet();
	public:
		xfNode * m_pAgentNodeTool;
	};
}}}
