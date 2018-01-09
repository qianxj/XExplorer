#include "StdAfx.h"
#include "XGridQueryDlg.hpp"
#include "gridframesvr.hpp"
#include "xofobject.h"
#include "XGridReport.hpp"
#include "IGridReport.hpp"
#include "XGridComplier.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridQueryDlg::XGridQueryDlg(void):m_bCanDestroy(true)
	{
		m_bCloseFlag = false;
		SetEventListons();
	}

	XGridQueryDlg::~XGridQueryDlg(void)
	{
		Hxsoft::XFrame::xfControl * xcQuery = GetControl(_T("querystr"));
		if(xcQuery)
		{
			SetWindowLongPtr(xcQuery->GetHWND(),GWLP_WNDPROC,(long)m_PrevWndFunc);
			::SetProp(xcQuery->GetHWND(),L"thisex",0);
		}
	}
	bool XGridQueryDlg::SetEventListons()
	{
		this->AttachListon(WM_ACTIVATE,(FNEvent)&XGridQueryDlg::EvtActive);
		//this->AttachListon(_T("edittype"),CBN_SELENDOK,(FNEvent)&XGridQueryDlg::EvtEditTypeSelected);
		this->AttachListon(_T("rpt"),DWV_DOUBLECLICKED,(FNEvent)&XGridQueryDlg::EvtGridDBClick);
		this->AttachListon(_T("rpt"),DWV_KEYDOWN,(FNEvent)&XGridQueryDlg::EvtGridKeyDown);
		return true;
	}
	bool XGridQueryDlg::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool XGridQueryDlg::EvtActive(TEvent* pEvent,LPARAM lParam)
	{
		if(LOWORD(pEvent->wParam)==WA_INACTIVE && m_bCanDestroy )
		{
			XGridReport * pSvr = (XGridReport *)m_pParam;
			/*if(!m_bCloseFlag)
			{
				CDwTable * pDwTable =  pSvr->GetGridDataSvr()->GetDwTableFromCell(pSvr->GetCol(),pSvr->GetRow());
				if(pDwTable)
				{
					int nRow,nCol;
					pDwTable->getRowColumnFromCell(pSvr->GetCol(),pSvr->GetRow(),nRow,nCol);
					pDwTable->setDwItemText(nRow,nCol,L"");
					pDwTable->setDwItemDisplayText(nRow,nCol,L"");
					pDwTable->event_ItemChanged(nRow,nCol,L"");
				}
			}*/
			::SetFocus(pSvr->GetHWND());
			::RemoveProp(m_hWnd,_T("this"));
			::DestroyWindow(pEvent->hWnd);
		}
		else if(LOWORD(pEvent->wParam) !=WA_INACTIVE)m_bCanDestroy = true;
		return true;
	}
	bool XGridQueryDlg::EvtGridKeyDown(TEvent * pEvent, LPARAM lParam)
	{
		UINT nChar = (UINT)pEvent->notify.pnmh->idFrom;// pEvent->wParam;
		switch(nChar)
		{
			case VK_RETURN:
			{
				return OnOK();
			}
			case VK_END:
			case VK_HOME:
			case VK_PRIOR:
			case VK_NEXT:
			case VK_UP:
			case VK_DOWN:
			{
				break;
			}
			default:
			{
				Hxsoft::XFrame::xfControl * xcQuery = GetControl(_T("querystr"));
				::SetFocus(xcQuery->GetHWND());
				//LPTSTR pStrContent;
				//::SetWindowText(xcQuery->GetHWND(),pStrContent);
				//if(pStrContent)
				//::SendMessage(xcQuery->GetHWND(),EM_SETSEL,0,_tcslen(pStrContent));
				//::SendMessage(xcQuery->GetHWND(), WM_KEYDOWN, nChar, pEvent->lParam);
			}
		}

		return true;
	}
	bool XGridQueryDlg::EvtGridDBClick(TEvent * pEvent, LPARAM lParam)
	{
		return OnOK();
	}
	bool XGridQueryDlg::OnOK()
	{
		//this->OnXCommand(_T("xmOK"), NULL);
		int nRow = -1;
		IDwView * pDwView = (IDwView *)GetIControl(_T("rpt"));
		nRow = pDwView->GetRow();
		if(nRow < 1) return 1;

		XGridReport * pSvr = (XGridReport *)m_pParam;
		//PStrItem pItem = pSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(pSvr->GetCol(),pSvr->GetRow());
		CReportBandSvr::TReportBand *  pReportBand = &(pSvr->m_pDataTempl)->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Detail.bValid) return false;
		int startRow = pReportBand->Detail.startRow;
		PStrItem pItem = pSvr->m_pDataTempl->m_pContentSvr->GetItemData(pSvr->GetCol(),startRow);
		if(pItem && pItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;
			LPTSTR pDataColumn = NULL;
			LPTSTR pDisplayColumn = NULL;
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"dddwdatacolumn",&var);
				if(var.bstrVal) pDataColumn = _tcsdup(var.bstrVal);

				pElement->getAttribute(L"dddwdisplaycolumn",&var);
				if(var.bstrVal) pDisplayColumn = _tcsdup(var.bstrVal);

				if(pDataColumn)
				{
					LPCTSTR pData = pDwView->GetItemString(nRow,pDataColumn);//pDataSvr->m_pContentSvr->GetItemString(pDataColumn,nRow);
					LPCTSTR pDisplay = pDwView->GetItemString(nRow,pDisplayColumn);//pDataSvr->m_pContentSvr->GetItemString(pDisplayColumn,nRow);
					if(pData)
					{
						pSvr->SetItemString(pSvr->GetRow(),pSvr->GetColumnIndex(pSvr->GetDwColumnAtCellColumn(pSvr->GetCol())),(TCHAR *)pData);
						pSvr->SetItemDisplayString(pSvr->GetRow(),pSvr->GetColumnIndex(pSvr->GetDwColumnAtCellColumn(pSvr->GetCol())),(TCHAR *)pDisplay);
						//PStrItem pItem = pSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(pSvr->GetCol(),pSvr->GetRow());
						//if(pItem && pItem->pStrText)delete pItem->pStrText;
						//if( pDisplay) pItem->pStrText =  _tcsdup(pDisplay);

						if(pSvr->m_pEditSvr && pSvr->m_pEditSvr->m_pEdit)
						{
							if( pSvr->m_pEditSvr->m_ACol == pSvr->GetCol() && 
									pSvr->m_pEditSvr->m_ARow == pSvr->GetRow())
							{
								::SetWindowText(pSvr->m_pEditSvr->m_pEdit->m_hWnd,pData);
							}
						}
						/*CDwTable * pDwTable =  pSvr->GetGridDataSvr()->GetDwTableFromCell(pSvr->GetCol(),pSvr->GetRow());
						if(pDwTable)
						{
							int nRow,nCol;
							pDwTable->getRowColumnFromCell(pSvr->GetCol(),pSvr->GetRow(),nRow,nCol);
							pDwTable->setDwItemText(nRow,nCol,(LPTSTR)pData);
							pDwTable->event_ItemChanged(nRow,nCol,(LPTSTR)pData,(LPTSTR)pDisplay);
						}*/
						pSvr->event_ItemChanged(pSvr->GetRow(), pSvr->GetCol(), (LPTSTR)pData, (LPTSTR)pDisplay);
						m_bCloseFlag = true;
						pSvr->m_pEditSvr->HiddenEditor();
						//::DestroyWindow(m_hWnd);
						//::PostMessage(m_hWnd,WM_POSTDESTROYWINDOW,0,0);
						
					}
				}
				if(pDataColumn)delete pDataColumn;
				if(pDisplayColumn)delete pDisplayColumn;

				pElement->Release();
				pNode->Release();
				::InvalidateRect(pSvr->GetHWND(),NULL,false);

				//(IGridReport *)pSvr->AcceptText();
				//IGridReport * pDwViewEx = (IGridReport *)m_pParam;
				//pDwViewEx->AcceptText();

				//::RemoveProp(m_hWnd,_T("this"));
				::DestroyWindow(m_hWnd);
				//delete this;
				//::PostMessage(m_hWnd,WM_POSTDESTROYWINDOW,0,0);
			}
		}
		::SetFocus(pSvr->GetHWND());
		return false;
	}
	int XGridQueryDlg::OnXCommand(LPCTSTR	pStrID, Hxsoft::XFrame::xfControl * pControl)
	{
		if(!pStrID) return 0;

		if(_tcsicmp(pStrID,_T("xmQueryInit"))==0)
		{
			//OnOpen();
			IGridReport * pDwView = (IGridReport *)GetIControl(_T("rpt"));
			pDwView->SelectRow(0,false);
			int nRow = -1;
			TCHAR strItem[1024];
			::GetWindowText(GetControl(_T("querystr"))->m_hWnd,strItem,1024);
			LPCTSTR qStr = strItem;
			if(!qStr) return -1;
			if(_tcscmp(L"", qStr) == 0) return -1;
			nRow = pDwView->GetRowCount();
			if(nRow < 1) return 1;
			int k = pDwView->GetRow();
			int i = k < 1?1:k + 1;
			k = k < 0?1:k;
			int j;
			int nCol = pDwView->GetColumnCount();
			//for(i=nRow; i>0; i--)
			while(true)
			{
				if(i > nRow) i = 1;
				bool flag = true;
				for(j=1; j<=nCol; j++)
				{
					LPCTSTR str = pDwView->GetItemString(i, j);
					if(!str) continue;
					if(_tcsstr(str, qStr))
					{
						flag = false;
						break;
					}					
				}
				if(!flag)
				{
					pDwView->SelectRow(0,false);
					pDwView->SelectRow(i);
					pDwView->SetRow(i);
					pDwView->ShowRowTo(i);
					break;
					//pDwView->DeleteRow(i);
				}
				if(i == k)
					break;
				i++;
			}
		}
		else if(_tcsicmp(pStrID,_T("xmQuery"))==0)
		{
			XGridReport * pSvr = (XGridReport *)m_pParam;
			//PStrItem pItem = pSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(pSvr->GetCol(),pSvr->GetRow());
			CReportBandSvr::TReportBand *  pReportBand = &(pSvr->m_pDataTempl)->m_ReportBand->m_ReportBand;		
			if(!pReportBand->Detail.bValid) return false;
			int startRow = pReportBand->Detail.startRow;
			PStrItem pItem = pSvr->m_pDataTempl->m_pContentSvr->GetItemData(pSvr->GetCol(),startRow);
			if(pItem && pItem->pStrXml)
			{
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMNode * pNode;
				IXMLDOMElement * pElement;
				_variant_t var;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pElement->getAttribute(L"dddwdataurl",&var);
					if(var.bstrVal && var.bstrVal[0]!=0)
					{
						LPTSTR pItemStr = var.bstrVal;
						if(pItemStr[0]=='=')
						{
							CXGridComplier x;
							if(x.SetSource(pItemStr, pSvr, 0, pSvr->GetRow(), pSvr->GetCol()))
								pItemStr = _tcsdup(x.GetString());						 	
						}

						TCHAR buf[512];
						if(_tcsstr(pItemStr,_T("xquery:"))==pItemStr)
							::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pItemStr);
						else
							::_stprintf_s(buf,512,_T("%s"),pItemStr);  
									
						TCHAR strItem[1024];
						::GetWindowText(GetControl(_T("querystr"))->m_hWnd,strItem,1024);
						TCHAR WKURL[512];
						::_stprintf_s(WKURL,512,_T("<args><arg name='__wkurl' value='%s'/><arg name='querytxt' value='%s'/></args>"),pItemStr?pItemStr:L"", strItem?strItem:L"");
						if(pItemStr) delete pItemStr;
						LPTSTR pArg= WKURL;
						xbXml xdata;
						if(xfUriLoader::FetchXml(m_pXPage->m_pStrUrl,buf,pArg,xdata)!=1) return -1;
						
						BSTR bstrXml;
						xdata.m_pXmlDoc->get_xml(&bstrXml);
						//(IGridReport *)pDwView->SetReadOnly(true);
						IGridReport * pDwViewEx = (IGridReport *)GetIControl(_T("rpt"));
						pDwViewEx->LoadXml(bstrXml);
						pDwViewEx->SetReadOnly(true);
						pDwViewEx->SetSelectionMode(1);
						pDwViewEx->ShowRowTo(1);
						pDwViewEx->SelectRow(0,false);
						::SysFreeString(bstrXml);
						
						::InvalidateRect(pDwViewEx->GetHWND(),NULL,false);
					}
				}
			}
			::SetFocus(GetControl(_T("rpt"))->GetHWND());
		}
		else if(_tcsicmp(pStrID,_T("xmQueryEx"))==0)
		{
			OnXCommand(_T("xmQuery"), NULL);
			IGridReport * pDwViewEx = (IGridReport *)GetIControl(_T("rpt"));
			int rows = pDwViewEx->GetRowCount();
			if(rows == 1)
			{
				pDwViewEx->SetRow(1);
				OnXCommand(_T("xmOK"), NULL);
				m_dwStatus &= ~WST_RESPONSELOOP;
				::PostMessage(m_hWnd,WM_QUIT,0,0);
				//::PostMessage(m_hWnd,WM_POSTDESTROYWINDOW,0,0);
			}
		}
		else if(_tcsicmp(pStrID,_T("xmClose"))==0)
		{
			::RemoveProp(m_hWnd,_T("this"));
			::DestroyWindow(m_hWnd);
			delete this;
		}
		else if(_tcsicmp(pStrID,_T("xmOK"))==0)
		{
			int nRow = -1;
			IDwView * pDwView = (IDwView *)GetIControl(_T("rpt"));
			nRow = pDwView->GetRow();
			if(nRow < 1) return 1;

			XGridReport * pSvr = (XGridReport *)m_pParam;
			//PStrItem pItem = pSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(pSvr->GetCol(),pSvr->GetRow());
			CReportBandSvr::TReportBand *  pReportBand = &(pSvr->m_pDataTempl)->m_ReportBand->m_ReportBand;		
			if(!pReportBand->Detail.bValid) return false;
			int startRow = pReportBand->Detail.startRow;
			PStrItem pItem = pSvr->m_pDataTempl->m_pContentSvr->GetItemData(pSvr->GetCol(),startRow);
			if(pItem && pItem->pStrXml)
			{
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMNode * pNode;
				IXMLDOMElement * pElement;
				_variant_t var;
				LPTSTR pDataColumn = NULL;
				LPTSTR pDisplayColumn = NULL;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pElement->getAttribute(L"dddwdatacolumn",&var);
					if(var.bstrVal) pDataColumn = _tcsdup(var.bstrVal);

					pElement->getAttribute(L"dddwdisplaycolumn",&var);
					if(var.bstrVal) pDisplayColumn = _tcsdup(var.bstrVal);

					if(pDataColumn)
					{
						LPCTSTR pData = pDwView->GetItemString(nRow,pDataColumn);//pDataSvr->m_pContentSvr->GetItemString(pDataColumn,nRow);
						LPCTSTR pDisplay = pDwView->GetItemString(nRow,pDisplayColumn);//pDataSvr->m_pContentSvr->GetItemString(pDisplayColumn,nRow);
						if(pData)
						{
							pSvr->SetItemString(pSvr->GetRow(),pSvr->GetColumnIndex(pSvr->GetDwColumnAtCellColumn(pSvr->GetCol())),(TCHAR *)pData);
							if(pDisplayColumn) pSvr->SetItemDisplayString(pSvr->GetRow(),pSvr->GetColumnIndex(pSvr->GetDwColumnAtCellColumn(pSvr->GetCol())),(TCHAR *)pDisplay);
							//pSvr->GetGridDataSvr()->m_pContentSvr->SetItemString(pSvr->GetCol(),pSvr->GetRow(),(TCHAR *)pData);
							//PStrItem pItem = pSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(pSvr->GetCol(),pSvr->GetRow());
							//if(pItem && pItem->pStrText)delete pItem->pStrText;
							//if( pDisplay) pItem->pStrText =  _tcsdup(pDisplay);

							if(pSvr->m_pEditSvr && pSvr->m_pEditSvr->m_pEdit)
							{
								if( pSvr->m_pEditSvr->m_ACol == pSvr->GetCol() && 
										pSvr->m_pEditSvr->m_ARow == pSvr->GetRow())
								{
									::SetWindowText(pSvr->m_pEditSvr->m_pEdit->m_hWnd,pData);
								}
							}
							/*CDwTable * pDwTable =  pSvr->GetGridDataSvr()->GetDwTableFromCell(pSvr->GetCol(),pSvr->GetRow());
							if(pDwTable)
							{
								int nRow,nCol;
								pDwTable->getRowColumnFromCell(pSvr->GetCol(),pSvr->GetRow(),nRow,nCol);
								pDwTable->setDwItemText(nRow,nCol,(LPTSTR)pData);
								pDwTable->event_ItemChanged(nRow,nCol,(LPTSTR)pData,(LPTSTR)pDisplay);
							}*/
							pSvr->event_ItemChanged(pSvr->GetRow(), pSvr->GetCol(), (LPTSTR)pData, (LPTSTR)pDisplay);
							m_bCloseFlag = true;
							pSvr->m_pEditSvr->HiddenEditor();
							//::DestroyWindow(m_hWnd);
							//::PostMessage(m_hWnd,WM_POSTDESTROYWINDOW,0,0);
							
						}
					}
					if(pDataColumn)delete pDataColumn;
					if(pDisplayColumn)delete pDisplayColumn;

					pElement->Release();
					pNode->Release();

					::SetFocus(pSvr->GetHWND());
					::InvalidateRect(pSvr->GetHWND(),NULL,false);
					OnXCommand(_T("xmClose"), NULL);
					//::RemoveProp(m_hWnd,_T("this"));
					//::DestroyWindow(m_hWnd);
					//delete this;
					//::PostMessage(m_hWnd,WM_POSTDESTROYWINDOW,0,0);
				}
			}
		}
		return 1;
	}

	int XGridQueryDlg::OnOpenQueryEx(LPTSTR pQueryStr)
	{
		XGridReport * pSvr = (XGridReport *)m_pParam;
		LPTSTR pStr = pQueryStr? pQueryStr :  (LPTSTR)pSvr->GetItemDisplayString(pSvr->GetRow(), pSvr->GetCol() + 1);
	
		if(pQueryStr)
		{
			::SetWindowText(GetControl(_T("querystr"))->m_hWnd,pQueryStr);
			OnXCommand(L"xmQuery", NULL);
		}else
		{
			OnXCommand(L"xmQuery", NULL);
			::SetWindowText(GetControl(_T("querystr"))->m_hWnd,pQueryStr);
		}
		//IGridReport * pDwView = (IGridReport *)GetIControl(_T("rpt"));
		//pDwView->SetRow(0);
		OnXCommand(L"xmQueryInit", NULL);	
		if(!pQueryStr)::SetWindowText(GetControl(_T("querystr"))->m_hWnd,L"");
		
		return 1;
	}

	int XGridQueryDlg::OnOpenQuery(LPTSTR pData)
	{
		XGridReport * pSvr = (XGridReport *)m_pParam;
		LPTSTR pStr = (LPTSTR)pSvr->GetItemDisplayString(pSvr->GetRow(), pSvr->GetCol() + 1);
		if(!pData)
		{
			OnXCommand(L"xmQuery", NULL);
			if(pStr)::SetWindowText(GetControl(_T("querystr"))->m_hWnd,pStr);
			//IGridReport * pDwView = (IGridReport *)GetIControl(_T("rpt"));
			//pDwView->SetRow(0);
			OnXCommand(L"xmQueryInit", NULL);	
			::SetWindowText(GetControl(_T("querystr"))->m_hWnd,L"");
		}
		else
		{
			if(pStr)::SetWindowText(GetControl(_T("querystr"))->m_hWnd,pStr);
			IGridReport * pDwViewEx = (IGridReport *)GetIControl(_T("rpt"));
			pDwViewEx->LoadXml(pData);
			pDwViewEx->SetReadOnly(true);
			pDwViewEx->SetSelectionMode(1);
			::InvalidateRect(pDwViewEx->GetHWND(),NULL,false);	
		}
		return 1;
	}
	int XGridQueryDlg::OnOpen()
	{
		Hxsoft::XFrame::xfControl * xcQuery = GetControl(_T("querystr"));
		m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(xcQuery->GetHWND(),GWLP_WNDPROC);
		SetWindowLongPtr(xcQuery->GetHWND(),GWLP_WNDPROC,(long)&XGridQueryDlg::WindowProc);
		::SetProp(xcQuery->GetHWND(),L"thisex",(HANDLE)this);

		XGridReport * pSvr = (XGridReport *)m_pParam;
		::SetFocus(pSvr->GetHWND());
		//PStrItem pItem = pSvr->GetGridDataSvr()->m_pContentSvr->GetItemData(pSvr->GetCol(),pSvr->GetRow());
		CReportBandSvr::TReportBand *  pReportBand = &(pSvr->m_pDataTempl)->m_ReportBand->m_ReportBand;		
		if(!pReportBand->Detail.bValid) return false;
		int startRow = pReportBand->Detail.startRow;
		PStrItem pItem = pSvr->m_pDataTempl->m_pContentSvr->GetItemData(pSvr->GetCol(),startRow);
		if(pItem && pItem->pStrXml)
		{
			xbXml xml;
			xml.LoadXml(pItem->pStrXml);
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;
			xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"dddwtemplateurl",&var);
				if(var.bstrVal && var.bstrVal[0] !=0)
				{
					TCHAR buf[512];
					if(_tcsstr(var.bstrVal,_T("dev:sheet"))==var.bstrVal)
						::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),var.bstrVal);
					else
						::_stprintf_s(buf,512,_T("%s"),var.bstrVal);

					xbXml xmlRpt;
					if(xfUriLoader::FetchXml(m_pXPage->m_pStrUrl,buf,NULL,xmlRpt)==1)
					{
						IGridReport * pDwView = (IGridReport *)GetIControl(_T("rpt"));
						pDwView->SetDataObject(xmlRpt.m_pXmlDoc);
						pDwView->SetSelectionMode(1);
					}
				}

				LPTSTR pContent = NULL;
				IDwView * pDwView = (IDwView *)GetIControl(_T("rpt"));
				//pDwView->SetReadOnly();
				/*if(pSvr->GetCurDwTable())
				{
					DWNMHDR hdr;
					int nRow,nCol;
					memset(&hdr,0,sizeof(DWNMHDR));
					hdr.hwndFrom =pSvr->GetHWND();
					hdr.idFrom = 0;
					hdr.code = DWV_CHILDCONTENT;
					hdr.DwTable = (IDwTable *)pSvr->GetCurDwTable()->GetInterface();
					
					pSvr->GetCurDwTable()->getRowColumnFromCell(pSvr->FCurrent.x,pSvr->FCurrent.y, nRow,nCol);
					hdr.row = nRow;
					hdr.col = nCol;
					hdr.colname = pSvr->GetCurDwTable()->getColumnName(nCol);

					SendMessage(::GetParent(pSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
					if(hdr.data) pContent = hdr.data;
				}*/
				if(pContent)
				{
					pDwView->LoadXml(pContent);
					::InvalidateRect(pDwView->GetHWND(),NULL,false);
				}else
				{
					/*OnXCommand(L"xmQuery", NULL);
					CGridFrameSvr * pSvr = (CGridFrameSvr *)m_pParam;
					LPTSTR pStr = pSvr->m_pEditSvr->m_pEdit->GetText();
					::SetWindowText(GetControl(_T("querystr"))->m_hWnd,pStr);
					//IGridReport * pDwView = (IGridReport *)GetIControl(_T("rpt"));
					//pDwView->SetRow(0);
					OnXCommand(L"xmQueryInit", NULL);
					pElement->getAttribute(L"dddwdataurl",&var);
					if(var.bstrVal && var.bstrVal[0]!=0)
					{
						TCHAR buf[512];
						if(_tcsstr(var.bstrVal,_T("xquery:"))==var.bstrVal)
							::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),var.bstrVal);
						else
							::_stprintf_s(buf,512,_T("%s"),var.bstrVal);  

						int nRow = pDwView->GetRow();
						
						LPTSTR pArg = NULL;
	
						if(pSvr->GetCurDwTable())
						{
							DWNMHDR hdr;
							int nRow,nCol;

							memset(&hdr,0,sizeof(DWNMHDR));
							hdr.hwndFrom =pSvr->GetHWND();
							hdr.idFrom = 0;
							hdr.code = DWV_CHILDARGS;
							hdr.DwTable = (IDwTable *)pSvr->GetCurDwTable()->GetInterface();
							
							pSvr->GetCurDwTable()->getRowColumnFromCell(pSvr->FCurrent.x,pSvr->FCurrent.y, nRow,nCol);
							hdr.row = nRow;
							hdr.col = nCol;
							hdr.colname = pSvr->GetCurDwTable()->getColumnName(nCol);

							::SendMessage(::GetParent(pSvr->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
							if(hdr.data) pArg = hdr.data;
						}
						if(!pArg)
						{						
							TCHAR WKURL[512];
							::_stprintf_s(WKURL,512,_T("<args><arg name='__wkurl' value='%s'/></args>"),var.bstrVal);
							pArg = WKURL;
						}
						xbXml xdata;
						if(xfUriLoader::FetchXml(m_pXPage->m_pStrUrl,buf,pArg,xdata)!=1) return -1;
						
						BSTR bstrXml;
						xdata.m_pXmlDoc->get_xml(&bstrXml);
						pDwView->LoadXml(bstrXml);
						//(IGridReport *)pDwView->SetReadOnly(true);
						IGridReport * pDwViewEx = (IGridReport *)GetIControl(_T("rpt"));
						pDwViewEx->SetReadOnly(true);
						pDwViewEx->SetSelectionMode(1);
						::SysFreeString(bstrXml);
						
						::InvalidateRect(pDwView->GetHWND(),NULL,false);
					}*/
				}
				pNode->Release();
				pElement->Release();
				::SetFocus(pDwView->GetHWND());
			}
		}

		return 1;
	}

	bool XGridQueryDlg::GetClientRect(RECT *pRect)
	{
		::GetClientRect(m_hWnd,pRect);
		return true;
	}
	HWND XGridQueryDlg::GetHWND()
	{
		return m_hWnd;
	}
	LRESULT CALLBACK XGridQueryDlg::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		XGridQueryDlg * pThis = ((XGridQueryDlg *)GetProp(hwnd,L"thisex"));
		switch(uMsg)
		{
			case WM_KEYDOWN:
				{
					UINT nChar = (UINT)wParam;
					if(nChar == VK_RETURN)
					{
						pThis->OnXCommand(_T("xmQuery"), NULL);
						break;
					}
				}
			default:
				return CallWindowProc(pThis->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
		}
	}
}}}}
