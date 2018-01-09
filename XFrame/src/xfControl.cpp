#include "StdAfx.h"
#include "xfControl.hpp"
#include "xfNodeControl.hpp"
#include "xfXPage.hpp"
#include "xfWin.hpp"
#include "xfmdiframe.hpp"
#include "ixfcontrol.hpp"
#include "xfApp.hpp"
#include "wpp.h"

namespace Hxsoft{ namespace XFrame
{
	xfControl::xfControl(void):m_pxfNode(NULL),m_bTransparent(false),
			m_dwExStyle(0),m_dwStyle(0),m_pszWinClass(NULL),
			m_bMdiClient(false),m_pObservers(NULL),m_BindControl(NULL),m_pBindData(NULL)
	{
		::SetRect(&m_DrawRect,0,0,0,0);
		
		this->AttachListon(WM_LBUTTONDOWN, (FNEvent)&xfControl::EvtLButton);
		this->AttachListon(WM_KEYDOWN,(FNEvent) &xfControl::EvtKeyDown);
	}

	xfControl::~xfControl(void)
	{
		ClearObserver();
		if(this->m_hWnd)
			::SetProp(this->m_hWnd,_T("this"),NULL);

		if(m_BindControl) delete m_BindControl;
		if(m_pBindData)
		{
			if(m_pBindData->root) delete m_pBindData->root;
			delete m_pBindData;
		}
	}

	HWND xfControl::CreateControl(LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam)
	{
		HWND hWnd = this->CreateEx(m_dwExStyle,m_dwStyle,m_pszWinClass,pszWndTitle,  rtPos, hWndParent, uID,hInstance,hMenu ,lpParam);

		if(m_pszWinClass && ::_tcsicmp(this->m_pszWinClass,_T("MDICLIENT"))==0)m_bMdiClient = true;
		if(m_bMdiClient)
		{
			xfWin * pWin = (xfWin *)::GetProp(hWndParent,_T("this"));
			if(pWin)
			{
				xfMdiFrame * pFrame = dynamic_cast<xfMdiFrame *>(pWin->GetFrameWindow());
				if(pFrame)pFrame->m_hMdiClient = hWnd;
			}
		}
		ControlCreated();  
		return hWnd;
	}
	void xfControl::Initial()
	{
	}

	void xfControl::Initial(IXMLDOMElement *pElement)
	{
	}

	int xfControl::ControlCreated()
	{
		return 1;
	}

	int xfControl::AdjustControlRect(RECT rect,bool redraw)
	{
		::MoveWindow(m_hWnd,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top +1,redraw);
		return 1;
	}

	bool xfControl::EvtLButton(TEvent * pEvent,LPARAM lPara)
	{
		SwitchLayer();
		return true;
	}

