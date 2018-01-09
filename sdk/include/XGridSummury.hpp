#pragma once

#include "xofbase.h"
#include "gridframesvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API XGridSummury : public CSvrbase
	{
	public:
		XGridSummury(void);
		~XGridSummury(void);
	public:
		class CGridDataSvr*  m_pDataTempl;
	public:
		class XGridReport* m_pGridReportSvr;
	public:
		virtual void OnDraw(HDC hdc,RECT &rect);
	public:
		int m_nTotailHeight;
	};
}}}}