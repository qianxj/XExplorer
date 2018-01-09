#pragma once

#include "ihttablestyle.hpp"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	#define CssClassIndex INT32
	class CHtTableStyle : public IHtTableStyle
	{
	public:
		CHtTableStyle(void);
		~CHtTableStyle(void);
	
		typedef XUtil::xuCsArray<INT32, CssClassIndex> * LPColCssClass;
		std::vector<LPColCssClass> * m_pCssClass; 
	public:
		const XUtil::Css::TBoxStyle *	GetCellCssStyle(INT32 ACol,INT32 ARow);
		int SetCellCssStyle(INT32 ACol,INT32 ARow, XUtil::Css::TBoxStyle * pBoxStyle);
		int SetCellCssStyle(RECT ARect, XUtil::Css::TBoxStyle * pBoxStyle);
		int SetFont(RECT ARect, LOGFONT	&logFont);
		int GetFont(INT32 ACol,INT32 ARow, LOGFONT	&logFont);
		int SetFont(INT32 ACol,INT32 ARow, LOGFONT	&logFont);
	public:
		int InsertRow(INT32 row, int nAmount);
		int RemoveRow(INT32 row, int nAmount);
		int InsertCol(INT32 col, int nAmount);
		int RemoveCol(INT32 col, int nAmount);
	public:
		void* m_pStyleSvr;
	public:
		void * GetClassStyle();
		void SetClassStyle(void * pClassStyle);
	};
}}}
