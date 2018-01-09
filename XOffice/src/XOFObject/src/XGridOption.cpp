#include "StdAfx.h"
#include "..\include\XGridOption.hpp"

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	COLORREF XGridOption::stdTextColor = RGB(0,0,0);

	int XGridOption::stdRowHeight = 24;
	int XGridOption::stdColumnWidth = 64;
	int XGridOption::stdColHeaderHeight = 24;
	int XGridOption::stdRowSelectorWidth = 28;
	COLORREF XGridOption::stdConerColor = RGB(0xd8,0xe7,0xfc);//RGB(0xcc,0xd8,0xcc);
	COLORREF XGridOption::stdBorderLineColor = RGB(0x00,0x00,0x00);
	int XGridOption::stdGridLineWidth = 1;

	COLORREF XGridOption::stdColHeaderColor0 = RGB(0xcc - 5,0xd8 - 5,0xcc - 5);
	COLORREF XGridOption::stdColHeaderColor1 = RGB(0xcc + 25 > 0xff ? 0xff : 0xcc + 25 ,
		                              0xd8 + 25 > 0xff ? 0xff : 0xd8 + 25,
									  0xcc + 25 > 0xff ? 0xff : 0xcc + 25);
	COLORREF XGridOption::stdRowSelecterColor0 = RGB(0xcc - 5,0xd8 - 5,0xcc - 5);
	COLORREF XGridOption::stdRowSelecterColor1 = RGB(0xcc + 25 > 0xff ? 0xff : 0xcc + 25 ,
		                              0xd8 + 25 > 0xff ? 0xff : 0xd8 + 25,
									  0xcc + 25 > 0xff ? 0xff : 0xcc + 25);
	
	COLORREF XGridOption::stdGridColor0  = RGB(0xcc,0xd8,0xcc);
	COLORREF XGridOption::stdGridColor1 = RGB(0xcc,0xd8,0xcc);
	COLORREF XGridOption::stdGridColor2 = RGB(0xcc,0xd8,0xcc);
	COLORREF XGridOption::stdGridColor3 = RGB(0xcc,0xd8,0xcc);

	XGridOption::XGridOption(void)
	{
	}

	XGridOption::~XGridOption(void)
	{
	}
}}}