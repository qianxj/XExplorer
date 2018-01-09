#pragma once

namespace Hxsoft{ namespace XFrame
{
	class xfNodeResource;
	class XFRAME_API xfNodeXPage :
		public xfNodeWin
	{
	public:
		xfNodeXPage(void);
		~xfNodeXPage(void);
	public:
		LPTSTR	m_pStrXPage;	//xpage
		LPTSTR	m_pStrUri;		//Uri
		LPTSTR	m_pStrXsl;		//Xsl
	public:
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
		int SuperParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
		static unsigned int __stdcall DoParseNode(void *pV) ;
	};
}}
