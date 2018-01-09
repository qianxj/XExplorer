#pragma once

#include "xframe.h"
#include "xcScrollBox.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API IXScrollBox : public IXFControl
	{
	public:
		IXScrollBox();
		~IXScrollBox();
	public:
		void SetContent(LPTSTR strUrl, LPVOID pParam = 0);
	};
}}