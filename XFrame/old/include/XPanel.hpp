#pragma once
#include "xwin.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		
		class XFRAME_API XPanel :public XWin
		{
		public:
			XPanel(void){};
		public:
			~XPanel(void){};
		public:
			int GetMinSize(SIZE cs){return cs.cx=120;cs.cy=80 ;};
			int GetHopeSize(SIZE cs){return cs.cx=240;cs.cy=480; };
			int AdjustLayer(DWORD arrow, int cx,int cy){};
		};
	}
}
