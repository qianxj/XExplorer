#pragma once
#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API CReportBandSvr :public CSvrbase
	{
	public:
		CReportBandSvr(void);
	public:
		~CReportBandSvr(void);
		enum enBandItem{enBandNone,enBandPageHeader,enBandHeader,enBandTitle,enBandDetail,
			enBandSummury,enBandFooter,enBandPageFooter};
	
	public:
		struct XCELL_API TReportBandItem
		{
			int startRow;
			int endRow;
			bool bValid;

			TReportBandItem():startRow(0),endRow(0),bValid(false){}
		};

		struct XCELL_API TReportBandGroup
		{
			TCHAR StrName[255];
			TReportBandItem Header;
			TReportBandItem Title;
			TReportBandItem Detail;
			TReportBandItem Summury;
			TReportBandItem Footer;
		};
		struct XCELL_API TReportBand
		{
			bool bValid;
			int	left;
			int right;
			TReportBandItem PageHeader;
			TReportBandItem Header;
			TReportBandItem Title;
			TReportBandItem Detail;
			TReportBandItem Summury;
			TReportBandItem Footer;
			TReportBandItem PageFooter;
			std::vector<TReportBandGroup>* m_pGroups;
			TReportBand():bValid(true),left(0),right(0),m_pGroups(new std::vector<TReportBandGroup>)
			{
				int start = 1;
				PageHeader.bValid = true;
				PageHeader.startRow = start; PageHeader.endRow = start;

				start += 2;
				Header.bValid = true;
				Header.startRow = start; Header.endRow = start;

				start += 2;
				Title.bValid = true;
				Title.startRow = start; Title.endRow = start;

				start += 2;
				Detail.bValid = true;
				Detail.startRow = start; Detail.endRow = start;

				start += 2;
				Summury.bValid = true;
				Summury.startRow = start; Summury.endRow = start;

				start += 2;
				Footer.bValid = true;
				Footer.startRow = start; Footer.endRow = start;

				start += 2;
				PageFooter.bValid = true;
				PageFooter.startRow = start; PageFooter.endRow = start;
			}
			~TReportBand()
			{
				if(m_pGroups)delete m_pGroups;
			}
		};

		TReportBand m_ReportBand;
		bool GetReportBand(int nRow,TReportBandItem* & pBandItem, int & nGroup); 
		LPTSTR GetReportBandTitle(TReportBandItem* pBandItem); 
		bool InsertRow(int ARow);
		bool DeleteRow(int ARow);

		int GetTitleHeight(CAxisSvr * pAxisSvr);
		int GetDetailHeight(CAxisSvr * pAxisSvr);

	};
}}}}