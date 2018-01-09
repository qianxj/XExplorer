#include "StdAfx.h"
#include "XShapeObjectSetItem.hpp"
#include "xshapeobject.hpp"

namespace Hxsoft{ namespace XFrame
{
	XShapeObjectSetItem::XShapeObjectSetItem(void):m_ShapeObjects(new vector<XShapeObject *>),m_pShapeUriName(NULL)
	{
	}

	XShapeObjectSetItem::~XShapeObjectSetItem(void)
	{
		for(int i=0;i<(int)m_ShapeObjects->size();i++)
		{
			if((*m_ShapeObjects)[i])delete (*m_ShapeObjects)[i];
		}
		m_ShapeObjects->clear();
		if(m_pShapeUriName)delete m_pShapeUriName;
	}

	int XShapeObjectSetItem::LoadShapeSetItemXml(LPTSTR pStrXml)
	{
		xbXml	xml;
		xml.LoadXml(pStrXml);
		IXMLDOMElement *pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int result = LoadShapeSetItem(pElement);
		pElement->Release();
		return result;
	}

	int XShapeObjectSetItem::LoadShapeSetItem(IXMLDOMElement * m_pElement)
	{
		IXMLDOMNodeList * pList;
		IXMLDOMElement*  pElementEx;
		IXMLDOMNode * pNode;
		m_pElement->selectNodes(L"shape",&pList);
		m_pShapeUriName = xbXml::GetStringAttribe(m_pElement,L"uri");

		long	nLen;
		pList->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			XShapeObject*	pObject = new XShapeObject();
			pList->get_item(i,&pNode);
			pElementEx = xbXml::GetElement(pNode);
			pObject->LoadShape(pElementEx);
			m_ShapeObjects->push_back(pObject);
			pElementEx->Release();
			pNode->Release();
		}
		pList->Release();

		return 0;
	}

	XShapeObject * XShapeObjectSetItem::Find(LPTSTR strShape)
	{
		for(int j=0;j<(int)m_ShapeObjects->size();j++)
		{
			if(::_tcsicmp((*m_ShapeObjects)[j]->m_pShapeName,strShape)==0)
				return (*m_ShapeObjects)[j];
		}
		return NULL;
	}

}}
