#include "StdAfx.h"
#include "EntityList.h"
#include "xcell.h"
#include "xofobject.h"

CEntityList::CEntityList(void):m_pEntityName(NULL),m_pStrAgent(NULL)

{
	this->AttachListon(WM_SETFOCUS,(FNEvent)&CEntityList::EvtSetFocus);
}

CEntityList::~CEntityList(void)
{
	this->ClearAllListonsTo();
	if(m_pEntityName)delete m_pEntityName;
	if(m_pStrAgent)delete m_pStrAgent;
}

bool CEntityList::EvtSetFocus(TEvent * pEvent, LPARAM lParam)
{
	if((HWND)pEvent->wParam != m_hWnd) SetAgent();
	return true;
}

int CEntityList::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && ::_tcsicmp(pStrID,_T("xmEntityNew"))==0)
	{
		//TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		//DWORD dwSize = INTERNET_MAX_URL_LENGTH;

		TEntityParam* pParam = new TEntityParam;
		::_tcscpy_s(pParam->pEntity,255,m_pEntityName);
		pParam->pKeyValue[0]='\0';

		LPTSTR pEntryGuid = _tcschr(m_pEntityName,'/')?_tcschr(m_pEntityName,'/') + 1:m_pEntityName; 
		int nIndex = 0;

		TCHAR url[255];
		_stprintf_s(url,255,_T("worknode:%s/new#%i"),((TEntityParam*)m_pParam)->pWorkNodeGuid,nIndex + 1);

		TCHAR strCmd[512];
		xbXml xml;
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/%s"),url);
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
		{
			IXMLDOMElement * pElementEx;
			xml.m_pXmlDoc->get_documentElement(&pElementEx);
			if(pElementEx)
			{
				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,_T("/business/xpage.xml"),strUrl,&dwSize,0);

				xfXPage * pXPage = new xfXPage();
				pXPage->m_pStrUrl = _tcsdup(strUrl);
				pXPage->m_pWkStrUrl = _tcsdup(strCmd);
				pXPage->Load(pElementEx);
				pElementEx->Release();

				_tcscpy_s(pParam->pWorkNodePath ,255,url);
				_tcscpy_s(pParam->pWorkNodeGuid ,48,((TEntityParam*)m_pParam)->pWorkNodeGuid);

				xfWin* pWin = xfWin::LoadXPageXml(pXPage,m_hWnd,xfApp::GetApp()->GetInstance(),pParam);
			}
		}
		return 1;

		//xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,_T("/business/entryview.xml"),strUrl,&dwSize,0);
		//xfWin::LoadXPage(NULL,strUrl,this->GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),pParam);

		//return 1;
	}

	if(pStrID && ::_tcsicmp(pStrID,_T("xmEntityOpen"))==0)
	{
		//TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		//DWORD dwSize = INTERNET_MAX_URL_LENGTH;

		TEntityParam* pParam = new TEntityParam;
		::_tcscpy_s(pParam->pEntity,255,m_pEntityName);

		XOffice::XCell::XGridSheetEx* pControl = (XOffice::XCell::XGridSheetEx*)GetControl(_T("xlist"));
		XOffice::XCell::XGridReport* pReport = ((XOffice::XCell::XGridSheetSvrEx*)pControl->m_pSheetSvr)->m_pGridReport;
		int row = pReport->GetRow();
		if(row < 1) return 1;
		LPCTSTR pStrNo = pReport->GetGridDataSvr()->m_pContentSvr->GetItemString(_T("guid"),row);
		if(!pStrNo) return 1;
		::_tcscpy_s(pParam->pKeyValue,255,pStrNo);

		int nIndex = 0;
		TCHAR url[255];
		_stprintf_s(url,255,_T("worknode:%s/open#%i"),((TEntityParam*)m_pParam)->pWorkNodeGuid,nIndex + 1);

		TCHAR strCmd[512];
		xbXml xml;
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/%s"),url);
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)==1)
		{
			IXMLDOMElement * pElementEx;
			xml.m_pXmlDoc->get_documentElement(&pElementEx);
			if(pElementEx)
			{
				TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,_T("/business/xpage.xml"),strUrl,&dwSize,0);

				xfXPage * pXPage = new xfXPage();
				pXPage->m_pStrUrl = _tcsdup(strUrl);
				pXPage->m_pWkStrUrl = _tcsdup(strCmd);
				pXPage->Load(pElementEx);
				pElementEx->Release();

				_tcscpy_s(pParam->pWorkNodePath ,255,url);
				_tcscpy_s(pParam->pWorkNodeGuid ,48,((TEntityParam*)m_pParam)->pWorkNodeGuid);

				xfWin* pWin = xfWin::LoadXPageXml(pXPage,m_hWnd,xfApp::GetApp()->GetInstance(),pParam);
			}
		}
		return 1;

		//xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,_T("/business/entryview.xml"),strUrl,&dwSize,0);
		//xfWin::LoadXPage(NULL,strUrl,this->GetFrameWindow()->m_hWnd,xfApp::GetApp()->GetInstance(),pParam);

		//return 1;
	}

	if(pStrID && ( ::_tcsicmp(pStrID,_T("condition-submit"))==0 || _tcsicmp(pStrID,_T("xmEntityRetrieve"))==0))
	{
		xbXml xml;
		xml.LoadXml(_T("<xquery xml=\"true\"/>"));
		IXMLDOMElementPtr m_pDocElement,m_pElement,m_pItemElement;
		xml.m_pXmlDoc->get_documentElement(&m_pDocElement);

		TCHAR strField[255];
		TCHAR strData[255];
		::GetWindowText(pControl->GetWin()->GetControl(_T("queryfield"))->m_hWnd,strField,255);
		::GetWindowText(pControl->GetWin()->GetControl(_T("querydata"))->m_hWnd,strData,255);

		if(strData[0]!='\0' && strField[0]!='\0')
		{
			xml.m_pXmlDoc->createElement(_T("condition"),&m_pElement);
			m_pDocElement->appendChild(m_pElement,NULL);

			xml.m_pXmlDoc->createElement(_T("item"),&m_pItemElement);
			m_pItemElement->setAttribute(L"name",(_variant_t)strField);
			m_pItemElement->setAttribute(L"op",(_variant_t)L"like");
			m_pItemElement->setAttribute(L"value",(_variant_t)strData);
			m_pElement->appendChild(m_pItemElement,NULL);
		}
		BSTR bstr = NULL;
		xml.m_pXmlDoc->get_xml(&bstr);
		
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
		Retrieve(bstr);
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		
		::SysFreeString(bstr);
		return 1;
	}

	if(pStrID && _tcscmp(pStrID,_T("xmFileClose"))==0)
	{
		xcLayerSheet * pLayerSheet = (xcLayerSheet *)this->GetControl(_T("mdilayer"));
		if(pLayerSheet->m_SheetSize>0)
		{
			HWND hWnd = (*pLayerSheet->m_pHwnds)[pLayerSheet->m_iCurSheet]; 
			pLayerSheet->m_pHwnds->erase(pLayerSheet->m_pHwnds->begin()+pLayerSheet->m_iCurSheet);
			pLayerSheet->RemoveSheet(pLayerSheet->m_iCurSheet);
			::DestroyWindow(hWnd);
		}
	}

	if(pStrID && _tcscmp(pStrID,_T("xmFileExit"))==0)
	{
		::PostQuitMessage(0);
		return 1;
	}

	return 0;
}

