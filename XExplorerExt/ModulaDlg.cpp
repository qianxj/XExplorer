#include "StdAfx.h"
#include "ModulaDlg.h"
#include "xcell.h"
#include "xoffice.h"
#include "entitylist.h"
#include "xflowchart.h"


CModulaDlg::CModulaDlg(void):m_pStrUrl(NULL),m_pWorkNodeType(NULL)
{
	this->AttachListon(WM_NOTIFY,(FNEvent)&CModulaDlg::EvtNotify);
	this->AttachListon(WM_ACTIVATE,(FNEvent)&CModulaDlg::EvtActive);
}

CModulaDlg::~CModulaDlg(void)
{
	if(m_pStrUrl)delete m_pStrUrl;
	this->ClearAllListonsTo();
	if(m_pWorkNodeType)delete m_pWorkNodeType;
}

int CModulaDlg::Create(LPTSTR pUrl,RECT bound,LPARAM lParam)
{
	xfXPage * pPage = new xfXPage();
	pPage->m_pWin = this;
	this->m_pXPage = pPage;

	m_pStrUrl = ::_tcsdup(pUrl);
	if(!pPage->LoadXPage(NULL,pUrl))
	{
		return -1;
		delete pPage;
		this->m_pXPage = NULL;
	}

	this->Open(pPage,WS_EX_TOOLWINDOW|/*WS_EX_TOPMOST|*/WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&bound,NULL,NULL,(LPVOID)lParam);
	return 1;
}

bool CModulaDlg::EvtActive(TEvent* pEvent,LPARAM lParam)
{
	if(LOWORD(pEvent->wParam)==WA_INACTIVE && m_bCanDestroy )::DestroyWindow(pEvent->hWnd);
	if(LOWORD(pEvent->wParam) !=WA_INACTIVE)m_bCanDestroy = true;
	return true;
}

int CModulaDlg::OnOpen()
{
	xfControl* pSchema = GetControl(_T("schemaview"));
	ListView_SetImageList(pSchema->m_hWnd,m_pXPage->m_pResourceMgr->m_hImageLists[0],LVSIL_NORMAL);

	HTREEITEM hItem;
	xfControl * pTree = GetControl(_T("DevExplorer1"));
	hItem = TreeView_GetRoot(pTree->m_hWnd);
	TreeView_Expand(pTree->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL);
	TreeView_SelectItem(pTree->m_hWnd,hItem);

	pTree = GetControl(_T("DevExplorer2"));
	hItem = TreeView_GetRoot(pTree->m_hWnd);
	TreeView_Expand(pTree->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL);
	TreeView_SelectItem(pTree->m_hWnd,hItem);

	return 1;
}

