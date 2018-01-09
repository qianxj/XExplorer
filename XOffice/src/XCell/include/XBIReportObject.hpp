#pragma once

#include "xframe.h"
#include "xcellsheetsvr.hpp"
#include "xcellsheet.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XBIReportObject :
		public xfAreaObject
	{
	public:
		XBIReportObject(void);
		~XBIReportObject(void);
	public:
		XCellSheetSvr* m_pCellSheetSvr ;
		XCellSheet * m_pCellSheet;
	protected:
		RECT FRect;
	public:
		int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		int GetWidth();
		int GetHeight();
	public:
		int Load(IXMLDOMElement * pElement);
		int ProcessRowNode(IXMLDOMNode * pNode,int &nRow,int nCol);
		int ProcessColNode(IXMLDOMNode * pNode,int nRow,int &nCol);
	private:
		int GetTextWidth(int ACol,int ARow,LPCTSTR pStr);
	public:
		IXMLDOMElement * m_pElement;
	public:
		int m_sRow;
		int m_sCol;
		int m_Height;
		int m_Width;
	public:
		int m_gRow;
	public:
		int SetFRect(RECT & rc);
		int GetClientRect(RECT &rt);
	};
}}}}
