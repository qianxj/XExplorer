#include "StdAfx.h"
#include "FavoritesDlg.h"

FavoritesDlg::FavoritesDlg(void)
{
	this->AttachListon(WM_NOTIFY,(FNEvent)&FavoritesDlg::EvtNotify);
}

FavoritesDlg::~FavoritesDlg(void)
{
}

bool FavoritesDlg::EvtNotify(TEvent* pEvent,LPARAM lParam)
{
	LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pEvent->notify.pnmh;
	if(ptvdi->hdr.code == TVN_ENDLABELEDIT)
	{
		if(ptvdi->item.pszText)
		{
			HTREEITEM hItem = ptvdi->item.hItem;
			TVITEM tvItem;
			if(!TreeView_GetParent(ptvdi->hdr.hwndFrom,hItem)) return true;
		
			xfControl * pTree = GetControl(_T("favoritetree"));

			bool bRoot = false;
			if(!TreeView_GetParent(pTree->m_hWnd,hItem))  bRoot = true;

			TCHAR strCmd[512];
			TCHAR guid[48];
			IXMLDOMElement * pElement;

			guid[0]='*';
			guid[1]='\0';
			tvItem.hItem = hItem;
			tvItem.mask = TVIF_PARAM|TVIF_CHILDREN ;
			TreeView_GetItem(pTree->m_hWnd,&tvItem);

			if(!bRoot)
			{
				_variant_t var;
				pElement = (IXMLDOMElement *)tvItem.lParam;
				pElement->getAttribute(L"guid",&var);
				if(var.bstrVal)
					_tcscpy_s(guid,48,var.bstrVal);
				BSTR bstr;
				pElement->get_tagName(&bstr);
				if(_tcsicmp(bstr,_T("folder"))!=0)
				{
					pElement->Release();
					::SysFreeString(bstr);
					return 1;
				}
				::SysFreeString(bstr);
			}

			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:favorites/folderupdate %s"),guid);

			BYTE * pData = NULL;
			int nLen;
			if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,ptvdi->item.pszText,pData,nLen)==1)
			{
				tvItem.mask = TVIF_HANDLE|TVIF_TEXT;
				tvItem.hItem = hItem;
				tvItem.pszText = ptvdi->item.pszText;
				tvItem.cchTextMax = (int)_tcslen(tvItem.pszText) + 1;
				TreeView_SetItem(ptvdi->hdr.hwndFrom,&tvItem);
			}
			if(pData)xfUriLoader::FreeData(pData);
		}
	}
	return true;
}

