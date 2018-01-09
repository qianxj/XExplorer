#include "StdAfx.h"
#include "..\include\IXFWin.hpp"
#include "xfWin.hpp"
#include "xfUriLoader.hpp"
#include "xfApp.hpp"
#include "xfNode.hpp"
#include "xfXpage.hpp"
#include "ixpage.hpp"
#include "xfControl.hpp"
#include "ITrans.hpp"
#include "xfNodeControl.hpp"

namespace Hxsoft{ namespace XFrame 
{
	IXFWin::IXFWin(void):m_pWin(NULL)
	{

	}

	IXFWin::~IXFWin(void)
	{

	}

	HWND IXFWin::GetHWND()
	{
		if(m_pWin) return m_pWin->GetHWND();
		return NULL;
	}

	IXFWin* IXFWin::GetFrameWindow()
	{
		return (IXFWin *)m_pWin->GetFrameWindow()->GetInterface();
	}
	int IXFWin::CloseWindow()
	{
		if(m_pWin && m_pWin->m_dwStatus & WST_RESPONSELOOP)
			m_pWin->m_dwStatus &= ~WST_RESPONSELOOP;
		else
		{
			PostMessage(m_pWin->GetHWND(),WM_CLOSE,0,0);
			/*
			if(m_pWin->GetHWND())
			::CloseWindow(m_pWin->GetHWND());
			if(m_pWin->GetHWND())
			{
				::PostMessage(m_pWin->GetHWND(),WM_POSTDESTROYWINDOW,0,0);
				//DestroyWindow(m_pWin->GetHWND());
			}*/
		}
		return 1;
	}
	int IXFWin::SetStyle(DWORD dwStyle)
	{
		return SetWindowLong(m_pWin->GetHWND(),GWL_STYLE,dwStyle);
	}
	int IXFWin::SetStyleEx(DWORD dwStyleEx)
	{
		return SetWindowLong(m_pWin->GetHWND(),GWL_EXSTYLE,dwStyleEx);
	}

	IXFWin* IXFWin::OpenWindow(LPCTSTR pServer,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx,DWORD dwStyle,LPCTSTR pCtrl)
	{
		if(pCtrl && pParent)
		{
			xfControl *pControl = pParent->m_pWin->GetControl(pCtrl);
			POINT ptStart,ptEnd;
			ptStart.x = pControl->m_pxfNode->m_AreaRect.left;
			ptStart.y = pControl->m_pxfNode->m_AreaRect.top;
			ClientToScreen(pParent->m_pWin->m_hWnd,&ptStart);
			ptEnd.x = pControl->m_pxfNode->m_AreaRect.right;
			ptEnd.y = pControl->m_pxfNode->m_AreaRect.bottom;
			ClientToScreen(pParent->m_pWin->m_hWnd,&ptEnd);
			RECT rt;
			::SetRect(&rt,ptStart.x,ptStart.y,ptEnd.x,ptEnd.y);
			return OpenWindow(pServer,pStrUrl,pParent,lParam,pCmdWin,dwStyleEx,dwStyle,&rt);
		}
		return OpenWindow(pServer,pStrUrl,pParent,lParam,pCmdWin,dwStyleEx,dwStyle,(LPRECT)NULL);
	}

