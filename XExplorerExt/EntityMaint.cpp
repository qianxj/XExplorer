#include "StdAfx.h"
#include "EntityMaint.h"
#include "xcell.h"

EntityMaint::EntityMaint(void):m_bstrData(NULL)
{
}

EntityMaint::~EntityMaint(void)
{
	TEntityParam *pParam = (TEntityParam *)this->m_pParam;
	//if(pParam)delete pParam;
	if(m_bstrData)::SysFreeString(m_bstrData);
}

int EntityMaint::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && ::_tcsicmp(pStrID,_T("xmEntitySave"))==0)
	{
		TEntityParam *pParam = (TEntityParam *)this->m_pParam;
		if(pParam && pParam->pEntity)
		{
			xbXml xml;

			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			XOffice::XOfficeFrm * pFrame = dynamic_cast<XOffice::XOfficeFrm *>(this->GetControl(_T("xoffice")));
			if(m_bstrData)
				xml.LoadXml(m_bstrData);
		
			for(int i=0;i<pFrame->m_pFrameSvr->m_Sheets.size();i++)
			{
				XOffice::XCell::XCellSheet * pSheet = dynamic_cast<XOffice::XCell::XCellSheet *>(pFrame->m_pFrameSvr->m_Sheets[i]);
				if(pSheet)
					((XOffice::XCell::XCellSheetSvr *)pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->DwUpdate(xml.m_pXmlDoc);
			}

			TCHAR pStrUrl[255];
			TEntityParam *pParam = (TEntityParam *)this->m_pParam;

			BSTR bstr;
			if(pParam->pKeyValue[0]=='\0')
			{
				IXMLDOMElement * pElement;
				_variant_t val;
				xml.m_pXmlDoc->get_documentElement(&pElement);
				if(pElement)
				{
					pElement->getAttribute(L"guid",&val);
					if(val.bstrVal)
						_tcscat_s(pParam->pKeyValue,_tcslen(val.bstrVal) + 1,val.bstrVal);
					pElement->Release();
				}
			}
			if(_tcsstr(pParam->pEntity,_T("sguids/"))== pParam->pEntity) 
			{
				IXMLDOMElement * pElement;
				xml.m_pXmlDoc->get_documentElement(&pElement);
				if(pElement)
				{
					pElement->setAttribute(L"guid",(_variant_t)pParam->pKeyValue);
				}
			}
			xml.m_pXmlDoc->get_xml(&bstr);


			int nStrLen = (int)_tcslen(pParam->pKeyValue) + 1 + (int)_tcslen(bstr) + 1 + 255;
			LPTSTR pBuf = new TCHAR[nStrLen];
			::ZeroMemory(pBuf,nStrLen * sizeof(TCHAR));
			_stprintf_s(pBuf,nStrLen,_T("<ID>%s</ID>%s"),pParam->pKeyValue,bstr);
			if(bstr)
			{
				if(m_bstrData)::SysFreeString(m_bstrData);
				m_bstrData = bstr;
			}

			_stprintf_s(pStrUrl,255,_T("uriserver/uri.hxsoft.com/xaserver/entitysvr/set/%s"),pParam->pEntity);
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH; 
			xfUriLoader::UrlCombine(this->m_pXPage->m_pStrUrl, pStrUrl,strUrl,&dwSize );

			BYTE * pData = NULL;
			int nLen;
			if(xfUriLoader::Load(strUrl,NULL,pBuf, pData,nLen)==1)
			{
				//ok
			}
			delete pBuf;
			if(pData)xfUriLoader::FreeData(pData);
			
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
		}

		return 1;
	}
	return 0;
}
int EntityMaint::OnOpen()
{
	TEntityParam *pParam;
	if(!m_pParam)
		m_pParam = (void *)new TEntityParam;

	pParam = (TEntityParam *)m_pParam;

	_variant_t var;
	this->m_pXPage->m_pxfNode->m_pElement->getAttribute(L"entityguid",&var);
	if(var.bstrVal && var.bstrVal[0])
		_tcscpy_s(pParam->pEntity ,255,var.bstrVal);
	else
		pParam->pEntity[0]='\0';
	if(_tcsstr(pParam->pEntity,_T("sguids/"))== pParam->pEntity) 
	{
		_tcscpy_s(pParam->pKeyValue ,255,_tcsstr(pParam->pEntity,_T("/")) + 1);
	}else
	{
		if(!pParam->pKeyValue[0])
		{
			this->m_pXPage->m_pxfNode->m_pElement->getAttribute(L"entityvalue",&var); 
			if(var.bstrVal && var.bstrVal[0])
				_tcscpy_s(pParam->pKeyValue ,255,var.bstrVal);
			else
				pParam->pKeyValue[0] = '\0'; 
		}
	}

	if(pParam && pParam->pEntity)
	{
		TCHAR Buf[MAX_PATH];
		xbXml xml;
		_stprintf_s(Buf,MAX_PATH,_T("%s%s %s"),_T("uri.hxsoft.com/xaserver/entitysvr/getdevitem/"),pParam->pEntity,_T("entity"));
		if(xfUriLoader::FetchXml(this->m_pXPage->m_pStrUrl,Buf,NULL,xml)!=1) return 1;
		
		long nLen;
		TCHAR strCmd[512];
		xfWin* pWin = NULL ;
		IXMLDOMNodeList*	pList;
		IXMLDOMNodeList*	pSheetList;
		IXMLDOMNode*		pNode;
		IXMLDOMElement*		pSheet;
		IXMLDOMElement*		pSchemaElement;
		_variant_t	var;
		std::wstring guids;
		std::vector<std::wstring> vguids;
		xml.m_pXmlDoc->selectNodes(_T("entry/edit/schema"),&pList);
		pList->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			pList->get_item(i,&pNode);
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSchemaElement);
			pNode->Release();

			std::wstring guids;
			pSchemaElement->selectNodes(_T("sheet"),&pSheetList);
			pSchemaElement->Release();
			long sheetLen;
			pSheetList->get_length(&sheetLen);
			for(int j=0;j<sheetLen;j++)
			{
				pSheetList->get_item(j,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSheet);
				pNode->Release();
				pSheet->getAttribute(L"sheetguid",&var);
				pSheet->Release();
				if(var.bstrVal)
					guids += var.bstrVal;
			}
			if(guids !=_T(""))
				vguids.push_back(guids);
			pSheetList->Release();
		}
		pList->Release();
		
		if(vguids.size() < 1) return 1;
		int nIndex = 0;

		if(nIndex < 0)nIndex = 0;
		if(nIndex >= 	(int)vguids.size()) return 1;
		::_stprintf_s(strCmd,512, _T("uri.hxsoft.com/xaserver/developer getdocbysheet %s"),	vguids[nIndex].c_str());
		if(xfUriLoader::FetchXml(GetFrameWindow()->m_pXPage->m_pStrUrl,strCmd,NULL,xml)!=1) return 1;

		pWin = this;

		XOffice::XOfficeFrm * pControl = (XOffice::XOfficeFrm *)pWin->GetControl(_T("xoffice"));
		if(pControl)
		{
			IXMLDOMElement * pDocElement;
			xml.m_pXmlDoc->get_documentElement(&pDocElement);
			if(pDocElement)
			{
				pControl->m_pFrameSvr->m_pSerializeSvr->LoadSheetData(NULL,pDocElement,pControl->m_pFrameSvr->m_pDataSvr);
				pControl->m_pFrameSvr->SelectSheet(0);
				pControl->m_pFrameSvr->SetFRect(pControl->m_pFrameSvr->FRect);
				::InvalidateRect(pControl->m_hWnd,NULL,true);
				pDocElement->Release();
			}
		}
	}

	if(pParam && pParam->pEntity && pParam->pKeyValue[0] !='\0')
	{
		xbXml xml;
		TCHAR pStrUrl[255];
		_stprintf_s(pStrUrl,255,_T("uri.hxsoft.com/xaserver/entitysvr/get/%s"),pParam->pEntity);
		if(xfUriLoader::FetchXml(m_pXPage->m_pStrUrl,pStrUrl,pParam->pKeyValue,xml)!=1) return 1;

		BSTR bstr;
		xml.m_pXmlDoc->get_xml(&bstr);
		if(bstr)
		{
			XOffice::XOfficeFrm * pFrame = dynamic_cast<XOffice::XOfficeFrm *>(this->GetControl(_T("xoffice")));

			for(int i=0;i<pFrame->m_pFrameSvr->m_Sheets.size();i++)
			{
				XOffice::XCell::XCellSheet * pSheet = dynamic_cast<XOffice::XCell::XCellSheet *>(pFrame->m_pFrameSvr->m_Sheets[i]);
				if(pSheet)
				{
					if(!pSheet->m_pSheetSvr->m_bContentLoaded)
					{
						pSheet->m_pSheetSvr->LoadTemplate(pFrame->m_pFrameSvr->m_SheetsElement[i],pFrame->m_pFrameSvr->m_state);
						pSheet->m_pSheetSvr->m_bContentLoaded = true;
					}
					((XOffice::XCell::XCellSheetSvr *)pSheet->m_pSheetSvr)->m_pGridFrameSvrs->GetGridDataSvr()->RetrieveXmlData(bstr);

				}
			}
			if(m_bstrData)::SysFreeString(m_bstrData);
			m_bstrData = bstr;
		}
	}
	return 1;
}

