#pragma once
#include "xframe.h"
#include "xofbase.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	class XFLOWCHART_API XFlowSheet :
		public XOfficeSheet
	{
	public:
		XFlowSheet(void);
		~XFlowSheet(void);
	public:
		virtual xbObject*  GetInterface();
	};
}}}}
