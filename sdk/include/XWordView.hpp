#pragma once
#include <xframe.h>
#include "xuxutil.h"
#include "xofbase.h"
#include "XWordSerializeSvr.hpp"
#include "XWordDataSvr.hpp"
#include "XWordBlockMgr.hpp"
#include "XWordPage.hpp"

using namespace Hxsoft::XFrame;
using namespace Hxsoft::XFrame::XOffice;
using namespace Hxsoft::XFrame::XOffice::XWord;

#define  goThumbTracking         0x8000

class XWordLayerDocument;
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
public:
	int FOptions;
public:
	POINT m_ptScroll;
public:
	XWordPage m_Page;
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
	int  PageDown(int ScrollBar);
	int GetTotalHeight();
	int GetMaxScrollExtent(int ScrollBar);
	//bool ClampInView(const XWordCursorInfoEx & CursorInfo);
};
