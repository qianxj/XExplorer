// XFrame.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "xbxbase.h"
#include "ixnode.hpp"
#include "ixpage.hpp"
#include "ixfwin.hpp"
#include "ixaserver.hpp"
#include "ixaserverarg.hpp"
#include "ixfcontrol.hpp"
#include "isvrtree.hpp"
#include "ientitysvr.hpp"
#include "isvrtree.hpp"

using namespace Hxsoft::XFrame;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

HINSTANCE hInst;
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

extern "C"
{
	bool WINAPI CreateInstance(LPTSTR pStrObject,xbObject * &pxbObject);
	bool WINAPI ReleaseObject(xbObject * pxbObject);
}

bool WINAPI ReleaseObject(xbObject * pxbObject)
{
	if(pxbObject)delete pxbObject;
	return true;
}

bool WINAPI CreateInstance(LPTSTR pStrObject,xbObject * &pxbObject)
{
	pxbObject = NULL;
	if(::_tcsicmp(pStrObject,_T("IXPage"))==0)
	{
		pxbObject = new IXPage();
	}else if(::_tcsicmp(pStrObject,_T("IXFWin"))==0)
	{
		pxbObject = new IXFWin();
	}else if(::_tcsicmp(pStrObject,_T("IXNode"))==0)
	{
		pxbObject = new IXNode();
	}else if(::_tcsicmp(pStrObject,_T("IXFControl"))==0)
	{
		pxbObject = new IXFControl();
	}else if(::_tcsicmp(pStrObject,_T("IXAServer"))==0)
	{
		pxbObject = new IXAServer();
	}else if(::_tcsicmp(pStrObject,_T("IXAServerArg"))==0)
	{
		pxbObject = new IXAServerArg();
	}else if(::_tcsicmp(pStrObject,_T("ITrans"))==0)
	{
		pxbObject = new ITrans();
	}else if(::_tcsicmp(pStrObject,_T("ISvrTree"))==0)
	{
		pxbObject = new ISvrTree();
	}else if(::_tcsicmp(pStrObject,_T("IEntitySvr"))==0)
	{
		pxbObject = new IEntitySvr();
	}else
		return false;

	return true;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
