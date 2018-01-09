// XControl.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "commctrl.h"
#include "xbxbase.h"
#include "xframe.h"
#include "xctoolbutton.hpp"
#include "xctreectrl.hpp"
#include "xcWCControl.hpp"
#include "xcSpliterBar.hpp"
#include "xclayersheet.hpp"
#include "xclabel.hpp"
#include "xcPropSheet.hpp"
#include "xcShapeIcon.hpp"
//#include "xcAgentEdit.hpp"
#include "xcSourceEdit.hpp"
#include "xcOleControlContain.hpp"
#include "XSciLexer\xsedit.hpp" 
#include "xcimageview.hpp"
#include "xcSHtml.hpp"
#include "xcimage.hpp"
#include "xczoomer.hpp"
#include "xcViewd3d.hpp"
#include "xcFile.hpp"
#include "xcPages.hpp"
#include "xcScrollBox.hpp"
#include "xcSideBar.hpp"

using namespace Hxsoft::XFrame;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C"
{
	bool WINAPI CreateInstance(LPTSTR pStrObject,xbObject * &pxbObject);
}

struct tagXClassRelInfo
{
	LPTSTR pStrXClass;
	LPTSTR pStrCtrlClass;
	DWORD dwStyle;
	DWORD dwStyleEx;
} * PXClassRelInfo;

