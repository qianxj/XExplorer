#pragma once

namespace Hxsoft{ namespace XFrame
{
	class  XBASE_API xbObjectLog
	{
	public:
		xbObjectLog();
		~xbObjectLog();
	public:
		int Log(int level,LPTSTR message);
	};
}}
