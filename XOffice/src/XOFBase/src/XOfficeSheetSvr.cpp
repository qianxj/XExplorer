#include "StdAfx.h"
#include "XOfficeSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeSheetSvr::XOfficeSheetSvr(void):m_pSheetCtrl(NULL),
		m_pStrTemplate(NULL),m_pStrDataUri(NULL),m_pStrXPath(NULL),
		m_pSerializeSvr(NULL),m_pDataSvr(NULL),m_bContentLoaded(false),m_pStrFile(NULL),m_state(goNormalState)
	{
		::SetRect(&FRect,0,0,0,0);
		//must create m_pSerializeSvr && m_pDataSvr 
	}

	XOfficeSheetSvr::~XOfficeSheetSvr(void)
	{
		if(m_pStrTemplate) delete m_pStrTemplate;
		if(m_pStrDataUri) delete m_pStrDataUri;
		if(m_pStrXPath)delete m_pStrXPath;
		if(m_pStrFile)delete m_pStrFile;

		//should destroy m_pSerializeSvr && m_pDataSvr 
	}

	int XOfficeSheetSvr::SetFRect(RECT rc)
	{
		FRect = rc;
		return 0;
	}
	
	int XOfficeSheetSvr::ShowSheet(bool bShow)
	{
		return 0;
	}

	int XOfficeSheetSvr::LoadTemplate()
	{
		return LoadTemplate(m_pStrTemplate,m_pStrXPath);
	}
	
	int XOfficeSheetSvr::LoadTemplate(xfNode * pNode)
	{
		if(pNode)
		{
			if(m_pStrTemplate) delete m_pStrTemplate;
			if(m_pStrDataUri) delete m_pStrDataUri;
			if(m_pStrXPath)delete m_pStrXPath;

			m_pStrTemplate = xbXml::GetStringAttribe(pNode->m_pElement,L"templatefile");
			if(m_pStrTemplate)
			{
				if(_tcsstr(m_pStrTemplate,_T("uri.hxsoft.com/xaserver"))==m_pStrTemplate)
				{
					int nLen = (int)_tcslen(m_pStrTemplate)+(int)_tcslen(_T("uriserver/"))+1;
					LPTSTR pStr = new TCHAR[nLen];
					_stprintf_s(pStr,nLen,_T("uriserver/%s"),m_pStrTemplate);
					if(m_pStrTemplate)delete m_pStrTemplate;
					m_pStrTemplate = pStr;
				}else if(_tcsstr(m_pStrTemplate,_T("dev:"))==m_pStrTemplate)
				{
					int nLen = (int)_tcslen(m_pStrTemplate)+(int)_tcslen(_T("uriserver/uri.hxsoft.com/xaserver/"))+1;
					LPTSTR pStr = new TCHAR[nLen];
					_stprintf_s(pStr,nLen,_T("uriserver/uri.hxsoft.com/xaserver/%s"),m_pStrTemplate);
					if(m_pStrTemplate)delete m_pStrTemplate;
					m_pStrTemplate = pStr;
				}
			}
			
			m_pStrXPath = xbXml::GetStringAttribe(pNode->m_pElement,L"templatexpath");
			m_pStrFile = xbXml::GetStringAttribe(pNode->m_pElement,L"src");
			m_pStrDataUri = xbXml::GetStringAttribe(pNode->m_pElement,L"uri");

			_variant_t val;
			pNode->m_pElement->getAttribute(L"state",&val);
			if(val.bstrVal)
			{
				if(_wcsicmp(val.bstrVal,L"run")==0)
				{
					this->m_state |= goRunState;
					this->m_state &= ~goDesignState;
				}
				else if(_wcsicmp(val.bstrVal,L"design")==0)
				{
					this->m_state |= goDesignState;
					this->m_state &= ~goRunState;
				}
				else
				{
					this->m_state &= ~goDesignState;
					this->m_state &= ~goRunState;
					this->m_state = goNormalState;
				}
			}else
			{
				this->m_state &= ~goDesignState;
				this->m_state &= ~goRunState;
				this->m_state = goNormalState;
			}

			if(m_pStrTemplate|| m_pStrFile)
			{
				xbXml xml;
				TCHAR pStrUrl[INTERNET_MAX_URL_LENGTH];
				DWORD dwSize=INTERNET_MAX_URL_LENGTH;
				TCHAR Path[MAX_PATH];
				if(m_pStrTemplate)
					xfUriLoader::UrlCombine(pNode->GetXPage()->m_pStrUrl,m_pStrTemplate,pStrUrl,&dwSize);
				else
					xfUriLoader::UrlCombine(pNode->GetXPage()->m_pStrUrl,m_pStrFile,pStrUrl,&dwSize);
				if(xfUriLoader::GetCacheFileName(pStrUrl,Path,MAX_PATH,true)==1)
					xml.Load(Path);
				else
					xml.Load(m_pStrTemplate);
				return LoadTemplate(xml.m_pXmlDoc);
			}else
			{
				IXMLDOMNodePtr pXmlNode;
				pNode->m_pElement->selectSingleNode(L"sheet",&pXmlNode);
				if(pXmlNode)
				{
					return LoadTemplate((IXMLDOMElementPtr)pXmlNode);
				}
			}

			//LoadTemplate();
		}
		return 1;
	}

	int XOfficeSheetSvr::LoadTemplate(LPTSTR pStrFile,LPTSTR pStrXPath)
	{
		if(m_pStrTemplate!=pStrFile)
			if(m_pStrTemplate) delete m_pStrTemplate;
		if(m_pStrXPath != pStrXPath)
			if(m_pStrXPath) delete m_pStrXPath;

		m_pStrTemplate = pStrFile;

		xbXml xml;
		TCHAR pStrUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize=INTERNET_MAX_URL_LENGTH;
		TCHAR Path[MAX_PATH];
		xfUriLoader::UrlCombine(m_pSheetCtrl->m_pxfNode->GetXPage()->m_pStrUrl,pStrFile,pStrUrl,&dwSize);
		if(xfUriLoader::GetCacheFileName(pStrUrl,Path,MAX_PATH,true)==1)
			xml.Load(Path);
		else
			xml.Load(m_pStrTemplate);

		return LoadTemplate(xml.m_pXmlDoc);
	}
	int XOfficeSheetSvr::LoadTemplate(IXMLDOMDocument * pXmlDoc)
	{
		IXMLDOMElement *	pElement=NULL;
		if(m_pStrXPath)
		{
			IXMLDOMNode * pNode;
			HRESULT hr = pXmlDoc->selectSingleNode(m_pStrXPath,&pNode);
			if(pNode)
			{
				pElement = xbXml::GetElement(pNode);
				pNode->Release();
			}
		}
		else
			pXmlDoc->get_documentElement(&pElement);
		if(pElement)
		{
			LoadTemplate(pElement,this->m_state);
			pElement->Release();
		}
		return 1;
	}

	int XOfficeSheetSvr::LoadTemplate(IXMLDOMElement * pElement,int state)
	{
		m_state = state;

		m_bContentLoaded = true;
		PreLoadTemplate(pElement,state);

		int nRet ;
		if(m_pSheetCtrl && m_pSheetCtrl->m_pxfNode)
			nRet = m_pSerializeSvr->LoadSheetData(m_pSheetCtrl->GetWin()->m_pXPage->m_pStrUrl, pElement,m_pDataSvr,state);
		else
			nRet = m_pSerializeSvr->LoadSheetData(NULL, pElement,m_pDataSvr,state);

		TemplateLoaded(pElement,state);
		return nRet;
	}

	int XOfficeSheetSvr::LoadTemplateByXml(LPTSTR pStrXml,LPTSTR pStrXPath)
	{
		if(m_pStrXPath != pStrXPath)
			if(m_pStrXPath) delete m_pStrXPath;
		m_pStrTemplate = NULL;
		m_pStrXPath = pStrXPath;
		xbXml xml;
		xml.LoadXml(pStrXml);

		return LoadTemplate(xml.m_pXmlDoc);
	}
	
	HWND XOfficeSheetSvr::GetHWND()
	{
		if(m_pSheetCtrl) return m_pSheetCtrl->m_hWnd;
		return NULL;
	}
	bool XOfficeSheetSvr::GetClientRect(RECT *pRect)
	{
		*pRect = FRect;
		return true;
	}
}}}
