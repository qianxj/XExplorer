#include "stdafx.h"
#include "xwordsheetsvr.hpp"
#include "XWordSerializeSvrEx.hpp"
#include "xwordhruler.hpp"
#include "xwordvruler.hpp"
#include "xwordconer.hpp"
#include "XFlowDocument.hpp"
#include "XFlowPara.hpp"
#include "XFlowChapter.hpp"
#include "xcControl.h"
#include "xcell.h"

#define ErrorHandler(Error) 
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{

	XWordSheetSvr::XWordSheetSvr():m_nCurrentDoc(-1),m_pWordLayer(NULL),m_pFlowDocument(NULL),
			m_pHRuler(NULL),m_pVRuler(NULL),m_pConer(NULL),m_pObj(NULL)
	{
		::SetRect(&FRect,0,0,0,0);
		m_pSerializeSvr = new XWordSerializeSvrEx;
		m_pDataSvr = new XWordDataSvr;


#ifdef V1
		m_pWordLayer = new XWordLayer;
		m_pWordLayer->m_pSheetSvr = this;
		m_pHRuler = new XWordHRuler;
		m_pVRuler = new XWordVRuler;
		m_pConer = new XWordConer;

		m_pHRuler->m_pLayer = m_pWordLayer;
		m_pHRuler->m_pSheetSvr = this;
		m_pVRuler->m_pLayer = m_pWordLayer;
		m_pVRuler->m_pSheetSvr = this;
#else
		m_pFlowDocument = new XFlowDocument;
		m_pFlowDocument->m_pSheetSvr = this;

		m_pHRuler = new XWordHRuler;
		m_pVRuler = new XWordVRuler;
		m_pConer  = new XWordConer;

		m_pHRuler->m_pLayer = NULL;
		m_pHRuler->m_pSheetSvr = this;
		m_pVRuler->m_pLayer = NULL;
		m_pVRuler->m_pSheetSvr = this;
		m_pHRuler->m_pFlowDocument = m_pFlowDocument;
		m_pVRuler->m_pFlowDocument = m_pFlowDocument;
		
		//BlankDoc();
		//m_pFlowDocument->m_pContent = 	((XWordDataSvr *)m_pDataSvr)->m_pContentSvr->m_pXWordDataDocument;
#endif


		SetEventListons();
	}

	XWordSheetSvr::~XWordSheetSvr()
	{
		if(m_pWordLayer) delete m_pWordLayer;
		if(m_pHRuler) delete m_pHRuler;
		if(m_pVRuler) delete m_pVRuler;
		if(m_pConer) delete m_pConer;

		if(m_pFlowDocument)delete m_pFlowDocument;

		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
		
		UnSetEventListons();
	}

	bool XWordSheetSvr::SetEventListons()
	{
#ifdef V1
		//~~this->AttachListonTo(pDest,WM_PAINT,(FNEvent)&XWordSheetSvr::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XWordSheetSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XWordSheetSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XWordSheetSvr::EvtMouseMove);
		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XWordSheetSvr::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&XWordSheetSvr::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&XWordSheetSvr::EvtHScroll);
		this->AttachListon(WM_SIZE,(FNEvent)&XWordSheetSvr::EvtSize);
		this->AttachListon(WM_CHAR,(FNEvent)&XWordSheetSvr::EvtChar);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XWordSheetSvr::EvtKeyDown);
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XWordSheetSvr::EvtSetFocus);
		this->AttachListon(WM_KILLFOCUS,(FNEvent)&XWordSheetSvr::EvtKillFocus);
		this->AttachListon(WM_XCOMMAND,(FNEvent)&XWordSheetSvr::EvtXCommand);
#else
		//~~this->AttachListonTo(pDest,WM_PAINT,(FNEvent)&XWordSheetSvr::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XWordSheetSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XWordSheetSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XWordSheetSvr::EvtMouseMove);
		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XWordSheetSvr::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&XWordSheetSvr::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&XWordSheetSvr::EvtHScroll);
		this->AttachListon(WM_SIZE,(FNEvent)&XWordSheetSvr::EvtSize);
		this->AttachListon(WM_CHAR,(FNEvent)&XWordSheetSvr::EvtChar);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XWordSheetSvr::EvtKeyDown);
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XWordSheetSvr::EvtSetFocus);
		this->AttachListon(WM_KILLFOCUS,(FNEvent)&XWordSheetSvr::EvtKillFocus);
		this->AttachListon(WM_XCOMMAND,(FNEvent)&XWordSheetSvr::EvtXCommand);
		this->AttachListon(WM_TIMER,(FNEvent)&XWordSheetSvr::EvtTimer);
