#pragma once
#include <vector>
#include "XFlowItem.hpp"
#include "XWordSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	#define TWordState	DWORD
	#define TWordState_Normal			0x00000000
	#define TWordState_Selecting		0x00000001
	#define TWordState_ObjectMoving		0x00000002
	#define TWordState_ObjectResizing	0x00000003
	#define TWordState_ObjectCreating	0x00000004
	#define TWordState_ImageCreating	0x00000005
	#define TWordState_ShapeCreating	0x00000006

	typedef int TWordDrawState;
	#define WDSSELECTION		0x0000
	#define WDSIMAGESELECTION	0x0001
	#define WDSSHAPESELECTION	0x0002
	#define WDSOBJECTSELECTION	0x0003

	#define  goThumbTracking         0x8000
	/**
		表示正常数据流排版的空间
	*/

	#define WVIEW_NORMAL		0x0000
	#define WVIEW_HTML			0x0001
	#define WVIEW_PAGE			0x0002
	#define WVIEW_READER		0x0003
	#define	WVIEW_NODRAWRULER	0x1000

	struct XWordSelectionEx
	{
		XWordDataBase * pStartBlock;
		int nStartPosChar;
		XWordDataBase * pEndBlock;
		int nEndPosChar;
		bool bSelection;
		bool bLast;
	};

	class XFlowChapter;
	class XFlowDocument : public XFlowItem
	{
	public:
		XFlowDocument(void);
		~XFlowDocument(void);
	public:
		XWordDataDocument * m_pContent;
	public:
		//显示流空间的内容
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		int CalcContentPageRect(int nPage,RECT * pDrawRect, RECT &rc);
		int DrawPageMarker(HDC hPaintDC,RECT &rcPage,RECT &rcPageMargin);
	public:
		RECT m_rcPage;
		RECT m_rcPageContent;
		RECT m_rcPageMargin;
	public:
		int m_nPage;
	public:
		int SetFRect(RECT &rc);
		//获取外部区域
		const RECT & GetFRect();
	public:
		//获取设定高度和宽度信息
		virtual POINT GetExtent();
		//获取实际占用的高度和宽度信息
		virtual POINT GetRealExtent();
		//重新计算位置信息
		virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC);
	public:
		int DoDraw(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,RECT * pDrawRect=NULL,RECT * pUpdateRect=NULL);
	public:
		XFlowChapter * GetChapterAtPointer(XWordDataSvr* pDataSvr,POINT pt);
		bool CalcCursorInfoAtPoint(XWordDataSvr* pDataSvr,POINT pt,XWordCursorInfoEx & CursorInfo);
		bool CalcCursorPosition(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle,XWordDataTextBlock *pBlock,int nTextIndex,XWordCursorInfoEx & CursorInfo);
	public:
		void InitCursorInfo(XWordCursorInfoEx &cursorInfo){::ZeroMemory(&cursorInfo,sizeof(XWordCursorInfoEx));}
		void ShowCurrentCaret(bool bShow);
		bool CalcLeftCursorInfo(XWordCursorInfoEx & CursorInfo);
		bool CalcRightCursorInfo(XWordCursorInfoEx & CursorInfo);
		bool CalcUpCursorInfo(XWordCursorInfoEx & CursorInfo);
		bool CalcDownCursorInfo(XWordCursorInfoEx & CursorInfo);
	public:
		XWordDataBase* GetControlBlockAtPoint(XWordDataSvr* pDataSvr,POINT pt);
		class XFlowPara* GetControlParaAtPoint(XWordDataSvr* pDataSvr,POINT pt);
	public:
		XWordSheetSvr * m_pSheetSvr;
		XWordCursorInfoEx m_CursorInfo;
	public:
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
		bool ClampInView(const XWordCursorInfoEx & CursorInfo);
	public:
		std::vector<XWordDataBase *> m_pSelectionBlocks;
		TWordState m_FState;
		TWordDrawState m_FDrawState;
		POINT m_ptStart;
		POINT m_ptCurrent;
	public:
		int FOptions;
	public:
		LPTSTR m_pStrShape;
	public:
		bool DeleteChar(XWordCursorInfoEx & CursorInfo);
		bool BackDeleteChar(XWordCursorInfoEx & CursorInfo);
		bool InsertChar(XWordCursorInfoEx & CursorInfo,TCHAR chr);
	public:
		int GetIndexFromParent(XWordDataBase   * pBlock);
		int GetIndex(XWordDataBase   * pBlock);
		XWordDataTextBlock  * GetPirorTextBlock(XWordDataTextBlock  * pBlock);
		XWordDataTextBlock  * GetNextTextBlock(XWordDataTextBlock  * pBlock);
	public:
		int RemoveBlock(XWordDataBase   * pBlock);
		bool MerginPara(XWordDataContentParagraph * pDestPara,XWordDataContentParagraph * pSrcPara);
	public:
		virtual int AdjustExtent();
	public:
		XWordSelectionEx m_Selection;
		void InitSelection(XWordSelectionEx &WordSelection){ZeroMemory(&WordSelection,sizeof(XWordSelectionEx));}
		bool BlockIsSelected(XWordDataTextBlock * pBlock,XWordDataTextBlock * pStartBlock,XWordDataTextBlock * pEndBlock);
		bool AdjustSelection(XWordCursorInfoEx & CursorInfo,TShiftState state);
		bool CursorInfoInSelection(XWordCursorInfoEx & CursorInfo);
		bool StandOnlySelection(XWordSelectionEx &Selection);
		bool StandOnlySelection(XWordSelectionEx &Selection,XWordDataTextBlock * &pStartBlockEx,XWordDataTextBlock * &pEndBlockEx);
		int GetBlockAndCharPos(int nRow,int nCol,XWordDataTextBlock * &pBlock,int &nCharPos,bool bRight=false);
		bool StandOnlyToPara(int nRow,int nCol);
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
		int DoPaste();
		int DoCopy();
		int DoCut();
	public:
		int m_nView;
		bool ViewConer();
		bool ViewHRuler();
		bool ViewVRuler();
	};
}}}}