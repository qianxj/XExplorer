#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class XREPORT_API XReportSerializeSvr :
		public XOffice::XOfficeSerializeSvr
	{
	public:
		XReportSerializeSvr(void);
		~XReportSerializeSvr(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	};
}}}}