int FavoritesDlg::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(!pStrID) return 1;

	if(_tcsicmp(pStrID,_T("xmFileClose"))==0)
	{
		m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}

	if(_tcsicmp(pStrID,_T("xmFileAdd"))==0)
	{
		int nLen;
		nLen = ::GetWindowTextLength(GetControl(_T("FavoritesName"))->m_hWnd);
		if(nLen < 1) return 1;
		
		_variant_t var;
		IXMLDOMElement * pElement = (IXMLDOMElement *)this->m_pParam;
		pElement->getAttribute(L"nodetype",&var);

		TCHAR buf[512];
		if(_tcsicmp(var.bstrVal,_T("实体节点"))==0)
		{
			int nIndex = (int)SendMessage(GetControl(_T("opitem"))->m_hWnd,CB_GETCURSEL,0,0);
			if(nIndex < 0) nIndex = 0;
			
			int nSchema = (int)SendMessage(GetControl(_T("schemaitem"))->m_hWnd,CB_GETCURSEL,0,0);
			if(nSchema < 0) nSchema = 0;

			pElement->getAttribute(L"guid",&var);
			if(nIndex==0)
				_stprintf_s(buf,512,_T("worknode:%s/new#%i"),var.bstrVal,nSchema + 1);
			else if(nIndex==1)
				_stprintf_s(buf,512,_T("worknode:%s/list#%i"),var.bstrVal,nSchema + 1);
			else if(nIndex==2)
				_stprintf_s(buf,512,_T("worknode:%s/check#%i"),var.bstrVal,nSchema + 1);

		}else
		{
			int nSchema = (int)SendMessage(GetControl(_T("schemaitem"))->m_hWnd,CB_GETCURSEL,0,0);
			if(nSchema < 0) nSchema = 0;
			pElement->getAttribute(L"guid",&var);
			_stprintf_s(buf,512,_T("worknode:%s/open#%i"),var.bstrVal,nSchema + 1);
		}

		xfControl * pTree = GetControl(_T("favoritetree"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);

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
		LPTSTR pStrText = new TCHAR[nLen + 1];
		::GetWindowText(GetControl(_T("FavoritesName"))->m_hWnd,pStrText,nLen + 1);
		
		TCHAR strCmd[512];
		if(!bRoot)
		{
			pElement = (IXMLDOMElement *)tvItem.lParam;
			pElement->getAttribute(L"guid",&var);
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:favorites/add %s %s"),var.bstrVal,buf);
		}else
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:favorites/add * %s"),buf);
		
		xbXml xml;
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,pStrText,xml)==1)
		{

			tvItem.cChildren = true;
			TreeView_SetItem(pTree->m_hWnd,&tvItem);

			xml.m_pXmlDoc->get_documentElement(&pElement);
			TVINSERTSTRUCT tvins;
			tvins.hInsertAfter = TVI_LAST;
			tvins.hParent = hItem;
			tvins.item.pszText = pStrText;
			tvins.item.cchTextMax = 255;
			tvins.item.lParam = (LPARAM)pElement;
			tvins.item.iImage = 20;
			tvins.item.iSelectedImage = 20;
			tvins.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			TreeView_InsertItem(pTree->m_hWnd,&tvins);
		}

		delete pStrText;
		m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(_tcsicmp(pStrID,_T("xmFolderAdd"))==0)
	{
		int nLen;
		nLen = ::GetWindowTextLength(GetControl(_T("newfolder"))->m_hWnd);
		if(nLen < 1) return 1;
		LPTSTR pStrText = new TCHAR[nLen + 1];
		::GetWindowText(GetControl(_T("newfolder"))->m_hWnd,pStrText,nLen + 1);

		xfControl * pTree = GetControl(_T("favoritetree"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);

		bool bRoot = false;
		if(!TreeView_GetParent(pTree->m_hWnd,hItem))  bRoot = true;
		
		TCHAR strCmd[512];
		TCHAR guid[48];
		IXMLDOMElement * pElement;

		guid[0]='*';
		guid[1]='\0';
		TVITEM  tvItem;
		tvItem.hItem = hItem;
		tvItem.mask = TVIF_PARAM|TVIF_CHILDREN ;
		TreeView_GetItem(pTree->m_hWnd,&tvItem);
		
		if(!bRoot)
		{
			_variant_t var;
			pElement = (IXMLDOMElement *)tvItem.lParam;
			pElement->getAttribute(L"guid",&var);
			if(var.bstrVal)
				_tcscpy_s(guid,48,var.bstrVal);
			BSTR bstr;
			pElement->get_tagName(&bstr);
			if(_tcsicmp(bstr,_T("folder"))!=0)
			{
				::SysFreeString(bstr);
				return 1;
			}
			::SysFreeString(bstr);
		}

		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:favorites/folderadd %s"),guid);
		xbXml xml;
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,pStrText,xml)==1)
		{

			tvItem.cChildren = true;
			TreeView_SetItem(pTree->m_hWnd,&tvItem);

			xml.m_pXmlDoc->get_documentElement(&pElement);
			TVINSERTSTRUCT tvins;
			tvins.hInsertAfter = TVI_LAST;
			tvins.hParent = hItem;
			tvins.item.pszText = pStrText;
			tvins.item.cchTextMax = 255;
			tvins.item.lParam = (LPARAM)pElement;
			tvins.item.iImage = 15;
			tvins.item.iSelectedImage = 15;
			tvins.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			TreeView_InsertItem(pTree->m_hWnd,&tvins);
		}

		return 1;
	}
	if(_tcsicmp(pStrID,_T("xmFolderDelete"))==0)
	{
		xfControl * pTree = GetControl(_T("favoritetree"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
		if(!hItem) return 1;
		if(!(GetKeyState(VK_CONTROL) & 0x8000))
			if(MessageBox(m_hWnd,_T("确定要删除指定的项"),_T("收藏夹"),MB_YESNO)==IDNO) return 1;

		bool bRoot = false;
		if(!TreeView_GetParent(pTree->m_hWnd,hItem)) return 1;
		
		TCHAR strCmd[512];
		TCHAR guid[48];
		IXMLDOMElement * pElement;

		guid[0]='*';
		guid[1]='\0';
		TVITEM  tvItem;
		tvItem.hItem = hItem;
		tvItem.mask = TVIF_PARAM|TVIF_CHILDREN ;
		TreeView_GetItem(pTree->m_hWnd,&tvItem);
		
		_variant_t var;
		pElement = (IXMLDOMElement *)tvItem.lParam;
		pElement->getAttribute(L"guid",&var);
		if(var.bstrVal)
			_tcscpy_s(guid,48,var.bstrVal);

		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/prefs:favorites/folderdel %s"),guid);
		BYTE * pData = NULL;
		int nLen;
		if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen)==1)
			TreeView_DeleteItem(pTree->m_hWnd,hItem);

		if(pData)xfUriLoader::FreeData(pData);
		return 1;
	}
	
	return 1;
}

