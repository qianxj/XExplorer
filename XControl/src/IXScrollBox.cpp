#include "stdafx.h"
#include "IXScrollBox.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXScrollBox::IXScrollBox()
	{
		
	}
	IXScrollBox::~IXScrollBox()
	{

	}
	void IXScrollBox::SetContent(LPTSTR strUrl, LPVOID pParam)
	{
		((xcScrollBox *)m_pControl)->SetContent(strUrl, pParam);
	}

}}