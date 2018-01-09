#pragma once

#include "systypes.h"
#include "sysdep.h"
#include "xbxbase.h"
#include <vector>

namespace Hxsoft{ namespace XFrame
{
	class xfNetwork :
		public xbObject
	{
	public:
		xfNetwork(void);
		~xfNetwork(void);

	public:
		std::vector<class xfConnect * > m_connects;
	};
}}
