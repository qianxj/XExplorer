#pragma once
#include "xofbase.h"
#include "xuxutil.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class XREPORT_API XReportAxisSvr :public CSvrbase
	{
	public:
		XReportAxisSvr(void);
		~XReportAxisSvr(void);
	public:
		XUtil::xuSummableCsArray< int, USHORT>*  m_pRowHeights;   
		XUtil::xuSummableCsArray< int, USHORT>*  m_pColWidths;
	public:
		int GetColWidth(int column);
		int GetRowHeight(int row);

	public:
		int SetColWidth(int column,int NewWidth);
		int SetRowHeight(int row ,int NewHeight);
	};
}}}}
