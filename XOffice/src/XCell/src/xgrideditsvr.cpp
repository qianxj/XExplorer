#include "stdafx.h"
#include "xgrideditsvr.hpp"
#include "xgridsheetsvrEx.hpp"
#include "XGridDataSvrEx.hpp"
#include "XGridReport.hpp"
#include "XGridComplier.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	int CInplaceEditEx::AttachProcEdit()
	{
		if(IsWindow(m_hWnd))
		{
			HWND hwnd = ::FindWindowEx(m_hWnd, NULL,NULL,NULL);
			if(!hwnd) hwnd = m_hWnd;
				
			m_PrevWndFunc = (WNDPROC)GetWindowLongPtr(hwnd,GWLP_WNDPROC);
			SetWindowLongPtr(hwnd,GWLP_WNDPROC,(long)&CInplaceEditEx::WindowProc);
			SetProp(hwnd,L"thisex",(HANDLE)this);
		}
		else
			m_PrevWndFunc = 0;
		return 1;
	}
	int CInplaceEditEx::DettachProcEdit()
	{
		if(IsWindow(m_hWnd))
		{
			HWND hwnd = ::FindWindowEx(m_hWnd, NULL,NULL,NULL);
			if(!hwnd) hwnd = m_hWnd;

			SetWindowLongPtr(hwnd,GWLP_WNDPROC,(long)m_PrevWndFunc);
		}
		return 1;
	}

	BOOL CInplaceEditEx::RightSide(HWND handle)
	{
	  long   EndPos;   
	  long   StartPos;
	  ::SendMessage(handle, EM_GETSEL, long(&StartPos), long(&EndPos));
	  return (StartPos == 0 || EndPos == StartPos ) && EndPos == ::GetWindowTextLength(handle);
	}

	BOOL CInplaceEditEx::LeftSide(HWND handle)
	{
	  long   EndPos;   
	  long   StartPos;
	  ::SendMessage(handle, EM_GETSEL, long(&StartPos), long(&EndPos));
	  return (StartPos == 0) && ((EndPos == 0) || (EndPos == ::GetWindowTextLength(handle)));
	}

	bool CInplaceEditEx::ToLeft(HWND handle)
	{
		CInplaceEditEx * pThis = ((CInplaceEditEx *)GetProp(handle,L"thisex"));
		int col = pThis->m_pEditSvr->m_ACol -1;
		int row = pThis->m_pEditSvr->m_ARow;
		if(col < 1)
		{
			if(row < 2) return true;
			col = pThis->m_pEditSvr->m_pGridReport->GetColumnCount();
			row--;
		}
		pThis->m_pEditSvr->ShowEditor(col, row);
		return true;
	}

	bool CInplaceEditEx::ToRight(HWND handle)
	{
		CInplaceEditEx * pThis = ((CInplaceEditEx *)GetProp(handle,L"thisex"));
		int col = pThis->m_pEditSvr->m_ACol + 1;
		int row = pThis->m_pEditSvr->m_ARow;
		if(col > pThis->m_pEditSvr->m_pGridReport->GetColumnCount())
		{
			col = 1;
			row++;
		}
		pThis->m_pEditSvr->ShowEditor(col, row);
		return true;
	}

	bool CInplaceEditEx::ToUp(HWND handle)
	{
		CInplaceEditEx * pThis = ((CInplaceEditEx *)GetProp(handle,L"thisex"));
		int col = pThis->m_pEditSvr->m_ACol;
		int row = pThis->m_pEditSvr->m_ARow - 1;
		if(row < 1) return true;
		pThis->m_pEditSvr->ShowEditor(col, row);
		pThis->m_pEditSvr->m_pGridReport->event_RowFocusChanged(row,row+1);

		return true;
	}

	bool CInplaceEditEx::ToDown(HWND handle)
	{
		CInplaceEditEx * pThis = ((CInplaceEditEx *)GetProp(handle,L"thisex"));
		int col = pThis->m_pEditSvr->m_ACol;
		int row = pThis->m_pEditSvr->m_ARow + 1;
		if(row > pThis->m_pEditSvr->m_pGridReport->GetRowCount()) return true;
		pThis->m_pEditSvr->ShowEditor(col, row);
		pThis->m_pEditSvr->m_pGridReport->event_RowFocusChanged(row,row - 1);
		return true;
	}

	LRESULT CALLBACK CInplaceEditEx::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_KEYUP:
			{
				break;
			}
		case WM_KEYDOWN:
			{
				int Shift = 0;
				if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
				if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;
				if(Shift > 0) break;

				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT)LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				TEvent evt;
				evt.key.nChar = nChar;
				evt.key.nFlags = nFlags;
				evt.key.nRepCnt = nRepCnt;

				switch(nChar)
				{
				case VK_UP:
					if(ToUp(hwnd)) return 0;
					break;
				case VK_DOWN:
					if(ToDown(hwnd)) return 0;
					break;
				case VK_NEXT:
				case VK_RIGHT:
					if(RightSide(hwnd) && ToRight(hwnd)) return 0;
					break;
				case VK_PRIOR:
				case VK_LEFT:
					if(LeftSide(hwnd) && ToLeft(hwnd)) return 0;
					break;
				}

				break;
			}
		case WM_CHAR:
			{
				int Shift = 0;
				if(GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE) Shift |= ssShift;
				if(GetKeyState(VK_CONTROL)& 0x8000 ? TRUE : FALSE) Shift |= ssCtrl;
				if(GetKeyState(VK_MENU)& 0x8000 ? TRUE : FALSE) Shift |= ssAlt;
				if(Shift > 0) break;

				UINT nChar = (UINT)wParam;
				UINT nRepCnt =(SHORT)LOWORD(lParam);
				UINT nFlags = (UINT)lParam;
				TEvent evt;
				evt.key.nChar = nChar;
				evt.key.nFlags = nFlags;
				evt.key.nRepCnt = nRepCnt;

				switch(nChar)
				{
				case VK_RETURN:
					{
						CInplaceEditEx * pThis = ((CInplaceEditEx *)GetProp(hwnd,L"thisex"));
						int col = pThis->m_pEditSvr->m_ACol;
						int row = pThis->m_pEditSvr->m_ARow + 1;
						int prow =  pThis->m_pEditSvr->m_ARow;
						bool bQuery, bShowButton;
						bQuery = pThis->m_pEditSvr->m_pGridReport->IsEditQuery(col, row - 1, bShowButton);
						if(bQuery)
						{
							pThis->m_pEditSvr->m_pGridReport->ShowQueryDlgEx(((CCellEditerEx *)pThis)->GetText());
							return 0; 
							break;
						}
						if(row > pThis->m_pEditSvr->m_pGridReport->GetTreeRowCount())
						{
							pThis->m_pEditSvr->HiddenEditor();
							return 0; 
						}
						if(goAllowEdit & pThis->m_pEditSvr->m_pGridReport->FOptions)
						{
							bool bEdit = true;
							//2017.8.23 LPCTSTR pColName = pThis->m_pEditSvr->m_pGridReport->GetColumnName(col + 1);
							LPCTSTR pColName = pThis->m_pEditSvr->m_pGridReport->GetColumnName(col);
							if(pColName)
							{
								for(int i=0; i < int(pThis->m_pEditSvr->m_pGridReport->m_ReadOnlyColumn.size()); i++)
								{
									if(_tcscmp(pColName, pThis->m_pEditSvr->m_pGridReport->m_ReadOnlyColumn[i]) == 0)
									{
										bEdit = false;
										break;
									}
								}
							}
							if(bEdit)
							{
								pThis->m_pEditSvr->ShowEditor(col, row);
								if(row <= pThis->m_pEditSvr->m_pGridReport->GetRowCount())
									if(prow!=row)pThis->m_pEditSvr->m_pGridReport->event_RowFocusChanged(row,prow);
								return 0;
							}
						}
						break;
					}
				case VK_PRIOR:
				case VK_LEFT:
					{
						//if(ToLeft(hwnd)) return 0;
						break;
					}
				case VK_NEXT:
				//case VK_RIGHT:
					/*{
						ToRight(hwnd);
						break;
					}*/
				//case VK_HOME:
				//case VK_END: 
				//case VK_F2:
				case VK_TAB:
					{
						CInplaceEditEx * pThis = ((CInplaceEditEx *)GetProp(hwnd,L"thisex"));
						int col = pThis->m_pEditSvr->m_ACol ;
						int row = pThis->m_pEditSvr->m_ARow;
						int i = 0;
						for(i = col+1;i<=pThis->m_pEditSvr->m_pGridReport->GetColumnCount();i++)
						{
							LPTSTR protect = pThis->m_pEditSvr->m_pGridReport->GetColumnProp(row,i,_T("cellprotect"));
							if(protect)
							{
								if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
									_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) continue;
							}
							break;
						}
						if(i > pThis->m_pEditSvr->m_pGridReport->GetColumnCount())
						{
							if(row<pThis->m_pEditSvr->m_pGridReport->GetRowCount())
							{
								int k = 1;
								for(int k = 1;k<=pThis->m_pEditSvr->m_pGridReport->GetColumnCount();k++)
								{
									LPTSTR protect =pThis->m_pEditSvr->m_pGridReport->GetColumnProp(row+1, k,_T("cellprotect"));
									if(protect)
									{
										if(_tcsicmp(protect,_T("是"))==0 || _tcsicmp(protect,_T("true"))==0 ||
											_tcsicmp(protect,_T("保护"))==0 || _tcsicmp(protect,_T("1"))==0) continue;
									}
									break;
								}
								if(k<=pThis->m_pEditSvr->m_pGridReport->GetColumnCount())
									pThis->m_pEditSvr->ShowEditor(k, row+1);
							}
						}else
							pThis->m_pEditSvr->ShowEditor(i, row);
						return 0;
						break;
					}
				}
				break;
			}
		case WM_KILLFOCUS:
			{
				CInplaceEditEx * pThis = ((CInplaceEditEx *)GetProp(hwnd,L"thisex"));
				//pThis->m_pEditSvr->HiddenEditor();  
			}
		}
		return CallWindowProc(((CInplaceEditEx *)GetProp(hwnd,L"thisex"))->m_PrevWndFunc,hwnd,uMsg,wParam,lParam);
	}

	LPTSTR	CCellEditerEx::GetText()
	{
		if(GetHWND())
		{
			int dwLen = GetWindowTextLength(GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(GetHWND(),pStr,dwLen + 1);
			return pStr;
		}
		else
			return NULL;
	}
	int	CCellEditerEx::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;

		if(pStrContent)
		{
			::SetWindowText(GetHWND(),pStrContent);
			::SendMessage(GetHWND(),EM_SETSEL,0,_tcslen(pStrContent));
		}
		else
		{
			::SetWindowText(GetHWND(),L"");
		}		
		return 1;
	}
	int CCellEditerEx::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}
	int CCellEditerEx::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	CControlEditerEx::CControlEditerEx():m_pxfControl(NULL)
	{
	}

	int CControlEditerEx::InstanceControl(HWND hParent,LPTSTR xClass)
	{
		m_szCotrolClass = ::_tcsdup(xClass);

		xbObject *	pObject;
		xfApp::GetApp()->m_ObjectMgr.CreateInstance(NULL,m_szCotrolClass,pObject);
		m_pxfControl = (xfControl *)pObject;
		if(m_pxfControl)
		{
			RECT rect;
			::SetRect(&rect,0,0,0,0);
			m_pxfControl->m_pxfNode= NULL;
			m_pxfControl->m_dwStyle &= ~WS_VISIBLE;
			m_pxfControl->CreateControl(NULL,rect,hParent,0,xfApp::GetApp()->GetInstance());
 			::SetProp(m_pxfControl->GetHWND(),_T("this"),m_pxfControl);
			m_pxfControl->Initial();
			this->m_hWnd = m_pxfControl->GetHWND();
		}
		
		return 1;
	}

	CControlEditerEx::~CControlEditerEx()
	{
	}

	LPTSTR	CControlEditerEx::GetText()
	{
		if(GetHWND())
		{
			LPTSTR cName = new TCHAR[255];
			::GetClassName(GetHWND(), cName, 255);
			if(_tcscmp(cName, _T("SysDateTimePick32")) == 0)
			{
				SYSTEMTIME sysTime;  
				memset(&sysTime,0,sizeof(sysTime));   
				int ret = ::SendMessage(GetHWND(), DTM_GETSYSTEMTIME, 0, (LPARAM)&sysTime);
				if(ret !=  GDT_VALID)
					return NULL;
				LPTSTR pStr = new TCHAR[11];
				_stprintf_s(pStr,11, L"%d-%d-%d", sysTime.wYear,sysTime.wMonth,sysTime.wDay);
				return pStr;
			}
			int dwLen = GetWindowTextLength(GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(GetHWND(),pStr,dwLen + 1);
			return pStr;
		}
		else
			return NULL;
	}
	int	CControlEditerEx::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;

		LPTSTR cName = new TCHAR[255];
		::GetClassName(GetHWND(), cName, 255);
		if(_tcscmp(cName, _T("SysDateTimePick32")) == 0)
		{
			int wYear,wMonth,wDay;
			if(pStrContent && _tcscmp(pStrContent, L"") != 0)
			{
				swscanf_s(pStrContent, L"%d-%d-%d ", &wYear,&wMonth,&wDay);
				SYSTEMTIME sysTime;  
				memset(&sysTime,0,sizeof(sysTime));   
				sysTime.wYear = wYear;
				sysTime.wMonth = wMonth; 
				sysTime.wDay = wDay;
				::SendMessage(GetHWND(), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)&sysTime);
			}
			else
			{
				::SetWindowText(GetHWND(),NULL);
				::SendMessage(GetHWND(), DTM_SETSYSTEMTIME, GDT_NONE, NULL);
			}
			return 1;
		}

		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	int CControlEditerEx::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}
	int CControlEditerEx::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	LPTSTR	CCellCheckBoxEditerEx::GetText()
	{
		if(GetHWND())
		{
			int r = ::SendMessage(GetHWND(), BM_GETSTATE, 0, 0);
			if(r == 8 || r == 0) return _T("0");
			else return _T("1");
		}
		else
			return NULL;
	}
	int	CCellCheckBoxEditerEx::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;
		
		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	LPTSTR	CCellCheckBoxEditerEx::GetTextData()
	{
		return NULL;
	}

	int CCellCheckBoxEditerEx::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}

	int CCellCheckBoxEditerEx::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	LPTSTR	CCellComboBoxEditerEx::GetText()
	{
		if(GetHWND())
		{
			int dwLen = GetWindowTextLength(GetHWND());
			if(dwLen < 1) return NULL;
			LPTSTR  pStr = new TCHAR[dwLen + 1];
			GetWindowText(GetHWND(),pStr,dwLen + 1);
			return pStr;
		}
		else
			return NULL;
	}
	LPTSTR	CCellComboBoxEditerEx::GetTextData()
	{
		if(GetHWND())
		{
			//LPTSTR  pText = this->GetText();
			int i;
			i = SendMessage(GetHWND(), CB_GETCURSEL, NULL, NULL);
			if(CB_ERR==(i = SendMessage(GetHWND(), CB_GETCURSEL, NULL, NULL)))
				return NULL;
			LRESULT strList = ::SendMessage(GetHWND(), CB_GETITEMDATA, i, NULL);
			LPTSTR pStrText = (LPTSTR)strList;
			return pStrText;
		}
		else
			return NULL;
	}

	int	CCellComboBoxEditerEx::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;
		
		//::SendMessage(GetHWND(),CB_RESETCONTENT ,0,0);  
		//::SendMessage(GetHWND(),CB_ADDSTRING ,0,(LPARAM)pStrContent);  
		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	int CCellComboBoxEditerEx::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			RECT	rc;
			int height;
			int	itemHeight;

			::GetWindowRect(GetHWND(),&rc);
			itemHeight = (int)::SendMessage(GetHWND(),CB_GETITEMHEIGHT,0,0);
			height = rc.bottom - rc.top + 1 + itemHeight * 30;
			height = height > cy?height:cy;
			::MoveWindow(GetHWND(),x,y,cx,height,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}

	int CCellComboBoxEditerEx::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}


	CEditSvrEx::CEditSvrEx(void):m_pGridReport(NULL),m_pEdit(NULL),m_ACol(NULL),m_ARow(NULL)
	{
	}
	CEditSvrEx::~CEditSvrEx(void)
	{
		for(int i=0;i<m_EditBuffer.size();i++)
		{
			if(m_EditBuffer[i]) delete m_EditBuffer[i];
		}
		m_EditBuffer.clear();
	}

	int CEditSvrEx::MoveLoc(int x,int y,int cx,int cy,int nCmdShow )
	{
		if(m_pEdit) return m_pEdit->MoveLoc(x,y,cx,cy,nCmdShow);
		return 1;
	}
	int CEditSvrEx::UpdateEditContent(int ACol,int ARow)
	{
		if(! m_pGridReport) return 0;
		//2017.8.23 LPCTSTR	pStrText = m_pGridReport->GetItemString(ARow, ACol + 1);
		LPCTSTR	pStrText = m_pGridReport->GetItemString(ARow, ACol);
		bool bDelete = false;
		//format
		{
			wchar_t* pDataType = 0;
			wchar_t* pEditFormat = 0;
			_variant_t var;
			xbXml xml;
			IXMLDOMElement * pElement = NULL;
			IXMLDOMNode * pNode;
			//PStrItem pItem = (PStrItem)m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );

			int startRow;
			CReportBandSvr::TReportBand *  pReportBand = &m_pGridReport->m_pDataTempl->m_ReportBand->m_ReportBand;
			startRow = pReportBand->Detail.startRow;
			PStrItem pItem = m_pGridReport->m_pDataTempl->m_pContentSvr->GetItemData(ACol,startRow);

			if(pItem && pItem->pStrXml)
			{
				xml.LoadXml(pItem->pStrXml);
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);

				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"celltype",&var);
				if(var.bstrVal)
					pDataType = _tcsdup(var.bstrVal);
				pElement->getAttribute(L"cellformat",&var);
				if(var.bstrVal)
					pEditFormat = _tcsdup(var.bstrVal);	
				pNode->Release();
				pElement->Release();

				if(pDataType && _tcscmp(pDataType,L"xs:decimal")==0)
				{
					if(pEditFormat && pEditFormat[0])
					{
						pStrText = XUtil::FormatDecimalString((LPTSTR)pStrText,pEditFormat,bDelete);
						int ret = m_pEdit->SetText((LPTSTR)pStrText);
						if(bDelete) delete pStrText;
						return ret;
					}
				}
				if(pEditFormat) delete pEditFormat;
				if(pDataType) delete pDataType;
			}

		}

		if(m_pEdit)
		{
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("button"))==0)
			{
				_variant_t var;
				xbXml xml;
				PStrItem pItem = (PStrItem)m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
				if(!pItem) return 0;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement = NULL;
				IXMLDOMNode * pNode;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pNode->Release();
				}
				if(pElement)
				{
					pElement->getAttribute(L"edittype",&var);
					if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,_T("checkbox"))==0)
					{
						if(pItem->pStrText)
							::SetWindowText(m_pEdit->GetHWND(),pItem->pStrText);
						else
							::SetWindowText(m_pEdit->GetHWND(),_T(""));
						pElement->getAttribute(L"editchecked",&var);
						if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,pStrText)==0)
							SendMessage(m_pEdit->GetHWND(),BM_SETCHECK,BST_CHECKED,0);
						pElement->Release();
						return 1;
					}
				}
			}
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("combobox"))==0)
			{
				::SendMessage(m_pEdit->GetHWND(),CB_RESETCONTENT ,0,0); 
				_variant_t var;
				int startRow,endRow;
				CReportBandSvr::TReportBand *  pReportBand = &m_pGridReport->m_pDataTempl->m_ReportBand->m_ReportBand;
				startRow = pReportBand->Detail.startRow;
				endRow = pReportBand->Detail.endRow - 1;

				LPTSTR pEditType = NULL;
				PStrItem pItem = m_pGridReport->m_pDataTempl->m_pContentSvr->GetItemData(ACol,startRow );
				if(!pItem) return 0;
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement;
				IXMLDOMNode * pNode;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pNode->Release();
				}
				if(pElement)pElement->getAttribute(L"ddlbcandedit",&var);
				if(var.bstrVal && _tcscmp(var.bstrVal, _T("Y"))==0)
				{
					::SendMessage(::GetWindow(m_pEdit->GetHWND(), GW_CHILD), EM_SETREADONLY, 0, 0);
				}
				else
				{
					::SendMessage(::GetWindow(m_pEdit->GetHWND(), GW_CHILD), EM_SETREADONLY, 1, 0);
				}
				xbXml ddlb;
				if(pElement)pElement->getAttribute(L"ddlbdataurl",&var);
				if(var.bstrVal && _tcscmp(var.bstrVal, _T(""))!=0)
				{
					//IXAServer::ExecXQuery(xfApp::GetApp()->m_pFrame->m_pXPage->m_pStrUrl,var.bstrVal,L"<args/>",ddlb);
					LPTSTR pItemStr = var.bstrVal;
					if(pItemStr[0]=='=')
					{
						CXGridComplier x;
						if(x.SetSource(pItemStr, this->m_pGridReport, 0, ARow, ACol))
							pItemStr = x.GetString();							
					}
					TCHAR buf[512];
					if(_tcsstr(pItemStr,_T("xquery:"))==pItemStr)
						::_stprintf_s(buf,512,_T("uri.hxsoft.com/xaserver/%s"),pItemStr);
					else
						::_stprintf_s(buf,512,_T("%s"),pItemStr);
					TCHAR WKURL[512];
					::_stprintf_s(WKURL,512,_T("<args><arg name='__wkurl' value='%s'/></args>"),pItemStr);
					if(xfUriLoader::FetchXml(xfApp::GetApp()->m_pFrame->m_pXPage->m_pStrUrl,buf,WKURL,ddlb)!=1) return -1;
				}else
				{
					if(pElement)pElement->getAttribute(L"ddlbxml",&var);
					if(var.bstrVal)ddlb.LoadXml(var.bstrVal);
				}
				if(var.bstrVal)
				{
					//xbXml ddlb;
					//ddlb.LoadXml(var.bstrVal);
					IXMLDOMNodeList *pList;
					IXMLDOMNode *pNodeEx;
					IXMLDOMNodeList *pList1;
					IXMLDOMNode *pNodeEx1;
					long nLen;
					//ddlb.m_pXmlDoc->selectNodes(L"//row/显示列",&pList);
					//ddlb.m_pXmlDoc->selectNodes(L"//row/数据列",&pList1);
					ddlb.m_pXmlDoc->selectNodes(L"/node()/node()/node()[position()=1]",&pList);
					ddlb.m_pXmlDoc->selectNodes(L"/node()/node()/node()[position()=2]",&pList1);
					pList->get_length(&nLen);
					int nIndex = -1;
					for(int i=0;i<nLen;i++)
					{
						pList->get_item(i,&pNodeEx);
						pList1->get_item(i,&pNodeEx1);
						BSTR bstr;
						LPTSTR bstr1;
						if(pNodeEx)
							pNodeEx->get_text(&bstr);
						else
							bstr = L"";
						if(pNodeEx1)
							pNodeEx1->get_text(&bstr1);
						else
							bstr1 = L"";
						if(bstr)
						{
							::SendMessage(m_pEdit->GetHWND(),CB_ADDSTRING,0,(LPARAM)bstr);
							::SysFreeString(bstr);
						}
						if(bstr1)
						{
							::SendMessage(m_pEdit->GetHWND(),CB_SETITEMDATA,i,(LPARAM)bstr1);
						}
						if(pStrText && bstr1 && (_tcscmp(pStrText, bstr1) == 0 || _tcscmp(pStrText, bstr) == 0))
							nIndex = i;
						pNodeEx->Release();
					}
					pList->Release();
					pList1->Release();
					pElement->Release();
					if(nIndex > -1)
						::SendMessage(m_pEdit->GetHWND(),CB_SETCURSEL,nIndex,0);
					else
						::SetWindowText(m_pEdit->GetHWND(),pStrText);
				}
				return 0;
			}
			bool bShowButton;
			bool bQuery = this->m_pGridReport->IsEditQuery(ACol,ARow,bShowButton);
			if(bQuery && m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("edit"))==0)
			{
				//2017.8.23 LPCTSTR pStrText2 = m_pGridReport->GetItemDisplayString(ARow, ACol + 1);
				 LPCTSTR pStrText2 = m_pGridReport->GetItemDisplayString(ARow, ACol);
				if(pStrText2) pStrText = pStrText2;
				
			}else
			{
				//2017.8.23 if(m_pGridReport->GetItemDisplayString(ARow, ACol + 1)) pStrText = m_pGridReport->GetItemDisplayString(ARow, ACol + 1);
				if(m_pGridReport->GetItemDisplayString(ARow, ACol)) pStrText = m_pGridReport->GetItemDisplayString(ARow, ACol);
			}
			return m_pEdit->SetText((LPTSTR)pStrText);
		}
		return 0;
	}
	int CEditSvrEx::UpdateCellContent(int ACol,int ARow)
	{
		if(! m_pGridReport) return 0;
		if(!m_pEdit) return 0;
		LPTSTR pStrText = m_pEdit->GetText();
		LPTSTR pStrText1 = m_pEdit->GetTextData();
		if(this->m_pGridReport->FOptions & goEditing)
		{ 
			PStrItem pItem = (PStrItem)m_pGridReport->GetGridDataSvr()->m_pContentSvr->GetItemData(ACol,ARow );
			if(pItem && pItem->pStrXml)
			{
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement = NULL;
				IXMLDOMNode * pNode;
				_variant_t var;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pNode->Release();
				}
				if(pElement)
				{
					pElement->getAttribute(L"edittype",&var);
					if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,_T("checkbox"))==0)
					{
						if(_tcsicmp(pStrText,_T("1"))==0 || _tcsicmp(pStrText,_T("Y"))==0)
						{
							pElement->getAttribute(L"editchecked",&var);
							pStrText1 = var.bstrVal;
						}
						else
						{
							pElement->getAttribute(L"uneditchecked",&var);
							pStrText1 = var.bstrVal;					
						}
						pStrText = NULL;
						if(pItem->pStrText)
							pStrText = pItem->pStrText;
						else
							pStrText = _T("");
					}
					else if(var.bstrVal && pStrText &&_tcsicmp(var.bstrVal,_T("ddlb"))==0)
					{
						if(!pStrText1 && pStrText)
						{
							int pIndex = ::SendMessage(m_pEdit->m_hWnd, CB_FINDSTRINGEXACT, 0, (LPARAM)pStrText);
							if(pIndex<0) pStrText1=pStrText;
							else
							{
								LRESULT strList = ::SendMessage(m_pEdit->m_hWnd, CB_GETITEMDATA, pIndex, NULL);
								pStrText1 = (LPTSTR)strList;
							}
						}
					}
				}
				//checkType
				//输入的数据 不包含选择的数据
				if(pStrText && !pStrText1)
				{
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pNode->Release();
					}
					if(pElement)
					{
						bool bFormatError = false;
						int nErrorId;
						LPWSTR pNextStr;
						pElement->getAttribute(L"celltype",&var);
						if(var.bstrVal && _tcsicmp(var.bstrVal,_T("xs:integer"))==0)
						{
							::wcstol(pStrText,&pNextStr,10);
							if(pNextStr && pNextStr[0]!='\0')
							{
								bFormatError=true;
								nErrorId = -1000;
							}
						}
						else if(var.bstrVal && _tcsicmp(var.bstrVal,_T("xs:decimal")) == 0)
						{
							::wcstod(pStrText,&pNextStr);
							if(pNextStr && pNextStr[0]!='\0')
							{
								bFormatError=true;
								nErrorId = -1001;
							}
						}
						else if(var.bstrVal && _tcsicmp(var.bstrVal,_T("xs:date")) == 0)
						{
							char * pStrText_1 = XUtil::strdupFromBstr(pStrText);
							int nYear,nMonth,nDay;
							int ndRet=sscanf(pStrText_1,"%d-%d-%d",&nYear,&nMonth,&nDay); 
							if(ndRet != 3) ndRet=sscanf(pStrText_1,"%d/%d/%d",&nYear,&nMonth,&nDay); 
							if(ndRet == 3)
							{
								if(nMonth<1 || nMonth>13) bFormatError=true;
								else if(nDay<1 || nDay>31) bFormatError=true;
								else if((nMonth==4||nMonth==6||nMonth==9||nMonth==11) && nDay>30) bFormatError=true;
								else if(nMonth==2){
									if(nDay>29) bFormatError=true;
									else if(((nYear%100==0 && nYear%400!=0)||nYear%4!=0) && nDay>28) bFormatError=true;
								}
							}
							else
								bFormatError=true;
							nErrorId = -1002;
						}
						if(bFormatError)
						{
							DWNMHDR hdr;
							memset(&hdr,0,sizeof(DWNMHDR));
							hdr.hwndFrom = m_pGridReport->GetHWND();
							hdr.idFrom = nErrorId;
							hdr.code = DWV_ITEMERROR;
							hdr.data = pStrText;
							hdr.col = ACol;
							hdr.row = ARow;
							if(pItem->pStrColumn)
								hdr.colname = pItem->pStrColumn;
							::SendMessage(::GetParent(m_pGridReport->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);
							return -1;
						}
					}
				}
			}
		}
		int ARow11 = m_pGridReport->GetTreeRow(ARow);
		if(pStrText1)
		{
			//2017.8.23 LPTSTR OriText = (LPTSTR)m_pGridReport->GetItemString(ARow, ACol + 1);
			LPTSTR OriText = (LPTSTR)m_pGridReport->GetItemString(ARow, ACol);
			if(OriText && _tcsicmp(pStrText1,OriText) == 0) return -1;
			if(!OriText & !pStrText1[0]) return -1;
			//if(!OriText | !OriText[0])
			if(m_pGridReport->event_ItemChanging(ARow,ACol,pStrText1)<0) return -1;
			//2017.8.23 ((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemString(ACol + 1,ARow11,pStrText1);
			((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemString(ACol,ARow11,pStrText1);
			//2017.8.23 ((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemDisplayString(ACol + 1,ARow11,pStrText);	
			((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemDisplayString(ACol,ARow11,pStrText);		
			m_pGridReport->event_ItemChanged(ARow,ACol,pStrText1,pStrText);
		}
		else
		{
			LPTSTR OriText = NULL;
			bool bButton;
			bool bQuery = m_pGridReport->IsEditQuery(ACol,ARow,bButton);
			if(bQuery)
			{
				if(!bButton) return -1;
				//2017.8.23 OriText = (LPTSTR)m_pGridReport->GetItemDisplayString(ARow, ACol + 1);
				OriText = (LPTSTR)m_pGridReport->GetItemDisplayString(ARow, ACol);
			}
			else
			{
				//2017.8.23 OriText = (LPTSTR)m_pGridReport->GetItemString(ARow, ACol + 1);
				OriText = (LPTSTR)m_pGridReport->GetItemString(ARow, ACol);
			}
			if((!OriText||!OriText[0]) && (!pStrText ||!pStrText[0])) return -1;
			if(OriText && pStrText && _tcsicmp(pStrText,OriText) == 0) return -1;
			if(m_pGridReport->event_ItemChanging(ARow,ACol,pStrText)<0) return -1;
			//2017.8.23 ((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemString(ACol + 1,ARow11,pStrText);
			((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemString(ACol,ARow11,pStrText);
			//2017.8.23
			//if((!pStrText || _tcsicmp(pStrText,_T(""))==0) && ((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->GetItemDisplayString(ACol + 1,ARow11))
			if((!pStrText || _tcsicmp(pStrText,_T(""))==0) && ((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->GetItemDisplayString(ACol,ARow11))
			{
				//2017.8.23 ((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemDisplayString(ACol + 1,ARow11,_T(""));
				((XGridDataSvrEx *)m_pGridReport->m_pSheetSvr->m_pDataSvr)->m_pContentSvr->SetItemDisplayString(ACol,ARow11,_T(""));
				m_pGridReport->event_ItemChanged(ARow,ACol,pStrText,_T(""));
			}
			else
			{
				m_pGridReport->event_ItemChanged(ARow,ACol,pStrText);
			}
		}

		if(pStrText1) delete pStrText1;
		if(pStrText) delete pStrText;

		return 1;
	}
	LPTSTR CEditSvrEx::GetEditContent()
	{
		if(m_pEdit)
		{
			LPTSTR	pStrText = NULL;
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("checkbox"))==0)
			{
				int startRow,endRow;
				CReportBandSvr::TReportBand *  pReportBand = &m_pGridReport->m_pDataTempl->m_ReportBand->m_ReportBand;
				startRow = pReportBand->Detail.startRow;
				endRow = pReportBand->Detail.endRow - 1;

				PStrItem pItem = m_pGridReport->m_pDataTempl->m_pContentSvr->GetItemData(m_ACol,startRow );

				_variant_t var;
				xbXml xml;
				xml.LoadXml(pItem->pStrXml);
				IXMLDOMElement * pElement = NULL;
				IXMLDOMNode * pNode;
				xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
					pNode->Release();
				}
				if(pElement)
				{
					if(SendMessage(m_pEdit->GetHWND(),BM_GETCHECK,0,0)==BST_CHECKED)
					{
						pElement->getAttribute(L"editchecked",&var);
						pElement->Release();
						return _tcsdup(var.bstrVal);
					}else
					{
						pElement->getAttribute(L"uneditchecked",&var);
						pElement->Release();
						return _tcsdup(var.bstrVal);
					}
				}
			}
			pStrText = m_pEdit->GetText();
			return pStrText;
		}
		return NULL;
	}
	CInplaceEditEx * CEditSvrEx::GetEditCtl(int ACol,int ARow)
	{
		CInplaceEditEx::eEditType	editType;
		LPTSTR pXClass = NULL;
		LPTSTR pEditType = NULL;

		if(!m_pGridReport) return NULL;
		if(m_pGridReport->m_pDataTempl)
		{
			int startRow,endRow;
			CReportBandSvr::TReportBand *  pReportBand = &m_pGridReport->m_pDataTempl->m_ReportBand->m_ReportBand;
			startRow = pReportBand->Detail.startRow;
			endRow = pReportBand->Detail.endRow - 1;

			PStrItem pItem = m_pGridReport->m_pDataTempl->m_pContentSvr->GetItemData(ACol,startRow );
			IXMLDOMElement * pElement = NULL;
			_variant_t var;
			if(pItem)
			{
				IXMLDOMNode * pNode;
				if(pItem->pStrXml)
				{	xbXml xml;
					xml.LoadXml(pItem->pStrXml);
					xml.m_pXmlDoc->selectSingleNode(_T("/cell/cellprop/edit"),&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pElement->getAttribute(L"edittype",&var);
						if(var.bstrVal)
							pEditType = _tcsdup(var.bstrVal);
						pNode->Release();
						if(pElement)pElement->Release();
					}
				}
			}
		}
		if(pEditType && _tcsicmp(pEditType,_T("datetimepick"))==0)
		{
			editType=CInplaceEditEx::_control;
			pXClass = L"xdatetimepick";
		}else if(pEditType && _tcsicmp(pEditType,_T("ddlb"))==0)
		{
			editType=CInplaceEditEx::_combobox;
		}else if(pEditType && _tcsicmp(pEditType,_T("checkbox"))==0)
		{
			editType=CInplaceEditEx::_checkbox;
		}else
			editType=CInplaceEditEx::_edit;
		
		if(pEditType)delete pEditType;

		for(int i=0;i< m_EditBuffer.size();i++)
		{
			if(m_EditBuffer[i]->editType == editType) 
				if(editType !=CInplaceEditEx::_control)
					return m_EditBuffer[i]->pEdit;
				else
					if(::_wcsicmp(m_EditBuffer[i]->pEdit->m_szCotrolClass,pXClass)==0)
						return m_EditBuffer[i]->pEdit;
		}

		CInplaceEditEx * pEdit;
		RECT	rc;
		::SetRect(&rc,0,0,0,0);
		switch(editType)
		{
		case CInplaceEditEx::_combobox:
			pEdit = new CCellComboBoxEditerEx();
			pEdit->m_dwStyle = WS_VISIBLE|WS_CHILD|CBS_DROPDOWN|CBS_AUTOHSCROLL|WS_VSCROLL|WS_HSCROLL;//WS_CHILD|CBS_DROPDOWN|CBS_AUTOHSCROLL;
			pEdit->m_pszWinClass =_tcsdup(_T("combobox"));
			pEdit->CreateControl(NULL,rc,(HWND)m_pGridReport->GetHWND(),0,xfApp::GetApp()->GetInstance());
			break;
		case CInplaceEditEx::_checkbox:
			{
				pEdit = new CCellCheckBoxEditerEx();
				pEdit->m_dwStyle = WS_CHILD|BS_AUTOCHECKBOX|BS_CHECKBOX|BS_VCENTER;
				pEdit->m_pszWinClass =_tcsdup(_T("button"));
				HWND hWnd = pEdit->CreateControl(NULL,rc,(HWND)m_pGridReport->GetHWND(),0,xfApp::GetApp()->GetInstance());
			}
			break;
		case CInplaceEditEx::_control:
			pEdit = new CControlEditerEx();
			((CControlEditerEx *)pEdit)->InstanceControl((HWND)m_pGridReport->GetHWND(),pXClass);
			break;
		default:
			pEdit = new  CCellEditerEx();
			pEdit->m_dwStyle = WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_AUTOVSCROLL|WS_BORDER|(m_pGridReport->GetEditUpperMode()?ES_UPPERCASE:0);
			pEdit->m_pszWinClass =_tcsdup(_T("edit"));
			pEdit->CreateControl(NULL,rc,(HWND)m_pGridReport->GetHWND(),0,xfApp::GetApp()->GetInstance());
		}
		LOGFONT	logFont;
		XUtil::Css::GetDefaultFont(logFont);
		HFONT	hFont =::CreateFontIndirect(&logFont);
		::SendMessage(pEdit->GetHWND(),WM_SETFONT,(WPARAM)hFont,false);	

		tagEditCtlBuff	* pBuffer = new tagEditCtlBuff();
		pBuffer->editType = editType;
		pBuffer->pEdit = pEdit;
		this->m_EditBuffer.push_back(pBuffer);
		pEdit->AttachProcEdit();
		pEdit->m_pEditSvr = this;
		return pEdit;
	}
	int CEditSvrEx::Draw(int ACol,int ARow,HDC hdc,RECT rect)
	{
		return 1;
	}

	int	CEditSvrEx::SelectCell(int ACol,int ARow)
	{
		//if(!m_pEdit) return -1;

		if(m_pEdit)
		{
			//this->UpdateCellContent(m_ARow,m_ACol);
			this->UpdateCellContent(m_ACol,m_ARow);
		}

		bool bQuery = false;
		bool bShowButton;
		bQuery = this->m_pGridReport->IsEditQuery(ACol,ARow,bShowButton);

		RECT rc;
		m_ARow = ARow;
		m_ACol = ACol;
		m_pEdit = this->GetEditCtl(ACol,ARow);

		DWNMHDR hdr;
		memset(&hdr,0,sizeof(DWNMHDR));
		hdr.hwndFrom = this->m_pGridReport->GetHWND();
		hdr.idFrom = (UINT_PTR)m_pEdit->GetHWND();
		hdr.code = DWV_ITEMBEINGEDIT;//DWV_CLICKED;
		hdr.DwTable = NULL;
		hdr.row = ARow;
		hdr.col = ACol;	
		if(!m_pGridReport->m_pDataTempl) return 1;
		PStrItem pItem = m_pGridReport->m_pDataTempl->m_pContentSvr->GetItemData(ACol,ARow );
		if(pItem && pItem->pStrColumn)
			hdr.colname = pItem->pStrColumn;
		if(pItem && pItem->pStrGroup)
			hdr.data = pItem->pStrGroup;
		::SendMessage(::GetParent(this->m_pGridReport->GetHWND()),WM_NOTIFY,0,(LPARAM)&hdr);

		UpdateEditContent(ACol,ARow);
		m_pGridReport->GetCellRect(ACol,ARow,rc);
		InflateRect(&rc,-1,-1);
		rc.top -= 1;
		if(bQuery)
		{
/*			HBITMAP hbp = NULL;
			TCHAR path[MAX_PATH];
			::GetModuleFileName(NULL,path,MAX_PATH);
			::PathRemoveFileSpec(path);
			TCHAR buf[MAX_PATH];
			::_stprintf_s(buf,MAX_PATH,_T("%s\\resource\\image\\query.bmp"),path);

			hbp = (HBITMAP)LoadImage(NULL,buf,IMAGE_BITMAP,16,16,LR_LOADFROMFILE|LR_VGACOLOR);
			HDC hdc = ::GetDC(this->m_pGridReport->GetHWND());
			HDC hMemDC = CreateCompatibleDC(hdc);
			hbp = (HBITMAP)::SelectObject(hMemDC,hbp);

			BitBlt(hdc,rc.right - 16,rc.top,16,16,hMemDC,0,0,SRCCOPY);

			hbp = (HBITMAP)::SelectObject(hMemDC,hbp);
			::DeleteObject(hbp);
			::DeleteDC(hMemDC);
*/
			rc.right -= 16;
		}
		MoveLoc(rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,SW_SHOW);
		if(m_pEdit)
		{
			//::SetFocus(m_pEdit->GetHWND());
			HWND hwnd = ::FindWindowEx(m_pEdit->GetHWND(), NULL,NULL,NULL);
			if(!hwnd) hwnd = m_pEdit->GetHWND();
			::SetFocus(hwnd);
		}
		return 1;
	}

	int CEditSvrEx::HiddenEditor()
	{
		if(m_pEdit)
		{
			int isVisible = ::IsWindowVisible(m_pEdit->GetHWND());
			if(isVisible != 0)
			{
				UpdateCellContent(m_ACol,m_ARow);
				if(m_pEdit) ::ShowWindow(m_pEdit->GetHWND(), SW_HIDE);
			}
			m_pEdit = NULL;
		}
		return 1;
	}

	int CEditSvrEx::ShowEditor(int ACol,int ARow)
	{
		if(m_pGridReport->FEditorMode)
		{
			if(m_pEdit)
			{
				HWND hwnd = ::FindWindowEx(m_pEdit->GetHWND(), NULL,NULL,NULL);
				if(hwnd) ::SendMessage(hwnd, WM_KILLFOCUS, NULL,NULL);
			}
			m_pGridReport->HideEditor();
		}
		m_pGridReport->FCurrent.x = ACol;
		m_pGridReport->FCurrent.y = ARow;
		m_ACol = ACol;
		m_ARow = ARow;
		m_pGridReport->ShowEditor();
		return 1;
	}
}}}}
