#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	class XPageSheetSerializeSvr :
		public XOfficeSerializeSvr
	{
	public:
		XPageSheetSerializeSvr(void);
		~XPageSheetSerializeSvr(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc); 
	};
}}}}
