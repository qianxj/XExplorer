#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class XREPORT_API XReportSheet :
		public XOfficeSheet
	{
	public:
		XReportSheet(void);
		~XReportSheet(void);
	public:
		virtual xbObject * QueryItem(LPTSTR pItem);
		virtual xbObject*  GetInterface();
	};

}}}}
