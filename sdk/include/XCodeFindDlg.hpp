#pragma once

#include "xframe.h"
#include "ixsedit.hpp"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class XCodeFindDlg :
		public xfWin
	{
	public:
		XCodeFindDlg(void);
		~XCodeFindDlg(void);
	public:
		int OnOpen();
	public:
		IXSEdit * m_pEdit;
	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);

	};
}}}
