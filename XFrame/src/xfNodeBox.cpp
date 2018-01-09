#include "StdAfx.h"
#include "xfnode.hpp"
#include "xfNodeBox.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNodeBox::xfNodeBox(void)
	{
	}

	xfNodeBox::~xfNodeBox(void)
	{
	}
	int xfNodeBox::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		VARIANT	val;
		if(!pNodeEx) return -1;
		pNodeEx->getAttribute(L"layerflow",&val);

		if(val.vt==VT_NULL)
			if(m_pParentNode && (m_LayerFlow!=LayerFlow_WE && m_LayerFlow  != LayerFlow_NS)) m_LayerFlow = m_pParentNode->m_LayerFlow;
		::VariantClear(&val);

		return xfNode::ParseNode(pNodeEx,ParseChild);
	}
}}
