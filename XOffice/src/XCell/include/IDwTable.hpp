#pragma once


#include "xofobject.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	//this interface all index param start by one
	class XCELL_API IDwTable :public IDwView
	{
	public:
		IDwTable(void);
		~IDwTable(void);
	public:
		int GetRow();
		int GetColumn();
		int GetRowCount();
		int GetColumnCount();
		int SetRow(int row);//设置焦点
		int SetColumn(int col);//设置焦点
	public:
		int GetGroupStartRow(int nGroup);
		int GetGroupRowCount(int nGroup);
		int InsertGroupRow(int nGroup,int nRow);
		int GetGroupFromRow(int row); //从数据行得到组
	public:
		int GetMaxDeep();
		int GetLevel();
		LPCTSTR GetDwXPath();
	public:
		LPCTSTR GetNote(int nRow,int nColumn);
		bool SetNote(int nRow,int nColumn,LPCTSTR pStrNote);
		bool AddNote(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL){return true;} //设置备注
		
		LPCTSTR GetNote1(int nRow,int nColumn){return NULL;}//得到修改
		bool SetNote1(int nRow,int nColumn,LPCTSTR pStrNote){return true;} //设置修改
		bool AddNote1(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) {return true;} //设置修改

		LPCTSTR GetNote2(int nRow,int nColumn){return NULL;}//得到批注
		bool SetNote2(int nRow,int nColumn,LPCTSTR pStrNote) {return true;} //设置批注
		bool AddNote2(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL){return true;} //设置批注

	public:
		int InsertRow();
		int InsertRow(int nRow);
		int DeleteRow();
		int DeleteRow(int nRow);
		int InsertRow(int nGroup,int nRow);
		int DeleteRow(int nGroup,int nRow);
	public:
		int InsertRows(int nRow,int nAmt){return -99;}//插入多行
		int DeleteRows(int nRow,int nAmt){return -99;};//删除多行
		int InsertRows(int nGroup,int nRow,int nAmt){return -99;} //插入组多行
		int DeleteRows(int nGroup,int nRow,int nAmt){return -99;}//删除组多行

	public:
		LPCTSTR GetColumnName(int nColumn);
		int GetColumnIndex(LPCTSTR pColumn);
	public:
		LPCTSTR GetItemString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal);
		LPCTSTR GetItemString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool ItemChangeTo(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool ItemChangeTo(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
	public:
		LPCTSTR GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal);
		LPCTSTR GetItemDisplayString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemDisplayString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemDisplayString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
	public:
		bool SetItemStatus(int nRow,int nColumn,RowStatus status,DWBuffer dwBuffer=dbuf_normal);
		RowStatus GetItemStatus(int nRow,int nColumn, DWBuffer dwBuffer=dbuf_normal);
	public:
		IDwView * GetParent();
	public:
		int GetChildCount();
		IDwView * GetChild(int nIndex); 

	public:
		int	SetKeyValue(int nRow,int nKeyValue);
		int GetKeyValue(int nRow);
		bool GetGuid(int nRow,TCHAR Guid[],int nMax);
		bool SetGuid(int nRow,LPCTSTR pStrGuid);
		LPCTSTR GetGuid(int nRow);
	

	public:
		int SetReadOnly(bool bReadOnly=true);
	public:
		int SelectRow(int nRow,bool bSelect=true){return -99;}
		int SelectRow(int sRow,int eRow, bool bSelect=true){return -99;}
		int GetNextSelectRow(int nStartRow){return -99;}
		bool IsRowSelected(int nRow){return false;}
		int SetSelectBkColor(COLORREF color){return -99;}
		int SetSelectionMode(int nMode){return -99;} //0 unselection 1 singleSelection 2 multiselection 3 multiselectionex 

	public:
		int	DeletedCount();
		int	ModifiedCount();
	public:
		int	Reset();
		int	ResetUpdateStatus();
	public:
		int DwUpdateTo(IXMLDOMDocument2 * pXmlDoc);
		int DwUpdateAllTo(IXMLDOMDocument2 * pXmlDoc);
		int DwUpdateToDiff(IXMLDOMDocument2 * pXmlDoc);
		int DwUpdateAllToDiff(IXMLDOMDocument2 * pXmlDoc);
	public:
		virtual int Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr) ;//读取数据
		virtual int Retrieve(ITrans *pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr);//读取数据
		virtual int Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr) ;//读取数据

		virtual int Retrieve(IXMLDOMElement * pElement);//读取数据
		virtual int Retrieve(xbXml &xml);//读取数据
		virtual int RetrieveDiff(IXMLDOMElement * pElement);//读取数据
		virtual int RetrieveDiff(xbXml &xml);//读取数据

		virtual int RetrieveEx(LPCTSTR pServer,LPCTSTR pEntity, LPCTSTR pEntityKey){return -99;}//读取数据
		virtual int RetrieveEx(ITrans *pTrans,LPCTSTR pEntity, LPCTSTR pEntityKey){return -99;}//读取数据
		virtual int RetrieveEx(LPCTSTR pEntity, LPCTSTR pEntityKey) {return -99;}//读取数据
	public:
		virtual int SetDataObject(LPCTSTR pServer,LPCTSTR pUrl) ;//设置模板对象
		virtual int SetDataObject(ITrans *pTrans,LPCTSTR pUrl) ;//设置模板对象
		virtual int SetDataObject(LPCTSTR pUrl) ;//设置模板对象
	
		virtual int SetDataObject(IXMLDOMElement * pElement) ;//设置模板对象
		virtual int SetDataObject(IXMLDOMDocument * pDoc);
	public:
		int ImportFile() ;
		int ImportFile(LPCTSTR pStrFile,LPCTSTR pType=NULL,bool Header = false);
		int ImportString(LPCTSTR pStr,LPCTSTR pType=NULL,bool Header = false);
	
	public:
		BSTR GetXml(LPCTSTR pRoot, bool bIncludeChild = true);
		int Load(IXMLDOMElement * pElement);
		int  LoadXml(BSTR bstr);//读取数据
		//读取数据
	public:
		int AddObserver(xbObserver * pObserver);
		int RemoveObserver(xbObserver * pObserver);
		int ClearObserver();
	public:
		LPCTSTR GetDwName();
		IDwView * FindDwTable(LPCTSTR pStrName,IDwView *pParent = NULL);
		IDwView * GetCurTable(int &nRow,int &nCol);
		IDwView * GetCurTable();

	public:
		int SetValues(LPCTSTR pColName,LPCTSTR pExpr){return -99;} //批量设置值
		int AddCalcColumn(LPCTSTR pColName,LPCTSTR pExpr){return -99;}//设置关联计算列
		int ClearCalcColumn(LPCTSTR pColName = NULL){return -99;}//清除关联计算列
	public:
		int GetNextModifyRow(int nStart){return -99;} //得到下一修改行
	public:
		int Modify(LPCTSTR pStr); //语法修改
		int Evaluate(int nRow,LPCTSTR pExpr){return -99;} //评估表达式
	public:
		int SetAutoFilter(bool bAutoFilter){return -99;} //设置自动过滤
		int SetFilter(LPCTSTR pStrFilter){return -99;} //设置过滤条件
		int Filter(){return -99;} //过滤
		int SetSort(LPCTSTR pStrSort){return -99;} //设置排序参数
		int Sort(){return -99;} //排序
		int Find(LPCTSTR pCondition = NULL){return -99;} //查找
		int FindNext(){return -99;} //查找
	public:
		int GetFirstVisibleRow(){return -99;} //得到当前显示的第一行
		int Expand(bool bExpand){return -99;}//收缩或展开数据视图
	public:
		int Print(LPCTSTR pXmlArg = NULL){return -99;} //打印
		int PrintPreview(LPCTSTR pXmlArg = NULL){return -99;} //打印预览
	public:
		int SaveAs(LPCTSTR pXmlArg = NULL){return -99;} //另存为
		int SaveAsEx(){return -99;} //整体另存为
		int SaveAsEx(LPCTSTR pStrFile){return -99;} //整体另存为
	public:
		LPTSTR GetColumnProps(LPCTSTR pColName); //得到列整体XML属性
		int SetColumnProps(LPCTSTR pColName,LPCTSTR pXmlProp); //设置列整体XML属性
		LPTSTR GetColumnProp(LPCTSTR pColName,LPCTSTR pItem); //得到列属性
		int SetColumnProp(LPCTSTR pColName,LPCTSTR pItem,LPCTSTR pProp);//设置列属性

		LPTSTR GetColumnProps(int nCol); //得到列整体XML属性
		int SetColumnProps(int nCol,LPCTSTR pXmlProp); //设置列整体XML属性
		LPTSTR GetColumnProp(int nCol,LPCTSTR pItem); //得到列属性
		int SetColumnProp(int nCol,LPCTSTR pItem,LPCTSTR pProp);//设置列属性
		int SetColumnProp(int nRow, int nCol,LPCTSTR pItem,LPCTSTR pProp);//设置列属性
	public:
		int DataBI(LPCTSTR pSchema = NULL){return -99;} //数据BI分析
	public:
		int chart(LPCTSTR pXmlArg = NULL){return -99;} //图形

	public:
		int event_ItemFocusChanged(int nRow,int nCol);
		int event_RowFocusChanged(int nRow,int nOldRow);
		int event_GetFocus();
		int event_LoseFocus();
		int event_ItemChanged(int nRow,int nCol,LPCTSTR pData);
		int event_ItemChanging(int nRow,int nCol,LPCTSTR pData);

	public:
		class CDwTable * m_pDwTable;
		class XCellSheet * m_pSheet;
	public:
		int GetDwType();
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
		LPTSTR GetHtml();
	public:
		int SetScrollState(bool both);
		int SetHScrollState(bool bShow);
		int SetVScrollState(bool bShow);

	public:
		COLORREF	GetDirectColor();
		void		SetDirectColor(COLORREF cr);
		COLORREF	GetAdvanceColor();
		void		SetAdvanceColor(COLORREF cr);
	public:
		LPTSTR		GetNoteString(int nRow, int nCol); 
		LPTSTR		GetOriginString(int nRow, int nCol); 
		LPTSTR		GetAdvanceString(int nRow, int nCol); 
		
		void		SetNoteString(int nRow, int nCol,LPTSTR pStr); 
		void		SetOriginString(int nRow, int nCol,LPTSTR pStr); 
		void		SetAdvanceString(int nRow, int nCol,LPTSTR pStr); 

		void		ClearNoteString(int nRow, int nCol); 
		void		ClearOriginString(int nRow, int nCol); 
		void		ClearAdvanceString(int nRow, int nCol); 
	public:
		int AcceptText();
		int getNullCell(POINT &p);
		int ExpandRow();
		int ExpandRow(int sRow, int eRow);
		int ExpandCellRow(int sRow, int eRow);
		int ExpandMarginRow(int sRow, int eRow);
		int GetCellRowFromRowColumn(int sRow, int sCol); //得到行列
		int SetSelectionBorder(int Flag);
		int SetItemText(int row, int col, LPTSTR name);
	public:
		void SetEditUpperMode(bool upper);
		bool GetEditUpperMode();
	public:
		void*	m_pDataObject;
	};
}}}}
