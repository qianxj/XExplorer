#include "StdAfx.h"
#include "..\include\xfAreaObject.hpp"
#include "xfapp.hpp"
#include "xfWin.hpp"
#include "xfControl.hpp"
#include "xfMdiFrame.hpp"
#include "xfNodeControl.hpp"
#include "xfXPage.hpp"
#include "xssparse.h"
#include "emmintrin.h"

#include "vlang.hpp"
#include "vlparser.hpp"
#include "vmlang.hpp"
#include "vmparser.hpp"
#include "vmtype.hpp"
#include "vmtuple.hpp"

#include "wpp.h"

using namespace  xcode;

namespace Hxsoft{ namespace XFrame
{
	static map<wstring,DWORD> g_mapEvent;

	xfAreaObject * xfAreaObject::m_CaptureObject = NULL;
	xfAreaObject::xfAreaObject(void):m_pListonsTo(NULL),m_pEventListons(NULL),m_pEventRouters(NULL),m_pEventSpecRouters(NULL)
	{
	}

	xfAreaObject::~xfAreaObject(void)
	{
		if(m_pListonsTo)m_pListonsTo->clear();
		if(m_pEventListons)m_pEventListons->clear();
		if(m_pEventRouters)m_pEventRouters->clear();
		if(m_pEventSpecRouters)
		{
			for(int i=0;i<(int)m_pEventSpecRouters->size();i++)
			{
				if((*m_pEventSpecRouters)[i])delete (*m_pEventSpecRouters)[i];
			}
		}
		if(m_pEventSpecRouters)m_pEventSpecRouters->clear();
	}
	
	int FindObject(vector<xfAreaObject *>* pvObjects, xfAreaObject * pObj)
	{
		for(int i=0;i < (int)pvObjects->size();i++)
		{
			if((*pvObjects)[i] == pObj) return i;
		}
		return -1;
	}
	bool xfAreaObject::DetachListon()
	{
		return DetachListonFrom(this);
	}

	bool xfAreaObject::DetachListon(DWORD dwMessageID)
	{
		return DetachListonFrom(this,dwMessageID);
	}

	bool xfAreaObject::DetachListonTo(class xfAreaObject * pDestObj )
	{
		return pDestObj->DetachListonFrom(this);
	}

	bool xfAreaObject::DetachListonTo(class xfAreaObject * pDestObj, DWORD dwMessageID)
	{
		return pDestObj->DetachListonFrom(this,dwMessageID);
	}

	bool xfAreaObject::DetachListonFrom(class xfAreaObject * pFromObj )
	{
		if(m_pEventListons)
		{
		    multimap <DWORD,TListonEvent>::iterator tr;
			for(tr = m_pEventListons->begin();tr!=m_pEventListons->end();tr = m_pEventListons->begin())
			{
				if(tr->second.pFromObject == pFromObj )m_pEventListons->erase(tr);
			}
		}
		return true;
	}


	bool xfAreaObject::DetachListonFrom(class xfAreaObject * pFromObj,DWORD dwMessageID)
	{
		if(m_pEventListons)
		{
		    multimap <DWORD,TListonEvent>::iterator tr,m_lIter,m_uIter;
			m_uIter = m_pEventListons->upper_bound(dwMessageID);
			m_lIter = m_pEventListons->lower_bound(dwMessageID);
			for(tr == m_uIter;tr!=m_lIter;tr--)
			{
				if(tr!= m_pEventListons->end() && tr->second.pFromObject == pFromObj && tr->first == dwMessageID  )
					m_pEventListons->erase(tr);
			}
		}
		return true;
	}
	
	bool xfAreaObject::AttachListonTo(xfAreaObject * pDest,DWORD dwMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		if(!m_pListonsTo)
		{
			m_pListonsTo = new vector<xfAreaObject *>;
			m_pListonsTo->push_back(pDest);
		}else
		{
			int nIndex;
			nIndex = FindObject(m_pListonsTo,pDest);
			if(nIndex < 0)m_pListonsTo->push_back(pDest);
		}
		return pDest->AttachListonFrom(this,dwMessageID,pXvmEvent,lParam);
	}

	bool xfAreaObject::AttachListonTo(xfAreaObject * pDest,DWORD dwMessageID,FNEvent fnEvent,LPARAM   lParam)
	{
		if(!m_pListonsTo)
		{
			m_pListonsTo = new vector<xfAreaObject *>;
			m_pListonsTo->push_back(pDest);
		}else
		{
			int nIndex;
			nIndex = FindObject(m_pListonsTo,pDest);
			if(nIndex < 0)m_pListonsTo->push_back(pDest);
		}
		return pDest->AttachListonFrom(this,dwMessageID,fnEvent,lParam);
	}
	
