#include "StdAfx.h"

#include "xfNode.hpp"
#include "xfXPage.hpp"
#include "xfwinbase.hpp"
#include "xfwin.hpp"
#include "xfnoderesource.hpp"
#include "WinInet.h"
#include "xfnodebox.hpp"
#include "xfnodecontrol.hpp"
#include "xfUriloader.hpp"
#include "IXPage.hpp"
#include "xfXPage.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfXPage::xfXPage(void):m_pxfNode(NULL),m_pWin(NULL),m_pStrLocal(NULL),m_pElement(NULL),m_pFocusNodeControl(NULL),
		m_pStrUrl(NULL),m_pWkStrUrl(NULL),m_pControl(NULL),m_pScript(NULL),m_pAgentContents(NULL)
	{
		m_pxbXml = new xbXml;
		m_pCCssStyle = new XUtil::xuCCssStyle;
		m_pStyleSheet = new XUtil::xuCssStyleSheet;
		m_pStyleSheet->m_pCssStyle = m_pCCssStyle; 
		m_pResourceMgr = new xfResourceMgr;
	}

	xfXPage::~xfXPage(void)
	{
		if(m_pAgentContents)
		{
			//for(int i=0;i<(int)m_pAgentContents->size();i++)
			for(int i=(int)m_pAgentContents->size();i>0;i--)
			{
				RemoveAgentNodeContent((*m_pAgentContents)[i - 1].pAgentNode,(*m_pAgentContents)[i - 1].pNode,false);
			}
			delete m_pAgentContents;
		}

		if(m_pxfNode)delete m_pxfNode;
		if(m_pxbXml) delete m_pxbXml;
		if(m_pCCssStyle) delete m_pCCssStyle;
		if(m_pResourceMgr) m_pResourceMgr->Release();
		if(m_pStrLocal)delete m_pStrLocal;
		if(m_pStrUrl) delete m_pStrUrl;
		if(m_pWkStrUrl) delete m_pWkStrUrl;
		if(m_pElement) m_pElement->Release();
		if(m_pScript)delete m_pScript;
		if(m_pStyleSheet) delete m_pStyleSheet;
	}

	bool xfXPage::Load(IXMLDOMElement* pElement)
	{
		if(m_pxfNode)delete m_pxfNode , m_pxfNode = NULL;
		m_pElement = pElement;
		pElement->AddRef();
		return ParsePage();
	}

	bool xfXPage::Load(LPCTSTR pStrXPageFile)
	{
		if(m_pxfNode)delete m_pxfNode , m_pxfNode = NULL;
		if(!m_pxbXml->Load(pStrXPageFile)) return false;
		return ParsePage();
	}
	bool xfXPage::LoadXPage(LPCTSTR pStrUri,LPCTSTR pStrUrl)
	{
		if(m_pxfNode)delete m_pxfNode , m_pxfNode = NULL;

		BYTE* pData = NULL;
		int nLen = 0;
		if(_tcsstr(pStrUrl,L"dev:")==pStrUrl)
		{
			if(xfUriLoader::FetchXml(pStrUri,pStrUrl,NULL, *m_pxbXml)!=1)
			{
				if(pData && nLen > 0)
					xfUriLoader::FreeData(pData);
				return false;
			}
		}else if(xfUriLoader::Load(pStrUrl, pStrUri,pData,nLen)==1)
		{
			TCHAR Path[MAX_PATH];
			xfUriLoader::GetCacheFileName(pStrUrl,Path);

			//::_tcscpy_s(m_pStrUrl,INTERNET_MAX_URL_LENGTH,pStrUrl);
			m_pStrUrl = _tcsdup(pStrUrl);
			this->Load(Path);
		}else
		{
			if(pData && nLen > 0)
				xfUriLoader::FreeData(pData);
			return false;
		}

		if(pData && nLen > 0)
			xfUriLoader::FreeData(pData);
		return true;
	}

	bool xfXPage::LoadXPage(LPCTSTR pStrPage)
	{
		if(m_pxfNode)delete m_pxfNode , m_pxfNode = NULL;
		if(!m_pxbXml->LoadXml(pStrPage)) return false;

		return ParsePage();
	}
	
	bool xfXPage::ParsePage()
	{
		if(m_pxbXml->m_pXmlDoc)
		{
			IXMLDOMNodeList *	pList;
			IXMLDOMNode *		pNode;
			IXMLDOMElement *	pElement;
			long	len = 0;


			//parseresource
			xfNodeResource*	pResource = NULL;
			if(m_pElement)
				m_pElement->getElementsByTagName(L"resource",&pList);
			else
				m_pxbXml->m_pXmlDoc->getElementsByTagName(L"resource",&pList);
			pList->get_length(&len);
			for(int i=0;i<len;i++)
			{
				pList->get_item(i,&pNode);
				pElement = m_pxbXml->GetElement(pNode);
				pResource = new xfNodeResource();
				pResource->m_pxfPage = this;
				m_pResourceMgr->AddResource(pResource);
				pResource->ParseNode(pElement);
				pElement->Release();
				pNode->Release();
			}
			pList->Release();
			
			
			//parse cssstyle
			if(m_pElement)
				m_pElement->getElementsByTagName(L"style",&pList);
			else
				m_pxbXml->m_pXmlDoc->getElementsByTagName(L"style",&pList);
			pList->get_length(&len);
			for(int i=0;i<len;i++)
			{
				BSTR bstr = NULL;
				pList->get_item(i,&pNode);
				pNode->get_text(&bstr);
				if(bstr)
				{
					this->m_pStyleSheet->ParseStyleSheet(bstr);
					::SysFreeString(bstr);
				}
				pNode->Release();
			}
			pList->Release();

			//parsenode
			if(m_pElement)
			{
				pElement = m_pElement;
				pElement->AddRef();
			}else
			{
				m_pxbXml->m_pXmlDoc->get_documentElement(&pElement);
			}

			//parse link cssstyle
			VARIANT	sval;
			if(m_pElement)
				m_pElement->getElementsByTagName(L"link",&pList);
			else
				m_pxbXml->m_pXmlDoc->getElementsByTagName(L"link",&pList);
			pList->get_length(&len);
			for(int i=0;i<len;i++)
			{
				BSTR bstr = NULL;
				pList->get_item(i,&pNode);
				((IXMLDOMElementPtr)pNode)->getAttribute(L"rel",&sval);
				if(sval.vt==VT_BSTR && sval.bstrVal )
				{
					if(_tcsicmp(L"stylesheet",sval.bstrVal)==0)
					{
						::VariantClear(&sval);
						BYTE * pData = NULL;
						int nLen = 0;
						((IXMLDOMElementPtr)pNode)->getAttribute(L"href",&sval);
						if(sval.vt==VT_BSTR && sval.bstrVal )
						{
							if(xfUriLoader::ProcessCmd(NULL,sval.bstrVal,L"",pData,nLen)==1)
							{
								if(pData)
								{
									if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
									{
										this->m_pStyleSheet->ParseStyleSheet((LPTSTR)(pData + 2));
									}else if((pData[0]==0xEF &&  pData[1]==0xBB && pData[2]==0xBF))
									{
										int _convert = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)pData+3, nLen, NULL, 0);
										TCHAR * psz = new TCHAR[_convert + 1];
										int nRet = MultiByteToWideChar(CP_UTF8, 0,(LPSTR)pData+3, nLen, psz, _convert);
										psz[_convert]='\0';
										this->m_pStyleSheet->ParseStyleSheet(psz);
										delete psz;
									}
									else if(pData[0] !=0 && pData[1]!=0) //ansi should gb2312
									{
										int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, nLen, NULL, 0);
										TCHAR * psz = new TCHAR[_convert + 1];
										int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, nLen, psz, _convert);
										psz[_convert]='\0';
										this->m_pStyleSheet->ParseStyleSheet(psz);
										delete psz;
									}else //should utf-16
									{
										this->m_pStyleSheet->ParseStyleSheet((LPTSTR)pData);
									}
								}
								if(pData)xfUriLoader::FreeData(pData);
							}
							::VariantClear(&sval);
						}
					}
					::VariantClear(&sval);
				}
				pNode->Release();
			}
			pList->Release();

			//parsenode
			if(m_pElement)
			{
				pElement = m_pElement;
				pElement->AddRef();
			}else
			{
				m_pxbXml->m_pXmlDoc->get_documentElement(&pElement);
			}

			LPTSTR pStrLocal;
			pStrLocal = xbXml::GetStringAttribe(pElement,_T("local"));
			if(pStrLocal)
			{
				if(m_pStrLocal) delete m_pStrLocal;
				m_pStrLocal = pStrLocal;
			}

			m_pxfNode = xfNode::CreateNode(pElement,true);
			m_pxfNode->m_pxfPage = this;
			m_pxfNode->ParseNode(pElement);
			pElement->Release();
		}
		return true;
	}
	int xfXPage::DoDraw(HDC hDC)
	{
		if( m_pxfNode)
			return m_pxfNode->DrawNode(hDC);
		else
			return -1;
	}

	xfNode * xfXPage::GetAgentNode(LPCTSTR pStr)
	{
		xfNode * pNode;
		pNode = m_pxfNode->GetNodeByName(pStr);
		HWND hWnd = m_pWin->m_hWnd;
		while(!pNode)
		{
FindParent: hWnd = ::GetParent(hWnd);
			if(!hWnd) break;
			xfWin * pWin = (xfWin *)::GetProp(hWnd,_T("this"));
			if(!pWin) goto FindParent;
			if(!pWin->m_pXPage || !pWin->m_pXPage->m_pxfNode) goto FindParent;
			pNode = pWin->m_pXPage->m_pxfNode->GetNodeByName(pStr);
		}
		return pNode;
	}

	xfNode* xfXPage::SetAgentNodeContent(xfNode* pAgentNode,IXMLDOMElement* pElement,xfNode * pOwnerNode)
	{
		if(!pAgentNode)
			pAgentNode = GetAgentNode();
		if(!pAgentNode) return NULL;

		xfNode * pNodeSheet = pAgentNode->GetXPage()->m_pxfNode->GetNodeByName(_T("auxiliaryarea"));
		if(pNodeSheet && (pNodeSheet->m_LayerFlow & LayerFlow_UnLayer)) return NULL;

		int k=0;
		if(!m_pAgentContents)m_pAgentContents = new vector<tagAgentContents>;
		for(k=0;k<(int)m_pAgentContents->size();k++)
		{
			if((*m_pAgentContents)[k].pNode->m_pElement == pElement)break;
		}

		xfNode * pNode;
		if(k< (int)m_pAgentContents->size())
		{
			pNode = (*m_pAgentContents)[k].pNode;
		}else
		{
			pNode = new xfNodeBox();
			pNode->m_pElement = pElement;
			pNode->m_pElement->AddRef();
			pAgentNode->CreateNodeControl(pAgentNode->GetXPage()->m_pWin->m_hWnd);
		}

		return SetAgentNodeContent(pAgentNode,pNode,pOwnerNode);
	}

	xfNode* xfXPage::SetAgentNodeContent(xfNode* pAgentNode,LPCTSTR pStrXml,xfNode * pOwnerNode)
	{
		xbXml	xml;
		xml.LoadXml(pStrXml);
		IXMLDOMElement *pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(!pElement) return NULL;

		xfNode* pNode = SetAgentNodeContent(pAgentNode,pElement,pOwnerNode);
		pElement->Release();
		return pNode;
	}

	int xfXPage::ClearAgentNodeContentEx(xfNode* pAgentNode,LPCTSTR pStrName)
	{
		if(!pAgentNode)
			pAgentNode = GetAgentNode();
		if(!pAgentNode) return 0;

		//xfNode * pNode = pAgentNode;

		/*if(pStrName)
			pNode = pAgentNode->GetNodeByName(pStrName);
		if(pNode)
			pNode->RemoveChild();
		*/

		for(int i=0;i<(int)pAgentNode->m_pChilds->size();i++)
		{
			(*pAgentNode->m_pChilds)[i]->ShowControl(false);
			(*pAgentNode->m_pChilds)[i]->m_LayerFlow |= LayerFlow_UnLayer;
		}
		pAgentNode->RemoveChild();
		pAgentNode->AdjustRect();
		pAgentNode->AdjustCtrlRect();
		::InvalidateRect(pAgentNode->GetXPage()->m_pWin->m_hWnd,&pAgentNode->m_AreaRect,false);

		return 1;
	}

	int xfXPage::ClearAgentNodeContent(xfNode* pAgentNode,LPCTSTR pStrName)
	{
		if(!pAgentNode)
			pAgentNode = GetAgentNode();
		if(!pAgentNode) return 0;

		//xfNode * pNode = pAgentNode;

		/*if(pStrName)
			pNode = pAgentNode->GetNodeByName(pStrName);
		if(pNode)
			pNode->RemoveChild();
		*/

		for(int i=0;i<(int)pAgentNode->m_pChilds->size();i++)
		{
			(*pAgentNode->m_pChilds)[i]->ShowControl(false);
			(*pAgentNode->m_pChilds)[i]->m_LayerFlow |= LayerFlow_UnLayer;
		}
		//pAgentNode->RemoveChild();
		pAgentNode->AdjustRect();
		pAgentNode->AdjustCtrlRect();
		::InvalidateRect(pAgentNode->GetXPage()->m_pWin->m_hWnd,&pAgentNode->m_AreaRect,false);

		return 1;
	}

	xfNode * xfXPage::SetAgentNodeContent(xfNode* pAgentNode,xfNode * pNode,xfNode * pOwnerNode)
	{
		if(!pAgentNode)
			pAgentNode = GetAgentNode();
		if(!pAgentNode) return pNode;

		xfNode * pNodeSheet = pAgentNode->GetXPage()->m_pxfNode->GetNodeByName(_T("auxiliaryarea"));
		if(pNodeSheet && (pNodeSheet->m_LayerFlow & LayerFlow_UnLayer)) return pNode;

		bool bfound = false;
		for(int i=0;i<(int)pAgentNode->m_pChilds->size();i++)
		{
			if((*pAgentNode->m_pChilds)[i]!=pNode)
			{
				(*pAgentNode->m_pChilds)[i]->ShowControl(false);
				(*pAgentNode->m_pChilds)[i]->m_LayerFlow |= LayerFlow_UnLayer;
			}else
			{
				(*pAgentNode->m_pChilds)[i]->m_LayerFlow &= ~(LayerFlow_UnLayer|LayerFlow_None);
				(*pAgentNode->m_pChilds)[i]->ShowControl(true);
				bfound = true;
			}
		}

		if(pNode && !bfound)
		{
			pNode->m_pParentNode = pAgentNode;
			pAgentNode->m_pChilds->push_back(pNode);

			pNode->AddRef();
			pAgentNode->AddRef();

			pNode->ParseNode(pNode->m_pElement);
			pNode->CreateNodeControl(pAgentNode->GetXPage()->m_pWin->m_hWnd);
			
			tagAgentContents agent;
			agent.pAgentNode = pAgentNode;
			agent.pNode = pNode;
			if(!m_pAgentContents)
				m_pAgentContents = new vector<tagAgentContents>;
			m_pAgentContents->push_back(agent);
		}

		pAgentNode->m_pData = pOwnerNode;

		pAgentNode->AdjustRect();
		pAgentNode->AdjustCtrlRect();
		::InvalidateRect(pAgentNode->GetXPage()->m_pWin->m_hWnd,&pAgentNode->m_AreaRect,false);

		return pNode;
	}

	int xfXPage::RemoveAgentNodeContent(xfNode* pAgentNode,xfNode * pNode)
	{
		return RemoveAgentNodeContent(pAgentNode,pNode,true);
	}

	int xfXPage::RemoveAgentNodeContent(xfNode* pAgentNode,xfNode * pNode,bool bOwner)
	{
		if(!pAgentNode)
			pAgentNode = GetAgentNode();
		if(!pAgentNode) return 1;

		for(int i=0;i<(int)pAgentNode->m_pChilds->size();i++)
		{
			if((*pAgentNode->m_pChilds)[i]==pNode)
			{
				if(bOwner)
				{
					for(int k=0;k<(int)m_pAgentContents->size();k++)
					{
						if((*m_pAgentContents)[k].pNode == pNode)
						{
							m_pAgentContents->erase(m_pAgentContents->begin() + k);
						}
					}
				}

				(*pAgentNode->m_pChilds)[i]->Release();
				pNode->RemoveChild(pNode);
				pNode->RemoveControl();
				pNode->Release(); 

				pAgentNode->m_pChilds->erase(pAgentNode->m_pChilds->begin() + i);
				if(pAgentNode->GetXPage()) ::InvalidateRect(pAgentNode->GetXPage()->m_pWin->m_hWnd,&pAgentNode->m_AreaRect,false);
				
				pAgentNode->Release();
				
				return 1;
			}
		}
		return 1;
	}

	xfNodeControl* xfXPage::GetFocusNodeControl()
	{
		return m_pFocusNodeControl;
	}

	xbObject * xfXPage::GetInterface()
	{
		if(m_pInterFace) return m_pInterFace;
		m_pInterFace = (xbObject *)new IXPage();
		((IXPage *)m_pInterFace)->m_pXPage = this;
		return m_pInterFace;
	}

	int xfXPage::AddScript(xfNodeScript * pScript)
	{
		if(!m_pScript)m_pScript = new std::vector<xfNodeScript  * >;
		m_pScript->push_back(pScript);
		return 1;
	}
	int xfXPage::RemoveScript(xfNodeScript * pNode)
	{
		std::vector<xfNodeScript  * >::iterator tr;
		for(m_pScript->begin(); tr!=m_pScript->end();tr++)
		{
			if(*tr == pNode)
			{
				m_pScript->erase(tr);
				break;
			}
		}
		return 1;
	}
}}
