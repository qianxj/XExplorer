#include "stdafx.h"
#include "xreporteditsvr.hpp"
#include "xreportsheetsvr.hpp"


namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	LPTSTR	CCellEditer::GetText()
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
	int	CCellEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;

		//LPTSTR pStr = pStrContent
		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	int CCellEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}
	int CCellEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	CControlEditer::CControlEditer():m_pxfControl(NULL)
	{
	}

	int CControlEditer::InstanceControl(HWND hParent,LPTSTR xClass)
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

	CControlEditer::~CControlEditer()
	{
	}

	LPTSTR	CControlEditer::GetText()
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
	int	CControlEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;
		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	int CControlEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
	{
		if(GetHWND())
		{
			::MoveWindow(GetHWND(),x,y,cx,cy,false);
			::ShowWindow(GetHWND(),nCmdShow);
			return 1;
		}else
			return 0;
	}
	int CControlEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}

	LPTSTR	CCellComboBoxEditer::GetText()
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
	int	CCellComboBoxEditer::SetText(LPTSTR pStrContent)
	{
		if(!GetHWND()) return 0;
		
		//::SendMessage(GetHWND(),CB_RESETCONTENT ,0,0);  
		//::SendMessage(GetHWND(),CB_ADDSTRING ,0,(LPARAM)pStrContent);  
		::SetWindowText(GetHWND(),pStrContent);
		return 1;
	}
	int CCellComboBoxEditer::MoveLoc(int x,int y,int cx,int cy,int nCmdShow)
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

	int CCellComboBoxEditer::Draw(HDC hdc,RECT rect,LPTSTR pStrText)
	{
		return 1;
	}


	CEditSvr::CEditSvr(void):m_pOwner(NULL),m_pEdit(NULL),m_ACol(NULL),m_ARow(NULL),m_pGetCellContent(NULL),m_pSetCellContent(NULL),m_pGetEditType(NULL),m_pGetCellRect(NULL),
		m_pGetFont(NULL)
	{

	}
	CEditSvr::~CEditSvr(void)
	{
		for(int i=0;i<m_EditBuffer.size();i++)
		{
			if(m_EditBuffer[i]) delete m_EditBuffer[i];
		}
		m_EditBuffer.clear();
	}

	int CEditSvr::MoveLoc(int x,int y,int cx,int cy,int nCmdShow )
	{
		if(m_pEdit) return m_pEdit->MoveLoc(x,y,cx,cy,nCmdShow);
		return 1;
	}
	int CEditSvr::UpdateEditContent(int ACol,int ARow)
	{
		int nRet = 0;
		if(! m_pOwner) return 0;

		LPTSTR	pStrText = this->GetCellContent(ACol,ARow);
		if(m_pEdit)
		{
			if(m_pEdit->m_pszWinClass && _tcsicmp(m_pEdit->m_pszWinClass,_T("combobox"))==0)
			{
				::SendMessage(m_pEdit->GetHWND(),CB_RESETCONTENT ,0,0); 
				int VCol = ((XReportSheetSvr *)m_pOwner)->GetVColumn(ACol);
				LPCTSTR pCol = ((XReportSheetSvr *)m_pOwner)->GetColumnName(VCol);
				if(pCol) 
				{
					LPCTSTR pData = ((XReportSheetSvr *)m_pOwner)->GetDDLBData(pCol);
					if(pData)
					{
						vector<LPTSTR> vt;
						Hxsoft::XUtil::GetTokens( pData, L"||", vt);
						for(int i=0;i<(int)vt.size();i++)
						{
							::SendMessage(m_pEdit->GetHWND(),CB_ADDSTRING,0,(LPARAM)vt[i]);
						}
					}
				}
			}

			nRet = m_pEdit->SetText(pStrText);
		}
		//if(pStrText) delete pStrText;
		return nRet;
	}
	int CEditSvr::UpdateCellContent(int ACol,int ARow)
	{
		if(!m_pEdit) return 0;
		LPTSTR	pStrText = m_pEdit->GetText();

		this->SetCellContent(ACol,ARow,pStrText);
		if(pStrText)delete pStrText;

		return 1;
	}
	LPTSTR CEditSvr::GetEditContent()
	{
		if(m_pEdit)
		{
			LPTSTR	pStrText = m_pEdit->GetText();
			return pStrText;
		}
		return NULL;
	}
	CInplaceEdit * CEditSvr::GetEditCtl(int ACol,int ARow)
	{
		CInplaceEdit::eEditType	editType;
		LPTSTR pXClass = NULL;
		LPTSTR pEditType = this->GetEditType(ACol,ARow);

		if(pEditType && _tcsicmp(pEditType,_T("datetimepick"))==0)
		{
			editType=CInplaceEdit::_control;
			pXClass = L"xdatetimepick";
		}else if(pEditType && _tcsicmp(pEditType,_T("ddlb"))==0)
		{
			editType=CInplaceEdit::_combobox;
		}else
		{
			if(_tcsstr(pEditType,_T("editor:ddlb")))
			{
				editType=CInplaceEdit::_combobox;
			}else if(_tcsstr(pEditType,_T("editor:datetimepick")))
			{
				editType=CInplaceEdit::_control;
				pXClass = L"xdatetimepick";
			}else if(_tcsstr(pEditType,_T("editor:checkbox")))
			{
				editType=CInplaceEdit::_null;
			}else
				editType=CInplaceEdit::_edit;
		}
		
		if(pEditType)delete pEditType;

		for(int i=0;i< m_EditBuffer.size();i++)
		{
			if(m_EditBuffer[i]->editType == editType) 
				if(editType !=CInplaceEdit::_control)
					return m_EditBuffer[i]->pEdit;
				else
					if(::_wcsicmp(m_EditBuffer[i]->pEdit->m_szCotrolClass,pXClass)==0)
						return m_EditBuffer[i]->pEdit;
		}

		CInplaceEdit * pEdit;
		RECT	rc;
		::SetRect(&rc,0,0,0,0);
		switch(editType)
		{
		case CInplaceEdit::_combobox:
			{
				pEdit = new CCellComboBoxEditer();
				pEdit->m_dwStyle = WS_CHILD|CBS_DROPDOWN|CBS_AUTOHSCROLL|WS_VSCROLL|WS_HSCROLL;
				pEdit->m_pszWinClass =_tcsdup(_T("combobox"));
				HWND hWnd = pEdit->CreateControl(NULL,rc,(HWND)m_pOwner->GetHWND(),0,xfApp::GetApp()->GetInstance());
			}
			break;
		case CInplaceEdit::_control:
			pEdit = new CControlEditer();
			((CControlEditer *)pEdit)->InstanceControl((HWND)m_pOwner->GetHWND(),pXClass);
			break;
		default:
			pEdit = new  CCellEditer();
			pEdit->m_dwStyle = WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_AUTOVSCROLL;
			pEdit->m_pszWinClass =_tcsdup(_T("edit"));
			pEdit->CreateControl(NULL,rc,(HWND)m_pOwner->GetHWND(),0,xfApp::GetApp()->GetInstance());
		}

		if(pEdit && ::IsWindow(pEdit->GetHWND()))
		{
			HFONT hFont = this->m_pGetFont(m_pOwner,ACol,ARow);
			::SendMessage(pEdit->GetHWND(),WM_SETFONT,(LPARAM)hFont,false);
		}

		tagEditCtlBuff	* pBuffer = new tagEditCtlBuff();
		pBuffer->editType = editType;
		pBuffer->pEdit = pEdit;
		this->m_EditBuffer.push_back(pBuffer);
		return pEdit;
	}
	int CEditSvr::Draw(int ACol,int ARow,HDC hdc,RECT rect)
	{
		return 1;
	}

	int	CEditSvr::SelectCell(int ACol,int ARow)
	{
		if(m_pEdit)
		{
			this->UpdateCellContent(m_ACol,m_ARow);
		}

		bool bQuery=false;

		RECT rc;
		m_ARow = ARow;
		m_ACol = ACol;
		m_pEdit = this->GetEditCtl(ACol,ARow);

		UpdateEditContent(ACol,ARow);

		GetCellRect(ACol,ARow,rc);

		InflateRect(&rc,-1,-1);
		if(bQuery) rc.right -= 16;
		MoveLoc(rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,SW_SHOW);
		::SetFocus(m_pEdit->GetHWND());
		return 1;
	}

	int CEditSvr::HiddenEditor()
	{
		if(m_pEdit)
		{
			this->UpdateCellContent(m_ACol,m_ARow);
			::ShowWindow(m_pEdit->GetHWND(), SW_HIDE);
			m_pEdit = NULL;
		}
		return 1;
	}
	
	LPTSTR CEditSvr::GetCellContent(int ACol,int ARow)
	{
		if(m_pGetCellContent) return m_pGetCellContent(m_pOwner,ACol,ARow);
		return NULL;
	}
	int  CEditSvr::SetCellContent(int ACol,int ARow,LPTSTR pStr)
	{
		if(m_pSetCellContent) return m_pSetCellContent(m_pOwner,ACol,ARow,pStr);
		return 1;
	}
	LPTSTR  CEditSvr::GetEditType(int ACol,int ARow)
	{
		if(m_pGetEditType) return m_pGetEditType(m_pOwner,ACol,ARow);
		return NULL;
	}

	int  CEditSvr::GetCellRect(int ACol,int ARow,RECT &rt)
	{
		if(m_pGetCellRect) return m_pGetCellRect(m_pOwner,ACol,ARow,rt);
		return 1;
	}
}}}}