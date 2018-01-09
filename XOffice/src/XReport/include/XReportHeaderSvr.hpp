#pragma once

#include "xofbase.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{

	struct XReportBandItem
	{
		int colid;
		LPTSTR pStrXPath;
		LPTSTR pStrText;
		LPTSTR name;
		int m_nIndexStyle;
		int m_nWidth;
		int m_nIndexEditor;
		int m_nIndexFormat;
		bool m_bHtml;
		XReportBandItem():pStrText(NULL),m_nIndexStyle(-1),m_nWidth(0),name(NULL),m_nIndexEditor(-1),m_nIndexFormat(-1),
			colid(-1),pStrXPath(NULL),m_bHtml(false){};
		~XReportBandItem()
		{
			if(pStrXPath) delete pStrXPath;
			if(pStrText) delete pStrText;
			if(name) delete name;
		}
	};

	class XReportBandSvr:public CSvrbase
	{
		enum TBand{none,header,group,detail,trail,footer};
	public:
		TBand eBand;
		int m_nIndexStyle;
		int m_nHeight;
		LPTSTR strXPath;
	public:
		class XReportDataSvr * m_pDataSvr;
	public:
		XReportBandSvr();
		~XReportBandSvr();
	public:
		virtual void OnDraw(HDC hdc,RECT &rect)=0;
	};

	struct ColHeaderItem :public XReportBandItem
	{
		XUtil::xuVector<ColHeaderItem *> m_HeaderItems;
		ColHeaderItem(){};
		~ColHeaderItem()
		{
			for(int i=0;i<m_HeaderItems.size();i++)
			{
				if(m_HeaderItems[i]) delete m_HeaderItems[i];
			}
		}
	};

	class XReportHeaderSvr :
		public XReportBandSvr
	{
	public:
		XReportHeaderSvr(void);
		~XReportHeaderSvr(void);

	public:
		int m_nStartCol;
		int m_nRowHeight;
	public:
		XUtil::xuVector<ColHeaderItem *> m_HeaderItems;
	public:
		int GetTotalLevel();
		int GetTotalLevel(ColHeaderItem * pHeaderItem);
		int GetColWidth(HDC hPaintDC,ColHeaderItem * pHeaderItem,XOfficeStyleSvr * pStyleSvr);
	public:
		virtual void OnDraw(HDC hdc,RECT &rect);
		virtual void OnDraw(HDC hdc,RECT &rect,XUtil::xuVector<ColHeaderItem *> & HeaderItems,int nLevel = 1);
	};

	class XReportDetailSvr :
		public XReportBandSvr
	{
	public:
		XReportDetailSvr(void);
		~XReportDetailSvr(void);

	public:
		int m_nStartRow;
		int m_nCurCol;
		int m_nCurRow;
	public:
		XUtil::xuVector<XReportBandItem *> m_DetailItems;
	public:
		virtual void OnDraw(HDC hdc,RECT &rect,class XReportSheetSvr* pReportSvr=NULL,int startRow = 0);
		virtual void OnDraw(HDC hdc,RECT &rect){};
		virtual int GetRowAtPoint(POINT pt,int &y);
		virtual int GetColAtPoint(POINT pt,int &x);
	public:
		int GetCellRect(int ACol,int ARow,RECT &rc,XReportSheetSvr* pReportSvr);
	};
}}}}
