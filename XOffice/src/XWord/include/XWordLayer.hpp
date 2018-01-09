#pragma once
#include "xbxbase.h"
#include "xworddatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	#define TShiftState int
	#define  ssCtrl                 0x0001
	#define  ssShift                0x0008
	#define  ssAlt                  0x0004

	#define TWordState	DWORD
	#define TWordState_Normal		0x00000000
	#define TWordState_Selecting	0x00000001
	#define TWordState_Readonly		0x00010000

	#define	STI_TEXTCOLOR	0x00000200
	#define	STI_TEXTBKCOLOR	0x00000400
	#define	STI_BKCOLOR		0x00000800

	class XWordLayer :
		public xbObject
	{
	public:
		class XWordDocumentBlock;

		struct XWordBlockExtentInfo
		{
			//in args
			int startx;
			int endx;
			HDC	hPaintDC;

			//out args
			int cx;
			int cy;
			int rows;
			int rowheight;
			int xPosLast;
			int nFitFirstRow;
			LPTSTR pStr;
			int nStrLen;
			int *  pDx;
			std::vector<int>  nFits;
			std::vector<int>  nZWFits;
			std::vector<int>  nLens;
			XWordBlockExtentInfo():hPaintDC(0),cx(0),cy(0),rows(0),rowheight(0),xPosLast(0),nFitFirstRow(0),pStr(NULL),nStrLen(0),pDx(NULL){}
			~XWordBlockExtentInfo(){if(hPaintDC && pDx) delete pDx;}
		};

		struct XWordBlock
		{
			int		m_nCssStyle;
			DWORD	m_dwOption;

			XWordDataBase * m_pContent;
			XWordBlock * m_pBlockParent;
			std::vector<XWordBlock * >* m_pChilds;

			DWORD	dwPositin;
			int x;
			int y;
			int width;
			int height;
			RECT	m_AreaRect;
			RECT	m_ContentRect;

			//operate
			XWordBlock():m_nCssStyle(-1),m_dwOption(0),dwPositin(0),x(0),y(0),width(0),height(0),m_pContent(NULL),
				m_pBlockParent(NULL),m_pChilds(new std::vector<XWordBlock * >)
			{
				::SetRect(&m_AreaRect,0,0,0,0);
				::SetRect(&m_ContentRect,0,0,0,0);
			};
			~XWordBlock()
			{
				if(m_pChilds)
				{
					for(int i=0;i<(int)m_pChilds->size();i++)
					{
						delete (*m_pChilds)[i];
					}
					delete m_pChilds;
				}
			}
		public:
			XWordDocumentBlock * GetDocumentBlock();
			virtual XWordBlock * CreateWordBlock(XWordDataBase * pWordDataBase);
		public:
			virtual int AdjustExtent();
			virtual int CalcContentRect();
			virtual bool CalcExtent(XWordBlockExtentInfo &extentInfo);
		public:
			virtual int DoDraw(HDC hPaintDC,POINT ptOffset);
			virtual int DoDrawBackGround(HDC hPaintDC,POINT ptOffset);
			virtual int LoadData(XWordDataBase * m_pContent,bool bLoadChild=true);
			virtual bool GetRects(std::vector<RECT> & rects);
		public:
			int GetIndexFromParent();
			XWordBlock  * GetPirorTextBlock();
			XWordBlock  * GetNextTextBlock();
		public:
			bool BlockIsSelected(XWordBlock * pStartBlock,XWordBlock * pEndBlock);
		};

		struct XWordSpanBlock : public XWordBlock
		{
			virtual bool GetRects(std::vector<RECT> & rects);
		};

		struct XWordSpanNullBlock : public XWordSpanBlock
		{
		};

		struct XWordTextBlock : public XWordSpanBlock
		{
			int* m_pDx;
			int m_nRowHeight;
			XWordTextBlock():m_pDx(NULL),m_nRowHeight(0){};
			~XWordTextBlock(){if(m_pDx) delete m_pDx;}
			virtual bool CalcExtent(XWordBlockExtentInfo &extentInfo);
			virtual int GetFitedWidth(int startChar,int nFited);
		};

		class XWordSpanControlBlock:public XWordSpanBlock
		{
			virtual bool CalcExtent(XWordBlockExtentInfo &extentInfo);
		};

		struct XWordParagraphRowInfo
		{
			struct RowBlock
			{
				XWordBlock * pBlock;
				int nStartCharPos;
				int nFited;
				int nStartX;
				int nLen;
				int nRowheight;
				int nZWFited;
			};
			std::vector<RowBlock> m_rowBlocks;
			int nRowHeight;
			int nBala;
		};

		class XWordContentParagraph:public XWordBlock
		{
		public:
			virtual int AdjustExtent();
			virtual bool GetRects(std::vector<RECT> & rects);
		};

		class XWordControlParagraph:public XWordContentParagraph
		{
		public:
			int DoDraw(HDC hPaintDC,POINT ptOffset);
		};

		class XWordParagraphBlock:public XWordContentParagraph
		{
		public:
			std::vector<XWordParagraphRowInfo> m_rowInfos;
			virtual int AdjustExtent();
		public:
			virtual int DoDraw(HDC hPaintDC,POINT ptOffset);
			virtual int DoDraw(HDC hPaintDC,POINT ptOffset,int & nStartRow, int & nHeight);
		};

		class XWordChapterBlock:public XWordBlock
		{
		public:
			virtual int AdjustExtent();
			virtual bool GetRects(std::vector<RECT> & rects);
		};

		class XWordDocumentBlock:public XWordBlock
		{
		public:
			XWordDocumentBlock():m_pLayer(NULL){};
			XWordDocumentBlock(XWordLayer * pLayer):m_pLayer(pLayer){};
		public:
			XWordLayer *	m_pLayer;
		public:
			virtual int AdjustExtent();
		};

		struct XWordCursorInfo
		{
			int nRow;
			int nCol;
			//int nBlock;
			//int nPosChar;
			int XPosCursur;
			int YPosCursur;
			int CursorHeight;
			//bool bControlParagraph;
			//XWordBlock * m_pParagraphBlock;
		};

		struct XWordSelection
		{
			//XWordBlock * pStartBlock;
			//int nStartPosChar;
			//XWordBlock * pEndBlock;
			//int nEndPosChar;
			int nStartRow;
			int nStartCol;
			int nEndRow;
			int nEndCol;
			bool bSelection;
			bool blast;
		};

	public:
		XWordLayer(void);
		~XWordLayer(void);
	public:
		XWordDataSvr*	m_pWordData;
		XWordDocumentBlock * m_pWordDocumentBlock;
		class XWordSheetSvr	* m_pSheetSvr;
	public:
		RECT m_rcPage;
		RECT m_rcPageContent;
		RECT m_rcPageMargin;
	public:
		TWordState m_State;
	public:
		int FOptions;
		RECT FRect;
	public:
		XWordCursorInfo m_CursorInfo;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		int CalcContentPageRect(int nPage,RECT * pDrawRect, RECT &rc);
	public:
		XWordBlock * GetObjectAtPoint(POINT pt,XWordParagraphRowInfo::RowBlock * &pRowBlock);
	public:
		int PosBlockIndex(int &y,std::vector<XWordBlock * > * pItems);
		int PosRowIndex(int &y,std::vector<XWordParagraphRowInfo> * pItems,int nStartRow=0);
		int PosCharIndex(int x,XWordParagraphRowInfo* pRowInfo,int &nPosChar,int &posX);
		int PosCharIndexEx(int x,XWordParagraphRowInfo* pRowInfo,int &nPosChar,int &posX,int &nCol);

		bool CalcCursorInfo(XWordCursorInfo & CursorInfo,int nRow,int nCol);
		bool CalcCursorInfoEx(XWordCursorInfo & CursorInfo,XWordBlock * pBlock,int nPosChar);
		bool CalcCursorInfoAtPoint(POINT pt,XWordCursorInfo & CursorInfo);
		void InitCursorInfo(XWordCursorInfo &cursorInfo){::ZeroMemory(&cursorInfo,sizeof(XWordCursorInfo));}
		void ShowCurrentCaret(bool bShow);

		int GetColCount(int nRow);
		int GetRowCount();
		bool CalcLeftCursorInfo(XWordCursorInfo & CursorInfo);
		bool CalcRightCursorInfo(XWordCursorInfo & CursorInfo);
		bool CalcUpCursorInfo(XWordCursorInfo & CursorInfo);
		bool CalcDownCursorInfo(XWordCursorInfo & CursorInfo);
		int  GetPageOfBlock(XWordBlock * pBlock,int nPosChar);
		int  GetPageOfParagraph(XWordParagraphBlock * pParagraph,int nParaRow);
		int  CalcParagraphRow(int nRow,XWordParagraphBlock * &pPara,int &nParaRow);
		int  CalcBlockCol(XWordParagraphBlock * pPara,int nParaRow,int nCol,int &nBlock,int &nCharPos,bool bRight = false);
		XWordBlock * GetBlock(int nRow,int nCol);
		XWordParagraphBlock * GetParagraphByRow(int nRow);
		XWordParagraphRowInfo::RowBlock * GetRowBlock(int nRow,int nCol);
		int RemoveBlock(XWordBlock * pBlock);

		int  GetBlockRowCol(XWordBlock * pBlock,int &nStartRow,int &nStartCol,int &nEndRow,int &nEndCol);
		int  GetChaterRow(XWordBlock * pChater);
		int  GetChaterRowCount(XWordBlock * pChater);
		int  GetParahraphRow(XWordBlock * pParahraph);
		int  GetParahraphRowCount(XWordBlock * pParahraph);
	public:
		int BlockCmp(XWordBlock * pFirst,XWordBlock* pTwo);

	public:
		XWordSelection m_Selection;
		void InitSelection(XWordSelection &WordSelection){ZeroMemory(&WordSelection,sizeof(XWordSelection));}
		bool BlockIsSelected(XWordBlock * pBlock,XWordBlock * pStartBlock,XWordBlock * pEndBlock);
		bool AdjustSelection(XWordCursorInfo & CursorInfo,TShiftState state);
		bool CursorInfoInSelection(XWordCursorInfo & CursorInfo);
		bool StandOnlySelection(XWordSelection &Selection);
		bool StandOnlySelection(XWordSelection &Selection,XWordBlock * &pStartBlockEx,XWordBlock * &pEndBlockEx);
		int GetBlockAndCharPos(int nRow,int nCol,XWordBlock * &pBlock,int &nCharPos,bool bRight=false);
		bool StandOnlyToPara(int nRow,int nCol);
		bool MerginPara(XWordParagraphBlock * pDestPara,XWordParagraphBlock * pSrcPara);

	public:
		bool DeleteChar(XWordCursorInfo & CursorInfo);
		bool BackDeleteChar(XWordCursorInfo & CursorInfo);
		bool InsertChar(XWordCursorInfo & CursorInfo,TCHAR chr);
	public:
		void SetFontBold();
		void SetFontItalic();
		void SetFontUnderline();
		void SetFont(LOGFONT &logFont,COLORREF color);
		bool ChooseFontDlg();
		bool ChooseColorDlg(DWORD flag);
		void SetColor(COLORREF color, DWORD flag);
		void SetAlign(DWORD flag);
	public:
		int	m_nPage;
		POINT	m_ptScroll;
		CXScrollBar * GetHScrollBar();
		CXScrollBar * GetVScrollBar();
		void WMVScroll(UINT nSBCode, UINT nPos, HWND hWndCtl);
		void WMHScroll(UINT nSBCode, UINT nPos, HWND hWndCtl);
		void WMWheel(POINT point,UINT_PTR	fwKeys,	short zDelta);
		int CalcScrollBar(int Value, int ScrollCode, int ScrollBar, int Pos);
		void ModifyScrollBar(int ScrollBar, int ScrollCode, int Pos);
		void UpdateScrollPos(int ScrollBar);
		int ScrollBarMin(int ScrollBar);
		int ScrollBarMax(int ScrollBar);
		int PageUp(int ScrollBar);
		int  PageDown(int ScrollBar);
		int GetTotalHeight();
		int GetMaxScrollExtent(int ScrollBar);
		bool ClampInView(const XWordCursorInfo & CursorInfo);
	public:
		int CalcPages();
		bool GetFirstRowOfPage(int nPage,int & nParaRow,XWordContentParagraph * & pParaBlockEx);
		bool DoDrawPage(HDC hPaintDC,int nPage,POINT ptOffset);
		int SetFRect(RECT &rc);
	public:
		int DoPaste();
		int DoCopy();
		int DoCut();
	};
}}}}
