#include "stdafx.h"
#include "DevView.h"
#include "xoffice.h"
#include "devschema.h"
#include "WorkNodeDlg.h"
#include "xflowchart.h"
#include "xreport.h"
#include "devxquery.h"
#include "xcell.h"

CDevView::CDevView(void)
{
}

CDevView::~CDevView(void)
{
}

xfControl * CDevView::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	if(_tcscmp(pCtrlID,_T("DevExplorer"))==0)
	{
		xfControl * pControl;
		pControl = new XDevTreeCtrl();
		pControl->m_dwExStyle = 0;
		pControl->m_dwStyle = WS_VISIBLE|WS_CHILD|TVS_HASLINES|/*TVS_LINESATROOT|*/TVS_SHOWSELALWAYS|TVS_HASBUTTONS;
		pControl->m_pszWinClass = WC_TREEVIEW;
		return pControl;
	}
	return NULL;
}

int CDevView::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
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

	if(pStrID && _tcsicmp(pStrID,_T("xmCancel"))==0 )
	{
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}
	if(pStrID && _tcsicmp(pStrID,_T("xmOkWorkNode"))==0 )
	{
		xfControl * pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
		if(!TreeView_GetParent(pTree->m_hWnd,hItem))
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择节点对象"),_T("提示"),MB_OK);
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
		if(_tcsicmp(var.bstrVal,_T("worknode"))!=0)
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择节点对象"),_T("提示"),MB_OK);
			return 1;
		}
		pControl->GetWin()->m_pParam = pElement;
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}


	if(pStrID && (_tcscmp(pStrID,_T("xmDevLink"))==0))
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet]; 
			xfWin * pWin = (xfWin *)::GetProp(hWnd,_T("this"));

			XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
			HTREEITEM hItem = (HTREEITEM)pWin->m_pParam; 
			TVITEMEX	tvItem;
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
			IXMLDOMElement * pElement = NULL;
			pElement = (IXMLDOMElement* )(tvItem.lParam);

			_variant_t type;
			BSTR bstr = NULL;
			pElement->getAttribute(L"type",&type);
			if(type.bstrVal && (::_wcsicmp(type.bstrVal,L"request")==0 || ::_wcsicmp(type.bstrVal,L"reqfile")==0  || ::_wcsicmp(type.bstrVal,L"template")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0))
			{
				XOffice::XOfficeFrm * pOfficeFrm = (XOffice::XOfficeFrm *)pWin->GetControl(_T("xoffice"));
				if(pOfficeFrm)
				{
					XOffice::XFlowChart::XFlowSheet * pSheet = dynamic_cast<XOffice::XFlowChart::XFlowSheet *>(pOfficeFrm->m_pFrameSvr->m_pSheet);
					if(pSheet)
					{
						XOffice::XFlowChart::XFlowChartObject * pObject = ((XOffice::XFlowChart::XFlowSheetSvr *)pSheet->m_pSheetSvr)->GetHitedObject();
						if(pObject)
						{
							TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
							DWORD dwSize = INTERNET_MAX_URL_LENGTH;
							xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("worknodeselectdlg.xpage"),strUrl,&dwSize,0);


							RECT rt = pControl->m_pxfNode->m_AreaRect;
							MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
							xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
	
							IXMLDOMElement * pWkElement = (IXMLDOMElement *)pWinEx->m_pParam;
							if(pWkElement)
							{
								TCHAR buf[255];
								_variant_t var;
								pWkElement->getAttribute(L"guid",&var);
								if(var.bstrVal)
								{
									_stprintf_s(buf,255,_T("worknode:%s/list#1"),var.bstrVal);

									HWND hWnd = m_pXPage->GetAgentNode()->GetXPage()->m_pWin->GetControl(L"wkname")->m_hWnd;
									::SetWindowText(hWnd,buf);

									IXMLDOMElement * pElement = pObject->m_pElement;
									IXMLDOMNode * pNode;
									IXMLDOMElement * pElementEx;
									xbXml xml;
									if(!pElement)
									{
										xml.m_pXmlDoc->createElement(L"data",&pElement);
										pObject->m_pElement = pElement;
									}
									pElement->selectSingleNode(L"devlink",&pNode);
									if(!pNode)
									{
										xml.m_pXmlDoc->createElement(L"devlink",&pElementEx);
										pElement->appendChild(pElementEx,NULL);
									}else
									{
										pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEx);
										pNode->Release();
									}
									pElementEx->setAttribute(L"url",(_variant_t)buf);
									pElementEx->Release();
								}
							}
							pWinEx->m_pParam = NULL;
							if(pWinEx) delete pWinEx ;
						}
					}
				}
			}
		}

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmOkDatasource"))==0 )
	{
		xfControl * pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
		HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
		if(!TreeView_GetParent(pTree->m_hWnd,hItem))
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择数据源定义对象"),_T("提示"),MB_OK);
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
		if(_tcsicmp(var.bstrVal,_T("xfile"))!=0)
		{
			MessageBox(pControl->GetWin()->m_hWnd,_T("请选择数据源定义对象"),_T("提示"),MB_OK);
			return 1;
		}
		pControl->GetWin()->m_pParam = pElement;
		pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
		return 1;
	}

	if(pStrID && (_tcscmp(pStrID,_T("xmDevDatasource"))==0))
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet]; 
			xfWin * pWin = (xfWin *)::GetProp(hWnd,_T("this"));

			XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
			HTREEITEM hItem = (HTREEITEM)pWin->m_pParam; 
			TVITEMEX	tvItem;
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
			IXMLDOMElement * pElement = NULL;
			pElement = (IXMLDOMElement* )(tvItem.lParam);

			_variant_t type;
			BSTR bstr = NULL;
			pElement->getAttribute(L"type",&type);
			if(type.bstrVal && (::_wcsicmp(type.bstrVal,L"request")==0 || ::_wcsicmp(type.bstrVal,L"reqfile")==0  || ::_wcsicmp(type.bstrVal,L"template")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0))
			{
				XOffice::XOfficeFrm * pOfficeFrm = (XOffice::XOfficeFrm *)pWin->GetControl(_T("xoffice"));
				if(pOfficeFrm)
				{
					XOffice::XCell::XCellSheet * pSheet = dynamic_cast<XOffice::XCell::XCellSheet *>(pOfficeFrm->m_pFrameSvr->m_pSheet);
					if(pSheet)
					{
						XOffice::XCell::XCellSheetSvr * pSheetSvr = (XOffice::XCell::XCellSheetSvr *)pSheet->m_pSheetSvr;
						
						TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
						DWORD dwSize = INTERNET_MAX_URL_LENGTH;
						xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("dataselectdlg.xpage"),strUrl,&dwSize,0);


						RECT rt = pControl->m_pxfNode->m_AreaRect;
						MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
						xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
	
						IXMLDOMElement * pWkElement = (IXMLDOMElement *)pWinEx->m_pParam;
						if(pWkElement)
						{
							TCHAR buf[255];
							_variant_t var;
							pWkElement->getAttribute(L"uri",&var);
							if(var.bstrVal)
							{
								_stprintf_s(buf,255,_T("dev:xfile[%s]"),var.bstrVal);
								((XOffice::XCell::CGridDataSvr *)pSheetSvr->m_pDataSvr)->SetSchmaSource(buf);
								HWND hWnd = m_pXPage->GetAgentNode()->GetXPage()->m_pWin->GetControl(L"datasource")->m_hWnd;
								::SetWindowText(hWnd,buf);
							}
						}
						pWinEx->m_pParam = NULL;
						if(pWinEx) delete pWinEx ;
					}
				}
			}
		}

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}
	if(pStrID && (_tcscmp(pStrID,_T("xmDevSchema"))==0))
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet]; 
			xfWin * pWin = (xfWin *)::GetProp(hWnd,_T("this"));

			XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
			HTREEITEM hItem = (HTREEITEM)pWin->m_pParam; 
			TVITEMEX	tvItem;
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
			IXMLDOMElement * pElement = NULL;
			pElement = (IXMLDOMElement* )(tvItem.lParam);

			_variant_t type;
			BSTR bstr = NULL;
			pElement->getAttribute(L"type",&type);
			if(type.bstrVal && (::_wcsicmp(type.bstrVal,L"request")==0 || ::_wcsicmp(type.bstrVal,L"reqfile")==0  || ::_wcsicmp(type.bstrVal,L"template")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0))
			{
				XOffice::XOfficeFrm * pOfficeFrm = (XOffice::XOfficeFrm *)pWin->GetControl(_T("xoffice"));
				if(pOfficeFrm)
				{
					XOffice::XCell::XCellSheet * pSheet = dynamic_cast<XOffice::XCell::XCellSheet *>(pOfficeFrm->m_pFrameSvr->m_pSheet);
					if(pSheet)
					{
						XOffice::XCell::XCellSheetSvr * pSheetSvr = (XOffice::XCell::XCellSheetSvr *)pSheet->m_pSheetSvr;
						LPTSTR pSchema = ((XOffice::XCell::CGridDataSvr *)pSheetSvr->m_pDataSvr)->m_pSchemaSource;
						if(pSchema)
						{
							TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
							DWORD dwSize = INTERNET_MAX_URL_LENGTH;
							xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("dataschemadlg.xpage"),strUrl,&dwSize,0);

							RECT rt = pControl->m_pxfNode->m_AreaRect;
							MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
							xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE, WS_CAPTION|WS_DLGFRAME|WS_POPUP,&rt,GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),pSheetSvr);
						}
					}
				}
			}
		}

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}

	if(pStrID && (_tcscmp(pStrID,_T("xmFileSave"))==0))
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet]; 
			xfWin * pWin = (xfWin *)::GetProp(hWnd,_T("this"));

			XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
			if(!pWin || !pWin->m_pParam) return 1;
			HTREEITEM hItem = (HTREEITEM)pWin->m_pParam; //TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
			TVITEMEX	tvItem;
			int	nLevel;
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
			IXMLDOMElement * pElement = NULL;
			pElement = (IXMLDOMElement* )(tvItem.lParam);

			_variant_t type;
			BSTR bstr = NULL;
			BSTR bstrex = NULL;
			pElement->getAttribute(L"type",&type);
			LPTSTR pWorkGuid = NULL;
			LPTSTR pItemGuid = NULL;

			_variant_t caption;
			pElement->getAttribute(L"caption",&caption);
			if(::_wcsicmp(type.bstrVal,L"request")==0)
			{
				if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xdv"))
				{
					type = L"xdv";
				}
			}
			if(::_wcsicmp(type.bstrVal,L"reqfile")!=0 && ::_wcsicmp(type.bstrVal,L"bffile")!=0 )
			{
				HTREEITEM hpItem = hItem;
				BSTR bstrTagName;
				IXMLDOMElement* pElementEx;
findworkitems1:		hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
				tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
				tvItem.hItem = hpItem;
				TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
				pElementEx = (IXMLDOMElement* )(tvItem.lParam);
				pElementEx->get_tagName(&bstrTagName);
				if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto findworkitems1;
				pWorkGuid = xbXml::GetStringAttribe(pElementEx,_T("guid"));
				pItemGuid = xbXml::GetStringAttribe(pElement,_T("guid"));
			}else
			{
				pItemGuid = xbXml::GetStringAttribe(pElement,_T("guid"));
			}

			TCHAR pSubType[255];
			pSubType[0] = 0;
			if(caption.bstrVal)
			{
				if(caption.bstrVal && _tcsstr(caption.bstrVal,L".wkl"))
					_tcscpy_s(pSubType,255,L"worklist");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".tpp"))
					_tcscpy_s(pSubType,255,L"code");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".js"))
					_tcscpy_s(pSubType,255,L"code");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".tpl"))
					_tcscpy_s(pSubType,255,L"template");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xa"))
					_tcscpy_s(pSubType,255,L"xaction");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xq"))
					_tcscpy_s(pSubType,255,L"xquery");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".wk"))
					_tcscpy_s(pSubType,255,L"worknode");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xpage"))
					_tcscpy_s(pSubType,255,L"xpage");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xsl"))
					_tcscpy_s(pSubType,255,L"xml");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xml"))
					_tcscpy_s(pSubType,255,L"xml");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xri"))
					_tcscpy_s(pSubType,255,L"xml");
				else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".dbmap"))
					_tcscpy_s(pSubType,255,L"xml");
			}

			if(::_wcsicmp(pSubType,L"xml")==0)
			{
				/*int nLen = ::GetWindowTextLength(pWin->GetControl(_T("codecontent"))->m_hWnd);
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				GetWindowText(pWin->GetControl(_T("codecontent"))->m_hWnd,bstr,nLen + 1);*/
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("content"));
				pControl->GetContent(&bstr);
			}
			else if((type.bstrVal && (::_wcsicmp(type.bstrVal,L"request")==0 /*|| ::_wcsicmp(type.bstrVal,L"reqfile")==0  */|| ::_wcsicmp(type.bstrVal,L"template")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0)) 
				||::_wcsicmp(pSubType,L"worklist")==0 ||::_wcsicmp(pSubType,L"template")==0)
			{
				XOffice::XOfficeFrm * pOfficeFrm = (XOffice::XOfficeFrm *)pWin->GetControl(_T("xoffice"));
				if(pOfficeFrm)
				{
					xbXml xml;
					xml.LoadXml(L"<?xml version=\"1.0\" encoding=\"UTF-16\"?><xdoc/>");
					
					IXMLDOMElement * pRootElement;
					xml.m_pXmlDoc->get_documentElement(&pRootElement);
					pOfficeFrm->m_pFrameSvr->m_pSerializeSvr->SerializeSheetData(pOfficeFrm->m_pFrameSvr->m_pDataSvr,pRootElement,xml.m_pXmlDoc);
					pRootElement->Release();
					xml.m_pXmlDoc->get_xml(&bstr);
				}
			}else if((::_wcsicmp(pSubType,L"entry")==0) ||(type.bstrVal && ::_wcsicmp(type.bstrVal,L"entry")==0))
			{
				CDevSchema * pSchema = (CDevSchema *)pWin;
				pSchema->m_pDocElement->setAttribute(L"workguid",(_variant_t)pWorkGuid);
				pSchema->m_pDocElement->setAttribute(L"guid",(_variant_t)pItemGuid);
				pSchema->m_pDocElement->get_xml(&bstr);
			}else if((::_wcsicmp(pSubType,L"worknode")==0) ||(type.bstrVal && ::_wcsicmp(type.bstrVal,L"worknode")==0))
			{
				CWorkNodeDlg * pWorkNode = (CWorkNodeDlg *)pWin;
				pWorkNode->m_pDocElement->setAttribute(L"workguid",(_variant_t)pWorkGuid);
				pWorkNode->m_pDocElement->setAttribute(L"guid",(_variant_t)pItemGuid);
				pWorkNode->m_pDocElement->get_xml(&bstr);
			}else if((::_wcsicmp(pSubType,L"xpage")==0) ||(type.bstrVal && ::_wcsicmp(type.bstrVal,L"xpage")==0))
			{
				/*int nLen = ::GetWindowTextLength(pWin->GetControl(_T("xpagecontent"))->m_hWnd);
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				GetWindowText(pWin->GetControl(_T("xpagecontent"))->m_hWnd,bstr,nLen + 1);*/
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("xpagecontent"));
				pControl->GetContent(&bstr);
				
				pControl = (XSEdit *)pWin->GetControl(_T("xcodecontent"));
				pControl->GetContent(&bstrex);

			}else if((::_wcsicmp(pSubType,L"code")==0 ) ||(type.bstrVal && (::_wcsicmp(type.bstrVal,L"code")==0)))
			{
				/*int nLen = ::GetWindowTextLength(pWin->GetControl(_T("codecontent"))->m_hWnd);
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				GetWindowText(pWin->GetControl(_T("codecontent"))->m_hWnd,bstr,nLen + 1);*/
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("codecontent"));
				pControl->GetContent(&bstr);

			}else if(type.bstrVal && (::_wcsicmp(type.bstrVal,L"xdv")==0 || ::_wcsicmp(type.bstrVal,L"reqfile")==0||(::_wcsicmp(pSubType,L"xdv")==0)))
			{
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("xcontent"));
				pControl->GetContent(&bstr);
			}else if(type.bstrVal && ::_wcsicmp(type.bstrVal,L"serialize")==0)
			{
				/*int nLen = ::GetWindowTextLength(pWin->GetControl(_T("codecontent"))->m_hWnd);
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				GetWindowText(pWin->GetControl(_T("codecontent"))->m_hWnd,bstr,nLen + 1);*/
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("content"));
				pControl->GetContent(&bstr);
			}else if(pSubType[0]==0 && type.bstrVal && (::_wcsicmp(type.bstrVal,L"xruler")==0 ||
				::_wcsicmp(type.bstrVal,L"bffile")==0))
			{
				/*int nLen = ::GetWindowTextLength(pWin->GetControl(_T("codecontent"))->m_hWnd);
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				GetWindowText(pWin->GetControl(_T("codecontent"))->m_hWnd,bstr,nLen + 1);*/
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("xcontent"));
				pControl->GetContent(&bstr);
			}else if(pSubType[0]==0 && type.bstrVal && ::_wcsicmp(type.bstrVal,L"xfile")==0)
			{
				/*int nLen = ::GetWindowTextLength(pWin->GetControl(_T("codecontent"))->m_hWnd);
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				GetWindowText(pWin->GetControl(_T("codecontent"))->m_hWnd,bstr,nLen + 1);*/
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("xcontent"));
				pControl->GetContent(&bstr);
			}else if(pSubType[0]==0 && type.bstrVal && ::_wcsicmp(type.bstrVal,L"xmode")==0)
			{
				/*int nLen = ::GetWindowTextLength(pWin->GetControl(_T("codecontent"))->m_hWnd);
				bstr = ::SysAllocStringLen(NULL,nLen + 1);
				GetWindowText(pWin->GetControl(_T("codecontent"))->m_hWnd,bstr,nLen + 1);*/
				XSEdit * pControl = (XSEdit *)pWin->GetControl(_T("xcontent"));
				pControl->GetContent(&bstr);
			}else if((::_wcsicmp(pSubType,L"xquery")==0)|| (::_wcsicmp(pSubType,L"xaction")==0)||(type.bstrVal && ::_wcsicmp(type.bstrVal,L"xquery")==0 || type.bstrVal && ::_wcsicmp(type.bstrVal,L"xaction")==0))
			{
				((DevXQuery *)pWin)->m_pElementEx->setAttribute(L"workguid",(_variant_t)pWorkGuid);
				((DevXQuery *)pWin)->m_pElementEx->setAttribute(L"guid",(_variant_t)pItemGuid);
				((DevXQuery *)pWin)->m_pElementEx->get_xml(&bstr);
			}
			if(pWorkGuid)delete pWorkGuid;
			if(pItemGuid)delete pItemGuid;

			//::_wcsicmp(type.bstrVal,L"worknode")==0 ||  || 
			//	::_wcsicmp(type.bstrVal,L"report")==0 || ::_wcsicmp(type.bstrVal,L"entry")==0 || 
			//	::_wcsicmp(type.bstrVal,L"xpage")==0 || ::_wcsicmp(type.bstrVal,L"code")==0 ||
			//	::_wcsicmp(type.bstrVal,L"resource")==0))


			if(bstr)
			{
				LPTSTR pSrc  = NULL;
				LPTSTR pStrGuid = NULL;
				LPTSTR pStrGuidItem = NULL;
				LPTSTR pCaption = NULL;
				LPTSTR pItemUri = NULL;

				pStrGuidItem = xbXml::GetStringAttribe(pElement,_T("guid"));
				pItemUri = xbXml::GetStringAttribe(pElement,_T("uri"));
				
				HTREEITEM hpItem = hItem;

				if(::_wcsicmp(type.bstrVal,L"reqfile")!=0 &&
					::_wcsicmp(type.bstrVal,L"bffile")!=0)
				{
					BSTR bstrTagName;
					IXMLDOMElement* pElementEx;
findworkitems:		hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
					tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
					tvItem.hItem = hpItem;
					TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
					pElementEx = (IXMLDOMElement* )(tvItem.lParam);
					pElementEx->get_tagName(&bstrTagName);
					if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto findworkitems;
				}

				pElement = (IXMLDOMElement* )(tvItem.lParam);
				pStrGuid = xbXml::GetStringAttribe(pElement,_T("guid"));
				nLevel = pDevTreeCtrl->GetLevel(hpItem);
				for(int i=nLevel;i >2;i--)
				{
					hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
				}
				tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
				tvItem.hItem = hpItem;
				TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
				pSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));

				TCHAR strCmd[512];
				if(::_wcsicmp(type.bstrVal,L"reqfile")==0 )
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer savedocex %s %s"),	pSrc,pStrGuid);
				else if(::_wcsicmp(type.bstrVal,L"bffile")==0)
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer saveflow %s %s"),	pSrc,pStrGuid);
				else
				{
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer savedoc %s %s %s"),	pSrc,pStrGuid,pStrGuidItem);
				}
				if(pItemUri)
				{
					_tcscat_s(strCmd,512,_T(" "));
					_tcscat_s(strCmd,512,pItemUri);
				}

				LPBYTE pData = NULL;
				int nLen;
				
				TCHAR wpath[MAX_PATH];
				TCHAR wpath1[MAX_PATH];
				TCHAR * dest=_T("c:\\xtemplate");
				if(!::PathFileExists(dest))::CreateDirectory(dest,NULL);
				pCaption = xbXml::GetStringAttribe(pElement,_T("caption"));
				for(int i=3;i>=1;i--)
				{
					_stprintf_s(wpath1,MAX_PATH,_T("%s\\%s%s.xdoc.bk%i"),dest,pCaption,pStrGuidItem,i + 1);
					_stprintf_s(wpath,MAX_PATH,_T("%s\\%s%s.xdoc.bk%i"),dest,pCaption,pStrGuidItem,i);
					if(::PathFileExists(wpath))::MoveFileEx(wpath,wpath1,MOVEFILE_REPLACE_EXISTING);
				}

				_stprintf_s(wpath1,MAX_PATH,_T("%s\\%s%s.xdoc.bk1"),dest,pCaption,pStrGuidItem);
				_stprintf_s(wpath,MAX_PATH,_T("%s\\%s%s.xdoc"),dest,pCaption,pStrGuidItem);
				if(::PathFileExists(wpath))::MoveFileEx(wpath,wpath1,MOVEFILE_REPLACE_EXISTING);

				BYTE Buf[2];
				Buf[0]=0xff;
				Buf[1]=0xfe;
				DWORD dwSize;
				HANDLE fp1 = CreateFile(wpath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				WriteFile(fp1,Buf,2,&dwSize,NULL);
				::WriteFile(fp1,bstr,(int)::_tcslen(bstr)*sizeof(TCHAR),&dwSize,NULL);
				::CloseHandle(fp1);

				if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,bstr,pData,nLen)==1)
				{
					if(pData) xfUriLoader::FreeData(pData);
					if(GetFrameWindow()->GetControl(_T("statusbar")))
						SendMessage(GetFrameWindow()->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("保存完成"));

				}else
				{
					SendMessage(GetFrameWindow()->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("文件保存失败"));
					MessageBox(m_hWnd,_T("文件保存失败"),_T("提示"),MB_OK);
				}
				if(bstrex && ::_wcsicmp(type.bstrVal,L"xpage")==0 )
				{
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer savedoc %s %s %s xss"),	pSrc,pStrGuid,pStrGuidItem);
					if(pItemUri)
					{
						_tcscat_s(strCmd,512,_T(" "));
						_tcscat_s(strCmd,512,pItemUri);
					}
					if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,bstrex,pData,nLen)==1)
					{
						if(pData) xfUriLoader::FreeData(pData);
						if(GetFrameWindow()->GetControl(_T("statusbar")))
							SendMessage(GetFrameWindow()->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("保存完成"));

					}else
					{
						SendMessage(GetFrameWindow()->GetControl(_T("statusbar"))->m_hWnd,SB_SETTEXT,0,(LPARAM)_T("文件保存失败"));
						MessageBox(m_hWnd,_T("代码文件保存失败"),_T("提示"),MB_OK);
					}
				}

				if(pCaption)delete pCaption;
				if(pItemUri) delete pItemUri;
				if(pStrGuidItem)delete pStrGuidItem;
				if(pStrGuid)delete pStrGuid;
				if(pSrc)delete pSrc;

			}else
			{

			}
		}
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
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

	if(	pStrID && _tcscmp(pStrID,_T("xmDevComplier"))==0)
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
		int	nLevel;
		TV_ITEM	tvItem;
		HTREEITEM hItem;
		hItem = TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
		tvItem.hItem = hItem;
		TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);

		HTREEITEM hpItem;
		hpItem = hItem;
		IXMLDOMElement * pElement = NULL;
		pElement = (IXMLDOMElement* )(tvItem.lParam);
		//IXMLDOMElement * pElementEx;

		BSTR bstrTagName;
		pElement->get_tagName(&bstrTagName);
		if(_tcsicmp(bstrTagName,_T("work"))!=0)
		{
			hpItem = hItem;
startfindworkitem:
			hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hpItem;
			TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
			pElement = (IXMLDOMElement* )(tvItem.lParam);
			::SysFreeString(bstrTagName);
			pElement->get_tagName(&bstrTagName);
			if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto startfindworkitem;
		}
		::SysFreeString(bstrTagName);
		nLevel = pDevTreeCtrl->GetLevel(hpItem);

		LPTSTR pSrc  = NULL;
		LPTSTR pStrGuid = NULL;

		pElement = (IXMLDOMElement* )(tvItem.lParam);
		pStrGuid = xbXml::GetStringAttribe(pElement,_T("guid"));
		hpItem = hItem;
		nLevel = pDevTreeCtrl->GetLevel(hpItem);
		for(int i=nLevel;i >2;i--)
		{
			hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
		}
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE|TVIF_TEXT;
		tvItem.hItem = hpItem;
		tvItem.pszText = new TCHAR[255];
		tvItem.cchTextMax = 255;
		TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
		delete tvItem.pszText;
		pSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));

		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer %s %s %s"),_T("build"),
			pSrc,pStrGuid);
		if(pSrc)delete pSrc;

		LPBYTE pData;
		int nLen;
		int nRet;

		xfControl * pOutput = GetControl(_T("output"));
		::SetWindowText(pOutput->m_hWnd,_T("正在编译.....\r\n"));
		pData = NULL;
		nRet = xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen);
		if(nRet==1 && pData)
		{
			LPSTR pText = new char[strlen((LPCSTR)pData) + (0x800 << 2)];
			int pos=0;
			int pos1=0;
			while(((LPCSTR)pData)[pos])
			{
				if(((LPCSTR)pData)[pos]==0x0A)
				{
					pText[pos1++] = 0x0D;
					pText[pos1++] = 0x0A;
				}else
					pText[pos1++] = pData[pos];
				pos++;
			}
			pText[pos1++] = 0;
			::SetWindowTextA(pOutput->m_hWnd,pText);
		}else
		{
			::SetWindowText(pOutput->m_hWnd,_T("运行编译程序错误"));
		}
		if(pData)delete pData;

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}

	if(pStrID && _tcscmp(pStrID,_T("xmDevWorkExport"))==0)
	{
		LPTSTR ext =_T("");
		LPTSTR filter = _T("全部文件(*.*)\0*.*\0");
		TCHAR szFileName[255],szPath[255];
		int	 nFilterIndex = 1;
		ZeroMemory(szFileName,255);
		ZeroMemory(szPath,255);

		OPENFILENAME ofn;
		ZeroMemory(&ofn,sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = this->GetHWND();
		ofn.hInstance = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = nFilterIndex;
		ofn.lpstrFile = szPath;
		ofn.nMaxFile = 255;
		ofn.lpstrFileTitle = szFileName;
		ofn.nMaxFileTitle = 255;
		ofn.lpstrTitle = _T("另存为");
		ofn.lpstrDefExt = ext;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_FILEMUSTEXIST;
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;
		ofn.lpTemplateName = NULL;
	
		if(!::GetSaveFileName(&ofn)) return 1;
	
		XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
		int	nLevel;
		TV_ITEM	tvItem;
		HTREEITEM hItem;
		hItem = TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
		tvItem.hItem = hItem;
		TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);

		HTREEITEM hpItem;
		hpItem = hItem;
		IXMLDOMElement * pElement = NULL;
		pElement = (IXMLDOMElement* )(tvItem.lParam);

		nLevel = pDevTreeCtrl->GetLevel(hItem);
		if(nLevel <= 2)	return true;

		_variant_t type,caption;
		pElement = (IXMLDOMElement* )(tvItem.lParam);
		if(pElement)
		{
			pElement->getAttribute(L"type",&type);
			if(type.bstrVal && (::_wcsicmp(type.bstrVal,L"request")==0 || ::_wcsicmp(type.bstrVal,L"template")==0 ||
				::_wcsicmp(type.bstrVal,L"worknode")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0 || 
				::_wcsicmp(type.bstrVal,L"report")==0 || ::_wcsicmp(type.bstrVal,L"entry")==0 || 
				::_wcsicmp(type.bstrVal,L"xpage")==0 || ::_wcsicmp(type.bstrVal,L"code")==0 ||
				::_wcsicmp(type.bstrVal,L"resource")==0 || ::_wcsicmp(type.bstrVal,L"reqfile")==0 ||
				::_wcsicmp(type.bstrVal,L"xquery")==0 || ::_wcsicmp(type.bstrVal,L"xaction")==0 ||
				::_wcsicmp(type.bstrVal,L"serialize")==0  || ::_wcsicmp(type.bstrVal,L"xruler")==0 ||
				::_wcsicmp(type.bstrVal,L"xfile")==0 || ::_wcsicmp(type.bstrVal,L"xmode")==0 ||
				::_wcsicmp(type.bstrVal,L"bffile")==0))
			{
				LPTSTR pStrGuidItem = NULL;
				pStrGuidItem = xbXml::GetStringAttribe(pElement,_T("guid"));

				HTREEITEM hpItem = hItem;
				if(::_wcsicmp(type.bstrVal,L"reqfile")!=0 &&
					::_wcsicmp(type.bstrVal,L"bffile")!=0)
				{
					BSTR bstrTagName;
					IXMLDOMElement* pElementEx;
findworkitem:		hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
					tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
					tvItem.hItem = hpItem;
					TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
					nLevel --;
					pElementEx = (IXMLDOMElement* )(tvItem.lParam);
					pElementEx->get_tagName(&bstrTagName);
					if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto findworkitem;
				}

				LPTSTR pSrc  = NULL;
				LPTSTR pStrGuid = NULL;

				pElement = (IXMLDOMElement* )(tvItem.lParam);
				pStrGuid = xbXml::GetStringAttribe(pElement,_T("guid"));
				nLevel = pDevTreeCtrl->GetLevel(hpItem);
				for(int i=nLevel;i >2;i--)
				{
					hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
				}
				tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
				tvItem.hItem = hpItem;
				TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
				pSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));

				TCHAR strCmd[512];
				if(::_wcsicmp(type.bstrVal,L"reqfile")==0)
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdocex %s %s"),	pSrc,pStrGuid);
				else if(::_wcsicmp(type.bstrVal,L"bffile")==0)
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getflow %s %s"),	pSrc,pStrGuid);
				else
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdoc %s %s %s"),	pSrc,pStrGuid,pStrGuidItem);

				if(pSrc)delete pSrc;
				LPBYTE pData = NULL;
				int nLen;
				int nRet;
				nRet = xfUriLoader::ProcessCmd(m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen);
				if(nRet !=1)
				{
					MessageBox(m_hWnd,_T("文件读取失败"),_T("提示"),MB_OK);
					return 1;
				}else
				{
					DWORD dwSize;
					HANDLE fp1 = CreateFile(szPath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
					::WriteFile(fp1,pData,nLen,&dwSize,NULL);
					::CloseHandle(fp1);
					return 1;
				}
			}
		}
		return 1;
	}

	//developerdlg
	if(pStrID &&( _tcscmp(pStrID,_T("xmDevNewWorkGroup"))==0||  
		          _tcscmp(pStrID,_T("xmDevNewWorkItem"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewWorkNode"))==0||
				  _tcscmp(pStrID,_T("xmDevWorkNode"))==0||
				  _tcscmp(pStrID,_T("xmDevNewDoc"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewDocEx"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewRulerEx"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewEntry"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewTemplate"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewList"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewReport"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewXPageFile"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewCodeFile"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewResouceFile"))==0 ||
				  _tcscmp(pStrID,_T("xmDevNewDocFolder"))==0||
				  _tcscmp(pStrID,_T("xmDevNewBFFolder"))==0||
				  _tcscmp(pStrID,_T("xmDevNewXQuery"))==0||
				  _tcscmp(pStrID,_T("xmDevNewXAction"))==0||
				  _tcscmp(pStrID,_T("xmDevNewSerialize"))==0||
				  _tcscmp(pStrID,_T("xmDevNewRuler"))==0||
				  _tcscmp(pStrID,_T("xmDevNewXFile"))==0||
				  _tcscmp(pStrID,_T("xmDevNewXMode"))==0||
				  _tcscmp(pStrID,_T("xmDevWorkModify"))==0||
				  _tcscmp(pStrID,_T("xmDevWorkDelete"))==0))

	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		xfUriLoader::UrlCombineW(pControl->m_pxfNode->GetXPage()->m_pStrUrl,_T("/developer/workdlg.xml"),strUrl,&dwSize,0);

		TPara Para;
		Para.pWin = pControl->m_pxfNode->GetXPage()->m_pWin;
		Para.pStrID = (LPTSTR)pStrID;

		XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
		
		Para.pElement=NULL;
		if(_tcscmp(pStrID,_T("xmDevWorkModify"))==0)
		{
			int	nLevel;
			TV_ITEM	tvItem;
			HTREEITEM hItem;
			hItem = TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
			tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
			tvItem.hItem = hItem;
			TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
			nLevel = pDevTreeCtrl->GetLevel(hItem);
			if(nLevel < 2) return 0;
			IXMLDOMElement * pElement = NULL;
			pElement = (IXMLDOMElement* )(tvItem.lParam);
			_variant_t var;
			pElement->getAttribute(L"type",&var);
			if(!var.bstrVal)return 0;
			if(_tcsicmp(L"WorkFolder",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkGroup");
			else if(_tcsicmp(L"work",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkItem");
			else if(_tcsicmp(L"worknode",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkNode");
			else if(_tcsicmp(L"request",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkDoc");
			else if(_tcsicmp(L"template",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkTemplate");
			else if(_tcsicmp(L"entry",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkEntry");
			else if(_tcsicmp(L"worklist",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkList");
			else if(_tcsicmp(L"report",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkReport");
			else if(_tcsicmp(L"xpage",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkXPage");
			else if(_tcsicmp(L"code",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkCode");
			else if(_tcsicmp(L"resource",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkResource");
			else if(_tcsicmp(L"xquery",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkXQuery");
			else if(_tcsicmp(L"xaction",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkXAction");
			else if(_tcsicmp(L"reqfile",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkDocEx");
			else if(_tcsicmp(L"bffile",var.bstrVal)==0)Para.pStrID=_T("xmDevWorkRulerEx");
			else if(_tcsicmp(L"docfolder",var.bstrVal)==0)Para.pStrID=_T("xmDevDocFolder");
			else if(_tcsicmp(L"bffolder",var.bstrVal)==0)Para.pStrID=_T("xmDevBFFolder");
			else if(_tcsicmp(L"serialize",var.bstrVal)==0)Para.pStrID=_T("xmDevSerialize");
			else if(_tcsicmp(L"xruler",var.bstrVal)==0)Para.pStrID=_T("xmDevRuler");
			else if(_tcsicmp(L"xfile",var.bstrVal)==0)Para.pStrID=_T("xmDevXFile");
			else if(_tcsicmp(L"xmode",var.bstrVal)==0)Para.pStrID=_T("xmDevXMode");
			else return 0;
			Para.pElement = pElement; 
		}
		if(_tcscmp(pStrID,_T("xmDevWorkDelete"))==0) return 1;

		xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,pControl->m_pxfNode->GetXPage()->m_pWin->m_hWnd,xfApp::GetApp()->GetInstance(),&Para);
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}
	return xfMdiFrame::OnXCommand(pStrID,pControl);
}

CDevView::XDevTreeCtrl::XDevTreeCtrl():m_pAgents(NULL)
{
	TCHAR strDev[MAX_PATH];
	::_stprintf_s(strDev,MAX_PATH,_T("%s\\%s"),xfApp::GetApp()->m_pXConfigPath,_T("developer.xml"));
	m_xml.Load(strDev);
	m_xml.m_pXmlDoc->selectSingleNode(L"/developer/agent",&pNodeAgent);

	this->AttachCtrlListon(TVN_SELCHANGED,XFRAME_FNEVENT_(CDevView::XDevTreeCtrl::SelectChanged));
	this->AttachCtrlListon(NM_DBLCLK,XFRAME_FNEVENT_(CDevView::XDevTreeCtrl::DoubleClicked)); 
}

CDevView::XDevTreeCtrl::~XDevTreeCtrl()
{
	if(m_pAgents)
	{
		for(int i=0;i<(int)m_pAgents->size();i++)
		{
			delete (*m_pAgents)[i].pType;
		}
		delete m_pAgents;
	}
	if(pNodeAgent)pNodeAgent->Release();
}

int CDevView::XDevTreeCtrl::GetLevel(HTREEITEM hItem)
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


XFRAME_EVENT_(CDevView::XDevTreeCtrl::DoubleClicked)
{
	HWND hWnd = pEvent->hWnd;
	HTREEITEM hItem = TreeView_GetSelection(this->m_hWnd);
	TVITEMEX	tvItem;
	int	nLevel;
	tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
	tvItem.hItem = hItem;
	TreeView_GetItem(m_hWnd,&tvItem);

	IXMLDOMElement * pElement = NULL;

	nLevel = this->GetLevel(hItem);
	if(nLevel <= 2)	return true;

	_variant_t type,caption;
	pElement = (IXMLDOMElement* )(tvItem.lParam);
	if(pElement)
	{
		pElement->getAttribute(L"type",&type);
		if(type.bstrVal && (::_wcsicmp(type.bstrVal,L"request")==0 || ::_wcsicmp(type.bstrVal,L"template")==0 ||
			   ::_wcsicmp(type.bstrVal,L"worknode")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0 || 
			   ::_wcsicmp(type.bstrVal,L"report")==0 || ::_wcsicmp(type.bstrVal,L"entry")==0 || 
			   ::_wcsicmp(type.bstrVal,L"xpage")==0 || ::_wcsicmp(type.bstrVal,L"code")==0 ||
			   ::_wcsicmp(type.bstrVal,L"resource")==0 || ::_wcsicmp(type.bstrVal,L"reqfile")==0 ||
			   ::_wcsicmp(type.bstrVal,L"xquery")==0 || ::_wcsicmp(type.bstrVal,L"xaction")==0 ||
			   ::_wcsicmp(type.bstrVal,L"serialize")==0 || ::_wcsicmp(type.bstrVal,L"xruler")==0||
			   ::_wcsicmp(type.bstrVal,L"xfile")==0 || ::_wcsicmp(type.bstrVal,L"xmode")==0 ||
			   ::_wcsicmp(type.bstrVal,L"bffile")==0))
		{
			TCHAR* pStrFormat;
			TCHAR xpath[255];
			pElement->getAttribute(L"caption",&caption);
			if(::_wcsicmp(type.bstrVal,L"request")==0)
			{
				if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xdv"))
				{
					type = L"xdv";
				}
			}

			TCHAR pSubType[255];
			pSubType[0] = 0;
			if(caption.bstrVal && _tcsstr(caption.bstrVal,L".wkl"))
				_tcscpy_s(pSubType,255,L"worklist");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".tpp"))
				_tcscpy_s(pSubType,255,L"code");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".js"))
				_tcscpy_s(pSubType,255,L"code");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".tpl"))
				_tcscpy_s(pSubType,255,L"template");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xa"))
				_tcscpy_s(pSubType,255,L"xaction");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xq"))
				_tcscpy_s(pSubType,255,L"xquery");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".wk"))
				_tcscpy_s(pSubType,255,L"worknode");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xpage"))
				_tcscpy_s(pSubType,255,L"xpage");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xsl"))
				_tcscpy_s(pSubType,255,L"xml");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xml"))
				_tcscpy_s(pSubType,255,L"xml");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".xri"))
				_tcscpy_s(pSubType,255,L"xml");
			else if(caption.bstrVal && _tcsstr(caption.bstrVal,L".dbmap"))
				_tcscpy_s(pSubType,255,L"xml");

			if(pSubType[0])
				_stprintf_s(xpath,255,_T("/developer/templates/%s/xframe"),pSubType);
			else
				_stprintf_s(xpath,255,_T("/developer/templates/%s/xframe"),type.bstrVal);

			IXMLDOMNodePtr	pNode;
			m_xml.m_pXmlDoc->selectSingleNode(xpath,&pNode);
			if(pNode)
			{
				BSTR bstr;
				pNode->get_xml(&bstr);
				pStrFormat = ::_tcsdup(bstr);
				::SysFreeString(bstr);
			}
			else
				pStrFormat =::_tcsdup(_T("<xframe style=\"background:#cecebe none #cecebe none\" caption=\"%s\" type=\"sheet\" winstate=\"maximize\" rect=\"0,0,0,0\" xdll=\"ext\\XExplorerExt.dll\"><xoffice  name=\"xoffice\"/></xframe>"));
			int nLen = (int)::_tcslen(pStrFormat) + 1 + 2*MAX_PATH +10;
			LPTSTR pStrXml = new TCHAR[nLen];

	
			pElement->getAttribute(L"caption",&caption);
			xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetWin()->GetControl(_T("mdilayer"));
			
			int i;
			for(i=0;i<pLayerSheet->m_SheetSize;i++)
			{
				xfWin * pWin = (xfWin *)::GetProp((*pLayerSheet->m_pHwnds)[i],_T("this"));
				if(pWin && pWin->m_pParam == hItem) break;
			}
			if(i>=pLayerSheet->m_SheetSize)
			{
				pElement->getAttribute(L"caption",&caption);
				if(caption.bstrVal)
					::_stprintf_s(pStrXml,nLen,pStrFormat,caption.bstrVal,_T(""));
				else
					::_stprintf_s(pStrXml,nLen,pStrFormat,_T("Untitle"),_T(""));


				xfXPage * pXPage = new xfXPage;
				//::_tcscpy_s(pXPage->m_pStrUrl,INTERNET_MAX_URL_LENGTH,GetWin()->m_pXPage->m_pStrUrl);
				pXPage->m_pStrUrl = _tcsdup(GetWin()->m_pXPage->m_pStrUrl);
				pXPage->LoadXPage(pStrXml);
				xfWin*  pWinEx = xfWin::LoadXPageXml(pXPage,this->GetWin()->m_hWnd,xfApp::GetApp()->GetInstance(),hItem);
				
				LPTSTR pStrGuidItem = NULL;
				pStrGuidItem = xbXml::GetStringAttribe(pElement,_T("guid"));
				
				HTREEITEM hpItem = hItem;
				if(::_wcsicmp(type.bstrVal,L"reqfile")!=0 &&
					::_wcsicmp(type.bstrVal,L"bffile")!=0)
				{
					BSTR bstrTagName;
					IXMLDOMElement* pElementEx;
findworkitem:		hpItem = TreeView_GetParent(m_hWnd, hpItem);
					tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
					tvItem.hItem = hpItem;
					TreeView_GetItem(m_hWnd,&tvItem);
					nLevel --;
					pElementEx = (IXMLDOMElement* )(tvItem.lParam);
					pElementEx->get_tagName(&bstrTagName);
					if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto findworkitem;
				}

				LPTSTR pSrc  = NULL;
				LPTSTR pStrGuid = NULL;

				pElement = (IXMLDOMElement* )(tvItem.lParam);
				pStrGuid = xbXml::GetStringAttribe(pElement,_T("guid"));
				nLevel = this->GetLevel(hpItem);
				for(int i=nLevel;i >2;i--)
				{
					hpItem = TreeView_GetParent(this->m_hWnd, hpItem);
				}
				tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
				tvItem.hItem = hpItem;
				TreeView_GetItem(this->m_hWnd,&tvItem);
				pSrc = xbXml::GetStringAttribe((IXMLDOMElement*)tvItem.lParam,_T("src"));
			
				TCHAR strCmd[512];
				if(::_wcsicmp(type.bstrVal,L"reqfile")==0)
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdocex %s %s"),	pSrc,pStrGuid);
				else if(::_wcsicmp(type.bstrVal,L"bffile")==0)
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getflow %s %s"),	pSrc,pStrGuid);
				else
					::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/developer getdoc %s %s %s"),	pSrc,pStrGuid,pStrGuidItem);

				if(pSrc)delete pSrc;
				LPBYTE pData = NULL;
				int nLen;
				int nRet;
				nRet = xfUriLoader::ProcessCmd(GetWin()->GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen);
				if(nRet !=1)
				{
					MessageBox(m_hWnd,_T("文件读取失败"),_T("提示"),MB_OK);
					return true;
				}

				if(nRet>=0 && nLen > 0)
				{
					xbXml xml;
					if(::_wcsicmp(pSubType,L"code")!=0 &&(::_wcsicmp(type.bstrVal,L"request")==0 || ::_wcsicmp(type.bstrVal,L"template")==0 ||
						::_wcsicmp(type.bstrVal,L"report")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0 ||
						::_wcsicmp(type.bstrVal,L"entry")==0 || ::_wcsicmp(type.bstrVal,L"worknode")==0 ||
						::_wcsicmp(type.bstrVal,L"reqfile")==0||::_wcsicmp(type.bstrVal,L"xquery")==0 || 
						::_wcsicmp(type.bstrVal,L"xaction")==0 || ::_wcsicmp(type.bstrVal,L"serialize")==0||
						::_wcsicmp(type.bstrVal,L"xdv")==0 || ::_wcsicmp(type.bstrVal,L"xruler")==0 ||
						::_wcsicmp(type.bstrVal,L"xfile")==0 || ::_wcsicmp(type.bstrVal,L"xmode")==0  ||
						::_wcsicmp(type.bstrVal,L"bffile")==0))
					{
						if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
						{
							xml.LoadXml((LPTSTR)(pData + 2));
						}else if((pData[0]==0xEF &&  pData[1]==0xBB && pData[2]==0xBF))
						{
							int _convert = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)pData+3, nLen, NULL, 0);
							TCHAR * psz = new TCHAR[_convert + 1];
							int nRet = MultiByteToWideChar(CP_UTF8, 0,(LPSTR)pData+3, nLen, psz, _convert);
							psz[_convert]='\0';
							xml.LoadXml(psz);
							delete psz;
						}
						else if(pData[0] !=0 && pData[1]!=0) //ansi should gb2312
						{
							int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, nLen, NULL, 0);
							TCHAR * psz = new TCHAR[_convert + 1];
							int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, nLen, psz, _convert);
							psz[_convert]='\0';
							xml.LoadXml(psz);
							delete psz;
						}else //should utf-16
						{
							xml.LoadXml((LPTSTR)pData);
						}
					}
					if(::_wcsicmp(pSubType,L"xml")==0)
					{
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("content"));
						pControl->LoadContent((LPTSTR)(pData + 2),L".xml"/*SCT_C_PLUS_PLUS*/);
						//pControl->SetSyntaxColoring(SCT_C_PLUS_PLUS);
						//SetFocus(pControl->m_hWnd);
						//SetWindowText(pWinEx->GetControl(_T("codecontent"))->m_hWnd,(LPTSTR)(pData + 2));
					}
					else if(::_wcsicmp(pSubType,L"worklist")==0|| ::_wcsicmp(pSubType,L"template")==0||::_wcsicmp(type.bstrVal,L"request")==0 || ::_wcsicmp(type.bstrVal,L"template")==0 ||
						::_wcsicmp(type.bstrVal,L"report")==0 || ::_wcsicmp(type.bstrVal,L"worklist")==0 /*|| ::_wcsicmp(type.bstrVal,L"reqfile")==0*/)
					{
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
					}else if(::_wcsicmp(pSubType,L"entry")==0 ||::_wcsicmp(type.bstrVal,L"entry")==0)
					{
						CDevSchema * pSchema = (CDevSchema *)pWinEx;
						IXMLDOMElement * pDocElement;
						xml.m_pXmlDoc->get_documentElement(&pDocElement);
						if(pDocElement)
						{
							
							pSchema->Initial(pDocElement);
							pDocElement->Release();
						}
						
					}else if(::_wcsicmp(pSubType,L"worknode")==0 || ::_wcsicmp(type.bstrVal,L"worknode")==0)
					{
						CWorkNodeDlg * pWorkNode = (CWorkNodeDlg *)pWinEx;
						IXMLDOMElement * pDocElement;
						xml.m_pXmlDoc->get_documentElement(&pDocElement);
						if(pDocElement)
						{
							
							pWorkNode->Initial(pDocElement);
							pDocElement->Release();
						}
						
					}else if(::_wcsicmp(pSubType,L"xdv")==0||::_wcsicmp(pSubType,L"reqfile")==0||::_wcsicmp(type.bstrVal,L"xdv")==0 ||::_wcsicmp(type.bstrVal,L"reqfile")==0 )
					{
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("xcontent"));
						BSTR bstr = xml.GetXml();
						pControl->LoadContent(bstr,L".xml");
						if(bstr)::SysFreeString(bstr);
					}else if(::_wcsicmp(pSubType,L"xpage")==0||::_wcsicmp(type.bstrVal,L"xpage")==0)
					{
						//SetWindowText(pWinEx->GetControl(_T("xpagecontent"))->m_hWnd,(LPTSTR)(pData + 2));
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("xpagecontent"));
						pControl->LoadContent((LPTSTR)(pData + 2),L".xml"/*SCT_HTML*/);

						xfUriLoader::FreeData(pData);
						_tcscat_s(strCmd,255,_T(" xss"));
						nRet = xfUriLoader::ProcessCmd(GetWin()->GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen);
						if(nRet >=0 && pData)
						{
							pControl = (XSEdit *)pWinEx->GetControl(_T("xcodecontent"));
							pControl->LoadContent((LPTSTR)(pData + 2),L".cpp"/*SCT_C_PLUS_PLUS*/);
						}						
					}else if((::_wcsicmp(pSubType,L"code")==0) || ::_wcsicmp(type.bstrVal,L"code")==0)
					{
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("codecontent"));
						if(caption.bstrVal && _tcsstr(caption.bstrVal,L".js"))
							pControl->LoadContent((LPTSTR)(pData + 2),L".js"/*SCT_C_PLUS_PLUS*/);
						else
							pControl->LoadContent((LPTSTR)(pData + 2),L".cpp"/*SCT_C_PLUS_PLUS*/);
						//pControl->SetSyntaxColoring(SCT_C_PLUS_PLUS);
						//SetFocus(pControl->m_hWnd);
						//SetWindowText(pWinEx->GetControl(_T("codecontent"))->m_hWnd,(LPTSTR)(pData + 2));

						
					}else if(type.bstrVal && ::_wcsicmp(type.bstrVal,L"serialize")==0)
					{
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("content"));
						pControl->LoadContent((LPTSTR)(pData + 2),L".xml"/*SCT_C_PLUS_PLUS*/);
						//pControl->SetSyntaxColoring(SCT_C_PLUS_PLUS);
						//SetFocus(pControl->m_hWnd);
						//SetWindowText(pWinEx->GetControl(_T("codecontent"))->m_hWnd,(LPTSTR)(pData + 2));
					}else if(pSubType[0]==0 && type.bstrVal && (::_wcsicmp(type.bstrVal,L"xruler")==0 || ::_wcsicmp(type.bstrVal,L"bffile")==0))
					{
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("xcontent"));
						pControl->LoadContent((LPTSTR)(pData + 2),L".xml"/*SCT_C_PLUS_PLUS*/);
						//pControl->SetSyntaxColoring(SCT_C_PLUS_PLUS);
						//SetFocus(pControl->m_hWnd);
						//SetWindowText(pWinEx->GetControl(_T("codecontent"))->m_hWnd,(LPTSTR)(pData + 2));
					}else if(pSubType[0]==0 && type.bstrVal && ::_wcsicmp(type.bstrVal,L"xfile")==0)
					{
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("xcontent"));
						pControl->LoadContent((LPTSTR)(pData + 2),L".xml"/*SCT_C_PLUS_PLUS*/);
						//pControl->SetSyntaxColoring(SCT_C_PLUS_PLUS);
						//SetFocus(pControl->m_hWnd);
						//SetWindowText(pWinEx->GetControl(_T("codecontent"))->m_hWnd,(LPTSTR)(pData + 2));
					}else if(pSubType[0]==0 && type.bstrVal && pSubType[0] && ::_wcsicmp(type.bstrVal,L"xmode")==0)
					{
						XSEdit * pControl = (XSEdit *)pWinEx->GetControl(_T("xcontent"));
						pControl->LoadContent((LPTSTR)(pData + 2),L".xml"/*SCT_C_PLUS_PLUS*/);
						//pControl->SetSyntaxColoring(SCT_C_PLUS_PLUS);
						//SetFocus(pControl->m_hWnd);
						//SetWindowText(pWinEx->GetControl(_T("codecontent"))->m_hWnd,(LPTSTR)(pData + 2));
					}else if(::_wcsicmp(pSubType,L"xquery")==0 || ::_wcsicmp(type.bstrVal,L"xquery")==0)
					{
						IXMLDOMElement * pElement;
						xml.m_pXmlDoc->get_documentElement(&pElement);
						if(!pElement)
						{
							xbXml xml;
							xml.LoadXml(_T("<?xml version=\"1.0\" encoding=\"UTF-16\"?><xquery/>"));
							xml.m_pXmlDoc->get_documentElement(&pElement);
						}
						if(pElement)
						{
							((DevXQuery *)pWinEx)->LoadContent(pElement);
							pElement->setAttribute(L"guid",(_variant_t)pStrGuidItem);
							pElement->Release();
						}
					}else if(::_wcsicmp(pSubType,L"xaction")==0 || ::_wcsicmp(type.bstrVal,L"xaction")==0)
					{
						IXMLDOMElement * pElement;
						xml.m_pXmlDoc->get_documentElement(&pElement);
						if(!pElement)
						{
							xbXml xml;
							xml.LoadXml(_T("<?xml version=\"1.0\" encoding=\"UTF-16\"?><xaction/>"));
							xml.m_pXmlDoc->get_documentElement(&pElement);
						}
						if(pElement)
						{
							((DevXQuery *)pWinEx)->LoadContent(pElement);
							pElement->setAttribute(L"guid",(_variant_t)pStrGuidItem);
							pElement->Release();
						}
					}
				}
				if(pData)delete pData;

			}else if(pLayerSheet->m_SheetSize >0)
			{
				pLayerSheet->SelectSheet(i);
			}
			delete pStrFormat;
			delete pStrXml;
		}
	}
	return true;

}

XFRAME_EVENT_(CDevView::XDevTreeCtrl::SelectChanged)
{
	HWND hWnd = pEvent->hWnd;
	HTREEITEM hItem = ((LPNMTREEVIEW)pEvent->notify.pnmh)->itemNew.hItem;
	TVITEMEX	tvItem;
	int	nLevel;
	tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
	tvItem.hItem = hItem;
	TreeView_GetItem(m_hWnd,&tvItem);

	IXMLDOMElement * pElement = NULL;

	nLevel = this->GetLevel(hItem);
	if(nLevel <= 0)
	{
		IXMLDOMNode* pNode;
		IXMLDOMElement* pElementEx;
		if(this->pNodeAgent)
		{
			TCHAR buf[128];
			if(nLevel == 1)
				_tcscpy_s(buf,128,L"projects");
			else
				_tcscpy_s(buf,128,L"project");

			if(m_pAgents)
			{
				int k = 0;
				for(k=0;k<(int)m_pAgents->size();k++)
				{
					if(_tcsicmp((*m_pAgents)[k].pType,buf)==0)break;
				}
				if(k<(int)m_pAgents->size())
				{
					this->m_pxfNode->GetXPage()->SetAgentNodeContent(NULL,(*m_pAgents)[k].pNode,m_pxfNode);
					return true;
				}
			}
			if (nLevel ==1)
			{
				pNodeAgent->selectSingleNode(L"projects",&pNode);
			}
			else
			{
				pNodeAgent->selectSingleNode(L"project",&pNode);
			}
			if(pNode)
			{
				pElementEx = xbXml::GetElement(pNode);
				xfNode * pAgent = this->m_pxfNode->GetXPage()->GetAgentNode();
				pAgent->GetXPage()->m_pFocusNodeControl = NULL;

				xfNode * pxfNode = this->m_pxfNode->GetXPage()->SetAgentNodeContent(NULL,pElementEx,m_pxfNode);  
				if(!m_pAgents) m_pAgents = new vector<tagAgent>;
				tagAgent agent;
				agent.pType = _tcsdup(buf);
				agent.pNode = pxfNode;
				m_pAgents->push_back(agent);

				pElementEx->Release();
				pNode->Release();
			}else
				this->m_pxfNode->GetXPage()->ClearAgentNodeContent();
		}
	}

	if(nLevel>0)
	{
		_variant_t sheet,type;
		pElement = (IXMLDOMElement* )(tvItem.lParam);
		if(pElement)
		{
			/*pElement->getAttribute(L"sheet",&sheet);
			if(sheet.bstrVal)
				SwitchLayer(sheet.bstrVal,_T("devframe"));
			else
				SwitchLayer(_T("worknullsheet"),_T("devframe"));*/

			LPTSTR pStrType;
			BSTR bstr;
			pElement->getAttribute(L"type",&type);
			if(!type.bstrVal)
			{
				pElement->get_tagName(&bstr);
				pStrType = new TCHAR[_tcslen(bstr) + 2];
				if(nLevel > 2)
					_stprintf_s(pStrType,_tcslen(bstr) + 2,_T("%ss"),bstr);
				else
					_stprintf_s(pStrType,_tcslen(bstr) + 2,_T("%s"),bstr);
				::SysFreeString(bstr);
			}
			else
				pStrType =_tcsdup(type.bstrVal);
			if(pStrType)
			{
				if(m_pAgents)
				{
					int k = 0;
					for(k=0;k<(int)m_pAgents->size();k++)
					{
						if(_tcsicmp((*m_pAgents)[k].pType,pStrType)==0)break;
					}
					if(k<(int)m_pAgents->size())
					{
						this->m_pxfNode->GetXPage()->SetAgentNodeContent(NULL,(*m_pAgents)[k].pNode,m_pxfNode);
						return true;
					}
				}

				IXMLDOMNode* pNode;
				IXMLDOMElement* pElementEx;
				if(this->pNodeAgent)
				{
					pNodeAgent->selectSingleNode(pStrType,&pNode);
					if(pNode)
					{
						pElementEx = xbXml::GetElement(pNode);
						xfNode* pxfNode = this->m_pxfNode->GetXPage()->SetAgentNodeContent(NULL,pElementEx,m_pxfNode);  
						pElementEx->Release();
						pNode->Release();

						if(!m_pAgents) m_pAgents = new vector<tagAgent>;
						tagAgent agent;
						agent.pType = _tcsdup(pStrType);
						agent.pNode = pxfNode;
						m_pAgents->push_back(agent);
					}else
						this->m_pxfNode->GetXPage()->ClearAgentNodeContent();
				}
				if(pStrType)delete pStrType;
			}else
			{
				this->m_pxfNode->GetXPage()->ClearAgentNodeContent();
			}
		}
	}
	return true;
}

int CDevView::PreSelectSheet(int nIndex)
{
	xcLayerSheet * pLayerSheet = (xcLayerSheet *)GetControl(_T("mdilayer"));
	if(nIndex >= pLayerSheet->m_SheetSize) return 1;
	xfWin * pWin = (xfWin *)::GetProp((*pLayerSheet->m_pHwnds)[nIndex],_T("this"));
	if(pWin )
	{
		XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
		HTREEITEM hItem;
		hItem = TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
		if(pWin->m_pParam && pWin->m_pParam != hItem )
		{
			TreeView_SelectItem(pDevTreeCtrl->m_hWnd,pWin->m_pParam);
		}
	}

	return 1;
}

int CDevView::OnClose()
{
	xfControl* pOutPut = GetControl(_T("output"));
	if(xfApp::GetApp()->m_pOutPut = pOutPut)
		xfApp::GetApp()->m_pOutPut = NULL;
	return 1;
}

int CDevView::OnOpen()
{
	xfControl* pOutPut = GetControl(_T("output"));
	if(!pOutPut) GetFrameWindow()->GetControl(_T("output"));
	if(pOutPut && !xfApp::GetApp()->m_pOutPut) xfApp::GetApp()->m_pOutPut = pOutPut;
	
	XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
	HTREEITEM hItem = TreeView_GetRoot(pDevTreeCtrl->m_hWnd);
	TreeView_Expand(pDevTreeCtrl->m_hWnd,hItem,TVE_EXPAND|TVE_EXPANDPARTIAL  );
	return 1;
}

int CDevView::SelectSheet(int nIndex)
{
	return 1;
}

int CDevView::AddSheet(xfWin * pWin)
{
	xcLayerSheet * pLayerSheet = (xcLayerSheet *)GetControl(_T("mdilayer"));
	if(pWin)
	{
		XDevTreeCtrl * pDevTreeCtrl = (CDevView::XDevTreeCtrl *)((xfNodeControl*)m_pXPage->m_pxfNode->GetNodeByName(_T("DevExplorer")))->m_pxfControl;
		if(!pLayerSheet->m_hImageList)
		{
			pLayerSheet->m_cxImage = 16;
			pLayerSheet->m_cyImage = 15;
			pLayerSheet->m_hImageList = TreeView_GetImageList(pDevTreeCtrl->m_hWnd,TVSIL_NORMAL);
		}

		TV_ITEM	tvItem;
		HTREEITEM hItem;
		hItem = TreeView_GetSelection(pDevTreeCtrl->m_hWnd);
		tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
		tvItem.hItem = hItem;
		TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
		IXMLDOMElement * pElement = NULL;
		
		int nLevel = pDevTreeCtrl->GetLevel(hItem);
		if(nLevel > 2)
		{
			_variant_t	val;
			pElement = (IXMLDOMElement* )(tvItem.lParam);

			pElement->getAttribute(L"type",&val);
			if(val.bstrVal && _tcsicmp(_T("WorkFolder"),val.bstrVal)==0)
			{
				pLayerSheet->m_pHwnds->push_back(pWin->m_hWnd);
				pLayerSheet->AddSheet(pWin->m_pXPage->m_pxfNode->m_pCaption,18,true);
			}else
			{
				pElement->getAttribute(L"image",&val);

				pLayerSheet->m_pHwnds->push_back(pWin->m_hWnd);
				pLayerSheet->AddSheet(pWin->m_pXPage->m_pxfNode->m_pCaption,(int)val,true);
			}
		}else
		{
			pLayerSheet->m_pHwnds->push_back(pWin->m_hWnd);
			pLayerSheet->AddSheet(pWin->m_pXPage->m_pxfNode->m_pCaption,18,true);
		}
	}
	return 1;
}

