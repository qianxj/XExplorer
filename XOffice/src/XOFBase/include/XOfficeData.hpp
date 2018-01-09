#pragma once

#include "svrbase.hpp"
#include "XOfficeStyleSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFFICE_API XOfficeData :
		public CSvrbase
	{
	public:
		XOfficeData(void);
		~XOfficeData(void);
	public:
		XOfficeStyleSvr * m_pStyleSvr;
	public:
		virtual XOfficeStyleSvr *  CreateStyleSvr();
	public:
		virtual int UpdateXml(IXMLDOMDocument2 * pXmlDoc);
		virtual int UpdateXml(IXMLElement * pXmlDoc);
		virtual int GetDataXml(BSTR bstr);
		virtual int OpenXml(IXMLDOMDocument2 * pXmlDoc);
		virtual int OpenXml(IXMLElement * pXmlDoc);
		virtual int OpenXml(BSTR bstr);
	};
}}}