int CEntityList::OnOpen()
{
	if(!m_pEntityName)
	{
		_variant_t var;
		this->m_pXPage->m_pxfNode->m_pElement->getAttribute(L"entityguid",&var);
		if(var.bstrVal && var.bstrVal[0]) m_pEntityName = _tcsdup(var.bstrVal);

		XOffice::XCell::XGridSheetEx* pControl = (XOffice::XCell::XGridSheetEx*)GetControl(_T("xlist"));
		if(pControl)
		{
			IXMLDOMElement * pDocElement;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			xbXml xml;

			TCHAR cmdStr[512];
			_stprintf_s(cmdStr,512,_T("%s?ask=xdoc"),m_pXPage->m_pWkStrUrl);
			if(xfUriLoader::FetchXml(m_pXPage->m_pStrUrl,cmdStr,NULL,xml)==1)
			{
				xml.m_pXmlDoc->get_documentElement(&pDocElement);
				if(pDocElement)
				{
					pDocElement->selectSingleNode(L"*",&pNode);
					if(pNode)
					{
						pNode->QueryInterface(IID_IXMLDOMElement,(void **) &pElement);
						pNode->Release();

						pControl->m_pSheetSvr->LoadTemplate(pElement);
						pControl->m_pSheetSvr->SetFRect(pControl->m_pSheetSvr->FRect);

						Retrieve(NULL);
						::InvalidateRect(pControl->m_hWnd,NULL,true);

						pElement->Release();
					}
				}
				BSTR bstr;
				xml.m_pXmlDoc->get_xml(&bstr);
				::SysFreeString(bstr);
			}
		}
	}else
	{
		m_pEntityName = (LPTSTR)m_pParam;
	}
	if(m_pEntityName)SetAgent();

	return 1;
}

