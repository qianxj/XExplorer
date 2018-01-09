#include "StdAfx.h"
#include "DevSchema.h"
#include "xreport.h"
#include "xoffice.h"
#include "templatedlg.h"

CDevSchema::CDevSchema(void):m_pDocElement(NULL)
{
	this->AttachListon(WM_NOTIFY,(FNEvent)&CDevSchema::EvtNotify);
}

CDevSchema::~CDevSchema(void)
{
	this->ClearAllListonsTo();
	if(m_pDocElement)m_pDocElement->Release();
}

int SchemaContentSvr::GetTreeData(vector<TTreeItem> * & pTreeData)
{
	pTreeData = &treeItems;
	return 1;
}

int MakeData(IXMLDOMNode * pNode,SchemaContentSvr * pContentSvr )
{
	IXMLDOMElementPtr pSchemaElement;
	pContentSvr->treeItems.clear();
	if(pNode)
	{
		pSchemaElement = pNode;
		pContentSvr->Load(pSchemaElement);
		XOffice::XXmlContentSvr::TTreeItem item;
		for(int i=0;i<pContentSvr->GetRowCount();i++)
		{
			IXMLDOMElement * pElement;
			_variant_t val;
			pElement = pContentSvr->GetRowElement(i + 1);
			pElement->getAttribute(L"type",&val);
			if(val.bstrVal && (::_tcsicmp(val.bstrVal,_T("单行集合"))==0 || ::_tcsicmp(val.bstrVal,_T("多行集合"))==0))
			{
				item.children = true;
				item.expanded = true;
				item.nImage = 22;
			}else
			{
				item.children =  false;
				item.expanded = true;
				item.nImage = 5;
			}
			item.nContentCol = 1;
			item.nContentRow = i + 1;
			item.nImageList = 0;
			item.nLevel = 1;
			if(i>0)
			{
				IXMLDOMNode * pNode;
				IXMLDOMNode * pParentNode;
				pElement->get_parentNode(&pParentNode);
				while(pParentNode)
				{
					item.nLevel++;
					pNode = pParentNode;
					pNode->get_parentNode(&pParentNode);
					pNode->Release();
				}
				item.nLevel--;
			}
			item.nSelectedImage = -1;
			pContentSvr->treeItems.push_back(item);
			pElement->Release();
		}
	}
	return 1;
}

