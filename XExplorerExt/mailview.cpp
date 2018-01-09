#include "StdAfx.h"
#include "MailView.h"
#include "xbxbase.h"
#include "cdosys.h"
#include "xccontrol.h"

#import "c:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "EndOfFile")

CMailView::CMailView(void):m_viewType(VT_SafeHtml)
{
}

CMailView::~CMailView(void)
{
}

int CMailView::OnOpen()
{
	xcTreeCtrl * pTree = dynamic_cast<xcTreeCtrl	*>(GetControl(_T("mailfolder")));

	HTREEITEM	hItem = TreeView_GetRoot(pTree->m_hWnd);
	TreeView_Expand(pTree->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL);
	TreeView_SelectItem(pTree->m_hWnd,hItem);

	hItem = TreeView_GetChild(pTree->m_hWnd,hItem);
	TreeView_Expand(pTree->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL);
	TreeView_SelectItem(pTree->m_hWnd,hItem);

	return 1;
}

int CMailView::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(!pStrID) return 0;
	if(_tcsicmp(pStrID,_T("xmMailNew"))==0)
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("mailedit.xml"),strUrl,&dwSize,0);

		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance());
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}
	if(_tcsicmp(pStrID,_T("xmViewText"))==0)
	{
		m_viewType = VT_Text;
		if(GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption)
		{
			delete GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption;
			GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption = _tcsdup(_T("纯文本"));
			::InvalidateRect(GetControl(_T("xmViewType"))->m_hWnd,NULL,false);

			int nItem = ListView_GetNextItem(GetControl(_T("maillist"))->m_hWnd,-1,LVNI_SELECTED);
			if(nItem >=0) ((XMailList *)GetControl(_T("maillist")))->SelectedItem(nItem);
		}
		return 1;
	}

	if(_tcsicmp(pStrID,_T("xmViewSafeHtml"))==0)
	{
		m_viewType = VT_SafeHtml;
		if(GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption)
		{
			delete GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption;
			GetControl(_T("xmViewType"))->m_pxfNode->m_pCaption = _tcsdup(_T("安全网页"));
			::InvalidateRect(GetControl(_T("xmViewType"))->m_hWnd,NULL,false);
			
			int nItem = ListView_GetNextItem(GetControl(_T("maillist"))->m_hWnd,-1,LVNI_SELECTED);
			if(nItem >=0) ((XMailList *)GetControl(_T("maillist")))->SelectedItem(nItem);
		}
		return 1;
	}

	return 0;
}

xfControl * CMailView::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	if(::_tcsicmp(pCtrlID,_T("mailfolder"))==0)
	{
		xfControl * pControl;
		pControl = new XMailFolder();
		pControl->m_dwExStyle = 0;
		pControl->m_dwStyle = WS_VISIBLE|WS_CHILD|TVS_HASLINES|/*TVS_LINESATROOT|*/TVS_SHOWSELALWAYS|TVS_HASBUTTONS;
		pControl->m_pszWinClass = WC_TREEVIEW;
		return pControl;
	}
	if(::_tcsicmp(pCtrlID,_T("maillist"))==0)
	{
		xfControl * pControl;
		pControl = new XMailList();
		pControl->m_dwExStyle = 0;
		pControl->m_dwStyle = WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHAREIMAGELISTS;
		pControl->m_pszWinClass = WC_LISTVIEW; 
		return pControl;
	}
	return NULL;
}


CMailView::XMailFolder::XMailFolder()
{
	this->AttachCtrlListon(TVN_SELCHANGED,XFRAME_FNEVENT_(CMailView::XMailFolder::SelectChanged));
	this->AttachCtrlListon(NM_DBLCLK,XFRAME_FNEVENT_(CMailView::XMailFolder::DoubleClicked)); 
}
CMailView::XMailFolder::~XMailFolder()
{
	XFRAME_EVENT_CLEAR;
}

XFRAME_EVENT_(CMailView::XMailFolder::DoubleClicked)
{
	return 1;
}

