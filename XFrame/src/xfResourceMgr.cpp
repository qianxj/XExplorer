#include "StdAfx.h"

#include "xfNode.hpp"
#include "xfNoderesource.hpp"
#include "xfResourceMgr.hpp"

namespace Hxsoft{ namespace XFrame
{
	LPTSTR xfResourceMgr::m_DefaultLocal=_T("cn");
	xfResourceMgr::xfResourceMgr(void)
	{
		m_pStringResource = new map<wstring,map<wstring,wstring>>;
	}

	xfResourceMgr::~xfResourceMgr(void)
	{
		for(int i=0;i<m_Resources.size();i++)
		{
			if(m_Resources[i]) delete m_Resources[i];
		}
		m_Resources.clear();

		for(int i=0;i<m_hImageLists.size();i++)
		{
			if(m_hImageLists[i])::ImageList_Destroy(m_hImageLists[i]);
		}
		m_hImageLists.clear();

		m_pStringResource->clear();
		if(m_pStringResource) delete m_pStringResource;
		m_pStringResource = NULL;
	}

	int xfResourceMgr::AddResource(xfNodeResource * pxfNodeResource)
	{
		pxfNodeResource->m_pResourceMgr = this;
		return m_Resources.push_back(pxfNodeResource);
	}

	int  xfResourceMgr::AddImageList(HIMAGELIST hImageList)
	{
		return m_hImageLists.push_back(hImageList);
	}

	Hxsoft::XUtil::xuVector<HIMAGELIST>&  xfResourceMgr::GetImageList()
	{
		return m_hImageLists;
	}

	LPCTSTR xfResourceMgr::GetStringItem(LPTSTR strItem)
	{
		return GetStringItem(m_DefaultLocal,strItem);
	}

	LPCTSTR xfResourceMgr::GetStringItem(LPTSTR strLocal,LPTSTR strItem)
	{
		if(!strLocal) strLocal = m_DefaultLocal;
		map<wstring,wstring> * pLocal = GetLocalItem(strLocal);
		if(!pLocal) return NULL;

		map<wstring,wstring>::const_iterator pr;
		pr = pLocal->find(strItem);
		if(pr!= pLocal->end())
			return pr->second.c_str();
		else
			return NULL;
	}
	bool xfResourceMgr::AddStringItem(LPTSTR strItemName,LPTSTR strItemText)
	{
		return AddStringItem(m_DefaultLocal, strItemName,strItemText);
	}
	bool xfResourceMgr::AddStringItem(LPCTSTR strLocal,LPCTSTR strItemName,LPCTSTR strItemText)
	{
		pair<wstring,wstring> pr;
		pr.first=strItemName;
		pr.second=strItemText;

		if(!strLocal) strLocal = m_DefaultLocal;
		map<wstring,wstring> * pLocal = GetLocalItem(strLocal);
		if(!pLocal)
		{
			pLocal = new map<wstring,wstring>;
			pair<wstring,map<wstring,wstring>> prLocal;
			pLocal->insert(pr);
			prLocal.first = strLocal;
			prLocal.second = *pLocal;
			m_pStringResource->insert(prLocal);
			delete pLocal;
		}else
			pLocal->insert(pr);
		return true;
	}

	map<wstring,wstring> * xfResourceMgr::GetLocalItem()
	{
		return GetLocalItem(m_DefaultLocal);
	}

	map<wstring,wstring> * xfResourceMgr::GetLocalItem(LPCTSTR strLocal)
	{
		map<wstring,map<wstring,wstring>>::const_iterator pr;
		pr = m_pStringResource->find(strLocal);
		if(pr!=  m_pStringResource->end())
			return (map<wstring,wstring> *)&((*pr).second);
		else
			return NULL;
	}

	int xfResourceMgr::AddLocalFile(LPTSTR pStrFile)
	{

		xbXml* pXml = new xbXml;
		pXml->Load(pStrFile);

		IXMLDOMNodeListPtr	pNodeList;
		IXMLDOMNodePtr	pNodeTool;
		VARIANT	sval;
		long	toolen;

		pXml->m_pXmlDoc->selectNodes(L"locals/local",&pNodeList);
		pNodeList->get_length(&toolen);
		for( int l=0;l<toolen;l++)
		{
			LPTSTR	 pLocal;
			pNodeList->get_item(l,&pNodeTool);
			((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"name",&sval);
			if(sval.vt==VT_BSTR && sval.bstrVal)
			{
				pLocal = XUtil::tcsdupFromBstr(sval.bstrVal);
			}
			::VariantClear(&sval);

			IXMLDOMNodeListPtr	pNodeListExd;
			IXMLDOMNodePtr	pNodeExd;
			IXMLDOMElement*	pElementexd;
			long	len1;
			LPTSTR pStrItem;
			LPTSTR pStrText;

			pNodeTool->selectNodes(L"item",&pNodeListExd);
			pNodeListExd->get_length(&len1);
			for(int m=0;m<len1;m++)
			{
				pNodeListExd->get_item(m,&pNodeExd);
				pElementexd = xbXml::GetElement(pNodeExd);
				
				pElementexd->getAttribute(L"name",&sval);
				if(sval.vt==VT_BSTR && sval.bstrVal)
				{
					pStrItem = XUtil::tcsdupFromBstr(sval.bstrVal);
				}
				::VariantClear(&sval);	

				BSTR bstr;
				pElementexd->get_text(&bstr);
				if(bstr)
				{
					pStrText = XUtil::tcsdupFromBstr(bstr);	
					::SysFreeString(bstr);
				}else
					pStrText = NULL;
				pElementexd->Release();
				AddStringItem(pLocal,pStrItem,pStrText);
				if(pStrItem) delete pStrItem;
				if(pStrText) delete pStrText;
			}
			if(pLocal) delete pLocal;
		}
		if(pXml) delete pXml;
		
		return 1;
	}
}}
