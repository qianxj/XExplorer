#include "StdAfx.h"
#include "IEntitySvr.hpp"
#include "xbxbase.h"
#include "xfUriLoader.hpp"
#include "IXAServer.hpp"
#include "ITrans.hpp"

namespace Hxsoft{ namespace XFrame
{
	IEntitySvr::IEntitySvr():m_pTrans(NULL)
	{
	}

	int IEntitySvr::SetTrans(ITrans * pTrans)
	{
		m_pTrans = pTrans;
		return 1;
	}
	ITrans * IEntitySvr::GetTrans()
	{
		return m_pTrans;
	}


	//读取实体列表模板
	int IEntitySvr::GetEntityListTemplate(LPCTSTR pServer,LPCTSTR pWkUrl,IXMLDOMElement * &pElement)
	{
		IXMLDOMElement * pDocElement;
		IXMLDOMNode * pNode;
		xbXml xml;

		pElement = NULL;
		TCHAR cmdStr[512];
		_stprintf_s(cmdStr,512,_T("%s?ask=xdoc"),pWkUrl);
		if(IXAServer::FetchXml(pServer,cmdStr,NULL,xml)==1)
		{
			xml.m_pXmlDoc->get_documentElement(&pDocElement);
			if(pDocElement)
			{
				pDocElement->selectSingleNode(L"*",&pNode);
				if(pNode)
				{
					pNode->QueryInterface(IID_IXMLDOMElement,(void **) &pElement);
					pNode->Release();
					return 1;
				}
				pDocElement->Release();
			}
		}
		return -1;
	}

	int IEntitySvr::GetEntityListTemplate(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement)
	{
		xbXml xml;
		pElement = NULL;
		if(_tcschr(pEntityName,'/'))pEntityName = _tcschr(pEntityName,'/') + 1;
		if(IXAServer::GetDevObject(pServer,_T("entity"),pEntityName,xml)!=1) return -1;

		long nLen;
		IXMLDOMNodeList*	pList;
		IXMLDOMNode*		pNode;
		IXMLDOMElement*		pSheet;
		IXMLDOMElement*		pSchemaElement;
		_variant_t	var;
		xml.m_pXmlDoc->selectNodes(_T("entry/list/schema"),&pList);
		pList->get_length(&nLen);
		if(nLen < nMode)
		{
			pList->Release();
			return -1;
		}

		pList->get_item(nMode ,&pNode);
		pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSchemaElement);
		pNode->Release();

