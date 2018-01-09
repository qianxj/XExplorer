#include "stdafx.h"
#include "DevFrame.h"
#include "xoffice.h"
#include "devschema.h"
#include "WorkNodeDlg.h"
#include "xflowchart.h"
#include "xreport.h"
#include "devxquery.h"
#include "xcell.h"

DevFrame::DevFrame(void)
{
}

DevFrame::~DevFrame(void)
{
}

xfControl * DevFrame::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int DevFrame::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && (_tcscmp(pStrID,_T("xmShowExplorer"))==0))
	{
		xfNode * pNodeSheet = m_pXPage->m_pxfNode->GetNodeByName(_T("explorer"));
		if(pNodeSheet->m_LayerFlow & LayerFlow_UnLayer)
		{
			pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow & ~LayerFlow_UnLayer;
			pNodeSheet->ShowControl(true);
		}else
		{
			pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow  | LayerFlow_UnLayer;
			pNodeSheet->ShowControl(false);
		}
		m_pXPage->m_pxfNode->AdjustRect();
		m_pXPage->m_pxfNode->AdjustCtrlRect();
		::InvalidateRect(m_hWnd,&m_pXPage->m_pxfNode->m_AreaRect,false);

		return 1;
	}

	if(pStrID && (_tcscmp(pStrID,_T("xmShowAgent"))==0))
	{
		xfNode * pNodeSheet = m_pXPage->m_pxfNode->GetNodeByName(_T("auxiliaryarea"));
		if(pNodeSheet->m_LayerFlow & LayerFlow_UnLayer)
		{
			pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow & ~LayerFlow_UnLayer;
			pNodeSheet->ShowControl(true);
		}else
		{
			pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow  | LayerFlow_UnLayer;
			pNodeSheet->ShowControl(false);
		}
		m_pXPage->m_pxfNode->AdjustRect();
		m_pXPage->m_pxfNode->AdjustCtrlRect();
		::InvalidateRect(m_hWnd,&m_pXPage->m_pxfNode->m_AreaRect,false);

		return 1;
	}


	if(pStrID && (_tcscmp(pStrID,_T("xmSheetClose"))==0 || _tcscmp(pStrID,_T("xmFileClose"))==0))
	{
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet]; 
			pLayerSheet->m_pHwnds->erase(pLayerSheet->m_pHwnds->begin()+pLayerSheet->m_iCurSheet);
			pLayerSheet->RemoveSheet(pLayerSheet->m_iCurSheet);
			::DestroyWindow(hWnd);
		}
		return 1;
	}

	/*
	if(pStrID && (_tcscmp(pStrID,_T("xmFileSave"))==0))
	{
		return 1;
	}

	if(	pStrID && _tcscmp(pStrID,_T("xmDevComplier"))==0)
	{
		return 1;
	}*/

	return xfMdiFrame::OnXCommand(pStrID,pControl);
}

int DevFrame::PreSelectSheet(int nIndex)
{
	xcLayerSheet * pLayerSheet = (xcLayerSheet *)GetControl(_T("mdilayer"));
	if(nIndex >= pLayerSheet->m_SheetSize) return 1;
	xfWin * pWin = (xfWin *)::GetProp((*pLayerSheet->m_pHwnds)[nIndex],_T("this"));
	if(pWin )
	{
		xfControl * pDevTreeCtrl =((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
		HTREEITEM hItem;
		hItem = TreeView_GetSelection(pDevTreeCtrl->GetHWND());
		if(pWin->m_pParam && pWin->m_pParam != hItem )
		{
			TreeView_SelectItem(pDevTreeCtrl->GetHWND(),pWin->m_pParam);
		}
	}

	return 1;
}

int DevFrame::OnClose()
{
	xfControl* pOutPut = GetControl(_T("output"));
	if(xfApp::GetApp()->m_pOutPut = pOutPut)
		xfApp::GetApp()->m_pOutPut = NULL;
	return 1;
}

int DevFrame::OnOpen()
{
	xfControl* pOutPut = GetControl(_T("output"));
	if(!pOutPut) GetFrameWindow()->GetControl(_T("output"));
	if(pOutPut && !xfApp::GetApp()->m_pOutPut) xfApp::GetApp()->m_pOutPut = pOutPut;
	
	xfControl * pDevTreeCtrl = ((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
	HTREEITEM hItem = TreeView_GetRoot(pDevTreeCtrl->GetHWND());
	TreeView_Expand(pDevTreeCtrl->GetHWND(),hItem,TVE_EXPAND|TVE_EXPANDPARTIAL  );
	return 1;
}

int DevFrame::SelectSheet(int nIndex)
{
	return 1;
}

int DevFrame::AddSheet(xfWin * pWin)
{
	xcLayerSheet * pLayerSheet = (xcLayerSheet *)GetControl(_T("mdilayer"));
	if(pWin)
	{
		xfControl * pDevTreeCtrl = ((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
		if(!pLayerSheet->m_hImageList)
		{
			pLayerSheet->m_cxImage = 16;
			pLayerSheet->m_cyImage = 15;
			pLayerSheet->m_hImageList = TreeView_GetImageList(pDevTreeCtrl->GetHWND(),TVSIL_NORMAL);
		}

		TV_ITEM	tvItem;
		HTREEITEM hItem;
		hItem = TreeView_GetSelection(pDevTreeCtrl->GetHWND());
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
		tvItem.hItem = hItem;
		TreeView_GetItem(pDevTreeCtrl->GetHWND(),&tvItem);
		IXMLDOMElement * pElement = (IXMLDOMElement *)tvItem.lParam;
		_variant_t val;
		pElement->getAttribute(L"image",&val);
		pLayerSheet->m_pHwnds->push_back(pWin->m_hWnd);

		_variant_t caption;
		pElement->getAttribute(L"caption",&caption);
		if(!caption.bstrVal)
			pLayerSheet->AddSheet(pWin->m_pXPage->m_pxfNode->m_pCaption,(int)val,true);
		else
			pLayerSheet->AddSheet(caption.bstrVal,(int)val,true);
	}
	return 1;
}