	IXFWin* IXFWin::OpenWindow(LPCTSTR pServer,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx,DWORD dwStyle,LPRECT pRect)
	{
		xfWin * pWin;
		if(!pServer || pServer[0]==0)
		{
			if(!pStrUrl) return NULL;
			if(UrlIsFileUrl(pStrUrl))
			{
				pWin = xfWin::LoadXPage(pStrUrl,(HWND)(pParent?pParent->GetHWND():NULL),xfApp::GetApp()->GetInstance(),(LPVOID)lParam,pCmdWin?pCmdWin->m_pWin:NULL);
				return pWin?(IXFWin*)pWin->GetInterface():NULL;
			}
			else
			{
				pWin = xfWin::LoadXPage(NULL,pStrUrl,dwStyleEx,dwStyle,pRect,(HWND)(pParent?pParent->GetHWND():NULL,(LPVOID)lParam,pCmdWin?pCmdWin->m_pWin:NULL),
					xfApp::GetApp()->GetInstance());
				return pWin?(IXFWin*)pWin->GetInterface():NULL;
			}
		}
		if(!pStrUrl) return NULL;
		
		if(_tcsstr(pStrUrl,_T("uri.hxsoft.com/xaserver")) || _tcsstr(pStrUrl,_T("dev:"))==pStrUrl || _tcsstr(pStrUrl,_T("worknode:"))==pStrUrl)
		{
			xbXml xml;
			TCHAR buf[4096];
			int nRet = 0;
			if(_tcsstr(pStrUrl,_T("dev:"))==pStrUrl)
			{
				_stprintf_s(buf,4096,_T("%s/%s"),_T("uri.hxsoft.com/xaserver"),pStrUrl);
				nRet = xfUriLoader::FetchXml(pServer,buf,NULL,xml);
			}else if(_tcsstr(pStrUrl,_T("worknode:"))==pStrUrl)
			{
				_stprintf_s(buf,4096,_T("%s/%s"),_T("uri.hxsoft.com/xaserver"),pStrUrl);
				nRet = xfUriLoader::FetchXml(pServer,buf,NULL,xml);
			}else 
				nRet = xfUriLoader::FetchXml(pServer,pStrUrl,NULL,xml);
			if(nRet==1)
			{
				IXMLDOMElement * pElementEx;
				xml.m_pXmlDoc->get_documentElement((IXMLDOMElement **)&pElementEx);
				if(pElementEx)
				{
					TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
					DWORD dwSize = INTERNET_MAX_URL_LENGTH;
					xfUriLoader::UrlCombineW(pServer,_T("/business/xpage.xml"),strUrl,&dwSize,0);

					xfXPage * pXPage = new xfXPage();
					pXPage->m_pStrUrl = _tcsdup(strUrl);
					pXPage->m_pWkStrUrl = _tcsdup(pStrUrl);
					pXPage->Load(pElementEx);
					pElementEx->Release();
					
					IXMLDOMElementPtr pNodeEx ;
					if(pXPage->m_pElement)
						pNodeEx = pXPage->m_pElement;
					else
						pXPage->m_pxbXml->m_pXmlDoc->get_documentElement(&pNodeEx);

					pWin = xfWin::CreateWinObject(pNodeEx,pXPage->m_pStrUrl);
					if(!pWin) pWin = new xfWin();
					if(pWin)pWin->Open(pXPage,dwStyleEx, dwStyle, pRect,pParent?(HWND)pParent->GetHWND():NULL,(HINSTANCE)xfApp::GetApp()->GetInstance(),(LPVOID)lParam,pCmdWin?pCmdWin->m_pWin:NULL);

					//pWin = xfWin::LoadXPageXml(pXPage,(HWND)pParent?pParent->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)lParam,pCmdWin?pCmdWin->m_pWin:NULL);
					return pWin?(IXFWin*)pWin->GetInterface():NULL;
				}else
					return NULL;
			}else
				return NULL;
			/*TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(pServer,_T("/business/xpage.xml"),strUrl,&dwSize,0);
			pWin = xfWin::LoadXPage(strUrl,pStrUrl,dwStyleEx,dwStyle,(LPRECT)pRect,pParent?(HWND)pParent->GetHWND():NULL,
				(HINSTANCE)xfApp::GetApp()->GetInstance(),(LPVOID)lParam,pCmdWin?pCmdWin->m_pWin:NULL);
			return pWin?(IXFWin*)pWin->GetInterface():NULL;*/

		}else
		{
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			//xfUriLoader::UrlCombineW(pServer,_T("/business/xpage.xml"),strUrl,&dwSize,0);
			xfUriLoader::UrlCombineW(pServer,pStrUrl,strUrl,&dwSize,0);
			//pWin = xfWin::LoadXPage(NULL,pStrUrl,dwStyleEx,dwStyle,pRect,pParent?(HWND)pParent->GetHWND():NULL,
			//		xfApp::GetApp()->GetInstance());
			pWin = xfWin::LoadXPage(NULL,strUrl,dwStyleEx,dwStyle,pRect,pParent?(HWND)pParent->GetHWND():NULL,
					xfApp::GetApp()->GetInstance());
			return pWin?(IXFWin*)pWin->GetInterface():NULL;
		}
		return NULL;
	}

	IXFWin* IXFWin::OpenWindow(LPCTSTR pServer,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,DWORD dwStyleEx,DWORD dwStyle,LPRECT pRect)
	{
		return OpenWindow(pServer,pStrUrl,pParent,lParam,NULL,dwStyleEx,dwStyle,pRect);
	}

	IXFWin* IXFWin::OpenWindow(ITrans * pTrans,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx,DWORD dwStyle,LPRECT pRect)
	{
		return OpenWindow(pTrans->m_pServerUrl,pStrUrl,pParent,lParam,NULL,dwStyleEx,dwStyle,pRect);
	}
	IXFWin* IXFWin::OpenWindow(ITrans * pTrans,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,DWORD dwStyleEx,DWORD dwStyle,LPRECT pRect)
	{
		return OpenWindow(pTrans,pStrUrl,pParent,lParam,NULL,dwStyleEx,dwStyle,pRect);
	}

