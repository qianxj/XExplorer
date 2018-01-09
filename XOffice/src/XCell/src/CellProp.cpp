#include "StdAfx.h"
#include "..\include\CellProp.hpp"
#include "xofobject.h"
#include "GridFrameSvr.hpp"
#include "IGridReport.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	CCellProp::CCellProp(void)
	{
		SetEventListons();
	}

	CCellProp::~CCellProp(void)
	{
	}
	
	bool CCellProp::SetEventListons()
	{
		this->AttachListon(_T("edittype"),CBN_SELENDOK,(FNEvent)&CCellProp::EvtEditTypeSelected);
		return true;
	}
	bool CCellProp::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	int CCellProp::OnXCommand(LPCTSTR	pStrID, Hxsoft::XFrame::xfControl * pControl)
	{
		if(!pStrID) return 1;
		if(_tcsicmp(pStrID,_T("xmddlb_addrow"))==0)
		{
			IDwView * pReort = (IDwView *)GetControl(_T("ddlbgrid"))->GetInterface();
			pReort->InsertRow(0);
			return 1;
		}
		if(_tcsicmp(pStrID,_T("xmddlb_insertrow"))==0)
		{
			IDwView * pReort = (IDwView *)GetControl(_T("ddlbgrid"))->GetInterface();
			pReort->InsertRow(pReort->GetRow());
			return 1;
		}
		if(_tcsicmp(pStrID,_T("xmddlb_deleterow"))==0)
		{
			IDwView * pReort = (IDwView *)GetControl(_T("ddlbgrid"))->GetInterface();
			pReort->DeleteRow(pReort->GetRow());
			return 1;
		}
		if(_tcsicmp(pStrID,_T("xmlistbox_addrow"))==0)
		{
			IDwView * pReort = (IDwView *)GetControl(_T("listboxgrid"))->GetInterface();
			pReort->InsertRow(0);
			return 1;
		}
		if(_tcsicmp(pStrID,_T("xmlistbox_insertrow"))==0)
		{
			IDwView * pReort = (IDwView *)GetControl(_T("listboxgrid"))->GetInterface();
			pReort->InsertRow(pReort->GetRow());
			return 1;
		}
		if(_tcsicmp(pStrID,_T("xmlistbox_deleterow"))==0)
		{
			IDwView * pReort = (IDwView *)GetControl(_T("listboxgrid"))->GetInterface();
			pReort->DeleteRow(pReort->GetRow());
			return 1;
		}


		if(_tcsicmp(pStrID,_T("selectdwtemplate"))==0)
		{
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(this->m_pParentWin->m_pXPage->m_pStrUrl,_T("/developer/templatedlgdw.xpage"),strUrl,&dwSize,0);


			RECT rt = pControl->m_pxfNode->m_AreaRect;
			MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
			xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|/*WS_EX_TOPMOST|*/WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
			return 1;
		}
		if(_tcsicmp(pStrID,_T("selectdatasource"))==0)
		{
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(this->m_pParentWin->m_pXPage->m_pStrUrl,_T("xqueryselectdlg.xpage"),strUrl,&dwSize,0);


			RECT rt = pControl->m_pxfNode->m_AreaRect;
			MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
			xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW|/*WS_EX_TOPMOST|*/WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
			return 1;
		}
		if(_tcsicmp(pStrID,_T("selectxpage"))==0)
		{
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(this->m_pParentWin->m_pXPage->m_pStrUrl,_T("xpageSelectdlg.xpage"),strUrl,&dwSize,0);


			RECT rt = pControl->m_pxfNode->m_AreaRect;
			MapWindowPoints(m_hWnd,NULL,(LPPOINT)&rt,2);
			xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,WS_EX_TOOLWINDOW/*|WS_EX_TOPMOST*/|WS_EX_NOACTIVATE,WS_DLGFRAME|WS_POPUP,&rt,m_hWnd,xfApp::GetApp()->GetInstance(),NULL,this);
			return 1;
		}
		if(pStrID && _tcsicmp(pStrID,_T("xmOKWorkNodeEx"))==0)
		{
			Hxsoft::XFrame::xfControl * pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
			HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
			if(!TreeView_GetParent(pTree->m_hWnd,hItem))
			{
				MessageBox(pControl->GetWin()->m_hWnd,_T("请选择XPage对象"),_T("提示"),MB_OK);
				return 1;
			}
			IXMLDOMElement * pElement;
			TVITEM	item;
			item.hItem = hItem;
			item.mask = TVIF_PARAM;
			TreeView_GetItem(pTree->m_hWnd,&item);
			pElement = (IXMLDOMElement *)item.lParam;
			_variant_t	var;
			_variant_t	var1;
			pElement->getAttribute(L"type",&var);
			if(_tcsicmp(var.bstrVal,_T("xpage"))!=0)
			{
				MessageBox(pControl->GetWin()->m_hWnd,_T("请选择XPage对象"),_T("提示"),MB_OK);
				return 1;
			}

			pElement->getAttribute(L"caption",&var);
			pElement->getAttribute(L"guid",&var1);

			TCHAR buf[512];
			_stprintf_s(buf,512,_T("dev:xpage%s?describe=%s"),var1.bstrVal,var.bstrVal);
			::SetWindowText(GetControl(_T("dddwxpageurl"))->m_hWnd,buf);
			pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;

			return 1;
		}
		if(pStrID && _tcsicmp(pStrID,_T("xmOKXQuery"))==0)
		{
			Hxsoft::XFrame::xfControl * pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
			HTREEITEM hItem = TreeView_GetSelection(pTree->m_hWnd);
			if(!TreeView_GetParent(pTree->m_hWnd,hItem))
			{
				MessageBox(pControl->GetWin()->m_hWnd,_T("请选择XQuery对象"),_T("提示"),MB_OK);
				return 1;
			}
			IXMLDOMElement * pElement;
			TVITEM	item;
			item.hItem = hItem;
			item.mask = TVIF_PARAM;
			TreeView_GetItem(pTree->m_hWnd,&item);
			pElement = (IXMLDOMElement *)item.lParam;
			_variant_t	var;
			_variant_t	var1;
			pElement->getAttribute(L"type",&var);
			if(_tcsicmp(var.bstrVal,_T("xquery"))!=0)
			{
				MessageBox(pControl->GetWin()->m_hWnd,_T("请选择XQuery对象"),_T("提示"),MB_OK);
				return 1;
			}

			pElement->getAttribute(L"caption",&var);
			pElement->getAttribute(L"guid",&var1);

			TCHAR buf[512];
			_stprintf_s(buf,512,_T("xquery:%s?describe=%s"),var1.bstrVal,var.bstrVal);
			::SetWindowText(GetControl(_T("dddwdataurl"))->m_hWnd,buf);
			pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;

			return 1;
		}
		if(pStrID && _tcsicmp(pStrID,_T("template_addsheet"))==0)
		{
			Hxsoft::XFrame::xfControl * pTree = pControl->GetWin()->GetControl(_T("DevExplorer"));
			Hxsoft::XFrame::xfControl * pView = pControl->GetWin()->GetControl(_T("sheetview"));

			TCHAR pText[255];
			GetWindowText(pControl->GetWin()->GetControl(_T("sheetname"))->m_hWnd,pText,255);
			int nIndex = ListView_GetNextItem(pView->m_hWnd,-1,LVNI_SELECTED);
			if(nIndex < 0) return 1;
			
			LVITEM	lvItem;
			lvItem.iItem = nIndex;
			lvItem.iSubItem = 0;
			lvItem.mask = LVIF_PARAM;
			ListView_GetItem(pView->m_hWnd,&lvItem);

			TCHAR buf[512];
			_stprintf_s(buf,512,_T("dev:sheet%s?describe=%s"),(LPTSTR)lvItem.lParam,pText);
			::SetWindowText(GetControl(_T("dddwtemplateurl"))->m_hWnd,buf);
			pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;

			return 1;
		}
		if(pStrID && _tcsicmp(pStrID,_T("template_close"))==0) //send by Stemplatedlg
		{
			pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
			return 1;
		}
		if(pStrID && _tcsicmp(pStrID,_T("xmCancelXQuery"))==0) //send by Stemplatedlg
		{
			pControl->GetWin()->m_dwStatus &= ~WST_RESPONSELOOP;
			return 1;
		}

		xbXml xml;
		IXMLDOMElement * pCellProp = NULL;
		IXMLDOMElement * pElement =  NULL;
		IXMLDOMElement * pDocElement;
		IXMLDOMNode * pNode;

		XOffice::XCell::CGridFrameSvr * pGridFrameSvr = (XOffice::XCell::CGridFrameSvr*)m_pParam;
		int ARow = pGridFrameSvr->GetRow();
		int ACol = pGridFrameSvr->GetCol();
		PStrItem pItem;
		pItem = pGridFrameSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(!pItem)
		{
			pItem = new StrItem ;
			pGridFrameSvr->GetGridDataSvr()->m_pContentSvr->SetItemData(ACol,ARow,pItem);
		}
		if(pItem)
		{
			if(!pItem->pStrXml)
				xml.LoadXml(_T("<cell/>"));
			else
				xml.LoadXml(pItem->pStrXml);

			xml.m_pXmlDoc->get_documentElement(&pDocElement);
		}
		
		pDocElement->selectSingleNode(L"cellprop",&pNode);
		if(pNode)
		{
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pCellProp);
			pNode->Release();
		}else
		{
			xml.m_pXmlDoc->createElement(L"cellprop",&pCellProp);
			pDocElement->appendChild(pCellProp,NULL);
		}
		if(_tcsicmp(pStrID,_T("xmCellOk"))==0)
		{
			TCHAR strItem[1024];

			//search column
			::GetWindowText(GetControl(_T("searchcolumn"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"searchcolumn",(_variant_t)strItem);

			//celltype
			::GetWindowText(GetControl(_T("celltype"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"celltype",(_variant_t)strItem);

			//cellformat
			::GetWindowText(GetControl(_T("cellformatex"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellformatex",(_variant_t)strItem);
			::GetWindowText(GetControl(_T("cellformat"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellformat",(_variant_t)strItem);

			//cellprotect
			::GetWindowText(GetControl(_T("cellprotectex"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellprotectex",(_variant_t)strItem);
			::GetWindowText(GetControl(_T("cellprotect"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellprotect",(_variant_t)strItem);

			//cellvisible
			::GetWindowText(GetControl(_T("cellvisibleex"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellvisibleex",(_variant_t)strItem);
			::GetWindowText(GetControl(_T("cellvisible"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellvisible",(_variant_t)strItem);

			//cellneed
			if(GetControl(_T("cellneeddata")))
			{
				::GetWindowText(GetControl(_T("cellneeddata"))->m_hWnd,strItem,1024);
				pCellProp->setAttribute(L"cellneeddata",(_variant_t)strItem);
			}

			//cellvalid
			::GetWindowText(GetControl(_T("cellvalid"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellvalid",(_variant_t)strItem);

			//cellinitial
			::GetWindowText(GetControl(_T("cellinitial"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellinitial",(_variant_t)strItem);

			//cellsource
			::GetWindowText(GetControl(_T("cellsource"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"cellsource",(_variant_t)strItem);

			//fontstyle
			if(GetControl(_T("fontstyle")))
			{
				::GetWindowText(GetControl(_T("fontstyle"))->m_hWnd,strItem,1024);
				pCellProp->setAttribute(L"fontstyle",(_variant_t)strItem);
			}

			//windowsheet
			::GetWindowText(GetControl(_T("windowurl"))->m_hWnd,strItem,1024);
			pCellProp->setAttribute(L"windowurl",(_variant_t)strItem);

			//get edit prop
			pCellProp->selectSingleNode(L"edit",&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pNode->Release();
			}else
			{
				xml.m_pXmlDoc->createElement(L"edit",&pElement);
				pCellProp->appendChild(pElement,NULL);
			}

			::GetWindowText(GetControl(_T("edittype"))->m_hWnd,strItem,1024);
			pElement->setAttribute(L"edittype",(_variant_t)strItem);

			if(_tcsicmp(strItem,_T("query"))==0)
			{
				//dddw
				::GetWindowText(GetControl(_T("dddwxpageurl"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwxpageurl",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("dddwdataurl"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwdataurl",(_variant_t)strItem);
				
				::GetWindowText(GetControl(_T("dddwtemplateurl"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwtemplateurl",(_variant_t)strItem);
				
				::GetWindowText(GetControl(_T("dddwdatacolumn"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwdatacolumn",(_variant_t)strItem);
				
				::GetWindowText(GetControl(_T("dddwdisplaycolumn"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwdisplaycolumn",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("dddwwidth"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwwidth",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("dddwheight"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwheight",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("dddwautoselect"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwautoselect",(_variant_t)strItem);
				
				::GetWindowText(GetControl(_T("dddwshowbutton"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwshowbutton",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("dddwcanedit"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"dddwcanedit",(_variant_t)strItem);
			}else if(_tcsicmp(strItem,_T("checkbox"))==0)
			{
				::GetWindowText(GetControl(_T("editchecked"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"editchecked",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("uneditchecked"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"uneditchecked",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("editcheckedother"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"editcheckedother",(_variant_t)strItem);
			}else if(_tcsicmp(strItem,_T("barcode"))==0)
			{
				::GetWindowText(GetControl(_T("barwidth"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"barwidth",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("barheight"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"barheight",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("bartype"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"bartype",(_variant_t)strItem);

				::GetWindowText(GetControl(_T("bartext"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"bartext",(_variant_t)strItem);

			}else if(_tcsicmp(strItem,_T("ddlb"))==0)
			{
				if(GetControl(_T("ddlbdataurl")))
				{ 
					::GetWindowText(GetControl(_T("ddlbdataurl"))->m_hWnd,strItem,1024);
					pElement->setAttribute(L"ddlbdataurl",(_variant_t)strItem);
				}
				if(GetControl(_T("ddlbcandedit")))
				{ 
					int ret = ::SendMessage(GetControl(_T("ddlbcandedit"))->m_hWnd, BM_GETSTATE, 0, 0);
					if((ret - 8) == BST_CHECKED)
						pElement->setAttribute(L"ddlbcandedit",(_variant_t)L"Y");
					else
						pElement->removeAttribute(L"ddlbcandedit");
				}
				IDwView * pReort = (IDwView *)GetControl(_T("ddlbgrid"))->GetInterface();
				((IGridReport*)pReort)->AcceptText();
				BSTR bstr;
				pReort->GetXml(bstr);
				pElement->setAttribute(L"ddlbxml",(_variant_t)bstr);
				::SysFreeString(bstr);
			}else if(_tcsicmp(strItem,_T("listbox"))==0)
			{
				IDwView * pReort = (IDwView *)GetControl(_T("listboxgrid"))->GetInterface();
				((IGridReport*)pReort)->AcceptText();
				BSTR bstr;
				pReort->GetXml(bstr);
				pElement->setAttribute(L"listboxxml",(_variant_t)bstr);
				::GetWindowText(GetControl(_T("xmlistbox_mulit"))->m_hWnd,strItem,1024);
				pElement->setAttribute(L"listbox_mulit",(_variant_t)strItem);
				::SysFreeString(bstr);
			}


			BSTR bstr;
			pDocElement->get_xml(&bstr);
			if(pItem->pStrXml)delete pItem->pStrXml;
			pItem->pStrXml = _tcsdup(bstr);
			::SysFreeString(bstr);

			if(pElement) pElement->Release();
			if(pCellProp)pCellProp->Release();
			
			m_dwStatus &= ~WST_RESPONSELOOP;
			return 1;
		}
		
		return 1;
	}

	int CCellProp::OnOpen()
	{
		xbXml xml;
		IXMLDOMElement * pElement =  NULL;
		IXMLDOMNode * pNode;

		XOffice::XCell::CGridFrameSvr * pGridFrameSvr = (XOffice::XCell::CGridFrameSvr*)m_pParam;
		int ARow = pGridFrameSvr->GetRow();
		int ACol = pGridFrameSvr->GetCol();
		PStrItem pItem;
		pItem = pGridFrameSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow);
		if(pItem && pItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop",&pNode);
			if(pNode)
			{
				_variant_t var;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				
				pElement->getAttribute(L"searchcolumn",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("searchcolumn"))->m_hWnd,var.bstrVal);
				
				pElement->getAttribute(L"celltype",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("celltype"))->m_hWnd,var.bstrVal);
				
				pElement->getAttribute(L"cellformat",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellformat"))->m_hWnd,var.bstrVal);
				pElement->getAttribute(L"cellformatex",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellformatex"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"cellinitial",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellinitial"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"cellprotect",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellprotect"))->m_hWnd,var.bstrVal);
				pElement->getAttribute(L"cellprotectex",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellprotectex"))->m_hWnd,var.bstrVal);
				
				pElement->getAttribute(L"cellvisible",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellvisible"))->m_hWnd,var.bstrVal);
				pElement->getAttribute(L"cellvisibleex",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellvisibleex"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"cellvalid",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellvalid"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"cellsource",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("cellsource"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"fontstyle",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("fontstyle"))->m_hWnd,var.bstrVal);

				//window
				pElement->getAttribute(L"windowurl",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("windowurl"))->m_hWnd,var.bstrVal);

				if(GetControl(_T("cellneeddata")))
				{
					pElement->getAttribute(L"cellneeddata",&var);
					if(var.bstrVal)::SetWindowText(GetControl(_T("cellneeddata"))->m_hWnd,var.bstrVal);
				}

				pNode->Release();
				pElement->Release();
			}
			xml.m_pXmlDoc->selectSingleNode(L"/cell/cellprop/edit",&pNode);
			if(pNode)
			{
				_variant_t var;
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				
				pElement->getAttribute(L"edittype",&var);
				if(var.bstrVal)
				{
					::SetWindowText(GetControl(_T("edittype"))->m_hWnd,var.bstrVal);
				
					TCHAR pSheet[80];
					_stprintf_s(pSheet,80,_T("cell%ssheet"),var.bstrVal);
					GetControl(_T("edittype"))->SwitchLayer(pSheet);
				}

				pElement->getAttribute(L"editchecked",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("editchecked"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"uneditchecked",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("uneditchecked"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"editcheckedother",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("editcheckedother"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwxpageurl",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwxpageurl"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwdataurl",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwdataurl"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwtemplateurl",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwtemplateurl"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwdatacolumn",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwdatacolumn"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwdisplaycolumn",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwdisplaycolumn"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwwidth",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwwidth"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwheight",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwheight"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwautoselect",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwautoselect"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwshowbutton",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwshowbutton"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"dddwcanedit",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("dddwcanedit"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"bartype",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("bartype"))->m_hWnd,var.bstrVal);
				
				pElement->getAttribute(L"barwidth",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("barwidth"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"barheight",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("barheight"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"bartext",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("bartext"))->m_hWnd,var.bstrVal);

				IDwView * pReort = (IDwView *)GetControl(_T("ddlbgrid"))->GetInterface();
				pElement->getAttribute(L"ddlbxml",&var);
				if(var.bstrVal)
					pReort ->LoadXml(var.bstrVal);

				pElement->getAttribute(L"ddlbdataurl",&var);
				if(var.bstrVal && GetControl(_T("ddlbdataurl")))::SetWindowText(GetControl(_T("ddlbdataurl"))->m_hWnd,var.bstrVal);

				pElement->getAttribute(L"ddlbcandedit",&var);
				if(var.bstrVal && GetControl(_T("ddlbcandedit")))
					::SendMessage(GetControl(_T("ddlbcandedit"))->m_hWnd,BM_SETCHECK,BST_CHECKED,0);

				IDwView * pReortListbox = (IDwView *)GetControl(_T("listboxgrid"))->GetInterface();
				pElement->getAttribute(L"listboxxml",&var);
				if(var.bstrVal)
					pReortListbox ->LoadXml(var.bstrVal);
				pElement->getAttribute(L"listbox_mulit",&var);
				if(var.bstrVal)::SetWindowText(GetControl(_T("xmlistbox_mulit"))->m_hWnd,var.bstrVal);

				pNode->Release();
			}
		}

		return 1;
	}


	bool CCellProp::EvtEditTypeSelected(TEvent *pEvent ,LPARAM lParam)
	{
		TCHAR pBuf[40];
		TCHAR pSheet[80];
		int nIndex = (int)::SendMessage(this->GetControl(_T("edittype"))->m_hWnd,CB_GETCURSEL,0,0);
		::SendMessage(this->GetControl(_T("edittype"))->m_hWnd,CB_GETLBTEXT,nIndex,(LPARAM)(LPTSTR)pBuf);
		_stprintf_s(pSheet,80,_T("cell%ssheet"),pBuf);
		this->GetControl(_T("edittype"))->SwitchLayer(pSheet);
		
		return true;
	}

	bool CCellProp::GetClientRect(RECT *pRect)
	{
		::GetClientRect(m_hWnd,pRect);
		return true;
	}
	HWND CCellProp::GetHWND()
	{
		return m_hWnd;
	}
}}}}
