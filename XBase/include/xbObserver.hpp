#pragma once

namespace Hxsoft{ namespace XFrame
{
	class XBASE_API xbObserver
	{
	public:
		xbObserver(void);
		virtual ~xbObserver(void);
	public:
		int RemoveBy(class xbObject * pObject){return 0;};
	};
}}
