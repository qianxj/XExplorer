#include "stdafx.h"
#include "xflowsheetsvr.hpp"
#include "XFlowChartSerializeSvr.hpp"
#include "xflowchartdataSvr.hpp"
#include "xflowchartcontentsvr.hpp"
#include "xflowchartobject.hpp"
#include "xflowchartlink.hpp"
#include "xcControl.h"
#include "IFlowChartEvent.hpp"
#include "xfloweditsvr.hpp"


namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowSheetSvr::XFlowSheetSvr():m_FState(XFONORMAL),m_pHitObject(NULL),m_nHitPos(XFOHITNONE),m_FDrawState(XFOSELECTION),m_pLink(NULL),m_nHotPos(-1),m_pStrShape(NULL),m_pShape(NULL),FInplaceObject(NULL),
		FEditorMode(false),m_pHitLink(NULL)
	{
		m_TopLeft.x = 0;
		m_TopLeft.y = 0;

		m_pSerializeSvr = new XFlowChartSerializeSvr;
		m_pDataSvr = new XFlowChartDataSvr;

		m_pEditSvr = new CEditSvr();
		m_pEditSvr->m_pSheetSvr = this;

		SetEventListons();
	}

	XFlowSheetSvr::~XFlowSheetSvr()
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
		if(m_pLink)delete m_pLink;
		if(m_pStrShape) delete m_pStrShape;
		//if(m_pShape)delete m_pShape;

		UnSetEventListons();
	}

	XFlowChartObject * XFlowSheetSvr::GetHitedObject()
	{
		return m_pHitObject;
	}

	bool XFlowSheetSvr::SetEventListons()
	{
		//this->AttachListonTo(pDest,WM_PAINT,(FNEvent)&XFlowSheetSvr::EvtPaint);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&XFlowSheetSvr::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONDBLCLK,(FNEvent)&XFlowSheetSvr::EvtDoubleClicked);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&XFlowSheetSvr::EvtLButtonUp);
		this->AttachListon(WM_MOUSEMOVE,(FNEvent)&XFlowSheetSvr::EvtMouseMove);
		this->AttachListon(WM_MOUSEWHEEL,(FNEvent)&XFlowSheetSvr::EvtMouseWheel);
		this->AttachListon(WM_VSCROLL,(FNEvent)&XFlowSheetSvr::EvtVScroll);
		this->AttachListon(WM_HSCROLL,(FNEvent)&XFlowSheetSvr::EvtHScroll);
		this->AttachListon(WM_SIZE,(FNEvent)&XFlowSheetSvr::EvtSize);
		this->AttachListon(WM_CHAR,(FNEvent)&XFlowSheetSvr::EvtChar);
		this->AttachListon(WM_KEYDOWN,(FNEvent)&XFlowSheetSvr::EvtKeyDown);
		this->AttachListon(WM_SETFOCUS,(FNEvent)&XFlowSheetSvr::EvtSetFocus);
		this->AttachListon(WM_KILLFOCUS,(FNEvent)&XFlowSheetSvr::EvtKillFocus);
		this->AttachListon(WM_XCOMMAND,(FNEvent)&XFlowSheetSvr::EvtXCommand);
		this->AttachListon(WM_NCHITTEST,(FNEvent)&XFlowSheetSvr::EvtNCHitTest);
		this->AttachListon(WM_SETCURSOR,(FNEvent)&XFlowSheetSvr::EvtSetCursor);
		this->AttachListon(WM_COMMAND,_T("frame:wkname"),EN_CHANGE,(FNEvent)&XFlowSheetSvr::EvtWkChanged);
		this->AttachListon(WM_COMMAND,_T("frame:wkid"),EN_CHANGE,(FNEvent)&XFlowSheetSvr::EvtWkIDChanged);
		return true;
	}

	bool XFlowSheetSvr::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool XFlowSheetSvr::EvtPaint(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XFlowSheetSvr::EvtLButtonDown(TEvent *pEvent ,LPARAM lParam)
	{
		if(GetHWND() && GetFocus() != GetHWND() && GetHWND()) ::SetFocus(GetHWND());
		POINT point = pEvent->mouse.point;
		UINT nFlags = pEvent->mouse.nFlags;
		
		point.x +=  m_TopLeft.x;
		point.y +=  m_TopLeft.y;

		XFlowChartDataSvr * pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);
		if(m_FDrawState & XFOWORKNODE && m_pStrShape)
		{
			m_pShape = NULL;
			m_ptOld = point;
			m_ptPrev = point;
			m_FState = XFOWORKNODING ;
			return true;
		}
		if(!pDataSvr->m_pContentSvr) return true;
		vector<class XFlowChartObject *> &DrawObjects = *pDataSvr->m_pContentSvr->m_pDrawObjects;
		vector<class XFlowChartLink *> &DrawLinks = *pDataSvr->m_pContentSvr->m_pDrawLinks;
		if(m_FDrawState & XFOLINK)
		{
			RECT rc;
			int nIndex;
			bool bHoted = false;
			if(m_FState !=XFOLINKING)
			{
				if(m_pLink) delete m_pLink;
				m_pLink = new XFlowChartLink;
				m_pLink->m_FState = XFOCREATING;
				m_pLink->m_FState |= XFOSELECTED;
				m_FState = XFOLINKING;
			}
			for(int i=0;i<(int)DrawObjects.size();i++)
			{
				nIndex = DrawObjects[i]->TestHotHited(point,rc);
				if(nIndex >=0) 
				{
					bHoted = true;
					m_pLink->m_pStartObject = DrawObjects[i];
					m_pLink->m_nStartIndex = nIndex;
					m_pLink->m_ptStart = (*DrawObjects[i]->m_phtPoints)[nIndex];
					::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&rc,false);

					break;
				}
			}
			if(!bHoted)
			{
				m_pLink->m_nStartIndex = -1;
				m_pLink->m_ptStart = point;
			}
		}else
		{

			XFOHitPosition htPos = XFOHITNONE;
			for(int i=0;i<(int)DrawObjects.size();i++)
			{
				if(DrawObjects[i]->m_FState & XFOSELECTED)
				{
					htPos = DrawObjects[i]->GetHitResizePat(point);
					if(htPos!=XFOHITNONE)
					{
						m_nHitPos = htPos;
						m_pHitObject = DrawObjects[i];
						break;
					}
				}
			}
			if(htPos!=XFOHITNONE)
			{
				if(!(m_state & goRunState))
				{
					m_FState = XFORESIZING;
					this->m_ptMouse = point;
				}

			}else
			{
				m_pHitLink = NULL;
				for(int i=0;i<(int)DrawLinks.size();i++)
				{
					this->FRect;
					if(DrawLinks[i]->IsHoted(point.x,point.y))
					{
						DrawLinks[i]->m_FState |= XFOSELECTED;
						m_pHitObject = NULL;
						m_pHitLink = DrawLinks[i]; 

						FLCNMHDR hdr;
						memset(&hdr,0,sizeof(FLCNMHDR));
						hdr.hwndFrom = m_pSheetCtrl->GetHWND();
						hdr.idFrom = 0;
						hdr.code = FLC_LINKSELECTED;
						hdr.pWorkNode = NULL;
						hdr.pObj = NULL;
						hdr.pLink = m_pHitLink;
						::SendMessage(m_pSheetCtrl->GetWin()->GetHWND(),WM_NOTIFY,0,(LPARAM)&hdr);

						break;
					}
				}

				if(!(MK_CONTROL & nFlags))
				{
					for(int i = (int)DrawLinks.size() - 1;i>=0; i--)
					{
						if(DrawLinks[i]->m_FState & XFOSELECTED && DrawLinks[i] != m_pHitLink )
						{
							DrawLinks[i]->m_FState = DrawLinks[i]->m_FState & ~XFOSELECTED;
							::InvalidateRect(GetHWND(),NULL,false);
						}
						if(DrawLinks[i] != m_pHitLink)
							DrawLinks[i]->m_FState = XFONORMAL;
						else
							::InvalidateRect(GetHWND(),NULL,false);
					}
				}
				

				if(!(MK_CONTROL & nFlags))
				{
					for(int i = (int)DrawObjects.size() - 1;i>=0; i--)
					{
						if(DrawObjects[i]->m_FState & XFOSELECTED)
						{
							DrawObjects[i]->m_FState = DrawObjects[i]->m_FState & ~XFOSELECTED;
							DrawObjects[i]->InvalidatePatRgn(m_pSheetCtrl->m_hWnd,m_TopLeft);
						}
						DrawObjects[i]->m_FState = XFONORMAL;
					}
				}

				if(m_pHitLink)
				{
					HideEditor();
					return true;
				}

				int i=(int)DrawObjects.size() - 1;
				for(;i>=0; i--)
				{
					if(PtInRect(&DrawObjects[i]->FRect,point))
					{
						DrawObjects[i]->m_FState |= XFOSELECTED;
						DrawObjects[i]->InvalidatePatRgn(m_pSheetCtrl->m_hWnd,m_TopLeft);
						this->m_ptMouse = point;

						if(m_state & goRunState)
						{
							m_pHitObject = DrawObjects[i];
							break;
						}
						if(m_pHitObject == DrawObjects[i])
						{
							ShowEditor();
							return true;
						}else
							m_pHitObject = DrawObjects[i];

						FLCNMHDR hdr;
						memset(&hdr,0,sizeof(FLCNMHDR));
						hdr.hwndFrom = m_pSheetCtrl->GetHWND();
						hdr.idFrom = 0;
						hdr.code = FLC_OBJECTSELECTED;
						hdr.pWorkNode = NULL;
						hdr.pObj = m_pHitObject;
						hdr.pLink = NULL;
						::SendMessage(m_pSheetCtrl->GetWin()->GetHWND(),WM_NOTIFY,0,(LPARAM)&hdr);

						if(m_pHitObject->m_pElement)
						{
							IXMLDOMNode * pNode;
							BSTR bstr = NULL;
							
							variant_t var;
							m_pHitObject->m_pElement->getAttribute(L"ID",&var);
							if(var.bstrVal && var.bstrVal[0])
							{
								if(m_pSheetCtrl->m_pxfNode)
								{
									xfNode * pxfNode = this->m_pSheetCtrl->GetWin()->m_pXPage->GetAgentNode();
									if(pxfNode)
									{
										Hxsoft::XFrame::xfControl * pControl = pxfNode->GetXPage()->m_pWin->GetControl(_T("wkid"));
										if(pControl)
										{
											HWND hWnd = pControl->m_hWnd;

											::SetWindowText(hWnd,var.bstrVal);
										}
									}
								}
							}else
							{
								if(m_pSheetCtrl->m_pxfNode)
								{
									xfNode * pxfNode = this->m_pSheetCtrl->GetWin()->m_pXPage->GetAgentNode();
									if(pxfNode)
									{
										Hxsoft::XFrame::xfControl * pControl = pxfNode->GetXPage()->m_pWin->GetControl(_T("wkid"));
										if(pControl)
										{
											HWND hWnd = pControl->m_hWnd;

											::SetWindowText(hWnd,L"");
										}
									}
								}
							}
							if(m_pHitObject->m_pDataElement)
							{
								m_pHitObject->m_pDataElement->selectSingleNode(L"devlink/@url",&pNode);
								if(pNode)
								{
									pNode->get_text(&bstr);
									pNode->Release();
								}
								
								if(bstr)
								{
									if(m_pSheetCtrl->m_pxfNode)
									{
										xfNode * pxfNode = this->m_pSheetCtrl->GetWin()->m_pXPage->GetAgentNode();
										if(pxfNode)
										{
											Hxsoft::XFrame::xfControl * pControl = pxfNode->GetXPage()->m_pWin->GetControl(_T("wkname"));
											if(pControl)
											{
												HWND hWnd = pControl->m_hWnd;

												::SetWindowText(hWnd,bstr);
											}
										}
									}
									::SysFreeString(bstr);
								}
							}
						}else
						{
							if(m_pSheetCtrl->m_pxfNode)
							{
								xfNode * pxfNode = this->m_pSheetCtrl->GetWin()->m_pXPage->GetAgentNode();
								if(pxfNode)
								{
									Hxsoft::XFrame::xfControl * pControl = pxfNode->GetXPage()->m_pWin->GetControl(_T("wkname"));
									if(pControl)
									{
										HWND hWnd = pControl->m_hWnd;

										::SetWindowText(hWnd,_T(""));
									}
								}
							}
						}
						break;
					}
				}
				if(i < 0)
				{
					HideEditor();
					m_pHitObject  = NULL;
					return true;
				}
				m_FState = XFOMOVING;
			}
		}
		HideEditor();

		return true;
	}
	bool XFlowSheetSvr::EvtLButtonUp(TEvent *pEvent ,LPARAM lParam)
	{
		XFlowChartDataSvr * pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);
		EvtMouseMove(pEvent,lParam);
		if(m_FDrawState & XFOLINK)
		{
			bool bHoted = false;
			if(m_FState ==XFOLINKING)
			{
				m_FState = XFONORMAL;
				if(abs(m_pLink->m_ptStart.x - m_pLink->m_ptEnd.x)  < 5 &&
					abs(m_pLink->m_ptStart.y - m_pLink->m_ptEnd.y) < 5)
				{
					delete m_pLink;
					m_pLink = NULL;
					::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&FRect,false);
				}else
				{
					m_pLink->AutoCalcPoints();
					m_pLink->m_FState &= ~XFOCREATING;
					m_pLink->m_FState &= ~XFOSELECTED;
					pDataSvr->m_pContentSvr->m_pDrawLinks->push_back(m_pLink);
					m_pLink = NULL;
					m_nHotPos = -1;
				}
				::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&FRect,false);
			}
		}
		m_FState = XFONORMAL;
		return true;
	}

	RECT NormalizeRect(RECT &rect)
	{
		int nTemp;
		if (rect.left > rect.right)
		{
			nTemp = rect.left;
			rect.left = rect.right;
			rect.right = nTemp;
		}
		if (rect.top > rect.bottom)
		{
			nTemp = rect.top;
			rect.top = rect.bottom;
			rect.bottom = nTemp;
		}
		return rect;
	};

	bool XFlowSheetSvr::EvtMouseMove(TEvent *pEvent ,LPARAM lParam)
	{
		HRGN	hRgn,hRgn0,hRgn1;
		RECT	rect;
		RECT	rectex;
		POINT point = pEvent->mouse.point;

		point.x +=  m_TopLeft.x;
		point.y +=  m_TopLeft.y;

		XFlowChartDataSvr * pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);
		if(m_FDrawState & XFOWORKNODE && m_FState == XFOWORKNODING)
		{
			RECT rc;
			::SetRect(&rc,m_ptOld.x,m_ptOld.y,point.x,point.y);
			if(rc.right - rc.left > 4 && rc.bottom - rc.top > 4 && !m_pShape)
			{
				if(!pDataSvr->m_pContentSvr)
					pDataSvr->m_pContentSvr = new XFlowChartContentSvr();

				IXMLDOMElement* pElement;
				xbXml xml;
				xml.m_pXmlDoc->createElement(L"shape",&pElement);
				pElement->setAttribute(L"refname",(_variant_t)m_pStrShape);
				pElement->setAttribute(L"startx",(_variant_t)point.x);
				pElement->setAttribute(L"starty",(_variant_t)point.y);
				pElement->setAttribute(L"endx",(_variant_t)_T("E80"));
				pElement->setAttribute(L"endy",(_variant_t)_T("E32"));

				m_pShape =  new XFlowChartObject();
				m_pShape->LoadObject(pElement);
				pElement->Release();
				m_pShape->m_pDataSvr = pDataSvr;
				(*pDataSvr-> m_pContentSvr->m_pDrawObjects).push_back(m_pShape);
			}
			if(m_pShape)m_pShape->SetFRect(rc);
			NormalizeRect(rc);
			::InflateRect(&rc,5,5);
			::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&rc,true);
			::SetRect(&rc,m_ptOld.x,m_ptOld.y,m_ptPrev.x,m_ptPrev.y);
			NormalizeRect(rc);
			::InflateRect(&rc,5,5);
			::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&rc,true);
			m_ptPrev = point;
			return true;
		}
		if(!pDataSvr->m_pContentSvr) return false;
		vector<class XFlowChartObject *> &DrawObjects = *pDataSvr->m_pContentSvr->m_pDrawObjects;
		if(m_FDrawState & XFOLINK)
		{
			RECT rc;
			int nIndex;
			bool bHoted = false;
			for(int i=0;i<(int)DrawObjects.size();i++)
			{
				nIndex = DrawObjects[i]->TestHotHited(point,rc);
				if(nIndex >=0) 
				{
					if(m_FState ==XFOLINKING)
					{
						m_pLink->m_pEndObject = DrawObjects[i];
						m_pLink->m_nEndIndex = nIndex;
						m_pLink->m_ptEnd = (*DrawObjects[i]->m_phtPoints)[nIndex];
						m_pLink->AutoCalcPoints();
						m_nHotPos = nIndex;
						m_pHotObject = DrawObjects[i];
						::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&FRect,false);
					}else
					{
						if(nIndex != m_nHotPos)
						{
							if(m_nHotPos >=0)
							{
								::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&m_hotrc,false);
							}else
							{
								m_nHotPos = nIndex;
								m_pHotObject = DrawObjects[i];
								m_hotrc = rc;
								::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&rc,false);
							}
						}
					}
					bHoted = true;
					break;
				}
			}
			if(!bHoted)
			{
				if(m_FState ==XFOLINKING)
				{
					m_pLink->m_nEndIndex = -1;
					m_pLink->m_ptEnd = point;
					m_pLink->AutoCalcPoints();
					m_nHotPos = -1;
					::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&FRect,false);
				}else
				{
					if(m_nHotPos >=0)
						::InvalidateRect(this->m_pSheetCtrl->m_hWnd,&m_hotrc,false);
					m_nHotPos = -1;
					m_pHotObject = NULL;
				}
			}
		}
		if(m_FState == XFOMOVING)
		{
			hRgn = ::CreateRectRgn(0,0,0,0);
			for(int i=0;i< (int)DrawObjects.size();i++)
			{
				if(DrawObjects[i]->m_FState & XFOSELECTED)
				{
					rect = DrawObjects[i]->FRect;
					rectex = rect;
					InflateRect(&rectex,7,7);
					hRgn0 = ::CreateRectRgnIndirect(&rectex);
					::OffsetRect(&rect,point.x - m_ptMouse.x,point.y - m_ptMouse.y);
					DrawObjects[i]->SetFRect(rect);
					//DrawObjects[i]->FRect = rect;
					XFlowChartDataSvr* pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);
					for(int k=0;k<(int)pDataSvr->m_pContentSvr->m_pDrawLinks->size();k++)
					{
						if((*pDataSvr->m_pContentSvr->m_pDrawLinks)[k]->m_pStartObject == DrawObjects[i] ||
							(*pDataSvr->m_pContentSvr->m_pDrawLinks)[k]->m_pEndObject == DrawObjects[i])
						{
							(*pDataSvr->m_pContentSvr->m_pDrawLinks)[k]->AutoCalcPoints();
						}
					}
					rectex = rect;
					InflateRect(&rectex,7,7);
					hRgn1 = ::CreateRectRgnIndirect(&rectex);
					::CombineRgn(hRgn0,hRgn0,hRgn1,RGN_OR);
					::CombineRgn(hRgn,hRgn,hRgn0,RGN_OR);
					::DeleteObject(hRgn0);
					::DeleteObject(hRgn1);
					m_ptMouse = point;
				}
			}
			::InvalidateRgn(m_pSheetCtrl->m_hWnd,NULL,false);
			//::InvalidateRgn(m_pSheetCtrl->m_hWnd,hRgn,false);
			::DeleteObject(hRgn);
		}
		if(m_FState == XFORESIZING)
		{
			hRgn = ::CreateRectRgn(0,0,0,0);
			rect = m_pHitObject->FRect;
			rectex = rect;
			InflateRect(&rectex,7,7);
			hRgn0 = ::CreateRectRgnIndirect(&rectex);

			m_pHitObject->HitAdjust(this->m_nHitPos,point.x - m_ptMouse.x,point.y - m_ptMouse.y);
			for(int k=0;k<(int)pDataSvr->m_pContentSvr->m_pDrawLinks->size();k++)
			{
				if((*pDataSvr->m_pContentSvr->m_pDrawLinks)[k]->m_pStartObject == m_pHitObject ||
					(*pDataSvr->m_pContentSvr->m_pDrawLinks)[k]->m_pEndObject == m_pHitObject)
				{
					(*pDataSvr->m_pContentSvr->m_pDrawLinks)[k]->AutoCalcPoints();
				}
			}
			rect = m_pHitObject->FRect;
			rectex = rect;
			InflateRect(&rectex,7,7);
			hRgn1 = ::CreateRectRgnIndirect(&rectex);
			::CombineRgn(hRgn0,hRgn0,hRgn1,RGN_OR);
			::CombineRgn(hRgn,hRgn,hRgn0,RGN_OR);
			::DeleteObject(hRgn0);
			::DeleteObject(hRgn1);
			m_ptMouse = point;

			::InvalidateRgn(m_pSheetCtrl->m_hWnd,NULL,false);
			//::InvalidateRgn(m_pSheetCtrl->m_hWnd,hRgn,false);
			::DeleteObject(hRgn);
		}
		return true;
	}
	bool XFlowSheetSvr::EvtMouseWheel(TEvent *pEvent ,LPARAM lParam)
	{
		if(pEvent->wheel.zDelta < 0)
			m_TopLeft.y += 5;
		if(pEvent->wheel.zDelta > 0)
		{
			if(this->m_TopLeft.y > 5)
				m_TopLeft.y -= 5;
			else
				m_TopLeft.y = 0;
		}

		RECT rc=FRect;
		rc.left = 0;
		rc.top = 0;
		::InvalidateRect(GetHWND(),&rc,false);
		return true;
	}
	bool XFlowSheetSvr::EvtVScroll(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XFlowSheetSvr::EvtHScroll(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XFlowSheetSvr::EvtSize(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XFlowSheetSvr::EvtChar(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}

	int XFlowSheetSvr::InvalidateRect()
	{
		RECT rc=FRect;
		rc.left = 0;
		rc.top = 0;
		::InvalidateRect(GetHWND(),&rc,false);
		return 1;
	}

	bool XFlowSheetSvr::EvtKeyDown(TEvent *pEvent ,LPARAM lParam)
	{
		if(pEvent->key.nChar==VK_DELETE)
		{
			XFlowChartDataSvr * pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);
			vector<class XFlowChartObject *> &DrawObjects = *pDataSvr->m_pContentSvr->m_pDrawObjects;
			vector<class XFlowChartLink *> &DrawLinks = *pDataSvr->m_pContentSvr->m_pDrawLinks;
			int cntlink = (int)DrawLinks.size();
			int cnt = (int)DrawObjects.size();
			for(int i=cnt - 1; i>=0;i--)
			{
				if(DrawObjects[i]->m_FState & XFOSELECTED)
				{

					for(int k=0;k<cntlink;k++)
					{
						if(DrawLinks[k]->m_pEndObject == DrawObjects[i])
							DrawLinks[k]->m_pEndObject = NULL;

						if(DrawLinks[k]->m_pStartObject == DrawObjects[i])
							DrawLinks[k]->m_pStartObject = NULL;
					}
					delete DrawObjects[i];
					DrawObjects.erase(DrawObjects.begin() + i);
					::InvalidateRect(GetHWND(),NULL,false);
					break;
				}
			}

			cnt = (int)DrawLinks.size();
			for(int i=cnt - 1; i>=0;i--)
			{
				if(DrawLinks[i]->m_FState & XFOSELECTED)
				{
					delete DrawLinks[i];
					DrawLinks.erase(DrawLinks.begin() + i);
					::InvalidateRect(GetHWND(),NULL,false);
					break;
				}
			}
		}
		else if(pEvent->key.nChar==VK_PRIOR)
		{
			RECT rc=FRect;
			if(this->m_TopLeft.y > rc.bottom - rc.top)
				m_TopLeft.y -= rc.bottom - rc.top;
			else
				m_TopLeft.y = 0;
			InvalidateRect();
		}
		else if(pEvent->key.nChar==VK_NEXT)
		{
			RECT rc=FRect;
			m_TopLeft.y += rc.bottom - rc.top;

			InvalidateRect();
		}
		else if(pEvent->key.nChar==VK_UP)
		{
			if(this->m_TopLeft.y > 5)
				m_TopLeft.y -= 5;
			else
				m_TopLeft.y = 0;
			InvalidateRect();
		}
		else if(pEvent->key.nChar==VK_DOWN)
		{
			m_TopLeft.y += 5;

			InvalidateRect();
		}
		else if(pEvent->key.nChar==VK_LEFT)
		{
			if(this->m_TopLeft.x > 5)
				m_TopLeft.x -= 5;
			else
				m_TopLeft.x = 0;
			InvalidateRect();
		}
		else if(pEvent->key.nChar==VK_RIGHT)
		{
			m_TopLeft.x += 5;

			InvalidateRect();
		}
		else if(pEvent->key.nChar==VK_HOME)
		{
			if(m_TopLeft.y != 0)
			{
				m_TopLeft.y = 0;
				InvalidateRect();
			}
		}
		return true;
	}
	bool XFlowSheetSvr::EvtSetFocus(TEvent *pEvent ,LPARAM lParam)
	{
		if(!this->m_pSheetCtrl->m_pxfNode) return true; 
		if(!this->m_pSheetCtrl->GetWin() || m_state & goRunState) return true;
		xfNode* pNode = this->m_pSheetCtrl->GetWin()->GetFrameWindow()->m_pXPage->GetAgentNode();

		if(pNode && pNode->GetXPage() )
		{
			if( pNode->GetXPage()->m_pFocusNodeControl && pNode->GetXPage()->m_pFocusNodeControl->GetHWND() != m_pSheetCtrl->GetHWND() ||
				!pNode->GetNodeByName(_T("xflowchartagent")))
				this->m_pSheetCtrl->FocusedSheet();
			else
				this->m_pSheetCtrl->FocusedSheet();
			if(m_FDrawState & XFOSELECTION)
			{
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = true;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_bChecked = false;
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_hWnd,NULL,true);
			}
			if(m_FDrawState & XFOLINK)
			{
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_bChecked = true;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_hWnd,NULL,true);
			}
			if(m_FDrawState & XFOWORKNODE)
			{
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_bChecked = false;
				((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
				::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_hWnd,NULL,true);
			}
		}
		return true;
	}
	bool XFlowSheetSvr::EvtKillFocus(TEvent *pEvent ,LPARAM lParam)
	{
		return true;
	}
	bool XFlowSheetSvr::EvtXCommand(TEvent *pEvent ,LPARAM lParam)
	{
		if(!pEvent->xcommand.pStrID) return true;
		if(_tcsicmp(pEvent->xcommand.pStrID,_T("xmPointerSelect"))==0)
		{
			m_FDrawState  = XFOSELECTION;
			((xcToolButton *)pEvent->xcommand.pCmdFrom)->m_bChecked = true;
			((xcToolButton *)((xcToolButton *)pEvent->xcommand.pCmdFrom)->GetWin()->GetControl(_T("xmLineSelect")))->m_bChecked = false;
			((xcToolButton *)((xcToolButton *)pEvent->xcommand.pCmdFrom)->GetWin()->GetControl(_T("xmNodeSelect")))->m_bChecked = false;

			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_hWnd,NULL,true);
			if(m_pStrShape)delete m_pStrShape;
			 m_pStrShape = NULL;
			 pEvent->dwRet = 1;
		}
		if(_tcsicmp(pEvent->xcommand.pStrID,_T("xmWkID"))==0)
		{
			if(m_pHitObject && m_pHitObject->m_pElement)
			{
				BSTR bstr = NULL;
				TCHAR buf[255];
				
				xfNode * pxfNode = this->m_pSheetCtrl->GetWin()->m_pXPage->GetAgentNode();
				if(pxfNode)
				{
					Hxsoft::XFrame::xfControl * pControl = pxfNode->GetXPage()->m_pWin->GetControl(_T("wkid"));
					if(pControl)
					{
						::GetWindowText(pControl->GetHWND(),buf,255);
						m_pHitObject->m_pElement->setAttribute(L"ID",(_variant_t)buf);
						m_pHitObject->m_pStrID = _tcsdup(buf);
					}
				}
			}
			pEvent->dwRet = 1;
		}
		if(_tcsicmp(pEvent->xcommand.pStrID,_T("xmLineSelect"))==0)
		{
			m_FDrawState =XFOLINK;
			((xcToolButton *)pEvent->xcommand.pCmdFrom)->m_bChecked = true;
			((xcToolButton *)((xcToolButton *)pEvent->xcommand.pCmdFrom)->GetWin()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;
			((xcToolButton *)((xcToolButton *)pEvent->xcommand.pCmdFrom)->GetWin()->GetControl(_T("xmNodeSelect")))->m_bChecked = false;

			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_hWnd,NULL,true);
			if(m_pStrShape)delete m_pStrShape;
			 m_pStrShape = NULL;
			 pEvent->dwRet = 1;
		}
		if(_tcsstr(pEvent->xcommand.pStrID,_T(":")))
		{
			
			m_FDrawState = XFOWORKNODE;
			((xcToolButton *)((Hxsoft::XFrame::xfControl *)pEvent->xcommand.pCmdFrom)->GetWin()->GetControl(_T("xmLineSelect")))->m_bChecked = false;
			((xcToolButton *)((Hxsoft::XFrame::xfControl *)pEvent->xcommand.pCmdFrom)->GetWin()->GetControl(_T("xmPointerSelect")))->m_bChecked = false;

			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmPointerSelect")))->m_hWnd,NULL,true);
			::InvalidateRect(((xcToolButton *)m_pSheetCtrl->GetWin()->GetFrameWindow()->GetControl(_T("xmLineSelect")))->m_hWnd,NULL,true);
			if(m_pStrShape)delete m_pStrShape;
			m_pStrShape = _tcsdup(pEvent->xcommand.pStrID);
			 pEvent->dwRet = 1;
		}
		return true;
	}

	bool XFlowSheetSvr::EvtDoubleClicked(TEvent *pEvent ,LPARAM lParam)
	{
		POINT point = pEvent->mouse.point;
		UINT nFlags = pEvent->mouse.nFlags;

		point.x +=  m_TopLeft.x;
		point.y +=  m_TopLeft.y;


		XFlowChartDataSvr * pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);
		if(!pDataSvr->m_pContentSvr) return true;
		vector<class XFlowChartObject *> &DrawObjects = *pDataSvr->m_pContentSvr->m_pDrawObjects;

		XFlowChartObject * pObject = NULL;
		for(int i = (int)DrawObjects.size() - 1;i>=0; i--)
		{
			if(PtInRect(&DrawObjects[i]->FRect,point)) 
			{
				pObject = DrawObjects[i];
				break;
			}
		}

		if(pObject && pObject->m_pDataElement && m_pHitObject)
		{
			IXMLDOMNode * pNode;
			BSTR bstr = NULL;
			m_pHitObject->m_pDataElement->selectSingleNode(L"devlink/@url",&pNode);
			if(pNode)
			{
				pNode->get_text(&bstr);
				pNode->Release();

				FLCNMHDR hdr;
				memset(&hdr,0,sizeof(FLCNMHDR));
				hdr.hwndFrom = m_pSheetCtrl->GetHWND();
				hdr.idFrom = 0;
				hdr.code = FLC_DOUBLECLICKED;
				hdr.pWorkNode = bstr;
				hdr.pObj = m_pHitObject;

				::SendMessage(m_pSheetCtrl->GetWin()->GetHWND(),WM_NOTIFY,0,(LPARAM)&hdr);
				::SysFreeString(bstr);
			}else
			{
				m_pHitObject->m_pDataElement->selectSingleNode(L"devlink/item",&pNode);
				if(pNode)
				{
					pNode->Release();

					FLCNMHDR hdr;
					memset(&hdr,0,sizeof(FLCNMHDR));
					hdr.hwndFrom = m_pSheetCtrl->GetHWND();
					hdr.idFrom = 0;
					hdr.code = FLC_DOUBLECLICKED;
					hdr.pWorkNode = NULL;
					hdr.pObj = m_pHitObject;

					::SendMessage(m_pSheetCtrl->GetWin()->GetHWND(),WM_NOTIFY,0,(LPARAM)&hdr);
				}
			}
		}

		/*if(pObject && m_pObservers)
		{
			for(int i=0;i<(int)m_pObservers->size();i++)
			{
				if((*m_pObservers)[i]) ((IFlowChartEvent *)(*m_pObservers)[i])->DoubleCilcked(this,NULL);
			}
		}*/
		return true;
	}

	bool XFlowSheetSvr::EvtWkChanged(TEvent *pEvent ,LPARAM lParam)
	{

		if(m_pHitObject && m_pHitObject->m_pElement)
		{
			IXMLDOMNode * pNode;
			TCHAR buf[255];
			::GetWindowText(pEvent->command.hCtrl,buf,255);
			m_pHitObject->m_pElement->selectSingleNode(L"devlink/@url",&pNode);
			if(pNode)
			{
				pNode->put_text(buf);
				pNode->Release();
			}
		}

		return true;
	}

	bool XFlowSheetSvr::EvtWkIDChanged(TEvent *pEvent ,LPARAM lParam)
	{
		if(m_pHitObject && m_pHitObject->m_pElement)
		{
			TCHAR buf[255];
			::GetWindowText(pEvent->command.hCtrl,buf,255);
			m_pHitObject->m_pElement->setAttribute(L"ID",(_variant_t)buf);
		}
		return true;
	}

	bool XFlowSheetSvr::EvtSetCursor(TEvent *pEvent ,LPARAM lParam)
	{
		HCURSOR Cur = NULL;
		XFlowChartDataSvr * pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);
		if(!pDataSvr || !pDataSvr->m_pContentSvr) return false;
		vector<class XFlowChartObject *> &DrawObjects = *pDataSvr->m_pContentSvr->m_pDrawObjects;
		if (pEvent->cursor.hWnd == GetHWND() && pEvent->cursor.nHitTest==HTCLIENT )
		{
			XFOHitPosition htPos = XFOHITNONE;
			if(this->m_FState & XFORESIZING)
				htPos = this->m_nHitPos;
			else
			{
				for(int i=0;i< (int)DrawObjects.size();i++)
				{
					if(DrawObjects[i]->m_FState & XFOSELECTED)
					{
						htPos = DrawObjects[i]->GetHitResizePat(m_ptHittest);
						if(htPos!=XFOHITNONE)break;
					}
				}
			}
			if(htPos!=XFOHITNONE)
			{
				if(htPos==XFOHITLEFTTOP || htPos==XFOHITRIGHTBOTTOM)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENWSE));//crVSplit
				if(htPos==XFOHITLEFTBOTTOM || htPos==XFOHITRIGHTTOP)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENESW));//crVSplit
				if(htPos==XFOHITLEFTCENTER || htPos==XFOHITRIGHTCENTER)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEWE));//crVSplit
				if(htPos==XFOHITBOTTOMCENTER || htPos==XFOHITTOPCENTER)
					Cur = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZENS));//crVSplit
			}
		}
		if(Cur != NULL)
		{
			SetCursor(Cur);
			pEvent->dwRet = TRUE;
		}
		return true;
	}
	bool XFlowSheetSvr::EvtNCHitTest(TEvent *pEvent ,LPARAM lParam)
	{
		POINT pt = pEvent->mouse.point;
		ScreenToClient(GetHWND(),&pt);
		m_ptHittest = pt;
		return true;
	}

	int XFlowSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRec)
	{
		XFlowChartDataSvr* pDataSvr = dynamic_cast<XFlowChartDataSvr *>(m_pDataSvr);

		RECT rc;
		if(pDrawRec)
			rc = *pDrawRec;
		else
			rc = FRect;

		if(!pDataSvr->m_pContentSvr) return 0;

		//::SetWindowOrgEx(hPaintDC,m_TopLeft.x,m_TopLeft.y,NULL);
		::SetViewportOrgEx(hPaintDC,- m_TopLeft.x,- m_TopLeft.y,NULL);
		for(int i=0;i<(int)pDataSvr->m_pContentSvr->m_pDrawObjects->size();i++)
		{
			if((*pDataSvr->m_pContentSvr->m_pDrawObjects)[i])
				(*pDataSvr->m_pContentSvr->m_pDrawObjects)[i]->DoDraw(hPaintDC,rc);

		}
		for(int i=0;i<(int)pDataSvr->m_pContentSvr->m_pDrawLinks->size();i++)
		{
			if((*pDataSvr->m_pContentSvr->m_pDrawLinks)[i])
				(*pDataSvr->m_pContentSvr->m_pDrawLinks)[i]->DoDraw(hPaintDC,rc);

		}

		if(m_nHotPos >=0) m_pHotObject->DrawHoted(hPaintDC,m_nHotPos);
		if(m_pLink)m_pLink->DoDraw(hPaintDC,rc);
				
		//::SetWindowOrgEx(hPaintDC,0,0,NULL);
		::SetViewportOrgEx(hPaintDC,0,0,NULL);
		return 0;
	}

	bool XFlowSheetSvr::CanEditShow()
	{
		return true;
	}

	void XFlowSheetSvr::HideEditor()
	{
		FEditorMode = FALSE;
		HideEdit();
	}
	void XFlowSheetSvr::HideEdit()
	{
		if(this->m_pEditSvr)this->m_pEditSvr->HiddenEditor();
		FInplaceObject = NULL;
	}
	void XFlowSheetSvr::UpdateEdit()
	{
		if(CanEditShow())
		{
			m_pEditSvr->SelectObject(m_pHitObject);
			FInplaceObject = m_pHitObject;
		}
	}
	void XFlowSheetSvr::UpdateEditor()
	{
		FEditorMode = FALSE;
		HideEdit();
	}
	void XFlowSheetSvr::ShowEditor()
	{
		FEditorMode = TRUE;
		UpdateEdit();
	}

	int XFlowSheetSvr::Clear()
	{
		//ShowEditor();
		m_pHitObject = NULL;

		m_FState = XFONORMAL;
		m_pHitObject = NULL;
		m_nHitPos = XFOHITNONE;
		m_FDrawState = XFOSELECTION;
		m_pLink = NULL;
		m_nHotPos = -1;
		m_pStrShape = NULL;
		m_pShape = NULL;
		FInplaceObject = NULL;
		FEditorMode = false;
		m_FState = XFONORMAL;
		
		return 1;
	}

	bool XFlowSheetSvr::SelectedObject(XFlowChartObject * pObject)
	{
		XFlowChartObject * pOldObject = this->GetHitedObject();
		if(pOldObject)
		{
			pOldObject->m_FState &=  ~XFOSELECTED;
			pOldObject = NULL;
		}
		if(pObject)
		{
			pObject->m_FState |=  XFOSELECTED;
			m_pHitObject = pObject;
		}
		return true;
	}

}}}}