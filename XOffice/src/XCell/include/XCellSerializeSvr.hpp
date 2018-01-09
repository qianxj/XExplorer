#pragma once
#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class CGridDataSvr;
	class XCELL_API XCellSerializeSvr :public XOfficeSerializeSvr
	{
	public:
		XCellSerializeSvr(void);
	public:
		~XCellSerializeSvr(void);

	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr,int state = goNormalState);
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc);
	public:
		virtual int LoadSheetData20(IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr);
		virtual int LoadSheetTable(IXMLDOMElement * pSheetElement,XOfficeData * pDataSvr);
	public:
		virtual LPTSTR SerializeSheetDataToHtml(XOfficeData * pDataSv);

	};
}}}}
