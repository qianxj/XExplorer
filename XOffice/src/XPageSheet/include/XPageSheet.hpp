#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	class XPAGESHEET_API XPageSheet :
		public XOfficeSheet
	{
	public:
		XPageSheet(void);
		~XPageSheet(void);
	public:
		int FocusedSheet();
	};
}}}}
