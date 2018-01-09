#include "StdAfx.h"
#include "XWordPage.hpp"
#include "xuxutil.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	extern XUtil::Css::tagCssData 	_CssData;
	XWordPage::XWordPage(void)
	{
		::SetRect(&m_rcPage,0,0, MulDiv(210,100*_CssData.nXPixelPerInch,2540),
		MulDiv(297,100*_CssData.nXPixelPerInch,2540));

		::SetRect(&m_rcPageMargin,60,60,60,60);
		m_rcPageContent.left = m_rcPage.left + m_rcPageMargin.left;
		m_rcPageContent.right = m_rcPage.right - m_rcPageMargin.right;
		m_rcPageContent.top = m_rcPage.top + m_rcPageMargin.top;
		m_rcPageContent.bottom = m_rcPage.bottom - m_rcPageMargin.bottom;
	}

	XWordPage::~XWordPage(void)
	{
	}

	RECT & XWordPage::GetPageRect()
	{
		return m_rcPage;
	}
	int XWordPage::GetLeft()
	{
		return m_rcPage.left;
	}
	int XWordPage::GetTop()
	{
		return m_rcPage.top;
	}
	int XWordPage::GetRight()
	{
		return m_rcPage.right;
	}
	int XWordPage::GetBottom()
	{
		return m_rcPage.bottom;
	}
	
	int XWordPage::GetWidth()
	{
		return m_rcPage.right - m_rcPage.left + 1;
	}

	int XWordPage::GetHeight()
	{
		return m_rcPage.bottom - m_rcPage.top + 1;
	}

}}}}