int CDevSchema::Initial(IXMLDOMElement * pElement)
{
	m_pDocElement = pElement;
	pElement->AddRef();

	XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
	SchemaContentSvr * pContentSvr = (SchemaContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;

	IXMLDOMNode * pNode;
	m_pDocElement->selectSingleNode(L"schema/*[1]",&pNode);
	if(pNode)
	{
		MakeData(pNode,pContentSvr);
		pNode->Release();
	}else
	{
		IXMLDOMElementPtr pElement;
		IXMLDOMDocumentPtr pDoc;
		m_pDocElement->get_ownerDocument(&pDoc);
		pDoc->createElement(L"schema",&pElement);
		m_pDocElement->appendChild(pElement,NULL);
	}

	m_pDocElement->selectSingleNode(L"list/schema",&pNode);
	if(!pNode)
	{
		IXMLDOMNode * pNode;
		IXMLDOMElement	*pElement;
		IXMLDOMElement	*pElementEx;
		IXMLDOMDocumentPtr pDoc;
		m_pDocElement->get_ownerDocument(&pDoc);
		pDoc->selectSingleNode(L"list/schema",&pNode);
		if(!pNode)
		{
			pDoc->createElement(L"list",&pElement);
			pDoc->createElement(L"schema",&pElementEx);
			pElementEx->setAttribute(L"caption",(_variant_t)L"常用");
			pElement->appendChild(pElementEx,NULL);
			pElementEx->Release();
			m_pDocElement->appendChild(pElement,NULL);
			pElement->Release();
		}
	}else
		pNode->Release();


	::InvalidateRect(pReportCtl ->m_hWnd,NULL,false);
	//edittree
	xcTreeCtrl * pEditTree = (xcTreeCtrl *)GetControl(_T("edittree"));
	xcTreeXmlDataSource * pSource = new xcTreeXmlDataSource();
	m_pDocElement->AddRef();
	pSource->m_pXmlElement = m_pDocElement;
	pSource->m_strXSrc = ::_tcsdup(_T("."));
	pSource->m_strXRootPath=::_tcsdup(_T("edit"));
	pSource->m_strXPath=::_tcsdup(_T("schema"));
	pSource->m_bLoop = true;
	pSource->m_strXText = ::_tcsdup(_T("@caption"));
	pSource->m_strXData = ::_tcsdup(_T("."));
	pSource->m_strXImage =::_tcsdup(_T("@image"));
	pSource->m_RLevel = 1;
	pSource->m_hItem = TVI_ROOT;
	pEditTree->m_pTreeSource->push_back(pSource);
	pEditTree->ExpandAll();

	HTREEITEM hItem = TreeView_GetRoot(GetControl(_T("edittree"))->m_hWnd);
	TreeView_Expand(GetControl(_T("edittree"))->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL  );
	//listtree
	pEditTree = (xcTreeCtrl *)GetControl(_T("listtree"));
	pSource = new xcTreeXmlDataSource();
	m_pDocElement->AddRef();
	pSource->m_pXmlElement = m_pDocElement;
	pSource->m_strXSrc = ::_tcsdup(_T("."));
	pSource->m_strXRootPath=::_tcsdup(_T("list"));
	pSource->m_strXPath=::_tcsdup(_T("schema"));
	pSource->m_bLoop = true;
	pSource->m_strXText = ::_tcsdup(_T("@caption"));
	pSource->m_strXData = ::_tcsdup(_T("."));
	pSource->m_strXImage =::_tcsdup(_T("@image"));
	pSource->m_RLevel = 1;
	pSource->m_hItem = TVI_ROOT;
	pEditTree->m_pTreeSource->push_back(pSource);
	pEditTree->ExpandAll();

	hItem = TreeView_GetRoot(GetControl(_T("listtree"))->m_hWnd);
	TreeView_Expand(GetControl(_T("listtree"))->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL  );

	m_pDocElement->selectSingleNode(L"Serialize",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
		pNode->Release();
		ShowSerialize(pElement);
		pElement->Release();
	}

	variant_t var;
	m_pDocElement->getAttribute(L"dburi",&var);
	if (var.bstrVal)
		SetWindowText(GetControl(_T("dburi"))->m_hWnd,var.bstrVal);

	return 1;
}

int CDevSchema::ShowSerialize(IXMLDOMElement *pElement)
{
	IXMLDOMNode * pNode;
	IXMLDOMElement * pItem;
	_variant_t var;

	pElement->selectSingleNode(L"preprocess",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
		pNode->Release();

		pItem->getAttribute(L"link",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("prelink"))->m_hWnd,var.bstrVal);
		pItem->getAttribute(L"method",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("premethod"))->m_hWnd,var.bstrVal);
		pItem->getAttribute(L"url",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("preurl"))->m_hWnd,var.bstrVal);

		pItem->Release();
	}

	pElement->selectSingleNode(L"process",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
		pNode->Release();

		pItem->getAttribute(L"link",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("link"))->m_hWnd,var.bstrVal);
		pItem->getAttribute(L"method",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("method"))->m_hWnd,var.bstrVal);
		pItem->getAttribute(L"url",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("url"))->m_hWnd,var.bstrVal);

		pItem->Release();
	}

	pElement->selectSingleNode(L"postprocess",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
		pNode->Release();

		pItem->getAttribute(L"link",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("postlink"))->m_hWnd,var.bstrVal);
		pItem->getAttribute(L"method",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("postmethod"))->m_hWnd,var.bstrVal);
		pItem->getAttribute(L"url",&var);
		if(var.bstrVal)SetWindowText(GetControl(_T("posturl"))->m_hWnd,var.bstrVal);

		pItem->Release();
	}

	return 1;
}

