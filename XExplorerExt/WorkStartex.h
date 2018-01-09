#pragma once

#include "defaultview.h"
#include "xccontrol.h"
#include "xofbase.h"
#include "xofobject.h"

using namespace Hxsoft::XFrame;


class CWorkStartEx :
	public CDefaultView
{
public:
	enum EViewType{ VT_Text,VT_SafeHtml,VT_Html};
	EViewType m_viewType;
public:
	CWorkStartEx(void);
	~CWorkStartEx(void);
public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
	virtual int OnOpen();
public:
	class IReportEventImpl *	m_pTaskListon;
	class IReportEventImplEx *	m_pMailListon;
public:
	static int MakeData(XOffice::XXmlContentSvr * pContentSvr );

};
