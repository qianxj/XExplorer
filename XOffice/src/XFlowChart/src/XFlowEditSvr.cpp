#include "stdafx.h"
#include "xfloweditsvr.hpp"
#include "XFlowChart.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
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
		
		::SendMessage(GetHWND(),CB_RESETCONTENT ,0,0);  
		::SendMessage(GetHWND(),CB_ADDSTRING ,0,(LPARAM)pStrContent);  
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


	CEditSvr::CEditSvr(void):m_pSheetSvr(NULL),m_pEdit(NULL),m_pObject(NULL)
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
	
	int CEditSvr::UpdateEditContent(XFlowChartObject * pObject)
	{
		if(! m_pObject) return 0;
		LPTSTR	pStrText = pObject->m_pStrCaption;
		if(m_pEdit) return m_pEdit->SetText(pStrText);
		return 0;
	}
	int CEditSvr::UpdateContent(XFlowChartObject * pObject)
	{
		if(! m_pSheetSvr) return 0;
		if(!m_pEdit) return 0;
		LPTSTR	pStrText = m_pEdit->GetText();

		if(pStrText)
		{
			if(pObject->m_pStrCaption)
				delete pObject->m_pStrCaption;
			pObject->m_pStrCaption = _tcsdup(pStrText);
		}

		delete pStrText;

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
	CInplaceEdit * CEditSvr::GetEditCtl(XFlowChartObject * pObject)
	{
		CInplaceEdit::eEditType	editType;
		LPTSTR pXClass = NULL;

		editType=CInplaceEdit::_edit;
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
			pEdit = new CCellComboBoxEditer();
			pEdit->m_dwStyle = WS_CHILD|CBS_DROPDOWN|CBS_AUTOHSCROLL|WS_VSCROLL|WS_HSCROLL;
			pEdit->m_pszWinClass =_tcsdup(_T("combobox"));
			pEdit->CreateControl(NULL,rc,m_pSheetSvr->GetHWND(),0,xfApp::GetApp()->GetInstance());
			break;
		case CInplaceEdit::_control:
			pEdit = new CControlEditer();
			((CControlEditer *)pEdit)->InstanceControl(m_pSheetSvr->GetHWND(),pXClass);
			break;
		default:
			pEdit = new  CCellEditer();
			pEdit->m_dwStyle = WS_CHILD|ES_AUTOHSCROLL|ES_MULTILINE|ES_AUTOVSCROLL | ES_NOHIDESEL ;
			pEdit->m_pszWinClass =_tcsdup(_T("edit"));
			pEdit->CreateControl(NULL,rc,m_pSheetSvr->GetHWND(),0,xfApp::GetApp()->GetInstance());
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

	int	CEditSvr::SelectObject(XFlowChartObject * pObject)
	{
		if(m_pEdit)
			this->UpdateContent(pObject);

		RECT rc;
		m_pObject = pObject;
		m_pEdit = this->GetEditCtl(pObject);
		UpdateEditContent(pObject);

		rc = pObject->FRect;
		InflateRect(&rc,-5,-5);
		MoveLoc(rc.left,rc.top,rc.right - rc.left,rc.bottom - rc.top,SW_SHOW);
		::SetFocus(m_pEdit->GetHWND());
		return 1;
	}

	int CEditSvr::HiddenEditor()
	{
		if(m_pEdit)
		{
			this->UpdateContent(m_pObject);
			::ShowWindow(m_pEdit->GetHWND(), SW_HIDE);
			m_pEdit = NULL;
		}
		return 1;
	}

}}}}