	void xfControl::SwitchLayer(LPTSTR pStrLayerSheet,LPTSTR pStrLayerFrame)
	{
		if(!m_pxfNode) return;
		if(!pStrLayerFrame) pStrLayerFrame = ((xfNodeControl *)m_pxfNode)->m_pLayerFrame;
		if(!pStrLayerSheet) pStrLayerSheet = ((xfNodeControl *)m_pxfNode)->m_pLayerSheet;

		if(pStrLayerSheet && pStrLayerFrame )
		{

			int nRet;
			nRet = GetWin()->PreSwitchLayer(this,pStrLayerSheet,pStrLayerFrame);
			if(nRet < 0) return;

			SendMessage( GetWin()->m_hWnd,WM_PRESWITCHLAYER,(WPARAM)pStrLayerSheet,(LPARAM)pStrLayerFrame);

			xfNode * pNodeFrame;
			xfNode * pNodeSheet;
			pNodeFrame = m_pxfNode->GetRootNode()->GetNodeByName(pStrLayerFrame);
			pNodeSheet = m_pxfNode->GetRootNode()->GetNodeByName(pStrLayerSheet);
			if(pNodeFrame && pNodeSheet)
			{
				int i;
				for(i=0;i< (int)pNodeFrame->m_pChilds->size();i++)
				{
					if(!((*pNodeFrame->m_pChilds)[i]->m_LayerFlow & LayerFlow_UnLayer) && (*pNodeFrame->m_pChilds)[i]->m_strNodeName)
					{
						if((*pNodeFrame->m_pChilds)[i] != pNodeSheet )
						{
							(*pNodeFrame->m_pChilds)[i]->m_LayerFlow |= LayerFlow_UnLayer;
							(*pNodeFrame->m_pChilds)[i]->ShowControl(false);

							//redraw tab label
							xfNode * pNode = this->m_pxfNode->GetRootNode()->GetLabelNode(true,
								pStrLayerFrame,(*pNodeFrame->m_pChilds)[i]->m_strNodeName);
							if(pNode)
							{
								pNode->m_bSelection = false;

								xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(pNode);
								if(pNodeControl && pNodeControl->m_pxfControl)
								{
									::InvalidateRect(m_pxfNode->GetXPage()->m_pWin->m_hWnd,
										&pNode->m_AreaRect,false);
									::InvalidateRect(pNodeControl->m_pxfControl->m_hWnd,NULL,false);
								}
								else
									::InvalidateRect(m_pxfNode->GetXPage()->m_pWin->m_hWnd,
										&pNode->m_AreaRect,false);

							}
						}
						break;
					}
				}
				if(i<(int)pNodeFrame->m_pChilds->size() && ((*pNodeFrame->m_pChilds)[i] != pNodeSheet || nRet==2))
				{
					pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow & ~LayerFlow_UnLayer;

					pNodeFrame->AdjustRect();
					pNodeFrame->AdjustCtrlRect();
					::InvalidateRect(pNodeFrame->GetXPage()->m_pWin->m_hWnd,&pNodeFrame->m_AreaRect,false);

					//redraw tab label
					m_pxfNode->m_bSelection = true;
					::InvalidateRect(m_pxfNode->GetXPage()->m_pWin->m_hWnd,
						&m_pxfNode->m_AreaRect,false);
					::InvalidateRect(m_hWnd,NULL,false);
					
					pNodeSheet->ShowControl(true);
				}
			}
			SendMessage( GetWin()->m_hWnd,WM_SWITCHLAYERED,(WPARAM)pStrLayerSheet,(LPARAM)pStrLayerFrame);
		}
	}

	xfWin* xfControl::GetWin()
	{
		return this->m_pxfNode->GetXPage()->m_pWin;
	}

	int xfControl::AddObserver(xbObserver * pObservers)
	{
		if(!m_pObservers)m_pObservers = new std::vector<xbObserver *>;
		m_pObservers->push_back(pObservers);

		return 1;
	}

	int xfControl::RemoveObserver(xbObserver * pObservers)
	{
		if(m_pObservers)
		{
			for(int i=0;i<(int)m_pObservers->size();i++)
			{
				if((*m_pObservers)[i] ==pObservers)
				{
					m_pObservers->erase(m_pObservers->begin() + i);
					break;
				}
			}
		}
		return 1;
	}

	int xfControl::ClearObserver()
	{
		if(m_pObservers) return 1;
		if(m_pObservers)
		{
			for(int i=0;i<(int)m_pObservers->size();i++)
			{
				if((*m_pObservers)[i])(*m_pObservers)[i]->RemoveBy(this);
			}
			if(m_pObservers)delete m_pObservers;
			m_pObservers = NULL;
		}

		return 1;
	}

	int xfControl::OnClose()
	{
		return 1;
	}

	xbObject * xfControl::GetInterface()
	{
		if(m_pInterFace) return m_pInterFace;
		m_pInterFace = (xbObject *)new IXFControl();
		((IXFControl *)m_pInterFace)->m_pControl = this;
		return m_pInterFace;
	}

	bool xfControl::SetLayerFlow(LPCTSTR pFlow,bool bRedraw)
	{

		GetXfNode()->m_pElement->setAttribute(L"Layerflow",(_variant_t)pFlow);
		if(GetXfNode()->m_pParentNode)
		{
			GetXfNode()->m_pParentNode->AdjustRect();
			GetXfNode()->m_pParentNode->AdjustCtrlRect();
		}
		return true;
	}

	SIZE xfControl::GetContentExtent()
	{
		SIZE sz;
		sz.cx = -1;
		sz.cy = -1;
		return sz;
	}

