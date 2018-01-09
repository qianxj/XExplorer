// xcell.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "xbxbase.h"
#include "xframe.h"
#include "xcellsheet.hpp" 
#include "xgridsheetex.hpp"
#include "xbireportsheet.hpp"
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
	{	_T("xcell"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	},
	{	_T("xgridex"),	XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	},
	{	_T("xbireport"),XFrameWndClass,WS_CHILD | WS_VISIBLE,	0	}
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
		if(::_tcsicmp(pStrObject,_T("xcell"))==0)
		{
			pxbObject = new XOffice::XCell::XCellSheet();
		}else if(::_tcsicmp(pStrObject,_T("xgridex"))==0)
		{
			pxbObject = new XOffice::XCell::XGridSheetEx();
		}else if(::_tcsicmp(pStrObject,_T("xbireport"))==0)
		{
			pxbObject = new XOffice::XCell::XBIReportSheet();
		}else
			return false;
		((xfControl *) pxbObject)->m_dwExStyle = XClassRelInfo[nIndex].dwStyleEx;
		((xfControl *) pxbObject)->m_dwStyle = XClassRelInfo[nIndex].dwStyle;
		((xfControl *) pxbObject)->m_pszWinClass = XClassRelInfo[nIndex].pStrCtrlClass;
	}else
		return false;
	return true;
}

HMODULE hCellModula = NULL;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	hCellModula = hModule;
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
