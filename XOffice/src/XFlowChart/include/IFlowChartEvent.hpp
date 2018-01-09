#pragma once

#include <xframe.h>
#include <xofbase.h>
#include "XFlowChartObject.hpp"
#include "XFlowSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	class XFLOWCHART_API IFlowChartEvent : public xbObserver
	{
	public:
		IFlowChartEvent(void);
		~IFlowChartEvent(void);
	public:
		virtual int DoubleCilcked(XFlowSheetSvr * pSheetSvr ,XFlowChartObject * pObject)=0;
	};
}}}}
