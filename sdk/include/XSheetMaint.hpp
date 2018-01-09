#pragma once
#include "xframe.h"
#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XSheetMaint :
		public xfWin
	{
	public:
		XSheetMaint(void);
		~XSheetMaint(void);
	public:
		int Create(POINT pt,LPARAM lParam = NULL);
		bool m_bCanDestroy;
		xfControl * m_pCtrl;
		vector<HTREEITEM> m_hItems;
	public:
		virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
		virtual int OnOpen();
		virtual int OnOpen(xfWin* pWin);
	public:
		bool EvtNotify(TEvent* pEvent,LPARAM lParam);
		bool EvtActive(TEvent* pEvent,LPARAM lParam);
	};
}}}
