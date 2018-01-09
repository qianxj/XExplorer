#include "StdAfx.h"
#include "xbObject.hpp"
#include "xbObjectMgr.hpp"
#include "xbXml.hpp"
#include <string>
namespace Hxsoft{ namespace XFrame
{
	xbObjectMgr::xbObjectMgr(void)
	{
		m_pObjectMgrs = new std::vector<xbObjectMgr *>;
		m_pmapObjectMgrItem = new std::multimap<std::wstring , xbObjectMgrItem *>;
	}

	xbObjectMgr::~xbObjectMgr(void)  
	{
		std::multimap<std::wstring , xbObjectMgrItem *>::const_iterator objMgrIter;
		for(objMgrIter = (*m_pmapObjectMgrItem).begin();objMgrIter != (*m_pmapObjectMgrItem).end();objMgrIter++ )
		{
			//if(objMgrIter->first) delete objMgrIter->first;
			if(objMgrIter->second) delete objMgrIter->second; 
		}
		(*m_pmapObjectMgrItem).clear();

		for( int i=0;i< (int)(*m_pObjectMgrs).size();i++)
		{
			if((*m_pObjectMgrs)[i]) delete (*m_pObjectMgrs)[i];
		}
		(*m_pObjectMgrs).clear();
	}

	bool xbObjectMgr::IsControl(LPTSTR pStrUri, LPTSTR pStrObject)
	{
		if(!pStrUri)
			pStrUri = _T("uri.hxsoft.com/xarch/xcontrol");
		std::pair<std::multimap<std::wstring , xbObjectMgrItem *>::const_iterator,
			std::multimap<std::wstring , xbObjectMgrItem *>::const_iterator> pr;
		pr = (*m_pmapObjectMgrItem).equal_range(pStrUri);
		while(pr.first != pr.second)
		{
			if(pr.first->second->m_setObject.find(pStrObject) != pr.first->second->m_setObject.end()) return true;
			pr.first++;
		}

		for(int i=0;i<(int)(*m_pObjectMgrs).size();i++ )
		{
			if((*m_pObjectMgrs)[i]->IsControl(pStrUri,pStrObject))return true;
		}
		return false;
	}

	bool xbObjectMgr::CreateInstance(LPTSTR pStrUri, LPTSTR pStrObject,xbObject * &pxbObject,bool bSetClassName)
	{
		pxbObject = NULL;
		if(!pStrUri)
			pStrUri = _T("uri.hxsoft.com/xarch/xcontrol");
		std::pair<std::multimap<std::wstring , xbObjectMgrItem *>::const_iterator,
			std::multimap<std::wstring , xbObjectMgrItem *>::const_iterator> pr;
		pr = (*m_pmapObjectMgrItem).equal_range(pStrUri);
		while(pr.first != pr.second)
		{
			if(pr.first->second->m_setObject.find(pStrObject) != pr.first->second->m_setObject.end())
			{
				if(!pr.first->second->m_hCtrlDLL)
				{
					pr.first->second->m_hCtrlDLL=::LoadLibrary(pr.first->second->m_pStrCtrlFile);  
				}
				if(!pr.first->second->m_pFnCreateInstance)
				{
					if(!pr.first->second->m_hCtrlDLL)
						pr.first->second->m_hCtrlDLL=::LoadLibrary(pr.first->second->m_pStrCtrlFile);

					if(pr.first->second->m_hCtrlDLL)
					{
						pr.first->second->m_pFnCreateInstance = (fnCreateInstance)::GetProcAddress(pr.first->second->m_hCtrlDLL,"CreateInstance");
					}
				}
				if(pr.first->second->m_pFnCreateInstance)
				{
					if(pr.first->second->m_pFnCreateInstance(pStrObject, pxbObject))
					{
						if(pxbObject && bSetClassName)pxbObject->SetClassName(pStrUri,pStrObject);
						return true;
					}
				}else
					return false;
			}
			pr.first++;
		}

		for(int i=0;i<(int)(*m_pObjectMgrs).size();i++ )
		{
			if((*m_pObjectMgrs)[i]->CreateInstance(pStrUri,pStrObject,pxbObject))
			{
				if(bSetClassName)pxbObject->SetClassName(::_tcsdup(pStrUri),::_tcsdup(pStrObject));
				return true;
			}
		}
		return false;
	}

