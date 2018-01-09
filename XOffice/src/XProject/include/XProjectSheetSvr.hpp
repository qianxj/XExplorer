#pragma once

#include "xofbase.h"
#include "xuxutil.h"
#include "XProjectDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	class XPROJECT_API XProjectSheetSvr :
		public XOfficeSheetSvr
	{
	public:
		XProjectSheetSvr(void);
		~XProjectSheetSvr(void);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	};
}}}}