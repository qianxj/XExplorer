#pragma once

#include "xbxbase.h"
#include "xframe.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class XREPORT_API IReportEvent : public xbObserver
	{
	public:
		IReportEvent(void);
		~IReportEvent(void);
	public:
		virtual int OnRowFoucsChanged(int nNewRow,int nOldRow){return -1;}
		virtual int OnDoubleClicked(int nRow){return -1;}
		virtual int OnRButtonDown(int nRow,TEvent* pEvent){return -1;}

	};
}}}}
