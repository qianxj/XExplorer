#include "StdAfx.h"
#include "xfnode.hpp"
#include "xfNodeLayers.hpp"
#include "xframe.h"

namespace Hxsoft{ namespace XFrame
{
	xfNodeLayers::xfNodeLayers(void):m_tabSize(27),m_nCurSheet(0),m_nTabArrow(taTop),m_nTabDrawStyle(tdsEclips),m_pStrFormNode(NULL),m_pFormNode(NULL)

	{
		m_LayerFlow = LayerFlow_Sheets;
	}

	xfNodeLayers::~xfNodeLayers(void)
	{
		if(m_pStrFormNode)delete m_pStrFormNode;
	}
	int xfNodeLayers::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		VARIANT	val;
		pNodeEx->getAttribute(L"tabsize",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			m_tabSize = ::wcstol(val.bstrVal,&pNextStr,10);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"tabplace",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			if(::_wcsicmp(val.bstrVal,L"top")==0)
				m_nTabArrow = taTop;
			else if(::_wcsicmp(val.bstrVal,L"bottom")==0)
				m_nTabArrow = taBottom;
			else if(::_wcsicmp(val.bstrVal,L"left")==0)
				m_nTabArrow = taLeft;
			else if(::_wcsicmp(val.bstrVal,L"right")==0)
				m_nTabArrow = taRight;

		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"tabstyle",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			if(::_wcsicmp(val.bstrVal,L"tdsRoundRect")==0)
				m_nTabDrawStyle = tdsRoundRect;
			else if(::_wcsicmp(val.bstrVal,L"tdsRoundRectEx")==0)
				m_nTabDrawStyle = tdsRoundRectEx;
			else if(::_wcsicmp(val.bstrVal,L"tdsShape")==0)
				m_nTabDrawStyle = tdsShape;
			else if(::_wcsicmp(val.bstrVal,L"tdsEclips")==0)
				m_nTabDrawStyle = tdsEclips;
			else if(::_wcsicmp(val.bstrVal,L"tdsEclipsEx")==0)
				m_nTabDrawStyle = tdsEclipsEx;
			else if(::_wcsicmp(val.bstrVal,L"tdsCanClose")==0)
				m_nTabDrawStyle = tdsCanClose;
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"currentsheet",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			LPWSTR pNextStr;
			m_nCurSheet = ::wcstol(val.bstrVal,&pNextStr,10);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"form",&val);
		if(val.vt==VT_BSTR && val.bstrVal)m_pStrFormNode = _tcsdup(val.bstrVal);
		::VariantClear(&val);

