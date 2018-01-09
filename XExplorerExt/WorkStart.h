#pragma once

#include "defaultview.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;

struct TaskParam
{
	TCHAR uri[255];
	TCHAR action[80]; 
	TCHAR mailid[60];
	TCHAR taskid[60]; 
	TCHAR billid[60];
	TCHAR billno[60];
	TCHAR para[60];
	TCHAR env[40];
	TCHAR mailstate[40];
	TCHAR taskstate[40];
	TCHAR caption[255]; 
	TCHAR * data;
	void* pData;	
};

class CWorkStart :
	public CDefaultView
{
public:
	enum EViewType{ VT_Text,VT_SafeHtml,VT_Html};
	EViewType m_viewType;
public:
	CWorkStart(void);
	~CWorkStart(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
public:
	class IReportEventImpl *	m_pTaskListon;
	class IReportEventImplEx *	m_pMailListon;
public:
	static int MakeData(XOffice::XXmlContentSvr * pContentSvr );
	static int MakeDataEx(XOffice::XXmlContentSvr * pContentSvr );

};
