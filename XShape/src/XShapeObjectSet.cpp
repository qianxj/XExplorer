#include "StdAfx.h"
#include "XShapeObjectSet.hpp"
#include "xshapeobjectsetitem.hpp"
#include "xshapeobject.hpp"

namespace Hxsoft{ namespace XFrame
{
	XShapeObjectSet::XShapeObjectSet(void):m_ShapeObjectSetItems(new vector<XShapeObjectSetItem *>)
	{
	}

	XShapeObjectSet::~XShapeObjectSet(void)
	{
		for(int i=0;i<(int)m_ShapeObjectSetItems->size();i++)
		{
			if((*m_ShapeObjectSetItems)[i])delete (*m_ShapeObjectSetItems)[i];
		}
		m_ShapeObjectSetItems->clear();
	}

	int XShapeObjectSet::LoadShapeSet(LPTSTR pStrFile)
	{
		xbXml	xml;
		xml.Load(pStrFile);
		IXMLDOMElement *pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int result = 0;
		if(pElement)
		{
			result = LoadShapeSet(pElement);
			pElement->Release();
		}
		return result;
	}

	int XShapeObjectSet::LoadShapeSetXml(LPTSTR pStrXml)
	{
		xbXml	xml;
		xml.LoadXml(pStrXml);
		IXMLDOMElement *pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int result = LoadShapeSet(pElement);
		pElement->Release();
		return result;
	}

	int XShapeObjectSet::LoadShapeSet(IXMLDOMElement * m_pElement)
	{
		IXMLDOMNodeList * pList;
		IXMLDOMElement*  pElementEx;
		IXMLDOMNode * pNode;
		m_pElement->selectNodes(L"shapeset",&pList);
		long	nLen;
		pList->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			XShapeObjectSetItem*	pObject = new XShapeObjectSetItem();
			pList->get_item(i,&pNode);
			pElementEx = xbXml::GetElement(pNode);
			pObject->LoadShapeSetItem(pElementEx);
			m_ShapeObjectSetItems->push_back(pObject);
			pElementEx->Release();
			pNode->Release();
		}
		if(pList) pList->Release();

		return 0;
	}

	XShapeObject * XShapeObjectSet::Find(LPTSTR strUri,LPTSTR strShape)
	{
		for(int i=0;i<(int)m_ShapeObjectSetItems->size();i++)
		{
			if(::_tcsicmp((*m_ShapeObjectSetItems)[i]->m_pShapeUriName,strUri)==0)
			{
				for(int j=0;j<(int)(*m_ShapeObjectSetItems)[i]->m_ShapeObjects->size();j++)
				{
					if(::_tcsicmp((*(*m_ShapeObjectSetItems)[i]->m_ShapeObjects)[j]->m_pShapeName,strShape)==0)
						return (*(*m_ShapeObjectSetItems)[i]->m_ShapeObjects)[j];
				}
			}
		}
		return NULL;
	}
}}
