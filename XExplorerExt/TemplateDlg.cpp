#include "StdAfx.h"
#include "TemplateDlg.h"

CTemplateDlg::CTemplateDlg(void):bEditModified(false)
{
	this->AttachListon(WM_NOTIFY,(FNEvent)&CTemplateDlg::EvtNotify);
	this->AttachListon(WM_COMMAND,(FNEvent)&CTemplateDlg::EvtCommand);
}

CTemplateDlg::~CTemplateDlg(void)
{
	this->ClearAllListonsTo();
	if(this->m_pParam)delete (LPTSTR)m_pParam;
}

int CTemplateDlg::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	return 0;
}

int CTemplateDlg::OnOpen()
{
	return 0;
}

bool CTemplateDlg::EvtCommand(TEvent* pEvent,LPARAM lParam)
{
	if(pEvent->command.wmEvent==EN_UPDATE)
	{
		bEditModified = true;
	}
	return true;
}

bool CTemplateDlg::EvtNotify(TEvent* pEvent,LPARAM lParam)
{
	LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pEvent->notify.pnmh;
	if(ptvdi->hdr.code == LVN_ITEMCHANGED && !bEditModified)
	{
		HWND hWnd = pEvent->notify.pnmh->hwndFrom;
		int index = ((LPNMLISTVIEW)pEvent->notify.pnmh)->iItem;
		TCHAR pText[255];
		LVITEM lvItem;
		lvItem.iItem = index;
		lvItem.mask = LVIF_TEXT;
		lvItem.pszText =  pText;
		lvItem.cchTextMax = 255;
		lvItem.iSubItem = 0;
		ListView_GetItem(hWnd,&lvItem);
		::SetWindowText(this->GetControl(_T("sheetname"))->m_hWnd,lvItem.pszText);

		return true;
	}
	if(ptvdi->hdr.code == TVN_SELCHANGED)
	{
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
					if(_tcsicmp(bstr,_T("template"))==0)
					{
						LPTSTR pSrc = NULL;
						LPTSTR pSrcFile = NULL;
						pSrcFile = xbXml::GetStringAttribe(pElement,_T("src"));

						//get work folder
						hItem = TreeView_GetParent(hWnd, hItem);
						hItem = TreeView_GetParent(hWnd, hItem);
						tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
						tvItem.hItem = hItem;
						TreeView_GetItem(hWnd,&tvItem);
						pElement = (IXMLDOMElement* )(tvItem.lParam);
						pSrc = xbXml::GetStringAttribe(pElement,_T("src"));

						nLevel = this->GetLevel(hItem);
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
						if(nIndex >=0) pHxdSrc[nIndex] = '\0';
						TCHAR buf[MAX_PATH];
						_stprintf_s(buf,MAX_PATH,_T("/developer/%s/%s"),pHxdSrc,pSrc);
						if(pHxdSrc)delete pHxdSrc;
						
						int nPos = (int)_tcslen(buf);
						while(buf[--nPos]!='.' && nPos >=0);
						if(nPos>=0) buf[nPos]='\0';
						_stprintf_s(buf,MAX_PATH,_T("%s/template/%s"),buf,pSrcFile);

						if(pSrc) delete pSrc;
						if(pSrcFile)delete pSrcFile;

						TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
						DWORD dwSize = INTERNET_MAX_URL_LENGTH;
						xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,buf,strUrl,&dwSize,0);

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
							xml.m_pXmlDoc->selectNodes(_T("xdoc/sheet"),&pNodeList);
							long nLen;
							pNodeList->get_length(&nLen);
							xfControl* pControl = GetControl(_T("sheetview"));

							ListView_SetImageList(pControl->m_hWnd,m_pXPage->m_pResourceMgr->m_hImageLists[0],LVSIL_NORMAL);
							LVITEM lvItem;
							int iItem;
							
							int cnt = ListView_GetItemCount(pControl->m_hWnd);
							for(int i=0;i<cnt;i++)
							{
								lvItem.mask = LVIF_PARAM;
								lvItem.iItem = i;
								ListView_GetItem(pControl->m_hWnd,&lvItem);
								if(lvItem.lParam)delete (LPTSTR)lvItem.lParam;
							}
							::SendMessage(pControl->m_hWnd,LVM_DELETEALLITEMS,0,0);
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
								lvItem.lParam = (LPARAM)::_tcsdup(var.bstrVal);

								lvItem.iImage = 28;
								iItem = (int)::SendMessage(pControl->m_hWnd,LVM_INSERTITEM,0,(LPARAM)&lvItem);

								pElement->Release();
							}
							pNodeList->Release();
						}else
						{
							if(pData && nLen > 0)
								xfUriLoader::FreeData(pData);
							return false;
						}
						if(pData && nLen > 0)
							xfUriLoader::FreeData(pData);

					}
					if(bDestroy)::SysFreeString(bstr);
				}
			}
		}
	}
	return true;
}

int CTemplateDlg::GetLevel(HTREEITEM hItem)
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


