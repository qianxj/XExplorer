#include "StdAfx.h"
#include "DevXQuery.h"
#include "xcell.h"

DevXQuery::DevXQuery(void):m_pElementEx(NULL)

{
	SetEventListons();
}

DevXQuery::~DevXQuery(void)
{
	if(m_pElementEx)m_pElementEx->Release();
}

bool DevXQuery::SetEventListons()
{
	this->AttachListon(_T("datasource"),CBN_SELENDOK,(FNEvent)&DevXQuery::EvtDatasourceSelected);
	return true;
}
bool DevXQuery::UnSetEventListons()
{
	this->ClearAllListonsTo();
	return true;
}

bool DevXQuery::EvtDatasourceSelected(TEvent *pEvent ,LPARAM lParam)
{

	xfControl * pControl = this->GetControl(_T("datasource"));

	IXMLDOMNodeList * pList;
	IXMLDOMNode * pNode;
	IXMLDOMElement * pElement;
	long nLen;
	_variant_t frame;

	pControl->m_pxfNode->m_pElement->getAttribute(L"layerframe",&frame);
	pControl->m_pxfNode->m_pElement->selectNodes(L"item",&pList);
	pList->get_length(&nLen);
	int nIndex = (int)::SendMessage(pControl->m_hWnd,CB_GETCURSEL,0,0);
	if(nIndex >=0 && nLen > nIndex)
	{
		pList->get_item(nIndex,&pNode);
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
		_variant_t sheet;
		pElement->getAttribute(L"layersheet",&sheet);
		if(sheet.bstrVal && frame.bstrVal)
		{
			pControl->SwitchLayer(sheet.bstrVal,frame.bstrVal);
		}
		pElement->Release();
		pNode->Release();
	}
	pList->Release();

	return true;
}



bool DevXQuery::GetClientRect(RECT *pRect)
{
	::GetClientRect(m_hWnd,pRect);
	return true;
}
HWND DevXQuery::GetHWND()
{
	return m_hWnd;
}

int DevXQuery::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(!pStrID) return 1;
	if(_tcsicmp(pStrID,_T("argadd"))==0)
	{
		XOffice::XCell::XGridSheetEx * pControl = (XOffice::XCell::XGridSheetEx *)GetControl(_T("parasgrid"));
		XOffice::XCell::IGridReport * pReport = (XOffice::XCell::IGridReport *)pControl->GetInterface();
		pReport->InsertRow(0);
		return 1;
	}
	else if(_tcsicmp(pStrID,_T("argdelete"))==0)
	{
		XOffice::XCell::XGridSheetEx * pControl = (XOffice::XCell::XGridSheetEx *)GetControl(_T("parasgrid"));
		XOffice::XCell::IGridReport * pReport = (XOffice::XCell::IGridReport *)pControl->GetInterface();
		int nRow = pReport->GetRow();
		if(nRow < 1) return 1;
		pReport->DeleteRow(nRow);

		return 1;
	}
	else if(_tcsicmp(pStrID,_T("xmShowDetail"))==0)
	{
		xfNode* pNodeSheet = this->m_pXPage->m_pxfNode->GetNodeByName(L"detailArea");
		if(pNodeSheet->m_LayerFlow & LayerFlow_UnLayer)
		{
			pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow & ~LayerFlow_UnLayer;
			pNodeSheet->ShowControl(true);
		}else
		{
			pNodeSheet->m_LayerFlow  = pNodeSheet->m_LayerFlow  | LayerFlow_UnLayer;
			pNodeSheet->ShowControl(false);
		}
		m_pXPage->m_pxfNode->AdjustRect();
		m_pXPage->m_pxfNode->AdjustCtrlRect();
		::InvalidateRect(m_hWnd,&m_pXPage->m_pxfNode->m_AreaRect,false);
	}
	else if(pStrID && _tcsicmp(pStrID,_T("xmEditFind"))==0)
	{
		OnCodeFind();
		return 1;
	}

	else if(pStrID && _tcsicmp(pStrID,_T("xmEditReplace"))==0)
	{
		OnCodeReplace();
		return 1;
	}
	else if(_tcsicmp(pStrID,_T("xmFileSave"))==0)
	{
		UpdateContent();
		this->m_pParentWin->OnXCommand(_T("xmFileSave"),pControl);
		return 1;
	}
	else if(_tcsicmp(pStrID,_T("xmFilePreview"))==0)
	{
		//UpdateContent();
		//this->m_pParentWin->OnXCommand(_T("xmFileSave"),pControl);
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
		
		_variant_t	var;
		TCHAR url[255];

		m_pElementEx->getAttribute(L"guid",&var);
		if(!var.bstrVal || !var.bstrVal[0]) return 1;

		_stprintf_s(url,255,_T("xquery:%s"),var.bstrVal);
		if(_tcsicmp(pControl->m_pxfNode->m_pCaption,_T("Ö´ÐÐ"))==0)
			_stprintf_s(url,255,_T("xaction:%s"),var.bstrVal);

		TCHAR strCmd[512];
		::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/%s"),url);
		
		LPBYTE pData;
		LPTSTR pStr = NULL;
		bool bDelete = false;;
		int nLen;
		if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,pData,nLen)==1 && pData)
		{
			if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
			{
				pStr = (LPTSTR)(pData + 2);
			}else if((pData[0]==0xEF &&  pData[1]==0xBB && pData[2]==0xBF))
			{
				int _convert = MultiByteToWideChar(CP_UTF8, 0, (LPSTR)pData+3, nLen, NULL, 0);
				TCHAR * psz = new TCHAR[_convert + 1];
				int nRet = MultiByteToWideChar(CP_UTF8, 0,(LPSTR)pData+3, nLen, psz, _convert);
				psz[_convert]='\0';
				pStr = psz;
				bDelete = true;
			}
			else if(pData[0] !=0 && pData[1]!=0) //ansi should gb2312
			{
				int _convert = MultiByteToWideChar(CP_ACP, 0, (LPSTR)pData, nLen, NULL, 0);
				TCHAR * psz = new TCHAR[_convert + 1];
				int nRet = MultiByteToWideChar(CP_ACP, 0,(LPSTR)pData, nLen, psz, _convert);
				psz[_convert]='\0';
				pStr = psz;
				bDelete = true;
			}else //should utf-16
			{
				pStr = (LPTSTR)pData;
			}
			XSEdit * pControl = (XSEdit *)GetControl(_T("resultcontent"));
			if(pStr)pControl->LoadContent(pStr,L".sql");
			pControl->SwitchLayer(_T("sheet2"),_T("frameex"));
			if(bDelete)delete pStr;
			if(pData)xfUriLoader::FreeData(pData);
		
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);

			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,(LPTSTR)"");

			return 1;
		}else if(pData)
		{
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,(LPTSTR)pData);
		}
		if(pData)
		{
			MessageBox(m_hWnd,(LPTSTR)pData,_T("ÌáÊ¾"),MB_OK);
			if(pData)xfUriLoader::FreeData(pData);
		}

		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}
	return 1;
}

