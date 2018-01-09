#include "StdAfx.h"
#include "..\include\XFlowChartDataSvr.hpp"
#include "XFlowChartContentSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowChartDataSvr::XFlowChartDataSvr(void):m_pContentSvr(NULL)
	{
	}

	XFlowChartDataSvr::~XFlowChartDataSvr(void)
	{
		if(m_pContentSvr) delete m_pContentSvr;
	}
}}}}
