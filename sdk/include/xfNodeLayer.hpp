#pragma once
#include "xfnodebox.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API xfNodeLayer :
		public xfNodeBox
	{
	public:
		xfNodeLayer(void);
		~xfNodeLayer(void);
	public:
		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
	};
}}