int DevXQuery::UpdateContent()
{
	TCHAR strSource[80];
	TCHAR strLink[80];
	BSTR pStrContent = NULL;


	strLink[0] ='\0';
	strSource[0] = '\0';

	::GetWindowText(GetControl(_T("datasource"))->m_hWnd,(LPWSTR)strSource,80);
	::GetWindowText(GetControl(_T("linkname"))->m_hWnd,(LPWSTR)strLink,80);
	/*
	nLenContent = ::GetWindowTextLength(GetControl(_T("sqlcontent"))->m_hWnd);
	if(nLenContent > 0)
	{
		pStrContent = new TCHAR[nLenContent + 1];
		::GetWindowText(GetControl(_T("sqlcontent"))->m_hWnd,pStrContent,nLenContent + 1);
	}*/
	XSEdit * pControl = (XSEdit *)GetControl(_T("sqlcontent"));
	pControl->GetContent(&pStrContent);

	XOffice::XCell::XGridSheetEx * pControlEx = (XOffice::XCell::XGridSheetEx*)GetControl(_T("parasgrid"));
	BSTR bXml=NULL;
	if(pControl)
	{
		XOffice::XCell::XGridDataSvrEx * pDataSvr =  (XOffice::XCell::XGridDataSvrEx *)pControlEx->m_pSheetSvr->m_pDataSvr;
		pDataSvr->m_pContentSvr->GetContent(bXml);
	}
		
	_variant_t var;
	IXMLDOMDocument * pDoc;
	IXMLDOMNode * pNode;
	IXMLDOMNode * pNodeItem;
	IXMLDOMElement * pElementEx;
	IXMLDOMElement * pElement;

	m_pElementEx->get_ownerDocument(&pDoc);
	if(bXml)
	{
		IXMLDOMNode * pNodeEx;
		m_pElementEx->selectSingleNode(L"paras",&pNodeEx);
		if(pNodeEx)
		{
			m_pElementEx->removeChild(pNodeEx,NULL);
			pNodeEx->Release();
		}

		xbXml xPara;
		xPara.LoadXml(bXml);
		IXMLDOMElement * pEle;
		xPara.m_pXmlDoc->get_documentElement(&pEle);
		if(pEle)
		{

			m_pElementEx->appendChild(pEle,NULL);

			pEle->Release();
		}
		::SysFreeString(bXml);
	}

	m_pElementEx->selectSingleNode(L"datasource",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
		pNode->Release();
	}else
	{
		pDoc->createElement(L"datasource",&pElement);
		m_pElementEx->appendChild(pElement,NULL);
	}
	pElement->setAttribute(L"link",(_variant_t)strLink);
	pElement->setAttribute(L"source",(_variant_t)strSource);
	pElement->selectSingleNode(L"sql",&pNodeItem);
	if(!pNodeItem)
	{
		pDoc->createElement(L"sql",&pElementEx);
		pElement->appendChild(pElementEx,NULL);
	}else
	{
		pNodeItem->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEx);
	}

	if(_tcschr(pStrContent,'\r'))
	{
		pElementEx->put_text(NULL);
		IXMLDOMCDATASection * pCDData;
		pDoc->createCDATASection(pStrContent,&pCDData);
		pElementEx->appendChild(pCDData,NULL);
		pCDData->Release();
	}else
		pElementEx->put_text(pStrContent);
	pElementEx->Release();
	pElement->Release();
	pDoc->Release();

	if(pStrContent)::SysFreeString(pStrContent);

	return 1;
}

