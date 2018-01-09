#pragma once
#include "xofbase.h"
#include "xuxutil.h"
#include "addrsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	#define CssClassIndex INT32
	typedef XUtil::xuCsArray< XCROW, CssClassIndex> * LPColCssClass;
	class XCellStyleSvr :
		public XOfficeStyleSvr
	{
	public:
		XCellStyleSvr(void);
		~XCellStyleSvr(void);
	public:
		XUtil::xuVector<LPColCssClass>*  m_pCssClass; 
	public:
		const XUtil::Css::TBoxStyle *	GetCellCssStyle(XCROW ACol,XCROW ARow);
		int SetCellCssStyle(XCROW ACol,XCROW ARow, XUtil::Css::TBoxStyle * pBoxStyle);
		int SetCellCssStyle(TGridRect ARect, XUtil::Css::TBoxStyle * pBoxStyle);
		int SetFont(TGridRect ARect, LOGFONT	&logFont);
		int GetFont(XCROW ACol,XCROW ARow, LOGFONT	&logFont);
		int SetFont(XCROW ACol,XCROW ARow, LOGFONT	&logFont);
	public:
		int InsertRow(XCROW row, int nAmount);
		int RemoveRow(XCROW row, int nAmount);
		int InsertCol(XCCOL col, int nAmount);
		int RemoveCol(XCCOL col, int nAmount);
	};
}}}}