		return xfNode::ParseNode(pNodeEx,ParseChild);
	}
	int xfNodeLayers::CreateNodeControl(HWND	hWnd)
	{
		BSTR	bstrTag = NULL;
		LPTSTR	szXClass = NULL;

		szXClass = ::_tcsdup(_T("xlayersheet"));

		xfControl * pControl = NULL;
		xfWin *	pWin = GetXPage()->m_pWin;
		if(pWin && pWin->m_hDLL && m_strNodeID)
		{
			/*xfControl *  (WINAPI * fnCreateObject)(xfWin* pWin,LPTSTR pCtrlID,LPTSTR pStrClass,IXMLDOMElement *) = NULL;
			fnCreateObject = (xfControl *  (WINAPI * )(xfWin * pWin,LPTSTR pCtrlID,LPTSTR pStrClass,IXMLDOMElement *))::GetProcAddress(pWin->m_hDLL,"CreateObject");
			if(fnCreateObject)
				pControl = (xfControl *)fnCreateObject(pWin,m_strNodeID,szXClass,m_pElement);*/
			pControl =  pWin->CreateControl(m_strNodeID,szXClass,m_pElement);
		}
		if(!pControl)
		{
			xbObject *	pObject;
			xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,szXClass,pObject);
			pControl = (xfControl *)pObject;
		}
		m_pxfControl = pControl;
		if(szXClass)delete szXClass;
		if(pControl)
		{
			RECT rect;
			::SetRect(&rect,0,0,0,0);
			pControl->m_pxfNode=this;
			pControl->CreateControl(m_pCaption,rect,hWnd,0,xfApp::GetApp()->GetInstance());
 		  ::SetProp(pControl->m_hWnd,_T("this"),pControl);
		  pControl->Initial();
		}
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			(*m_pChilds)[i]->CreateNodeControl(hWnd);
		}
		return 1;
	}

	int xfNodeLayers::AdjustRect()
	{
		RECT rectex;
		::SetRect(&rectex,0,0,0,0);
		this->CalcContentRect();
		RECT rect = m_ContentRect;

		if(this->m_nTabArrow==taTop)
			rect.top += ((xfNodeLayers *)this)->m_tabSize;
		else if(this->m_nTabArrow==taBottom)
			rect.bottom -= ((xfNodeLayers *)this)->m_tabSize;
		else if(this->m_nTabArrow==taLeft)
			rect.left += ((xfNodeLayers *)this)->m_tabSize;
		else if(this->m_nTabArrow==taRight)
			rect.right -= ((xfNodeLayers *)this)->m_tabSize;

		if(!this->m_pStrFormNode)
		{
			for(int i=0;i<(int)m_pChilds->size();i++)
			{
				if(i==m_nCurSheet)
				{
					(*m_pChilds)[i]->m_AreaRect = rect;
					(*m_pChilds)[i]->CalcContentRect();
					(*m_pChilds)[i]->AdjustRect();
				}else
				{
					(*m_pChilds)[i]->m_AreaRect = rectex;
					(*m_pChilds)[i]->CalcContentRect();
					(*m_pChilds)[i]->AdjustRect();
				}
			}
		}
		return 1;
	}

	int xfNodeLayers::AdjustCtrlRect()
	{
		RECT rect = m_ContentRect;
		if(m_pxfControl)
		{
			if(this->m_nTabArrow==taTop)
				rect.bottom = rect.top + ((xfNodeLayers *)this)->m_tabSize;
			else if(this->m_nTabArrow==taBottom)
				rect.top = rect.bottom - ((xfNodeLayers *)this)->m_tabSize;
			else if(this->m_nTabArrow==taLeft)
				rect.right = rect.left + ((xfNodeLayers *)this)->m_tabSize;
			else if(this->m_nTabArrow==taRight)
				rect.left = rect.right - ((xfNodeLayers *)this)->m_tabSize;
			::MoveWindow(m_pxfControl->m_hWnd,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top + 1 ,true);
		}

		if(!this->m_pStrFormNode)
		{
			for(int i=0;i<(int)m_pChilds->size();i++)
			{
				if(i==m_nCurSheet)(*m_pChilds)[i]->AdjustCtrlRect();
			}
		}
		return 1;
	}

	int xfNodeLayers::InsertSheet(int nIndex,xfNode * pNode,bool bCreateControl)
	{
		pNode->m_pParentNode = this;
		if(nIndex < 0 || nIndex >= (int)m_pChilds->size())
		{
			m_pChilds->push_back(pNode);
			nIndex = (int)m_pChilds->size() -1;
		}
		else
			m_pChilds->insert(m_pChilds->begin() + nIndex,pNode);
		if(bCreateControl)
			pNode->CreateNodeControl(GetXPage()->m_pWin->GetHWND());
		return nIndex;

	}
	int xfNodeLayers::RemoveSheet(int nIndex)
	{
		if(nIndex < 0 || nIndex >= (int)m_pChilds->size()) return -1;
		if((*m_pChilds)[nIndex])
		{
			(*m_pChilds)[nIndex]->RemoveChild();
			(*m_pChilds)[nIndex]->RemoveControl();
			(*m_pChilds)[nIndex]->Release();
			m_pChilds->erase(m_pChilds->begin() + nIndex);

		}
		if(m_nCurSheet >= nIndex )
		{
			if(m_nCurSheet > 0) m_nCurSheet--;
			if(m_pChilds->size() > 0)
			{
				SelectSheet(m_nCurSheet,true);
			}else
				if(m_pFormNode)m_pFormNode->m_pChilds->clear();
		}
		return 0;
	}
	int xfNodeLayers::Clear()
	{
		this->RemoveChild();
		m_pChilds->clear();
		if(m_pFormNode)m_pFormNode->m_pChilds->clear();
		return 0;
	}


	int xfNodeLayers::SelectSheet(int newSheet,bool redrawcursheet)
	{
		if(newSheet <0 || newSheet >= (int)m_pChilds->size()) return 0;
		if(newSheet ==m_nCurSheet && !redrawcursheet) return 0;

		if(m_nCurSheet!= newSheet) (*m_pChilds)[m_nCurSheet]->ShowControl(false);

		this->m_nCurSheet = newSheet;
		xfNodeLayer * pNode = (xfNodeLayer *)(*m_pChilds)[newSheet];
		//pNode->CreateNodeControl(GetXPage()->m_pWin->GetHWND());
		if(!this->m_pStrFormNode)
		{
			this->AdjustRect();
			this->AdjustCtrlRect();
		}else
		{
			if(!m_pFormNode)m_pFormNode = GetRootNode()->GetNodeByName(m_pStrFormNode);
			if(m_pFormNode)
			{
				if(m_pFormNode->m_pChilds->size() < 1)
					m_pFormNode->m_pChilds->push_back((*m_pChilds)[m_nCurSheet]);
				else
				{
					(*m_pFormNode->m_pChilds)[0]->Release();
					(*m_pFormNode->m_pChilds)[0] = (*m_pChilds)[m_nCurSheet];
				}
				(*m_pChilds)[m_nCurSheet]->AddRef();
				m_pFormNode->AdjustRect();
				m_pFormNode->AdjustCtrlRect();
			}
		}
		(*m_pChilds)[m_nCurSheet]->ShowControl(true);

		return 0;
	}

	int xfNodeLayers::SelectSheet(int newSheet)
	{
		return SelectSheet(newSheet,false);
	}

	int xfNodeLayers::ShowControl(bool bShow)
	{
		if(m_pxfControl && m_pxfControl->m_hWnd)
		{
			if(m_bShow && bShow)
			{
				::ShowWindow(m_pxfControl->m_hWnd,SW_SHOW);
			}else if(!bShow)
			{
				::ShowWindow(m_pxfControl->m_hWnd,SW_HIDE);
			}
		}

		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if(i==m_nCurSheet)
			{
				if(m_bShow && bShow)
				{
					(*m_pChilds)[i]->ShowControl(bShow);
				}else if(!bShow)
				{
					(*m_pChilds)[i]->ShowControl(bShow);
				}
			}
		}
		return 1;
	}
	xfNode * xfNodeLayers::GetLabelNode(bool bSelection ,LPTSTR pLayerFrame,LPTSTR pLayerSheet)
	{
		for(int i=0;i<(int)m_pChilds->size();i++)
		{
			if(i==m_nCurSheet)
				return  (*m_pChilds)[i]->GetLabelNode(bSelection,pLayerFrame,pLayerSheet);
		}
		return NULL;
	}
}}
