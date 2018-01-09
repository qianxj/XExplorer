#pragma once
#include "msxml2.h"

namespace Hxsoft{ namespace XFrame
{
	class XBASE_API xbXml
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
		static LPTSTR	BStr2T(BSTR bstr);
	public:
		BSTR GetXml();
	public:
		IXMLDOMDocument2* GetXmlDoc();
	public:
		static xbXml * CreateInstance();
		static void ReleaseInstance(xbXml * p);
	public:
		static IXMLDOMSchemaCollection2* CreateSchemaCache();
		static bool ValidSchema(LPTSTR pSchema ,BSTR &bstr);
		static bool ValidSchema(xbXml &xml ,xbXml &schema ,BSTR &bstr);
	public:
		static int processElement(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaElement * pSchemaEle);
		static int processGroup(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaModelGroup * pGroup);
		static int processComplexType(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaComplexType * pType);
		static int processSimpleType(IXMLDOMDocument * pDocument, IXMLDOMElement * pParentEle,ISchemaType * pType);
		static int GetSchemaXml(LPTSTR pSchema,xbXml &x);
	};
}}
