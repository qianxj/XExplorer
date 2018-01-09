#pragma once

#include "xofbase.h"
#include "xuxutil.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	class XProjectSerializeSvr :
		public XOfficeSerializeSvr
	{
	public:
		XProjectSerializeSvr(void);
		~XProjectSerializeSvr(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	};
}}}}
