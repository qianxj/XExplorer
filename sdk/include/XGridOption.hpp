#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XGridOption :
		public CSvrbase
	{
	public:
		XGridOption(void);
		~XGridOption(void);
	public:
		static COLORREF stdTextColor;
		static int stdRowHeight;
		static int stdColumnWidth;
		static int stdColHeaderHeight;
		static int stdRowSelectorWidth;
		static COLORREF stdConerColor;
		static COLORREF stdBorderLineColor;
		static int stdGridLineWidth;

		static COLORREF stdColHeaderColor0;
		static COLORREF stdColHeaderColor1;
		static COLORREF stdRowSelecterColor0;
		static COLORREF stdRowSelecterColor1;

		static COLORREF stdGridColor0;
		static COLORREF stdGridColor1;
		static COLORREF stdGridColor2;
		static COLORREF stdGridColor3;
	};
}}}