#pragma once
#include <xframe.h>
#include "xuxutil.h"
#include "xofbase.h"
#include "XWordSerializeSvr.hpp"
#include "XWordDataSvr.hpp"
#include "XWordPage.hpp"
#include "XWordCursor.hpp"
#include "XWordPred.hpp"
#include "wpp4.h"

using namespace Hxsoft::XFrame;
using namespace Hxsoft::XFrame::XOffice;
using namespace Hxsoft::XFrame::XOffice::XWord;

#define TWordState	DWORD
#define TWordState_Normal			0x00000000
#define TWordState_Selecting		0x00000001
#define TWordState_ObjectMoving		0x00000002
#define TWordState_ObjectResizing	0x00000003
#define TWordState_ObjectCreating	0x00000004
#define TWordState_ImageCreating	0x00000005
#define TWordState_ShapeCreating	0x00000006
#define TWordState_ReadOnly			0x00010000

typedef int TWordDrawState;
#define WDSSELECTION		0x0000
#define WDSIMAGESELECTION	0x0001
#define WDSSHAPESELECTION	0x0002
#define WDSOBJECTSELECTION	0x0003

#define  goThumbTracking         0x8000

class XWordLayerDocument;

struct XWSelection
{
	int startRow;
	int startCol;
	int endRow;
	int endCol;
	bool bSelection;
	bool bLast;
};

#define TShiftState int
#define  ssCtrl                 0x0001
#define  ssShift                0x0008
#define  ssAlt                  0x0004

#define	STI_TEXTCOLOR	0x00000200
#define	STI_TEXTBKCOLOR	0x00000400
#define	STI_BKCOLOR		0x00000800

class XWordView : public XOfficeSheetSvr
{
public:
	XWordView(void);
	~XWordView(void);
public:
	POINT FTopLeft;
public:
	int FPageCount;
	int FCurPage;
public:
	int FCurRow;
	int FCurCol;
public:
	int FTopViewRow;
public:
	virtual bool SetEventListons();
	virtual bool UnSetEventListons(); 
public:
	int DrawPageMarker(HDC hPaintDC,RECT &rcPage,RECT &rcPageMargin);
public:
	virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	virtual int SetFRect(RECT rc);
	virtual int ShowSheet(bool bShow);
public:
	//事件处理
	bool EvtPaint(TEvent *pEvent ,LPARAM lParam);
	bool EvtLButtonDown(TEvent *pEvent ,LPARAM lParam);
	bool EvtLButtonUp(TEvent *pEvent ,LPARAM lParam);
	bool EvtMouseMove(TEvent *pEvent ,LPARAM lParam);
	bool EvtMouseWheel(TEvent *pEvent ,LPARAM lParam);
	bool EvtVScroll(TEvent *pEvent ,LPARAM lParam);
	bool EvtHScroll(TEvent *pEvent ,LPARAM lParam);
	bool EvtSize(TEvent *pEvent ,LPARAM lParam);
	bool EvtChar(TEvent *pEvent ,LPARAM lParam);
	bool EvtKeyDown(TEvent *pEvent ,LPARAM lParam);
	bool EvtSetFocus(TEvent *pEvent ,LPARAM lParam);
	bool EvtKillFocus(TEvent *pEvent ,LPARAM lParam);
	bool EvtXCommand(TEvent *pEvent ,LPARAM lParam);
	bool EvtTimer(TEvent *pEvent ,LPARAM lParam);
	bool EvtSetCursor(TEvent *pEvent ,LPARAM lParam);
	bool EvtNCHitTest(TEvent *pEvent ,LPARAM lParam);
private:
	struct HitPosition
	{
		static const int none		= 0x0000;
		static const int lefttop	= 0x0001;
		static const int righttop	= 0x0002;	
		static const int leftbottom	= 0x0004;	
		static const int rightbottom= 0x0008;	
	
