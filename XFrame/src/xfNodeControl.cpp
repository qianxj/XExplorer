#include "StdAfx.h"
#include "xfnode.hpp"
#include "xfNodeControl.hpp"
#include "xfControl.hpp"
#include "xuxutil.h"
#include "xframe.h"
#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame
{
	xfNodeControl::xfNodeControl(void):m_pxfControl(NULL),m_ImageIndex(0),m_ImagePos(-1),m_pLayerFrame(NULL),m_pLayerSheet(NULL),m_bShow(true),m_hBackGround(NULL)
	{
		m_LayerFlow = LayerFlow_Control;
	}

	xfNodeControl::~xfNodeControl(void)
	{
		if(m_hBackGround)DeleteObject(m_hBackGround);
		if(m_pxfControl) delete m_pxfControl;
		if(m_pLayerFrame)delete m_pLayerFrame;
		if(m_pLayerSheet)delete m_pLayerSheet;
		if(m_hBackGround)DeleteObject(m_hBackGround);

	}
	int xfNodeControl::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		IXMLDOMElement * pElement;
		VARIANT	svalex;
		pElement = xbXml::GetElement(pNodeEx);
		pElement->getAttribute(L"image",&svalex);
		if(svalex.vt==VT_BSTR && svalex.bstrVal)
		{
			LPTSTR	strValues;
			strValues = XUtil::wcsdupFromBstr(svalex.bstrVal);
			vector<LPTSTR> vals;
			XUtil::GetTokens(strValues,_T(","),vals);
			if(vals.size()>0)
			{
				LPTSTR pstrNext;
				if(vals.size() < 2)
					m_ImagePos = ::_tcstol(vals[0],&pstrNext,10);
				else
				{
					m_ImageIndex = ::_tcstol(vals[0],&pstrNext,10);
					m_ImagePos = ::_tcstol(vals[1],&pstrNext,10);
				}
			}
			if(strValues) delete strValues;
		}
		::VariantClear(&svalex);

		m_pLayerFrame = xbXml::GetStringAttribe(pElement,_T("layerframe"));
		m_pLayerSheet = xbXml::GetStringAttribe(pElement,_T("layersheet"));

		pElement->Release();


		return xfNode::ParseNode(pNodeEx,false);
	}
	int xfNodeControl::CreateNodeControl(HWND	hWnd)
	{
		if(m_pxfControl && ::GetParent(m_pxfControl->m_hWnd)==hWnd ) return 0;

		BSTR	bstrTag = NULL;
		LPTSTR	szXClass = NULL;

		_variant_t var;
		m_pElement->getAttribute(L"controlclass",&var);
		if(var.bstrVal && var.bstrVal[0] !='\0')
			szXClass = XUtil::tcsdupFromBstr(var.bstrVal);
		else
		{
			m_pElement->get_tagName(&bstrTag);
			szXClass = XUtil::tcsdupFromBstr(bstrTag);
		}
		::SysFreeString(bstrTag);

		xfControl * pControl = NULL;
		xfWin *	pWin = NULL;
		if(GetXPage()) pWin = GetXPage()->m_pWin;
		if(pWin && pWin->m_hDLL && (m_strNodeID || m_strNodeName))
		{
			if(m_strNodeName)
				pControl = pWin->CreateControl(m_strNodeName,szXClass,m_pElement);
			else
				pControl = pWin->CreateControl(m_strNodeID,szXClass,m_pElement);

		}
		if(!pControl)
		{
			xbObject *	pObject;
			xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,szXClass,pObject);
			pControl = (xfControl *)pObject;
		}else
		{
			pControl->SetClassName(NULL,szXClass);
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

		return 1;
	}

	int xfNodeControl::AdjustCtrlRect()
	{
		RECT rect = m_ContentRect;
		if(m_pxfControl )
			m_pxfControl->AdjustControlRect(rect,true);
		return 1;
	}

	int xfNodeControl::OnClose()
	{
		if(this->m_pxfControl)
			m_pxfControl->OnClose();
		return 1;
	}

	int xfNodeControl::RemoveControl()
	{
		if(this->m_pxfControl)
		{
			::DestroyWindow(this->m_pxfControl->m_hWnd);
			m_pxfControl = NULL;
		}
		return 1;
	}
	int xfNodeControl::ShowControl(bool bShow)
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
		return 1;
	}
	xfNode * xfNodeControl::GetLabelNode(bool bSelection ,LPCTSTR pLayerFrame,LPCTSTR pLayerSheet)
	{
		if(m_bSelection == bSelection && m_pLayerFrame && m_pLayerSheet && 
			::_tcsicmp(pLayerFrame,m_pLayerFrame)==0 && ::_tcsicmp(pLayerSheet,m_pLayerSheet)==0) return this;
		return NULL;
	}

	HWND xfNodeControl::GetHWND()
	{
		if(this->m_pxfControl)
			return this->m_pxfControl->GetHWND();
		else
			return xfNode::GetHWND();
	}

	int xfNodeControl::DrawNode(HDC hDC)
	{
		if(m_pxfControl && m_pxfControl->m_hWnd && m_pxfControl->m_bTransparent)
		{
			HWND hParent = ::GetParent(m_pxfControl->m_hWnd);
			if(m_hBackGround)DeleteObject(m_hBackGround);
			
			RECT rc;
			::GetWindowRect(m_pxfControl->m_hWnd,&rc);
			::ScreenToClient(hParent,(LPPOINT)&rc);
			::ScreenToClient(hParent,((LPPOINT)&rc) + 1);

			HDC hMemDC = ::CreateCompatibleDC(hDC);
			m_hBackGround = ::CreateCompatibleBitmap(hDC,rc.right - rc.left + 1,rc.bottom - rc.top + 1);
			m_hBackGround = (HBITMAP)::SelectObject(hMemDC,m_hBackGround);

			::BitBlt(hMemDC,0,0,rc.right - rc.left + 1,rc.bottom - rc.top + 1,hDC,rc.left,rc.top,SRCCOPY);

			m_hBackGround = (HBITMAP)::SelectObject(hMemDC,m_hBackGround);
			::DeleteDC(hMemDC);
		}
		xfNode::DrawNode(hDC);

		return true;
	}

	int xfNodeControl::DrawNodeEx(HDC hDC)
	{
		return xfNode::DrawNode(hDC);
	}

	int xfNodeControl::GetXml(BSTR &bstr)
	{
		bstr = NULL;
		if(this->m_pxfControl)
			return m_pxfControl->GetXml(bstr);
		else
			return NULL;
	}
}}
