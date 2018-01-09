#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class CCellProp :
	public xfWin
	{
	public:
		CCellProp(void);
		~CCellProp(void);
	public:
		virtual bool SetEventListons();
		virtual bool UnSetEventListons();
	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
		virtual int OnOpen();
	public:
		virtual bool GetClientRect(RECT *pRect);
		virtual HWND GetHWND();
	public:
		bool EvtEditTypeSelected(TEvent *pEvent ,LPARAM lParam);

	};
}}}}
