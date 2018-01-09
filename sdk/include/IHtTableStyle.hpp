#pragma once
#include "xbxbase.h"
#include "xuxutil.h"
namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class IHtTableStyle : public xbObject
	{
	public:
		virtual const XUtil::Css::TBoxStyle *	GetCellCssStyle(INT32 ACol,INT32 ARow)=0;
		virtual int SetCellCssStyle(INT32 ACol,INT32 ARow, XUtil::Css::TBoxStyle * pBoxStyle)=0;
		virtual int SetCellCssStyle(RECT ARect, XUtil::Css::TBoxStyle * pBoxStyle)=0;
		virtual int SetFont(RECT ARect, LOGFONT	&logFont)=0;
		virtual int GetFont(INT32 ACol,INT32 ARow, LOGFONT	&logFont)=0;
		virtual int SetFont(INT32 ACol,INT32 ARow, LOGFONT	&logFont)=0;
	public:
		virtual int InsertRow(INT32 row, int nAmount)=0;
		virtual int RemoveRow(INT32 row, int nAmount)=0;
		virtual int InsertCol(INT32 col, int nAmount)=0;
		virtual int RemoveCol(INT32 col, int nAmount)=0;
	public:
		virtual void * GetClassStyle()=0;
		virtual void SetClassStyle(void * pClassStyle)=0;
	};

}}}