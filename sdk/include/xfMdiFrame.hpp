#pragma once

#include "xfwin.hpp"
namespace Hxsoft{ namespace XFrame
{
	class XFRAME_API xfMdiFrame :
		public  xfWin
	{
	public:
		xfMdiFrame(void);
		~xfMdiFrame(void);
	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);

	public:
		HWND m_hMdiClient;
	public:
		virtual int AddSheet(xfWin * pWin){return 1;}
		virtual int SelectSheet(int nIndex){return 1;}
		virtual int PreSelectSheet(int nIndex){return 1;}
	public:
		virtual HWND GetActiveSheet();

	};
}}