#include "StdAfx.h"
#include "XXmlSheet.h"
#include "xcControl.h"
#include <strstream>
using namespace std;
#include "Scintilla.h"

XXmlSheet::XXmlSheet(void)
{
}


XXmlSheet::~XXmlSheet(void)
{
}

int XXmlSheet::OnOpen()
{
	xfControl * pEdit = GetControl(L"content");
	if(pEdit)
	{
		SendMessage(pEdit->GetHWND(),SCI_AUTOCSETMAXHEIGHT,14,0);

	}
	return 1;
	
}

int XXmlSheet::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
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
		XSEdit * pControl = (XSEdit *)GetControl(_T("content"));
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
				_stprintf_s(ErrText,4096,_T("XXXml 错误:%s\r\n     代码: 0x%x\r\n     位置: 第 %i 行 第 %i 列\r\n     原因: %s"),ErrText,nErrCode,nLine,nLinePos,bstr);
				::SysFreeString(bstr);
				::SetWindowText(pOutPut->m_hWnd,ErrText);

				int nIndex = (int)SendMessage(GetControl(_T("content"))->m_hWnd,EM_LINEINDEX,nLine - 1,0);
				nIndex += nLinePos - 1;
				SendMessage(GetControl(_T("content"))->m_hWnd,EM_SETSEL,nIndex,nIndex);
				SendMessage(GetControl(_T("content"))->m_hWnd,EM_SCROLLCARET,0,0);
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

xfControl * XXmlSheet::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int XXmlSheet::OnCodeFind()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("content"))->GetInterface();
	pEdit->OnFindDlg();
	return 1;
}
int XXmlSheet::OnCodeReplace()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("content"))->GetInterface();
	pEdit->OnReplaceDlg();
	return 1;
}
