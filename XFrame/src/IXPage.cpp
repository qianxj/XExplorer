#include "StdAfx.h"
#include "..\include\IXPage.hpp"
#include "xfAreaObject.hpp"
#include "xfXPage.hpp"
#include "xfNode.hpp"
#include "wininet.h"
#include "xfControl.hpp"
#include "xfWin.hpp"
#include "xfuriloader.hpp"
#include "itrans.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXPage::IXPage(void):m_pXPage(NULL)
	{
	}

	IXPage::~IXPage(void)
	{
	}
	bool IXPage::Load(LPCTSTR pStrXPageFile)
	{
		return m_pXPage->Load(pStrXPageFile);
	}
	bool IXPage::Load(IXMLDOMElement* pElement)
	{
		return m_pXPage->Load(pElement);
	}
	bool IXPage::LoadXPage(LPCTSTR pServer,LPCTSTR pStrUrl)
	{
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pServer,pStrUrl,strUrl,&dwSize,0);
		return m_pXPage->LoadXPage(NULL,strUrl);
	}

	bool IXPage::LoadXPage(ITrans *pTrans,LPCTSTR pStrUrl)
	{
		return LoadXPage(pTrans->GetServerUrl(),pStrUrl);
	}

	bool IXPage::LoadXPage(LPCTSTR pStrPage)
	{
		return m_pXPage->LoadXPage( pStrPage);
	}

	IXFControl * IXPage::GetControl()
	{
		if(m_pXPage->m_pControl) return (IXFControl *)m_pXPage->m_pControl->GetInterface();
		return NULL;
	}
	IXFWin *	 IXPage::GetWin()
	{
		if(m_pXPage->m_pWin) return (IXFWin *)m_pXPage->m_pWin->GetInterface(); 
		return NULL;
	}

	LPCTSTR IXPage::GetStrLocal()
	{
		return m_pXPage->m_pStrLocal;
	}
	LPCTSTR IXPage::GetUrl()
	{
		return m_pXPage->m_pStrUrl;

	}
	LPCTSTR IXPage::GetWkUrl()
	{
		return m_pXPage->m_pWkStrUrl;

	}

	int IXPage::SetStrLocal(LPCTSTR pStr)
	{
		if(m_pXPage->m_pStrLocal)delete m_pXPage->m_pStrLocal;
		if(!pStr)
			m_pXPage->m_pStrLocal = NULL;
		else
			m_pXPage->m_pStrLocal = _tcsdup(pStr);
		return 1;
	}
	int IXPage::SetUrl(LPCTSTR pStr)
	{
		if(m_pXPage->m_pStrUrl)delete m_pXPage->m_pStrUrl;
		if(!pStr)
			m_pXPage->m_pStrUrl = NULL;
		else
			m_pXPage->m_pStrUrl = _tcsdup(pStr);
		return 1;
	}
	int IXPage::SetWkUrl(LPCTSTR pStr)
	{
		if(m_pXPage->m_pWkStrUrl)delete m_pXPage->m_pWkStrUrl;
		if(!pStr)
			m_pXPage->m_pWkStrUrl = NULL;
		else
			m_pXPage->m_pWkStrUrl = _tcsdup(pStr);
		return 1;
	}

	int IXPage::SetXNode(IXNode * pNode,bool bReCeate )
	{
		return -99;
	}
	int IXPage::SetElement(IXNode * pNode,bool bReCeate )
	{
		return -99;
	}

	int IXPage::RemoveControl()
	{
		if(m_pXPage->m_pxfNode)
			m_pXPage->m_pxfNode->RemoveControl();
		return 1;
	}
	int IXPage::CreateNodeControl(HWND hWnd)
	{
		if(m_pXPage->m_pxfNode)
			return m_pXPage->m_pxfNode->CreateNodeControl(hWnd);
		return 1;

	}

	int IXPage::RemoveChild()
	{
		if(m_pXPage->m_pxfNode)
			m_pXPage->m_pxfNode->RemoveChild();
		return 1;
	}
	int IXPage::RemoveChild(IXNode * pParentNode)
	{
		if(pParentNode)
			pParentNode->m_pNode->RemoveChild();
		return 1;

	}
	IXNode * IXPage::CreateNode(IXMLDOMElement * pElement,bool setClassName)
	{
		xfNode * pNode = xfNode::CreateNode( pElement,setClassName);
		if(pNode)
			return (IXNode *)pNode->GetInterface();
		else
			return NULL;
	}

	int IXPage::TransToControl(IXNode * pNode,LPCTSTR pControlClass)
	{
		return -99;
	}

	BOOL IXPage::IsControlNode(IXNode *pNode)
	{
		return pNode->IsControlNode(pNode);
	}

	DWORD IXPage::GetLayerFlow()
	{
		return (DWORD)m_pXPage->m_pxfNode->m_LayerFlow;
	}

	int IXPage::SetLayerFlow(DWORD dwLayerFlow)
	{
		m_pXPage->m_pxfNode->m_LayerFlow = dwLayerFlow;
		return 1;
	}

	DWORD IXPage::GetLayerFlow(IXNode * pNode)
	{
		(DWORD)m_pXPage->m_pxfNode->m_LayerFlow;
		return 1;
	}

	int IXPage::SetLayerFlow(IXNode * pNode,DWORD dwLayerFlow)
	{
		m_pXPage->m_pxfNode->m_LayerFlow = dwLayerFlow;
		return 1;
	}
	
	//draw 
	int IXPage::DrawNode(HDC hDC)
	{
		return m_pXPage->m_pxfNode->DrawNode(hDC);
	}

	IXPage* IXPage::CreateInstance()
	{
		return new IXPage();
	}

	int IXPage::ReleaseInstance(IXPage* pPage)
	{
		if(pPage)delete pPage;
		return 1;
	}

}}