		static const int topcenter	 = 0x0010;
		static const int bottomcenter= 0x0020;
		static const int leftcenter	 = 0x0040;
		static const int rightcenter = 0x0080;
	};
	int	GetHitResizePat(POINT pt, RECT& rt, int cx);
	int m_hitPos;
	POINT m_ptHittest;
public:
	int FOptions;
public:
	std::vector<vord_t>* m_pSelectionBlocks;
	vord_t m_pObj;
	POINT	 m_ptObj;
public:
	bool IsSelected(vord_t pBlock);
	RECT GetImageRect(SIZE sz,RECT &rt);
	RECT GetImageBoundRect(SIZE sz,RECT &rt);
public:
	POINT m_ptScroll;
public:
	XWordPage m_Page;
	int m_pages;
public:
	TWordState m_State;
public:
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
	int PageDown(int ScrollBar);
	int GetTotalHeight();
	int GetMaxScrollExtent(int ScrollBar);
	bool ClampInView(const XWCursorInfo & CursorInfo);
public:
	XWCursorInfo m_CursorInfo;
public:
	/*
	int CalcContentPageRect(int nPage,RECT * pDrawRect, RECT &rc);
public:
	XWordBlock * GetObjectAtPoint(POINT pt,XWordParagraphRowInfo::RowBlock * &pRowBlock);
public:
	int PosBlockIndex(int &y,std::vector<XWordBlock * > * pItems);
	int PosRowIndex(int &y,std::vector<XWordParagraphRowInfo> * pItems,int nStartRow=0);
	int PosCharIndex(int x,XWordParagraphRowInfo* pRowInfo,int &nPosChar,int &posX);
	int PosCharIndexEx(int x,XWordParagraphRowInfo* pRowInfo,int &nPosChar,int &posX,int &nCol);

	*/
	bool CalcCursorInfo(XWCursorInfo & CursorInfo,int nRow,int nCol);
	bool CalcCursorInfoEx(XWCursorInfo & CursorInfo,vord_t pBlock,int nPosChar);
	bool CalcCursorInfoAtPoint(POINT pt,XWCursorInfo & CursorInfo);
	void InitCursorInfo(XWCursorInfo &cursorInfo){::ZeroMemory(&cursorInfo,sizeof(XWCursorInfo));}
	void ShowCurrentCaret(bool bShow);

	int GetColCount(int nRow);
	int GetRowCount();
	bool CalcLeftCursorInfo(XWCursorInfo & CursorInfo);
	bool CalcRightCursorInfo(XWCursorInfo & CursorInfo);
	bool CalcUpCursorInfo(XWCursorInfo & CursorInfo);
	bool CalcDownCursorInfo(XWCursorInfo & CursorInfo);

	/*int  GetPageOfBlock(XWordBlock * pBlock,int nPosChar);
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
*/
public:
	XWSelection m_Selection;
	void InitSelection(XWSelection &WordSelection){ZeroMemory(&WordSelection,sizeof(XWSelection));}
	bool AdjustSelection(XWCursorInfo & CursorInfo,TShiftState state);
	bool AdjustSelection(XWCursorInfo & CursorInfo);

/*	bool BlockIsSelected(XWordBlock * pBlock,XWordBlock * pStartBlock,XWordBlock * pEndBlock);
	bool CursorInfoInSelection(XWordCursorInfo & CursorInfo);
	int GetBlockAndCharPos(int nRow,int nCol,XWordBlock * &pBlock,int &nCharPos,bool bRight=false);
*/	bool StandOnlyToPara(int nRow,int nCol);
	bool StandOnlySelection(XWSelection &Selection);
	bool StandOnlySelection(XWSelection &Selection,vord_t & first,vord_t & last);
/*	bool MerginPara(XWordParagraphBlock * pDestPara,XWordParagraphBlock * pSrcPara);
*/
public:
	void CalcBlockExtent(vord_t pBlock);
public:
	bool DeleteChar(XWCursorInfo & CursorInfo);
	bool DeleteSelection(XWCursorInfo & CursorInfo);
	bool BackDeleteChar(XWCursorInfo & CursorInfo);
	bool InsertChar(XWCursorInfo & CursorInfo,TCHAR ch);

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
	int GetPageAtRow(int row);
	int GetPageRect(int nPage, RECT &rt);
	int GetPageAtPoint(POINT pt);
	int GetRowAtPoint(POINT pt);
public:
	//int GetRowColAtPoint(POINT pt);
	//int GetPointAtRowCol(int nRow,int nCol);
public:
	int CalcPages();
	bool DoDrawPage(HDC hPaintDC,int nPage,POINT ptOffset,RECT * pDrawRect,RECT * pUpdateRect);
	bool DrawPage(HDC hPaintDC,int nPage,POINT ptOffset,RECT * contentRect,RECT * pDrawRect,RECT * pUpdateRect);
	int	 DrawParaControl(HDC hPaintDC,vord_t item,POINT ptOffset,RECT * contentRect, RECT * pDrawRect,RECT * pUpdateRect);
	bool GetFirstRowOfPage(int nPage,int & nRow,int &nParaRow, vord_t & pParaBlock);
	/*int SetFRect(RECT &rc);*/
public:
	int DoPaste();
	int DoCopy();
	int DoCut();
public:
	vord_t GetParaAtRow(int & nRow);
	vord_t GetFirstBlockAtRow(int nRow);
	vord_t GetBlockAtRowColumn(int nRow,int nCol);
	vord_t GetBlockAtRowColumn(int nRow,int nCol, int &index);
public:
	bool GetRowColumnByBlock(vord_t block,int charPos,  int & nRow,int & nCol);
public:
	bool IsControl(vord_t & item,wchar_t * typ);
public:
	void SetReadOnly(bool bReadOnly);
	bool GetReadOnly();
};
