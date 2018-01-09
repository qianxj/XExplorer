#include "StdAfx.h"
#include "XShapeMgr.hpp"
#include "XShapeObject.hpp"
#include "XShapeObjectSet.hpp"
#include "shlwapi.h"

namespace Hxsoft{ namespace XFrame
{
	XShapeMgr::XShapeMgr(void)
	{
		m_pmapShapeMgrItem = new multimap<std::wstring , TShapeMgrItem *>;
		m_pmapShapeObjectSet = new	map<std::wstring, class XShapeObjectSet *>;
	}

	XShapeMgr::~XShapeMgr(void)
	{
		multimap<wstring , TShapeMgrItem *>::const_iterator objMgrIter;
		for(objMgrIter = (*m_pmapShapeMgrItem).begin();objMgrIter != (*m_pmapShapeMgrItem).end();objMgrIter++ )
		{
			if(objMgrIter->second) delete objMgrIter->second;
		}
		(*m_pmapShapeMgrItem).clear();

		map<wstring , XShapeObjectSet *>::const_iterator ShapeSetIter;
		for(ShapeSetIter = (*m_pmapShapeObjectSet).begin();ShapeSetIter != (*m_pmapShapeObjectSet).end();ShapeSetIter++ )
		{
			if(ShapeSetIter->second) delete ShapeSetIter->second;
		}
		(*m_pmapShapeObjectSet).clear();
	}
	
	XShapeObjectSet * XShapeMgr::GetShapeSet(LPTSTR pStrUri)
	{
		TCHAR pTemp[255];
		if(!pStrUri)
			pStrUri = _T("uri.hxsoft.com/xarch/xshape");
		if(::_tcsstr(pStrUri,_T("uri."))!=pStrUri)
		{
			_stprintf_s(pTemp,255,_T("%s/%s"),_T("uri.hxsoft.com/xarch/xshape"),pStrUri);
			pStrUri = pTemp;
		}
		std::pair<std::multimap<std::wstring , TShapeMgrItem *>::const_iterator,
			std::multimap<std::wstring , TShapeMgrItem *>::const_iterator> pr;
		pr = (*m_pmapShapeMgrItem).equal_range(pStrUri);
		while(pr.first != pr.second)
		{
			if(pr.first->second->m_pStrUri)
			{
				map<std::wstring, XShapeObjectSet *>::const_iterator pos;
				pos = m_pmapShapeObjectSet->find(pr.first->second->m_pStrFile);

				XShapeObjectSet * pShapeSet = NULL;
				if(pos== m_pmapShapeObjectSet->end())
				{
					pShapeSet = new XShapeObjectSet();
					TCHAR path[MAX_PATH];
					::GetModuleFileName(NULL,path,MAX_PATH);
					::PathRemoveFileSpec(path);
					::PathCombine(path,path,pr.first->second->m_pStrFile);
					pShapeSet->LoadShapeSet(path/*pr.first->second->m_pStrFile*/);
					pair<std::wstring,XShapeObjectSet *> pi;
					pi.first = pr.first->second->m_pStrFile;
					pi.second = pShapeSet;
					(*m_pmapShapeObjectSet).insert(pi);
				}else
					pShapeSet = (*pos).second;
				return pShapeSet;
			}
			pr.first++;
		}
		return NULL;
	}

	bool XShapeMgr::CreateShape(LPTSTR pStrUri, LPTSTR pStrObject,class XShapeObject * &pShape,bool bSetClassName)
	{
		pShape = NULL;
		TCHAR pTemp[255];
		if(!pStrUri)
			pStrUri = _T("uri.hxsoft.com/xarch/xshape");
		if(::_tcsstr(pStrUri,_T("uri."))!=pStrUri)
		{
			_stprintf_s(pTemp,255,_T("%s/%s"),_T("uri.hxsoft.com/xarch/xshape"),pStrUri);
			pStrUri = pTemp;
		}
		std::pair<std::multimap<std::wstring , TShapeMgrItem *>::const_iterator,
			std::multimap<std::wstring , TShapeMgrItem *>::const_iterator> pr;
		pr = (*m_pmapShapeMgrItem).equal_range(pStrUri);
		while(pr.first != pr.second)
		{
			if(pr.first->second->m_setShape.find(pStrObject) != pr.first->second->m_setShape.end())
			{
				if(pr.first->second->m_pStrUri)
				{
					map<std::wstring, XShapeObjectSet *>::const_iterator pos;
					pos = m_pmapShapeObjectSet->find(pr.first->second->m_pStrFile);

					XShapeObjectSet * pShapeSet = NULL;
					if(pos== m_pmapShapeObjectSet->end())
					{
						pShapeSet = new XShapeObjectSet();

						TCHAR path[MAX_PATH];
						::GetModuleFileName(NULL,path,MAX_PATH);
						::PathRemoveFileSpec(path);
						::PathCombine(path,path,pr.first->second->m_pStrFile);
						pShapeSet->LoadShapeSet(path);//pr.first->second->m_pStrFile);
						pair<std::wstring,XShapeObjectSet *> pi;
						pi.first = pr.first->second->m_pStrFile;
						pi.second = pShapeSet;
						(*m_pmapShapeObjectSet).insert(pi);
					}else
						pShapeSet = (*pos).second;
					pShape = pShapeSet->Find(pr.first->second->m_pStrUri,pStrObject);
					if(pShape) return  true;
				}
			}
			pr.first++;
		}
		return false;
	}

	void XShapeMgr::RegisterShapesByFile(LPTSTR pStrFile )
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

					pStr = pxbXmlSrc->GetStringAttribe(pSrcElement,_T("src"));
					TShapeMgrItem * pItem = new TShapeMgrItem;
					pItem->m_pStrFile = pStr;

					pair<std::wstring,TShapeMgrItem *> pr;

					pItem->m_pStrUri = pxbXmlSrc->GetStringAttribe(pSrcElement,_T("uri"));
					pr.first = pItem->m_pStrUri;
					pr.second = pItem;
					m_pmapShapeMgrItem->insert(pr);
					pSrcElement->Release();

					pSrcNode->selectNodes(L"shape",&pObjList);
					pObjList->get_length(&lenObj);
					for(int k=0;k<lenObj;k++)
					{
						pObjList->get_item(k,&pObjNode);
						pObjElement = pxbXmlSrc->GetElement(pObjNode);
						pStr = pxbXmlSrc->GetStringAttribe(pObjElement,_T("name"));
						pItem->m_setShape.insert(pStr);
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

}}