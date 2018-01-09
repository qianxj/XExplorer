#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API XBIReportSheet :
		public XOfficeSheet
	{
	public:
		XBIReportSheet(void);
		~XBIReportSheet(void);
	public:
		virtual xbObject*  GetInterface();
	};
}}}}