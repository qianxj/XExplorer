#include "StdAfx.h"
#include "xdvSheet.h"
#include "xcControl.h"
#include <strstream>
using namespace std;
#include "Scintilla.h"

xdvSheet::xdvSheet(void):m_nSheet(0)
{
	AttachListon(L"viewtype",CBN_SELCHANGE,(FNEvent)&xdvSheet::EvtViewTypeSelection);
}


xdvSheet::~xdvSheet(void)
{
}

bool xdvSheet::EvtViewTypeSelection(TEvent * pEvent, LPARAM lParam)
{
	HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
	hCursor = ::SetCursor(hCursor);

	IXAServerArg arg;
	BSTR bstr;
	XSEdit * pControl = (XSEdit *)GetControl(_T("xcontent"));

	TCHAR buf[255];
	int nIndex = (int)::SendMessage(GetControl(L"viewtype")->GetHWND(),CB_GETCURSEL,0,0);
	if(nIndex < 0) return true;
	::SendMessage(GetControl(_T("viewtype"))->GetHWND(),CB_GETLBTEXT,nIndex,(LPARAM)buf);
	pControl->GetContent(&bstr);
	arg.AddArg(L"content",bstr);
	arg.AddArg(L"viewtype",buf);
	LPBYTE pData = NULL;
	int len;
	if(IXAServer::ExecXQuery(GetFrameWindow()->m_pXPage->m_pStrUrl,L"[dev.xdv.trans]",arg.GetString(),pData,len)==1)
	{
		xcSHtml* pHtml = (xcSHtml*)GetControl(_T("xcontentview"));
		//if(pHtml->m_pBlock)delete pHtml->m_pBlock;
		//pHtml->m_pBlock = pHtml->ParseXml((LPTSTR)(pData));
		pHtml->LoadHtml((LPCTSTR)(pData));
		if(pData) xfUriLoader::FreeData(pData);
	}
	::SysFreeString(bstr);

	hCursor = ::SetCursor(hCursor);
	DestroyCursor(hCursor);

	return true;
}

int xdvSheet::OnOpen()
{
	xfControl * pEdit = GetControl(L"xcontent");
	if(pEdit)
	{
		SendMessage(pEdit->GetHWND(),SCI_AUTOCSETMAXHEIGHT,14,0);

	}
	return 1;
	
}

int xdvSheet::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmFileSave"))==0)
	{
		m_pParentWin->OnXCommand(pStrID,pControl);
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

xfControl * xdvSheet::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int xdvSheet::PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame)
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
		BSTR bstr;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xcontent"));

		TCHAR buf[255];
		::GetWindowText(GetControl(_T("viewtype"))->GetHWND(),buf,255);
		pControl->GetContent(&bstr);
		arg.AddArg(L"content",bstr);
		arg.AddArg(L"viewtype",buf);
		LPBYTE pData = NULL;
		int len;
		if(IXAServer::ExecXQuery(GetFrameWindow()->m_pXPage->m_pStrUrl,L"[dev.xdv.trans]",arg.GetString(),pData,len)==1)
		{
			xcSHtml* pHtml = (xcSHtml*)GetControl(_T("xcontentview"));
			//if(pHtml->m_pBlock)delete pHtml->m_pBlock;
			//pHtml->m_pBlock = pHtml->ParseXml((LPTSTR)(pData));
			pHtml->LoadHtml((LPCTSTR)(pData));
			if(pData) xfUriLoader::FreeData(pData);
		}
		::SysFreeString(bstr);

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);

	}	
	return 1;
}

int xdvSheet::OnCodeFind()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcontent"))->GetInterface();
	pEdit->OnFindDlg();
	return 1;
}
int xdvSheet::OnCodeReplace()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcontent"))->GetInterface();
	pEdit->OnReplaceDlg();
	return 1;
}
