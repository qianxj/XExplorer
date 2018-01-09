#pragma once
#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{

	class XREPORT_API XOptionSvr :public CSvrbase
	{
	public:
		XOptionSvr(void){};
		~XOptionSvr(void){};
	public:
		static int stdRowHeight;
		static int stdColumnWidth;
		static int stdColHeaderHeight;
		static int stdRowSelectorWidth;
		static COLORREF stdConerColor;
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
}}}}
