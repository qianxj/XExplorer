#pragma once

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWordTextMeta
	{
	public:
		XWordTextMeta(void);
		~XWordTextMeta(void);
	public:
		static int GetFitedText(HDC hdc,LPTSTR pStrText,int nWidth);
	};
}}}}
