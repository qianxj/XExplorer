#pragma once

#include "xofbase.h"
#include "griddatasvr.hpp"
#include "xcellsheetsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API XBIReportSheetSvr :
		public XOfficeSheetSvr
	{
	public:
		XBIReportSheetSvr(void);
		~XBIReportSheetSvr(void);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		int GetObjectsCount();
		xbObject * GetObjectByIndent(int nIndex);
		int RemoveObject(int nIndex);
		int AddObject(xbObject * pObject ,RECT &rt);
	public:
		int GetWidth();
		int GetHeight();
		int GetObjectWidth(int nIndex);
		int GetObjectHeight(int nIndex);
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	public:
		virtual bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtMButtonDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtLButtonUp(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtMouseMove(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtKeyDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtSetCursor(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtSetFocus(TEvent * pEvent, LPARAM lParam);

		virtual bool EvtDoubleClicked(TEvent * pEvent, LPARAM lParam);

		virtual bool EvtMouseWheel(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtHScroll(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtVScroll(TEvent * pEvent, LPARAM lParam);
	public:
		virtual int LoadTemplate(IXMLDOMElement * pElement,int state = goNormalState);
	public:
		POINT FTopLeft;
		vector<RECT > * m_pRts;
	public:
		bool m_bWheelVert; 
	
	public:
		xfAreaObject* GetObjectAtPoint(POINT pt);
	public:
		Hxsoft::XFrame::xfControl * m_pControlEdit;
		int HideEditor();
		int ShowEditor();
		int InstanceEdit(Hxsoft::XFrame::xfControl * pControl,RECT rt);
		int MoveEditor(RECT &rt);
	public:
		POINT ptMouse;
		vector<POINT> * ptMousees;
		int dwMouseTick;
	};
}}}}