	bool xfAreaObject::AttachListon(LPTSTR pCtlName,DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		multimap<DWORD,TListonEvent>::iterator tr;
		pair<DWORD,TListonEvent> pr1;
		for(tr=m_pEventListons->begin();tr!=m_pEventListons->end();)
		{
			pr1 = *tr;
			if( (pr1.first == WM_COMMAND) &&
				pr1.second.fnEvent == NULL && 
			   (pr1.second.pXvmEvent &&
				//pr1.second.pXvmEvent->pvm == pXvmEvent->pvm &&
				//pr1.second.pXvmEvent->nobj == pXvmEvent->nobj &&
			   (pr1.second.pXvmEvent->fctname == pXvmEvent->fctname|| 
				_tcsicmp(pr1.second.pXvmEvent->fctname, pXvmEvent->fctname)==0)) &&
				pr1.second.lparam == lParam && 
				pr1.second.pFromObject ==  this &&
				(pr1.second.pCtlName && pCtlName && _tcsicmp(pr1.second.pCtlName, pCtlName)==0) &&
				pr1.second.dwCtlEvent == dwCtlEvent
			 )
			{
				m_pEventListons->erase(tr++);
				break;
			}
			else
				++tr;
		}		
		for(tr=m_pEventListons->begin();tr!=m_pEventListons->end();)
		{
			pr1 = *tr;
			if( (pr1.first == WM_NOTIFY) &&
				pr1.second.fnEvent == NULL && 
			   (pr1.second.pXvmEvent &&
				//pr1.second.pXvmEvent->pvm == pXvmEvent->pvm &&
				//pr1.second.pXvmEvent->nobj == pXvmEvent->nobj &&
			   (pr1.second.pXvmEvent->fctname == pXvmEvent->fctname|| 
				_tcsicmp(pr1.second.pXvmEvent->fctname, pXvmEvent->fctname)==0)) &&
				pr1.second.lparam == lParam && 
				pr1.second.pFromObject ==  this &&
				(pr1.second.pCtlName && pCtlName && _tcsicmp(pr1.second.pCtlName, pCtlName)==0) &&
				pr1.second.dwCtlEvent == dwCtlEvent
			 )
			{
				m_pEventListons->erase(tr++);
				break;
			}
			else
				++tr;
		}	

/*
		multimap<DWORD,TListonEvent>::iterator tr;
		pair<DWORD,TListonEvent> pr1;
		for(tr=m_pEventListons->begin();tr!=m_pEventListons->end();)
		{
			pr1 = *tr;
			if( (pr1.first == WM_COMMAND || pr1.first == WM_NOTIFY) &&
				pr1.second.fnEvent == NULL && 
			   (pr1.second.pXvmEvent &&
				//pr1.second.pXvmEvent->pvm == pXvmEvent->pvm &&
				//pr1.second.pXvmEvent->nobj == pXvmEvent->nobj &&
			   (pr1.second.pXvmEvent->fctname == pXvmEvent->fctname|| 
				_tcsicmp(pr1.second.pXvmEvent->fctname, pXvmEvent->fctname)==0)) &&
				//pr1.second.lparam == lParam && 
				pr1.second.pFromObject ==  this &&
				_tcsicmp(pr1.second.pCtlName, pCtlName)==0 &&
				pr1.second.dwCtlEvent == dwCtlEvent
			 )
				m_pEventListons->erase(tr++);
			else
				++tr;
		}		

		multimap<DWORD,TListonEvent>::const_iterator tr;
		pair<DWORD,TListonEvent> pr1;
		for(tr=m_pEventListons->begin();tr!=m_pEventListons->end();tr++)
		{
			pr1 = *tr;
			if(	pr1.first == WM_NOTIFY && 
				pr1.second.fnEvent == NULL && 
			   (pr1.second.pXvmEvent &&
				//pr1.second.pXvmEvent->pvm == pXvmEvent->pvm &&
				//pr1.second.pXvmEvent->nobj == pXvmEvent->nobj &&
			   (pr1.second.pXvmEvent->fctname == pXvmEvent->fctname|| 
				_tcsicmp(pr1.second.pXvmEvent->fctname, pXvmEvent->fctname)==0)) &&
				pr1.second.lparam == lParam && 
				pr1.second.pFromObject ==  this &&
				_tcsicmp(pr1.second.pCtlName, pCtlName)==0 &&
				pr1.second.dwCtlEvent == dwCtlEvent
			 ) return true;
		}
*/

		pair<DWORD,TListonEvent> pr;
		pr.first = WM_NOTIFY;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = pCtlName;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);

		pr.first = WM_COMMAND;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = pCtlName;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);

