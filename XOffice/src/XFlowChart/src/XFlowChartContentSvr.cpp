#include "StdAfx.h"
#include "..\include\XFlowChartContentSvr.hpp"
#include "XFlowChartObject.hpp"
#include "XFlowChartLink.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowChartContentSvr::XFlowChartContentSvr(void):m_nIndexStyle(-1),m_pShapeSetItem(new XShapeObjectSetItem),
		m_pDrawObjects(new	vector<class XFlowChartObject *>),m_pDrawLinks(new vector<class XFlowChartLink *>)
	{
	}

	XFlowChartContentSvr::~XFlowChartContentSvr(void)
	{
		if(m_pShapeSetItem)  delete m_pShapeSetItem;
		for(int i=0;i<(int)(*m_pDrawObjects).size();i++)
		{
			if((*m_pDrawObjects)[i]) delete (*m_pDrawObjects)[i];
		}
		if(m_pDrawObjects)delete m_pDrawObjects;
		for(int i=0;i<(int)m_pDrawLinks->size();i++)
		{
			if((*m_pDrawLinks)[i]) delete (*m_pDrawLinks)[i];
		}
		if(m_pDrawLinks)delete m_pDrawLinks;
	}
}}}}
