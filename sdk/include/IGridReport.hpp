#pragma once

#include "xframe.h"
#include "xofobject.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API  IGridReport :
		public IDwView
	{
	public:
		IGridReport(void);
		~IGridReport(void);
	public:
		int GetRow();
		int GetColumn() {return -1;}

		int GetRowCount();
		int SetRow(int row);
	public:
		int GetColumnCount();
		LPCTSTR GetColumnName(int col);
		int GetColumnIndex(LPCTSTR pColumn);
	public:
		LPCTSTR GetNote(int nRow,int nColumn){return NULL;}//得到备注
		bool SetNote(int nRow,int nColumn,LPCTSTR pStrNote){return true;} //设置备注
		bool AddNote(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL){return true;} //设置备注
		
		LPCTSTR GetNote1(int nRow,int nColumn){return NULL;}//得到修改
		bool SetNote1(int nRow,int nColumn,LPCTSTR pStrNote){return true;} //设置修改
		bool AddNote1(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) {return true;} //设置修改

		LPCTSTR GetNote2(int nRow,int nColumn){return NULL;}//得到批注
		bool SetNote2(int nRow,int nColumn,LPCTSTR pStrNote) {return true;} //设置批注
		bool AddNote2(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL){return true;} //设置批注
	public:
		LPCTSTR GetItemString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal);
		LPCTSTR GetItemString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemStringEx(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool ItemChangeTo(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool ItemChangeTo(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
	public:
		bool SetItemStatus(int nRow,int nColumn,RowStatus status,DWBuffer dwBuffer=dbuf_normal){return false;}
		RowStatus GetItemStatus(int nRow,int nColumn, DWBuffer dwBuffer=dbuf_normal){return (RowStatus)-1;}
	public:
		int InsertRow(int nRow);
		int DeleteRow(int nRow);
	public:
		int InsertRows(int nRow,int nAmt){return -99;}//插入多行
		int DeleteRows(int nRow,int nAmt){return -99;};//删除多行

	public:
		int  GetXml(BSTR &bstr);
		int  GetXml(BSTR &bstr,LPCTSTR pGuid);
		int  LoadXml(BSTR bstr);
		int Load(IXMLDOMElement * pElement);
	public:
		int SetReadOnly(bool bReadOnly=true);
	public:
		int SelectRow(int nRow,bool bSelect=true);
		int SelectRow(int sRow,int eRow, bool bSelect=true);
		int GetNextSelectRow(int nStartRow);
		int GetPrevSelectRow(int nStartRow);
		bool IsRowSelected(int nRow);
		int SetSelectBkColor(COLORREF color);
		int SetSelectionMode(int nMode); //0 unselection 1 singleSelection 2 multiselection 3 multiselectionex 

	public:
		int AddObserver(xbObserver * pObserver);
		int RemoveObserver(xbObserver * pObserver);
		int ClearObserver();
	public:
		int	SetKeyValue(int nRow,int nKeyValue){return -1;}
		int GetKeyValue(int nRow){return -1;}
		bool GetGuid(int nRow,TCHAR Guid[],int nMax){return false;}
		bool SetGuid(int nRow,LPCTSTR pStrGuid){return false;}
	public:
		int DwUpdateTo(IXMLDOMDocument2 * pXmlDoc);
		int DwUpdateAllTo(IXMLDOMDocument2 * pXmlDoc);
		int DwUpdateAllToEx(IXMLDOMDocument2 * pXmlDoc);

		int GetBIData(IXMLDOMDocument2 * pXmlDoc);
	public:
		virtual int Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr) ;//读取数据
		virtual int Retrieve(ITrans *pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr);//读取数据
		virtual int Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr) ;//读取数据

		virtual int Retrieve(IXMLDOMElement * pElement);//读取数据
		virtual int Retrieve(xbXml &xml);//读取数据

		virtual int RetrieveEx(LPCTSTR pServer,LPCTSTR pEntity, LPCTSTR pEntityKey){return -99;}//读取数据
		virtual int RetrieveEx(ITrans *pTrans,LPCTSTR pEntity, LPCTSTR pEntityKey) {return -99;}//读取数据
		virtual int RetrieveEx(LPCTSTR pEntity, LPCTSTR pEntityKey){return -99;}//读取数据
	public:
		virtual int SetDataObject(LPCTSTR pServer,LPCTSTR pUrl) ;//设置模板对象
		virtual int SetDataObject(ITrans *pTrans,LPCTSTR pUrl) ;//设置模板对象
		virtual int SetDataObject(LPCTSTR pUrl) ;//设置模板对象

		virtual int SetDataObject(IXMLDOMElement * pElement) ;//设置模板对象
		virtual int SetDataObject(IXMLDOMDocument * pDoc);
	public:
		int PostRetrieve();
	public:
		int ImportFile() ;
		int ImportFile(LPCTSTR pStrFile,LPCTSTR pType=NULL,bool Header = false);
		int ImportString(LPCTSTR pStr,LPCTSTR pType=NULL,bool Header = false);
	
	public:
		int	DeletedCount(){return -1;}
		int	ModifiedCount(){return -1;}
	public:
		int	Reset();
		int	ResetUpdateStatus();
	public:
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
		int SaveAs(LPCTSTR pXmlArg = NULL); //另存为
		int SaveAs(xbXml &xml, LPCTSTR pFileName = NULL); //另存为
		int SaveAsEx(){return -99;} //整体另存为
		int SaveAsEx(LPCTSTR pStrFile){return -99;} //整体另存为
	public:
		LPTSTR GetColumnProps(LPCTSTR pColName){return NULL;} //得到列整体XML属性
		int SetColumnProps(LPCTSTR pColName,LPCTSTR pXmlProp);//设置列整体XML属性
		LPTSTR GetColumnProp(LPCTSTR pColName,LPCTSTR pItem); //得到列属性
		int SetColumnProp(LPCTSTR pColName,LPCTSTR pItem,LPCTSTR pProp){return -99;}//设置列属性

		LPTSTR GetColumnProps(int nCol){return NULL;}; //得到列整体XML属性
		int SetColumnProps(int nCol,LPCTSTR pXmlProp);//设置列整体XML属性
		LPTSTR GetColumnProp(int nCol,LPCTSTR pItem); //得到列属性
		int SetColumnProp(int nCol,LPCTSTR pItem,LPCTSTR pProp){return -99;};//设置列属性

	public:
		LPTSTR FDataBISchema;
		int DataBI(LPCTSTR pSchema = NULL); //数据BI分析
		LPTSTR DataAnalysis(LPCTSTR pSchema = NULL); //数据BI分析
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
		class XGridReport * m_pGridReport;
	public:
		int GetDwType();
	public:
		IXMLDOMElement * GetRowElement(int nRow);
	public:
		int DwShareTo(IGridReport * pPrimaryReport);
	public:
		int SetAskStyle(bool bAskStyle);
		int CreateTree(LPCTSTR pPath,int nCol, int nImage1,int nImage2);
		int CreateTreeEx(LPCTSTR pPath,int nCol, int nImage1,int nImage2);
	public:
		LPCTSTR GetItemDisplayString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal);
		LPCTSTR GetItemDisplayString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemDisplayString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemDisplayString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
	public:
		int AllowSort(bool sort);
	public: 
		int FindByColumn(int startRow, int columnIndex, LPCTSTR columnValue);
		int FindByColumn(int startRow, LPCTSTR column, LPCTSTR columnValue);

		int SetReadOnlyColumn(int columnIndex, bool flag=true);
		int SetReadOnlyColumn(LPCTSTR column, bool flag=true);

		int SetColumnWidth(int columnIndex, int width=1);
		int SetColumnWidth(LPCTSTR column, int width=1);
		int GetColumnWidth(int columnIndex);
		int GetColumnWidth(LPCTSTR column);
		int SetColumnState(int columnIndex, bool state);
		int SetColumnState(LPCTSTR column, bool state);

		void SetDataToClip(LPCTSTR DataStr);
		int AcceptText();
		int ShowRowTo(int row);
		int GetShowRow();
		LPCTSTR GetTitleName(int col);
	public:
		int SetRowSort(LPTSTR colName, bool colOrder=true);
		int SetClickEditFlag(bool f);
		int RedrawEx();
		bool SetRowMove(int row, int movecount);
		int GetTreeRow(int row);
		int GetTreeRowCount();
		int TreeRowExpand(int row, bool exp);
		int RetrieveDiff(xbXml &xml);//读取数据
		int Filter(LPTSTR pColumn, LPTSTR pStr);
	public:
		int SetHeaderText(int row, int col, LPTSTR name);
		int MoveColumnTo(int fromcolumn, int tocolumn);
	public:
		void SetEditUpperMode(bool upper);
		bool GetEditUpperMode();
	};
}}}}
