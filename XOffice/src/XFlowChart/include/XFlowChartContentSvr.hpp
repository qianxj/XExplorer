#pragma once

#include "xofbase.h"
#include "xshape.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	class XFLOWCHART_API XFlowChartContentSvr :
		public xbObject
	{
	public:
		XFlowChartContentSvr(void);
		~XFlowChartContentSvr(void);
	public:
		int	m_nIndexStyle;
		XShapeObjectSetItem*	m_pShapeSetItem;

		vector<class XFlowChartObject *>* m_pDrawObjects;
		vector<class XFlowChartLink *>* m_pDrawLinks;
	};
}}}}
