#pragma once
#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame 
{
	class XFRAME_API IMail : xbObject
	{
	public:
		static void * LoadMessage(LPTSTR mailId);
	};
}}
