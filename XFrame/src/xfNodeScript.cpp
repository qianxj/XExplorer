#include "StdAfx.h"
#include "xfnode.hpp"
#include "xfNodeScript.hpp"
#include "xfXPage.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNodeScript::xfNodeScript(void):m_pSrc(NULL)
	{
	}

	xfNodeScript::~xfNodeScript(void)
	{
		if(m_pSrc) delete m_pSrc;
	}
	int xfNodeScript::ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild)
	{
		m_pSrc = xbXml::GetStringAttribe(pNodeEx,_T("src"));
		GetXPage()->AddScript(this);
		return 1;
	}
}}
