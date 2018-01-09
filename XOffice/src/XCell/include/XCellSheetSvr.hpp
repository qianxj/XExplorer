#pragma once
#include "xofbase.h"
#include "optionsvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	#define CELL_SELECTCHANGED	 0x0201
	#define CELL_ITEMCHANGED     0x0202
	#define CELL_ROWDELETED		 0x0203
	#define CELL_COLUMNDELETED	 0x0204
	#define CELL_NEEDTIPTEXT	 0x0205

	class CGridFrameSvr;
	class CRowSelectorSvr;
	class CColHeaderSvr;
	class XCELL_API XCellSheetSvr :public XOfficeSheetSvr 
	{
	public:
		XCellSheetSvr(void);
		~XCellSheetSvr(void);
	public:
		CGridFrameSvr*		m_pGridFrameSvrs;
		CRowSelectorSvr *	m_pSelectorSvr;
		CColHeaderSvr *		m_pColHeaderSvr;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	
	public:
		HWND	Handle;
	public:
		virtual int SetFRect(RECT rect);
	public:
		void DrawBackGround(HDC hdc, RECT &rect);
		virtual void DrawConer(HDC hdc,RECT &rect,COLORREF color=COptionSvr::stdConerColor);
	public:
		int InitDrawInfo(TGridDrawInfo * pDrawInfo);
	public:
		virtual HWND GetHWND();
	public:
		int initialAgent(bool bInit = true);
	public:
		virtual int TemplateLoaded(IXMLDOMElement * pElement,int state=goNormalState);
	public:
		int SetRowSelectorWidth(int nWidth);
		int ResetRowSelector();
		int GetRowSelectorWidth();
		int GetRowSelectorDefaultWidth();

		int SetColHeaderHeight(int nHeight);
		int GetColHeaderDefaultHeight();
		int ResetColHeaderHeight();
		int GetColHeaderHeight();
	public:
		POINT m_ptOffset;
		POINT GetOffset();
		bool SetOffset(POINT &pt);
	public:
		COLORREF	crDirect;	//ÐÞ¸Ä
		COLORREF	crAdvance;	//½¨Òé
		COLORREF	GetDirectColor() {return crDirect;}
		void		SetDirectColor(COLORREF cr) {crDirect = cr;}
		COLORREF	GetAdvanceColor() {return crAdvance;}
		void		SetAdvanceColor(COLORREF cr) {crAdvance = cr;}
	public:
		LPCTSTR		GetNoteString(int ACol,int ARow); 
		LPCTSTR		GetOriginString(int ACol,int ARow); 
		LPCTSTR		GetAdvanceString(int ACol,int ARow); 
		
		void		SetNoteString(int ACol,int ARow,LPTSTR pStr); 
		void		SetOriginString(int ACol,int ARow,LPTSTR pStr); 
		void		SetAdvanceString(int ACol,int ARow,LPTSTR pStr); 

		void		ClearNoteString(int ACol,int ARow); 
		void		ClearOriginString(int ACol,int ARow); 
		void		ClearAdvanceString(int ACol,int ARow); 
	};
}}}}
