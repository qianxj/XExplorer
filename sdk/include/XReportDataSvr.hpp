#pragma once

#include "xofbase.h"
#include "XOFObject.h"
#include "XReportHeadersvr.hpp"
#include "XRowSelectorSvr.hpp"
#include "XReportAxisSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	#define TReportOption int
	#define troRowSelector		 0x0010
	#define troRowSelectorEx	 0x0020
	#define troRightSelector	 0x0040

	class XREPORT_API XReportDataSvr :
		public XOffice::XOfficeData
	{
	public:
		XReportDataSvr(void);
		~XReportDataSvr(void);
	public:
		XReportHeaderSvr * m_pHeaderSvr;
		XReportDetailSvr * m_pDetailSvr;
		XRowSelectorSvr * m_pRowSelectorSvr;
		XRowSelectorSvr * m_pRightRowSelectorSvr;
		XReportAxisSvr	* m_pAixsSvr;
		XXmlContentSvr*	m_pContentSvr;
		LPTSTR m_pQueryXml;
		LPTSTR m_pQueryUrl;
		TReportOption FOption;

		vector<LPTSTR>* m_pEditor; 
		vector<LPTSTR>* m_pFormat; 
		vector<LPTSTR>* m_pClassEditor; 
		vector<LPTSTR>* m_pClassFormat; 
	};
}}}}