int CDevSchema::GetSerializeXml(IXMLDOMElement *pElement)
{
	IXMLDOMDocument * pDocument;
	IXMLDOMNode * pNode;
	IXMLDOMElement * pItem;

	TCHAR link[60];
	TCHAR method[60];
	TCHAR url[1024];

	pElement->get_ownerDocument(&pDocument);
	pElement->selectSingleNode(L"preprocess",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
		pNode->Release();
	}else
	{
		pDocument->createElement(L"preprocess",&pItem);
		pElement->appendChild(pItem,NULL);
	}

	GetWindowText(GetControl(_T("prelink"))->m_hWnd,link,60);
	GetWindowText(GetControl(_T("premethod"))->m_hWnd,method,60);
	GetWindowText(GetControl(_T("preurl"))->m_hWnd,url,1024);

	pItem->setAttribute(L"link",(_variant_t)link);
	pItem->setAttribute(L"method",(_variant_t)method);
	pItem->setAttribute(L"url",(_variant_t)url);

	pItem->Release();

	pElement->selectSingleNode(L"process",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
		pNode->Release();
	}else
	{
		pDocument->createElement(L"process",&pItem);
		pElement->appendChild(pItem,NULL);
	}

	GetWindowText(GetControl(_T("link"))->m_hWnd,link,60);
	GetWindowText(GetControl(_T("method"))->m_hWnd,method,60);
	GetWindowText(GetControl(_T("url"))->m_hWnd,url,1024);

	pItem->setAttribute(L"link",(_variant_t)link);
	pItem->setAttribute(L"method",(_variant_t)method);
	pItem->setAttribute(L"url",(_variant_t)url);

	pItem->Release();

	pElement->selectSingleNode(L"postprocess",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pItem);
		pNode->Release();
	}else
	{
		pDocument->createElement(L"postprocess",&pItem);
		pElement->appendChild(pItem,NULL);
	}

	GetWindowText(GetControl(_T("postlink"))->m_hWnd,link,60);
	GetWindowText(GetControl(_T("postmethod"))->m_hWnd,method,60);
	GetWindowText(GetControl(_T("posturl"))->m_hWnd,url,1024);

	pItem->setAttribute(L"link",(_variant_t)link);
	pItem->setAttribute(L"method",(_variant_t)method);
	pItem->setAttribute(L"url",(_variant_t)url);

	pItem->Release();
	pDocument->Release();

	return 1;
}


