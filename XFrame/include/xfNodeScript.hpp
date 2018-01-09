#pragma once

namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API xfNodeScript :
		public xfNode
	{
	public:
		xfNodeScript(void);
		~xfNodeScript(void);
	public: 
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true);
	public:
		LPTSTR m_pSrc;
	};
}}