	IXFWin* IXFWin::OpenWindow(LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx,DWORD dwStyle,LPRECT pRect)
	{
		return OpenWindow(GetServerUrl(),pStrUrl,pParent,lParam,NULL,dwStyleEx,dwStyle,pRect);
	}
	IXFWin* IXFWin::OpenWindow(LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,DWORD dwStyleEx,DWORD dwStyle,LPRECT pRect)
	{
		return OpenWindow(GetServerUrl(),pStrUrl,pParent,lParam,NULL,dwStyleEx,dwStyle,pRect);
	}

	 IXFWin * IXFWin::OpenWindow(xbXml &xml,IXFWin* pParent,LPARAM lParam,DWORD dwStyleEx,DWORD DwStyle,LPRECT pRect)
	{
		xfXPage * pXPage = new xfXPage();
		if(pParent && pParent->m_pWin->m_pXPage)
			pXPage->m_pStrUrl = pParent->m_pWin->m_pXPage->m_pStrUrl;

		IXMLDOMElement * pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		xfWin * pWin = NULL;
		if(pElement)
		{
			pXPage->Load(pElement);
			xfWin * pWin = xfWin::LoadXPageXml(pXPage,(HWND)pParent?pParent->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)lParam);
			pElement->Release();
		}
		return pWin?(IXFWin*)pWin->GetInterface():NULL;
	}