int CModulaDlg::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmAdjustFavorites"))==0 )
	{
		this->m_bCanDestroy = false;
		::EnableWindow(m_hWnd,false);

		xfWin* pWin = xfWin::LoadXPage(NULL,_T("/developer/AdjustFavoritesDlg.xml"),
			WS_EX_TOPMOST,0,NULL,this->GetFrameWindow()->m_hWnd,xfApp::GetApp()->m_hInst);

		::EnableWindow(m_hWnd,true);
		m_bCanDestroy = true;
		::SetActiveWindow(m_hWnd);
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmWorkFlowsStart"))==0 )
	{

		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		

		xfControl * pTree = GetControl(_T("DevExplorer2"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
		if(hItem)
		{
			TVITEM tvItem;
			tvItem.hItem = hItem;
			tvItem.mask = TVIF_PARAM|TVIF_CHILDREN ;
			TreeView_GetItem(pTree->m_hWnd,&tvItem);
			
			IXMLDOMElement * pElement;
			pElement = (IXMLDOMElement *)tvItem.lParam;

			BSTR bstr;
			pElement->get_tagName(&bstr);
			if(_tcsicmp(bstr,_T("file"))==0)
			{
				_variant_t var;
				pElement->getAttribute(L"guid",&var);

				TEntityParam* pParam = new TEntityParam;
				_tcscpy_s(pParam->pEntity,255,var.bstrVal);

				pElement->getAttribute(L"caption",&var);
				_stprintf_s(pParam->pWorkNodePath,255,_T("<<@编号>> %s"),var.bstrVal);

				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("/developer/taskdlg.xml"),strUrl,&dwSize,0);
				xfApp::GetApp()->m_pFrame = this->GetFrameWindow();
				xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,this->GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),pParam);

				::DestroyWindow(m_hWnd);
				::SetActiveWindow(pWinEx->m_hWnd);
			}
			::SysFreeString(bstr);
		}

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}
	

	if(pStrID && _tcsicmp(pStrID,_T("xmImportWorkFlows"))==0 )
	{
		this->m_bCanDestroy = false;

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(this->m_pParentWin->m_pXPage->m_pStrUrl,_T("/developer/workflowselectdlg.xpage"),strUrl,&dwSize,0);

		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);

		m_bCanDestroy = true;
		::SetActiveWindow(m_hWnd);
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmOKWorkFlow"))==0 )
	{
		Hxsoft::XFrame::xfControl * pTreeEx = pControl->GetWin()->GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTreeEx->m_hWnd);
		if(!TreeView_GetParent(pTreeEx->m_hWnd,hItem))
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择工作流对象"),_T("提示"),MB_OK);
			return 1;
		}
		IXMLDOMElement * pElement;
		TVITEM	item;
		item.hItem = hItem;
		item.mask = TVIF_PARAM;
		TreeView_GetItem(pTreeEx->m_hWnd,&item);
		pElement = (IXMLDOMElement *)item.lParam;
		_variant_t	var;
		_variant_t	var1;
		pElement->getAttribute(L"type",&var);
		if(_tcsicmp(var.bstrVal,_T("worklist"))!=0)
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择工作流对象"),_T("提示"),MB_OK);
			return 1;
		}

		pElement->getAttribute(L"caption",&var);
		pElement->getAttribute(L"guid",&var1);


		xfControl * pTree = GetControl(_T("DevExplorer2"));
		hItem = TreeView_GetSelection(pTree->m_hWnd);

		bool bRoot = false;
		if(!TreeView_GetParent(pTree->m_hWnd,hItem))  bRoot = true;

		TVITEM tvItem;
		tvItem.hItem = hItem;
		tvItem.mask = TVIF_PARAM|TVIF_CHILDREN ;
		TreeView_GetItem(pTree->m_hWnd,&tvItem);

		BSTR bstr ;
		while(true)
		{
			if(!TreeView_GetParent(pTree->m_hWnd,hItem))  bRoot = true;
			if(bRoot)break;

			pElement = (IXMLDOMElement *)tvItem.lParam;
			pElement->get_tagName(&bstr);
			if(_tcsicmp(bstr,_T("folder"))==0)
			{
				::SysFreeString(bstr);
				break;
			}
			::SysFreeString(bstr);
			hItem = TreeView_GetParent(pTree->m_hWnd,hItem);
			if(!hItem)return 1;
			tvItem.hItem = hItem;
			tvItem.mask = TVIF_PARAM|TVIF_CHILDREN ;
			TreeView_GetItem(pTree->m_hWnd,&tvItem);
		}

		TCHAR strCmd[512];
		if(!bRoot)
		{
			_variant_t fvar;
			pElement = (IXMLDOMElement *)tvItem.lParam;
			pElement->getAttribute(L"guid",&fvar);
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:workflow/add %s %s"),fvar.bstrVal,var1.bstrVal);
		}else
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:workflow/add * %s"),var1.bstrVal);
		
		xbXml xml;
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,var.bstrVal,xml)==1)
		{
			tvItem.cChildren = true;
			TreeView_SetItem(pTree->m_hWnd,&tvItem);

			xml.m_pXmlDoc->get_documentElement(&pElement);
			TVINSERTSTRUCT tvins;
			tvins.hInsertAfter = TVI_LAST;
			tvins.hParent = hItem;
			tvins.item.pszText = var.bstrVal;
			tvins.item.cchTextMax = 255;
			tvins.item.lParam = (LPARAM)pElement;
			tvins.item.iImage = 20;
			tvins.item.iSelectedImage = 20;
			tvins.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			TreeView_InsertItem(pTree->m_hWnd,&tvins);

			TreeView_Expand(pTree->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL  );
		}
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmCancelFlow"))==0 )
	{
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmAddToFavorites"))==0 )
	{
		xfControl * pView = GetControl(_T("worknodeview"));
		int nIndex = ListView_GetNextItem(pView->m_hWnd,-1,LVNI_SELECTED);
		if(nIndex <0) return 1;
		if(!this->m_pWorkNodeType) return 1;

		this->m_bCanDestroy = false;
		::EnableWindow(m_hWnd,false);
		
		xbXml xml;
		xml.LoadXml(_T("<param/>"));
		
		IXMLDOMElement* pElement;
		IXMLDOMElement* pItemElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(this->m_pWorkNodeType)
			pElement->setAttribute(L"nodetype",(_variant_t)this->m_pWorkNodeType);
		
		LVITEM lvitem;
		TCHAR buf[255];
		LPTSTR pEntryGuid;
		lvitem.mask = LVIF_PARAM|LVIF_TEXT;
		lvitem.iItem = nIndex;
		lvitem.iSubItem = 0;
		lvitem.cchTextMax = 255;
		lvitem.pszText = buf;
		ListView_GetItem(pView->m_hWnd,&lvitem);
		pEntryGuid = (LPTSTR)lvitem.lParam;
		
		pElement->setAttribute(L"guid",(_variant_t)pEntryGuid);
		pElement->setAttribute(L"caption",(_variant_t)lvitem.pszText);
		
		xfControl* pSchema = GetControl(_T("schemaview"));
		int nLen = ListView_GetItemCount(pSchema->m_hWnd);

		for(int i=0;i<nLen;i++)
		{
			xml.m_pXmlDoc->createElement(L"item",&pItemElement);

			lvitem.mask = LVIF_PARAM|LVIF_TEXT;
			lvitem.iItem = i;
			lvitem.iSubItem = 0;
			lvitem.cchTextMax = 255;
			lvitem.pszText = buf;
			ListView_GetItem(pSchema->m_hWnd,&lvitem);
			pItemElement->setAttribute(L"caption",(_variant_t)lvitem.pszText);

			pElement->appendChild(pItemElement,NULL);
			pItemElement->Release();
		}

		xfWin* pWin = xfWin::LoadXPage(NULL,_T("/developer/FavoritesDlg.xml"),
			WS_EX_TOPMOST,0,NULL,this->GetFrameWindow()->m_hWnd,xfApp::GetApp()->m_hInst,pElement);
		pElement->Release();

		::EnableWindow(m_hWnd,true);
		m_bCanDestroy = true;
		::SetActiveWindow(m_hWnd);

		return 1;
	}

	if(pStrID && (_tcsicmp(pStrID,_T("xmEntryNew"))==0 || _tcsicmp(pStrID,_T("xmOperateOpen"))==0))
	{
		xfControl * pTree = GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);

		LPTSTR pPackage = NULL;
		LPTSTR pWorkGuid = NULL;
		LPTSTR pEntryGuid = NULL;
		LPTSTR pItemGuid = NULL;
		LPTSTR pStrXPage = NULL;
		LPTSTR pPackageEx = NULL;
		LPTSTR pWorkGuidEx = NULL;
		LPTSTR pEntryGuidEx = NULL;
		xbXml xml;

		xfControl * pView = GetControl(_T("worknodeview"));
		int nIndex = ListView_GetNextItem(pView->m_hWnd,-1,LVNI_SELECTED);
		LVITEM lvitem;
		lvitem.mask = LVIF_PARAM;
		lvitem.iItem = nIndex;
		lvitem.iSubItem = 0;
		ListView_GetItem(pView->m_hWnd,&lvitem);
		pEntryGuid = (LPTSTR)lvitem.lParam;

		_variant_t	var;
		TCHAR url[255];

		xfControl* pSchema = GetControl(_T("schemaview"));
		nIndex = ListView_GetNextItem(pSchema->m_hWnd,-1,LVNI_SELECTED);
		if(nIndex < 0)nIndex = 0;

		_stprintf_s(url,255,_T("worknode:%s/new#%i"),pEntryGuid,nIndex + 1);

		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/%s"),url);
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
		{
			IXMLDOMElement * pElementEx;
			xml.m_pXmlDoc->get_documentElement(&pElementEx);
			if(pElementEx)
			{
				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombineW(m_pStrUrl,_T("/business/xpage.xml"),strUrl,&dwSize,0);

				xfXPage * pXPage = new xfXPage();
				pXPage->m_pStrUrl = _tcsdup(strUrl);
				pXPage->m_pWkStrUrl = _tcsdup(strCmd);
				pXPage->Load(pElementEx);
				pElementEx->Release();

				TEntityParam* pParam = new TEntityParam;
				pParam->pEntity[0]   =  '\0';
				pParam->pKeyValue[0] =  '\0';
				_tcscpy_s(pParam->pWorkNodePath,255,url);
				_tcscpy_s(pParam->pWorkNodeGuid,48,pEntryGuid);

				xfWin* pWin = xfWin::LoadXPageXml(pXPage,m_hWnd,xfApp::GetApp()->GetInstance(),pParam);
				if(pWin) ::DestroyWindow(m_hWnd);
			}
		}
		if(pEntryGuid)delete pEntryGuid;
		if(m_hWnd)::DestroyWindow(m_hWnd);
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmEntryList"))==0)
	{
		xfControl * pTree = GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);

		LPTSTR pPackage = NULL;
		LPTSTR pWorkGuid = NULL;
		LPTSTR pEntryGuid = NULL;
		LPTSTR pItemGuid = NULL;
		LPTSTR pStrXPage = NULL;
		LPTSTR pPackageEx = NULL;
		LPTSTR pWorkGuidEx = NULL;
		LPTSTR pEntryGuidEx = NULL;
		xbXml xml;

		xfControl * pView = GetControl(_T("worknodeview"));
		int nIndex = ListView_GetNextItem(pView->m_hWnd,-1,LVNI_SELECTED);
		if(nIndex < 0) return 1;

		LVITEM lvitem;
		lvitem.mask = LVIF_PARAM;
		lvitem.iItem = nIndex;
		lvitem.iSubItem = 0;
		ListView_GetItem(pView->m_hWnd,&lvitem);
		pEntryGuid = (LPTSTR)lvitem.lParam;

		_variant_t	var;
		TCHAR url[255];
		xfControl* pSchema = GetControl(_T("schemaview"));
		nIndex = ListView_GetNextItem(pSchema->m_hWnd,-1,LVNI_SELECTED);
		if(nIndex < 0)nIndex = 0;

		_stprintf_s(url,255,_T("worknode:%s/list#%i"),pEntryGuid,nIndex + 1);

		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/%s"),url);
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
		{
			IXMLDOMElement * pElementEx;
			xml.m_pXmlDoc->get_documentElement(&pElementEx);
			if(pElementEx)
			{
				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombineW(m_pStrUrl,_T("/business/xpage.xml"),strUrl,&dwSize,0);

				xfXPage * pXPage = new xfXPage();
				pXPage->m_pStrUrl = _tcsdup(strUrl);
				pXPage->m_pWkStrUrl = _tcsdup(strCmd);
				pXPage->Load(pElementEx);
				pElementEx->Release();

				TEntityParam* pParam = new TEntityParam;
				pParam->pEntity[0]   =  '\0';
				pParam->pKeyValue[0] =  '\0';
				_tcscpy_s(pParam->pWorkNodePath,255,url);
				_tcscpy_s(pParam->pWorkNodeGuid,48,pEntryGuid);

				xfWin* pWin = xfWin::LoadXPageXml(pXPage,m_hWnd,xfApp::GetApp()->GetInstance(),pParam);
				if(pWin) ::DestroyWindow(m_hWnd);
			}
		}
		if(pEntryGuid)delete pEntryGuid;
		if(m_hWnd)::DestroyWindow(m_hWnd);
		return 1;
	}
	return 0;
}

