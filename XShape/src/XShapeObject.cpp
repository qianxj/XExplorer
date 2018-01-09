#include "StdAfx.h"
#include "..\include\XShapeObject.hpp"

namespace Hxsoft{ namespace XFrame
{
	XShapeObject::XShapeObject(void):m_DrawObjects(new vector<XShapeDrawObject *>),m_pShapeName(NULL),m_nCssStyle(0),
		m_pShapeLabel(NULL)
	{
	}

	XShapeObject::~XShapeObject(void)
	{
		for(int i=0;i<(int)m_DrawObjects->size();i++)
		{
			if((*m_DrawObjects)[i])delete (*m_DrawObjects)[i];
		}
		m_DrawObjects->clear();
		if(m_pShapeName)delete m_pShapeName;
		if(m_pShapeLabel)delete m_pShapeLabel;
	}

	int XShapeObject::LoadShapeXml(LPTSTR pStrXml)
	{
		xbXml	xml;
		xml.LoadXml(pStrXml);
		IXMLDOMElement *pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int result = LoadShape(pElement);
		pElement->Release();
		return result;
	}

	int XShapeObject::LoadShape(IXMLDOMElement * m_pElement)
	{
		IXMLDOMNodeList * pList;
		IXMLDOMElement*  pElementEx;
		IXMLDOMNode * pNode;
		m_pElement->selectNodes(L"*",&pList);
		m_pShapeName = xbXml::GetStringAttribe(m_pElement,L"name");
		m_pShapeLabel = xbXml::GetStringAttribe(m_pElement,L"label");

		long	nLen;
		pList->get_length(&nLen);
		for(int i=0;i<nLen;i++)
		{
			XShapeDrawObject*	pObject = new XShapeDrawObject();
			pList->get_item(i,&pNode);
			pElementEx = xbXml::GetElement(pNode);
			pObject->LoadObject(pElementEx);
			m_DrawObjects->push_back(pObject);
			pElementEx->Release();
			pNode->Release();
		}
		pList->Release();

		return 0;
	}

	int XShapeObject::DoDraw(HDC hPaintDC,RECT rc,DWORD dwDrawMask)
	{
		for(int i=0;i<(int)m_DrawObjects->size();i++)
		{
			if((*m_DrawObjects)[i])
				(*m_DrawObjects)[i]->DoDraw(hPaintDC,rc,dwDrawMask);
		}
		return 1;
	}
	
	bool XShapeObject::GetHotHitedObject(int nIndex, XShapeDrawObject * & pObj,int & nIndexObj)
	{
		int ttlIndex=0;
		for(int i=0;i<(int)m_DrawObjects->size();i++)
		{
			ttlIndex += (int)(*m_DrawObjects)[i]->m_phtPoints->size();
			if(ttlIndex >= nIndex)
			{
				pObj = (*m_DrawObjects)[i];
				nIndexObj = nIndex - ( ttlIndex - (int)(*m_DrawObjects)[i]->m_phtPoints->size());
				return true;
			}
		}
		return false;
	}

	int XShapeObject::GetHotHitedIndex(XShapeDrawObject * pObj,int  nIndexObj)
	{
		int ttlIndex=0;
		for(int i=0;i<(int)m_DrawObjects->size();i++)
		{
			if((*m_DrawObjects)[i] != pObj)
			{
				ttlIndex += (int)(*m_DrawObjects)[i]->m_phtPoints->size();
			}else
				return ttlIndex +  nIndexObj;
		}

		return -1;
	}

	int XShapeObject::GetHotedPoints(const RECT &rect,vector<POINT>* pPoints)
	{
		POINT ptCur;
		ptCur.x = rect.left;
		ptCur.y = rect.right;

		vector<POINT>* ptPoints = new vector<POINT>;
		for(int i=0;i<(int)m_DrawObjects->size();i++)
		{
			if((*m_DrawObjects)[i])
			{
				ptPoints->clear();
				(*m_DrawObjects)[i]->CalcPoints(ptCur,rect,(*m_DrawObjects)[i]->m_pPoints,ptPoints);
				if(i>0 && ptPoints->size()>0)
					ptCur = (*ptPoints)[ptPoints->size() - 1];
				(*m_DrawObjects)[i]->GetHotedPoints(rect,ptCur,pPoints);
			}
		}
		if(ptPoints)delete ptPoints;
		return 1;
	}

	int XShapeObject::GetHotedArrows( vector<int>* pArrows)
	{
		for(int i=0;i<(int)m_DrawObjects->size();i++)
		{
			if((*m_DrawObjects)[i])
			{
				(*m_DrawObjects)[i]->GetHotedArrows(pArrows);
			}
		}
		return 1;
	}


	int XShapeObject::GetSelectPoints(const RECT &rect,vector<POINT>* pPoints)
	{
		POINT ptCur;
		ptCur.x = rect.left;
		ptCur.y = rect.right;

		vector<POINT>* ptPoints = new vector<POINT>;
		for(int i=0;i<(int)m_DrawObjects->size();i++)
		{
			if((*m_DrawObjects)[i])
			{
				ptPoints->clear();
				(*m_DrawObjects)[i]->CalcPoints(ptCur,rect,(*m_DrawObjects)[i]->m_pPoints,ptPoints);
				if(i>0 && ptPoints->size()>0)
					ptCur = (*ptPoints)[ptPoints->size() - 1];
				(*m_DrawObjects)[i]->GetSelectPoints(rect,ptCur,pPoints);
			}
		}
		if(ptPoints)delete ptPoints;
		return 1;
	}

}}