	IXFWin* IXFWin::OpenWindow(IXMLDOMElement * pElement,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx,DWORD DwStyle,LPRECT pRect)
	{
		xfXPage * pXPage = new xfXPage();
		if(pParent && pParent->m_pWin->m_pXPage)
			pXPage->m_pStrUrl = pParent->m_pWin->m_pXPage->m_pStrUrl;
		pXPage->Load(pElement);
		xfWin * pWin = xfWin::LoadXPageXml(pXPage,(HWND)pParent?pParent->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)lParam,pCmdWin?pCmdWin->m_pWin:NULL);
		return pWin?(IXFWin*)pWin->GetInterface():NULL;
	}

	IXFWin* IXFWin::OpenWindow(IXMLDOMElement * pElement,IXFWin* pParent,LPARAM lParam,DWORD dwStyleEx,DWORD DwStyle,LPRECT pRect)
	{
		xfXPage * pXPage = new xfXPage();
		if(pParent && pParent->m_pWin->m_pXPage)
			pXPage->m_pStrUrl = pParent->m_pWin->m_pXPage->m_pStrUrl;
		pXPage->Load(pElement);
		xfWin * pWin = xfWin::LoadXPageXml(pXPage,(HWND)pParent?pParent->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)lParam);
		return pWin?(IXFWin*)pWin->GetInterface():NULL;

	}
	IXFWin* IXFWin::OpenWindow(IXPage * pPage,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx,DWORD DwStyle,LPRECT pRect)
	{
		xfWin * pWin = xfWin::LoadXPageXml(pPage->m_pXPage,(HWND)pParent?pParent->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)lParam,(xfWin *)(pCmdWin?pCmdWin->m_pWin:NULL));
		return pWin?(IXFWin*)pWin->GetInterface():NULL;

	}
	IXFWin* IXFWin::OpenWindow(IXPage * pPage,IXFWin* pParent,LPARAM lParam,DWORD dwStyleEx,DWORD DwStyle,LPRECT pRect)
	{
		xfWin * pWin = xfWin::LoadXPageXml(pPage->m_pXPage,(HWND)pParent?pParent->GetHWND():NULL,xfApp::GetApp()->GetInstance(),(LPVOID)lParam);
		return pWin?(IXFWin*)pWin->GetInterface():NULL;

	}

	IXFControl * IXFWin::GetControl(LPCTSTR pControlName)
	{
		xfControl * pControl = m_pWin->GetControl(pControlName);
		return  (IXFControl *)(pControl?pControl->GetInterface():NULL);

	}
	
	IXFControl * IXFWin::GetControl(LPCTSTR pControlName,IXNode * pNode)
	{
		xfControl * pControl = m_pWin->GetControl(pControlName,pNode->m_pNode);
		return  (IXFControl *)(pControl?pControl->GetInterface():NULL);
	}

	IXFControl * IXFWin::GetControl(LPCTSTR pControlName,bool bInsUnlayered)
	{
		xfControl * pControl = m_pWin->GetControl(pControlName,bInsUnlayered);
		return  (IXFControl *)(pControl?pControl->GetInterface():NULL);

	}

	int IXFWin::GetAttachObject()
	{
		return GetAttachObject(m_pWin->GetHWND());
	}

	int IXFWin::GetAttachObject(HWND hWnd)
	{
		return (int)GetProp(hWnd,L"AttachObject");
	}

	LPCTSTR IXFWin::GetServerUrl()
	{
		return m_pWin->m_pXPage->m_pStrUrl;
	}

	int IXFWin::Redraw()
	{
		::InvalidateRect(GetHWND(),NULL,false);
		return 1;
	}

	IXNode * IXFWin::GetAgentNode(LPCTSTR pStrName)
	{
		xfNode * pNode = m_pWin->m_pXPage->GetAgentNode(pStrName);
		return (IXNode *)(pNode?pNode->GetInterface():NULL);
	}

	IXNode* IXFWin::SetAgentNodeContent(IXNode* pAgentNode,IXMLDOMElement* pElement)
	{
		return SetAgentNodeContent(pAgentNode, pElement,NULL);
	}
	IXNode* IXFWin::SetAgentNodeContent(IXNode* pAgentNode,LPCTSTR pStrXml)
	{
		return SetAgentNodeContent(pAgentNode,pStrXml,NULL);
	}
	IXNode* IXFWin::SetAgentNodeContent(IXNode* pAgentNode,IXNode* pNode)
	{
		return SetAgentNodeContent(pAgentNode,pNode,NULL);
	}
	IXNode* IXFWin::SetAgentNodeContent(IXNode* pAgentNode,IXNode* pNode,IXNode * pOwnerNode)
	{
		xfNode * pNode1 = m_pWin->m_pXPage->SetAgentNodeContent(pAgentNode?pAgentNode->m_pNode:NULL,pNode?pNode->m_pNode:NULL,pOwnerNode?pOwnerNode->m_pNode:NULL);
		if(pNode)
			return (IXNode *)pNode1->GetInterface();
		else
			return NULL;
	}
	IXNode* IXFWin::SetAgentNodeContent(IXNode* pAgentNode,IXMLDOMElement* pElement,IXNode * pOwnerNode)
	{
		xfNode * pNode = m_pWin->m_pXPage->SetAgentNodeContent(pAgentNode?pAgentNode->m_pNode:NULL,pElement,pOwnerNode?pOwnerNode->m_pNode:NULL);
		if(pNode)
			return (IXNode *)pNode->GetInterface();
		else
			return NULL;
	}
	IXNode* IXFWin::SetAgentNodeContent(IXNode* pAgentNode,LPCTSTR pStrXml,IXNode * pOwnerNode)
	{
		xfNode * pNode = m_pWin->m_pXPage->SetAgentNodeContent(pAgentNode?pAgentNode->m_pNode:NULL,pStrXml,pOwnerNode?pOwnerNode->m_pNode:NULL);
		if(pNode)
			return (IXNode *)pNode->GetInterface();
		else
			return NULL;
	}

	int IXFWin::ClearAgentNodeContent(IXNode* pAgentNode,LPCTSTR pStrName )
	{
		return m_pWin->m_pXPage->ClearAgentNodeContent(pAgentNode?pAgentNode->m_pNode:NULL,pStrName);
	}

	int IXFWin::ClearAgentNodeContentEx(IXNode* pAgentNode,LPCTSTR pStrName )
	{
		return m_pWin->m_pXPage->ClearAgentNodeContentEx(pAgentNode?pAgentNode->m_pNode:NULL,pStrName);
	}
	
	int IXFWin::RemoveAgentNodeContent(IXNode* pAgentNode , IXNode *pNode)
	{
		return m_pWin->m_pXPage->RemoveAgentNodeContent(pAgentNode?pAgentNode->m_pNode:NULL,pNode?pNode->m_pNode:NULL);
	}

	IXNode * IXFWin::GetXNode()
	{
		xfNode * pNode = m_pWin->m_pXPage->m_pxfNode;
		return (IXNode *)(pNode?pNode->GetInterface():NULL);
	}
	IXPage * IXFWin::GetXPage()
	{
		xfXPage * pPage = m_pWin->m_pXPage;
		return (IXPage *)(pPage?pPage->GetInterface():NULL);
	}
	IXMLDOMElement * IXFWin::GetElement()
	{
		return m_pWin->m_pXPage->m_pxfNode->m_pElement;
	}

	void * IXFWin::GetParam()
	{
		return m_pWin->m_pParam ;
	}

	bool IXFWin::SetParam(void * pParam)
	{
		m_pWin->m_pParam = pParam;
		return true;
	}

	void * IXFWin::GetFrameParam()
	{
		return m_pWin->m_pFrameParam ;
	}

	bool IXFWin::SetFrameParam(void * pParam)
	{
		m_pWin->m_pFrameParam = pParam;
		return true;
	}


	bool IXFWin::AttachXvmEvent(LPTSTR msgid,int pObj, LPTSTR fctname)
	{
#if  _DEBUG
		assert(m_pWin);
#endif		
		return m_pWin->AttachXvmEvent(msgid,pObj,fctname);
	}

	bool IXFWin::AttachXvmEvent(IXFWin * pWin,LPTSTR pCtlName,LPTSTR msgid,int pObj, LPTSTR fctname)
	{
#if  _DEBUG
		assert(m_pWin);
#endif
		return pWin->AttachXvmEvent(pCtlName,msgid,pObj,fctname);
	}
	bool IXFWin::AttachXvmEvent(LPTSTR pCtlName, LPTSTR msgid,int pObj, LPTSTR fctname)
	{
#if  _DEBUG
		assert(m_pWin);
#endif
		return m_pWin->AttachXvmEvent(pCtlName,msgid,pObj,fctname);
	}

	bool IXFWin::AttachXvmEvent(HWND hWndControl,LPTSTR msgid,int pObj, LPTSTR fctname)
	{
#if  _DEBUG
		assert(m_pWin);
#endif
		return m_pWin->AttachXvmEvent(hWndControl,msgid,pObj,fctname);

	}

	bool IXFWin::AttachEvent(LPTSTR msgid,FEvent pEvt,void * pThis)
	{
#if  _DEBUG
		assert(m_pWin);
#endif
		return m_pWin->AttachEvent(msgid,pEvt,pThis);
	}
	bool IXFWin::AttachEvent(LPTSTR pCtlName, LPTSTR msgid,FEvent pEvt,void * pThis)
	{
#if  _DEBUG
		assert(m_pWin);
#endif
		return m_pWin->AttachEvent(pCtlName, msgid,pEvt,pThis);
	}

	bool IXFWin::AttachVmEvent(LPTSTR msgid,int pEvt,void * pThis)
	{
		return m_pWin->AttachVmEvent(msgid,pEvt,pThis);
	}

	bool IXFWin::AttachVmEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis)
	{
		return m_pWin->AttachVmEvent(pCtlName, msgid,pEvt,pThis);
	}

	bool IXFWin::AttachWppEvent(LPTSTR msgid,int pEvt,void * pThis)
	{
		return m_pWin->AttachWppEvent(msgid,pEvt,pThis);
	}

	bool IXFWin::AttachWppEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis)
	{
		return m_pWin->AttachWppEvent(pCtlName, msgid,pEvt,pThis);
	}

	void IXFWin::SwitchLayer(LPTSTR pStrLayerSheet,LPTSTR pStrLayerFrame)
	{
		if(pStrLayerSheet && pStrLayerFrame )
		{
			xfNode * pNodeFrame;
			xfNode * pNodeSheet;

			pNodeFrame = GetXNode()->m_pNode->GetNodeByName(pStrLayerFrame);
			pNodeSheet = GetXNode()->m_pNode->GetNodeByName(pStrLayerSheet);
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
							xfNode * pNode = GetXNode()->m_pNode->GetLabelNode(true,
								pStrLayerFrame,(*pNodeFrame->m_pChilds)[i]->m_strNodeName);
							if(pNode)
							{
								pNode->m_bSelection = false;

								xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(pNode);
								if(pNodeControl && pNodeControl->m_pxfControl)
									::InvalidateRect(pNodeControl->m_pxfControl->m_hWnd,NULL,false);
								else
									::InvalidateRect(GetXPage()->m_pXPage->m_pWin->m_hWnd,
										&pNode->m_AreaRect,false);

							}
						}
						break;
					}
				}
				if(i<(int)pNodeFrame->m_pChilds->size() && ((*pNodeFrame->m_pChilds)[i] != pNodeSheet))
				{
					pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow & ~LayerFlow_UnLayer;

					pNodeFrame->AdjustRect();
					pNodeFrame->AdjustCtrlRect();
					::InvalidateRect(pNodeFrame->GetXPage()->m_pWin->m_hWnd,&pNodeFrame->m_AreaRect,false);

					pNodeSheet->ShowControl(true);
				}
			}
		}

	}

	IXFWin* IXFWin::CreateInstance()
	{
		return new IXFWin();
	}
	int IXFWin::ReleaseInstance(IXFWin* pWin)
	{
		if(pWin)delete pWin;
		return 1;
	}
}}
