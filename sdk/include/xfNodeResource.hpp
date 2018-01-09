#pragma once

namespace Hxsoft{ namespace XFrame
{
	class xfResourceMgr;
	class XFRAME_API xfNodeResource :
		public xfNode
	{
	public:
		xfNodeResource(void);
		~xfNodeResource(void);
	public:
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
	public:
		xfResourceMgr * m_pResourceMgr;
	};
}}