		pSchemaElement->selectSingleNode(_T("sheet"),&pNode);
		pSchemaElement->Release();
		if(pNode)
		{
			pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pSheet);
			pNode->Release();
			pSheet->getAttribute(L"sheetguid",&var);
			pSheet->Release();
			if(var.bstrVal)
			{
				xbXml xml;
				int nRet = IXAServer::GetDevObject(pServer,_T("sheet"),var.bstrVal,xml);
				if(nRet==1)
					xml.m_pXmlDoc->get_documentElement(&pElement);
				return nRet;
			}
		}
		return -1;
	}

	int IEntitySvr::GetEntityListTemplate(ITrans * pTrans,LPCTSTR pWkUrl,IXMLDOMElement * &pElement)
	{
		if(!pTrans) return -1;
		return GetEntityListTemplate(pTrans->GetServerUrl(),pWkUrl,pElement);
	}
	int IEntitySvr::GetEntityListTemplate(ITrans * pTrans,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement)
	{
		if(!pTrans) return -1;
		return GetEntityListTemplate(pTrans->GetServerUrl(),pEntityName,nMode, pElement);
	}
	int IEntitySvr::GetEntityListTemplate(LPCTSTR pWkUrl,IXMLDOMElement * &pElement)
	{
		if(!m_pTrans) return -1;
		return GetEntityListTemplate(m_pTrans,pWkUrl,pElement);
	}
	int IEntitySvr::GetEntityListTemplate(LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement)
	{
		if(!m_pTrans) return -1;
		return GetEntityListTemplate(m_pTrans,pEntityName,nMode, pElement);
	}

	//读取实体列表数据
	int IEntitySvr::GetEntityListData(LPCTSTR pServer,LPCTSTR pWkUrl,LPCTSTR pArgs,xbXml &xml,DWORD dwBox)
	{
		TCHAR cmdStr[512];
		xbXml xmlInfo;
		_stprintf_s(cmdStr,512,_T("%s?ask=info"),pWkUrl);
		if(xfUriLoader::FetchXml((LPTSTR)pServer,cmdStr,NULL,xmlInfo)==1)
		{
			IXMLDOMNode * pNode;
			xmlInfo.m_pXmlDoc->selectSingleNode(L"worknode/entity/@guid",&pNode);
			if(pNode)
			{
				BSTR pEntityName;
				pNode->get_text(&pEntityName);
				int nRet = IEntitySvr::GetEntityListData(pServer,pEntityName,0,pArgs,xml);
				pNode->Release();
				return nRet;
			}
		}

		return -1;
	}

	int IEntitySvr::GetEntityListData(LPCTSTR pServer,LPTSTR pEntityName,int nMode,LPCTSTR pArs,xbXml &xml,DWORD dwBox)
	{
		TCHAR Buf[MAX_PATH];
		_stprintf_s(Buf,MAX_PATH,_T("%s%s"),_T("uri.hxsoft.com/xaserver/entitysvr/querylist/"),pEntityName);
		return IXAServer::FetchXml((LPTSTR)pServer,Buf,NULL,xml);
	}

	int IEntitySvr::GetEntityListData(ITrans * pTrans,LPCTSTR pWkUrl,LPCTSTR pArgs,xbXml &xml,DWORD dwBox)
	{
		if(!pTrans) return -1;
		return GetEntityListData(pTrans->GetServerUrl(),pWkUrl,pArgs,xml,dwBox);
	}
	int IEntitySvr::GetEntityListData(ITrans * pTrans,LPTSTR pEntityName,int nMode,LPCTSTR pArs,xbXml &xml,DWORD dwBox)
	{
		if(!pTrans) return -1;
		return GetEntityListData(pTrans->GetServerUrl(),pEntityName,nMode,pArs,xml,dwBox);
	}
	int IEntitySvr::GetEntityListData(LPCTSTR pWkUrl,LPCTSTR pArgs,xbXml &xml,DWORD dwBox)
	{
		if(!m_pTrans) return -1;
		return GetEntityListData(m_pTrans,pWkUrl,pArgs,xml,dwBox);
	}
	int IEntitySvr::GetEntityListData(LPTSTR pEntityName,int nMode,LPCTSTR pArs,xbXml &xml,DWORD dwBox)
	{
		if(!m_pTrans) return -1;
		return GetEntityListData(m_pTrans,pEntityName,nMode,pArs,xml,dwBox);
	}

	//读取实体列表数据
	int IEntitySvr::GetEntityListAgent(LPCTSTR pServer,LPCTSTR pWkUrl,xbXml &xml)
	{
		BSTR pEntityName = NULL;
		int nMode=-1;

		IEntitySvr::GetEntityName(pServer,pWkUrl,pEntityName,nMode);
		if(pEntityName)
		{
			int nRet = GetEntityListAgent(pServer,pEntityName,nMode,xml);
			::SysFreeString(pEntityName);
			return nRet;
		}
		return -1;
	}

	LPTSTR  IEntitySvr::GetEntityListAgent(LPCTSTR pServer,LPTSTR pEntityName,int nMode)
	{
		LPTSTR pStr = NULL;
		BSTR bstr=NULL;
		xbXml xml;
		
		IEntitySvr::GetEntityListAgent(pServer,pEntityName,0,xml);
		xml.m_pXmlDoc->get_xml(&bstr);
		if(bstr) pStr = _tcsdup(bstr);
		::SysFreeString(bstr);

		return pStr;
	}

	int IEntitySvr::GetEntityListAgent(LPCTSTR pServer,LPTSTR pEntityName,int nMode,xbXml &xml)
	{
		TCHAR Buf[MAX_PATH];
		_stprintf_s(Buf,MAX_PATH,_T("uri.hxsoft.com/xaserver/entitysvr/listagent/%s"),pEntityName);
		return IXAServer::FetchXml((LPTSTR)pServer,Buf,NULL,xml);
	}

	int IEntitySvr::GetEntityListAgent(ITrans * pTrans,LPCTSTR pWkUrl,xbXml &xml)
	{
		if(!pTrans) return -1;
		return GetEntityListAgent(pTrans->GetServerUrl(),pWkUrl,xml);
	}

	LPTSTR  IEntitySvr::GetEntityListAgent(ITrans * pTrans,LPTSTR pEntityName,int nMode)
	{
		if(!pTrans) return NULL;
		return GetEntityListAgent(pTrans->GetServerUrl(),pEntityName, nMode);
	}
	int IEntitySvr::GetEntityListAgent(ITrans * pTrans,LPTSTR pEntityName,int nMode,xbXml &xml)
	{
		if(!pTrans) return -1;
		return GetEntityListAgent(pTrans->GetServerUrl(),pEntityName, nMode,xml);
	}

	int IEntitySvr::GetEntityListAgent(LPCTSTR pWkUrl,xbXml &xml)
	{
		if(!m_pTrans) return -1;
		return GetEntityListAgent(m_pTrans->GetServerUrl(),pWkUrl,xml);
	}
	LPTSTR  IEntitySvr::GetEntityListAgent(LPTSTR pEntityName,int nMode)
	{
		if(!m_pTrans) return NULL;
		return GetEntityListAgent(m_pTrans->GetServerUrl(),pEntityName, nMode);
	}
	int IEntitySvr::GetEntityListAgent(LPTSTR pEntityName,int nMode,xbXml &xml)
	{
		if(!m_pTrans) return -1;
		return GetEntityListAgent(m_pTrans->GetServerUrl(),pEntityName, nMode,xml);
	}


		//读取实体编辑模板
	int IEntitySvr::GetEntityDoc(LPCTSTR pServer,LPCTSTR pWkUrl,IXMLDOMElement * &pElement)
	{
		pElement = NULL;
		BSTR pEntityName = NULL;
		int nMode=-1;

		IEntitySvr::GetEntityName(pServer,pWkUrl,pEntityName,nMode);
		if(!pEntityName) return -1;
		int nRet = GetEntityDoc(pServer, pEntityName,nMode,pElement);
		::SysFreeString(pEntityName);
		return nRet;

	}

	int IEntitySvr::GetEntityDoc(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement)
	{
		if(!pEntityName)return -1;
		if(_tcschr(pEntityName,'/'))pEntityName = _tcschr(pEntityName,'/') + 1;

		xbXml xml;
		pElement = NULL;
		if(IXAServer::GetDevObject((LPTSTR)pServer,_T("entity"),pEntityName,xml)!=1) return -1;

		long nLen;
		TCHAR strCmd[512];
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
		
		if(vguids.size() < 1) return -1;
		int nIndex = nMode - 1;
		if(nIndex < 0)nIndex = 0;
		if(nIndex >= 	(int)vguids.size()) return -1;
		::_stprintf_s(strCmd,512, _T("uri.hxsoft.com/xaserver/developer getdocbysheet %s"),	vguids[nIndex].c_str());
		if(IXAServer::FetchXml((LPTSTR)pServer,strCmd,NULL,xml)!=1) return -1;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		if(pElement) return 1;
		return -1;
	}

	int IEntitySvr::GetEntityDoc(ITrans * pTrans,LPCTSTR pWkUrl,IXMLDOMElement * &pElement)
	{
		if(!pTrans) return -1;
		return GetEntityDoc(pTrans->GetServerUrl(), pWkUrl,pElement);
	}

	int IEntitySvr::GetEntityDoc(ITrans * pTrans,LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement)
	{
		if(!pTrans) return -1;
		return GetEntityDoc(pTrans->GetServerUrl(), pEntityName,nMode,pElement);
	}
	int IEntitySvr::GetEntityDoc(LPCTSTR pWkUrl,IXMLDOMElement * &pElement)
	{
		if(!m_pTrans) return -1;
		return GetEntityDoc(m_pTrans->GetServerUrl(), pWkUrl,pElement);
	}

	int IEntitySvr::GetEntityDoc(LPCTSTR pEntityName,int nMode,IXMLDOMElement * &pElement)
	{
		if(!m_pTrans) return -1;
		return GetEntityDoc(m_pTrans->GetServerUrl(), pEntityName,nMode,pElement);
	}

	
	//读取实体编辑数据
	int IEntitySvr::GetEntityData(LPCTSTR pServer,LPCTSTR pWkUrl,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox,DWORD Version,DWORD SubVersion)
	{
		BSTR pEntityName = NULL;
		int nMode=-1;

		IEntitySvr::GetEntityName(pServer,pWkUrl,pEntityName,nMode);
		if(!pEntityName) return -1;

		int nRet = GetEntityData(pServer,pEntityName,nMode,pExtStr,xml);
		::SysFreeString(pEntityName);
		return nRet;
	}

	int IEntitySvr::GetEntityData(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox,DWORD Version,DWORD SubVersion)
	{
		TCHAR Buf[MAX_PATH];
		_stprintf_s(Buf,MAX_PATH,_T("uri.hxsoft.com/xaserver/entitysvr/get/%s"),pEntityName);
		return IXAServer::FetchXml((LPTSTR)pServer,Buf,(LPTSTR)pExtStr,xml);
	}

	int IEntitySvr::GetEntityData(ITrans * pTrans,LPCTSTR pWkUrl,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox,DWORD Version,DWORD SubVersion)
	{
		if(!pTrans) return -1;
		return GetEntityData(pTrans->GetServerUrl(),pWkUrl,pExtStr,xml,dwBox,Version,SubVersion);
	}
	int IEntitySvr::GetEntityData(ITrans * pTrans,LPCTSTR pEntityName,int nMode,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox,DWORD Version,DWORD SubVersion)
	{
		if(!pTrans) return -1;
		return GetEntityData(pTrans->GetServerUrl(),pEntityName,nMode,pExtStr,xml,dwBox,Version,SubVersion);
	}
	int IEntitySvr::GetEntityData(LPCTSTR pWkUrl,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox,DWORD Version,DWORD SubVersion)
	{
		if(!m_pTrans) return -1;
		return GetEntityData(m_pTrans->GetServerUrl(),pWkUrl,pExtStr,xml,dwBox,Version,SubVersion);
	}
	int IEntitySvr::GetEntityData(LPCTSTR pEntityName,int nMode,LPCTSTR pExtStr,xbXml &xml,DWORD dwBox,DWORD Version,DWORD SubVersion)
	{
		if(!m_pTrans) return -1;
		return GetEntityData(m_pTrans->GetServerUrl(),pEntityName,nMode,pExtStr,xml,dwBox,Version,SubVersion);
	}


	
	LPTSTR   IEntitySvr::GetEntityName(LPCTSTR pServer,LPCTSTR pWkUrl)
	{
		int nMode;
		return IEntitySvr::GetEntityName(pServer,pWkUrl,nMode);
	}

	LPTSTR  IEntitySvr::GetEntityName(LPCTSTR pServer,LPCTSTR pWkUrl,int & nMode)
	{
		BSTR bstr= NULL;
		IEntitySvr::GetEntityName(pServer, pWkUrl,bstr,nMode);
		if(!bstr) return NULL;
		LPTSTR pStr = _tcsdup(bstr);
		::SysFreeString(bstr);
		return pStr;
	}

	LPTSTR   IEntitySvr::GetEntityName(ITrans * pTrans,LPCTSTR pWkUrl)
	{
		if(!pTrans) return NULL;
		return GetEntityName(pTrans->GetServerUrl(),pWkUrl);
	}

	LPTSTR  IEntitySvr::GetEntityName(ITrans * pTrans,LPCTSTR pWkUrl,int & nMode)
	{
		if(!pTrans) return NULL;
		return GetEntityName(pTrans->GetServerUrl(),pWkUrl,nMode);
	}

	LPTSTR   IEntitySvr::GetEntityName(LPCTSTR pWkUrl)
	{
		if(!m_pTrans) return NULL;
		return GetEntityName(m_pTrans->GetServerUrl(),pWkUrl);
	}

	LPTSTR  IEntitySvr::GetEntityName(LPCTSTR pWkUrl,int & nMode)
	{
		if(!m_pTrans) return NULL;
		return GetEntityName(m_pTrans->GetServerUrl(),pWkUrl,nMode);
	}

	//得到实体GUID
	int IEntitySvr::GetEntityName(LPCTSTR pServer,LPCTSTR pWkUrl,BSTR & pEntity,int & nMode)
	{
		TCHAR cmdStr[512];
		xbXml xmlInfo;
		pEntity = NULL;
		nMode = -1;
		_stprintf_s(cmdStr,512,_T("%s?ask=info"),pWkUrl);
		if(xfUriLoader::FetchXml((LPTSTR)pServer,cmdStr,NULL,xmlInfo)==1)
		{
			IXMLDOMNode * pNode;
			xmlInfo.m_pXmlDoc->selectSingleNode(L"worknode/entity/@guid",&pNode);
			if(pNode)
			{
				pNode->get_text(&pEntity);
				pNode->Release();
			}

			BSTR pMode;
			xmlInfo.m_pXmlDoc->selectSingleNode(L"worknode/entity/@mode",&pNode);
			if(pNode)
			{
				pNode->get_text(&pMode);
				pNode->Release();
				if(pMode)
				{
					nMode = ::_ttoi(pMode);
					::SysFreeString(pMode);
				}
			}
			return 1;
		}
		return -1;
	}

	int IEntitySvr::GetEntityName(ITrans * pTrans,LPCTSTR pWkUrl,BSTR & pEntity,int & nMode)
	{
		if(pTrans) return -1;
		return GetEntityName(pTrans->GetServerUrl(), pWkUrl,pEntity,nMode);
	}
	int IEntitySvr::GetEntityName(LPCTSTR pWkUrl,BSTR & pEntity,int & nMode)
	{
		if(m_pTrans) return -1;
		return GetEntityName(m_pTrans->GetServerUrl(), pWkUrl,pEntity,nMode);
	}

	//保存实体数据
	 int IEntitySvr::SaveEntityData(LPCTSTR pServer,LPCTSTR pWkUrl,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox)
	 {
		BSTR pEntityName = NULL;
		int nMode=-1;

		IEntitySvr::GetEntityName(pServer,pWkUrl,pEntityName,nMode);
		if(!pEntityName) return -1;

		int nRet = SaveEntityData(pServer,pEntityName,nMode,pEntityKey,pEntityData);
		::SysFreeString(pEntityName);
		return nRet;
		
	 }

	//保存实体数据
	int IEntitySvr::SaveEntityData(LPCTSTR pServer,LPCTSTR pEntityName,int nMode,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox)
	{
		int nStrLen = (int)_tcslen(pEntityKey) + 1 + (int)_tcslen(pEntityData) + 1 + 255;
		LPTSTR pBuf = new TCHAR[nStrLen];
		::ZeroMemory(pBuf,nStrLen * sizeof(TCHAR));
		_stprintf_s(pBuf,nStrLen,_T("<ID>%s</ID>%s"),pEntityKey,pEntityData);

		TCHAR pStrUrl[255];
		_stprintf_s(pStrUrl,255,_T("uri.hxsoft.com/xaserver/entitysvr/set/%s"),pEntityName);

		LPBYTE pData = NULL;
		int nLen;
		int nRet = IXAServer::ProcessCmd((LPTSTR)pServer,pStrUrl,pBuf,pData,nLen);
		if(pData)xfUriLoader::FreeData(pData);
		delete pBuf;
		return nRet;
	}

	int IEntitySvr::SaveEntityData(ITrans * pTrans,LPCTSTR pWkUrl,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox)
	{
		if(!pTrans) return -1;
		return SaveEntityData(pTrans->GetServerUrl(),pWkUrl,pEntityKey,pEntityData,dwBox);
	}
	int IEntitySvr::SaveEntityData(ITrans * pTrans,LPCTSTR pEntityName,int nMode,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox)
	{
		if(!pTrans) return -1;
		return SaveEntityData(pTrans->GetServerUrl(),pEntityName,nMode,pEntityKey, pEntityData, dwBox);
	}
	int IEntitySvr::SaveEntityData(LPCTSTR pWkUrl,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox)
	{
		if(!m_pTrans) return -1;
		return SaveEntityData(m_pTrans->GetServerUrl(),pWkUrl,pEntityKey,pEntityData,dwBox);
	}
	int IEntitySvr::SaveEntityData(LPCTSTR pEntityName,int nMode,LPCTSTR pEntityKey,LPCTSTR pEntityData,DWORD dwBox)
	{
		if(!m_pTrans) return -1;
		return SaveEntityData(m_pTrans->GetServerUrl(),pEntityName,nMode,pEntityKey, pEntityData, dwBox);
	}
}}