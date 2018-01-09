#include "StdAfx.h"
#include "..\include\XBIReportSheetSvr.hpp"
#include "gridframesvr.hpp"
#include "xbireportobject.hpp"
#include "xbireportDatasvr.hpp"
#include "XBIReportSerializeSvr.hpp"
#include "xframe.h"
#include "xcellsheet.hpp"

#define HandTime 3000
#define HandDiv  2
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XBIReportSheetSvr::XBIReportSheetSvr(void):m_bWheelVert(true),m_pControlEdit(NULL),m_pRts(NULL),ptMousees(new vector<POINT>)
	{
		m_pDataSvr = new XBIReportDataSvr;

		FTopLeft.x = 1;
		FTopLeft.y = 1;

		this->SetEventListons();

	}

	XBIReportSheetSvr::~XBIReportSheetSvr(void)
	{
		if(m_pControlEdit)
		{
			::RemoveProp(m_pControlEdit->GetHWND(),L"this");
		}
		if(m_pRts)delete m_pRts;

		this->UnSetEventListons();
		if(ptMousees) delete ptMousees;
	}

	bool XBIReportSheetSvr::SetEventListons()
	{
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XBIReportSheetSvr::EvtSetFocus);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XBIReportSheetSvr::EvtLButtonDown);
		this->AttachListon(WM_MBUTTONDOWN,(FNEvent)&XBIReportSheetSvr::EvtMButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XBIReportSheetSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XBIReportSheetSvr::EvtMouseMove);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XBIReportSheetSvr::EvtKeyDown);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XBIReportSheetSvr::EvtSetCursor);

		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XBIReportSheetSvr::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&XBIReportSheetSvr::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&XBIReportSheetSvr::EvtHScroll);

		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&XBIReportSheetSvr::EvtDoubleClicked);

		return true;
	}

	bool XBIReportSheetSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool XBIReportSheetSvr::EvtLButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		if(::GetFocus() !=GetHWND()) ::SetFocus(GetHWND());

		this->SetCapture();
		this->ptMousees->clear();
		this->ptMouse = pEvent->mouse.point;
		this->dwMouseTick = ::GetTickCount();
		this->ptMousees->push_back(this->ptMouse);

		return true;
	}

	bool XBIReportSheetSvr::EvtMButtonDown(TEvent * pEvent, LPARAM lParam)
	{
		m_bWheelVert = !m_bWheelVert;
		return true;
	}

	int GetMouseTraceExtent(vector<POINT> * pts,SIZE &sz)
	{
		int MinX,MaxX;
		int MinY,MaxY;
		vector<POINT>::const_iterator tr;
		tr=pts->begin();
		MinX = (*tr).x;
		MaxX = (*tr).x;
		MinY = (*tr).y;
		MaxY = (*tr).y;
		for(;tr!=pts->end();tr++)
		{
			if((*tr).x < MinX) MinX = (*tr).x;
			if((*tr).y < MinY) MinY = (*tr).y;
			if((*tr).x > MaxX) MaxX = (*tr).x;
			if((*tr).y > MaxY) MaxY = (*tr).y;
		}
		sz.cx = MaxX - MinX;
		sz.cy = MaxY - MinY;
		return true;
	}

	bool XBIReportSheetSvr::EvtLButtonUp(TEvent * pEvent, LPARAM lParam)
	{
		this->ReleaseCapture();
		
		bool bHand = false;
		if(dwMouseTick > 0 && GetTickCount() - dwMouseTick <  HandTime)
		{
			ptMousees->push_back( pEvent->mouse.point);
			POINT ptStart = this->ptMouse;
			POINT ptEnd = pEvent->mouse.point;

			SIZE sz;
			GetMouseTraceExtent(ptMousees,sz);
			if((ptEnd.x - ptStart.x==0 || abs(ptEnd.y - ptStart.y) /abs(ptEnd.x - ptStart.x) >= 2) && abs(ptEnd.y - ptStart.y) > 10 )
			{
				if(ptEnd.y - ptStart.y >  20 && abs((ptEnd.y - ptStart.y) - sz.cy) < 2 )
				{
					//scroll down
					this->FTopLeft.y -= abs(ptEnd.y - ptStart.y)/HandDiv;
					bHand = true;
				}else if (ptEnd.y - ptStart.y < - 20 && abs(abs(ptEnd.y - ptStart.y) - sz.cy) < 2) 
				{
					//scroll up
					this->FTopLeft.y += abs(ptEnd.y - ptStart.y)/HandDiv;
					if(this->FTopLeft.y < 0 ) this->FTopLeft.y = 0;
					bHand = true;
				}
			}else if((ptEnd.y - ptStart.y==0 || abs(ptEnd.x - ptStart.x) /abs(ptEnd.y - ptStart.y) >= 2) && abs(ptEnd.x - ptStart.x) > 10 )
			{
				if(ptEnd.x - ptStart.x >  20 && abs((ptEnd.x - ptStart.x) - sz.cx) < 2)
				{
					//scroll right
					this->FTopLeft.x -= abs(ptEnd.x - ptStart.x)/HandDiv;
					bHand = true;
				}else if (ptEnd.x - ptStart.x < - 20 && abs(abs(ptEnd.x - ptStart.x) - sz.cx) < 2) 
				{
					//scroll left
					this->FTopLeft.x += abs(ptEnd.x - ptStart.x) /HandDiv ;
					if(this->FTopLeft.x < 0 ) this->FTopLeft.x = 0;
					bHand = true;
				}
			}else 
			{
				if(sz.cx > 20 || sz.cy > 20)
				{
					::SendMessage(::GetParent(GetHWND()),WM_READNOTE,pEvent->wParam,pEvent->lParam);
				}
				bHand = true;
			}
			if(!bHand)
			{
				::SendMessage(::GetParent(GetHWND()),WM_READNOTE,pEvent->wParam,pEvent->lParam);
				bHand = true;
			}

			if(bHand)
			{
				RECT rc=FRect;
				rc.left = 0;
				rc.top = 0;
				if(GetHWND()) ::InvalidateRect(GetHWND(),&rc,false);

				XBIReportDataSvr * pDataSvr = (XBIReportDataSvr *)m_pDataSvr;
				for(int i=0;i<(int)pDataSvr->m_pObjects->size();i++)
				{
					XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet * >((*pDataSvr->m_pObjects)[i]);
					RECT rt;
					if(pSheet && pSheet->m_pSheetSvr)
					{
						rt = (*m_pRts)[i];
						::OffsetRect(&rt,-FTopLeft.x,-FTopLeft.y);
						::MoveWindow(pSheet->GetHWND(),rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,true);
					}
				}
			}

			this->ptMousees->clear();
			this->dwMouseTick = -1;
		}
		return true;
	}
	bool XBIReportSheetSvr::EvtMouseMove(TEvent * pEvent, LPARAM lParam)
	{
		if(dwMouseTick > 0)
		{
			if( GetTickCount() - dwMouseTick >  HandTime) 
			{
				dwMouseTick = -1;
				this->ptMousees->clear();
			}else
			{
				HDC hdc;
				hdc = ::GetDC(this->GetHWND());
				HPEN hPen = ::CreatePen(PS_SOLID,2,RGB(255,0,255));
				hPen = (HPEN)::SelectObject(hdc,hPen);
				::MoveToEx(hdc,(*ptMousees)[ptMousees->size() - 1].x,(*ptMousees)[ptMousees->size() - 1].y,NULL);
				::LineTo(hdc,pEvent->mouse.point.x,pEvent->mouse.point.y);
				hPen = (HPEN)::SelectObject(hdc,hPen);
				::DeleteObject(hPen);
				::ReleaseDC(this->GetHWND(),hdc);

				if(abs(pEvent->mouse.point.x - (*ptMousees)[ptMousees->size() - 1].x) + 
					abs(pEvent->mouse.point.y - (*ptMousees)[ptMousees->size() - 1].y) > 1)ptMousees->push_back( pEvent->mouse.point);
			}
		}
		return true;
	}
	bool XBIReportSheetSvr::EvtKeyDown(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}
	bool XBIReportSheetSvr::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		xfAreaObject * pObj = this->GetObjectAtPoint(pEvent->mouse.point);
		if(pObj)pObj->ProcessMessage(GetHWND(),pEvent->message, pEvent->wParam,pEvent->lParam);

		return true;
	}
	bool XBIReportSheetSvr::EvtSetFocus(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XBIReportSheetSvr::EvtDoubleClicked(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}

	bool XBIReportSheetSvr::EvtMouseWheel(TEvent * pEvent, LPARAM lParam)
	{
		if((::GetKeyState(VK_CONTROL)<0 && m_bWheelVert) || (!m_bWheelVert && !(::GetKeyState(VK_CONTROL)<0)) )
		{
			if(pEvent->wheel.zDelta < 0)
				FTopLeft.x += 25;
			if(pEvent->wheel.zDelta > 0)
			{
				if(this->FTopLeft.x > 25)
					FTopLeft.x -= 25;
				else
					FTopLeft.x = 0;
			}
		}else
		{
			if(pEvent->wheel.zDelta < 0)
				FTopLeft.y += 5;
			if(pEvent->wheel.zDelta > 0)
			{
				if(this->FTopLeft.y > 5)
					FTopLeft.y -= 5;
				else
					FTopLeft.y = 0;
			}
		}
		
		RECT rc=FRect;
		rc.left = 0;
		rc.top = 0;
		if(GetHWND()) ::InvalidateRect(GetHWND(),&rc,false);

		XBIReportDataSvr * pDataSvr = (XBIReportDataSvr *)m_pDataSvr;
		for(int i=0;i<(int)pDataSvr->m_pObjects->size();i++)
		{
			XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet * >((*pDataSvr->m_pObjects)[i]);
			RECT rt;
			if(pSheet && pSheet->m_pSheetSvr)
			{
				rt = (*m_pRts)[i];
				::OffsetRect(&rt,-FTopLeft.x,-FTopLeft.y);
				::MoveWindow(pSheet->GetHWND(),rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,true);
			}
		}

		return true;
	}

	bool XBIReportSheetSvr::EvtHScroll(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}
	bool XBIReportSheetSvr::EvtVScroll(TEvent * pEvent, LPARAM lParam)
	{
		return true;
	}


	int XBIReportSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		RECT rect;
		if(pDrawRect)
			rect = *pDrawRect;
		else
			rect = FRect;
		
		int startY = FRect.top;
		int startX = FRect.left;

		XBIReportDataSvr * pDataSvr = (XBIReportDataSvr *)m_pDataSvr;
		for(int i=0;i<(int)pDataSvr->m_pObjects->size();i++)
		{
			XBIReportObject * pReport = dynamic_cast<XBIReportObject *>((*pDataSvr->m_pObjects)[i]);
			if(pReport)
			{
				RECT rt;
				rt.left = startX + 5;
				rt.top  = startY;
				rt.right =  rt.left + pReport->GetWidth();
				rt.bottom = rt.top  + pReport->GetHeight();

				::OffsetRect(&rt,- FTopLeft.x, - FTopLeft.y);
				//pReport->DoDraw(hPaintDC,&rt);

				startY = rt.bottom + 10;

			}else
			{
				XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet *>((*pDataSvr->m_pObjects)[i]);
				if(pSheet)
				{
					RECT rt;
					rt.left = startX + 5;
					rt.top  = startY;
					rt.right =  rt.left + 2400;
					rt.bottom = rt.top  + 240;

					::OffsetRect(&rt,- FTopLeft.x, - FTopLeft.y);
#if 0
					pSheet->DoDraw(hPaintDC,&rt);
#endif
					startY = rt.bottom + 10;
				}else
				{
					Hxsoft::XFrame::xfControl * pControl = dynamic_cast<Hxsoft::XFrame::xfControl *>((*pDataSvr->m_pObjects)[i]);

					if( pControl)
					{
						SIZE sz = pControl->GetContentExtent();

						RECT rt;
						rt.left = startX + 5;
						rt.top  = startY;
						rt.right =  rt.left + 2400;
						if(sz.cy >0)
							rt.bottom = rt.top  + sz.cy;
						else
							rt.bottom = rt.top  + 160;

						::OffsetRect(&rt,- FTopLeft.x, - FTopLeft.y);

						if(pControl)
						{
							pControl->DoDraw(hPaintDC,&rt);
							startY = rt.bottom + 10;
						}
					}
				}
			}
		}

		return 1;
	}
	
	int XBIReportSheetSvr::LoadTemplate(IXMLDOMElement * pElement,int state)
	{
		this->FTopLeft.x = 0;
		this->FTopLeft.y = 0;

		if(m_pRts)m_pRts->clear();

		XBIReportDataSvr * pDataSvr;
		if(m_pDataSvr)
		{
			pDataSvr = (XBIReportDataSvr *)m_pDataSvr;
			for(int i=0;i<(int)pDataSvr->m_pObjects->size();i++)
			{
				xfAreaObject * pObj = dynamic_cast<xfAreaObject *>((*pDataSvr->m_pObjects)[i]);
				if(pObj)
				{
					this->RemoveRouterMessageTo(pObj);
				}
#if 1
				XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet * >((*pDataSvr->m_pObjects)[i]);
				if(pSheet && pSheet->m_pSheetSvr)
				{
					Hxsoft::XFrame::xfControl * pControl =  (Hxsoft::XFrame::xfControl *)pSheet;
					if(pControl->GetHWND())
					{
						::RemoveProp(pControl->GetHWND(),_T("this"));
						::DestroyWindow(pControl->GetHWND());
					}
				}
				XBIReportObject * pReport = dynamic_cast<XBIReportObject *>((*pDataSvr->m_pObjects)[i]);
				if(pReport)
				{
					Hxsoft::XFrame::xfControl * pControl =  (Hxsoft::XFrame::xfControl *) pReport->m_pCellSheet;
					if(pControl->GetHWND())
					{
						::RemoveProp(pControl->GetHWND(),_T("this"));
						::DestroyWindow(pControl->GetHWND());
					}
				}

#endif
			}
			delete m_pDataSvr;
		}

		pDataSvr = new XBIReportDataSvr;
		m_pDataSvr = pDataSvr;

		if(!m_pSerializeSvr)m_pSerializeSvr = new XBIReportSerializeSvr;
		m_pSerializeSvr->LoadSheetData(NULL,pElement,pDataSvr,state);
		//return 1;

		int startY = 0 ;
		int startX = 0 ;
		for(int i=0;i<(int)pDataSvr->m_pObjects->size();i++)
		{
			XBIReportObject * pReport = dynamic_cast<XBIReportObject *>((*pDataSvr->m_pObjects)[i]);
			if(pReport)
			{
				RECT rt;
				rt.left = startX;
				rt.top = startY;
				rt.right = rt.left + pReport->GetWidth();
				rt.bottom = rt.top + pReport->GetHeight();
				if(rt.right - rt.left <90)rt.right = rt.left + 90;
				pReport->SetFRect(rt);
				if(!m_pRts) m_pRts = new vector<RECT>;
				this->m_pRts->push_back(rt);

				startY += pReport->GetHeight();

#if 1
					Hxsoft::XFrame::xfControl * pControl =  (Hxsoft::XFrame::xfControl *)pReport->m_pCellSheet;
					pControl->m_pxfNode= NULL;
					//pControl->m_dwStyle &= ~WS_VISIBLE;
					//pControl->m_dwStyle |= WS_OVERLAPPEDWINDOW;
					HWND hWnd = pControl->CreateControl(NULL,rt,GetHWND(),0,xfApp::GetApp()->GetInstance());
					::SetProp(pControl->GetHWND(),_T("this"),pControl);
					//pControl->Initial();
#endif


			}else
			{
				XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet * >((*pDataSvr->m_pObjects)[i]);
				RECT rt;
				if(pSheet && pSheet->m_pSheetSvr)
				{
					//pSheet->m_hWnd = GetHWND();

					::SetRect(&rt,startX, startY,2400,startY + 240);
					pSheet->m_pSheetSvr->SetFRect(rt);
					if(!m_pRts) m_pRts = new vector<RECT>;
					this->m_pRts->push_back(rt);

					startY = rt.bottom + 10;
#if 1
					Hxsoft::XFrame::xfControl * pControl =  (Hxsoft::XFrame::xfControl *)pSheet;
					pControl->m_pxfNode= NULL;
					//pControl->m_dwStyle &= ~WS_VISIBLE;
					//pControl->m_dwStyle |= WS_OVERLAPPEDWINDOW;
					HWND hWnd = pControl->CreateControl(NULL,rt,GetHWND(),0,xfApp::GetApp()->GetInstance());
					::SetProp(pControl->GetHWND(),_T("this"),pControl);
					//pControl->Initial();
#endif
				}else
				{
					XCellSheet * pCellSheet = dynamic_cast<XCellSheet*>(pSheet);
					if(pCellSheet)
					{
						((XCellSheetSvr *)pCellSheet->m_pSheetSvr)->SetRowSelectorWidth(0);
						((XCellSheetSvr *)pCellSheet->m_pSheetSvr)->SetColHeaderHeight(0); 
					}else
					{
						Hxsoft::XFrame::xfControl * pControl = dynamic_cast<Hxsoft::XFrame::xfControl *>((*pDataSvr->m_pObjects)[i]);
						if(pControl)
						{
							
							int height = 240;
							SIZE sz = pControl->GetContentExtent();
							if(sz.cy >=0) height = sz.cy ;

							::SetRect(&rt,startX, startY,2400,startY + height);
							if(!m_pRts) m_pRts = new vector<RECT>;
							this->m_pRts->push_back(rt);

							startY += height + 10;
						}

					}
				}
				
			}
		}
		
		return 1;
	}

	int XBIReportSheetSvr::GetWidth()
	{
		return 1;
	}

	int XBIReportSheetSvr::GetHeight()
	{
		return 1;
	}
	
	xfAreaObject* XBIReportSheetSvr::GetObjectAtPoint(POINT pt)
	{
		pt.x += FTopLeft.x;
		pt.y += FTopLeft.y;

		XBIReportDataSvr * pDataSvr;
		if(!m_pDataSvr) return NULL;

		pDataSvr = (XBIReportDataSvr *)m_pDataSvr;
		xfAreaObject * pObj;
		RECT rc;
		for(int i=0;i<(int)pDataSvr->m_pObjects->size();i++)
		{
			pObj = dynamic_cast<xfAreaObject *>((*pDataSvr->m_pObjects)[i]);
			if(pObj)
			{
				XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet *>(pObj);
				if(pSheet)
				{
					rc = pSheet->m_pSheetSvr->FRect;
					if(pObj && ::PtInRect(&rc,pt)) return pObj;
				}
			}
		}

		return NULL;
	}

	int XBIReportSheetSvr::HideEditor()
	{
		if(m_pControlEdit && m_pControlEdit->GetHWND())
		{
			::ShowWindow(m_pControlEdit->GetHWND(),SW_HIDE);
		}
		return 1;
	}
	int XBIReportSheetSvr::ShowEditor()
	{
		if(m_pControlEdit && m_pControlEdit->GetHWND())
		::ShowWindow(m_pControlEdit->GetHWND(),SW_SHOW);
		return 1;
	}

	int XBIReportSheetSvr::InstanceEdit(Hxsoft::XFrame::xfControl * pControl,RECT rt)
	{
		if(m_pControlEdit && m_pControlEdit != pControl)
		{
			if(m_pControlEdit->GetHWND())
			{
				::RemoveProp(m_pControlEdit->GetHWND(),_T("this"));
				::DestroyWindow(m_pControlEdit->GetHWND());
			}
		}
		pControl->m_pxfNode= NULL;
		pControl->m_dwStyle &= ~WS_VISIBLE;
		pControl->CreateControl(NULL,rt,GetHWND(),0,xfApp::GetApp()->GetInstance());
		::SetProp(pControl->GetHWND(),_T("this"),pControl);
		return 1;
	}

	int XBIReportSheetSvr::MoveEditor(RECT &rt)
	{
		if(m_pControlEdit && m_pControlEdit->GetHWND())
			::MoveWindow(m_pControlEdit->GetHWND(),rt.left,rt.top,rt.right,rt.bottom,false);
		return 1;
	}

	int XBIReportSheetSvr::GetObjectsCount()
	{
		XBIReportDataSvr * pDataSvr = (XBIReportDataSvr *)m_pDataSvr;
		if(pDataSvr) return (int)pDataSvr->m_pObjects->size();
		return 0;
	}

	xbObject * XBIReportSheetSvr::GetObjectByIndent(int nIndex)
	{
		XBIReportDataSvr * pDataSvr = (XBIReportDataSvr *)m_pDataSvr;
		if(!pDataSvr) return NULL;
		if(nIndex < 0) return NULL;
		if(nIndex > (int)pDataSvr->m_pObjects->size()) return NULL;

		XBIReportObject * pReport = dynamic_cast<XBIReportObject *>((*pDataSvr->m_pObjects)[nIndex]);
		if(pReport)
		{
			return pReport->m_pCellSheet->QueryItem(L"ixcell");
		}
		
		XOfficeSheet * pSheet = dynamic_cast<XOfficeSheet *>((*pDataSvr->m_pObjects)[nIndex]);
		if(pSheet)
			return pSheet->GetInterface();
		else
		{
			Hxsoft::XFrame::xfControl * pControl = dynamic_cast<Hxsoft::XFrame::xfControl *>((*pDataSvr->m_pObjects)[nIndex]);
			if(pControl)
				return pControl->GetInterface();
			else
				return (*pDataSvr->m_pObjects)[nIndex];
		}
	}

	int XBIReportSheetSvr::RemoveObject(int nIndex)
	{
		return -1;
	}
	int XBIReportSheetSvr::AddObject(xbObject * pObject ,RECT &rt)
	{
		return -1;
	}

}}}}
