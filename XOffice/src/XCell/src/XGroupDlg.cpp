#include "StdAfx.h"
#include "..\include\XGroupDlg.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGroupDlg::XGroupDlg(void):m_pPreLastStr(NULL),m_pPreStr(NULL),m_pNewPreLastStr(NULL),m_pNewPreStr(NULL),m_bOk(false)
	{

	}

	XGroupDlg::~XGroupDlg(void)
	{
		if(m_pNewPreLastStr)delete m_pNewPreLastStr;
		if(m_pNewPreStr)delete m_pNewPreStr;
	}
	
	int XGroupDlg::OnXCommand(LPCTSTR pStrID,xfControl *pControl)
	{
		if(pStrID && ::_tcsicmp(pStrID,_T("ok"))==0)
		{
			int nLen;
			nLen = ::GetWindowTextLength(this->GetControl(_T("prestr"))->m_hWnd);
			if(nLen > 0)
			{
				m_pNewPreStr = new TCHAR[nLen + 1];
				::GetWindowText(this->GetControl(_T("prestr"))->m_hWnd,m_pNewPreStr,nLen + 1);
			}

			nLen = ::GetWindowTextLength(this->GetControl(_T("prelaststr"))->m_hWnd);
			if(nLen > 0)
			{
				m_pNewPreLastStr = new TCHAR[nLen + 1];
				::GetWindowText(this->GetControl(_T("prelaststr"))->m_hWnd,m_pNewPreLastStr,nLen + 1);
			}
			m_dwStatus &= ~WST_RESPONSELOOP;
			m_bOk = true;
			return 1;
		}
		if(pStrID && ::_tcsicmp(pStrID,_T("cancel"))==0)
		{
			m_dwStatus &= ~WST_RESPONSELOOP;
			return 1;
		}
		return 0;
	}

	int XGroupDlg::OnOpen()
	{
		if(this->m_pPreStr)::SetWindowText(this->GetControl(_T("prestr"))->m_hWnd,m_pPreStr);
		if(this->m_pPreLastStr)::SetWindowText(this->GetControl(_T("prelaststr"))->m_hWnd,m_pPreLastStr);
		return 1;
	}
}}}}