XFRAME_EVENT_(CMailView::XMailFolder::SelectChanged)
{
	HWND hWnd = pEvent->hWnd;
	HTREEITEM hItem = ((LPNMTREEVIEW)pEvent->notify.pnmh)->itemNew.hItem;
	IXMLDOMElement * pElement;
	TV_ITEM	tvItem;
	int nLevel = GetLevel(hItem);

	if(nLevel==3)
	{
		HTREEITEM hpItem;
		hpItem = TreeView_GetParent(m_hWnd, hItem);
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
		tvItem.hItem = hpItem;
		TreeView_GetItem(m_hWnd,&tvItem);
		if(::_tcsicmp((LPTSTR)tvItem.lParam,_T("localfolder"))==0)
		{
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(m_hWnd,&tvItem);
		
			TCHAR buf[255];

			_stprintf_s(buf,255,_T("uriserver/uri.hxsoft.com/xaserver/entity/mail/list"));
			XMailList * pMailListCtrl;
			pMailListCtrl = (XMailList *)this->GetWin()->GetControl(_T("maillist"));
			pMailListCtrl->Retrieve(buf,(LPTSTR)tvItem.lParam);

			return 1;
		}

		if(::_tcsicmp((LPTSTR)tvItem.lParam,_T("clientfolder"))==0)
		{
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(m_hWnd,&tvItem);
			pElement = (IXMLDOMElement *)tvItem.lParam;
			
			IXMLDOMNode * pNode;
			BSTR bstr;
			pElement->selectSingleNode(L"@id",&pNode);
			if(!pNode) return 1 ;
			pNode->get_text(&bstr);

			TCHAR buf[1024];
			::_stprintf_s(buf,1024,_T("\r\n<client_id>%s</client_id>"),bstr);
			::SysFreeString(bstr);
			pNode->Release();
			XMailList * pMailListCtrl;
			pMailListCtrl = (XMailList *)this->GetWin()->GetControl(_T("maillist"));
			pMailListCtrl->Retrieve(_T("uriserver/uri.hxsoft.com/xaserver/entity/mail/query"),buf);
		}
	}
	return 1;
}

int CMailView::XMailFolder::GetLevel(HTREEITEM hItem)
{
	int level = 0;
	HTREEITEM hpItem;
	hpItem = hItem;
	while(hpItem)
	{
		level ++;
		hpItem = TreeView_GetParent(m_hWnd, hpItem);
	}
	return level;
}

CMailView::XMailList::XMailList()
{
	this->AttachCtrlListon(LVN_ITEMCHANGED,XFRAME_FNEVENT_(CMailView::XMailList::SelectChanged));
	this->AttachCtrlListon(NM_DBLCLK,XFRAME_FNEVENT_(CMailView::XMailList::DoubleClicked)); 
}

CMailView::XMailList::~XMailList()
{
	XFRAME_EVENT_CLEAR;
}

XFRAME_EVENT_(CMailView::XMailList::DoubleClicked)
{
	return true;
}

