#pragma once
#include "xcellsheetsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XGridSheetSvrEx : public XOfficeSheetSvr
	{
	public:
		XGridSheetSvrEx(void);
		~XGridSheetSvrEx(void);
	public:
		virtual int TemplateLoaded(IXMLDOMElement * pElement,int state=goNormalState);
		virtual int LoadTemplate(IXMLDOMElement * pElement,int state = goNormalState);
	public:
		class XGridReport*			m_pGridReport;
		class XGridRowSelector *	m_pSelectorSvr;
		class XGridHeaderEx *		m_pColHeaderSvr;
		class XGridSummury *		m_pXGridSummurySvr;
	public:
		virtual int SetFRect(RECT rect);
		virtual HWND GetHWND();
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		void DrawBackGround(HDC hdc, RECT &rect);
		virtual void DrawConer(HDC hdc,RECT &rect,COLORREF color=COptionSvr::stdConerColor);
	public:
		CXScrollBar * GetHScrollBar();
		CXScrollBar * GetVScrollBar();
	public:
		virtual bool EvtLButtonDown(TEvent * pEvent, LPARAM lParam);
		virtual bool EvtKeyDown(TEvent *pEvent ,LPARAM lParam);
		bool EvtRetrieveRow(TEvent * pEvent, LPARAM lParam);
		bool EvtRetrievFinish(TEvent *pEvent ,LPARAM lParam);
	public:
		int InitDrawInfo(struct TGridDrawInfoEx * pDrawInfo);
	public:
		int ImportString(LPCTSTR pStr,LPCTSTR pType=NULL,bool Header = false);
		int ImportClipBoard();
	};
}}}}