int FavoritesDlg::OnOpen()
{
	xfControl * pTree = GetControl(_T("favoritetree"));
	::SetWindowLong(pTree->m_hWnd,GWL_STYLE,::GetWindowLong(pTree->m_hWnd,GWL_STYLE) | TVS_EDITLABELS); 

	if(m_pParam)
	{
		_variant_t var;
		IXMLDOMElement * pElement = (IXMLDOMElement *)this->m_pParam;
		pElement->getAttribute(L"caption",&var);
		if(var.bstrVal)
			::SetWindowText(GetControl(_T("FavoritesName"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"nodetype",&var);
		if(_tcsicmp(var.bstrVal,_T("实体节点"))==0)
		{
			::SendMessage(GetControl(_T("opitem"))->m_hWnd,CB_ADDSTRING,0,(LPARAM)_T("新建"));
			::SendMessage(GetControl(_T("opitem"))->m_hWnd,CB_ADDSTRING,0,(LPARAM)_T("列表"));
			::SendMessage(GetControl(_T("opitem"))->m_hWnd,CB_ADDSTRING,0,(LPARAM)_T("审核"));
			::SetWindowText(GetControl(_T("opitem"))->m_hWnd,_T("新建"));
		}else
		{
			::SendMessage(GetControl(_T("opitem"))->m_hWnd,CB_ADDSTRING,0,(LPARAM)_T("打开"));
			::SetWindowText(GetControl(_T("opitem"))->m_hWnd,_T("打开"));
		}

		IXMLDOMNodeList * pList;
		IXMLDOMNode* pNode;
		IXMLDOMElement * pElementEx;
		pElement->selectNodes(L"item",&pList);
		long nLen;
		pList->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			pList->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEx);

			pElementEx->getAttribute(L"caption",&var);
			::SendMessage(GetControl(_T("schemaitem"))->m_hWnd,CB_ADDSTRING,0,(LPARAM)var.bstrVal);
			if(i==0)::SetWindowText(GetControl(_T("schemaitem"))->m_hWnd,var.bstrVal);

			pElementEx->Release();
			pNode->Release();
		}
	}

	HTREEITEM hItem;
	hItem = TreeView_GetRoot(pTree->m_hWnd);
	TreeView_Expand(pTree->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL);
	TreeView_SelectItem(pTree->m_hWnd,hItem);

	return 1;
}