int CEntityList::Retrieve(LPTSTR  pXmlArgs)
{
	xfControl* pControl = this->GetControl(_T("xlist"));

	TCHAR Buf[MAX_PATH];
	_stprintf_s(Buf,MAX_PATH,_T("%s%s"),_T("uri.hxsoft.com/xaserver/entitysvr/querylist/"),m_pEntityName);
	XOffice::XCell::XGridDataSvrEx* pDataSvr = (XOffice::XCell::XGridDataSvrEx*)((XOffice::XCell::XGridSheetSvrEx *)((XOffice::XCell::XGridSheetEx *)pControl)->m_pSheetSvr)->m_pDataSvr;
	
	LPBYTE pData;
	int nLen;
	if(xfUriLoader::ProcessCmd(m_pXPage->m_pStrUrl,Buf,pXmlArgs,pData,nLen)!=1) return -1;
	pDataSvr->m_pContentSvr->LoadXml((LPTSTR)pData);
	xfUriLoader::FreeData(pData);
	::InvalidateRect(pControl->m_hWnd,NULL,false);
	
	return pDataSvr->m_pContentSvr->GetRowCount();
}


xfControl * CEntityList::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int CEntityList::SetAgent()
{
	if(!m_pStrAgent)
	{
		TCHAR Buf[MAX_PATH];
		_stprintf_s(Buf,MAX_PATH,_T("%s%s"),_T("uriserver/uri.hxsoft.com/xaserver/entitysvr/listagent/"),m_pEntityName);
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;

		xfUriLoader::UrlCombineW(this->m_pXPage->m_pStrUrl,Buf,strUrl,&dwSize,0);

		int len;
		LPBYTE pData;
		if(xfUriLoader::Load(strUrl,NULL,pData,len,false)==1)
		{
			LPBYTE pStr = new BYTE[len +sizeof(TCHAR)];
			::memcpy_s(pStr,len +sizeof(TCHAR),pData,len);
			pStr[len]=0;pStr[len + 1]=0;
			xfUriLoader::FreeData(pData);
			m_pStrAgent = (LPTSTR)pStr;
		}

	}

	xfNode* pNode = m_pXPage->GetAgentNode();
	xfControl * pControl = GetControl(_T("xlist"));
	if(pNode && m_pStrAgent && pNode->GetXPage()->m_pFocusNodeControl !=dynamic_cast<xfNodeControl *>(pControl->m_pxfNode))
	{
		pNode->GetXPage()->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(pControl->m_pxfNode);

		xbXml xml;
		xml.LoadXml(m_pStrAgent);
		IXMLDOMElement * pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			m_pXPage->SetAgentNodeContent(pNode,pElement,pControl->m_pxfNode);
			pElement->Release();
		}
		m_pXPage->m_pFocusNodeControl = dynamic_cast<xfNodeControl *>(pControl->m_pxfNode);
	}
	return 1;
}

