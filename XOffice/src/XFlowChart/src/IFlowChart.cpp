#include "StdAfx.h"
#include "iflowchart.hpp"
#include "xflowsheetsvr.hpp"
#include "xflowsheet.hpp"
#include "xflowchartdatasvr.hpp"
#include "xflowchartcontentsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	IFlowChart::IFlowChart():m_pFlowChart(NULL)
	{
	}
	IFlowChart::~IFlowChart()
	{
	}

	XFlowChartObject * IFlowChart::GetSelectedObject()
	{
		return ((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->GetHitedObject();
	}

	int IFlowChart::GetObjectCount()
	{
		if(!(dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr)return 0;

		vector<class XFlowChartObject *>* pObjects = (dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawObjects;
		if(!pObjects) return 0;
		return (int)pObjects->size();
	}

	XFlowChartObject* IFlowChart::GetObjectByIndex(int nIndex)
	{
		if(!(dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr)return NULL;
		vector<class XFlowChartObject *>* pObjects = (dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawObjects;
		if(!pObjects) return NULL;
		if(nIndex >= (int)pObjects->size()) return NULL;
		if(nIndex < 0) return NULL;
		return (*pObjects)[nIndex];
	}

	int IFlowChart::GetLinkCount()
	{
		if(!(dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr)return 0;
		vector<class XFlowChartLink *>* pLinks = (dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawLinks;
		if(!pLinks) return 0;
		return (int)pLinks->size();
	}

	XFlowChartLink* IFlowChart::GetLinkbyIndex(int nIndex)
	{
		if(!(dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr)return NULL;
		vector<class XFlowChartLink *>* pLinks = (dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawLinks;
		if(!pLinks) return NULL;
		if(nIndex >= (int)pLinks->size()) return NULL;
		if(nIndex < 0) return NULL;
		return (*pLinks)[nIndex];
	}

	bool IFlowChart::RemoveObject(XFlowChartObject * pObject,bool bDeletedLink)
	{
		if(!(dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr) return false;
		vector<class XFlowChartObject *>* pObjects = (dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawObjects;
		if(!pObjects) return false;
		bool bDeleted = false;
		for(int i=0;i<(int)pObjects->size();i++)
		{
			if((*pObjects)[i]==pObject)
			{
				pObjects->erase(pObjects->begin() + i);
				bDeleted = true;
				break;
			}
		}

		if(bDeleted)
		{
			vector<class XFlowChartLink *>* pLinks = (dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawLinks;
			if(pLinks)
			{
				for(int i=(int)pLinks->size() - 1;i>=0; i--)
				{
					if((*pLinks)[i]->m_pStartObject==pObject)
					{
						if(bDeletedLink)
						{
							delete (*pLinks)[i];
							pLinks->erase(pLinks->begin() + i);
							continue;
						}else
							(*pLinks)[i]->m_pStartObject = NULL;
					}
					if((*pLinks)[i]->m_pEndObject==pObject)
					{
						if(bDeletedLink)
						{
							delete (*pLinks)[i];
							pLinks->erase(pLinks->begin() + i);
							continue;
						}else
							(*pLinks)[i]->m_pEndObject = NULL;
					}
				}
			}
			delete pObject;
			::InvalidateRect(m_pFlowChart->GetHWND(),NULL,false);

			return true;
		}

		return false;
	}

	XFlowChartObject * IFlowChart::AddObject(LPTSTR pShapeType,LPTSTR pCaption,LPTSTR pUrl,RECT * pRect)
	{
		XFlowChartDataSvr * pDataSvr = dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr);
		
		if(!pDataSvr->m_pContentSvr)
			pDataSvr->m_pContentSvr = new XFlowChartContentSvr();

		RECT rt;
		if(pRect)
			rt = *pRect;
		else
		{
			POINT pt = ((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_TopLeft;
			::SetRect(&rt,pt.x+ 10 ,pt.y + 10, pt.x + 10 + 100,pt.y + 10 + 32);
		}
		
		IXMLDOMElement* pElement;
		xbXml xml;
		xml.m_pXmlDoc->createElement(L"shape",&pElement);
		pElement->setAttribute(L"refname",(_variant_t)pShapeType);
		pElement->setAttribute(L"startx",(_variant_t)rt.left);
		pElement->setAttribute(L"starty",(_variant_t)rt.top);
		pElement->setAttribute(L"endx",(_variant_t)_T("E100"));
		pElement->setAttribute(L"endy",(_variant_t)_T("E32"));
		pElement->setAttribute(L"caption",(_variant_t)pCaption);

		XFlowChartObject * pShape =  new XFlowChartObject();
		pShape->LoadObject(pElement);
		pElement->Release();
		pShape->m_pDataSvr = pDataSvr;

		if(!pDataSvr-> m_pContentSvr->m_pDrawObjects)
			pDataSvr-> m_pContentSvr->m_pDrawObjects = new vector<class XFlowChartObject *>;
		(*pDataSvr-> m_pContentSvr->m_pDrawObjects).push_back(pShape);
		pShape->SetFRect(rt);

		if(pUrl && pUrl[0])
			pShape->SetUrl(pUrl);

		::InvalidateRect(m_pFlowChart->GetHWND(),NULL,false);
		return pShape;
	}

	bool IFlowChart::SetobjectUrl(XFlowChartObject * pObject,LPTSTR pUrl)
	{
		if(pObject)
			return pObject->SetUrl(pUrl);
		else
			return false;
	}
	bool IFlowChart::SetobjectCaption(XFlowChartObject * pObject,LPTSTR pCaption)
	{
		if(pCaption)
			return pObject->SetCaption(pCaption);
		else
			return false;
	}
	
	bool IFlowChart::SelectedObject(XFlowChartObject * pObject)
	{
		((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->SelectedObject(pObject);
		return true;
	}
	
	XFlowChartLink * IFlowChart::AddLink(XFlowChartObject * pFrom,XFlowChartObject * pTo,bool bDoubleArrow)
	{
		XFlowChartLink * pLink;
		pLink = new XFlowChartLink;

		pLink->m_pStartObject = pFrom;
		pLink->m_nStartIndex = 1;
		pLink->m_ptStart = (*pFrom->m_phtPoints)[pLink->m_nStartIndex];

		pLink->m_pEndObject = pTo;
		pLink->m_nEndIndex = 3;
		pLink->m_ptEnd = (*pTo->m_phtPoints)[pLink->m_nEndIndex];

		vector<XFlowChartLink *>* pLinks = (dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawLinks;
		if(!pLinks)
		{
			pLinks = new vector<class XFlowChartLink *>;
			(dynamic_cast<XFlowChartDataSvr *>(((XFlowSheetSvr *)m_pFlowChart->m_pSheetSvr)->m_pDataSvr))->m_pContentSvr->m_pDrawLinks = pLinks;
		}
		pLinks->push_back(pLink);
		pLink->AutoCalcPoints();

		return pLink;
	}
	XFlowChartObject * IFlowChart::GetObjectAt(POINT pt)
	{
		return NULL;
	}

	int IFlowChart::Save(IXMLDOMElement * pElement)
	{
		if(!pElement) return -1;
		IXMLDOMDocument * pXmlDoc;
		pElement->get_ownerDocument(&pXmlDoc);
		m_pFlowChart->m_pSheetSvr->m_pSerializeSvr->SerializeSheetData(m_pFlowChart->m_pSheetSvr->m_pDataSvr,pElement,pXmlDoc);
		pXmlDoc->Release();
		return 1;
	}
	int IFlowChart::Load(IXMLDOMElement * pElement)
	{
		if(!pElement) return -1;
		XOffice::XFlowChart::XFlowChartDataSvr * pDataSvr = new XOffice::XFlowChart::XFlowChartDataSvr();
		m_pFlowChart->m_pSheetSvr->m_pSerializeSvr->LoadSheetData(NULL,pElement,pDataSvr);

		XOffice::XFlowChart::XFlowChartDataSvr * pOldDataSvr = (XOffice::XFlowChart::XFlowChartDataSvr *)m_pFlowChart->m_pSheetSvr->m_pDataSvr;
		((XOffice::XFlowChart::XFlowSheetSvr*)m_pFlowChart->m_pSheetSvr)->Clear();
		m_pFlowChart->m_pSheetSvr->m_pDataSvr = pDataSvr;
		if(pOldDataSvr)delete pOldDataSvr;

		::InvalidateRect(m_pFlowChart->m_hWnd,NULL,false);

		return 1;
	}

}}}}