	xfControl::TBindData::TBindData():vc(0),tk(0),hrow(0),hcol(0),datachanged(0),insertrow(0),deleterow(0),remove(0),
	 		getdata(0),getdata1(0),datachanged1(0),insertrow1(0),deleterow1(0),rowcount(0),rowchanged(0)
	{
	}
	bool xfControl::hasBindData()
	{
		if(!m_pBindData) return false;
		return true;
	}

	int xfControl::bindData(void * pcall,int vc,int tk)
	{
		return bindData(pcall, vc,tk,NULL);
	}

	int xfControl::bindData(void * pcall,int vc,int tk,const wchar_t* root)
	{
		if(!vc) return 0;
		int cf = *(int *)vc;
		if(!cf) return 0;

		if(!m_pBindData) m_pBindData = new TBindData;
		m_pBindData->vc = vc;
		m_pBindData->tk = tk;
		m_pBindData->root = wcsdup(root);

		TBindDataCall* ptcall = (TBindDataCall*)pcall;
		m_pBindData->datachanged =   ptcall->datachanged;
		m_pBindData->datachanged1 =   ptcall->datachanged1;
		m_pBindData->insertrow = ptcall->insertrow;
		m_pBindData->deleterow = ptcall->deleterow;
		m_pBindData->insertrow1 = ptcall->insertrow1;
		m_pBindData->deleterow1 = ptcall->deleterow1;
		//m_pBindData->remove = ptcall->remove;
		m_pBindData->getdata = ptcall->getdata;
		m_pBindData->getdata1 = ptcall->getdata1;
		m_pBindData->rowcount = ptcall->rowcount;
		m_pBindData->rowchanged = ptcall->rowchanged;

		//test
		//bindGetData(L"goods", L"", L"no");

		return 1;
	}

	extern int Filter1(unsigned int code, struct _EXCEPTION_POINTERS *ep);
	int xfControl::manageCall(void * context, void * p, int fn)
	{
		int ret = 0;
		wpp::wpp_context* ptr_context = (wpp::wpp_context*)context;
		__try
		{
			ret = ptr_context->get_runtime()->call_method(ptr_context,p,CWord(fn,0));
		}
		__except(Filter1(GetExceptionCode(), GetExceptionInformation()))
		{
			wprintf(L"one except has happen.\r\n");
		} 
		return ret;
	}

	int xfControl::bindNotifyDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->datachanged) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->datachanged;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;
			
