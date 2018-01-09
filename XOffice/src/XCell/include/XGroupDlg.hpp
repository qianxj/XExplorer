#pragma once

#include "xframe.h"
#include "ContentSvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XGroupDlg :
		public xfWin
	{
	public:
		XGroupDlg(void);
		~XGroupDlg(void);
	public:
		virtual int OnXCommand(LPCTSTR pStrID,xfControl *pControl);
	public:
		virtual int OnOpen();
	public:
		LPTSTR m_pPreLastStr;
		LPTSTR m_pPreStr;
		LPTSTR m_pNewPreLastStr;
		LPTSTR m_pNewPreStr;
		bool m_bOk;
	};
}}}}
