#pragma once
#include <xframe.h>
#include "xuxutil.h"
#include "xofbase.h"
#include "xwordlayer.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
		struct XWordCursorInfoEx
		{
			int nRow;
			int nCol;
			int XPosCursur;
			int YPosCursur;
			int CursorHeight;
			int nIndex;
			XWordDataTextBlock * pBlock;
		};

	class XWORD_API XWordSheetSvr:
		public XOfficeSheetSvr
	{
	public:
		XWordSheetSvr();
		~XWordSheetSvr();
	public:
		int m_nCurrentDoc;
	public:
		class XFlowDocument * m_pFlowDocument;
		XWordLayer* m_pWordLayer;
		class XWordHRuler * m_pHRuler;
		class XWordVRuler * m_pVRuler;
		class XWordConer  * m_pConer;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		virtual int SetFRect(RECT rc);
		virtual int ShowSheet(bool bShow);
	public:
		virtual void AdjustSelection(XWordLayer::XWordCursorInfo & CursorInfo);
		virtual void AdjustSelection(XWordCursorInfoEx & CursorInfo);
		
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
		virtual int LoadTemplate(IXMLDOMElement * pElement,int state = goNormalState);
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	private:
		void DrawVBorder(POINT pt);
	private:
		void BlankDoc();
	public:
		void * m_pObj;
		POINT m_ptObj;
	};
}}}}
