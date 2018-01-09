#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWordSerializeSvrEx :
		public XOfficeSerializeSvr
	{
	public:
		XWordSerializeSvrEx(void);
		~XWordSerializeSvrEx(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	};
}}}}
