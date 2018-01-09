#include "StdAfx.h"
#include "xfnode.hpp"
#include "xfNodeLayer.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNodeLayer::xfNodeLayer(void)
	{
		m_LayerFlow = LayerFlow_WE;
	}

	xfNodeLayer::~xfNodeLayer(void)
	{
	}
	int xfNodeLayer::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		return xfNode::ParseNode(pNodeEx,ParseChild);
	}
}}
