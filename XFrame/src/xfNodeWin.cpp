#include "StdAfx.h"
#include "xfnode.hpp"
#include "xfnodebox.hpp"
#include "xfNodeWin.hpp"
#include "xfWin.hpp"
#include "xfApp.hpp"
#include "xfXPage.hpp"
#include "xfUriLoader.hpp"
#include "xfConnect.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNodeWin::xfNodeWin(void):m_pXScript(NULL),m_pXDll(NULL),m_pstrGuid(NULL),m_pstrWorkGuid(NULL),m_pstrScriptUrl(NULL),m_pstrScriptClass(NULL),m_pStrUrl(NULL)
	{
		m_LayerFlow = LayerFlow_NS;
	}

	xfNodeWin::~xfNodeWin(void)
	{
		if(m_pXScript)delete m_pXScript;
		if(m_pXDll)delete m_pXDll;
		if(m_pstrGuid)delete m_pstrGuid;
		if(m_pstrWorkGuid)delete m_pstrWorkGuid;
		if(m_pStrUrl)delete m_pStrUrl;
	}
	
	int xfNodeWin::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		m_pXScript = xbXml::GetStringAttribe(pNodeEx,_T("xscript"));
		m_pXDll = xbXml::GetStringAttribe(pNodeEx,_T("xdll"));
		m_pstrGuid = xbXml::GetStringAttribe(pNodeEx,_T("guid"));
		m_pstrWorkGuid = xbXml::GetStringAttribe(pNodeEx,_T("workguid"));
		m_pstrScriptUrl = xbXml::GetStringAttribe(pNodeEx,_T("script_url"));
		m_pstrScriptClass = xbXml::GetStringAttribe(pNodeEx,_T("script_class"));
		m_pStrUrl = xbXml::GetStringAttribe(pNodeEx,_T("src"));
		if(!m_pStrUrl) m_pStrUrl = xbXml::GetStringAttribe(pNodeEx,_T("url"));

		m_pElement = pNodeEx;
		return xfNode::ParseNode(pNodeEx,ParseChild);
	}

	int xfNodeWin::CreateNodeControl(HWND	hWnd)
	{
		bool reload = 
			(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
		if(!reload) return -1;

		if(!m_pStrUrl) return xfNode::CreateNodeControl(hWnd);
		
		xfWin * pWin = NULL;
		LPTSTR pStrUrl = m_pStrUrl;
		LPTSTR pServer = GetXPage()->m_pStrUrl;
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

					pWin = xfWin::CreateWinObject(m_pElement,pServer);
					if(!pWin) pWin = new xfWin;
					if(this->m_pParam)
						pWin->m_pParam = this->m_pParam;
					if((!IsLayerNode() || m_LayerFlow & LayerFlow_Hidden))
					{
						RECT rt;
						SetRect(&rt,0,0,0,0);
						pWin->Open(pXPage,0,WS_CHILD,&rt,GetXPage()->m_pWin->GetHWND(),
							xfApp::GetApp()->GetInstance());
						if(pWin)::ShowWindow(pWin->m_hWnd,SW_HIDE);
					}else
					{
						RECT rt;
						this->GetClientRect(&rt);
						pWin->Open(pXPage,0,WS_CHILD|WS_VISIBLE,&rt,GetXPage()->m_pWin->GetHWND(),
							xfApp::GetApp()->GetInstance());
						//pWin = xfWin::LoadXPageXml(pXPage,(HWND)GetXPage()->m_pWin->GetHWND(),xfApp::GetApp()->GetInstance());
					}
				}else
					return -1;
			}else
				return -1;
		}else
		{
			if((!IsLayerNode() || m_LayerFlow & LayerFlow_Hidden))
			{
				RECT rt;
				SetRect(&rt,0,0,0,0);
				pWin = xfWin::LoadXPage(NULL,m_pStrUrl,0,WS_CHILD,&rt,GetXPage()->m_pWin->GetHWND(),
					xfApp::GetApp()->GetInstance());
				if(pWin)::ShowWindow(pWin->m_hWnd,SW_HIDE);
			}else
				pWin = xfWin::LoadXPage(NULL,m_pStrUrl,0,WS_CHILD,NULL,GetXPage()->m_pWin->GetHWND(),
					xfApp::GetApp()->GetInstance());
		}
		if(pWin)m_pxfPage =  pWin->m_pXPage;

		return 1;
	}

	int xfNodeWin::AdjustCtrlRect()
	{
		if(!m_pStrUrl) return xfNode::AdjustCtrlRect();

		RECT rect = m_ContentRect;
		if(m_pxfPage && m_pxfPage->m_pWin)
		{
			::MoveWindow(m_pxfPage->m_pWin->m_hWnd,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top +1,true);
		}
		return 1;
	}
	int xfNodeWin::RemoveControl()
	{
		if(!m_pStrUrl) return xfNode::RemoveControl();
		if(m_pxfPage && m_pxfPage->m_pWin)
		{
			::DestroyWindow(m_pxfPage->m_pWin->m_hWnd);
			m_pxfPage = NULL;
		}
		return 1;
	}
	int xfNodeWin::ShowControl(bool bShow)
	{
		if(!m_pStrUrl) return xfNode::ShowControl(bShow);

		if(m_pxfPage && m_pxfPage->m_pWin)
		{
			if(bShow)
			{
				::ShowWindow(m_pxfPage->m_pWin->m_hWnd,SW_SHOW);
			}else 
			{
				::ShowWindow(m_pxfPage->m_pWin->m_hWnd,SW_HIDE);
			}
		}
		return 1;
	}

}}