			int result = 0;
			__asm push data;
			__asm push col;
			__asm push row;
			__asm push path;
			__asm push tk;
			//this call
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,data);
		ptr_context->get_runtime()->push_arg(p,col);
		ptr_context->get_runtime()->push_arg(p,row);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
			
		return manageCall(ptr_context,p,m_pBindData->datachanged);
	}

	int xfControl::bindNotifyRowChanged(wchar_t* path,int row)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->rowchanged) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->rowchanged;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;
			
			int result = 0;
			__asm push row;
			__asm push path;
			__asm push tk;
			//this call
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,row);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
			
		return manageCall(ptr_context,p,m_pBindData->rowchanged);
	}

	int xfControl::bindNotifyDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->datachanged) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->datachanged;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			int result = 0;
			__asm push data;
			__asm push col;
			__asm push id;
			__asm push path;
			__asm push tk;
			//this call
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,data);
		ptr_context->get_runtime()->push_arg(p,col);
		ptr_context->get_runtime()->push_arg(p,id);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return manageCall(ptr_context,p,m_pBindData->datachanged);
	}

	int xfControl::bindNotifyGetRowCount(wchar_t* path)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->rowcount) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->rowcount;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			int result = 0;
			__asm push path;
			__asm push tk;
			//this call
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return manageCall(ptr_context,p,m_pBindData->rowcount);
	}

	int xfControl::bindNotifyInsertRow(wchar_t* path,int row)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->insertrow) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->insertrow;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			int result = 0;
			__asm push row;
			__asm push path;
			__asm push tk;
			//this call
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,row);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return manageCall(ptr_context,p,m_pBindData->insertrow);
	}

	int xfControl::bindNotifyInsertRow(wchar_t* path,wchar_t* id)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->insertrow1) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->insertrow1;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			int result = 0;
			__asm push id;
			__asm push path;
			__asm push tk;
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,id);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return manageCall(ptr_context,p,m_pBindData->insertrow1);
	}

	int xfControl::bindNotifyDeleteRow(wchar_t* path,int row)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->deleterow) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->deleterow;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			int result = 0;
			__asm push row;
			__asm push path;
			__asm push tk;
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax; 
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,row);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return manageCall(ptr_context,p,m_pBindData->deleterow);
	}

	int xfControl::bindNotifyDeleteRow(wchar_t* path,wchar_t* id)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->deleterow1) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->deleterow1;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			int result = 0;
			__asm push id;
			__asm push path;
			__asm push tk;
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax; 
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,id);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return manageCall(ptr_context,p,m_pBindData->deleterow1);
	}

	int xfControl::bindNotifyRemove()
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->remove) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->remove;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			int result = 0;
			__asm push tk;
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result; 
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		int ret = manageCall(ptr_context,p,m_pBindData->remove);

		if(m_pBindData)
		{
			if(m_pBindData->root)delete m_pBindData->root;
			delete m_pBindData;
		}
		return ret;
	}

	wchar_t* xfControl::bindGetData(wchar_t* path, int row, wchar_t*col)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->getdata) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->getdata;
			int tk = (int)this->GetInterface();// m_pBindData->tk;
			int th = m_pBindData->vc;

			wchar_t* result = 0;
			__asm push col;
			__asm push row;
			__asm push path;
			__asm push tk;
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result; 
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,col);
		ptr_context->get_runtime()->push_arg(p,row);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return (wchar_t*) manageCall(ptr_context,p,m_pBindData->getdata);
	}

	wchar_t* xfControl::bindGetData(wchar_t* path, wchar_t* id, wchar_t*col)
	{
		if(!m_pBindData) return 0;
		if(!m_pBindData->getdata1) return 0;

		if(m_pBindData->tk==1000)
		{
			int p = m_pBindData->getdata1;
			int tk = (int)this->GetInterface();//m_pBindData->tk;
			int th = m_pBindData->vc;

			wchar_t* result = 0;
			__asm push col;
			__asm push id;
			__asm push path;
			__asm push tk;
			//__asm push th;
			__asm mov ecx, th;
			__asm call p;
			__asm mov result,eax;
			return result;
		}

		wpp::wpp_context* ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
		void * p = ptr_context->get_runtime()->prepare_call();
		ptr_context->get_runtime()->push_arg(p,col);
		ptr_context->get_runtime()->push_arg(p,id);
		ptr_context->get_runtime()->push_arg(p,path);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->tk);
		ptr_context->get_runtime()->push_arg(p,m_pBindData->vc);
		
		return (wchar_t*) manageCall(ptr_context,p,m_pBindData->getdata1);
	}

	int xfControl::bindUpdateDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data)
	{
		return 0;
	}

	int xfControl::bindUpdateInsertRow(wchar_t* path,int row)
	{
		return 0;
	}

	int xfControl::bindUpdateDeleteRow(wchar_t* path,int row)
	{
		return 0;
	}

	int xfControl::bindUpdateDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data)
	{
		return 0;
	}

	int xfControl::bindUpdateInsertRow(wchar_t* path,wchar_t* id)
	{
		return 0;
	}

	int xfControl::bindUpdateDeleteRow(wchar_t* path,wchar_t* id)
	{
		return 0;
	}
	int xfControl::addBindControl(UINT key, LPTSTR name, UINT ev)
	{
		TBindControl bind;
		bind.key = key;
		bind.name = name;
		bind.fireEvent = ev;
		if(!m_BindControl) m_BindControl = new vector<TBindControl>; 
		m_BindControl->push_back(bind);
		return 1;
	}

	int xfControl::fireBindControl(UINT key)
	{
		if(!m_BindControl) return 1;
		int count = (int)m_BindControl->size();
		if(count == 0) return 1;
		for(int i=count - 1; i>-1; i--)
		{
			TBindControl bind = (*m_BindControl)[i];
			if(bind.key == key)
			{
				xfControl * xc = this->GetWin()->GetControl(bind.name);
				if(xc)
				{
					::PostMessage(xc->GetHWND(), bind.fireEvent, NULL, NULL);
				}
				break;
			}
		}

		return 1;
	}

	bool xfControl::EvtKeyDown(TEvent * pEvent, LPARAM lParam)
	{
		UINT nChar = pEvent->key.nChar;
		fireBindControl(nChar);
		return true;
	}

}}
