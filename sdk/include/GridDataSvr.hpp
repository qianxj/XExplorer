#pragma once
#include "xofbase.h"
#include "xuxutil.h"

#include "AxisSvr.hpp"
#include "ContentSvr.hpp"
#include "SpanRectSvr.hpp"
#include "xcellStyleSvr.hpp"
#include "FormatSvr.hpp"
#include "NameSvr.hpp"
#include "NoteSvr.hpp"
#include "TemplDefSvr.hpp"
#include "AttribSvr.hpp"
#include "reportband.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	enum RowStatus {
		rs_new=0,
		rs_newmodified=1,
		rs_datamodified=2,
		rs_datanotmodified=3,
		rs_deleted=4,
		rs_modified = 0x1000
	};

	enum DWBuffer{dbuf_normal=0,dbuf_filter,dbuf_delete};

	class CDwTable;
	class CAxisSvr;
	class CContentSvr;
	class CSpanRectSvr;
	class XCellStyleSvr;
	class CFormatSvr;
	class CNameSvr;
	class CNoteSvr;
	class CTemplDefSvr;
	class CAttribSvr;
	typedef struct tagChildSvr
	{
		int startRow;
		int endRow;
		CAxisSvr* pAxis;
	}TChildSvr;

	class XCELL_API CGridDataSvr : public XOfficeData
	{
	public:
		CGridDataSvr(void);
		~CGridDataSvr(void);
	public:
		CAxisSvr*			m_pAxisSvr;
		CContentSvr*		m_pContentSvr;
		CSpanRectSvr*		m_pSpanSvr;
		CFormatSvr*			m_pFormatSvr;
		CNameSvr*			m_pNameSvr;
		CNoteSvr*			m_pNoteSvr;
		CTemplDefSvr*		m_pTemplDefSvr;
		CAttribSvr*			m_pAttribSvr;
		CReportBandSvr*		m_ReportBand;
	public:
		LPTSTR				m_pSchemaSource;
		LPTSTR				m_pSchemaStr;
		void				SetSchmaSource(LPCTSTR pSchema);
		LPCTSTR				GetSchemaSource();
		void				SetSchmaString(LPCTSTR pSchemaStr);
		LPCTSTR				GetSchemaString();
	public:
		enum enView{envNormal,envDesign,envReportDesign,envGridReportDesign,envReport,envPrintDesign};
		enView m_enView;
	public:
		XCellStyleSvr*		GetStyleSvr();
	
	#pragma warning(disable: 4251)
	public:
		vector<wstring> FRowBands;
		vector<POINT> FRowBandRects;
		vector<CDwTable *> FBandDwTables;
		vector<int> FBandLevels;
	public:
		//数据表格
		vector<CDwTable*> m_pDwTable;
		bool AddTemplData(CGridDataSvr* m_pTemplData,bool bDestroyTemplData = false);
		bool ResetTemplData();
		void CalcRptBands();
	#pragma warning(default: 4251)


	public:
		CDwTable * GetDwTableFromCell(int ACol,int ARow);
	public:
		int RetrieveXmlData(LPTSTR pStrXml);
		int RetrieveXmlData(CDwTable * pDwtable,LPTSTR pStrXml);
		static int DwRetrieve(CDwTable * pDwtable,IXMLDOMElement * pElement,int nRow,IXMLDOMDocument2 * pXmlDoc = NULL);
		int RetrieveXmlDataDiff(LPTSTR pStrXml);
		int RetrieveXmlDataDiff(CDwTable * pDwtable,LPTSTR pStrXml);
		static int DwRetrieveDiff(CDwTable * pDwtable,IXMLDOMElement * pElement,int nRow,IXMLDOMDocument2 * pXmlDoc = NULL);
		int DwUpdate(CDwTable * pDwtable,IXMLDOMDocument2 * pXmlDoc);
		int DwUpdate(IXMLDOMDocument2 * pXmlDoc);

		int DwUpdateDiff(CDwTable * pDwTable,IXMLDOMDocument2 * pXmlDoc);
		int DwUpdateDiff(IXMLDOMDocument2 * pXmlDoc);
	public:
		std::vector<TChildSvr * > * m_pChilds;
		int GetChildIndex(int ARow);
		CAxisSvr * GetAxisSvr(int ARow);
		int AddChild(int nStartRow,int nEndRow, CAxisSvr* pAxis = NULL);
		int RemoveChild(int ARow);
		int GetChildIndexFollow(int ARow);
	};
}}}}