int CDevSchema::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmEntrySave"))==0)
	{
		IXMLDOMNode * pNode;
		IXMLDOMElement * pElement;
		m_pDocElement->selectSingleNode(L"Serialize",&pNode);
		if(pNode)
		{
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pNode->Release();
		}else
		{
			IXMLDOMDocument * pDocument;
			m_pDocElement->get_ownerDocument(&pDocument);
			pDocument->createElement(L"Serialize",&pElement);
			m_pDocElement->appendChild(pElement,NULL);
			pDocument->Release();
		}
		GetSerializeXml(pElement);
		pElement->Release();

		TCHAR pText[255];
		GetWindowText(GetControl(_T("dburi"))->m_hWnd,pText,255);
		m_pDocElement->setAttribute(_T("dburi"),(_variant_t)pText);

		this->m_pParentWin->OnXCommand(_T("xmFileSave"),pControl);
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmAddMode"))==0)
	{
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("templatedlgex.xpage"),strUrl,&dwSize,0);


		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);

		return 1;
	}
	if(pStrID && (_tcsicmp(pStrID,_T("xmAddSheet"))==0 || _tcsicmp(pStrID,_T("xmAddSheetRef"))==0 || _tcsicmp(pStrID,_T("xmAddList"))==0))
	{
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("templatedlg.xpage"),strUrl,&dwSize,0);


		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),(LPVOID)_tcsdup(pStrID),this);
		return 1;
	}
	//删除模式	删除页面	删除页面选项 删除列表
	if(pStrID &&  ((_tcsicmp(pStrID,_T("xmDeleteMode"))==0)||(_tcsicmp(pStrID,_T("xmDeleteSheet"))==0)||(_tcsicmp(pStrID,_T("xmDeleteSheetRef"))==0) ||(_tcsicmp(pStrID,_T("xmDeleteList"))==0)))
	{
		xfControl* pEtControl ;
		if (_tcsicmp(pStrID,_T("xmDeleteList"))==0)
			pEtControl= GetControl(_T("listtree"));
		else
			pEtControl= GetControl(_T("edittree"));

		HTREEITEM	hItem = TreeView_GetSelection(pEtControl->m_hWnd);
		if(!hItem)hItem = TreeView_GetRoot(pEtControl->m_hWnd);
		if(!hItem) return 1;

		int nLevel = GetLevel(pEtControl,hItem);

		if(_tcsicmp(pStrID,_T("xmDeleteMode"))==0)
		{
			if(nLevel != 1) return 1;
			if (::MessageBox(pControl->GetWin()->m_hWnd,_T("是否确定删除指定的模式?"),_T("提示"),MB_YESNO) != IDYES) return 1;
		}

		if(_tcsicmp(pStrID,_T("xmDeleteSheet"))==0)
		{
			if(nLevel != 2) return 1;
			if (::MessageBox(pControl->GetWin()->m_hWnd,_T("是否确定删除指定的页面?"),_T("提示"),MB_YESNO) != IDYES) return 1;
		}

		if(_tcsicmp(pStrID,_T("xmDeleteSheetRef"))==0)
		{
			if(nLevel != 3) return 1;
			if (::MessageBox(pControl->GetWin()->m_hWnd,_T("是否确定删除指定的页面选项?"),_T("提示"),MB_YESNO) != IDYES) return 1;
		}

		if(_tcsicmp(pStrID,_T("xmDeleteList"))==0)
		{
			if(nLevel != 2) return 1;
			if (::MessageBox(pControl->GetWin()->m_hWnd,_T("是否确定删除指定的列表?"),_T("提示"),MB_YESNO) != IDYES) return 1;
		}

		TVITEM  tvItem;
		tvItem.hItem = hItem;
		tvItem.mask = TVIF_PARAM;
		TreeView_GetItem(pEtControl->m_hWnd,&tvItem);
		IXMLDOMElement * pElement = (IXMLDOMElement *)tvItem.lParam;
		IXMLDOMNode * pXmlNode;
		pElement->get_parentNode(&pXmlNode);
		pXmlNode->removeChild(pElement,NULL);
		pElement->Release();
		pXmlNode->Release();
		TreeView_DeleteItem(pEtControl->m_hWnd,hItem);

		return 1;
	}


	if(pStrID && (_tcsicmp(pStrID,_T("additem"))==0 || _tcsicmp(pStrID,_T("insertitem"))==0 ||
		 _tcsicmp(pStrID,_T("addsubitem"))==0 || _tcsicmp(pStrID,_T("modifyitem"))==0))
	{
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("Schemadlg.xpage"),strUrl,&dwSize,0);

		
		RECT rt = pControl->m_pxfNode->m_AreaRect;
		MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),(LPVOID)_tcsdup(pStrID),this);
		return 1;
	}
	if(pStrID && (_tcsicmp(pStrID,_T("deleteitem"))==0 ))
	{
		if (::MessageBox(pControl->GetWin()->m_hWnd,_T("是否确定删除指定的项?"),_T("提示"),MB_YESNO) != IDYES) return 1;
		XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
		SchemaContentSvr * pContentSvr = (SchemaContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;
	
		int row = ((XOffice::XReport::XReportSheetSvr *)pReportCtl->m_pSheetSvr)->GetRow();
		if (row < 1) return 1;

		std::vector<XOffice::XXmlContentSvr::TTreeItem >* pData;
		pContentSvr->GetTreeData(pData);
		int nIndex = pContentSvr->GetTreeIndex(pData,row);

		TCHAR xPath[255];
		IXMLDOMNode* pNode,*pParentNode;
		_stprintf_s(xPath,255,_T("(schema//*)[%i]"),nIndex + 1);
		m_pDocElement->selectSingleNode(xPath,&pNode);
		pNode->get_parentNode(&pParentNode);
		pParentNode->removeChild(pNode,NULL);
		pNode->Release();
		pParentNode->Release();

		m_pDocElement->selectSingleNode(_T("schema/*[1]"),&pNode);
		IXMLDOMElementPtr pSchemaElement = pNode;
		pContentSvr->Load(pSchemaElement);

		MakeData(pNode,pContentSvr);
		pNode->Release();		
		::InvalidateRect(pReportCtl ->m_hWnd,NULL,false);

		return 1;
	}

	
	if(pStrID && _tcsicmp(pStrID,_T("template_addmode"))==0) //send by Stemplatedlg
	{
		xfControl* pEtControl = GetControl(_T("edittree"));
		TCHAR pText[255];
		GetWindowText(pControl->GetWin()->GetControl(_T("modename"))->m_hWnd,pText,255);

		IXMLDOMDocumentPtr pDoc;
		IXMLDOMElement* pMode;
		m_pDocElement->get_ownerDocument(&pDoc);

		pDoc->createElement(L"schema",&pMode);
		pMode->setAttribute(L"caption",(_variant_t)pText);
		pMode->setAttribute(L"image",(_variant_t)15);

		IXMLDOMNodePtr pNode;
		IXMLDOMElementPtr pEditElement;
		m_pDocElement->selectSingleNode(L"edit",&pNode);
		if(!pNode)
		{
			pDoc->createElement(L"edit",&pEditElement);
			m_pDocElement->appendChild(pEditElement,NULL);
			
		}else
			pEditElement = pNode;
		pEditElement->appendChild(pNode,NULL);

		TVINSERTSTRUCT tvins;
		tvins.hInsertAfter = TVI_LAST;
		tvins.hParent = TVI_ROOT;
		tvins.item.pszText = pText;
		tvins.item.cchTextMax = 255;
		tvins.item.lParam = (LPARAM)pMode;
		tvins.item.iImage = 15;
		tvins.item.iSelectedImage = 15;
		tvins.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		TreeView_InsertItem(pEtControl->m_hWnd,&tvins);
		

		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("template_addsheet"))==0) //send by Stemplatedlg
	{

		xfControl* pEtControl ;
		if (_tcsicmp((const wchar_t *)pControl->GetWin()->m_pParam,_T("xmAddList"))==0)
			pEtControl= GetControl(_T("listtree"));
		else
			pEtControl= GetControl(_T("edittree"));

		HTREEITEM	hItem = TreeView_GetSelection(pEtControl->m_hWnd);
		if(!hItem)hItem = TreeView_GetRoot(pEtControl->m_hWnd);
		if(!hItem) return 1;

		xfControl* pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
		xfControl* pView = pControl->GetWin()->GetControl(_T("sheetview"));

		TCHAR pText[255];
		GetWindowText(pControl->GetWin()->GetControl(_T("sheetname"))->m_hWnd,pText,255);
		int nIndex = ListView_GetNextItem(pView->m_hWnd,-1,LVNI_SELECTED);
		int nLevel = GetLevel(pEtControl,hItem);
		if(pControl->GetWin()->m_pParam &&  _tcsicmp((LPTSTR)pControl->GetWin()->m_pParam,_T("xmAddSheetRef"))==0)
		{
			if(nLevel < 2) return 1;
		}
		if(nIndex >=0)
		{
			
			LVITEM	lvItem;
			lvItem.iItem = nIndex;
			lvItem.iSubItem = 0;
			lvItem.mask = LVIF_PARAM;
			ListView_GetItem(pView->m_hWnd,&lvItem);
			IXMLDOMDocumentPtr pDoc;
			IXMLDOMElement* pSheet;
			m_pDocElement->get_ownerDocument(&pDoc);


			pDoc->createElement(L"sheet",&pSheet);
			pSheet->setAttribute(L"sheetguid",(_variant_t)(LPTSTR)lvItem.lParam);
			pSheet->setAttribute(L"caption",(_variant_t)pText);
			pSheet->setAttribute(L"image",(_variant_t)23);

			if(pControl->GetWin()->m_pParam &&  _tcsicmp((LPTSTR)pControl->GetWin()->m_pParam,_T("xmAddSheetRef"))==0)
			{
				while(nLevel-- > 2)hItem = TreeView_GetParent(pEtControl->m_hWnd,hItem);
			}
			else
				while(nLevel-- > 1)hItem = TreeView_GetParent(pEtControl->m_hWnd,hItem);
			TVITEMEX	tvItem;
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pEtControl->m_hWnd,&tvItem);
			IXMLDOMElement* pElement = (IXMLDOMElement*)tvItem.lParam;
			pElement->appendChild(pSheet,NULL);
			TVINSERTSTRUCT tvins;
			tvins.hInsertAfter = TVI_LAST;
			tvins.hParent = hItem;
			tvins.item.pszText = pText;
			tvins.item.cchTextMax = 255;
			tvins.item.lParam = (LPARAM)pSheet;
			tvins.item.iImage = 23;
			tvins.item.iSelectedImage = 23;
			tvins.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			TreeView_InsertItem(pEtControl->m_hWnd,&tvins);
			ReadSheet(pElement);

			tvItem.mask = TVIF_CHILDREN|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pEtControl->m_hWnd,&tvItem);
			if(!tvItem.cChildren)
			{
				tvItem.cChildren = true;
				TreeView_SetItem(pEtControl->m_hWnd,&tvItem);
			}
			TreeView_Expand(pEtControl->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL  );
		}
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("template_close"))==0) //send by Stemplatedlg
	{
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmSchemaOK"))==0) //send by Schemadlg
	{
		XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
		SchemaContentSvr * pContentSvr = (SchemaContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;
		
		int row = ((XOffice::XReport::XReportSheetSvr *)pReportCtl->m_pSheetSvr)->GetRow();
		
		TCHAR pText[255];

		IXMLDOMDocumentPtr  pDoc;
		IXMLDOMElementPtr pElement;
		this->m_pDocElement->get_ownerDocument(&pDoc);
		if(_tcsicmp((LPTSTR)pControl->GetWin()->m_pParam,_T("modifyitem"))!=0)
		{
			pDoc->createElement(L"item",&pElement);
			::GetWindowText(pControl->GetWin()->GetControl(_T("colName"))->m_hWnd,pText,255);
			pElement->setAttribute(_T("colname"),(_variant_t)pText);
			::GetWindowText(pControl->GetWin()->GetControl(_T("Name"))->m_hWnd,pText,255);
			pElement->setAttribute(_T("name"),(_variant_t)pText);
			::GetWindowText(pControl->GetWin()->GetControl(_T("colType"))->m_hWnd,pText,255);
			pElement->setAttribute(_T("type"),(_variant_t)pText);
			::GetWindowText(pControl->GetWin()->GetControl(_T("colRef"))->m_hWnd,pText,255);
			pElement->setAttribute(_T("colref"),(_variant_t)pText);
			::GetWindowText(pControl->GetWin()->GetControl(_T("colNote"))->m_hWnd,pText,255);
			pElement->setAttribute(_T("note"),(_variant_t)pText);
			::GetWindowText(pControl->GetWin()->GetControl(_T("dim"))->m_hWnd,pText,255);
			pElement->setAttribute(_T("dim"),(_variant_t)pText);
			::GetWindowText(pControl->GetWin()->GetControl(_T("measure"))->m_hWnd,pText,255);
			pElement->setAttribute(_T("measure"),(_variant_t)pText);
		}
		IXMLDOMNodePtr pNode;
		IXMLDOMElementPtr pSchema;

		if(row < 1 || row > pContentSvr->GetRowCount())
		{		
			this->m_pDocElement->selectSingleNode(_T("schema"),&pNode);
			pSchema = pNode;
			pSchema->appendChild(pElement,NULL);
		}
		else
		{
			std::vector<XOffice::XXmlContentSvr::TTreeItem >* pData;
			pContentSvr->GetTreeData(pData);
			int nIndex = pContentSvr->GetTreeIndex(pData,row);

			TCHAR xPath[255];
			_stprintf_s(xPath,255,_T("(schema//*)[%i]"),nIndex + 1);
			m_pDocElement->selectSingleNode(xPath,&pNode);

			IXMLDOMElementPtr pNodeElement;
			pNodeElement = pNode;
			if(_tcsicmp((LPTSTR)pControl->GetWin()->m_pParam,_T("additem"))==0)
			{
				if(m_pDocElement != pNodeElement)
					pNodeElement->get_parentNode(&pNode);
				pSchema = pNode;
				pSchema->appendChild(pElement,NULL);
			}
			if(_tcsicmp((LPTSTR)pControl->GetWin()->m_pParam,_T("insertitem"))==0)
			{
				IXMLDOMNodePtr pParentNode;
				IXMLDOMNode * p;
				if(m_pDocElement != pNodeElement)
					pNodeElement->get_parentNode(&pParentNode);
				pSchema = pParentNode;
				p = pNode;
				VARIANT v;
				v.vt = VT_DISPATCH;
				v.pdispVal = (IDispatch*)p;
				pSchema->insertBefore(pElement,v,NULL);
	//			p->Release();
			}
			if(_tcsicmp((LPTSTR)pControl->GetWin()->m_pParam,_T("addsubitem"))==0)
			{
				XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
				SchemaContentSvr * pContentSvr = (SchemaContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;

				int row = ((XOffice::XReport::XReportSheetSvr *)pReportCtl->m_pSheetSvr)->GetRow();
				
				if (row < 1) return 1;

				std::vector<XOffice::XXmlContentSvr::TTreeItem >* pData;
				pContentSvr->GetTreeData(pData);
				int nIndex = pContentSvr->GetTreeIndex(pData,row);


				LPCTSTR pType = pContentSvr->GetItemString(_T("type"),nIndex + 1);

				if (_tcsicmp(pType,_T("单行集合"))==0 || _tcsicmp(pType,_T("多行集合"))==0)
				{
					TCHAR xPath[255];
					_stprintf_s(xPath,255,_T("(schema//*)[%i]"),nIndex + 1);
					m_pDocElement->selectSingleNode(xPath,&pNode);
					pNode->appendChild(pElement,NULL);
				}
				else
					return 1;
				//if(pType) delete pType;
			}
			if(_tcsicmp((LPTSTR)pControl->GetWin()->m_pParam,_T("modifyitem"))==0)
			{
				::GetWindowText(pControl->GetWin()->GetControl(_T("colName"))->m_hWnd,pText,255);
				pNodeElement->setAttribute(_T("colname"),(_variant_t)pText);
				::GetWindowText(pControl->GetWin()->GetControl(_T("Name"))->m_hWnd,pText,255);
				pNodeElement->setAttribute(_T("name"),(_variant_t)pText);
				::GetWindowText(pControl->GetWin()->GetControl(_T("colType"))->m_hWnd,pText,255);
				pNodeElement->setAttribute(_T("type"),(_variant_t)pText);
				::GetWindowText(pControl->GetWin()->GetControl(_T("colRef"))->m_hWnd,pText,255);
				pNodeElement->setAttribute(_T("colref"),(_variant_t)pText);
				::GetWindowText(pControl->GetWin()->GetControl(_T("colNote"))->m_hWnd,pText,255);
				pNodeElement->setAttribute(_T("note"),(_variant_t)pText);
				::GetWindowText(pControl->GetWin()->GetControl(_T("dim"))->m_hWnd,pText,255);
				pNodeElement->setAttribute(_T("dim"),(_variant_t)pText);
				::GetWindowText(pControl->GetWin()->GetControl(_T("measure"))->m_hWnd,pText,255);
				pNodeElement->setAttribute(_T("measure"),(_variant_t)pText);
			}
			
		}
		

		m_pDocElement->selectSingleNode(_T("schema/*[1]"),&pNode);
		IXMLDOMElementPtr pSchemaElement = pNode;
		pContentSvr->Load(pSchemaElement);

		MakeData(pNode,pContentSvr);
		
		::InvalidateRect(pReportCtl ->m_hWnd,NULL,false);

		delete (LPTSTR)pControl->GetWin()->m_pParam;
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmSchemaCancel"))==0) //send by Schemadlg
	{
		delete (LPTSTR)pControl->GetWin()->m_pParam;
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
		
	return 0;
}

int CDevSchema::OnOpen(xfWin *pWin)
{
	if (pWin->m_pParam && _tcsicmp((LPTSTR)pWin->m_pParam,_T("modifyitem")) == 0)
	{
		XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)GetControl(_T("xreport"));
		SchemaContentSvr * pContentSvr = (SchemaContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;

		int row = ((XOffice::XReport::XReportSheetSvr *)pReportCtl->m_pSheetSvr)->GetRow();

		if (row < 1) return 1;
		IXMLDOMElement *pElement;
		pElement = pContentSvr->GetRowElement(row);
		_variant_t var;
		pElement->getAttribute(L"colref",&var);
		if (var.bstrVal)
			SetWindowText(pWin->GetControl(_T("colRef"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"type",&var);
		if (var.bstrVal)
			SetWindowText(pWin->GetControl(_T("colType"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"note",&var);
		if (var.bstrVal)
			SetWindowText(pWin->GetControl(_T("colNote"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"colname",&var);
		if (var.bstrVal)
			SetWindowText(pWin->GetControl(_T("colName"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"name",&var);
		if (var.bstrVal)
			SetWindowText(pWin->GetControl(_T("Name"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"dim",&var);
		if (var.bstrVal)
			SetWindowText(pWin->GetControl(_T("dim"))->m_hWnd,var.bstrVal);
		pElement->getAttribute(L"measure",&var);
		if (var.bstrVal)
			SetWindowText(pWin->GetControl(_T("measure"))->m_hWnd,var.bstrVal);

/*		BSTR bstr;
		pElement->get_tagName(&bstr);
		if (bstr)
			SetWindowText(pWin->GetControl(_T("colName"))->m_hWnd,bstr);
		::SysFreeString(bstr);
		*/
	}
	return 1;
}

int CDevSchema::OnOpen()
{
	pSchemaContentSvr = new SchemaContentSvr();
	xbXml xml;
	xml.LoadXml(_T("<datasource xroot=\"//*\"> \
				  <col name=\"colname\" xpath=\"@colname\" desc=\"列名\" />  \
				  <col name=\"type\" xpath=\"@type\" desc=\"类型\" /> \
				  <col name=\"name\" xpath=\"@name\" desc=\"名称\" />  \
				  <col name=\"dim\" xpath=\"@dim\" desc=\"维\" /> \
				  <col name=\"measure\" xpath=\"@measure\" desc=\"度量\" /> \
				  <col name=\"note\" xpath=\"@note\" desc=\"备注\" /> \
				  </datasource>"));
	IXMLDOMElement * pElement ;
	xml.m_pXmlDoc->get_documentElement(&pElement);
	pSchemaContentSvr->Initial(pElement);

	XOffice::XReport::XReportSheet * pReportCtl = (XOffice::XReport::XReportSheet *)this->GetControl(_T("xreport"));
	XOffice::XXmlContentSvr * pContentSvr = (XOffice::XXmlContentSvr *)((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr;
	if(pContentSvr)
	{
		delete pContentSvr;
		((XOffice::XReport::XReportDataSvr *)pReportCtl->m_pSheetSvr->m_pDataSvr)->m_pContentSvr = pSchemaContentSvr;
	}

	return 1;
}

bool CDevSchema::EvtNotify(TEvent* pEvent,LPARAM lParam)
{
	LPNMTVDISPINFO ptvdi = (LPNMTVDISPINFO)pEvent->notify.pnmh;
	if(ptvdi->hdr.code == TVN_SELCHANGED && ptvdi->hdr.hwndFrom != GetControl(_T("bitree"))->m_hWnd)
	{
		HWND hWnd = pEvent->notify.pnmh->hwndFrom;
		HTREEITEM hItem = ((LPNMTREEVIEW)pEvent->notify.pnmh)->itemNew.hItem;
		TVITEMEX	tvItem;
		//int	nLevel;
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
		tvItem.hItem = hItem;
		TreeView_GetItem(hWnd,&tvItem);

		IXMLDOMElement * pElement;
		pElement = (IXMLDOMElement* )(tvItem.lParam);
		ReadSheet(pElement);
	}
	return true;

}

int CDevSchema::GetLevel(xfControl * pControl,HTREEITEM hItem)
{
	int level = 0;
	HTREEITEM hpItem;
	hpItem = hItem;
	while(hpItem)
	{
		level ++;
		hpItem = TreeView_GetParent(pControl->m_hWnd, hpItem);
	}
	return level;
}

int CDevSchema::ReadSheet(IXMLDOMElement * pElement)
{
	xfControl * pControl = GetControl(_T("edittree")); 

	IXMLDOMNodeList * pList;
	IXMLDOMNode * pNode;
	IXMLDOMElement * pSheet;
	pElement->selectNodes(L"sheet",&pList);
	long nLen;
	pList->get_length(&nLen);
	std::wstring guids;
	_variant_t var;
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
		LPBYTE pData = NULL;
		int nLen;
		int nRet;
		nRet = xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen);
		if(nRet ==1)
		{
			xbXml xml;
			xml.LoadXml((LPTSTR)pData);
			XOffice::XOfficeFrm * pControl = (XOffice::XOfficeFrm *)GetControl(_T("editoffice"));
			IXMLDOMElement * pDocElement;
			xml.m_pXmlDoc->get_documentElement(&pDocElement);
			if(pDocElement)
			{
				pControl->m_pFrameSvr->m_pSerializeSvr->LoadSheetData(NULL,pDocElement,pControl->m_pFrameSvr->m_pDataSvr);
				pControl->m_pFrameSvr->SelectSheet(0);
				//pControl->m_pFrameSvr->SetFRect(pControl->m_pFrameSvr->FRect);
				/*RECT rc;
				rc = pControl->m_pxfNode->m_AreaRect;
				::OffsetRect(&rc, - rc.left , -rc.top);
				pControl->m_pFrameSvr->SetFRect(rc);*/

				::InvalidateRect(pControl->m_hWnd,NULL,true);
				pDocElement->Release(); 
			}
		}
		if(pData) xfUriLoader::FreeData(pData);
	}
	return 1;
}