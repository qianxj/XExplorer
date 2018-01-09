#pragma once

#include "xframe.h"
#include "xflowchartobject.hpp"
#include "xflowchartlink.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{

#define goPointerSelect 0x0001
#define goLineSelect	0x0002
#define goNodeSelect	0x0004

#define FLC_DOUBLECLICKED		0x0001
#define FLC_OBJECTSELECTED		0x0002
#define FLC_LINKSELECTED		0x0003
#define FLC_OBJECTWILLDELETED	0x0004
#define FLC_LINKWILLDELETED		0x0005
#define FLC_OBJECTADDED			0x0006
#define FLC_LINKADDED			0x0007

	struct 	FLCNMHDR : public NMHDR
	{
		LPTSTR pWorkNode;
		XFlowChartObject * pObj;
		XFlowChartLink * pLink;
	};

	class XFLOWCHART_API XFlowSheetSvr:
		public XOffice::XOfficeSheetSvr 
	{
	public:
		XFlowSheetSvr();
		~XFlowSheetSvr();
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	
	private:
		POINT	m_ptMouse;
		POINT	m_ptHittest;

	public:
		XFOState m_FState;
		XFODrawState m_FDrawState;
	public:
		DWORD	FSelect;
	private:
		LPTSTR m_pStrShape;
		XFlowChartObject * m_pShape;
		POINT	m_ptOld;
		POINT	m_ptPrev;

	public:
		bool EvtPaint(TEvent *pEvent ,LPARAM lParam);
		bool EvtLButtonDown(TEvent *pEvent ,LPARAM lParam);
		bool EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam);
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
		bool EvtSetCursor(TEvent *pEvent ,LPARAM lParam);
		bool EvtNCHitTest(TEvent *pEvent ,LPARAM lParam);
		bool EvtWkChanged(TEvent *pEvent ,LPARAM lParam);
		bool EvtWkIDChanged(TEvent *pEvent ,LPARAM lParam);
	private:
		XFlowChartObject * m_pHitObject;
		XFlowChartLink * m_pHitLink;
		XFOHitPosition	m_nHitPos;
	public:
		XFlowChartObject * GetHitedObject();
		bool SelectedObject(XFlowChartObject * pObject);
	private:
		XFlowChartObject * m_pHotObject;
		XFOHitPosition	m_nHotPos;
		RECT m_hotrc;
	public:
		class XFlowChartLink *  m_pLink;
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	public:
		class CEditSvr*	m_pEditSvr;
		bool FCanEditModify;
		BOOL FEditorMode;

		XFlowChartObject * FInplaceObject;
		bool CanEditShow();
		void HideEditor();
		void HideEdit();
		void UpdateEdit();
		void UpdateEditor();
		void ShowEditor();
	public:
		int Clear();
	public:
		POINT m_TopLeft;
	public:
		int InvalidateRect();
	};
}}}}