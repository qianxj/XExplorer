#pragma once
#include "xuxutil.h"
#include "svrbase.hpp"
#include "xofficesheetsvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	//class XCellFrameSvr;
	//class CGridFrameSvr;
	class XOFBaseFrm;
	class XOfficeSheet;
	class XOfficeSheetSvr;
	class CXScrollBar;
	class CXTabSheet;
	class CXNavigate;
	template class XOFFICE_API Hxsoft::XUtil::xuVector<XOfficeSheet *>;
	template class XOFFICE_API Hxsoft::XUtil::xuVector<IXMLDOMElement *>;
	class XOFFICE_API XOFBaseFrmSvr : public XOfficeSheetSvr
	{
	public:
		XOFBaseFrmSvr();
		~XOFBaseFrmSvr(void);
		bool Initial(XOFBaseFrm * m_pFrame);
	public:
		RECT	m_AreaRect;
		RECT	m_ClientRect;

		POINT	m_ptHitTest;
		//TGridState	FGridState; 

		int Draw(HDC hdc, RECT * pRectClip = NULL);
		BOOL OnSetCursor(HWND hWnd ,UINT nHitTest, UINT message);
		int OnNcHitTest(HWND hWnd,POINT point);
	public:
		XOfficeSheet*		m_pSheet;
		Hxsoft::XUtil::xuVector<XOfficeSheet *>	m_Sheets;
		Hxsoft::XUtil::xuVector<IXMLDOMElement *>	m_SheetsElement;

		CXScrollBar*		pVScrollBar[2];
		CXScrollBar*		pHScrollBar[2];
		CXTabSheet*			m_pTabSheet;
		CXNavigate*			m_pNavigate;
	public:
		HWND	m_hWnd;
		HWND GetHwnd(){return m_hWnd;}
		//int InitDrawInfo(TGridDrawInfo * pDrawInfo);
	public:
		XOFBaseFrm * m_pOfficeFrm;
	public:
		int SelectSheet(int newSheet);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		bool EvtCreate(TEvent* pEvent,LPARAM lParam);
		bool EvtSetFocus(TEvent* pEvent,LPARAM lParam);
		bool EvtSize(TEvent* pEvent,LPARAM lParam);
		bool EvtPaint(TEvent* pEvent,LPARAM lParam);
	public:
		bool SetEventListons();
		bool UnSetEventListons();
	public:
		virtual HWND GetHWND();
		virtual bool GetClientRect(RECT *pRect);
	public:
		int Reset();
	};
}}}
