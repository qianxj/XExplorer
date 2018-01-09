#include "StdAfx.h"
#include "xfnode.hpp"
#include "xfnodebox.hpp"
#include "xfnodewin.hpp"
#include "xfNodeXPage.hpp"
#include "xfuriLoader.hpp"
#include "xfXPage.hpp"
#include <process.h>
#include "xfApp.hpp"
#include "xfConnect.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNodeXPage::xfNodeXPage(void):m_pStrXPage(NULL),m_pStrUri(NULL),m_pStrXsl(NULL)
	{
		m_LayerFlow = LayerFlow_NS;
	}

	xfNodeXPage::~xfNodeXPage(void)
	{
		if(m_pStrXPage) delete m_pStrXPage;
		if(m_pStrUri)delete m_pStrUri;
		if(m_pStrXsl)delete m_pStrXsl;

	}
	
	struct ParseNodeInfo
	{
		xfNodeXPage * pThis;
		IXMLDOMElement* pNodeEx;
		bool bParseChild;
	};

	unsigned int __stdcall xfNodeXPage::DoParseNode(void *pV)
	{
		:: CoInitialize(NULL);
		ParseNodeInfo * pParseNodeInfo = (ParseNodeInfo*)pV;
		xfNodeXPage * pThis = pParseNodeInfo->pThis;
		IXMLDOMElement* pNodeEx = pParseNodeInfo->pNodeEx;
		bool ParseChild = pParseNodeInfo->bParseChild; 

		bool reload = 
			(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
		if(!reload) return 0;

		pThis->m_pStrXPage = xbXml::GetStringAttribe(pNodeEx,_T("src"));
		pThis->m_pStrUri = xbXml::GetStringAttribe(pNodeEx,_T("uri"));
		pThis->m_pStrXsl = xbXml::GetStringAttribe(pNodeEx,_T("xsl"));

		int Result = pThis->SuperParseNode(pNodeEx,false);
		if(Result>=0 && pThis->m_pStrXPage )
		{
			xbXml* pxbXml = new xbXml();
			BYTE * pData = NULL;
			int len;
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH; 
			TCHAR Path[MAX_PATH];

			if(pThis->m_pStrXPage && _tcsstr(pThis->m_pStrXPage,L"dev:")==pThis->m_pStrXPage)
			{
				xfUriLoader::FetchXml(NULL,pThis->m_pStrXPage,NULL, *pxbXml);
			}else
			{
				xfUriLoader::UrlCombine(pThis->GetXPage()->m_pStrUrl,pThis->m_pStrXPage,strUrl,&dwSize );
				xfUriLoader::Load(strUrl,pThis->m_pStrUri,pData,len);
				
				if(pThis->m_pStrXPage[0]!='\0')
				{
					xfUriLoader::GetCacheFileName(strUrl,Path);
					if(!pxbXml->Load(Path))
						pxbXml->LoadXml((LPTSTR)pData);
				}else
					pxbXml->LoadXml((LPTSTR)pData);
				if(pData) xfUriLoader::FreeData(pData);
			}

			
			if(pThis->m_pStrXsl)
			{
				dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombine(pThis->GetXPage()->m_pStrUrl,pThis->m_pStrXsl,strUrl,&dwSize );
				xfUriLoader::Load(strUrl,NULL,pData,len);
				xfUriLoader::GetCacheFileName(strUrl,Path);

				xbXml* pxbXsl = new xbXml();
				if(!pxbXsl->Load(Path))
					pxbXsl->LoadXml((LPTSTR)pData);
				if(pData) xfUriLoader::FreeData(pData);

				BSTR bstr;
				pxbXml->m_pXmlDoc->transformNode(pxbXsl->m_pXmlDoc,&bstr);
				pxbXml->LoadXml(bstr);
				delete pxbXsl;
			}

			IXMLDOMElement * pElement;
			pxbXml->m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				pThis->m_pElement->appendChild(pElement,NULL);
				xfNode * pxfNode = pThis->CreateNode(pElement);
				pxfNode->m_pParentNode = pThis;
				pThis->m_pChilds->push_back(pxfNode);
				pxfNode->ParseNode(pElement,true);
			}

			delete pxbXml;
		}
		pNodeEx->Release();
		delete pParseNodeInfo;
		CoUninitialize();

		return Result;
	}

	int xfNodeXPage::SuperParseNode(IXMLDOMElement* pNodeEx,bool ParseChild) 
	{
		return xfNodeWin::ParseNode(pNodeEx,ParseChild);
	}

	int xfNodeXPage::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		/*
		UINT threadID;
		ParseNodeInfo * pParseNodeInfo = new ParseNodeInfo;
		pParseNodeInfo->pThis = this;
		pParseNodeInfo->pNodeEx = pNodeEx;
		pParseNodeInfo->bParseChild = ParseChild;

		xfNodeWin* pxfNodeWin = static_cast<xfNodeWin*>(this);

		pNodeEx->AddRef();
		::_beginthreadex(0, 0, DoParseNode, (void*)pParseNodeInfo, 0, &threadID);*/

		bool reload = 
			(!xfApp::GetApp()->m_pConnect || xfApp::GetApp()->m_pConnect && xfApp::GetApp()->m_pConnect->IsConnected())? true:false;
		if(!reload) return -1;

		m_pStrXPage = xbXml::GetStringAttribe(pNodeEx,_T("url"));
		if(m_pStrXPage) 
			m_pStrUrl = m_pStrXPage;
		else
			m_pStrXPage = xbXml::GetStringAttribe(pNodeEx,_T("src"));
		m_pStrUri = xbXml::GetStringAttribe(pNodeEx,_T("uri"));
		m_pStrXsl = xbXml::GetStringAttribe(pNodeEx,_T("xsl"));
		

		//int Result = xfNodeWin::ParseNode(pNodeEx,false);
		int Result = xfNode::ParseNode(pNodeEx,false);
		if(Result>=0 && m_pStrXPage )
		{
			xbXml* pxbXml = new xbXml();

			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH; 

			if(m_pStrXPage && _tcsstr(m_pStrXPage,L"dev:")==m_pStrXPage)
			{
				xfUriLoader::FetchXml(NULL,m_pStrXPage,NULL, *pxbXml);
			}else
			{
				//if(_tcsstr(m_pStrXPage,L"dev:")>=0)
				//	_tcscpy_s(strUrl,INTERNET_MAX_URL_LENGTH,m_pStrXPage);
				//else
					xfUriLoader::UrlCombine(this->GetXPage()->m_pStrUrl,m_pStrXPage,strUrl,&dwSize );

				BYTE * pData = NULL;
				int len;
				xfUriLoader::Load(strUrl,m_pStrUri,pData,len);

				TCHAR Path[MAX_PATH];
				if(m_pStrXPage[0]!='\0')
				{
					xfUriLoader::GetCacheFileName(strUrl,Path);
					if(!pxbXml->Load(Path))
						pxbXml->LoadXml((LPTSTR)pData);
				}else
					pxbXml->LoadXml((LPTSTR)pData);
				if(pData) xfUriLoader::FreeData(pData);
			}

			BYTE * pData = NULL;
			int len;
			TCHAR Path[MAX_PATH];
			if(m_pStrXsl)
			{
				dwSize = INTERNET_MAX_URL_LENGTH;
				xfUriLoader::UrlCombine(this->GetXPage()->m_pStrUrl,m_pStrXsl,strUrl,&dwSize );
				xfUriLoader::Load(strUrl,NULL,pData,len);
				xfUriLoader::GetCacheFileName(strUrl,Path);

				xbXml* pxbXsl = new xbXml();
				if(!pxbXsl->Load(Path))
					pxbXsl->LoadXml((LPTSTR)pData);
				if(pData) xfUriLoader::FreeData(pData);

				BSTR bstr;
				pxbXml->m_pXmlDoc->transformNode(pxbXsl->m_pXmlDoc,&bstr);
				pxbXml->LoadXml(bstr);
				delete pxbXsl;
			}

			IXMLDOMElement * pElement;
			pxbXml->m_pXmlDoc->get_documentElement(&pElement);
			if(pElement)
			{
				this->m_pElement->appendChild(pElement,NULL);
				xfNode * pxfNode = CreateNode(pElement);
				pxfNode->m_pParentNode = this;
				m_pChilds->push_back(pxfNode);
				pxfNode->ParseNode(pElement,true);
			}

			delete pxbXml;
			return Result;
		}else
			return Result;
	}
}}
