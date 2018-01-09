#pragma once

#include "xframe.h"
#include "xcZoomer.hpp"

namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API IXZoomer : public IXFControl
	{
	public:
		IXZoomer();
		~IXZoomer();
	public:
		int GetMax();
		int GetPos();
	public:
		int SetMax(int nMax);
		int SetPos(int nPos);
	};
}}