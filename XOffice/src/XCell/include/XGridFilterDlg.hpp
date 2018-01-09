#pragma once

#include "xofbase.h"
#include "XGridHeaderEx.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XGridFilterDlg : public xfWin
	{
	public:
		XGridFilterDlg(void);
		~XGridFilterDlg(void);
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
		virtual int OnOpen();
	public:
		bool EvtRadioClick(TEvent* pEvent,LPARAM lParam);
	public:
		LPTSTR SetFilterStr(LPTSTR pColumn, LPTSTR pOp, LPTSTR pText,bool bind = false);
	};
}}}}