#pragma once
#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XShow
{
	class XSHOW_API XShowSheetSvr :
		public XOffice::XOfficeSheetSvr
	{
	public:
		XShowSheetSvr(void);
		~XShowSheetSvr(void);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		virtual int SetFRect(RECT rc);
	};
}}}}
