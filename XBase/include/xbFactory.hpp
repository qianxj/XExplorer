#pragma once

namespace Hxsoft{ namespace XFrame
{
	class xbFactory : public xbObject
	{
		xbObject * CreateInstance(GUID &guid);
	}
}}