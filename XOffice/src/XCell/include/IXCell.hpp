#pragma once

#include "StdAfx.h"
#include "xframe.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class  XCELL_API IXCell : public IXFControl
	{
	public:
		IXCell();
		~IXCell();
	public:
		int GetRow();
		int GetColumn();

		int GetAnchorRow();
		int GetAnchorCol();

		int SetRowColumn(int ARow,int ACol);
		int SetAnchorRowColumn(int ARow,int ACol);

		int GetRowCount();
		int SetRowCount();

		int GetColumnCount();
		int SetColumnCount();

		LPCTSTR GetItemString(int ARow,int ACol);
		int SetItemString(int ARow,int ACol,LPTSTR pText);

		int SetBorder(int nBorder);

		int Copy();
		int Cut();
		int Paste();

		int GetColumnWidth(int nCol);
		bool SetColumnWidth(int nCol,int nWidth);
		int GetRowHeight(int nRow);
		bool SetRowHeight(int nRow,int nHeight);
	public:
		int SetRowSelectorWidth(int nWidth);
		int ResetRowSelector();
		int GetRowSelectorWidth();
		int GetRowSelectorDefaultWidth();

		int SetColHeaderHeight(int nHeight);
		int GetColHeaderDefaultHeight();
		int ResetColHeaderHeight();
		int GetColHeaderHeight();
	public:
		LPTSTR GetCellProps(int nRow,int nCol); //得到单元整体XML属性
		int SetCellProps(int nRow,int nCol,LPCTSTR pXmlProp); //得到单元整体XML属性
		LPTSTR GetCellProp(int nRow,int nCol,LPCTSTR pItem); //得到单元属性
		int SetCellProp(int nRow,int nCol,LPCTSTR pItem,LPCTSTR pProp);//得到单元属性
	public:
		LPTSTR GetHtml();
		LPTSTR GetData();
	public:
		void * GetSheetSvr();
	public:
		void		SetSchmaSource(LPCTSTR pSchema);
		LPCTSTR		GetSchemaSource();
		void		SetSchmaString(LPCTSTR pSchemaStr);
		LPCTSTR		GetSchemaString();
	public:
		int			GetSpanRight(int ARow,int ACol);
		int			GetSpanBottom(int ARow,int ACol);
		int			GetSpanLeft(int ARow,int ACol);
		int			GetSpanTop(int ARow,int ACol);
		bool		IsSpanCell(int ARow,int ACol);
	public:
		int			GetValidRow();
		int			GetValidCol(int ARow);
		int			GetValidRowCol(int &ARow,int &ACol);
		int			GetValidCol(); 
	public:
		int			LoadTemplate(IXMLDOMDocument * pDoc,LPTSTR strState);
		int			LoadTemplate(IXMLDOMElement * pElement,LPTSTR strState);
	public:
		int SaveFileAs();
		int SaveFile();
		int SaveFile(wchar_t * fileName);
	public:
		int OpenFile(wchar_t* fileName);
		int OpenFile();
	public:
		wchar_t* m_pFileName;
	public:
		int SetReadOnly(bool bReadOnly);
		int SetRunState();
		int SetRunState(int runstate);
	public:
		void SetEditUpperMode(bool upper);
		bool GetEditUpperMode();
	};
}}}}