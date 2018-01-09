#include "StdAfx.h"
#include "XSheetMaint.hpp"
#include "xtabsheet.hpp"
#include "xframe.h"
#include "XOFBaseFrmSvr.hpp"
#include "XOFBaseFrm.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	struct TParam
	{
		LPTSTR pStrID;
	} ;

	XSheetMaint::XSheetMaint(void):m_bCanDestroy(true),m_pCtrl(NULL)
	{
		this->AttachListon(WM_NOTIFY,(FNEvent)&XSheetMaint::EvtNotify);
		this->AttachListon(WM_ACTIVATE,(FNEvent)&XSheetMaint::EvtActive);
	}

	XSheetMaint::~XSheetMaint(void)
	{
		this->ClearAllListonsTo();
	}

	int XSheetMaint::Create(POINT point,LPARAM lParam)
	{
		xfXPage * pPage = new xfXPage();
		pPage->m_pWin = this;
		this->m_pXPage = pPage;
		xbXml xml;
		
		IXMLDOMNode * pNode;
		IXMLDOMElement * pElement;
		TCHAR path[MAX_PATH];
		::_stprintf_s(path,MAX_PATH,_T("%s\\%s"),xfApp::GetApp()->m_pXConfigPath,_T("xoffice.xml"));
		xml.Load(path);
		xml.m_pXmlDoc->selectSingleNode(L"/xoffice/sheets/maint/box",&pNode);
		pElement = xml.GetElement(pNode);
		pNode->Release();
		pPage->Load(pElement);
		pElement->Release();

		_variant_t	var;
		int width,height;
		pElement->getAttribute(L"width",&var);
		width = var;
		pElement->getAttribute(L"height",&var);
		height = var;

		RECT	rc;
		rc.left =  point.x;
		rc.top  =  point.y - height;
		rc.right = rc.left + width;
		rc.bottom = rc.top + height;

		int scWidth,scHeight;
		scWidth = GetSystemMetrics(SM_CXSCREEN);
		scHeight = GetSystemMetrics(SM_CYSCREEN);

		if(rc.right > scWidth)
			::OffsetRect(&rc,- rc.right + scWidth - 15,0);

		this->CreateEx(WS_EX_TOOLWINDOW/*|WS_EX_TOPMOST*/,WS_DLGFRAME|WS_POPUP,XFrameWndClass,NULL,rc,(HWND)NULL,0,xfApp::GetApp()->m_hInst);
		::GetClientRect(this->m_hWnd,&rc);  

		if(pPage->m_pxfNode)
		{
			pPage->m_pxfNode->AdjustRect(rc);
			pPage->m_pxfNode->CreateNodeControl(m_hWnd);
			pPage->m_pxfNode->AdjustCtrlRect();
		}
		this->m_pParam = (LPVOID)lParam;
		OnOpen();
		::ShowWindow(m_hWnd,SW_SHOW);
		::InvalidateRect(m_hWnd,NULL,true);
		return 1;
	}

	bool XSheetMaint::EvtNotify(TEvent* pEvent,LPARAM lParam)
	{
		LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pEvent->notify.pnmh;
		if(ptvdi->hdr.code == TVN_ENDLABELEDIT)
		{
			if(ptvdi->item.pszText)
			{
				HTREEITEM hItem = ptvdi->item.hItem;
				TVITEM tvItem;
				tvItem.mask = TVIF_HANDLE|TVIF_TEXT;
				tvItem.hItem = hItem;
				tvItem.pszText = ptvdi->item.pszText;
				tvItem.cchTextMax = (int)_tcslen(tvItem.pszText) + 1;
				TreeView_SetItem(ptvdi->hdr.hwndFrom,&tvItem);
				int i;
				for(i=0;i<(int)m_hItems.size();i++)
				{
					if(m_hItems[i]==hItem)break;
				}
				CXTabSheet* pTabSheet = (CXTabSheet *)m_pParam;
				if(pTabSheet->m_pSheets[i]) delete pTabSheet->m_pSheets[i];;
				pTabSheet->m_pSheets[i] = _tcsdup(tvItem.pszText);
				pTabSheet->Redraw();
			}
		}
		if(ptvdi->hdr.code == TVN_SELCHANGED)
		{
			HWND hWnd = pEvent->notify.pnmh->hwndFrom;
			HTREEITEM hItem = ((LPNMTREEVIEW)pEvent->notify.pnmh)->itemNew.hItem;
			if(!hItem || hItem < 0) return true;

			int i;
			for(i=0;i<(int)m_hItems.size();i++)
			{
				if(m_hItems[i]==hItem)break;
			}
			if(i >=(int)m_hItems.size()) return true;

			_variant_t guid;
			CXTabSheet* pTabSheet = (CXTabSheet *)m_pParam;
			pTabSheet->m_pOfficeSvr->m_SheetsElement[i]->getAttribute(L"guid",&guid);
			if(guid.bstrVal)
			{
				::SetWindowText(GetControl(_T("sheetguid"))->m_hWnd,guid.bstrVal);
			}

		}
		return true;
	}

	bool XSheetMaint::EvtActive(TEvent* pEvent,LPARAM lParam)
	{
		if(LOWORD(pEvent->wParam)==WA_INACTIVE && m_bCanDestroy )::DestroyWindow(pEvent->hWnd);
		if(LOWORD(pEvent->wParam) !=WA_INACTIVE)m_bCanDestroy = true;
		return true;
	}

	HTREEITEM TreeMoveItem(HWND hWndTree,HTREEITEM hItem,HTREEITEM hParentItem ,HTREEITEM  hAfterItem)
	{
		TV_INSERTSTRUCT tvstruct;
		HTREEITEM hNewItem;

		// get information of the source item
		tvstruct.item.hItem = hItem;
		tvstruct.item.cchTextMax = 255;
		tvstruct.item.pszText = new TCHAR[255];
		tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE |     TVIF_IMAGE | TVIF_SELECTEDIMAGE |TVIF_PARAM||TVIF_TEXT;
		TreeView_GetItem(hWndTree,&tvstruct.item);  
	        
		//insert the item at proper location
		tvstruct.hParent = hParentItem;
		tvstruct.hInsertAfter = hAfterItem;
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT |TVIF_PARAM |TVIF_CHILDREN ;
		hNewItem = TreeView_InsertItem(hWndTree,&tvstruct);
		delete tvstruct.item.pszText;

		//now copy item data and item state.
		TreeView_SelectItem(hWndTree,hNewItem);
		//TreeView_SetItemState(hWndTree,hNewItem,TreeView_GetItemState(hWndTree,hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);
		TreeView_DeleteItem(hWndTree,hItem);
		return hNewItem;
	}

	int XSheetMaint::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
	{
		if(!pStrID) return 1;
		if(pStrID && _tcsicmp(pStrID,_T("xmEditDeleteSheet"))==0)
		{
			this->m_bCanDestroy = false;
			HTREEITEM hItem;
			int i;
			hItem = TreeView_GetSelection(m_pCtrl->m_hWnd);
			if(!hItem ) return 1;
			for(i=0;i<(int)m_hItems.size();i++)
			{
				if(m_hItems[i]==hItem)break;
			}

			CXTabSheet* pTabSheet = (CXTabSheet *)m_pParam;
			TCHAR buf[255];
			::_stprintf_s(buf,255,_T("确信删除'%s'页面"),pTabSheet->m_pSheets[i]);
			if(MessageBox(m_hWnd,buf,_T("提示"),MB_YESNO) ==IDYES )
			{
				pTabSheet->m_pOfficeSvr->m_Sheets.erase(i);
				pTabSheet->m_pOfficeSvr->m_SheetsElement.erase(i);
				pTabSheet->RemoveSheet(i);
				TreeView_DeleteItem(m_pCtrl->m_hWnd,m_hItems[i]);
				m_hItems.erase(m_hItems.begin() + i);
			}
			::SetFocus(m_pCtrl->m_hWnd);
			return 1;
		}

		if(_tcsicmp(pStrID,_T("xmEditUpSheet"))==0 || _tcsicmp(pStrID,_T("xmEditDownSheet"))==0)
		{
			bool bUpshift = false;
			if(_tcsicmp(pStrID,_T("xmEditUpSheet"))==0)bUpshift = true;
			int i;
			HTREEITEM hItem = TreeView_GetSelection(m_pCtrl->m_hWnd);
			if(!hItem ) return 1;
			for(i=0;i<(int)m_hItems.size();i++)
			{
				if(m_hItems[i]==hItem)break;
			}
			if(m_hItems.size() == 1) return 1;
			if(i==0 && bUpshift) return 1;
			if(i==m_hItems.size() - 1 && !bUpshift) return 1;

			CXTabSheet* pTabSheet = (CXTabSheet *)m_pParam;
			if(bUpshift)
			{
				HTREEITEM hItemex = m_hItems[i - 1];
				m_hItems[i - 1] = TreeMoveItem(m_pCtrl->m_hWnd,hItem,NULL,i==1?TVI_FIRST:m_hItems[i - 2]);
				m_hItems[i] = hItemex;

				LPTSTR pStr;
				pStr = pTabSheet->m_pSheets[i - 1];
				pTabSheet->m_pSheets[i - 1] = pTabSheet->m_pSheets[i];
				pTabSheet->m_pSheets[i] = pStr;

				XOfficeSheet* pSheet;
				pSheet = pTabSheet->m_pOfficeSvr->m_Sheets[i - 1];
				pTabSheet->m_pOfficeSvr->m_Sheets[i - 1] = pTabSheet->m_pOfficeSvr->m_Sheets[i];
				pTabSheet->m_pOfficeSvr->m_Sheets[i] = pSheet;

				IXMLDOMElement * pElement;
				pElement = pTabSheet->m_pOfficeSvr->m_SheetsElement[i - 1];
				pTabSheet->m_pOfficeSvr->m_SheetsElement[i - 1] = pTabSheet->m_pOfficeSvr->m_SheetsElement[i];
				pTabSheet->m_pOfficeSvr->m_SheetsElement[i] = pElement;

				
				if(i== pTabSheet->m_iCurSheet)
				{
					this->m_bCanDestroy = false;
					pTabSheet->EventSheetChanged(i - 1);
					this->m_bCanDestroy = true;
				}else
				{
					if(i - 1== pTabSheet->m_iCurSheet)pTabSheet->m_iCurSheet ++;
					pTabSheet->Redraw();
				}
			}else
			{
				HTREEITEM hItemex = m_hItems[i + 1];
				m_hItems[i + 1] = TreeMoveItem(m_pCtrl->m_hWnd,hItem,NULL,m_hItems[i + 1]);
				m_hItems[i] = hItemex;

				LPTSTR pStr;
				pStr = pTabSheet->m_pSheets[i + 1];
				pTabSheet->m_pSheets[i + 1] = pTabSheet->m_pSheets[i];
				pTabSheet->m_pSheets[i] = pStr;

				XOfficeSheet* pSheet;
				pSheet = pTabSheet->m_pOfficeSvr->m_Sheets[i + 1];
				pTabSheet->m_pOfficeSvr->m_Sheets[i + 1] = pTabSheet->m_pOfficeSvr->m_Sheets[i];
				pTabSheet->m_pOfficeSvr->m_Sheets[i] = pSheet;

				IXMLDOMElement * pElement;
				pElement = pTabSheet->m_pOfficeSvr->m_SheetsElement[i + 1];
				pTabSheet->m_pOfficeSvr->m_SheetsElement[i + 1] = pTabSheet->m_pOfficeSvr->m_SheetsElement[i];
				pTabSheet->m_pOfficeSvr->m_SheetsElement[i] = pElement;

				
				if(i== pTabSheet->m_iCurSheet)
				{
					this->m_bCanDestroy = false;
					pTabSheet->EventSheetChanged(i + 1);
					this->m_bCanDestroy = true;
				}else
				{
					if(i + 1== pTabSheet->m_iCurSheet)pTabSheet->m_iCurSheet --;
					pTabSheet->Redraw();
				}
			}

			return 1;
		}
		if(_tcsicmp(pStrID,_T("xmEditInsertSheet"))==0 || _tcsicmp(pStrID,_T("xmEditAddSheet"))==0)
		{
			this->m_bCanDestroy = false;

			xbXml xml;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			TCHAR path[MAX_PATH];
			::_stprintf_s(path,MAX_PATH,_T("%s\\%s"),xfApp::GetApp()->m_pXConfigPath,_T("xoffice.xml"));
			xml.Load(path);
			xml.m_pXmlDoc->selectSingleNode(L"/xoffice/sheets/addsheet/xframe",&pNode);
			pElement = xml.GetElement(pNode);
			pNode->Release();

			TParam param;
			param.pStrID = (LPTSTR)pStrID;

			CXTabSheet* pTabSheet = (CXTabSheet *)m_pParam;
			EnableWindow(pTabSheet->GetWin()->m_hWnd,false);
			xfWin* pWin =  xfWin::LoadXPage(pElement,m_hWnd,xfApp::GetApp()->m_hInst,&param,this);
			EnableWindow(pTabSheet->GetWin()->m_hWnd,true);
			pElement->Release();
			return 1;
		}
		
		if(_tcsicmp(pStrID,_T("xmOk"))==0)
		{
			xfWin * pDlg = pControl->m_pxfNode->GetXPage()->m_pWin;
			xfControl * pEdit = pDlg->GetControl(_T("xmSheetName"));
			int nLen = ::GetWindowTextLength(pEdit->m_hWnd);

			if(nLen < 1)
			{
				MessageBox(pDlg->m_hWnd,_T("请输入页面名称"),_T("提示"),MB_OK);
				return 1;
			}
			TCHAR* pBuf = new TCHAR[nLen + 1];
			::GetWindowText(pEdit->m_hWnd,pBuf,nLen+1);

			xfControl * pDocList = pDlg->GetControl(_T("xdoclisttree"));
			HTREEITEM hItem = TreeView_GetSelection(pDocList->m_hWnd);
			TV_ITEM	tvItem;
			if(!hItem )
			{
				MessageBox(pDlg->m_hWnd,_T("请先选择页面的类型"),_T("提示"),MB_OK);
				return 1;
			}
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pDocList->m_hWnd,&tvItem);

			LPTSTR pEditor = (LPTSTR)tvItem.lParam;
			CXTabSheet* pTabSheet = (CXTabSheet *)m_pParam;

			xbObject * pObject = NULL;
			XOfficeSheet * pSheet = NULL;
			xbXml xml;
			TCHAR xDoc[1024];
			IXMLDOMElement * pElement;
			if(_tcsicmp(pEditor,_T("xreportex"))==0)
				xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,_T("xcell"),pObject);
			else
				xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,pEditor,pObject);

			pSheet = dynamic_cast<XOfficeSheet *>(pObject);
			if(pSheet)
			{
				pSheet->m_pHScrollBar = pTabSheet->m_pOfficeSvr->pHScrollBar[0];
				pSheet->m_pVScrollBar = pTabSheet->m_pOfficeSvr->pVScrollBar[0];
				pSheet->m_hWnd = pTabSheet->m_pOfficeSvr->m_hWnd;
				pSheet->m_Standalone = false;
				
				pSheet->m_pxfNode = pTabSheet->m_pOfficeSvr->m_pOfficeFrm->m_pxfNode;
			}

			IXMLDOMNode * pNode;
			IXMLDOMElement * pElementEx;
			TCHAR path[MAX_PATH];
			TCHAR xpath[MAX_PATH];
			::_stprintf_s(path,MAX_PATH,_T("%s\\%s"),xfApp::GetApp()->m_pXConfigPath,_T("xoffice.xml"));
			xml.Load(path);
			
			::_stprintf_s(xpath,MAX_PATH,_T("/xoffice/sheets/sheetinitial/%s/sheet"),pEditor);
			xml.m_pXmlDoc->selectSingleNode(xpath,&pNode);
			if(pNode)
			{
				BSTR bstr;
				pElementEx = xml.GetElement(pNode);
				pNode->Release();

				pElementEx->setAttribute(L"caption",(_variant_t)pBuf);
				pElementEx->get_xml(&bstr);
				xml.LoadXml(bstr);
				xml.m_pXmlDoc->get_documentElement(&pElement);
				::SysFreeString(bstr);
				pElementEx->Release();
			}else
			{
				::_stprintf_s(xDoc,1024,_T("<sheet version=\"3.0\" editer=\"%s\" caption=\"%s\" />"),pEditor,pBuf);
				xml.LoadXml(xDoc);
				xml.m_pXmlDoc->get_documentElement(&pElement);
			}
			TParam* pParam = (TParam *)pDlg->m_pParam;
			if(_tcsicmp(pParam->pStrID,_T("xmEditAddSheet"))==0)
			{
				pTabSheet->m_pOfficeSvr->m_SheetsElement.push_back(pElement);
				pTabSheet->m_pOfficeSvr->m_Sheets.push_back(pSheet);
				pTabSheet->AddSheet(pBuf);

				TVINSERTSTRUCT tvi;
				tvi.hParent = NULL;
				tvi.hInsertAfter = TVI_LAST;
				tvi.itemex.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
				tvi.itemex.iImage = 0;
				tvi.itemex.iSelectedImage =0;
				tvi.itemex.pszText = pBuf;
				tvi.itemex.cchTextMax = (int)::_tcslen(pBuf)+ 1;
				hItem = TreeView_InsertItem(m_pCtrl->m_hWnd,&tvi);
				m_hItems.push_back(hItem);

			}else //insert
			{
				HTREEITEM hItem;
				int i;
				hItem = TreeView_GetSelection(m_pCtrl->m_hWnd);
				for(i=0;i<(int)m_hItems.size();i++)
				{
					if(m_hItems[i]==hItem)break;
				}
				pTabSheet->m_pOfficeSvr->m_SheetsElement.insert(i,pElement);
				pTabSheet->m_pOfficeSvr->m_Sheets.insert(i,pSheet);
				pTabSheet->InsertSheet(i,pBuf);

				TVINSERTSTRUCT tvi;
				tvi.hParent = NULL;
				if(i>0)
					tvi.hInsertAfter = m_hItems[i - 1];
				else
					tvi.hInsertAfter = TVI_FIRST;
				tvi.itemex.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
				tvi.itemex.iImage = 0;
				tvi.itemex.iSelectedImage =0;
				tvi.itemex.pszText = pBuf;
				tvi.itemex.cchTextMax = (int)::_tcslen(pBuf)+ 1;
				hItem = TreeView_InsertItem(m_pCtrl->m_hWnd,&tvi);
				m_hItems.insert(m_hItems.begin()+i,hItem);
			}

			pDlg->m_dwStatus &= ~WST_RESPONSELOOP;
			return 1;
		}
		if(_tcsicmp(pStrID,_T("xmCancel"))==0)
		{
			xfWin * pDlg = pControl->m_pxfNode->GetXPage()->m_pWin;
			pDlg->m_dwStatus &= ~WST_RESPONSELOOP;
			return 1;
		}

		return xfWin::OnXCommand(pStrID, pControl);
	}

	int XSheetMaint::OnOpen()
	{

		CXTabSheet* pTabSheet = (CXTabSheet *)m_pParam;
		m_pCtrl = ((xfNodeControl *)m_pXPage->m_pxfNode->GetNodeByName(_T("sheetmainttree")))->m_pxfControl;

		::SetWindowLong(m_pCtrl->m_hWnd,GWL_STYLE,::GetWindowLong(m_pCtrl->m_hWnd,GWL_STYLE) | TVS_EDITLABELS); 
		for(int i=0;i<pTabSheet->m_SheetSize;i++)
		{
			TVINSERTSTRUCT tvi;
			HTREEITEM hItem;
			tvi.hParent = NULL;
			tvi.hInsertAfter = TVI_LAST;
			tvi.itemex.mask = TVIF_IMAGE|TVIF_PARAM|TVIF_SELECTEDIMAGE|TVIF_TEXT;
			tvi.itemex.iImage = 0;
			tvi.itemex.iSelectedImage =0;
			tvi.itemex.pszText = pTabSheet->m_pSheets[i];
			tvi.itemex.cchTextMax = (int)::_tcslen(pTabSheet->m_pSheets[i])+ 1;
			hItem = TreeView_InsertItem(m_pCtrl->m_hWnd,&tvi);
			m_hItems.push_back(hItem);
			if(i==pTabSheet->m_iCurSheet)
				TreeView_SelectItem(m_pCtrl->m_hWnd,hItem);
		}
		::SetFocus(m_pCtrl->m_hWnd);
		::GetWindowLong(m_pCtrl->m_hWnd,GWL_STYLE);
		return 1;
	}

	int XSheetMaint::OnOpen(xfWin* pWin)
	{
		TParam* pParam = (TParam *)pWin->m_pParam;

		if(_tcsicmp(pParam->pStrID,_T("xmEditInsertSheet"))==0)
		{
			::SetWindowText(pWin->m_hWnd,_T("插入新页面"));
		}
		if(_tcsicmp(pParam->pStrID,_T("xmEditAddSheet"))==0)
		{
			::SetWindowText(pWin->m_hWnd,_T("增加新页面"));
		}
		return 1;
	}
}}}
