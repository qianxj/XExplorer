#include "stdafx.h"
#include "xofbase.h"
#include "optionsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	COLORREF COptionSvr::stdTextColor = RGB(0,0,0);

	int COptionSvr::stdRowHeight = 20;
	int COptionSvr::stdColumnWidth = 64;
	int COptionSvr::stdColHeaderHeight = 20;
	int COptionSvr::stdRowSelectorWidth = 40;
	COLORREF COptionSvr::stdConerColor = RGB(225,229,238);//RGB(0xd8,0xe7,0xfc);
	COLORREF COptionSvr::stdBorderLineColor = RGB(159,171,187);//RGB(128,128,255);
	int COptionSvr::stdGridLineWidth = 1;

	COLORREF COptionSvr::stdColHeaderColor0 = RGB(0xcc - 5,0xd8 - 5,0xcc - 5);
	COLORREF COptionSvr::stdColHeaderColor1 = RGB(0xcc + 25 > 0xff ? 0xff : 0xcc + 25 ,
		                              0xd8 + 25 > 0xff ? 0xff : 0xd8 + 25,
									  0xcc + 25 > 0xff ? 0xff : 0xcc + 25);
	COLORREF COptionSvr::stdRowSelecterColor0 = RGB(0xcc - 5,0xd8 - 5,0xcc - 5);
	COLORREF COptionSvr::stdRowSelecterColor1 = RGB(0xcc + 25 > 0xff ? 0xff : 0xcc + 25 ,
		                              0xd8 + 25 > 0xff ? 0xff : 0xd8 + 25,
									  0xcc + 25 > 0xff ? 0xff : 0xcc + 25);
	
	COLORREF COptionSvr::stdGridColor0  = RGB(0xcc,0xd8,0xcc);
	COLORREF COptionSvr::stdGridColor1 = RGB(0xcc,0xd8,0xcc);
	COLORREF COptionSvr::stdGridColor2 = RGB(0xcc,0xd8,0xcc);
	COLORREF COptionSvr::stdGridColor3 = RGB(0xcc,0xd8,0xcc);

}}}}