#endif
		return true;
	}

	bool XWordSheetSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	void XWordSheetSvr::AdjustSelection(XWordCursorInfoEx & CursorInfo)
	{
		TShiftState Shift = 0;
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
		if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;
		m_pFlowDocument->AdjustSelection(CursorInfo,Shift);
	}

	void XWordSheetSvr::AdjustSelection(XWordLayer::XWordCursorInfo & CursorInfo)
	{
		TShiftState Shift = 0;
		if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
		if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
		if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;
		m_pWordLayer->AdjustSelection(CursorInfo,Shift);
	}

	bool XWordSheetSvr::EvtPaint(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}

	bool XWordSheetSvr::EvtTimer(TEvent *pEvent ,LPARAM lParam)
	{
		if(m_pObj)
		{
			::KillTimer(GetHWND(),91);
			//XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			//pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			//pCellSvr->m_pGridFrameSvrs->EvtTimer(pEvent,lParam);
			return true;
		}
		return true;
	}

	bool XWordSheetSvr::EvtXCommand(TEvent *pEvent ,LPARAM lParam)
	{
		LPTSTR	pStrID = pEvent->xcommand.pStrID;
#ifdef V1
		if(pStrID && _tcscmp(pStrID,_T("xmEditPaste"))==0)
		{
			m_pWordLayer->DoPaste();
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmEditCopy"))==0)
		{
			m_pWordLayer->DoCopy();
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmEditCut"))==0)
		{
			m_pWordLayer->DoCut();
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatFont"))==0)
		{
			m_pWordLayer->ChooseFontDlg();
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatItalic"))==0)
		{
			m_pWordLayer->SetFontItalic();
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatUnderline"))==0)
		{
			m_pWordLayer->SetFontUnderline();
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatBold"))==0)
		{
			m_pWordLayer->SetFontBold();
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatTextColor"))==0)
		{
			m_pWordLayer->ChooseColorDlg(STI_TEXTCOLOR);
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatTextBkColor"))==0)
		{
			m_pWordLayer->ChooseColorDlg(STI_BKCOLOR);
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignCenter"))==0)
		{
			m_pWordLayer->SetAlign(DT_CENTER);
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignRight"))==0)
		{
			m_pWordLayer->SetAlign(DT_RIGHT);
			pEvent->dwRet = 1;
		}
		if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignLeft"))==0)
		{
			m_pWordLayer->SetAlign(DT_LEFT);
			pEvent->dwRet = 1;
		}
#else
		if(m_pObj)
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			pCellSvr->m_pGridFrameSvrs->EvtXCommand(pEvent,lParam);
			return true;
		}

		if(pStrID && _tcscmp(pStrID,_T("xmEditPaste"))==0)
		{
			m_pFlowDocument->DoPaste();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmEditCopy"))==0)
		{
			m_pFlowDocument->DoCopy();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmEditCut"))==0)
		{
			m_pFlowDocument->DoCut();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatFont"))==0)
		{
			m_pFlowDocument->ChooseFontDlg();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatItalic"))==0)
		{
			m_pFlowDocument->SetFontItalic();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatUnderline"))==0)
		{
			m_pFlowDocument->SetFontUnderline();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatBold"))==0)
		{
			m_pFlowDocument->SetFontBold();
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatTextColor"))==0)
		{
			m_pFlowDocument->ChooseColorDlg(STI_TEXTCOLOR);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatTextBkColor"))==0)
		{
			m_pFlowDocument->ChooseColorDlg(STI_BKCOLOR);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignCenter"))==0)
		{
			m_pFlowDocument->SetAlign(DT_CENTER);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignRight"))==0)
		{
			m_pFlowDocument->SetAlign(DT_RIGHT);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmFormatAlignLeft"))==0)
		{
			m_pFlowDocument->SetAlign(DT_LEFT);
			pEvent->dwRet = 1;
		}
		else if(pStrID && _tcscmp(pStrID,_T("xmShapeImage"))==0)
		{
			this->m_pFlowDocument->m_FDrawState=WDSIMAGESELECTION;
			pEvent->dwRet = 1;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = true;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
		}else if(pStrID && _tcscmp(pStrID,_T("xmShapeObject"))==0)
		{
			pEvent->dwRet = 1;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = true;
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
		}else if(pStrID && _tcscmp(pStrID,_T("xmPointerSelect"))==0)
		{
			this->m_pFlowDocument->m_FDrawState=WDSSELECTION;
			pEvent->dwRet = 1;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = true;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
			((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
		}
		else if(_tcsstr(pEvent->xcommand.pStrID,_T(":")))
		{
			this->m_pFlowDocument->m_FDrawState=WDSSHAPESELECTION;
			if(this->m_pFlowDocument->m_pStrShape)delete this->m_pFlowDocument->m_pStrShape;
			this->m_pFlowDocument->m_pStrShape = _tcsdup(pEvent->xcommand.pStrID);
			pEvent->dwRet = 1;
		}
#endif
		return true;
	}

	bool XWordSheetSvr::EvtSetFocus(TEvent *pEvent ,LPARAM lParam)
	{
#ifdef V1
			if(m_pWordLayer)
			{
				if(m_pWordLayer->m_CursorInfo.nRow < 1)
					m_pWordLayer->CalcCursorInfo(m_pWordLayer->m_CursorInfo,1,1);
				m_pWordLayer->ShowCurrentCaret(true);
			}
#else
		if(m_pObj)
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			pCellSvr->m_pGridFrameSvrs->EvtSetFocus(pEvent,lParam);
			return true;
		}

		if(this->m_pFlowDocument)
		{
			if(this->m_pFlowDocument->m_FDrawState & WDSSELECTION)
			{
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = true;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
			}
			else if(this->m_pFlowDocument->m_FDrawState & WDSIMAGESELECTION)
			{
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = true;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = false;
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
			}
			else if(this->m_pFlowDocument->m_FDrawState & WDSSHAPESELECTION)
			{
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_bChecked = false;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_bChecked = true;
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeImage")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmShapeObject")))->m_hWnd,NULL,true);
			}
		}
#endif
		return true;
	}

	int XWordSheetSvr::ShowSheet(bool bShow)
	{
		if(!bShow)
			DestroyCaret();
		else
			EvtSetFocus(NULL,NULL);
		return 1;
	}

	bool XWordSheetSvr::EvtKillFocus(TEvent *pEvent ,LPARAM lParam)
	{
#ifdef V1
		if((HWND)pEvent->wParam != pEvent->hWnd)DestroyCaret();
#else
		if((HWND)pEvent->wParam != pEvent->hWnd)DestroyCaret();
#endif
		return true;
	}

	bool XWordSheetSvr::EvtVScroll(TEvent *pEvent ,LPARAM lParam)
	{
#ifdef V1
		if(m_pWordLayer) m_pWordLayer->WMVScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
#else
		if(m_pFlowDocument) m_pFlowDocument->WMVScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
#endif
		return true;
	}

	bool XWordSheetSvr::EvtHScroll(TEvent *pEvent ,LPARAM lParam)
	{
#ifdef V1
		if(m_pWordLayer) m_pWordLayer->WMHScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
#else
		if(m_pFlowDocument) m_pFlowDocument->WMHScroll(pEvent->scroll.nSBCode,pEvent->scroll.nPos,NULL);
#endif
		return true;
	}

	bool XWordSheetSvr::EvtMouseWheel(TEvent *pEvent ,LPARAM lParam)
	{
#ifdef V1
		if(m_pWordLayer) m_pWordLayer->WMWheel(pEvent->wheel.point,pEvent->wheel.fwKeys,pEvent->wheel.zDelta);
#else
		if(m_pObj)
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			pEvent->mouse.point.x -= m_ptObj.x;
			pEvent->mouse.point.y -= m_ptObj.y;
			pCellSvr->m_pGridFrameSvrs->EvtMouseWheel(pEvent,lParam);
			return true;
		}
		if(m_pFlowDocument) m_pFlowDocument->WMWheel(pEvent->wheel.point,pEvent->wheel.fwKeys,pEvent->wheel.zDelta);
#endif
		return true;
	}

	bool XWordSheetSvr::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
#ifdef V1
		switch (pEvent->key.nChar) 
		{
		case 'V': //paste
			if(m_pWordLayer)
			{
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				{
					m_pWordLayer->DoPaste();
				}
			}
			break;
		case 'C': //paste
			if(m_pWordLayer)
			{
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				{
					m_pWordLayer->DoCopy();
				}
			}
			break;
		case 'X': //paste
			if(m_pWordLayer)
			{
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				{
					m_pWordLayer->DoCut();
				}
			}
			break;
		case VK_HOME:       // Home 
			if(m_pWordLayer)
			{
				m_pWordLayer->CalcCursorInfo(m_pWordLayer->m_CursorInfo,0,0);
				m_pWordLayer->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,0);
			}
			break; 
		case VK_END:        // End 
			if(m_pWordLayer)
			{
				m_pWordLayer->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,100);
			}
			break; 

		case VK_PRIOR:      // Page Up 
			if(m_pWordLayer)
			{
				m_pWordLayer->ModifyScrollBar(SB_VERT,SB_PAGEUP,0);
			}
			break; 

		case VK_NEXT:       // Page Down 
			if(m_pWordLayer)
			{
				m_pWordLayer->ModifyScrollBar(SB_VERT,SB_PAGEDOWN,0);
			}
			break; 
		case VK_LEFT:       // Left arrow
			{
				if(m_pWordLayer)
				{
					XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
					if(m_pWordLayer->CalcLeftCursorInfo(CursorInfo))
					{
						m_pWordLayer->m_CursorInfo = CursorInfo;
						m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
						AdjustSelection(m_pWordLayer->m_CursorInfo);
					}
				}
			}
			break; 
		case VK_RIGHT:      // Right arrow 
			{
				if(m_pWordLayer)
				{
					XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
					if(m_pWordLayer->CalcRightCursorInfo(CursorInfo))
					{
						m_pWordLayer->m_CursorInfo = CursorInfo;
						m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
						AdjustSelection(m_pWordLayer->m_CursorInfo);
					}
				}
			}
			break; 

		case VK_UP:         // Up arrow 
			{
				if(m_pWordLayer)
				{
					XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
					if(m_pWordLayer->CalcUpCursorInfo(CursorInfo))
					{
						m_pWordLayer->m_CursorInfo = CursorInfo;
						m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
						AdjustSelection(m_pWordLayer->m_CursorInfo);
					}
				}
			}
			break; 

		case VK_DOWN:       // Down arrow 
			{
				if(m_pWordLayer)
				{
					XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
					if(m_pWordLayer->CalcDownCursorInfo(CursorInfo))
					{
						m_pWordLayer->m_CursorInfo = CursorInfo;
						m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
						AdjustSelection(m_pWordLayer->m_CursorInfo);
					}
				}
			}
			break; 

		case VK_DELETE:     // Delete
			m_pWordLayer->DeleteChar(m_pWordLayer->m_CursorInfo);
			m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
			break; 
		}
#else
		if(m_pObj)
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			pCellSvr->m_pGridFrameSvrs->EvtKeyDown(pEvent,lParam);
			return true;
		}
		switch (pEvent->key.nChar) 
		{
		case 'V': //paste
			if(m_pFlowDocument)
			{
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				{
					m_pFlowDocument->DoPaste();
				}
			}
			break;
		case 'C': //paste
			if(m_pFlowDocument)
			{
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				{
					m_pFlowDocument->DoCopy();
				}
			}
			break;
		case 'X': //paste
			if(m_pFlowDocument)
			{
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				{
					m_pFlowDocument->DoCut();
				}
			}
			break;
		case VK_HOME:       // Home 
			if(this->m_pFlowDocument)
			{
				//m_pWordLayer->CalcCursorInfo(m_pWordLayer->m_CursorInfo,0,0);
				this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,0);
			}
			break; 
		case VK_END:        // End 
			if(this->m_pFlowDocument)
			{
				this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_THUMBPOSITION,100);
			}
			break; 

		case VK_PRIOR:      // Page Up 
			if(this->m_pFlowDocument)
			{
				this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_PAGEUP,0);
			}
			break; 

		case VK_NEXT:       // Page Down 
			if(this->m_pFlowDocument)
			{
				this->m_pFlowDocument->ModifyScrollBar(SB_VERT,SB_PAGEDOWN,0);
			}
			break; 
		case VK_LEFT:       // Left arrow
			{
				if(m_pFlowDocument)
				{
					if(m_pFlowDocument->CalcLeftCursorInfo(m_pFlowDocument->m_CursorInfo))
					{
						m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
						AdjustSelection(m_pFlowDocument->m_CursorInfo);
						m_pFlowDocument->ShowCurrentCaret(true);
					}
				}
			}
			break; 
		case VK_RIGHT:      // Right arrow 
			{
				if(m_pFlowDocument)
				{
					if(m_pFlowDocument->CalcRightCursorInfo(m_pFlowDocument->m_CursorInfo))
					{
						m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
						AdjustSelection(m_pFlowDocument->m_CursorInfo);
						m_pFlowDocument->ShowCurrentCaret(true);
					}
				}
			}
			break; 

		case VK_UP:         // Up arrow 
			{
				/*
				if(m_pWordLayer)
				{
					XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
					if(m_pWordLayer->CalcUpCursorInfo(CursorInfo))
					{
						m_pWordLayer->m_CursorInfo = CursorInfo;
						m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
						AdjustSelection(m_pWordLayer->m_CursorInfo);
					}
				}*/
			}
			break; 

		case VK_DOWN:       // Down arrow 
			{
				/*
				if(m_pWordLayer)
				{
					XWordLayer::XWordCursorInfo CursorInfo = m_pWordLayer->m_CursorInfo;
					if(m_pWordLayer->CalcDownCursorInfo(CursorInfo))
					{
						m_pWordLayer->m_CursorInfo = CursorInfo;
						m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
						AdjustSelection(m_pWordLayer->m_CursorInfo);
					}
				}*/
			}
			break; 

		case VK_DELETE:     // Delete
			if(m_pFlowDocument)
			{
				m_pFlowDocument->DeleteChar(m_pFlowDocument->m_CursorInfo);
				m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
			}
			
			break; 
		} 
#endif
		return true;
	}

	bool XWordSheetSvr::EvtLButtonDown(TEvent *pEvent ,LPARAM lParam)
	{
#ifdef V1
		POINT point = pEvent->mouse.point;
		if(::GetFocus() != m_pSheetCtrl->m_hWnd)::SetFocus(m_pSheetCtrl->m_hWnd);
		XWordLayer::XWordCursorInfo curInfo;
		if(!PtInRect(&FRect,point)) return true;
		if(m_pWordLayer->CalcCursorInfoAtPoint(point,curInfo))
		{
			AdjustSelection(curInfo);
			m_pWordLayer->m_CursorInfo = curInfo;
			m_pWordLayer->ShowCurrentCaret(true);
		}
		m_pWordLayer->m_State = TWordState_Selecting;
#else
		m_pObj = NULL;

		POINT point = pEvent->mouse.point;
		point.x += m_pFlowDocument->m_ptScroll.x;
		point.y += m_pFlowDocument->m_ptScroll.y;
		if(::GetFocus() != m_pSheetCtrl->m_hWnd)::SetFocus(m_pSheetCtrl->m_hWnd);
		
		if(m_pFlowDocument && (m_pFlowDocument->m_FDrawState == WDSIMAGESELECTION||
			m_pFlowDocument->m_FDrawState == WDSSHAPESELECTION || 
			m_pFlowDocument->m_FDrawState == WDSOBJECTSELECTION))
		{
			m_pFlowDocument->m_ptStart = point;
			m_pFlowDocument->m_ptCurrent = point;
			DrawVBorder(point);
			if(m_pFlowDocument->m_FDrawState == WDSIMAGESELECTION)
				m_pFlowDocument->m_FState = TWordState_ImageCreating;
			else if(m_pFlowDocument->m_FDrawState == WDSSHAPESELECTION )
				m_pFlowDocument->m_FState = TWordState_ShapeCreating;
			else 
				m_pFlowDocument->m_FState = TWordState_ObjectCreating;
		}
		else
		{
			XWordDataSvr * pDataSvr = (XWordDataSvr *)m_pDataSvr;
			XWordCursorInfoEx CursorInfo;
			if(m_pFlowDocument->CalcCursorInfoAtPoint(pDataSvr,point,CursorInfo))
			{
				m_pFlowDocument->m_CursorInfo = CursorInfo;
				m_pFlowDocument->ShowCurrentCaret(true);
				m_pFlowDocument->m_pSelectionBlocks.clear();

				for(int i=0;i<(int)m_pFlowDocument->m_pSelectionBlocks.size();i++)
				{
					if(m_pFlowDocument->m_pSelectionBlocks[i]->eWordDataType==_XWordDataSpanControlBlock)
					{
						XWordDataSpanControlBlock* pSpanControl = (XWordDataSpanControlBlock*)m_pFlowDocument->m_pSelectionBlocks[i];
						pSpanControl->bSelected = false;
					}
				}
				m_pFlowDocument->m_pSelectionBlocks.clear();
				AdjustSelection(CursorInfo);
				::InvalidateRect(GetHWND(),NULL,false);
				
				m_pFlowDocument->m_FState = TWordState_Selecting;
			}else
			{
				XWordDataBase* pBlock = m_pFlowDocument->GetControlBlockAtPoint(pDataSvr,point);
				if(pBlock)
				{
					if(pBlock->eWordDataType==_XWordDataControlParagraph)
					{
						XWordDataControlParagraph* pCtlPara = (XWordDataControlParagraph*)pBlock;
						if(_tcsicmp(pCtlPara->m_pStrControlClass,L"xcell")==0)
						{
							XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)pCtlPara->m_pObj;
							pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
							m_pObj = pCellSvr;
							XFlowPara * pPara = m_pFlowDocument->GetControlParaAtPoint(pDataSvr,point);
							m_ptObj.x = pPara->FRect.left;
							m_ptObj.y = pPara->FRect.top;

							RECT rt;
							::SetRect(&rt,pPara->FRect.left,pPara->FRect.top,
									pPara->FRect.left + pCtlPara->width,
									pPara->FRect.top + pCtlPara->height);
							pEvent->mouse.point.x -= m_ptObj.x;
							pEvent->mouse.point.y -= m_ptObj.y;
							pCellSvr->m_pGridFrameSvrs->EvtLButtonDown(pEvent,lParam);
						}
					}else
					{
						if(!(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE))
						{
							for(int i=0;i<(int)m_pFlowDocument->m_pSelectionBlocks.size();i++)
							{
								if(m_pFlowDocument->m_pSelectionBlocks[i]->eWordDataType==_XWordDataSpanControlBlock)
								{
									XWordDataSpanControlBlock* pSpanControl = (XWordDataSpanControlBlock*)m_pFlowDocument->m_pSelectionBlocks[i];
									pSpanControl->bSelected = false;
								}
							}
							m_pFlowDocument->m_pSelectionBlocks.clear();
						}
						bool bFound = false;
						for(int i=0;i<(int)m_pFlowDocument->m_pSelectionBlocks.size();i++)
						{
							if(m_pFlowDocument->m_pSelectionBlocks[i]==pBlock)
							{
								bFound = true;
								break;
							}
						}
						if(!bFound)
						{
							if(pBlock->eWordDataType==_XWordDataSpanControlBlock)
							{
								XWordDataSpanControlBlock* pSpanControl = (XWordDataSpanControlBlock*)pBlock;
								pSpanControl->bSelected = true;
								m_pFlowDocument->m_pSelectionBlocks.push_back(pSpanControl);
							}
						}
						::InvalidateRect(GetHWND(),NULL,false);
						m_pFlowDocument->m_FState = TWordState_ObjectMoving;
						m_pFlowDocument->m_ptStart = point;
					}
				}
			}
		}
#endif
		return true;
	}

	bool XWordSheetSvr::EvtLButtonUp(TEvent *pEvent ,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
#ifdef V1
		if(m_pWordLayer)
			m_pWordLayer->m_State = TWordState_Normal;
#else
		if(m_pObj)
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			pEvent->mouse.point.x -= m_ptObj.x;
			pEvent->mouse.point.y -= m_ptObj.y;
			pCellSvr->m_pGridFrameSvrs->EvtLButtonUp(pEvent,lParam);
			return true;
		}

		point.x += m_pFlowDocument->m_ptScroll.x;
		point.y += m_pFlowDocument->m_ptScroll.y;
		if(m_pFlowDocument && (m_pFlowDocument->m_FState == TWordState_ImageCreating||
			m_pFlowDocument->m_FState == TWordState_ShapeCreating|| 
			m_pFlowDocument->m_FState == TWordState_ObjectCreating))
		{
			DrawVBorder(m_pFlowDocument->m_ptCurrent);
			m_pFlowDocument->m_ptCurrent = point;
			
			if( abs( m_pFlowDocument->m_ptCurrent.x -  m_pFlowDocument->m_ptStart.x)>8 ||
					abs( m_pFlowDocument->m_ptCurrent.y -  m_pFlowDocument->m_ptStart.y)>8 )
			{
				if(m_pFlowDocument->m_FState == TWordState_ImageCreating)
				{
					XFlowChapter * pChapter = m_pFlowDocument->GetChapterAtPointer((XWord::XWordDataSvr*)this->m_pDataSvr,m_pFlowDocument->m_ptStart);
					if(pChapter)
					{
						XFlowPara  * pPara =  pChapter->GetParaAtPointer((XWord::XWordDataSvr*)this->m_pDataSvr,m_pFlowDocument->m_ptStart);
						if(pPara)
						{
							XWordDataSpanControlBlock* pDataBlock = new XWordDataSpanControlBlock;
							pDataBlock->m_pStrControlClass = _tcsdup(L"ximage");
							pDataBlock->param = NULL;

							pDataBlock->x = m_pFlowDocument->m_ptStart.x - pPara->FRect.left ;
							pDataBlock->y = m_pFlowDocument->m_ptStart.y - pPara->FRect.top   ;
							pDataBlock->width  = m_pFlowDocument->m_ptCurrent.x - m_pFlowDocument->m_ptStart.x ;
							pDataBlock->height = m_pFlowDocument->m_ptCurrent.y - m_pFlowDocument->m_ptStart.y ;
							pDataBlock->m_pDataParent = pPara->m_pContent;
							pPara->m_pContent->m_pChilds->push_back(pDataBlock);
						}
						if(pPara) delete pPara;
						if(pChapter) delete pChapter;
						::InvalidateRect(GetHWND(),NULL,false);
					}
				}
				else if(m_pFlowDocument->m_FState == TWordState_ShapeCreating)
				{
					XFlowChapter * pChapter = m_pFlowDocument->GetChapterAtPointer((XWord::XWordDataSvr*)this->m_pDataSvr,m_pFlowDocument->m_ptStart);
					if(pChapter)
					{
						XFlowPara  * pPara =  pChapter->GetParaAtPointer((XWord::XWordDataSvr*)this->m_pDataSvr,m_pFlowDocument->m_ptStart);
						if(pPara )
						{
							XWordDataSpanControlBlock* pDataBlock = new XWordDataSpanControlBlock;
							pDataBlock->m_pStrControlClass = NULL;
							pDataBlock->m_pStrShape = _tcsdup(m_pFlowDocument->m_pStrShape);
							pDataBlock->param = NULL;
							pDataBlock->m_bShape = true;
							pDataBlock->x = m_pFlowDocument->m_ptStart.x - pPara->FRect.left  ;
							pDataBlock->y = m_pFlowDocument->m_ptStart.y - pPara->FRect.top  ;
							pDataBlock->width  = m_pFlowDocument->m_ptCurrent.x - m_pFlowDocument->m_ptStart.x ;
							pDataBlock->height = m_pFlowDocument->m_ptCurrent.y - m_pFlowDocument->m_ptStart.y ;
							pDataBlock->m_pDataParent = pPara->m_pContent;
							pPara->m_pContent->m_pChilds->push_back(pDataBlock);
						}
						if(pPara) delete pPara;
						if(pChapter) delete pChapter;
						::InvalidateRect(GetHWND(),NULL,false);
					}
				}
			}
			m_pFlowDocument->m_FState = TWordState_Normal;

		}else
			m_pFlowDocument->m_FState = TWordState_Normal;
#endif
		return true;
	}
	bool XWordSheetSvr::EvtMouseMove(TEvent *pEvent ,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
#ifdef V1
		if(m_pWordLayer->m_State == TWordState_Selecting)
		{
			XWordLayer::XWordCursorInfo curInfo;
			if(m_pWordLayer->CalcCursorInfoAtPoint(point,curInfo))
			{			
				m_pWordLayer->AdjustSelection(curInfo,ssShift);
				m_pWordLayer->m_CursorInfo = curInfo;
				m_pWordLayer->ShowCurrentCaret(true);
			}
		}
#else
		point.x += m_pFlowDocument->m_ptScroll.x;
		point.y += m_pFlowDocument->m_ptScroll.y;
		if(m_pObj)
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			pEvent->mouse.point.x -= m_ptObj.x;
			pEvent->mouse.point.y -= m_ptObj.y;
			pCellSvr->m_pGridFrameSvrs->EvtMouseMove(pEvent,lParam);
			return true;
		}
		if(m_pFlowDocument->m_FState == TWordState_Selecting)
		{
			XWordCursorInfoEx curInfo;
			if(m_pFlowDocument->CalcCursorInfoAtPoint((XWordDataSvr*)m_pDataSvr,point,curInfo))
			{			
				m_pFlowDocument->AdjustSelection(curInfo,ssShift);
				m_pFlowDocument->m_CursorInfo = curInfo;
				m_pFlowDocument->ShowCurrentCaret(true);
			}
		}
		else if(m_pFlowDocument && (m_pFlowDocument->m_FState == TWordState_ImageCreating||
			m_pFlowDocument->m_FState == TWordState_ShapeCreating|| 
			m_pFlowDocument->m_FState == TWordState_ObjectCreating))
		{
			DrawVBorder(m_pFlowDocument->m_ptCurrent);
			DrawVBorder(point);
			m_pFlowDocument->m_ptCurrent = point;
		}
		else if(m_pFlowDocument->m_FState == TWordState_ObjectMoving)
		{
			m_pFlowDocument->m_ptCurrent = point;
			for(int i=0;i<(int)m_pFlowDocument->m_pSelectionBlocks.size();i++)
			{
				if(m_pFlowDocument->m_pSelectionBlocks[i]->eWordDataType==_XWordDataSpanControlBlock)
				{
					XWordDataSpanControlBlock* pSpanControl = (XWordDataSpanControlBlock*)m_pFlowDocument->m_pSelectionBlocks[i];
					pSpanControl->x += m_pFlowDocument->m_ptCurrent.x - m_pFlowDocument->m_ptStart.x;
					pSpanControl->y += m_pFlowDocument->m_ptCurrent.y - m_pFlowDocument->m_ptStart.y;
				}
			}
			 m_pFlowDocument->m_ptStart = m_pFlowDocument->m_ptCurrent;
			::InvalidateRect(GetHWND(),NULL,false);
		}
#endif
		return true;
	}
	
	int XWordSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		int nSaveDC = ::SaveDC(hPaintDC);

#ifdef V1
		if(m_pWordLayer)
		{
			RECT rc;
			if(pDrawRect)
			{
				SetViewportOrgEx(hPaintDC,pDrawRect->left + 1,pDrawRect->top + 1,NULL);
				::IntersectClipRect(hPaintDC,1,1,
					pDrawRect->right - pDrawRect->left - 1 ,
					pDrawRect->bottom - pDrawRect->top -1);
				SetMetaRgn(hPaintDC);
				rc = *pDrawRect;
				::SetRect(&rc,1,1,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
			}
			else
				rc = FRect;

			if(m_pHRuler)
				m_pHRuler->DoDraw(hPaintDC);

			if(m_pVRuler)
				m_pVRuler->DoDraw(hPaintDC);

			if(m_pHRuler && m_pVRuler)
				m_pConer->DoDraw(hPaintDC);

			if(m_pWordLayer)
				m_pWordLayer->DoDraw(hPaintDC);
		}
#else
		RECT rc;
		if(pDrawRect)
		{
			SetViewportOrgEx(hPaintDC,pDrawRect->left + 1,pDrawRect->top + 1,NULL);
			::IntersectClipRect(hPaintDC,1,1,
				pDrawRect->right - pDrawRect->left - 1 ,
				pDrawRect->bottom - pDrawRect->top -1);
			SetMetaRgn(hPaintDC);
			rc = *pDrawRect;
			::SetRect(&rc,1,1,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
		}
		else
		{
			//::GetClientRect(GetHWND(),&rc);
			rc = m_pFlowDocument->FRect;
		}

		XWordDataSvr * pDataSvr = (XWordDataSvr *)m_pDataSvr;
		m_pFlowDocument->DoDraw(pDataSvr,NULL,hPaintDC,&rc,&rc);

		if(m_pHRuler && m_pFlowDocument->ViewHRuler())
			m_pHRuler->DoDraw(hPaintDC);

		if(m_pVRuler && m_pFlowDocument->ViewVRuler())
			m_pVRuler->DoDraw(hPaintDC);

		if(m_pConer && m_pFlowDocument->ViewConer())
			m_pConer->DoDraw(hPaintDC);

#endif		
		::RestoreDC(hPaintDC,nSaveDC);
		return 0;
	}

	int XWordSheetSvr::SetFRect(RECT rc)
	{
		XOfficeSheetSvr::SetFRect(rc);
		
		RECT rect;
		if(m_pHRuler)
		{
			rect = rc;
			InflateRect(&rect,-1,-1);
			if(m_pVRuler) rect.left += m_pVRuler->m_nWidth;
			rect.bottom = rect.top + m_pHRuler->m_nHeight;
			m_pHRuler->SetFRect(rect);
		}
		if(m_pVRuler)
		{
			rect = rc;
			InflateRect(&rect,-1,-1);
			if(m_pVRuler) rect.top += m_pHRuler->m_nHeight;
			rect.right  = rect.left + m_pVRuler->m_nWidth;
			m_pVRuler->SetFRect(rect);
		}
		if(m_pVRuler && m_pHRuler)
		{
			rect = rc;
			InflateRect(&rect,-1,-1);
			rect.bottom = rect.top + m_pHRuler->m_nHeight;
			rect.right  = rect.left + m_pVRuler->m_nWidth;
			m_pConer->SetFRect(rect);
		}

		if(m_pWordLayer)
		{
			rect = rc;
			InflateRect(&rect,-1,-1);
			if(m_pVRuler) rect.top += m_pHRuler->m_nHeight ;
			if(m_pVRuler) rect.left += m_pVRuler->m_nWidth;
			m_pWordLayer->SetFRect(rect);
		}

		if(m_pFlowDocument)
		{
			rect = rc;
			InflateRect(&rect,-1,-1);
			if(m_pVRuler) rect.top += m_pHRuler->m_nHeight ;
			if(m_pVRuler) rect.left += m_pVRuler->m_nWidth;
			m_pFlowDocument->SetFRect(rect);
		}

		return 0;
	}

	bool XWordSheetSvr::EvtSize(TEvent *pEvent ,LPARAM lParam)
	{
		if(m_pWordLayer)
		{
			if(m_pWordLayer->m_CursorInfo.nRow < 1)
			m_pWordLayer->CalcCursorInfo(m_pWordLayer->m_CursorInfo,1,1);
			m_pWordLayer->ShowCurrentCaret(true);
		}
		if(m_pFlowDocument)
		{

		}
		return true;
	}
	bool XWordSheetSvr::EvtChar(TEvent *pEvent ,LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		UINT nRepCnt = pEvent->key.nRepCnt;
		UINT nFlags = pEvent->key.nFlags;

#ifdef V1
		switch (nChar) 
		{
		case 0x08:          // Backspace 
			if(m_pWordLayer)
			{
				m_pWordLayer->BackDeleteChar(m_pWordLayer->m_CursorInfo);
				m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
			}
			break; 
		case 0x09:          // Tab 
			// Tab stops exist every four spaces, so add 
			// spaces until the user hits the next tab. 

			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 

			break; 

		case 0x0D: // Carriage return 
			// Go to the beginning of the next line. 
			// The bottom line wraps around to the top. 
			if(m_pWordLayer)
			{
				if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) nChar=0x0B;
				m_pWordLayer->InsertChar(m_pWordLayer->m_CursorInfo,nChar);
				m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
			}
			break; 
		case 0x0B:          // Carriage return 
			// Go to the beginning of the next line. 
			// The bottom line wraps around to the top. 
			if(m_pWordLayer)
			{
				m_pWordLayer->InsertChar(m_pWordLayer->m_CursorInfo,nChar);
				m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
			}
			break; 

		case 0x1B:        // Escape 
		case 0x0A:        // Linefeed 
			MessageBeep((UINT) -1); 
			break; 
		default:
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				break;
			if(m_pWordLayer)
			{
				m_pWordLayer->InsertChar(m_pWordLayer->m_CursorInfo,nChar);
				m_pWordLayer->ClampInView(m_pWordLayer->m_CursorInfo);
			}
			break; 
		}
#else
		if(m_pObj)
		{
			XCell::XCellSheetSvr* pCellSvr = (XCell::XCellSheetSvr*)m_pObj;
			pCellSvr->m_pSheetCtrl = this->m_pSheetCtrl;
			pCellSvr->m_pGridFrameSvrs->EvtChar(pEvent,lParam);
			return true;
		}

		switch (nChar) 
		{
		case 0x08:          // Backspace 
			if(m_pFlowDocument)
			{
				m_pFlowDocument->BackDeleteChar(m_pFlowDocument->m_CursorInfo);
				m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
			}
			break; 
		case 0x09:          // Tab 
			// Tab stops exist every four spaces, so add 
			// spaces until the user hits the next tab. 

			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 
			SendMessage(m_pSheetCtrl->m_hWnd, WM_CHAR, ' ', 1L); 

			break; 

		case 0x0D: // Carriage return 
			// Go to the beginning of the next line. 
			// The bottom line wraps around to the top. 
			if(m_pFlowDocument)
			{
				if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) nChar=0x0B;
				m_pFlowDocument->InsertChar(m_pFlowDocument->m_CursorInfo,nChar);
				m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
			}
			break; 
		case 0x0B:          // Carriage return 
			// Go to the beginning of the next line. 
			// The bottom line wraps around to the top. 
			if(m_pFlowDocument)
			{
				m_pFlowDocument->InsertChar(m_pFlowDocument->m_CursorInfo,nChar);
				m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
			}
			break; 

		case 0x1B:        // Escape 
		case 0x0A:        // Linefeed 
			MessageBeep((UINT) -1); 
			break; 
		default:
			if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE)
				break;
			if(m_pFlowDocument)
			{
				m_pFlowDocument->InsertChar(m_pFlowDocument->m_CursorInfo,nChar);
				m_pFlowDocument->ClampInView(m_pFlowDocument->m_CursorInfo);
			}
			break; 
		}
#endif
		return true;
	}
	
	int XWordSheetSvr::LoadTemplate(IXMLDOMElement * pElement,int state)
	{
		XOfficeSheetSvr::LoadTemplate(pElement);
		XWordDataSvr* pWordData = dynamic_cast<XWordDataSvr* >(m_pDataSvr);
		if(pWordData->m_pContentSvr)
		{
			if(m_pWordLayer)
			{
				m_pWordLayer->m_pWordData = pWordData;
				m_pWordLayer->m_pWordDocumentBlock->LoadData(pWordData->m_pContentSvr->m_pXWordDataDocument );
				m_pWordLayer->m_pWordDocumentBlock->AdjustExtent();
			}
			if(m_pFlowDocument)
			{
				//if(m_pFlowDocument->m_pContent)delete m_pFlowDocument->m_pContent;
				m_pFlowDocument->m_pContent = 	((XWordDataSvr *)m_pDataSvr)->m_pContentSvr->m_pXWordDataDocument;;
			}
		}
		return 1;
	}

	void XWordSheetSvr::DrawVBorder(POINT pt)
	{
		HPEN         OldPen;
		int          rop;
		HDC			hDC;

		hDC = ::GetDC(GetHWND());
		OldPen = CreatePen(PS_DOT,3,RGB(96,96,96));
		OldPen = (HPEN)::SelectObject(hDC,OldPen);
		rop = ::SetROP2(hDC,R2_XORPEN );

		RECT rt;
		::SetRect(&rt,m_pFlowDocument->m_ptStart.x,
				m_pFlowDocument->m_ptStart.y,
				pt.x,pt.y);

		::OffsetRect(&rt,- m_pFlowDocument->m_ptScroll.x,
					- m_pFlowDocument->m_ptScroll.y);
		::InflateRect(&rt,-1,-1);
		::MoveToEx(hDC,rt.left,rt.top,NULL);
		::LineTo(hDC,rt.left,rt.bottom);
		::LineTo(hDC,rt.right,rt.bottom);
		::LineTo(hDC,rt.right,rt.top);
		::LineTo(hDC,rt.left, rt.top);
		//::Rectangle(hDC,rt.left,rt.top,rt.right,rt.bottom);

		::SetROP2(hDC,rop);
		::SelectObject(hDC,OldPen);
		if(OldPen)::DeleteObject(OldPen);
		::ReleaseDC(GetHWND(),hDC);
	}

	void XWordSheetSvr::BlankDoc()
	{
		//if(m_pDataSvr)delete m_pDataSvr;
		//m_pDataSvr = new XWordDataSvr;
		(( XWordDataSvr *)m_pDataSvr)->m_pContentSvr = new XWordContentSvr;
		(( XWordDataSvr *)m_pDataSvr)->m_pContentSvr->m_pXWordDataDocument = new  XWordDataDocument;

		 XWordDataChapter * pChater = new XWordDataChapter();
		 XWordDataParagraph* pPara = new XWordDataParagraph();
		 XWordDataTextBlock* pText = new XWordDataTextBlock();
		 pText->m_pStrText = _tcsdup(L" ");
		 pChater->m_pDataParent = (( XWordDataSvr *)m_pDataSvr)->m_pContentSvr->m_pXWordDataDocument;
		 pChater->m_pChilds->push_back(pPara);
		 pPara->m_pDataParent = pChater;
		 pPara->m_pChilds->push_back(pText);
		 pText->m_pDataParent = pPara;
		 (( XWordDataSvr *)m_pDataSvr)->m_pContentSvr->m_pXWordDataDocument->m_pChilds->push_back(pChater);
	}

}}}}