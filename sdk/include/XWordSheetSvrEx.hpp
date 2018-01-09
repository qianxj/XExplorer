#pragma once
#include <xframe.h>
#include "xuxutil.h"
#include "xofbase.h"
#include "xwordlayer.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWORD_API XWordSheetSvr:
		public XOfficeSheetSvr
	{
		class CParagraph;
		class CChapter;
		class CDocument;
		class CBlock;
		class CPage;
		class CBaseBlock
		{
		public:
			CBaseBlock();
			~CBaseBlock();
		public:
			RECT	m_AreaRect;
			RECT	m_ContentRect;

		public:
			int		m_nHeight;
		public:
			bool CalcContentRect();
		public:
			bool	m_bRecalcPosition; //should recalc position
			Hxsoft::XUtil::xuVector<RECT> m_Rects;
			Hxsoft::XUtil::xuVector<int> m_RectStrPos;
		public:
			virtual bool	HitedBlock(POINT pt,bool bInRects = false);
		public:
			virtual bool AdjustExtent();
			virtual bool AdjustExtent(HDC hPaintDC)=0;
			virtual int GetExtent(SIZE &size);
		public:
			virtual CBlock * GetObjectAtPoint(POINT pt)=0;
			virtual int GetRect(RECT &rect)=0;
		public:
			virtual int DoDraw(HDC hPaintDC, POINT ptOffset)=0;
		public:
			virtual int GetIndexFromParent() = 0;
			virtual void * GetNextObject()=0;
			virtual void* GetPeriorObject()=0;
		public:
			virtual  int GetHeight();
		public:
			void*	m_pParent;
			void*	GetParent();
			void	SetParent(void*	pParent);
		};

		class CBlock : public CBaseBlock
		{
		public:
			CBlock();
			~CBlock();
		public:
			LPTSTR	m_strText;
		public:
			int	m_nRowHeight;
			int m_nFontSize;
		public:
			int	m_nCurIndex;
			int m_nCurPos;
			int m_nCurPoint;
			int m_nCaretHeight;
			int* m_pDx;
		public:
			int GetCaretPoint(POINT &pt);
		public:
			virtual bool AdjustExtent(HDC hPaintDC);
			virtual CBlock * GetObjectAtPoint(POINT pt);
			virtual int GetRect(RECT &rect);
			virtual int GetIndexFromParent();
			virtual void * GetNextObject();
			virtual void* GetPeriorObject();
		public:
			int SetFont(LOGFONT &LogFont);
			int SetCssStyle(LPTSTR cssStyle);
			int MargeCssStyle(LPTSTR cssStyle);
			int GetFont(LOGFONT &LogFont);
		public:
			virtual int DoDraw(HDC hPaintDC,POINT ptOffset);
		public:
			CParagraph * GetParagraph();
		};

		class CParagraph : public CBaseBlock
		{
		public:
			CParagraph();
			~CParagraph();
		public:
			Hxsoft::XUtil::xuVector<CBlock *> m_Blocks;
			int GetHeight();
		public:
			virtual int DoDraw(HDC hPaintDC, POINT ptOffset);
		public:
			virtual bool AdjustExtent(HDC hPaintDC);
			virtual CBlock * GetObjectAtPoint(POINT pt);
			virtual int GetIndexFromParent();
			virtual int GetRect(RECT &rect);
		public:
			virtual void * GetNextObject();
			virtual void* GetPeriorObject();
		public:
			CChapter * GetChapter();
		public:
			int		nPreSpaceOfFirstRow;
		};

		class CChapter : public CBaseBlock
		{
		public:
			CChapter();
			~CChapter();
		public:
			Hxsoft::XUtil::xuVector<CParagraph *> m_Paragraph;
		public:
			virtual int DoDraw(HDC hPaintDC, POINT ptOffset);
		public:
			virtual bool AdjustExtent(HDC hPaintDC);
			virtual CBlock * GetObjectAtPoint(POINT pt);
			virtual int GetRect(RECT &rect);
			virtual int GetIndexFromParent();
		public:
			virtual void * GetNextObject();
			virtual void* GetPeriorObject();
		public:
			CDocument * GetDocument();
		};

		class CDocument : public CBaseBlock
		{
		public:
			CDocument();
			~CDocument();
		public:
			Hxsoft::XUtil::xuVector<CChapter *> m_Chapters;
			LPTSTR	m_strFile;
			int	m_nCurPage;
		public:
			virtual bool AdjustExtent(HDC hPaintDC);
			virtual CBlock * GetObjectAtPoint(POINT pt);
			virtual int GetRect(RECT &rect);
			virtual int GetIndexFromParent();
		public:
			virtual void * GetNextObject();
			virtual void * GetPeriorObject();
		public:
			virtual int DoDraw(HDC hPaintDC, POINT ptOffset);
		public:
			CPage	* m_pPage;
		};

		class CPage
		{
			RECT m_rtPage;
		};

	public:
		XWordSheetSvr();
		~XWordSheetSvr();
	public:
		//Hxsoft::XUtil::CCssStyle* pCssStyles;
		template class XWORD_API Hxsoft::XUtil::xuVector<CDocument *>;
		Hxsoft::XUtil::xuVector<CDocument *> m_Documents;
		int m_nCurrentDoc;
		POINT	m_ptOffset;
	public:
		XWordLayer* m_pWordLayer;
	public:
		int nCaretPosX,           // x-position of caret 
			nCaretPosY,           // y-position of caret 
			nCaretHeight;           // height of caret 
		UINT uOldBlink;            // previous blink rate 
		int x, y;                  // coordinates for text matrix 
		TEXTMETRIC tm;
	public:
		virtual int HandleMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam);
	public:
		int MoveCaret(bool bCreate = false);
		int MoveCaret(POINT pt,bool bCreate = false);
	public:
		CBlock * m_pBlock;
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		virtual int SetFRect(RECT rc);


	public:
		virtual void OnLButtonDown(UINT nFlags,POINT point);
		virtual void OnLButtonUp(UINT nFlags,POINT point);
		virtual void OnMouseMove(UINT nFlags,POINT point);
		virtual void OnChar( UINT nChar,UINT nRepCnt,UINT nFlags);
		virtual int  OnSize(UINT nFlags, POINT point);
	public:
		virtual int LoadTemplate(IXMLDOMElement * pElement);
	};
}}}}