int CMailView::XMailList::SelectedItem(int nItem)
{
	::SetWindowText(this->GetWin()->GetControl(_T("mailcontent"))->m_hWnd,NULL);


	LVITEM lvItem;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	lvItem.mask = LVIF_PARAM;
	ListView_GetItem(m_hWnd,&lvItem);

	//int nMailID = (int)lvItem.lParam;
	LPTSTR nMailID = (LPTSTR)lvItem.lParam;
	TCHAR buf[64];
	LPBYTE pData;
	int nLen;
	//::_stprintf_s(buf,30,_T("%i"),nMailID);
	::_stprintf_s(buf,64,_T("%s"),nMailID); 
	//TCHAR Path[MAX_PATH];
	TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
	DWORD dwSize = INTERNET_MAX_URL_LENGTH;
	xfUriLoader::UrlCombineW(m_pxfNode->GetXPage()->m_pStrUrl,_T("uriserver/uri.hxsoft.com/xaserver/entity/mail/querybody"),strUrl,&dwSize,0);

	if( xfUriLoader::Load(strUrl,NULL,buf,pData,nLen,false)==1)
	{
		CDO::IMessage* pMsg = NULL;
		HRESULT hr = CoCreateInstance( __uuidof(CDO::Message), NULL, CLSCTX_INPROC_SERVER, __uuidof(CDO::IMessage), (void**) &pMsg );

		/*
		int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, -1,  NULL, 0);
		TCHAR* pszA = new TCHAR[_convert];
		int nRet = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, -1, pszA, _convert);
		*/


		SAFEARRAY FAR* psa;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nLen - 2;
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		psa->pvData = pData;

		VARIANT var;
		::VariantInit(&var);
		var.vt = VT_ARRAY|VT_UI1;
		var.parray = psa;

		_variant_t   varOptional(DISP_E_PARAMNOTFOUND,VT_ERROR);  
		_StreamPtr   adostream;
		adostream.CreateInstance(_T( "ADODB.Stream"));
		adostream-> Type   =   adTypeBinary;
		//adostream-> Type   =   adTypeText;//adTypeBinary;
		adostream->put_Mode(adModeReadWrite);
		adostream-> Open(varOptional,adModeUnknown,   adOpenStreamUnspecified,   _T(""),   _T(""));  
		//adostream-> LoadFromFile( "d:\\aa.txt");
		//adostream->WriteText((LPTSTR)pData,adWriteChar);
		adostream->Write(var);
		//adostream->put_Position(0);
		//_variant_t   vReadTo   =   adostream-> ReadText(adReadAll);  
		long   blob_size   =   adostream-> GetSize();


		CDO::IDataSource * pDataSource;
		pMsg->get_DataSource(&pDataSource);
		hr = pDataSource->OpenObject(adostream,L"_Stream");
		adostream-> Close(); 
		adostream->Release();

		//SafeArrayDestroy(psa);

		//pMsg->put_HTMLBody((LPTSTR)pszA);

		BSTR bstrText;
		BSTR bstrHtml = NULL;
		try
		{
			pMsg->get_TextBody(&bstrText);
			if(bstrText)
			{
				::SetWindowText(this->GetWin()->GetControl(_T("mailcontent"))->m_hWnd,bstrText);
			}
			//::SysFreeString(bstr);

			pMsg->get_HTMLBody(&bstrHtml);
		}catch(...)
		{
		}
		if(true)
		{
			IWebBrowser2	*webBrowser2;
			LPDISPATCH		lpDispatch;
			IHTMLDocument2	*htmlDoc2;
			SAFEARRAY		*sfArray;
			VARIANT			*pVar;
			BSTR			bstr = NULL;
			SAFEARRAYBOUND ArrayBound = {1, 0};


			xcOleControlContain*  pControl = dynamic_cast<xcOleControlContain *>(GetWin()->GetControl(_T("mailcontentex")));
			if(!pControl->m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
			{
				if (!webBrowser2->get_Document(&lpDispatch))
				{
					if (!lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&htmlDoc2))
					{
						//htmlDoc2->put_designMode(L"on");
						if(((CMailView *)GetWin())->m_viewType==CMailView::VT_Text || ((!bstrHtml || !bstrHtml[0]) && bstrText))
						{
							IHTMLElement *pElement;
							htmlDoc2->get_body(&pElement);
							pElement->put_innerText(bstrText);
						}
						else
						{
							if ((sfArray = SafeArrayCreate(VT_VARIANT, 1, (SAFEARRAYBOUND *)&ArrayBound)))
							{
								if (!SafeArrayAccessData(sfArray, (void**)&pVar))
								{
									pVar->vt = VT_BSTR;
									bstr = SysAllocString(bstrHtml);

									if ((pVar->bstrVal = bstr))
									{
										htmlDoc2->write(sfArray);
										htmlDoc2->close();
									}
								}
								SafeArrayDestroy(sfArray);
							}
						}

						htmlDoc2->Release();
					}
					lpDispatch->Release();
				}
			}
		}
		if(bstrText)::SysFreeString(bstrText);
		if(bstrHtml)::SysFreeString(bstrHtml);

		pMsg->Release();
		//delete pszA;

		xfUriLoader::FreeData(pData);

		return 1;
	}
	return 1;
}

XFRAME_EVENT_(CMailView::XMailList::SelectChanged)
{

	if (((LPNMLISTVIEW) pEvent->notify.pnmh)->uChanged & LVIF_STATE &&
		((LPNMLISTVIEW) pEvent->notify.pnmh)->uNewState & LVIS_SELECTED)
	{

		this->SelectedItem(((LPNMLISTVIEW) pEvent->notify.pnmh)->iItem);
	}
	return true;
}