bool CModulaDlg::EvtNotify(TEvent* pEvent,LPARAM lParam)
{
	if(pEvent->notify.pnmh->code == LVN_ITEMCHANGED && pEvent->notify.pnmh->hwndFrom == GetControl(_T("worknodeview"))->m_hWnd)
	{
		LPNMLISTVIEW lpv = (LPNMLISTVIEW)pEvent->notify.pnmh;
		if(lpv->iItem < 0) return true;

		LPTSTR pPackage = NULL;
		LPTSTR pWorkGuid = NULL;
		LPTSTR pEntryGuid = NULL;

		LVITEM lvitem;
		lvitem.mask = LVIF_PARAM;
		lvitem.iItem = lpv->iItem;
		lvitem.iSubItem = 0;
		ListView_GetItem(pEvent->notify.pnmh->hwndFrom,&lvitem);
		pEntryGuid = (LPTSTR)lvitem.lParam;

		xfControl * pTree = GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);

		IXMLDOMElement * pElement;
		TVITEM	item;
		item.hItem = hItem;
		item.mask = TVIF_PARAM;
		TreeView_GetItem(pTree->m_hWnd,&item);
		pElement = (IXMLDOMElement *)item.lParam;
		_variant_t	var;
		pElement->getAttribute(L"guid",&var);
		if(var.bstrVal)pWorkGuid = var.bstrVal;

		HTREEITEM hpItem = hItem;
		int nLevel = GetLevel(hpItem);
		for(int i=nLevel;i >1;i--)
		{
			hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
		}
		item.mask = TVIF_PARAM|TVIF_HANDLE;
		item.hItem = hpItem;
		TreeView_GetItem(pTree->m_hWnd,&item);
		pPackage = xbXml::GetStringAttribe((IXMLDOMElement*)item.lParam,_T("src"));

		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdoc %s %s %s"),	pPackage,pWorkGuid ,pEntryGuid);

		xbXml xml;
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)!=1) return 1;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			pPackage = NULL;
			pWorkGuid = NULL;
			pEntryGuid = NULL;

			pElement->getAttribute(L"nodetype",&var);
			if(!var.bstrVal)
			{
				if(m_pWorkNodeType)delete m_pWorkNodeType;
				m_pWorkNodeType = NULL;
			}else if(!m_pWorkNodeType || _tcsicmp(m_pWorkNodeType,var.bstrVal)!=0)
			{
				if(_tcsicmp(var.bstrVal,_T("实体节点"))==0)
				{
					pTree->SwitchLayer(_T("xmEntryOperate"),_T("xmOperate"));
				}else
					pTree->SwitchLayer(_T("xmNormalOperate"),_T("xmOperate"));
				if(m_pWorkNodeType)delete m_pWorkNodeType;
				m_pWorkNodeType = _tcsdup(var.bstrVal);
			}else
			{
				if(m_pWorkNodeType)delete m_pWorkNodeType;
				m_pWorkNodeType = _tcsdup(var.bstrVal);
			}

			pElement->getAttribute(L"package",&var);
			if(var.bstrVal)pPackage = _tcsdup(var.bstrVal);
			pElement->getAttribute(L"workguid",&var);
			if(var.bstrVal)pWorkGuid = _tcsdup(var.bstrVal);
			pElement->getAttribute(L"entryguid",&var);
			if(var.bstrVal)pEntryGuid = _tcsdup(var.bstrVal);

			xfControl* pSchema = GetControl(_T("schemaview"));
			ListView_DeleteAllItems(pSchema->m_hWnd);
			m_guids.clear();

			if(!pPackage || !pWorkGuid || !pEntryGuid ||pPackage[0]=='\0' || pWorkGuid[0]=='\0' || pEntryGuid[0]=='\0' )
			{
				if(pPackage)delete pPackage;
				if(pWorkGuid)delete pWorkGuid;
				if(pEntryGuid)delete pEntryGuid;
				return true;
			}
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdoc %s %s %s"),	pPackage,pWorkGuid ,pEntryGuid);
			if(pPackage)delete pPackage; 
			if(pWorkGuid)delete pWorkGuid;
			if(pEntryGuid)delete pEntryGuid;

			if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)!=1) return 1;

			IXMLDOMNodeList*	pList;
			IXMLDOMNodeList*	pSheetList;
			IXMLDOMNode*		pNode;
			IXMLDOMElement*		pSheet;
			IXMLDOMElement*		pSchemaElement;

			long nLen;
			xml.m_pXmlDoc->selectNodes(_T("entry/list/schema/*"),&pList);
			pList->get_length(&nLen);
			if(nLen > 0)
			{
				pList->get_item(0,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSheet);
				pNode->Release();
				pSheet->getAttribute(L"sheetguid",&var);
				m_listguid = var.bstrVal;
				pSheet->Release();

			}
			pList->Release();

			std::wstring guids;
			xml.m_pXmlDoc->selectNodes(_T("entry/edit/schema"),&pList);
			pList->get_length(&nLen);
			LVITEM	lvItem;
			for(int i=0;i<nLen;i++)
			{
				pList->get_item(i,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSchemaElement);
				pNode->Release();
				pSchemaElement->getAttribute(L"caption",&var);
				if(var.bstrVal)
				{
					lvItem.iItem = i;
					lvItem.iSubItem = 0;
					lvItem.pszText = var.bstrVal;
					lvItem.lParam = i;
					lvItem.pszText = var.bstrVal;
					lvItem.cchTextMax = (int)_tcslen(var.bstrVal) + 1;
					lvItem.iImage = 0;
					lvItem.mask = LVIF_TEXT|LVIF_PARAM|LVIF_IMAGE;
					ListView_InsertItem(pSchema->m_hWnd,&lvItem);
					//::SendMessage(pSchema->m_hWnd,CB_ADDSTRING,0,(LPARAM)var.bstrVal);
				}
				if(i==0)
				{
					ListView_SetItemState(pSchema->m_hWnd,0,  LVIS_SELECTED,   LVIS_SELECTED);
					::SetWindowText(pSchema->m_hWnd,var.bstrVal);
				}

				std::wstring guids;
				pSchemaElement->selectNodes(_T("sheet"),&pSheetList);
				pSchemaElement->Release();
				long sheetLen;
				pSheetList->get_length(&sheetLen);
				for(int j=0;j<sheetLen;j++)
				{
					pSheetList->get_item(j,&pNode);
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSheet);
					pNode->Release();
					pSheet->getAttribute(L"sheetguid",&var);
					pSheet->Release();
					if(var.bstrVal)
						guids += var.bstrVal;
				}
				if(guids !=_T(""))
					this->m_guids.push_back(guids);
				pSheetList->Release();
			}
			pList->Release();
			pElement->Release();
		}
	}

	if(pEvent->notify.pnmh->code == NM_DBLCLK)
	{
		xfControl * pTree = GetControl(_T("DevExplorer1"));
		if(pEvent->notify.pnmh->hwndFrom == pTree->m_hWnd)
		{
			HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);

			IXMLDOMElement * pElement;
			TVITEM	item;
			item.hItem = hItem;
			item.mask = TVIF_PARAM;
			TreeView_GetItem(pTree->m_hWnd,&item);
			pElement = (IXMLDOMElement *)item.lParam;
			_variant_t	var;
			pElement->getAttribute(L"url",&var);
			if(var.bstrVal && var.bstrVal[0])
			{
				TCHAR strCmd[512];
				xbXml xml;
				::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/%s"),var.bstrVal);
				if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
				{
					IXMLDOMElement * pElementEx;
					xml.m_pXmlDoc->get_documentElement(&pElementEx);
					if(pElementEx)
					{
						TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
						DWORD dwSize = INTERNET_MAX_URL_LENGTH;
						xfUriLoader::UrlCombineW(m_pStrUrl,_T("/business/xpage.xml"),strUrl,&dwSize,0);

						xfXPage * pXPage = new xfXPage();
						pXPage->m_pStrUrl = _tcsdup(strUrl);
						pXPage->m_pWkStrUrl = _tcsdup(strCmd);
						pXPage->Load(pElementEx);
						pElementEx->Release();

						TEntityParam* pParam = new TEntityParam;
						pParam->pEntity[0]   =  '\0';
						pParam->pKeyValue[0] =  '\0';
						_tcscpy_s(pParam->pWorkNodePath,255,var.bstrVal);
						pParam->pWorkNodeGuid[0] = '\0';

						LPTSTR pStr0 = _tcsstr(pParam->pWorkNodePath,_T("worknode:{"));
						if(pStr0)
						{
							pStr0 += _tcslen(_T("worknode:{")) - 1;
							LPTSTR pStr1 = _tcsstr(pStr0,_T("}"));
							if(pStr1)
							{
								if(pStr1 - pStr0 < 47)
								{
									_tcsncpy_s((LPTSTR)pParam->pWorkNodeGuid,48,pStr0,pStr1 - pStr0 + 1);
									pParam->pWorkNodeGuid[pStr1 - pStr0 + 1] = '\0';
								}
							}
						}


						HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
						hCursor = ::SetCursor(hCursor);
						m_bAutoDeleted = false;
						xfWin* pWin = xfWin::LoadXPageXml(pXPage,m_hWnd,xfApp::GetApp()->GetInstance(),pParam);
						if(pWin) ::DestroyWindow(m_hWnd);
						m_bAutoDeleted = true;
						hCursor = ::SetCursor(hCursor);
						DestroyCursor(hCursor);
					}
				}
			}
		}
		return 1;
	}
	
	if(pEvent->notify.pnmh->code == TVN_SELCHANGED)
	{
		if(pEvent->notify.pnmh->hwndFrom == GetControl(_T("DevExplorer2"))->m_hWnd)
		{
			LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pEvent->notify.pnmh;
			HWND hWnd = pEvent->notify.pnmh->hwndFrom;
			HTREEITEM hItem = ((LPNMTREEVIEW)pEvent->notify.pnmh)->itemNew.hItem;
			TVITEMEX	tvItem;
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(hWnd,&tvItem);

			IXMLDOMElement * pElement;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pDataElement;
			pElement = (IXMLDOMElement* )(tvItem.lParam);

			_variant_t var;
			BSTR bstr;
			pElement->get_tagName(&bstr);
			if(_tcsicmp(bstr,_T("file"))==0)
			{
				TCHAR strCmd[512];
				pElement = (IXMLDOMElement *)tvItem.lParam;
				pElement->getAttribute(L"guid",&var);
				::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:workflow/getitem %s"),var.bstrVal);

				xbXml xml;
				if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,var.bstrVal,xml)==1)
				{
					xml.m_pXmlDoc->selectSingleNode(L"/sheet",&pNode);
					if(!pNode)
						xml.m_pXmlDoc->selectSingleNode(L"/xdoc/sheet[1]",&pNode);
					if(!pNode) return true;
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pDataElement);
					pNode->Release();
					if(!pDataElement) return true;

					//get flowchart
					XOffice::XFlowChart::XFlowSheet* pSheet =  (XOffice::XFlowChart::XFlowSheet*)GetControl(_T("worknodeflow"));
					XOffice::XFlowChart::XFlowChartDataSvr * pDataSvr = new XOffice::XFlowChart::XFlowChartDataSvr();
					XOffice::XFlowChart::XFlowChartDataSvr * pOldDataSvr;
					pSheet->m_pSheetSvr->m_pSerializeSvr->LoadSheetData(NULL,pDataElement,pDataSvr);

					pOldDataSvr = (XOffice::XFlowChart::XFlowChartDataSvr *)pSheet->m_pSheetSvr->m_pDataSvr;
					((XOffice::XFlowChart::XFlowSheetSvr*)pSheet->m_pSheetSvr)->Clear();
					pSheet->m_pSheetSvr->m_pDataSvr = pDataSvr;
					if(pOldDataSvr)delete pOldDataSvr;

					::InvalidateRect(pSheet->m_hWnd,NULL,false);

					pDataElement->Release();
				}
			}
			::SysFreeString(bstr);
			return true;
		}
	
				
		if(pEvent->notify.pnmh->hwndFrom == GetControl(_T("DevExplorer"))->m_hWnd)
		{
			xfControl* pSchema = GetControl(_T("schemaview"));
			ListView_DeleteAllItems(pSchema->m_hWnd);

			xfControl* pControl = GetControl(_T("worknodeview"));
			::SendMessage(pControl->m_hWnd,LVM_DELETEALLITEMS,0,0);
			ListView_SetImageList(pControl->m_hWnd,m_pXPage->m_pResourceMgr->m_hImageLists[0],LVSIL_NORMAL);
			LVITEM lvItem;
			int iItem;
			int cnt = ListView_GetItemCount(pControl->m_hWnd);
			for(int i=0;i<cnt;i++)
			{
				lvItem.mask = LVIF_PARAM;
				lvItem.iItem = i;
				lvItem.iSubItem = 0;
				ListView_GetItem(pControl->m_hWnd,&lvItem);
				if(lvItem.lParam)delete (LPTSTR)lvItem.lParam;
			}

			pControl->SwitchLayer(_T("xmBlackOperate"),_T("xmOperate"));

			LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pEvent->notify.pnmh;
			HWND hWnd = pEvent->notify.pnmh->hwndFrom;
			HTREEITEM hItem = ((LPNMTREEVIEW)pEvent->notify.pnmh)->itemNew.hItem;
			TVITEMEX	tvItem;
			int	nLevel;
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(hWnd,&tvItem);

			IXMLDOMElement * pElement;
			pElement = (IXMLDOMElement* )(tvItem.lParam);

			nLevel = this->GetLevel(hItem);
			if(nLevel>1)
			{
				_variant_t sheet,type;

				if(pElement)
				{
					BSTR bstr;
					bool bDestroy = false;
					pElement->getAttribute(L"type",&type);  
					if(!type.bstrVal)
					{
						pElement->get_tagName(&bstr);
						bDestroy = true;
					}
					else
						bstr = type.bstrVal;
					if(bstr)
					{
						if(_tcsicmp(bstr,_T("work"))==0)
						{
							LPTSTR pSrc = NULL;
							pSrc = xbXml::GetStringAttribe(pElement,_T("src"));

							for(int i=nLevel;i > 1;i--)
							{
								hItem = TreeView_GetParent(hWnd, hItem);
							}
							tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
							tvItem.hItem = hItem;
							TreeView_GetItem(hWnd,&tvItem);
							LPTSTR pHxdSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));
							int nIndex = (int)_tcslen(pHxdSrc);
							while(pHxdSrc[--nIndex]!='/');
							if(nIndex >=0) pHxdSrc[nIndex]='\0';
							TCHAR buf[MAX_PATH];
							_stprintf_s(buf,MAX_PATH,_T("/developer/%s/%s"),pHxdSrc,pSrc);
							if(pHxdSrc) delete pHxdSrc;
							if(pSrc) delete pSrc;

							TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
							DWORD dwSize = INTERNET_MAX_URL_LENGTH;
							xfUriLoader::UrlCombineW(m_pStrUrl,buf,strUrl,&dwSize,0);

							BYTE* pData;
							int nLen = 0;
							if(xfUriLoader::Load(strUrl, NULL,pData,nLen)==1)
							{
								TCHAR Path[MAX_PATH];
								xfUriLoader::GetCacheFileName(strUrl,Path);
								xbXml xml;
								xml.Load(Path);

								IXMLDOMNodeList* pNodeList;
								IXMLDOMNode*	 pNode;
								xml.m_pXmlDoc->selectNodes(_T("/work/client/worknode/file|/work/client/worklist/file"),&pNodeList);
								long nLen;
								pNodeList->get_length(&nLen);

								for(int i=0;i<nLen;i++)
								{
									lvItem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
									lvItem.iItem = i;
									lvItem.iSubItem = 0;

									pNodeList->get_item(i,&pNode);
									pElement = xbXml::GetElement(pNode);
									pNode->Release();

									_variant_t var;
									pElement->getAttribute(L"caption",&var);
									lvItem.pszText =var.bstrVal;
									lvItem.cchTextMax =(int)_tcslen(lvItem.pszText);
									pElement->getAttribute(L"guid",&var);
									if(var.bstrVal)lvItem.lParam = (LPARAM)_tcsdup(var.bstrVal);

									pElement->getAttribute(L"image",&var);
									if(var.bstrVal)lvItem.iImage = var;
									iItem = (int)::SendMessage(pControl->m_hWnd,LVM_INSERTITEM,0,(LPARAM)&lvItem);


									pElement->Release();
								}
								if(nLen >0)
								{
									ListView_SetItemState(pControl->m_hWnd,0,  LVIS_SELECTED,   LVIS_SELECTED);
								}
								pNodeList->Release();
							}else
							{
								if(pData && nLen > 0)
									xfUriLoader::FreeData(pData);
								return true;
							}
							if(pData && nLen > 0)
								xfUriLoader::FreeData(pData);

						}
						if(bDestroy)::SysFreeString(bstr);
					}
				}
			}
		}
	}
	return true;
}

int CModulaDlg::GetLevel(HTREEITEM hItem)
{
	int level = 0;
	HTREEITEM hpItem;
	hpItem = hItem;
	xfControl * pControl = GetControl(_T("DevExplorer"));
	while(hpItem)
	{
		level ++;
		hpItem = TreeView_GetParent(pControl->m_hWnd, hpItem);
	}
	return level;
}