static tagXClassRelInfo XClassRelInfo[] = {
	{	_T("xlistviewl"),	WC_LISTVIEW,	WS_VISIBLE|WS_CHILD|LVS_ICON|LVS_SHOWSELALWAYS ,	0	},
	{	_T("xlistview"),	WC_LISTVIEW,	WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS ,	0	},
	{	_T("xtab"),	WC_TABCONTROL,	WS_VISIBLE|WS_CHILD,	0	},
	{	_T("xtree"),	WC_TREEVIEW,	WS_VISIBLE|WS_CHILD|TVS_HASLINES|/*TVS_LINESATROOT|*/TVS_SHOWSELALWAYS|TVS_HASBUTTONS ,	0	},
	{	_T("xcombobox"),	WC_COMBOBOX,	WS_VISIBLE|WS_CHILD|CBS_DROPDOWN|CBS_AUTOHSCROLL|WS_VSCROLL|WS_HSCROLL  ,	0	},
	{	_T("xbutton"),	WC_BUTTON,	WS_VISIBLE|WS_CHILD|BS_NOTIFY,	0	},
	{	_T("xcheckbox"),	WC_BUTTON ,	WS_VISIBLE|WS_CHILD|BS_CHECKBOX|BS_AUTOCHECKBOX,	0	},
	{	_T("xradiobutton"),	WC_BUTTON,	WS_VISIBLE|WS_CHILD|/*BS_RADIOBUTTON|*/BS_AUTORADIOBUTTON,	0	},
	{	_T("xtext"),	WC_STATIC,	WS_VISIBLE|WS_CHILD,	0	},
	{	_T("xedit"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|ES_LEFT|ES_AUTOHSCROLL,	0	},
	{	_T("xuedit"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|ES_LEFT|ES_AUTOHSCROLL|ES_UPPERCASE,	0	},
	{	_T("xaedit"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|ES_LEFT,	0	},
	{	_T("xmedit"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|WS_VSCROLL|ES_LEFT|ES_AUTOVSCROLL | ES_MULTILINE,	0	},
	//{	_T("xsedit"),	XFrameWndClass,	WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE,	0	},
	//{	_T("xsedit"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL|ES_LEFT  | ES_MULTILINE,	0	},
	{	_T("xsedit"),	STR_SCINTILLAWND,WS_CHILD|WS_VISIBLE|WS_TABSTOP,	0	},
	{	_T("xseditex"),	STR_SCINTILLAWND,WS_CHILD|WS_VISIBLE|WS_TABSTOP,	0	},
	{	_T("xlistbox"),	WC_LISTBOX,	WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL|LBS_NOTIFY ,	0	},
	{	_T("xscrollbar"),	WC_SCROLLBAR,	WS_VISIBLE|WS_CHILD|SBS_VERT,	0	},
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
	{	_T("xdatetimepick"),	DATETIMEPICK_CLASS,	WS_VISIBLE|WS_CHILD|DTS_SHOWNONE|DTS_SHORTDATECENTURYFORMAT ,	0	},
	{	_T("xipaddress"),	WC_IPADDRESS,	WS_VISIBLE|WS_CHILD,	0	},
	{	_T("xpager"),	WC_PAGESCROLLER,	WS_VISIBLE|WS_CHILD,	0	},
	{	_T("xcolumn"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|ES_LEFT,	0	},
	{	_T("xpassword"),	WC_EDIT,	WS_VISIBLE|WS_CHILD|ES_LEFT|ES_PASSWORD,	0	},
	{	_T("xmdiclient"),	_T("MDICLIENT"),WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL |WS_VISIBLE,	0	},
	{	_T("xtoolbutton"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xspliterbar"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xpropsheet"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xcell"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xwindesign"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xword"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xflowclass"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xflowgraph"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xworkflow"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xflowtable"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xlayersheet"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xlabel"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("xolecontrol"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	} ,
	{	_T("ximageview"),	XFrameWndClass,WS_CHILD | WS_VISIBLE ,	0	} ,
	{	_T("xshtml"),	XFrameWndClass,WS_CHILD | WS_VISIBLE ,	0	} ,
	{	_T("xshapeview"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	}, 
	{	_T("xzoomer"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	}, 
	{	_T("ximage"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	},
	{	_T("xview3d"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	},
	{	_T("xfile"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	},
	{	_T("xpages"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	},
	{	_T("xscrollbox"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	},
	{	_T("xsidebar"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	}
};

static int Lookup(LPCTSTR strXClass)
{
	if(!strXClass)return -1;
	int cnt = sizeof(XClassRelInfo)/sizeof(XClassRelInfo[0]);
	for(int i=0;i<cnt;i++)
	{
		if(::_tcsicmp(XClassRelInfo[i].pStrXClass,strXClass)==0) return i;
	}
	return -1;
}
LPTSTR GetControlClass(LPCTSTR strXClass)
{
	int index = Lookup(strXClass);
	if(index < 0) return NULL;
	return XClassRelInfo[index].pStrCtrlClass;
}
DWORD GetControlDefStyle(LPCTSTR strXClass)
{
	int index = Lookup(strXClass);
	if(index < 0) return NULL;
	return XClassRelInfo[index].dwStyle;
}
DWORD GetControlDefStyleEx(LPCTSTR strXClass)
{
	int index = Lookup(strXClass);
	if(index < 0) return NULL;
	return XClassRelInfo[index].dwStyleEx;
}

bool WINAPI CreateInstance(LPTSTR pStrObject,xbObject * &pxbObject)
{
	int nIndex = Lookup(pStrObject);

	if(nIndex>=0)
	{
		/*if(::_tcsicmp(pStrObject,_T("xaedit"))==0)
		{
			pxbObject = new xcAgentEdit();
		}else*/ if(::_tcsicmp(pStrObject,_T("xsedit"))==0)
		{
			//pxbObject = new xcSourceEdit();
			pxbObject = new XSEdit();
		}else if(::_tcsicmp(pStrObject,_T("xseditex"))==0)
		{
			pxbObject = new XSEdit();
		}
		else if(::_tcsicmp(pStrObject,_T("xtree"))==0)
		{
			pxbObject = new xcTreeCtrl();
		}else if(::_tcsicmp(pStrObject,_T("xtoolbutton"))==0)
			pxbObject =  new xcToolButton();
		else if(::_tcsicmp(pStrObject,_T("xspliterbar"))==0)
			pxbObject =  new xcSpliterBar();
		else if(::_tcsicmp(pStrObject,_T("xlayersheet"))==0)
			pxbObject =  new xcLayerSheet();
		else if(::_tcsicmp(pStrObject,_T("xlabel"))==0)
			pxbObject =  new xcLabel();
		else if(::_tcsicmp(pStrObject,_T("xpropsheet"))==0)
			pxbObject =  new xcPropSheetCtrl();
		else if(::_tcsicmp(pStrObject,_T("xolecontrol"))==0)
			pxbObject =  new xcOleControlContain();
		else if(::_tcsicmp(pStrObject,_T("xshapeview"))==0)
			pxbObject =  new xcShapeIcon();
		else if(::_tcsicmp(pStrObject,_T("ximageview"))==0)
			pxbObject =  new xcImageView();
		else if(::_tcsicmp(pStrObject,_T("xshtml"))==0)
			pxbObject =  new xcSHtml();
		else if(::_tcsicmp(pStrObject,_T("ximage"))==0)
			pxbObject =  new xcImage();
		else if(::_tcsicmp(pStrObject,_T("xzoomer"))==0)
			pxbObject =  new xcZoomer();
		else if(::_tcsicmp(pStrObject,_T("xview3d"))==0)
			pxbObject =  new xcViewd3d();
		else if(::_tcsicmp(pStrObject,_T("xfile"))==0)
			pxbObject =  new xcFile();
		else if(::_tcsicmp(pStrObject,_T("xpages"))==0)
			pxbObject =  new xcPages();		
		else if(::_tcsicmp(pStrObject,_T("xscrollbox"))==0)
			pxbObject =  new xcScrollBox();		
		else if(::_tcsicmp(pStrObject,_T("xsidebar"))==0)
			pxbObject =  new xcSideBar();
		else
		{
			pxbObject = new xcWCControl();
		}
		if(pxbObject)
		{
			((xfControl *) pxbObject)->m_dwExStyle = XClassRelInfo[nIndex].dwStyleEx;
			((xfControl *) pxbObject)->m_dwStyle = XClassRelInfo[nIndex].dwStyle;
			((xfControl *) pxbObject)->m_pszWinClass = XClassRelInfo[nIndex].pStrCtrlClass;
		}
	}else
		return false;
	return true;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
