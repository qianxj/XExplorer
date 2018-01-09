#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API XBIReportSerializeSvr :
		public XOfficeSerializeSvr
	{
	public:
		XBIReportSerializeSvr(void);
		~XBIReportSerializeSvr(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	};
}}}}
