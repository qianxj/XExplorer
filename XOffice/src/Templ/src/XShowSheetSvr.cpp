#include "StdAfx.h"
#include "..\include\XShowSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XShow
{
	XShowSheetSvr::XShowSheetSvr(void)
	{
	}

	XShowSheetSvr::~XShowSheetSvr(void)
	{
	}

	int XShowSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRec)
	{
		return 0;
	}
	int XShowSheetSvr::SetFRect(RECT rc)
	{
		XOfficeSheetSvr::SetFRect(rc);
		return 0;
	}

}}}}
