#include "stdafx.h"
#include "xofbase.h"
#include "xoptionsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	int XOptionSvr::stdRowHeight = 24;
	int XOptionSvr::stdColumnWidth = 64;
	int XOptionSvr::stdColHeaderHeight = 24;
	int XOptionSvr::stdRowSelectorWidth = 32;
	COLORREF XOptionSvr::stdConerColor = RGB(0xcc,0xd8,0xcc);
	int XOptionSvr::stdGridLineWidth = 1;

	COLORREF XOptionSvr::stdColHeaderColor0 = RGB(0xcc - 5,0xd8 - 5,0xcc - 5);
	COLORREF XOptionSvr::stdColHeaderColor1 = RGB(0xcc + 25 > 0xff ? 0xff : 0xcc + 25 ,
		                              0xd8 + 25 > 0xff ? 0xff : 0xd8 + 25,
									  0xcc + 25 > 0xff ? 0xff : 0xcc + 25);
	COLORREF XOptionSvr::stdRowSelecterColor0 = RGB(0xcc - 5,0xd8 - 5,0xcc - 5);
	COLORREF XOptionSvr::stdRowSelecterColor1 = RGB(0xcc + 25 > 0xff ? 0xff : 0xcc + 25 ,
		                              0xd8 + 25 > 0xff ? 0xff : 0xd8 + 25,
									  0xcc + 25 > 0xff ? 0xff : 0xcc + 25);
	
	COLORREF XOptionSvr::stdGridColor0  = RGB(0xcc,0xd8,0xcc);
	COLORREF XOptionSvr::stdGridColor1 = RGB(0xcc,0xd8,0xcc);
	COLORREF XOptionSvr::stdGridColor2 = RGB(0xcc,0xd8,0xcc);
	COLORREF XOptionSvr::stdGridColor3 = RGB(0xcc,0xd8,0xcc);

}}}}