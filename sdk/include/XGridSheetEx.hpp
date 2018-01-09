#pragma once
#include "xcellsheet.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class IGridReport;
	class  XCELL_API XGridSheetEx : public XOfficeSheet
	{
	public:
		XGridSheetEx(void);
		~XGridSheetEx(void);
	public:
		IGridReport * m_pGridReport;
		virtual xbObject*  GetInterface();
	};
}}}}
