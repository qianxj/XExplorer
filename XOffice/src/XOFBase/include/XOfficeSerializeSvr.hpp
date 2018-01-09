#pragma once
#include "svrbase.hpp"
#include "XOfficeData.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	#define  goDesignState		     0x100000
	#define  goRunState			     0x200000
	#define  goNormalState			 0x400000

	class XOFFICE_API XOfficeSerializeSvr :
		public CSvrbase
	{
	public:
		XOfficeSerializeSvr(void);
		~XOfficeSerializeSvr(void);
	public:
		LPTSTR m_pStrGuid;
		int	m_Ver;
	public:
		virtual int LoadSheetData(LPTSTR pStrUrl,IXMLDOMElement * pElement,XOfficeData * pDataSvr,int state = goNormalState )=0;
		virtual int SerializeSheetData(XOfficeData * pDataSvr, IXMLDOMElement * pElement, IXMLDOMDocument * pXmlDoc)=0;
	};
}}}