int CMailView::XMailList::ControlCreated()
{
	ListView_SetExtendedListViewStyle(m_hWnd,LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);
	return 1;
}
int CMailView::XMailList::Retrieve(LPTSTR Url,LPTSTR pXmlQuery)
{
	TCHAR Path[MAX_PATH];
	TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
	DWORD dwSize = INTERNET_MAX_URL_LENGTH;
	xfUriLoader::UrlCombineW(m_pxfNode->GetXPage()->m_pStrUrl,Url,strUrl,&dwSize,0);
	if(xfUriLoader::GetCacheFileName(strUrl,pXmlQuery,Path,MAX_PATH,true)==1)
	{
		xbXml xml;  
		xml.Load(Path);

		IXMLDOMNodeList* pNodeList;
		IXMLDOMNode* pNode;
		IXMLDOMElement* pElement;
		xml.m_pXmlDoc->selectNodes(L"/mail/row",&pNodeList);
		long nLen;
		pNodeList->get_length(&nLen);

		LVITEM lvItem;
		_variant_t var;
		int iItem;
		::SendMessage(m_hWnd,LVM_DELETEALLITEMS,0,0);
		for(int i=0;i<nLen;i++)
		{
			lvItem.mask = LVIF_TEXT|LVIF_PARAM;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;

			pNodeList->get_item(i,&pNode);
			pElement = xbXml::GetElement(pNode);
			pNode->Release();

			pElement->getAttribute(L"id",&var);
			lvItem.lParam = (LPARAM)var.bstrVal;
			pElement->getAttribute(L"FromAddr",&var);
			
			if(var.bstrVal)
			{
				if(_tcsstr(var.bstrVal,_T("<")))_tcsstr(var.bstrVal,_T("<"))[0]='\0';
				lvItem.pszText =var.bstrVal;
			}else
				lvItem.pszText=L"";
			if(var.bstrVal)
				lvItem.cchTextMax = (int)_tcslen(lvItem.pszText) + 1;
			else
				lvItem.cchTextMax = 1;
			iItem = (int)::SendMessage(m_hWnd,LVM_INSERTITEM,0,(LPARAM)&lvItem);

			lvItem.iItem = iItem;
			lvItem.mask = LVIF_TEXT;
			pElement->getAttribute(L"Subject",&var);

			lvItem.iSubItem = 1;
			lvItem.pszText =var.bstrVal;
			if(var.bstrVal)
				lvItem.cchTextMax = (int)_tcslen(lvItem.pszText) + 1;
			else
				lvItem.cchTextMax = 1;
			::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)&lvItem);

			pElement->getAttribute(L"SendDate",&var);
			lvItem.iSubItem++;
			lvItem.pszText =var.bstrVal;
			if(var.bstrVal)
				lvItem.cchTextMax = (int)_tcslen(lvItem.pszText) + 1;
			else
				lvItem.cchTextMax = 1;
			::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)&lvItem);

			pElement->getAttribute(L"ToAddr",&var);
			lvItem.iSubItem++;
			lvItem.pszText =var.bstrVal;
			if(var.bstrVal)
				lvItem.cchTextMax = (int)_tcslen(lvItem.pszText) + 1;
			else
				lvItem.cchTextMax = 1;
			::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)&lvItem);

			pElement->getAttribute(L"RecDate",&var);
			lvItem.iSubItem++;
			lvItem.pszText =var.bstrVal;
			if(var.bstrVal)
				lvItem.cchTextMax = (int)_tcslen(lvItem.pszText) + 1;
			else
				lvItem.cchTextMax = 1;
			::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)&lvItem);

			pElement->getAttribute(L"Account",&var);
			lvItem.iSubItem++;
			lvItem.pszText =var.bstrVal;
			if(var.bstrVal)
				lvItem.cchTextMax = (int)_tcslen(lvItem.pszText) + 1;
			else
				lvItem.cchTextMax = 1;
			::SendMessage(m_hWnd,LVM_SETITEM,0,(LPARAM)&lvItem);

			pElement->Release();
		}
		pNodeList->Release();
	}else
		return 0;

	return 1;
}



