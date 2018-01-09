#include "stdafx.h"
#include "xcontrolmgr.hpp"
#include "commctrl.hpp"
#include "xapp.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		struct tagXClassRelInfo
		{
			LPTSTR pStrXClass;
			LPTSTR pStrCtrlClass;
			DWORD dwStyle;
			DWORD dwStyleEx;
		} * PXClassRelInfo;
		
		static tagXClassRelInfo XClassRelInfo[] = {
			{	_T("xlist"),	WC_LISTVIEW,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xtab"),	WC_TABCONTROL,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xtree"),	WC_TREEVIEW,	WS_VISIBLE|WS_CHILD|TVS_HASLINES|/*TVS_LINESATROOT|*/TVS_SHOWSELALWAYS|TVS_HASBUTTONS ,	0	},
			{	_T("xcombobox"),	WC_COMBOBOX,	WS_VISIBLE|WS_CHILD|CBS_DROPDOWNLIST  ,	0	},
			{	_T("xbutton"),	WC_BUTTON,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xtext"),	WC_STATIC,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xedit"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|WS_VSCROLL|ES_LEFT  | ES_MULTILINE,	0	},
			{	_T("xsedit"),	_T("Scintilla"),WS_CHILD|WS_VISIBLE|WS_TABSTOP,	0	},
			{	_T("xlistbox"),	WC_LISTBOX,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xscrollbar"),	WC_SCROLLBAR,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xlink"),	WC_LINK ,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xheader"),	WC_HEADER,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xtoolbar"),	TOOLBARCLASSNAME,	WS_VISIBLE|WS_CHILD|CCS_NOPARENTALIGN|CCS_NORESIZE|CCS_NODIVIDER|TBSTYLE_FLAT|TBSTYLE_TRANSPARENT|TBSTYLE_LIST,	TBSTYLE_EX_MIXEDBUTTONS|TBSTYLE_EX_DRAWDDARROWS 	},
			{	_T("xrebar"),	REBARCLASSNAME,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xtooltip"),	TOOLTIPS_CLASS,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xstatusbar"),	STATUSCLASSNAME,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xtrackbar"),	TRACKBAR_CLASS,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xupdown"),	UPDOWN_CLASS,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xprogress"),	PROGRESS_CLASS,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xmonthcal"),	MONTHCAL_CLASS,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xdatetimepick"),	DATETIMEPICK_CLASS,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xipaddress"),	WC_IPADDRESS,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xpager"),	WC_PAGESCROLLER,	WS_VISIBLE|WS_CHILD,	0	},
			{	_T("xmdiclient"),	_T("MDICLIENT"),WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL |WS_VISIBLE,	0	},
			{	_T("xtoolbutton"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xpropsheet"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xcell"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xwindesign"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xword"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xflowclass"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xflowgraph"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xworkflow"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xflowtable"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
			{	_T("xlayersheet"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} 
		};

		static int Lookup(LPCTSTR strXClass)
		{
			if(!strXClass)return -1;
			int cnt = sizeof(XClassRelInfo)/sizeof(XClassRelInfo[0]);
			for(int i=0;i<cnt;i++)
			{
				if(::_tcscmp(XClassRelInfo[i].pStrXClass,strXClass)==0) return i;
			}
			return -1;
		}
		LPTSTR XControlMgr::GetControlClass(LPCTSTR strXClass)
		{
			int index = Lookup(strXClass);
			if(index < 0) return NULL;
			return XClassRelInfo[index].pStrCtrlClass;
		}
		DWORD XControlMgr::GetControlDefStyle(LPCTSTR strXClass)
		{
			int index = Lookup(strXClass);
			if(index < 0) return NULL;
			return XClassRelInfo[index].dwStyle;
		}
		DWORD XControlMgr::GetControlDefStyleEx(LPCTSTR strXClass)
		{
			int index = Lookup(strXClass);
			if(index < 0) return NULL;
			return XClassRelInfo[index].dwStyleEx;
		}
	}
}