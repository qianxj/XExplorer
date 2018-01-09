#pragma once

namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API xfNodeBox :
		public xfNode
	{
	public:
		xfNodeBox(void);
		~xfNodeBox(void);
	public: 
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
	};
}}
