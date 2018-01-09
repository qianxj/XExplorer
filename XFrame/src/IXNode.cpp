#include "StdAfx.h"
#include "xfAreaObject.hpp"
#include "..\include\IXNode.hpp"
#include "xfNode.hpp"
#include "xfXPage.hpp"
#include "xfNodeControl.hpp"
#include "xfControl.hpp"
#include "xfwin.hpp"
#include "xfNodeLayers.hpp"
#include "IXPage.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXNode::IXNode(void):m_pNode(NULL)
	{
	}

	IXNode::~IXNode(void)
	{
	}

	LPCTSTR IXNode::GetNodeName()
	{
		return m_pNode->m_strNodeName;
	}
	LPCTSTR IXNode::GetNodeID()
	{
		return m_pNode->m_strNodeID;
	}

	int IXNode::SetNodeName(LPCTSTR pNodeName)
	{
		if(m_pNode->m_strNodeName)delete m_pNode->m_strNodeName;
		m_pNode->m_strNodeName = _tcsdup(pNodeName);
		return 1;
	}
	int IXNode::SetNodeID(LPCTSTR pNodeID)
	{
		if(m_pNode->m_strNodeID)delete m_pNode->m_strNodeID;
		m_pNode->m_strNodeID = _tcsdup(pNodeID);
		return 1;
	}

	LPCTSTR IXNode::GetCaption()
	{
		return m_pNode->m_pCaption;
	}
	int IXNode::SetCaption(LPCTSTR pCaption)
	{
		if(m_pNode->m_pCaption)delete m_pNode->m_pCaption;
		m_pNode->m_pCaption = _tcsdup(pCaption);
		return 1;
	}

	IXNode * IXNode::GetNodeByID(LPCTSTR pStrNodeID)
	{
		return (IXNode *)m_pNode->GetNodeByID(pStrNodeID)->GetInterface();
	}

	IXNode * IXNode::GetNodeByName(LPCTSTR pStrName)
	{
		return (IXNode *)m_pNode->GetNodeByName(pStrName)->GetInterface();
	}

	IXNode * IXNode::GetXNode()
	{
		return (IXNode *)m_pNode->GetInterface();
	}

	IXPage * IXNode::GetXPage()
	{
		return (IXPage *)m_pNode->GetXPage()->GetInterface();
	}

	IXMLDOMElement * IXNode::GetElement()
	{
		return m_pNode->m_pElement;
	}

	IXNode * IXNode::GetRootNode()
	{
		return (IXNode *)m_pNode->GetRootNode()->GetInterface();
	}

	BOOL IXNode::IsControlNode()
	{
		return dynamic_cast<xfNodeControl *>(m_pNode)?true:false;
	}
	BOOL IXNode::IsControlNode(IXNode *pNode)
	{
		return dynamic_cast<xfNodeControl *>(pNode->m_pNode)?true:false;
	}

	IXFControl * IXNode::GetControl()
	{
		xfNodeControl * pNode = dynamic_cast<xfNodeControl *>(m_pNode);
		if(!pNode) return NULL;
		return (IXFControl *)pNode->m_pxfControl->GetInterface();
	}

	int IXNode::SetXNode(IXNode * pNode,bool bReCeate)
	{
		return -99;
	}
	int IXNode::SetElement(IXNode * pNode,bool bReCeate)
	{
		return -99;
	}

	int IXNode::RemoveControl()
	{
		return m_pNode->RemoveControl();
	}
	int IXNode::CreateControl(HWND hWnd)
	{
		return m_pNode->CreateNodeControl(hWnd);
	}

	int IXNode::RemoveChild()
	{
		return m_pNode->RemoveChild();
	}
	int IXNode::RemoveChild(IXNode * pParentNode)
	{
		return m_pNode->RemoveChild(pParentNode->m_pNode);
	}

	IXNode * IXNode::CreateNode(IXMLDOMElement * pElement,bool setClassName )
	{
		return (IXNode *)m_pNode->CreateNode(pElement,setClassName)->GetInterface();
	}

	int IXNode::TransToControl(LPCTSTR pControlClass)
	{
		return -99;
	}
	int IXNode::TransToControl(IXNode * pNode,LPCTSTR pControlClass)
	{
		return -99;
	}

	RECT & IXNode::GetAreaRect()
	{
		return m_pNode->m_AreaRect;
	}
	RECT & IXNode::GetContentRect()
	{
		return m_pNode->m_ContentRect;
	}

	int IXNode::SetAreaRect(RECT & rc)
	{
		m_pNode->m_AreaRect = rc;
		return 1;
	}
	int IXNode::SetContentRect(RECT & rc)
	{
		m_pNode->m_ContentRect = rc;
		return 1;
	}

	//adjust or size rect
	int IXNode::AdjustRect() //adjust content rect,adjust child node or brother node
	{
		return m_pNode->AdjustRect();
	}
	int IXNode::AdjustRect(RECT rect) //adjust content rect,adjust child node or brother node
	{
		return m_pNode->AdjustRect(rect);
	}
	int IXNode::AdjustRect(int dx) //adjust content rect,adjust child node or brother node
	{
		return m_pNode->AdjustRect(dx);
	}
	int IXNode::CalcContentRect()	//calc content rect
	{
		return m_pNode->CalcContentRect();
	}

	int IXNode::CreateNodeControl(HWND	hWnd)
	{
		return m_pNode->CreateNodeControl(hWnd);
	}
	int IXNode::AdjustCtrlRect()
	{
		return m_pNode->AdjustCtrlRect();
	}
	int IXNode::SetWidthHeight(int width, int height)
	{
		int ret = m_pNode->SetWidthHeight(width, height);
		this->GetXPage()->m_pXPage->m_pxfNode->AdjustRect();
		this->GetXPage()->m_pXPage->m_pxfNode->AdjustCtrlRect();
		return ret;
	}
	int IXNode::MoveTo(LPTSTR Name)
	{
		int ret = m_pNode->MoveTo(Name);
		this->GetXPage()->m_pXPage->m_pxfNode->AdjustRect();
		this->GetXPage()->m_pXPage->m_pxfNode->AdjustCtrlRect();
		return ret;
	}
	//draw 
	int IXNode::DrawNode(HDC hDC)
	{
		return m_pNode->DrawNode(hDC);
	}

	IXNode * IXNode::GetParentNode()
	{
		return (IXNode *)m_pNode->m_pParentNode->GetInterface();
	}
	int IXNode::GetChildNodesCount()
	{
		return (int)m_pNode->m_pChilds->size();
	}

	IXNode* IXNode::GetChild(int nItem) //from 0
	{
		if(nItem >= (int)m_pNode->m_pChilds->size()) return NULL;
		return (IXNode*)(*m_pNode->m_pChilds)[nItem]->GetInterface();
	}

	IXNode* IXNode::InsertChildNode(int nIndex, LPTSTR pXml)
	{
		xbXml xml;
		xml.LoadXml(pXml);
		IXMLDOMElement * pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			IXNode * pNode = InsertChildNode(nIndex,pElement);
			pElement->Release();
			return pNode;
		}
		else
			return NULL;
	}

	IXNode* IXNode::InsertChildNode(int nIndex, IXMLDOMElement * pElement )
	{
#if  _DEBUG
		assert(pElement);
#endif
		xfNode * pNode = xfNode::CreateNode(pElement);
		pNode->m_pParentNode = m_pNode;
		pNode->ParseNode(pElement);
		pNode->CreateNodeControl(this->m_pNode->GetXPage()->m_pWin->GetHWND());
		pNode->AdjustRect();
		pNode->AdjustCtrlRect();

		if(nIndex < 0 || nIndex >= (int)m_pNode->m_pChilds->size())
			m_pNode->m_pChilds->push_back(pNode);
		else
			m_pNode->m_pChilds->insert(m_pNode->m_pChilds->begin() + nIndex,pNode);
		return (IXNode *)pNode->GetInterface();
	}

	bool IXNode::RemoveChildNode(int nIndex)
	{
		if(nIndex < 0 || nIndex >=(int) m_pNode->m_pChilds->size()) return false;
		(*m_pNode->m_pChilds)[nIndex]->RemoveChild();
		(*m_pNode->m_pChilds)[nIndex]->RemoveControl();
		(*m_pNode->m_pChilds)[nIndex]->Release();
		m_pNode->m_pChilds->erase(m_pNode->m_pChilds->begin() + nIndex);

		return true;
	}


	DWORD IXNode::GetLayerFlow()
	{
		return (DWORD)m_pNode->m_LayerFlow;
	}

	int IXNode::SetLayerFlow(DWORD dwLayerFlow)
	{
		//m_pNode->m_pElement->setAttribute(L"Layerflow",(_variant_t)pFlow);
		m_pNode->m_LayerFlow = dwLayerFlow;
		if(m_pNode->m_pParentNode)
		{
			//m_pNode->m_pParentNode->AdjustRect();
			//m_pNode->m_pParentNode->AdjustCtrlRect();
			if(dwLayerFlow & LayerFlow_LayerChild)
				m_pNode->ShowControl(true);
			else
				m_pNode->ShowControl(false);

			this->GetXPage()->m_pXPage->m_pxfNode->AdjustRect();
			this->GetXPage()->m_pXPage->m_pxfNode->AdjustCtrlRect();
		}
		return 1;
	}

	DWORD IXNode::GetLayerFlow(IXNode * pNode)
	{
		(DWORD)pNode->m_pNode->m_LayerFlow;
		return 1;
	}

	int IXNode::SetLayerFlow(IXNode * pNode,DWORD dwLayerFlow)
	{
		pNode->m_pNode->m_LayerFlow = dwLayerFlow;
		return 1;
	}

	IXNode * IXNode::CreateInstance()
	{
		return new  IXNode();
	}
	int IXNode::ReleaseInstance(IXNode* pNode)
	{
		if(pNode)delete pNode;
		return 1;
	}

	LPTSTR IXNode::GetXml()
	{
		BSTR bstr = NULL;
		m_pNode->GetXml(bstr);
		if(bstr)
		{
			LPTSTR pStr = _tcsdup(bstr);
			::SysFreeString(bstr);
			return pStr;

		}
		return NULL;
	}

	HWND IXNode::GetHWND()
	{
		return m_pNode->GetHWND();
	}
}}