	void xbObjectMgr::RegisterObjectsByXmlFile(LPTSTR pStrFile )
	{
		xbXml *	pxbXml = new xbXml();
		if(!pxbXml) return;

		if(!pxbXml->Load(pStrFile))
		{
			delete pxbXml;
			return;
		}

		IXMLDOMNodeList*	pList;
		IXMLDOMNode*		pNode;
		IXMLDOMElement*		pElement;
		IXMLDOMNodeList*	pSrcList;
		IXMLDOMNode*		pSrcNode;
		IXMLDOMElement*		pSrcElement;
		IXMLDOMNodeList*	pObjList;
		IXMLDOMNode*		pObjNode;
		IXMLDOMElement*		pObjElement;
		long	len;
		long	lenSrc;
		long	lenObj;
		LPTSTR	pSrcFile = NULL;
		LPTSTR pStr;

		pxbXml->m_pXmlDoc->selectNodes(L"//file",&pList);
		pList->get_length(&len);

		for(int i=0;i<len;i++)
		{
			pList->get_item(i,&pNode);
			pElement = pxbXml->GetElement(pNode);
			

			_variant_t var;
			pElement->getAttribute(L"src",&var);
			pSrcFile = new wchar_t[::wcslen(var.bstrVal)+1];
			::wcscpy_s(pSrcFile,::wcslen(var.bstrVal)+1,var.bstrVal); 
			//pSrcFile = pxbXml->GetStringAttribe(pElement,_T("src"));
			
			if(pSrcFile )
			{
				xbXml * pxbXmlSrc = new xbXml();
				pxbXmlSrc->Load(pSrcFile);
				pxbXmlSrc->m_pXmlDoc->selectNodes(L"items/item",&pSrcList);
				pSrcList->get_length(&lenSrc);
				for(int j=0;j<lenSrc;j++)
				{
					pSrcList->get_item(j,&pSrcNode);
					pSrcElement = pxbXmlSrc->GetElement(pSrcNode);

					pStr = pxbXmlSrc->GetStringAttribe(pSrcElement,_T("dll"));
					xbObjectMgrItem * pItem = new xbObjectMgrItem;
					pItem->m_pStrCtrlFile = pStr;

					std::pair<LPTSTR,xbObjectMgrItem *> pr;
					pr.first = pxbXmlSrc->GetStringAttribe(pSrcElement,_T("name"));
					pr.second = pItem;
					(*m_pmapObjectMgrItem).insert(pr);
					pSrcElement->Release();


					pSrcNode->selectNodes(L"object",&pObjList);
					pObjList->get_length(&lenObj);
					for(int k=0;k<lenObj;k++)
					{
						pObjList->get_item(k,&pObjNode);
						pObjElement = pxbXmlSrc->GetElement(pObjNode);
						pStr = pxbXmlSrc->GetStringAttribe(pObjElement,_T("name"));
						pItem->m_setObject.insert(pStr);
						pObjNode->Release();
						pObjElement->Release();
					}
					pObjList->Release();
					pSrcNode->Release();
				}
				pSrcList->Release();
				delete pxbXmlSrc;
				if(pSrcFile) delete pSrcFile;
			}

			pElement->Release();
			pNode->Release();
		}

		pList->Release();
		delete pxbXml;
		return;
	}
	void xbObjectMgr::RegisterObjectsByObjectMgr(LPTSTR pStrUri,xbObjectMgr * pObjectMgr )
	{
		(*m_pObjectMgrs).push_back( pObjectMgr);
	}
}}
