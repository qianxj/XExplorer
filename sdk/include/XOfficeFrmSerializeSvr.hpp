#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOfficeFrmSerializeSvr :
		public XOfficeSerializeSvr
	{
	public:
		XOfficeFrmSerializeSvr(void);
		~XOfficeFrmSerializeSvr(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	};
}}}
