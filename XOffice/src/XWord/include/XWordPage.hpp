#pragma once

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWordPage
	{
	public:
		XWordPage(void);
		~XWordPage(void);
	public:
		RECT m_rcPage;
	public:
		RECT m_rcPageMargin;
		RECT m_rcPageContent;
	public:
		RECT & GetPageRect();
	public:
		int GetLeft();
		int GetTop();
		int GetRight();
		int GetBottom();
	public:
		int GetWidth();
		int GetHeight();
	};
}}}}
