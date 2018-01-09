#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	class XFLOWCHART_API XFlowChartSerializeSvr :
		public XOfficeSerializeSvr
	{
	public:
		XFlowChartSerializeSvr(void);
		~XFlowChartSerializeSvr(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	};
}}}}