		return true;
	}

	bool xfAreaObject::AttachListonEx(HWND hWndControl,DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		multimap<DWORD,TListonEvent>::const_iterator tr;
		pair<DWORD,TListonEvent> pr1;
		for(tr=m_pEventListons->begin();tr!=m_pEventListons->end();tr++)
		{
			pr1 = *tr;
			if(	pr1.first == WM_NOTIFY && 
				pr1.second.fnEvent == NULL && 
			   (pr1.second.pXvmEvent &&
				//pr1.second.pXvmEvent->pvm == pXvmEvent->pvm &&
				//pr1.second.pXvmEvent->nobj == pXvmEvent->nobj &&
				_tcsicmp(pr1.second.pXvmEvent->fctname, pXvmEvent->fctname)==0) &&
				pr1.second.lparam == lParam && 
				pr1.second.pFromObject ==  this &&
				pr1.second.hCtrl == hWndControl &&
				pr1.second.dwCtlEvent == dwCtlEvent
			 )
			{
				delete pXvmEvent;
				return true;
			}
		}

		pair<DWORD,TListonEvent> pr;
		pr.first = WM_NOTIFY;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = NULL;
		pr.second.hCtrl = hWndControl;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);

		pr.first = WM_COMMAND;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = NULL;
		pr.second.hCtrl = hWndControl;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);

		return true;
	}

	bool xfAreaObject::AttachListon(DWORD dwMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam )
	{
		return AttachListonFrom(this,dwMessageID,pXvmEvent,lParam);
	}

	bool xfAreaObject::AttachListon(DWORD dwMessageID,LPTSTR pCtlName,DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		multimap<DWORD,TListonEvent>::const_iterator tr;
		pair<DWORD,TListonEvent> pr1;
		for(tr=m_pEventListons->begin();tr!=m_pEventListons->end();tr++)
		{
			pr1 = *tr;
			if(	pr1.first == dwMessageID && 
				pr1.second.fnEvent == NULL && 
			   (pr1.second.pXvmEvent &&
				//pr1.second.pXvmEvent->pvm == pXvmEvent->pvm &&
				//pr1.second.pXvmEvent->nobj == pXvmEvent->nobj &&
				_tcsicmp(pr1.second.pXvmEvent->fctname, pXvmEvent->fctname)==0) &&
				pr1.second.lparam == lParam && 
				pr1.second.pFromObject ==  this &&
				_tcsicmp(pr1.second.pCtlName, pCtlName)==0 &&
				pr1.second.dwCtlEvent == dwCtlEvent
			 ) return true;
		}

		pair<DWORD,TListonEvent> pr;
		pr.first = dwMessageID;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = pCtlName;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);
		return true;
	}

	bool xfAreaObject::AttachCtrlListon(DWORD dwCtlEvent,TXvmEvent* pXvmEvent,LPARAM   lParam )
	{
		return AttachListon((LPTSTR)NULL,dwCtlEvent,pXvmEvent,lParam);
	}


	bool xfAreaObject::AttachListon(DWORD dwMessageID,FNEvent fnEvent,LPARAM   lParam)
	{
		return AttachListonFrom(this,dwMessageID,fnEvent,lParam);
	}


	bool xfAreaObject::AttachListon(DWORD dwMessageID,LPTSTR pCtlName,DWORD dwCtlEvent,FNEvent fnEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = dwMessageID;
		pr.second.fnEvent = fnEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = pCtlName;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);
		return true;
	}

	bool xfAreaObject::AttachCtrlListon(DWORD dwCtlEvent,FNEvent fnEvent,LPARAM   lParam)
	{
		return AttachListon(NULL,dwCtlEvent,fnEvent,lParam);
	}
	bool xfAreaObject::AttachListon(LPTSTR pCtlName,DWORD dwCtlEvent,FNEvent fnEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = WM_NOTIFY;
		pr.second.fnEvent = fnEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = pCtlName;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);

		pr.first = WM_COMMAND;
		pr.second.fnEvent = fnEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pCtlName = pCtlName;
		pr.second.dwCtlEvent = dwCtlEvent;
		m_pEventListons->insert(pr);

		return true;
	}


	bool xfAreaObject::AttachXCommand(LPTSTR pXCmdID,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = WM_XCOMMAND;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pXCmdID = pXCmdID;
		m_pEventListons->insert(pr);
		return true;

	}

	bool xfAreaObject::AttachXCommand(LPTSTR pXCmdID,FNEvent fnEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = WM_XCOMMAND;
		pr.second.fnEvent = fnEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.pXCmdID = pXCmdID;
		m_pEventListons->insert(pr);
		return true;

	}

	bool xfAreaObject::AttachCommand(DWORD nCmdID,TXvmEvent* pXvmEvent,LPARAM   lParam )
	{
		return AttachCommand(nCmdID,nCmdID,pXvmEvent,lParam);
	}

	bool xfAreaObject::AttachCommand(DWORD nCmdID,FNEvent fnEvent,LPARAM   lParam )
	{
		return AttachCommand(nCmdID,nCmdID,fnEvent,lParam);
	}

	bool xfAreaObject::AttachCommand(DWORD nStartCmdID,DWORD nEndCmdID,TXvmEvent* pXvmEvent,LPARAM   lParam )
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = WM_COMMAND;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.nStartCmdID = nStartCmdID;
		pr.second.nEndCmdID = nEndCmdID;
		m_pEventListons->insert(pr);
		return true;
	}

	bool xfAreaObject::AttachCommand(DWORD nStartCmdID,DWORD nEndCmdID,FNEvent fnEvent,LPARAM   lParam )
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = WM_COMMAND;
		pr.second.fnEvent = fnEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  this;
		pr.second.nStartCmdID = nStartCmdID;
		pr.second.nEndCmdID = nEndCmdID;
		m_pEventListons->insert(pr);
		return true;
	}

	bool xfAreaObject::AttachListonFrom(xfAreaObject * pFromObj, DWORD dwMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = dwMessageID;
		pr.second.fnEvent = NULL;
		pr.second.pXvmEvent = pXvmEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  pFromObj;
		m_pEventListons->insert(pr);
		return true; 
	}

	bool xfAreaObject::AttachListonFrom(xfAreaObject * pFromObj, DWORD dwMessageID,FNEvent fnEvent,LPARAM   lParam)
	{
		if(!m_pEventListons)m_pEventListons = new multimap<DWORD,TListonEvent>;
		
		pair<DWORD,TListonEvent> pr;
		pr.first = dwMessageID;
		pr.second.fnEvent = fnEvent;
		pr.second.lparam = lParam;
		pr.second.pFromObject =  pFromObj;
		m_pEventListons->insert(pr);
		return true; 
	}

	bool xfAreaObject::ClearAllListonsTo(bool bIncludeOwner)
	{
		if(bIncludeOwner)this->DetachListon();
		for(int i=0;m_pListonsTo && i<(int)m_pListonsTo->size();i++)
		{
			if(!bIncludeOwner && (*m_pListonsTo)[i]== this) continue;
			this->DetachListonTo((*m_pListonsTo)[i]);
		}
		return true;
	}

	bool xfAreaObject::AddRouterMessageTo(TSpecRouter * pSpecRouter)
	{
		if(!m_pEventSpecRouters)
		{
			m_pEventSpecRouters = new vector<TSpecRouter *>;
			
		}
		m_pEventSpecRouters->push_back(pSpecRouter);
		return true;
	}

	bool xfAreaObject::AddRouterMessageTo(xfAreaObject * pDestObj)
	{
		if(!m_pEventRouters)
		{
			m_pEventRouters = new vector<xfAreaObject *>;
			m_pEventRouters->push_back(pDestObj);
		}
		else
		{
			int nIndex;
			nIndex = FindObject(m_pEventRouters,pDestObj);
			if(nIndex < 0)m_pEventRouters->push_back(pDestObj);
		}
		return true;
	}
	bool xfAreaObject::AddRouterMessageFrom(xfAreaObject * pFromObj)
	{
		return pFromObj->AddRouterMessageTo(this);
	}
	bool xfAreaObject::RemoveRouterMessageTo(xfAreaObject * pDestObj)
	{
		if(m_pEventRouters)
		{
			int nIndex;
			nIndex = FindObject(m_pEventRouters,pDestObj);
			if(nIndex >=0)m_pEventRouters->erase(m_pEventRouters->begin() + nIndex);
		}

		if(m_pEventSpecRouters)
		{
			for(int i=0;i<(int)m_pEventSpecRouters->size();i++)
			{
				if((*m_pEventSpecRouters)[i] && (*m_pEventSpecRouters)[i]->pDestObj==pDestObj)
				{
					delete (*m_pEventSpecRouters)[i];
					m_pEventSpecRouters->erase(m_pEventSpecRouters->begin() + i);
				}
			}
		}
		return true;
	}
	bool xfAreaObject::RemoveRouterMessageFrom(xfAreaObject * pFromObj)
	{
		return pFromObj->RemoveRouterMessageTo(this);
	}

	HWND  xfAreaObject::GetHWND()
	{
		return NULL;
	}

	bool  xfAreaObject::GetClientRect(RECT *pRect) 
	{
		::SetRect(pRect,0,0,0,0);
		return true;
	}

	void InitEvent(TEvent &tEvent,HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
	{
		::ZeroMemory(&tEvent,sizeof(TEvent));
		tEvent.hWnd = hWnd;
		tEvent.message = message;
		tEvent.wParam = wParam;
		tEvent.lParam = lParam;
		tEvent.pt.x = (SHORT)LOWORD(::GetMessagePos()); 
		tEvent.pt.y = (SHORT)HIWORD(::GetMessagePos());
		tEvent.time = ::GetMessageTime();
		switch(message)
		{
		case WM_NOTIFY:
			tEvent.notify.idCtrl = (int)wParam;
			tEvent.notify.pnmh = (LPNMHDR)lParam;
			break;
		case WM_SIZE:
			tEvent.size.nFlags = (UINT)wParam; 
			tEvent.size.point.x = (SHORT)LOWORD(lParam); 
			tEvent.size.point.y = (SHORT)HIWORD(lParam);
			break;
		case WM_ERASEBKGND:
			tEvent.paint.hdc = (HDC) wParam;
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
		case WM_NCMOUSEMOVE:
		case WM_NCHITTEST:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			tEvent.size.nFlags = (UINT)wParam; 
			tEvent.size.point.x = (SHORT)LOWORD(lParam); 
			tEvent.size.point.y = (SHORT)HIWORD(lParam);
			break;
		case WM_SETCURSOR:
			tEvent.cursor.hWnd = (HWND)wParam;
			tEvent.cursor.nHitTest = LOWORD(lParam);
			tEvent.cursor.mouseMsg = HIWORD(lParam);
			break;
		case WM_HSCROLL:
		case WM_VSCROLL:
			tEvent.scroll.nSBCode = LOWORD(wParam);
			tEvent.scroll.nPos	= HIWORD(wParam);
			tEvent.scroll.hWndCtl = (HWND)lParam;
			break;
		case WM_MOUSEWHEEL:
			tEvent.wheel.fwKeys = GET_KEYSTATE_WPARAM(wParam);
			tEvent.wheel.zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			tEvent.wheel.point.x = MAKEPOINTS(lParam).x;
			tEvent.wheel.point.y = MAKEPOINTS(lParam).y;
			break;
		case WM_CHAR:
		case WM_KEYDOWN: 
		case WM_KEYUP: 
			tEvent.key.nChar = (UINT)wParam;
			tEvent.key.nRepCnt =(SHORT) LOWORD(lParam);
			tEvent.key.nFlags = (UINT)lParam;
			break;
		case WM_COMMAND:
			tEvent.command.wmId    = LOWORD(wParam);
			tEvent.command.wmEvent = HIWORD(wParam);
			tEvent.command.hCtrl = (HWND)lParam;
			break;
		case WM_XCOMMAND:
			tEvent.xcommand.pStrID =(LPTSTR)wParam;
			tEvent.xcommand.pCmdFrom= reinterpret_cast<xfAreaObject *>(lParam);
			break;
		}
	}

	LPARAM  xfAreaObject::ProcessMessage(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam,bool bContinue)
	{
		if(WM_POSTDESTROYWINDOW==message)
		{
			if(ProcessMessage(hWnd,WM_CLOSE,0,0)==0) ::DestroyWindow(hWnd);
			return 0;
		}
		if(message==WM_PAINT)
		{
			if(bContinue)
			{
				PAINTSTRUCT ps;
				HDC hdc;
				hdc = BeginPaint(hWnd, &ps);

				ProcessMessage(hWnd,message,(WPARAM)hdc,0,false);

				EndPaint(hWnd, &ps);

				return 0;
			}
		}
		if(m_pEventRouters)
		{
			for(int i=0;i<(int)m_pEventRouters->size();i++)
			{
				LPARAM lRet = 0;
				if((*m_pEventRouters)[i])
					lRet = (*m_pEventRouters)[i]->ProcessMessage(hWnd,message,wParam,lParam,bContinue); 
				if(lRet != 0) return lRet;
			}
		}
		if(m_pEventSpecRouters)
		{
			for(int i=0;i<(int)m_pEventSpecRouters->size();i++)
			{
				LPARAM lRet = 0;
				if((*m_pEventSpecRouters)[i] && (*m_pEventSpecRouters)[i]->pDestObj)
				{
					for(int k=0;k<(int)(*m_pEventSpecRouters)[i]->Msgs.size();k++)
					{
						if((*m_pEventSpecRouters)[i]->Msgs[k]==message)
						{
							lRet = (*m_pEventSpecRouters)[i]->pDestObj->ProcessMessage(hWnd,message,wParam,lParam,bContinue); 
							break;
						}
					}
				}
				if(lRet != 0) return lRet;
			}
		}
		switch(message)
		{
		case WM_COMMAND:
			if(bContinue)
			{
				TEvent tEvent;
				InitEvent(tEvent,hWnd,message,wParam,lParam);
				tEvent.pSrcObject = this;
				xfAreaObject * pObject = (xfAreaObject *)::GetProp(tEvent.command.hCtrl,_T("this"));
				if(pObject)
				{
					pObject->ProcessMessage(hWnd,message,wParam,lParam,false);
					xfMdiFrame* pFrame = dynamic_cast<xfMdiFrame*>(this);
					if(pFrame)
					{
						HWND hWndSheet = pFrame->GetActiveSheet();
						xfAreaObject * pSheetObject = (xfAreaObject *)::GetProp(hWndSheet,_T("this"));
						if(pSheetObject)
						{
							pSheetObject->ProcessMessage(hWndSheet,message,wParam,lParam,false);//xiugai
							xfNodeControl* pFoucsNode = ((xfWin *)pSheetObject)->m_pXPage->GetFocusNodeControl();
							if(pFoucsNode && pFoucsNode->m_pxfControl  )
							{
								pFoucsNode->m_pxfControl->ProcessMessage(hWndSheet,message,wParam,lParam,false);//xiugai
							}
						}
					}
				}
			}
			break;
		case WM_NOTIFY:
			if(bContinue)
			{
				TEvent tEvent;
				InitEvent(tEvent,hWnd,message,wParam,lParam);
				tEvent.pSrcObject = this;
				xfAreaObject * pObject = (xfAreaObject *)::GetProp(tEvent.notify.pnmh->hwndFrom,_T("this"));
				if(pObject)
				{
					pObject->ProcessMessage(hWnd,message,wParam,lParam,false);
					xfMdiFrame* pFrame = dynamic_cast<xfMdiFrame*>(this);
					if(pFrame)
					{
						HWND hWndSheet = pFrame->GetActiveSheet();
						xfAreaObject * pSheetObject = (xfAreaObject *)::GetProp(hWndSheet,_T("this"));
						if(pSheetObject)
						{
							pSheetObject->ProcessMessage(hWndSheet,message,wParam,lParam,false); //xiugai
							xfNodeControl* pFoucsNode = ((xfWin *)pSheetObject)->m_pXPage->GetFocusNodeControl();
							if(pFoucsNode && pFoucsNode->m_pxfControl  )
							{
								pFoucsNode->m_pxfControl->ProcessMessage(hWndSheet,message,wParam,lParam,false);//xiugai
							}
						}
					}
				}
			}
			break;
		}

		if(m_pEventListons && m_pEventListons->size() >0 )
		{
		    multimap <DWORD,TListonEvent>::iterator tr,m_lIter,m_uIter;
			m_uIter = m_pEventListons->upper_bound(message);
			m_lIter = m_pEventListons->lower_bound(message);

			pair<multimap<DWORD,TListonEvent>::iterator,
				multimap<DWORD,TListonEvent>::iterator> pr;
			pr = m_pEventListons->equal_range(message);
			while(pr.first != pr.second)
			{
			//for(tr = m_lIter; tr!=m_uIter ;tr++)
			//{
				tr = pr.first;
				if(tr!= m_pEventListons->end() && tr->first == message  )
				{
					TEvent tEvent;
					InitEvent(tEvent,hWnd,message,wParam,lParam);
					tEvent.pSrcObject = this;   
					bool bRet;
					switch(message)
					{
					case WM_LBUTTONDOWN:
					case WM_LBUTTONUP:
					case WM_RBUTTONDOWN:
					case WM_RBUTTONUP:
					case WM_MOUSEMOVE:
						{
							if(m_CaptureObject && m_CaptureObject != tr->second.pFromObject )break;
							UINT	nFlags;
							POINT	point;
							nFlags = (UINT)wParam; 
							point.x = (SHORT)LOWORD(lParam); 
							point.y = (SHORT)HIWORD(lParam);

							RECT rc;
							tr->second.pFromObject->GetClientRect(&rc);
							if(::PtInRect(&rc,point) || tr->second.pFromObject == m_CaptureObject)
							{
								//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								if(tr->second.fnEvent)
									bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								else
									bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
							}
							break;
						}
					case WM_LBUTTONDBLCLK:
					case WM_RBUTTONDBLCLK:
						{
							if(m_CaptureObject && m_CaptureObject != tr->second.pFromObject )break;
							UINT	nFlags;
							POINT	point;
							nFlags = (UINT)wParam; 
							point.x = (SHORT)LOWORD(lParam); 
							point.y = (SHORT)HIWORD(lParam);

							RECT rc;
							tr->second.pFromObject->GetClientRect(&rc);
							if(::PtInRect(&rc,point) || tr->second.pFromObject == m_CaptureObject)
							{
								//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								if(tr->second.fnEvent)
									bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								else
									bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);

							}
							break;
						}
					case WM_PAINT:
						{
							//PAINTSTRUCT ps;
							HDC hdc;
							//hdc = BeginPaint(hWnd, &ps);
							hdc = (HDC)wParam;
							tEvent.paint.hdc = hdc;
							//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							if(tr->second.fnEvent)
								bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							else
								bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);

							//EndPaint(hWnd, &ps);
							break;
						}
					case WM_XCOMMAND:
						{
							if(tr->second.pXCmdID)
							{
								if(::_tcsicmp(tr->second.pXCmdID,tEvent.xcommand.pStrID)==0)
								{
									//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
									if(tr->second.fnEvent)
										bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
									else
										bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
								}
								break;
							}else
							{
								//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								if(tr->second.fnEvent)
									bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								else
									bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
							}
							break;
							
						}
					case WM_NOTIFY:
						{
							if(tr->second.hCtrl )
							{
								if(::IsWindow(tr->second.hCtrl))
								{
									if(tr->second.hCtrl ==tEvent.notify.pnmh->hwndFrom && tr->second.dwCtlEvent == tEvent.notify.pnmh->code)
									{
										//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										if(tr->second.fnEvent)
											bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										else
											bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
									}
								}
							}
							else
							{
								if(!tr->second.pCtlName  && (tr->second.dwCtlEvent == tEvent.notify.pnmh->code || tr->second.dwCtlEvent == -1))
								{
									//bRet = (this->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
									if(tr->second.fnEvent)
										bRet = (this->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
									else
										bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);


								}else
								{
									if(tr->second.pCtlName /*&&  bContinue*/)
									{
										xfAreaObject * pObject = (xfAreaObject *)::GetProp(hWnd,_T("this"));
										xfWin * pWin = dynamic_cast<xfWin *>(pObject);
										if(pWin)
										{
											xfControl * pControl = pWin->GetControl(tr->second.pCtlName);
											if(pControl)
											{
												if(pControl->m_hWnd ==tEvent.notify.pnmh->hwndFrom && tr->second.dwCtlEvent == tEvent.notify.pnmh->code)
												{
													tr->second.hCtrl = pControl->m_hWnd;
													//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
													if(tr->second.fnEvent)
														bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
													else
														bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
												}
											}
										}
									}
								}
								if( /*bContinue &&*/ !tr->second.pCtlName && tr->second.dwCtlEvent && tr->second.dwCtlEvent==0)
								{
									xfWinbase * pObject = (xfWinbase *)::GetProp(hWnd,_T("this"));
									xfWin * pWin = dynamic_cast<xfWin *>(pObject);
									if(pWin)
									{
										//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										if(tr->second.fnEvent)
											bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										else
											bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
									}
								}
							}
							break;
						}
					case WM_COMMAND:
						{
							if(tEvent.command.hCtrl)
							{
								if(tr->second.hCtrl )
								{
									if(::IsWindow(tr->second.hCtrl))
									{
										if(tr->second.hCtrl ==tEvent.command.hCtrl && tr->second.dwCtlEvent == tEvent.command.wmEvent /*&& tEvent.command.wmEvent >= 1*/)
										{
											//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
											if(tr->second.fnEvent)
												bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
											else
												bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
										}
									}
								}
								else
								{
									if(!tr->second.pCtlName && tr->second.dwCtlEvent == tEvent.command.wmEvent)
									{
										//bRet = (this->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										if(tr->second.fnEvent)
											bRet = (this->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										else
											bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
									}else
									{
										/*
										if(tr->second.pCtlName && bContinue)
										{
											xfAreaObject * pObject = (xfAreaObject *)::GetProp(hWnd,_T("this"));
											xfWin * pWin = dynamic_cast<xfWin *>(pObject);
											if(pWin)
											{
												xfControl * pControl = pWin->GetControl(tr->second.pCtlName);
												if(pControl)
												{
													tr->second.hCtrl = pControl->m_hWnd;
													if(tr->second.hCtrl ==tEvent.command.hCtrl && tr->second.dwCtlEvent == tEvent.command.wmEvent && tEvent.command.wmEvent > 1)
													{
														//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
														if(tr->second.fnEvent)
															bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
														else
															bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
													}
												}
											}
										}*/
										if(tr->second.pCtlName && bContinue)
										{
											xfAreaObject * pObject = (xfAreaObject *)::GetProp(hWnd,_T("this"));
											xfWin * pWin = dynamic_cast<xfWin *>(pObject);
											if(pWin)
											{
												xfControl * pControl = pWin->GetControl(tr->second.pCtlName);
												if(pControl)
												{
													if(pControl->m_hWnd ==tEvent.command.hCtrl && tr->second.dwCtlEvent == tEvent.command.wmEvent /*&& tEvent.command.wmEvent > 1*/)
													{
														tr->second.hCtrl = pControl->m_hWnd;
														//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
														if(tr->second.fnEvent)
															bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
														else
															bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
													}
												}
											}
										}
									}
								}
							}else
							{
								if(tr->second.nStartCmdID)
								{
									if((int)tr->second.nStartCmdID <= tEvent.command.wmId && (int)tr->second.nEndCmdID >= tEvent.command.wmId)
									{
										//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										if(tr->second.fnEvent)
											bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
										else
											bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
									}
								}else
								{
									//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
									if(tr->second.fnEvent)
										bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
									else
										bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
								}
							}
							break;
						}
					default:
						{
							//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							if(tr->second.fnEvent)
								bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							else
								bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
						}
					}
					if(tEvent.dwRet !=0) return tEvent.dwRet;
				}
				pr.first++;
			}

			m_uIter = m_pEventListons->upper_bound(0);
			m_lIter = m_pEventListons->lower_bound(0);
			for(tr = m_lIter;tr!=m_uIter ;tr++)
			{
				if(tr!= m_pEventListons->end() && tr->first == message  )
				{
					TEvent tEvent;
					InitEvent(tEvent,hWnd,message,wParam,lParam);
					tEvent.pSrcObject = this;
					bool bRet;
					switch(message)
					{
					case WM_LBUTTONDOWN:
					case WM_RBUTTONDOWN:
					case WM_LBUTTONUP:
					case WM_RBUTTONUP:
					case WM_MOUSEMOVE:
						{
							if(this->IsNotMeCaptured())break;
							UINT	nFlags;
							POINT	point;
							nFlags = (UINT)wParam; 
							point.x = (SHORT)LOWORD(lParam); 
							point.y = (SHORT)HIWORD(lParam);

							RECT rc;
							this->GetClientRect(&rc);
							if(::PtInRect(&rc,point) || this->IsCaptured())
							{
								//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								if(tr->second.fnEvent)
									bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
								else
									bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
							}
							break;
						}
					case WM_PAINT:
						{
							//PAINTSTRUCT ps;
							HDC hdc;
							//hdc = BeginPaint(hWnd, &ps);
							hdc = (HDC)wParam;
							tEvent.paint.hdc = hdc;
							
							//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							if(tr->second.fnEvent)
								bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							else
								bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
							//EndPaint(hWnd, &ps);
							break;
						}
					default:
						{
							//bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							if(tr->second.fnEvent)
								bRet = (tr->second.pFromObject->*tr->second.fnEvent)(&tEvent,tr->second.lparam);
							else
								bRet = this->xvmEventRecall(tr->second.pXvmEvent,&tEvent,tr->second.lparam);
						}
					}
					if(tEvent.dwRet !=0) return tEvent.dwRet; 
				}
			}
		}
		return 0;
	}

	int  xfAreaObject::SetCapture(xfAreaObject * pObj,HWND hWnd)
	{
		int nRet =1;
		EnterCriticalSection(&xfApp::GetApp()->m_csCapture);
		if(m_CaptureObject==pObj) goto SetCaptureEnd;
		if(m_CaptureObject)
		{
			nRet = -1;
			goto SetCaptureEnd;
		}
		m_CaptureObject =  pObj;
		::SetCapture(hWnd);
SetCaptureEnd:
		LeaveCriticalSection(&xfApp::GetApp()->m_csCapture);
		return nRet;

	}
	int  xfAreaObject::SetCapture()
	{
		return SetCapture(this,GetHWND());
	}

	int  xfAreaObject::ReleaseCapture()
	{
		int nRet =1;
		EnterCriticalSection(&xfApp::GetApp()->m_csCapture);
		m_CaptureObject =  NULL;
		::ReleaseCapture();
		LeaveCriticalSection(&xfApp::GetApp()->m_csCapture);
		return nRet;
	}

	bool xfAreaObject::IsCaptured()
	{
		if(m_CaptureObject==this) 
			return true;
		else
			return false;
	}

	bool xfAreaObject::HasCaptured()
	{
		if(m_CaptureObject) 
			return true;
		else
			return false;
	}

	xfAreaObject *  xfAreaObject::GetCaptured()
	{
		return m_CaptureObject;
	}

	bool xfAreaObject::IsNotMeCaptured()
	{
		if(m_CaptureObject && m_CaptureObject != this) 
			return true;
		else
			return false;
	}
    
	int Filter1(unsigned int code, struct _EXCEPTION_POINTERS *ep)
	{
		if (code == EXCEPTION_ACCESS_VIOLATION)
		{
			//wprintf(L"Caught access violation expected.\r\n");
			MessageBox(NULL, L"检查一下，程序中有空指针调用.\r\n",L"错误",MB_OK|MB_ICONERROR);
			
			LogMessage msg(Log::get_Log());
			msg.appendString(L"检查一下，程序中有空指针调用.\r\n");
			msg.appendString(debuger::get_debuger()->get_method_stack_string().c_str());
			msg.write();

			return EXCEPTION_EXECUTE_HANDLER;
		}
		else
		{
			MessageBox(NULL, L"出现一个意外了...\r\n",L"错误",MB_OK|MB_ICONERROR);

			LogMessage msg(Log::get_Log());
			msg.appendString(L"出现一个意外了...\r\n");
			msg.appendString(debuger::get_debuger()->get_method_stack_string().c_str());
			msg.write();

			return EXCEPTION_EXECUTE_HANDLER;
			//return EXCEPTION_CONTINUE_SEARCH;
		}
	}

	extern bool FetchData1(const tchar * p,void* param);
	/*{
		xfControl* pOutPut = (xfControl*)param;
		int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
		SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
		SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)p);
		SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

		return true;
	}*/

	bool xfAreaObject::xvmEventRecall(TXvmEvent * pXvmEvent,TEvent * pEvent,LPARAM lParam )
	{
		assert(pXvmEvent);
		if(pXvmEvent->fx)
		{
			wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
			if(!ptr_context) return 0;

			xfControl* pOutPut = ((xfWin*)this)->GetControl(_T("output"));
			if(!pOutPut)((xfWin*)this)->GetFrameWindow()->GetControl(_T("output"));
			if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;

			void * p = ptr_context->get_runtime()->prepare_call();
			ptr_context->get_runtime()->push_arg(p,(int)lParam);
			ptr_context->get_runtime()->push_arg(p,pEvent);
			ptr_context->get_runtime()->push_arg(p,pXvmEvent->pThis);
			__try
			{
				int ret = ptr_context->get_runtime()->call_method(ptr_context,p,CWord(pXvmEvent->fx,0));
				pEvent->dwRet = ret;
			}
			__except(Filter1(GetExceptionCode(), GetExceptionInformation()))
			{
				wprintf(L"one except has happen.\r\n");
			} 
			return 0;
		}
		else if(pXvmEvent->fn)
		{
			vl::vmlang3 * pm = (vl::vmlang3 *)xfApp::GetApp()->m_vmlang;
			if(!pm) return 0;
			/*if(!pm)
			{
				vl::vlang3 * pv =  new vl::vlang3;
				pm = new vl::vmlang3();
				pm->setVLang(pv);
				pm->setArrayMgr(pv->getArrayMgr());

				vlerror3* perr = new vlerror3();
				xfControl* pOutPut = pWin->GetControl(_T("output"));
				if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
				if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;
				perr->m_pOutPut = pOutPut;
				pv->setError(perr);

				pv->GetSource = xfUriLoader::LoadScript;
				pv->FreeSource= xfUriLoader::FreeScript;
				pv->GetSourceDT = xfUriLoader::GetScriptDT;
				pv->GetSourceVER = xfUriLoader::GetScriptVER;
			}*/

			xfControl* pOutPut = ((xfWin*)this)->GetControl(_T("output"));
			if(!pOutPut)((xfWin*)this)->GetFrameWindow()->GetControl(_T("output"));
			if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;

			int cls = (int)pXvmEvent->pThis;
			int f = pXvmEvent->fn;
			int tr = pm->getTr();
			vl::vmtuple::push(pm->getArrayMgr(),tr,vl::vmtype::classT,cls);
			vl::vmtuple::push(pm->getArrayMgr(),tr,vl::vmtype::intT,(int)pEvent);
			vl::vmtuple::push(pm->getArrayMgr(),tr,vl::vmtype::intT,lParam);
			__try
			{
				pm->call(f,0,pm->getTr(),FetchData1,pOutPut);
			}
			__except(Filter1(GetExceptionCode(), GetExceptionInformation()))
			{
				wprintf(L"one except has happen.\r\n");
			} 
			//pm->call(f,0,pm->getTr());
			return vl::vmtuple::getItem(pm->getArrayMgr(),tr,vl::vmtuple::getLength(pm->getArrayMgr(),tr) - 1)?true:false;
		}
		else if(!pXvmEvent->FEvent)
		{
			if(!pXvmEvent->pvm) pXvmEvent->pvm = xfApp::GetApp()->m_xsscp;

			code cd;
			int pc;

			memset(&cd,0,sizeof(code));
			pc = ((xsscomplier*)pXvmEvent->pvm)->m_pxvm->m_pc;
			//((xsscomplier*)pXvmEvent->pvm)->m_pxvm->m_pc = 0;
			//if(((xsscomplier*)pXvmEvent->pvm)->m_pxvm->m_bp>0) ((xsscomplier*)pXvmEvent->pvm)->m_pxvm->m_bp=0;

			if(pc!=0)
			{
				//::DebugBreak();
			}
			cd.order = XC_PUSH;
			cd.h = xa_nclass;
			cd.n = 0;
			((xsscomplier*)pXvmEvent->pvm)->m_pxvm->eval(cd);

			cd.order = XC_PUSH;
			cd.h = xa_nstruct;
			cd.n =(int)pEvent;
			((xsscomplier*)pXvmEvent->pvm)->m_pxvm->eval(cd);

			cd.order = XC_CALL;
			cd.h = 2;
			cd.p[0] = pXvmEvent->fctname;

			//int oldbp = ((xsscomplier*)pXvmEvent->pvm)->m_pxvm->m_bp;

			((xsscomplier*)pXvmEvent->pvm)->m_pxvm->eval(cd,pXvmEvent->nobj);
			//delete cd.p[0];
			((xsscomplier*)pXvmEvent->pvm)->m_pxvm->m_pc = pc;

			//if(((xsscomplier*)pXvmEvent->pvm)->m_pxvm->m_bp!=oldbp)::DebugBreak();

			dataitem d;
			d = (*((xsscomplier*)pXvmEvent->pvm)->m_pxvm->pstack)[((xsscomplier*)pXvmEvent->pvm)->m_pxvm->pstack->m_nsp];
			return d.i?true:false;
		}else
		{
			__try
			{
				int result = 0;
				void * pThis = pXvmEvent->pThis;
				void * p = pXvmEvent->FEvent;
				__asm mov ecx,pThis ;
				__asm push  lParam;
				__asm push  pEvent;
				__asm call p;
				__asm add esp,8;
				//ret = pXvmEvent->FEvent(pEvent,lParam);
				__asm mov result, eax;
				pEvent->dwRet = result;
				return result? true: false;
			}__except(Filter1(GetExceptionCode(), GetExceptionInformation()))
			{
				wprintf(L"one except has happen.\r\n");
			} 
			return false;
		}
		//return true;
	}

	bool xfAreaObject::AttachXvmEvent(LPTSTR msgid,int pObj, LPTSTR fctname)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = fctname;
		pxvmEvent->nobj = pObj;
		pxvmEvent->pThis = NULL;
		pxvmEvent->FEvent = NULL;

		return AttachListon(msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachXvmEvent(LPTSTR pCtlName, LPTSTR msgid,int pObj, LPTSTR fctname)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = fctname;
		pxvmEvent->nobj = pObj;
		pxvmEvent->pThis = NULL;
		pxvmEvent->FEvent = NULL;
		return AttachListon(pCtlName,msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachXvmEvent(HWND hWndControl,LPTSTR msgid,int pObj, LPTSTR fctname)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = fctname;
		pxvmEvent->nobj = pObj;
		pxvmEvent->pThis = NULL;
		pxvmEvent->FEvent = NULL;
		return AttachListonEx(hWndControl,msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachEvent(LPTSTR msgid,FEvent pEvt,void * pThis)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = NULL;
		pxvmEvent->nobj = NULL;
		pxvmEvent->pThis = pThis;
		pxvmEvent->FEvent = pEvt;

		return AttachListon(msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachEvent(LPTSTR pCtlName, LPTSTR msgid,FEvent pEvt,void * pThis)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = NULL;
		pxvmEvent->nobj = NULL;
		pxvmEvent->pThis = pThis;
		pxvmEvent->FEvent = pEvt;
		return AttachListon(pCtlName,msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachVmEvent(LPTSTR msgid,int pEvt,void * pThis)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = NULL;
		pxvmEvent->nobj = NULL;
		pxvmEvent->pThis = pThis;
		pxvmEvent->FEvent = NULL;
		pxvmEvent->fn = pEvt;

		return AttachListon(msgid,pxvmEvent,NULL);
	}
	bool xfAreaObject::AttachVmEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = NULL;
		pxvmEvent->nobj = NULL;
		pxvmEvent->pThis = pThis;
		pxvmEvent->FEvent = NULL;
		pxvmEvent->fn = pEvt;
		return AttachListon(pCtlName,msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachWppEvent(LPTSTR msgid,int pEvt,void * pThis)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = NULL;
		pxvmEvent->nobj = NULL;
		pxvmEvent->pThis = pThis;
		pxvmEvent->FEvent = NULL;
		pxvmEvent->fx = pEvt;

		return AttachListon(msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachWppEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis)
	{
		TXvmEvent * pxvmEvent = new TXvmEvent();
		pxvmEvent->pvm = NULL;
		pxvmEvent->fctname = NULL;
		pxvmEvent->nobj = NULL;
		pxvmEvent->pThis = pThis;
		pxvmEvent->FEvent = NULL;
		pxvmEvent->fx = pEvt;
		return AttachListon(pCtlName,msgid,pxvmEvent,NULL);
	}

	bool xfAreaObject::AttachListon(LPTSTR pCtlName,LPTSTR pMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		DWORD msgid ;
		if(!GetMssageID(pMessageID,msgid )) return false;
		return AttachListon(pCtlName,msgid,pXvmEvent,lParam);
	}

	bool xfAreaObject::AttachListonEx(HWND hWndControl,LPTSTR pMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		DWORD msgid ;
		if(!GetMssageID(pMessageID,msgid )) return false;
		return AttachListonEx(hWndControl,msgid,pXvmEvent,lParam);
	}

	bool xfAreaObject::AttachListon(LPTSTR pMessageID,TXvmEvent* pXvmEvent,LPARAM   lParam)
	{
		DWORD msgid ;
		if(!GetMssageID(pMessageID,msgid )) return false;
		return AttachListon(msgid,pXvmEvent,lParam);
	}

	bool xfAreaObject::GetMssageID(LPTSTR pMessageID,DWORD &dwMsg)
	{
		if(pMessageID[0] >='0' && pMessageID[0]<='9')
		{
			dwMsg = ::_ttol(pMessageID);
			return true;
		}
			
		if(g_mapEvent.size() < 1)
		{
			pair<wstring,DWORD> pr;
			#define XEVT(x,y,z) pr.first = x, pr.second = y, g_mapEvent.insert(pr);
			#include "XEvt.hpp"
		}
		map<wstring,DWORD>::const_iterator tr = g_mapEvent.find(pMessageID);
		if(tr==g_mapEvent.end()) return false;
		dwMsg = (*tr).second;
		return true;
	}

}}
