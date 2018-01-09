#pragma once
#include "msxml2.h"

namespace xsm
{
	class  xbXml
	{
	public:
		xbXml(void);
		~xbXml(void);
	public:
		IXMLDOMDocument2*	m_pXmlDoc;
	public:
		bool Load(LPCTSTR pStrFile);
		bool LoadXml(LPCTSTR pStrXml);
	public:
		static IXMLDOMElement * GetElement(IXMLDOMNode * pNode);
		static LPTSTR GetStringAttribe(IXMLDOMElement * pElement,LPCTSTR attrib);
	public:
		BSTR GetXml();
	public:
		static xbXml * CreateInstance();
		static void ReleaseInstance(xbXml * p);
	};
}