int DevXQuery::LoadContent(IXMLDOMElement * pElement)
{
	if(m_pElementEx)m_pElementEx->Release();
	m_pElementEx = pElement;
	m_pElementEx->AddRef();

	_variant_t var;

	IXMLDOMNode * pNode;
	IXMLDOMNode * pNodeItem;
	IXMLDOMElement * pElementEx;
	m_pElementEx->selectSingleNode(L"datasource",&pNode);
	if(pNode)
	{
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEx);
		pElementEx->getAttribute(L"source",&var);
		if(var.bstrVal)
		{
			::SetWindowText(GetControl(_T("datasource"))->m_hWnd,var.bstrVal);
			if(_tcsicmp(var.bstrVal,_T("XSQL"))==0 || _tcsicmp(var.bstrVal,_T("XQUERY"))==0 )
			{
				pNode->selectSingleNode(L"sql",&pNodeItem); 
				if(pNodeItem)
				{
					BSTR bstr;

					IXMLDOMText* pText;
					IXMLDOMNode* pNodeText;
					pNodeItem->selectSingleNode(L".//text()",(IXMLDOMNode **)&pNodeText);
					if(pNodeText)
					{
						pNodeText->QueryInterface(IID_IXMLDOMText,(void **)&pText);
						pNodeText->Release();
						pText->get_data(&bstr);
						pText->Release();
					}else
						pNodeItem->get_text(&bstr);
					
					XSEdit * pControl = (XSEdit *)GetControl(_T("sqlcontent"));
					if(bstr)
					{
						pControl->LoadContent(bstr,L".sql");
						//::SetWindowText(GetControl(_T("sqlcontent"))->m_hWnd,bstr);
						::SysFreeString(bstr);
					}else
					{	
						bstr =::SysAllocString(L"");
						pControl->LoadContent(bstr,L".sql");
						::SysFreeString(bstr);
					}

					
					pNodeItem->Release();
				}
				pElementEx->getAttribute(L"link",&var);
				if(var.bstrVal)
					::SetWindowText(GetControl(_T("linkname"))->m_hWnd,var.bstrVal);
			}
		}
		pElementEx->Release();
		pNode->Release();
	}

	m_pElementEx->selectSingleNode(L"paras",&pNodeItem);
	if(pNodeItem)
	{
		pNodeItem->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
		pNodeItem->Release();

		XOffice::XCell::XGridSheetEx * pControl = (XOffice::XCell::XGridSheetEx*)GetControl(_T("parasgrid"));
		if(pControl)
		{
			XOffice::XCell::XGridDataSvrEx * pDataSvr =  (XOffice::XCell::XGridDataSvrEx *)pControl->m_pSheetSvr->m_pDataSvr;
			pDataSvr->m_pContentSvr->Load(pElement);
			::InvalidateRect(pControl->m_hWnd,NULL,false);
		}
		pElement->Release();
	}
	return 1;
}

int  DevXQuery::OnCodeFind()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("sqlcontent"))->GetInterface();
	pEdit->OnFindDlg();
	return 1;
}

int  DevXQuery::OnCodeReplace()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("sqlcontent"))->GetInterface();
	pEdit->OnReplaceDlg();
	 return 1;
}