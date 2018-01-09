#include "StdAfx.h"
#include "..\include\IXFControl.hpp"
#include "xfNode.hpp"
#include "xfControl.hpp"
#include "xfWin.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXFControl::IXFControl(void):m_pControl(NULL)
	{
	}

	IXFControl::~IXFControl(void)
	{
	}

	HWND IXFControl::GetHWND()
	{
		return m_pControl->GetHWND();
	}

	IXFWin* IXFControl::GetWindow()
	{
		xfWin * pWin = m_pControl->GetWin();
		return (IXFWin*)(pWin? pWin->GetInterface():NULL);
	}

	IXNode * IXFControl::GetXNode()
	{
		return (IXNode *)(m_pControl->m_pxfNode?m_pControl->m_pxfNode->GetInterface():NULL);
	}
	IXMLDOMElement * IXFControl::GetElement()
	{
		return m_pControl->m_pxfNode->m_pElement;
	}

	RECT & IXFControl::GetAreaRect()
	{
		return m_pControl->m_pxfNode->m_AreaRect;
	}

	RECT & IXFControl::GetContentRect()
	{
		return m_pControl->m_pxfNode->m_ContentRect;
	}

	int IXFControl::SetAreaRect(RECT & rc)
	{
		m_pControl->m_pxfNode->m_AreaRect=rc;
		return 1;
	}

	int IXFControl::SetContentRect(RECT & rc)
	{
		m_pControl->m_pxfNode->m_ContentRect = rc;
		return 1;
	}

	RECT  IXFControl::GetClientRect()
	{
		RECT rc;
		::GetClientRect(GetHWND(),&rc);
		return rc;
	}
	int IXFControl::Redraw()
	{
		if(::IsWindowVisible(GetHWND()))
			::InvalidateRect(GetHWND(),NULL,false);
		return 1;

	}
	int IXFControl::DoAcceptText()
	{
		return AcceptText();
	}
	int IXFControl::AddObserver(xbObserver * pObservers)
	{
		return m_pControl->AddObserver(pObservers);
	}

	int IXFControl::RemoveObserver(xbObserver * pObservers)
	{
		return m_pControl->RemoveObserver(pObservers);
	}

	int IXFControl::ClearObserver()
	{
		return m_pControl->ClearObserver();
	}

	int IXFControl::SetStyle(DWORD dwStyle)
	{
		return SetWindowLong(m_pControl->GetHWND(),GWL_STYLE,dwStyle);
	}

	int IXFControl::SetStyleEx(DWORD dwStyleEx)
	{
		return SetWindowLong(m_pControl->GetHWND(),GWL_EXSTYLE,dwStyleEx);
	}

	bool IXFControl::SetLayerFlow(LPCTSTR pFlow,bool bRedraw)
	{
		return m_pControl->SetLayerFlow(pFlow,bRedraw);
	}
	bool  IXFControl::SwitchLayer(LPTSTR pStrLayerSheet,LPTSTR pStrLayerFrame)
	{
		m_pControl->SwitchLayer( pStrLayerSheet,pStrLayerFrame);
		return true;
	}
	
	xbObject * IXFControl::QueryItem(LPTSTR pItem)
	{
		return m_pControl->QueryItem(pItem);
	}

	bool IXFControl::GetContentExtent(SIZE &sz)
	{
		sz = m_pControl->GetContentExtent();
		return true;
	}
	int IXFControl::addBindControl(UINT key, LPTSTR name, UINT ev)
	{
		return m_pControl->addBindControl(key, name, ev);
	}
	int IXFControl::fireBindControl(UINT key)
	{
		return m_pControl->fireBindControl(key);
	}
	int IXFControl::bindData(void * pcall, int vc,int tk)
	{
		return m_pControl->bindData(pcall, vc,tk);
	}

	int IXFControl::bindData(void * pcall, int vc,int tk,const wchar_t* root)
	{
		return m_pControl->bindData(pcall,vc,tk,root);
	}

	int IXFControl::bindNotifyDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data)
	{
		return m_pControl->bindNotifyDataChanged(path,row,col,data);
	}

	int IXFControl::bindNotifyInsertRow(wchar_t* path,int row)
	{
		return m_pControl->bindNotifyInsertRow(path,row);
	}

	int IXFControl::bindNotifyDeleteRow(wchar_t* path,int row)
	{
		return m_pControl->bindNotifyDeleteRow(path,row);
	}

	wchar_t* IXFControl::bindGetData(wchar_t* path, int row, wchar_t*col)
	{
		return m_pControl->bindGetData(path,row,col);
	}

	wchar_t* IXFControl::bindGetData(wchar_t* path, wchar_t* id, wchar_t*col)
	{
		return m_pControl->bindGetData(path,id,col);
	}

	int IXFControl::bindNotifyRemove()
	{
		return m_pControl->bindNotifyRemove();
	}

	int IXFControl::bindUpdateDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data)
	{
		return m_pControl->bindUpdateDataChanged(path,row,col,data);
	}

	int IXFControl::bindNotifyDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data)
	{
		return m_pControl->bindNotifyDataChanged(path,id,col,data);
	}
	int IXFControl::bindNotifyInsertRow(wchar_t* path,wchar_t* id)
	{
		return m_pControl->bindNotifyInsertRow(path,id);
	}
	int IXFControl::bindNotifyDeleteRow(wchar_t* path,wchar_t* id)
	{
		return m_pControl->bindNotifyDeleteRow(path,id);
	}

	int IXFControl::bindUpdateInsertRow(wchar_t* path,int row)
	{
		return m_pControl->bindUpdateInsertRow(path,row);
	}

	int IXFControl::bindUpdateDeleteRow(wchar_t* path,int row)
	{
		return m_pControl->bindUpdateDeleteRow(path,row);
	}
	int IXFControl::bindUpdateDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data)
	{
		return m_pControl->bindUpdateDataChanged(path,id,col,data);
	}

	int IXFControl::bindUpdateInsertRow(wchar_t* path,wchar_t* id)
	{
		return m_pControl->bindUpdateInsertRow(path,id);
	}

	int IXFControl::bindUpdateDeleteRow(wchar_t* path,wchar_t* id)
	{
		return m_pControl->bindUpdateDeleteRow(path,id);
	}
}}
