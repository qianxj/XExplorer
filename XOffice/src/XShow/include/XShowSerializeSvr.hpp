#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XShow
{
	class XSHOW_API XShowSerializeSvr :
		public XOffice::XOfficeSerializeSvr
	{
	public:
		XShowSerializeSvr(void);
		~XShowSerializeSvr(void);
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	};
}}}}
