#include "StdAfx.h"
#include "WorkNodeDlg.h"
#include "xoffice.h"

CWorkNodeDlg::CWorkNodeDlg(void):m_pDocElement(NULL),m_pStrID(NULL)
{
}

CWorkNodeDlg::~CWorkNodeDlg(void)
{
	if(m_pStrID)delete m_pStrID;
	if(m_pDocElement)m_pDocElement->Release();
}


int CWorkNodeDlg::Initial(IXMLDOMElement * pElement)
{
	m_pDocElement = pElement;
	m_pDocElement->AddRef();
	_variant_t var;
	pElement->getAttribute(L"nodetype",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("nodetype"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"nodedetail",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("nodedetail"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"entry",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("entry"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"entryguid",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("entryguid"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"workguid",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("workguid"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"package",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("package"))->m_hWnd,var.bstrVal);

	pElement->getAttribute(L"xpage",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("xpage"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"xpageguid",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("xpageguid"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"xpageworkguid",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("xpageworkguid"))->m_hWnd,var.bstrVal);

	pElement->getAttribute(L"xpage1",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("xpage1"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"xpageguid1",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("xpageguid1"))->m_hWnd,var.bstrVal);
	pElement->getAttribute(L"xpageworkguid1",&var);
	if(var.bstrVal)SetWindowText(GetControl(_T("xpageworkguid1"))->m_hWnd,var.bstrVal);
	
	return 1;
}

int CWorkNodeDlg::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmCancel"))==0 )
	{
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmPreview"))==0)
	{

		LPTSTR pPackage = NULL;
		LPTSTR pWorkGuid = NULL;
		LPTSTR pWorkGuidEx = NULL;
		LPTSTR pEntryGuid = NULL;
		LPTSTR pStrXPage = NULL;
		LPTSTR pStrXPageGuid = NULL;
		LPTSTR pStrXPageWorkGuid = NULL;

		TCHAR pText[255];
		::GetWindowText(GetControl(_T("entryguid"))->m_hWnd,pText,255);
		pEntryGuid = _tcsdup(pText);
		::GetWindowText(GetControl(_T("package"))->m_hWnd,pText,255);
		pPackage = _tcsdup(pText);
		::GetWindowText(GetControl(_T("workguid"))->m_hWnd,pText,255);
		pWorkGuid = _tcsdup(pText);

		::GetWindowText(GetControl(_T("xpage"))->m_hWnd,pText,255);
		pStrXPage =  _tcsdup(pText);
		::GetWindowText(GetControl(_T("xpageguid"))->m_hWnd,pText,255);
		pStrXPageGuid =  _tcsdup(pText);
		::GetWindowText(GetControl(_T("xpageworkguid"))->m_hWnd,pText,255);
		pStrXPageWorkGuid =  _tcsdup(pText);

		::GetWindowText(GetControl(_T("xpage1"))->m_hWnd,pText,255);
		pStrXPage =  _tcsdup(pText);
		::GetWindowText(GetControl(_T("xpageguid1"))->m_hWnd,pText,255);
		pStrXPageGuid =  _tcsdup(pText);
		::GetWindowText(GetControl(_T("xpageworkguid1"))->m_hWnd,pText,255);
		pStrXPageWorkGuid =  _tcsdup(pText);

		if(pStrXPage && pStrXPage[0])
		{
			xfControl * pTree = pControl->GetWin()->m_pParentWin->GetControl(_T("DevExplorer"));

			TVITEM	tvItem;
			HTREEITEM hItem = (HTREEITEM)m_pParam;
			BSTR bstrTagName;
			HTREEITEM hpItem = hItem;
			IXMLDOMElement* pElementEx;
findworkitems:	hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hpItem;
			TreeView_GetItem(pTree->m_hWnd,&tvItem);
			pElementEx = (IXMLDOMElement* )(tvItem.lParam);
			pElementEx->get_tagName(&bstrTagName);
			if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto findworkitems;

			IXMLDOMElement * pWorkElement = (IXMLDOMElement* )(tvItem.lParam);
			_variant_t var;
			pWorkElement->getAttribute(L"guid",&var);
			if(var.bstrVal)pWorkGuidEx = _tcsdup(var.bstrVal);

			int nLevel = GetLevel(pTree->m_hWnd,hpItem);
			for(int i=nLevel;i >2;i--)
			{
				hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
			}
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hpItem;
			TreeView_GetItem(pTree->m_hWnd,&tvItem);
			LPTSTR pHxdSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));

			LPTSTR pSrc = NULL;
			pSrc = xbXml::GetStringAttribe(pWorkElement,_T("src"));

			int nPos = (int)_tcslen(pHxdSrc);
			while(pHxdSrc[--nPos]!='/');
			if(nPos >=0) pHxdSrc[nPos]='\0';
			TCHAR buf[MAX_PATH];
			_stprintf_s(buf,MAX_PATH,_T("/developer/%s/%s"),pHxdSrc,pSrc);
			if(pHxdSrc) delete pHxdSrc;
			if(pSrc) delete pSrc;

			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,buf,strUrl,&dwSize,0);

			BYTE* pData;
			int nLen = 0;
			xbXml WorkXml;
			if(xfUriLoader::Load(strUrl, NULL,pData,nLen)==1)
			{
				TCHAR Path[MAX_PATH];
				xfUriLoader::GetCacheFileName(strUrl,Path);
				WorkXml.Load(Path);

				TCHAR xPath[MAX_PATH];
				IXMLDOMNode * pNode = NULL;
				_stprintf_s(xPath,MAX_PATH,_T("//file[@type='xpage' and @caption='%s']"),pStrXPage);

				WorkXml.m_pXmlDoc->selectSingleNode(xPath,&pNode);
				BSTR bstr;
				WorkXml.m_pXmlDoc->get_xml(&bstr);

				if(pNode)
				{
					IXMLDOMElement * pElement;
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pElement->getAttribute(L"guid",&var);
					if(var.bstrVal)
					{
						LPTSTR pItemGuid = _tcsdup(var.bstrVal);
						TCHAR strCmd[512];
						xbXml xml;
						::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdoc %s %s %s"),	pHxdSrc,pWorkGuidEx ,pItemGuid);
						if(pItemGuid)delete pItemGuid;
						if(pWorkGuidEx)delete pWorkGuidEx;
						if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
						{
							IXMLDOMElement * pElementEx;
							xml.m_pXmlDoc->get_documentElement(&pElementEx);
							if(pElementEx)
							{
								xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,_T("/developer/xpage.xml"),strUrl,&dwSize,0);

								xfXPage * pXPage = new xfXPage();
								pXPage->m_pStrUrl = _tcsdup(strUrl);
								pXPage->Load(pElementEx);
								pElementEx->Release();
								xfWin* pWin = xfWin::LoadXPageXml(pXPage,m_hWnd,xfApp::GetApp()->GetInstance());
							}
						}
					}
					pElement->Release();
				}
				if(pNode)pNode->Release();
			}
		}
		if(pEntryGuid && pEntryGuid[0])
		{
			TCHAR strCmd[512];
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdoc %s %s %s"),	pPackage,pWorkGuid ,pEntryGuid);
			
			/*if(pPackage)delete pPackage;
			if(pWorkGuid)delete pWorkGuid;
			if(pEntryGuid)delete pEntryGuid;*/

			xbXml xml;
			if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)!=1) return 1;

			IXMLDOMNodeList*	pList;
			IXMLDOMNode*		pNode;
			IXMLDOMElement*		pSheet;
			long nLen;
			std::wstring guids;
			_variant_t var;
			xml.m_pXmlDoc->selectNodes(_T("entry/edit/schema[1]/sheet"),&pList);
			pList->get_length(&nLen);
			for(int i=0;i<nLen;i++)
			{
				pList->get_item(i,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSheet);
				pNode->Release();
				pSheet->getAttribute(L"sheetguid",&var);
				pSheet->Release();
				if(var.bstrVal)
					guids += var.bstrVal;
			}
			pList->Release();
			if(nLen > 0)
			{
				TCHAR strCmd[512];
				::_stprintf_s(strCmd,512, _T("uri.hxsoft.com/xaserver/developer getdocbysheet %s"),	guids.c_str());
				if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)!=1) return 1;
				
				xfWin * pWinEx = xfWin::LoadXPage(NULL,_T("entryview.xml"),GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance());
				XOffice::XOfficeFrm * pControl = (XOffice::XOfficeFrm *)pWinEx->GetControl(_T("xoffice"));
				IXMLDOMElement * pDocElement;
				xml.m_pXmlDoc->get_documentElement(&pDocElement);
				if(pDocElement)
				{
					pControl->m_pFrameSvr->m_pSerializeSvr->LoadSheetData(NULL,pDocElement,pControl->m_pFrameSvr->m_pDataSvr);
					pControl->m_pFrameSvr->SelectSheet(0);
					pControl->m_pFrameSvr->SetFRect(pControl->m_pFrameSvr->FRect);
					::InvalidateRect(pControl->m_hWnd,NULL,true);
					pDocElement->Release();
				}
			}
		}
		if(pEntryGuid)delete pEntryGuid;
		if(pPackage)delete pPackage;
		if(pWorkGuid)delete pWorkGuid;
		if(pStrXPage)delete pStrXPage;
		if(pStrXPageGuid)delete pStrXPageGuid;
		if(pStrXPageWorkGuid)delete pStrXPageWorkGuid;
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmOkWorkNode"))==0 )
	{
		xfControl * pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
		if(!TreeView_GetParent(pTree->m_hWnd,hItem))
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择实体对象"),_T("提示"),MB_OK);
			return 1;
		}
		IXMLDOMElement * pElement;
		TVITEM	item;
		item.hItem = hItem;
		item.mask = TVIF_PARAM;
		TreeView_GetItem(pTree->m_hWnd,&item);
		pElement = (IXMLDOMElement *)item.lParam;
		_variant_t	var;
		pElement->getAttribute(L"type",&var);
		if(_tcsicmp(var.bstrVal,_T("entry"))!=0)
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择实体对象"),_T("提示"),MB_OK);
			return 1;
		}

		LPTSTR pSrc  = NULL;
		LPTSTR pStrGuid = NULL;
		LPTSTR pStrGuidItem = NULL;
		LPTSTR pCaption = NULL;

		pElement->getAttribute(L"caption",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("entry"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"guid",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("entryguid"))->m_hWnd,var.bstrVal);

		HTREEITEM hpItem = TreeView_GetParent(pTree->m_hWnd, hItem);
		hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
		item.mask = TVIF_PARAM|TVIF_HANDLE;
		item.hItem = hpItem;
		TreeView_GetItem(pTree->m_hWnd,&item);

		pElement = (IXMLDOMElement* )(item.lParam);
		pElement->getAttribute(L"guid",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("workguid"))->m_hWnd,var.bstrVal);
		int nLevel = GetLevel(pTree->m_hWnd,hpItem);
		for(int i=nLevel;i >1;i--)
		{
			hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
		}
		item.mask = TVIF_PARAM|TVIF_HANDLE;
		item.hItem = hpItem;
		TreeView_GetItem(pTree->m_hWnd,&item);
		pSrc = xbXml::GetStringAttribe((IXMLDOMElement*)item.lParam,_T("src"));;
		SetWindowText(GetControl(_T("package"))->m_hWnd,pSrc);
		if(pSrc)delete pSrc;


		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmWorkNodeSelect"))==0 )
	{
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("entityselectdlg.xpage"),strUrl,&dwSize,0);

		if(m_pStrID)delete m_pStrID;
		m_pStrID = _tcsdup(pStrID);

		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmOkWorkNodeEx"))==0 )
	{
		xfControl * pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
		if(!TreeView_GetParent(pTree->m_hWnd,hItem))
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择XPage对象"),_T("提示"),MB_OK);
			return 1;
		}
		
		bool bXPage1 =false;
		if(_tcsicmp(m_pStrID,_T("xmWorkNodeSelectEx1"))==0)bXPage1 = true;

		IXMLDOMElement * pElement;
		TVITEM	item;
		item.hItem = hItem;
		item.mask = TVIF_PARAM;
		TreeView_GetItem(pTree->m_hWnd,&item);
		pElement = (IXMLDOMElement *)item.lParam;
		_variant_t	var;
		pElement->getAttribute(L"type",&var);
		if(_tcsicmp(var.bstrVal,_T("xpage"))!=0)
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择XPage对象"),_T("提示"),MB_OK);
			return 1;
		}

		LPTSTR pSrc  = NULL;
		LPTSTR pStrGuid = NULL;
		LPTSTR pStrGuidItem = NULL;
		LPTSTR pCaption = NULL;

		if(!bXPage1)
		{
			pElement->getAttribute(L"caption",&var);
			if(var.bstrVal)SetWindowText(GetControl(_T("xpage"))->m_hWnd,var.bstrVal);
			pElement->getAttribute(L"guid",&var);
			if(var.bstrVal)SetWindowText(GetControl(_T("xpageguid"))->m_hWnd,var.bstrVal);
		}else
		{
			pElement->getAttribute(L"caption",&var);
			if(var.bstrVal)SetWindowText(GetControl(_T("xpage1"))->m_hWnd,var.bstrVal);
			pElement->getAttribute(L"guid",&var);
			if(var.bstrVal)SetWindowText(GetControl(_T("xpageguid1"))->m_hWnd,var.bstrVal);
		}

		HTREEITEM hpItem = TreeView_GetParent(pTree->m_hWnd, hItem);
		hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
		hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
		hpItem = TreeView_GetParent(pTree->m_hWnd, hpItem);
		item.mask = TVIF_PARAM|TVIF_HANDLE;
		item.hItem = hpItem;
		TreeView_GetItem(pTree->m_hWnd,&item);

		pElement = (IXMLDOMElement* )(item.lParam);
		pElement->getAttribute(L"guid",&var);
		if(!bXPage1)
			if(var.bstrVal)SetWindowText(GetControl(_T("xpageworkguid"))->m_hWnd,var.bstrVal);
		else
			if(var.bstrVal)SetWindowText(GetControl(_T("xpageworkguid1"))->m_hWnd,var.bstrVal);
		
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmWorkNodeSelectEx"))==0 )
	{
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("xpageSelectdlg.xpage"),strUrl,&dwSize,0);

		if(m_pStrID)delete m_pStrID;
		m_pStrID = _tcsdup(pStrID);

		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmWorkNodeSelectEx1"))==0 )
	{
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("xpageSelectdlg.xpage"),strUrl,&dwSize,0);

		if(m_pStrID)delete m_pStrID;
		m_pStrID = _tcsdup(pStrID);

		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmWorkNodeSave"))==0 && m_pDocElement)
	{
		TCHAR pText[255];
		::GetWindowText(GetControl(_T("nodetype"))->m_hWnd,pText,255); 
		m_pDocElement->setAttribute(L"nodetype",(_variant_t)pText);
		::GetWindowText(GetControl(_T("nodedetail"))->m_hWnd,pText,255);
		m_pDocElement->setAttribute(L"nodedetail",(_variant_t)pText);

		::GetWindowText(GetControl(_T("entry"))->m_hWnd,pText,255);
		if(pText[0])
		{
			m_pDocElement->setAttribute(L"entry",(_variant_t)pText);
			::GetWindowText(GetControl(_T("entryguid"))->m_hWnd,pText,255);
			m_pDocElement->setAttribute(L"entryguid",(_variant_t)pText);
			::GetWindowText(GetControl(_T("package"))->m_hWnd,pText,255);
			m_pDocElement->setAttribute(L"package",(_variant_t)pText);
			::GetWindowText(GetControl(_T("workguid"))->m_hWnd,pText,255);
			m_pDocElement->setAttribute(L"workguid",(_variant_t)pText);
		}else
		{
			m_pDocElement->setAttribute(L"entry",(_variant_t)_T(""));
			m_pDocElement->setAttribute(L"entryguid",(_variant_t)_T(""));
			m_pDocElement->setAttribute(L"package",(_variant_t)_T(""));
			m_pDocElement->setAttribute(L"workguid",(_variant_t)_T(""));
		}

		::GetWindowText(GetControl(_T("xpage"))->m_hWnd,pText,255);
		if(pText[0])
		{
			m_pDocElement->setAttribute(L"xpage",(_variant_t)pText);
			::GetWindowText(GetControl(_T("xpageguid"))->m_hWnd,pText,255);
			m_pDocElement->setAttribute(L"xpageguid",(_variant_t)pText);
			::GetWindowText(GetControl(_T("xpageworkguid"))->m_hWnd,pText,255);
			m_pDocElement->setAttribute(L"xpageworkguid",(_variant_t)pText);
		}else
		{
			m_pDocElement->setAttribute(L"xpage",(_variant_t)_T(""));
			m_pDocElement->setAttribute(L"xpageguid",(_variant_t)_T(""));
			m_pDocElement->setAttribute(L"xpageworkguid",(_variant_t)_T(""));
		}

		::GetWindowText(GetControl(_T("xpage1"))->m_hWnd,pText,255);
		if(pText[0])
		{
			m_pDocElement->setAttribute(L"xpage1",(_variant_t)pText);
			::GetWindowText(GetControl(_T("xpageguid1"))->m_hWnd,pText,255);
			m_pDocElement->setAttribute(L"xpageguid1",(_variant_t)pText);
			::GetWindowText(GetControl(_T("xpageworkguid1"))->m_hWnd,pText,255);
			m_pDocElement->setAttribute(L"xpageworkguid1",(_variant_t)pText);
		}else
		{
			m_pDocElement->setAttribute(L"xpage1",(_variant_t)_T(""));
			m_pDocElement->setAttribute(L"xpageguid1",(_variant_t)_T(""));
			m_pDocElement->setAttribute(L"xpageworkguid1",(_variant_t)_T(""));
		}

		this->m_pParentWin->OnXCommand(_T("xmFileSave"),pControl);
	}
	return 0;
}
int CWorkNodeDlg::OnOpen()
{
	return 1;
}

int CWorkNodeDlg::GetLevel(HWND hWnd,HTREEITEM hItem)
{
	int level = 0;
	HTREEITEM hpItem;
	hpItem = hItem;
	while(hpItem)
	{
		level ++;
		hpItem = TreeView_GetParent(hWnd, hpItem);
	}
	return level;
}
