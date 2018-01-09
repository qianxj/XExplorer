#pragma once

#include "xframe.h"
#include "xcPages.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API IXPages : public IXFControl
	{
	public:
		IXPages();
		~IXPages();
	public:
		int SetMaxPage(int mpage);
		int SetCurPage(int cpage);
		int GetMaxPage();
		int GetCurPage();
	};
}}