#include "StdAfx.h"
#include "XModeSheet.h"
#include "xcControl.h"
#include <strstream>
using namespace std;
#include "Scintilla.h"

XModeSheet::XModeSheet(void):m_nSheet(0)
{
}


XModeSheet::~XModeSheet(void)
{
}

int XModeSheet::OnOpen()
{
	xfControl * pEdit = GetControl(L"xcontent");
	if(pEdit)
	{
		SendMessage(pEdit->GetHWND(),SCI_AUTOCSETMAXHEIGHT,14,0);

	}
	return 1;
	
}

int XModeSheet::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmFileSave"))==0)
	{
		m_pParentWin->OnXCommand(pStrID,pControl);
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmValid"))==0)
	{
		xbXml xml;
		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xcontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;

		xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
		::SetWindowText(pOutPut->m_hWnd,_T(""));
		if(!xml.LoadXml(pXml))
		{
			IXMLDOMParseError* pError;
			xml.m_pXmlDoc->get_parseError(&pError);
			if(pError)
			{
				TCHAR ErrText[4096]={0};

				long nLine;
				long nLinePos;
				long nErrCode;
				pError->get_line(&nLine);
				pError->get_linepos(&nLinePos);
				pError->get_errorCode(&nErrCode);

				BSTR bstr;
				pError->get_reason(&bstr);
				_stprintf_s(ErrText,4096,_T("XXMode 错误:%s\r\n     代码: 0x%x\r\n     位置: 第 %i 行 第 %i 列\r\n     原因: %s"),ErrText,nErrCode,nLine,nLinePos,bstr);
				::SysFreeString(bstr);
				::SetWindowText(pOutPut->m_hWnd,ErrText);

				int nIndex = (int)SendMessage(GetControl(_T("xcontent"))->m_hWnd,EM_LINEINDEX,nLine - 1,0);
				nIndex += nLinePos - 1;
				SendMessage(GetControl(_T("xcontent"))->m_hWnd,EM_SETSEL,nIndex,nIndex);
				SendMessage(GetControl(_T("xcontent"))->m_hWnd,EM_SCROLLCARET,0,0);
			}
		}
		::SysFreeString(pXml);
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmEditFind"))==0)
	{

		OnCodeFind();
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmEditReplace"))==0)
	{
		OnCodeReplace();
		return 1;
	}
	return 0;
}

xfControl * XModeSheet::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int XModeSheet::PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame)
{
	if(_tcsicmp(pStrLayerSheet,_T("sheet1"))==0)
		m_nSheet = 0;
	else if(_tcsicmp(pStrLayerSheet,_T("sheet2"))==0)
		m_nSheet = 1;

	if(m_nSheet == 1)
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		IXAServerArg arg;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xcontent"));

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
	}else if(m_nSheet==2)
	{

	}
	return 1;
}

int XModeSheet::OnCodeFind()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcontent"))->GetInterface();
	pEdit->OnFindDlg();
	return 1;
}
int XModeSheet::OnCodeReplace()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcontent"))->GetInterface();
	pEdit->OnReplaceDlg();
	return 1;
}
