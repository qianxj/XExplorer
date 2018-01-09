#pragma once

#include "xofbase.h"
#include "xshape.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	class XFLOWCHART_API XFlowChartDataSvr :
		public XOfficeData
	{
	public:
		XFlowChartDataSvr(void);
		~XFlowChartDataSvr(void);
	public:
		class XFlowChartContentSvr * m_pContentSvr;
	};
}}}}
