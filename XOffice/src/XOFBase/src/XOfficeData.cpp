#include "StdAfx.h"
#include "..\include\XOfficeData.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeData::XOfficeData(void)
	{
		m_pStyleSvr = CreateStyleSvr();
	}

	XOfficeData::~XOfficeData(void)
	{
		if(m_pStyleSvr) delete m_pStyleSvr;
	}
	XOfficeStyleSvr * XOfficeData::CreateStyleSvr()
	{
		return new XOfficeStyleSvr();
	}

	int XOfficeData::UpdateXml(IXMLDOMDocument2 * pXmlDoc){ return 0;}
	int XOfficeData::UpdateXml(IXMLElement * pXmlDoc){ return 0;}
	int XOfficeData::GetDataXml(BSTR bstr){ return 0;}
	int XOfficeData::OpenXml(IXMLDOMDocument2 * pXmlDoc){ return 0;}
	int XOfficeData::OpenXml(IXMLElement * pXmlDoc){ return 0;}
	int XOfficeData::OpenXml(BSTR bstr){ return 0;}

}}}
