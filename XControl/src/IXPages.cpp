#include "stdafx.h"
#include "IXPages.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXPages::IXPages()
	{
		
	}
	IXPages::~IXPages()
	{

	}
	int IXPages::GetCurPage()
	{
		return ((xcPages *)m_pControl)->m_CurPage;
	}
	int IXPages::GetMaxPage()
	{
		return ((xcPages *)m_pControl)->m_MaxPage;
	}
	int IXPages::SetMaxPage(int mpage)
	{
		return ((xcPages *)m_pControl)->SetMaxPage(mpage);
	}
	int IXPages::SetCurPage(int cpage)
	{
		return ((xcPages *)m_pControl)->SetCurPage(cpage);
	}


}}