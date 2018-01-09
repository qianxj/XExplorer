#pragma once

namespace Hxsoft
{
	namespace XFrame
	{
		class XFRAME_API XControlMgr
		{
		public:
			static LPTSTR GetControlClass(LPCTSTR strXClass);
			static DWORD	GetControlDefStyle(LPCTSTR strXClass);
			static DWORD	GetControlDefStyleEx(LPCTSTR strXClass);
		};
	}
}