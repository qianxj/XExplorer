#include "stdafx.h"
#include <xframe.h>
#include "systypes.h"
#include "sysdep.h"
#include <atlbase.h>
#include "defaultview.h"
#include "devview.h"
#include "devworkdlg.h"
#include "mdiframe.h"
#include "officeframe.h"
#include "mailview.h"
#include "entityexplorer.h"
#include "entitymaint.h"
#include "AuthWin.h"
#include "devschema.h"
#include "TemplateDlg.h"
#include "worknodedlg.h"
#include "xPageSheet.h"
#include "entitylist.h"
#include "devxquery.h"
#include "FavoritesDlg.h"
#include "taskdlg.h"
#include "WorkFrame.h"
#include "chatwnd.h"
#include "workstart.h"
#include "workstartex.h"
#include "xcodesheet.h"
#include "xdvsheet.h"
#include "rulersheet.h"
#include "xfilesheet.h"
#include "xmodesheet.h"
#include "xxmlsheet.h"
#include "dataschemadlg.h"
#include "devframe.h"
#include "DataAnalysis.h"

using namespace Hxsoft::XFrame;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

//extern XFRAME_API Hxsoft::XFrame::xfApp*  g_pXApp ;

BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C"
{
	 xfWin* WINAPI CreateInstance(LPTSTR pWinClass,LPTSTR pWinName);
}

xfWin* WINAPI CreateInstance(LPTSTR pWinClass,LPTSTR pWinName)
{

	if(pWinName && pWinName[0]=='{') return NULL;

	if(pWinName && ::_tcsicmp(pWinName,_T("devworknodeview"))==0)
		return new CWorkNodeDlg();
	if(pWinName && ::_tcsicmp(pWinName,_T("chatwin"))==0)
		return new CChatWnd();
	if(pWinName && ::_tcsicmp(pWinName,_T("taskDlg"))==0)
		return new CTaskDlg();
	if(pWinName && ::_tcsicmp(pWinName,_T("devxpagesheet"))==0)
		return new xPageSheet();
	if(pWinName && ::_tcsicmp(pWinName,_T("devxcodesheet"))==0)
		return new xCodeSheet();
	if(pWinName && ::_tcsicmp(pWinName,_T("devxdvsheet"))==0)
		return new xdvSheet();
	if(pWinName && ::_tcsicmp(pWinName,_T("devschemaview"))==0)
		return new CDevSchema();
	if(pWinName && ::_tcsicmp(pWinName,_T("developerWorkDlg"))==0)
		return new CDevWorkDlg();
	if(pWinName && ::_tcsicmp(pWinName,_T("workframe"))==0) 
		return new CWorkFrame();
	if(pWinName && ::_tcsicmp(pWinName,_T("devxquerysheet"))==0)
		return new DevXQuery();
	if(pWinName && ::_tcsicmp(pWinName,_T("devrulersheet"))==0)
		return new RulerSheet();
	if(pWinName && ::_tcsicmp(pWinName,_T("devxfilesheet"))==0)
		return new XFileSheet();
	if(pWinName && ::_tcsicmp(pWinName,_T("devxmodesheet"))==0)
		return new XModeSheet();
	if(pWinName && ::_tcsicmp(pWinName,_T("devxxmlsheet"))==0)
		return new XXmlSheet();
	if(pWinName && ::_tcsicmp(pWinName,_T("workstart"))==0)
		return new CWorkStart();
	if(pWinName && ::_tcsicmp(pWinName,_T("workstartex"))==0)
		return new CWorkStartEx();
	if(pWinName && ::_tcsicmp(pWinName,_T("FavoritesDlg"))==0)
		return new FavoritesDlg();
	if(pWinName && ::_tcsicmp(pWinName,_T("devframe"))==0)
		return new DevFrame();
	else if(pWinName && ::_tcsicmp(pWinName,_T("developerframe"))==0)
		return new CDevView();
	else if(pWinName && ::_tcsicmp(pWinName,_T("templatedlg"))==0)
		return new CTemplateDlg();
	else if(pWinName && ::_tcsicmp(pWinName,_T("dataschemadlg"))==0)
		return new CDataSchemaDlg();
	else if(pWinName && ::_tcsicmp(pWinName,_T("xofficeframe"))==0)
		return new COfficeFrame();
	else if(pWinName && ::_tcsicmp(pWinName,_T("mailframe"))==0)
		return new CMailView();
	else if(pWinName && ::_tcsicmp(pWinName,_T("DataAnalysis"))==0)
		return new CDataAnalysis();
	else if(pWinClass && ::_tcsicmp(pWinClass,_T("mdiframe"))==0)
		return new CMdiFrame();
	else if(!pWinName && pWinClass && ::_tcsicmp(pWinClass,_T("EntityExplorer"))==0)
		return new CEntityExplorer();
	else if(!pWinName && pWinClass && ::_tcsicmp(pWinClass,_T("EntityMaint"))==0)
		return new EntityMaint();
	else if(!pWinName && pWinClass && ::_tcsicmp(pWinClass,_T("authwin"))==0)
		return new CAuthWin();
	else if(!pWinName && pWinClass && ::_tcsicmp(pWinClass,_T("EntityList"))==0)
		return new CEntityList();

	
	return  new CDefaultView();
}