#pragma once

#include <xframe.h>
#include "xofobject.h"
#include "xreport.h"

namespace Hxsoft{namespace XFrame
{
	class xcListWin :
		public xfWin
	{
	public:
		xcListWin(void);
		~xcListWin(void);
	public:
		RECT FRect;
		int Show(bool bShow = true,RECT * pRect = NULL);
	private:
		LPTSTR m_pStrPage;
		int	   m_nRptIndex;
	private:
		XOffice::XReport::XReportSerializeSvr* m_pSerializeSvr;
		std::vector<XOffice::XReport::XReportDataSvr *>* m_pDataSvrs;
	public:
		int SwitchReport(int nIndex);
		int AddReport(LPTSTR pStr);
		int SetReportData(int nIndex, LPTSTR pStr);
	};
}}
