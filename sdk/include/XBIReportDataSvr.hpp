#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API XBIReportDataSvr :
		public XOfficeData
	{
	public:
		XBIReportDataSvr(void);
		~XBIReportDataSvr(void);
	public:
		std::vector<xbObject* >* m_pObjects;
	};
}}}}