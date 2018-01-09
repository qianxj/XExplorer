#pragma once


#include "xframe.h"

namespace Hxsoft{ namespace XFrame
{
	#define DWV_ITEMFOCUSCHANGED 0x0001
	#define DWV_ROWFOCUSCHANGED  0x0002
	#define DWV_GETFOCUS		 0x0003
	#define DWV_LOSEFOCUS		 0x0004
	#define DWV_ITEMCHANGED		 0x0005
	#define DWV_ITEMCHANGING	 0x0006

	#define DWT_CELL			 0x0000
	#define DWT_GRID			 0x0001

	//this interface all index param start by one
	class xbXml;
	class XOFOBJECT_API IDwView : public IXFControl
	{
	public:
		IDwView():m_pTrans(NULL){};
	public:
		enum RowStatus{rs_new=0,rs_newmodified,rs_datamodified,rs_datanotmodified};
		enum DWBuffer{dbuf_normal=0,dbuf_filter,dbuf_delete};
	public:
		virtual int GetRow() = 0;//得到行
		virtual int GetColumn() = 0;//得到列
		virtual int GetRowCount() = 0; //得到行数
		virtual int GetColumnCount() =0; //得到列数
	public:
		virtual LPCTSTR GetNote(int nRow,int nColumn) = 0;//得到备注
		virtual bool SetNote(int nRow,int nColumn,LPCTSTR pStrNote) = 0; //设置备注
		virtual bool AddNote(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) = 0; //设置备注
		
		virtual LPCTSTR GetNote1(int nRow,int nColumn) = 0;//得到修改
		virtual bool SetNote1(int nRow,int nColumn,LPCTSTR pStrNote) = 0; //设置修改
		virtual bool AddNote1(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) = 0; //设置修改

		virtual LPCTSTR GetNote2(int nRow,int nColumn) = 0;//得到批注
		virtual bool SetNote2(int nRow,int nColumn,LPCTSTR pStrNote) = 0; //设置批注
		virtual bool AddNote2(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) = 0; //设置批注

	public:
		virtual int InsertRow(int nRow) = 0;//插入行
		virtual int DeleteRow(int nRow) = 0;//删除行
		virtual int InsertRows(int nRow,int nAmt) = 0;//插入多行
		virtual int DeleteRows(int nRow,int nAmt) = 0;//删除多行
	public:
		virtual LPCTSTR GetColumnName(int nColumn) = 0;//得到列名
		virtual int GetColumnIndex(LPCTSTR pColumn) = 0; //得到列号
	public:
		virtual LPCTSTR GetItemString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal) = 0;//得到数据值
		virtual LPCTSTR GetItemString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer=dbuf_normal) = 0;//得到数据值
		virtual bool SetItemString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal) = 0;//设置数据值
		virtual bool SetItemString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal) = 0;//设置数据值
	public:
		virtual bool SetItemStatus(int nRow,int nColumn,RowStatus status,DWBuffer dwBuffer=dbuf_normal) = 0;//设置行列状态
		virtual RowStatus GetItemStatus(int nRow,int nColumn, DWBuffer dwBuffer=dbuf_normal) = 0;//得到行列状态

	public:
		virtual int	SetKeyValue(int nRow,int nKeyValue) = 0;//设置整数关健值
		virtual int GetKeyValue(int nRow) = 0;//得到整数关健值
		virtual bool GetGuid(int nRow,TCHAR Guid[],int nMax) = 0;//设置guid
		virtual bool SetGuid(int nRow,LPCTSTR pStrGuid) = 0; //设置guid

	public:
		int SetReadOnly(bool bReadOnly=true); //设置只读
	public:
		int SelectRow(int nRow,bool bSelect=true);//选中单行
		int SelectRow(int sRow,int eRow, bool bSelect=true);//选中多行
		int GetNextSelectRow(int nStartRow); //得到下拉修改行
		bool IsRowSelected(int nRow); //判断行是否被选种
		int SetSelectBkColor(COLORREF color); //设置选择的背景颜色
		int SetSelectionMode(int nMode); //设置选择的模式//0 unselection 1 singleSelection 2 multiselection 3 multiselectionex 

	public:
		virtual int	DeletedCount() = 0; //得到删除行行数
		virtual int	ModifiedCount() = 0; //得到修改行行数
	public:
		virtual int	Reset() = 0; //清除数据
		virtual int	ResetUpdateStatus() = 0;//清除修改标记
	public:
		virtual BSTR GetXml(LPCTSTR pRoot, bool bIncludeChild ){return NULL;} //获取XML数据
		int  GetXml(BSTR &bstr){bstr = NULL; return -1;}//获取XML数据
		int  GetXml(BSTR &bstr,LPCTSTR pGuid){bstr = NULL; return -1;} //获取XML数据
		virtual int Load(IXMLDOMElement * pElement) = 0;//读取数据
		virtual int  LoadXml(BSTR bstr)=0;//读取数据
	public:
		virtual int AddObserver(xbObserver * pObserver) = 0; //增加事件侦听对象
		virtual int RemoveObserver(xbObserver * pObserver) = 0;//去除事件侦听对象
		virtual int ClearObserver() = 0;//清除事件侦听对象
	public:
		virtual int DwUpdateTo(IXMLDOMDocument2 * pXmlDoc)=0;//把数据更新到XML对象中
		virtual int DwUpdateAllTo(IXMLDOMDocument2 * pXmlDoc)=0;//把数据更新到XML对象中
	public:
		virtual int Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr) = 0;//读取数据
		virtual int Retrieve(ITrans *pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr) = 0;//读取数据
		virtual int Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr) = 0;//读取数据

		virtual int Retrieve(IXMLDOMElement * pElement)=0;//读取数据
		virtual int Retrieve(xbXml &xml)=0;//读取数据

		virtual int RetrieveEx(LPCTSTR pServer,LPCTSTR pEntity, LPCTSTR pEntityKey) = 0;//读取数据
		virtual int RetrieveEx(ITrans *pTrans,LPCTSTR pEntity, LPCTSTR pEntityKey) = 0;//读取数据
		virtual int RetrieveEx(LPCTSTR pEntity, LPCTSTR pEntityKey) = 0;//读取数据
	public:
		virtual int SetDataObject(LPCTSTR pServer,LPCTSTR pUrl) = 0;//设置模板对象
		virtual int SetDataObject(ITrans *pTrans,LPCTSTR pUrl) = 0;//设置模板对象
		virtual int SetDataObject(LPCTSTR pUrl) = 0;//设置模板对象

		virtual int SetDataObject(IXMLDOMElement * pElement) = 0;//设置模板对象
		virtual int SetDataObject(IXMLDOMDocument * pDoc)=0;
	public:
		virtual int ImportFile()=0 ; //转入数据
		virtual int ImportFile(LPCTSTR pStrFile,LPCTSTR pType=NULL,bool Header = false)=0; //转入数据
		virtual int ImportString(LPCTSTR pStr,LPCTSTR pType=NULL,bool Header = false)=0; //转入文本数据
	public:
		virtual int SetValues(LPCTSTR pColName,LPCTSTR pExpr)=0; //批量设置值
		virtual int AddCalcColumn(LPCTSTR pColName,LPCTSTR pExpr)=0;//设置关联计算列
		virtual int ClearCalcColumn(LPCTSTR pColName = NULL)=0;//清除关联计算列
	public:
		virtual int GetNextModifyRow(int nStart)=0; //得到下一修改行
	public:
		virtual int Modify(LPCTSTR pStr)=0; //语法修改
		virtual int Evaluate(int nRow,LPCTSTR pExpr)=0; //评估表达式
	public:
		virtual int SetAutoFilter(bool bAutoFilter)=0; //设置自动过滤
		virtual int SetFilter(LPCTSTR pStrFilter)=0; //设置过滤条件
		virtual int Filter()=0; //过滤
		virtual int SetSort(LPCTSTR pStrSort)=0; //设置排序参数
		virtual int Sort()=0; //排序
		virtual int Find(LPCTSTR pCondition = NULL)=0; //查找
		virtual int FindNext()=0; //查找
	public:
		virtual int GetFirstVisibleRow()=0; //得到当前显示的第一行
		virtual int Expand(bool bExpand)=0;//收缩或展开数据视图
	public:
		virtual int Print(LPCTSTR pXmlArg = NULL)=0; //打印
		virtual int PrintPreview(LPCTSTR pXmlArg = NULL)=0; //打印预览
	public:
		virtual int SaveAs(LPCTSTR pXmlArg = NULL)=0; //另存为
		virtual int SaveAsEx()=0; //整体另存为
		virtual int SaveAsEx(LPCTSTR pStrFile)=0; //整体另存为
	public:
		virtual LPTSTR GetColumnProps(LPCTSTR pColName)=0; //得到列整体XML属性
		virtual int SetColumnProps(LPCTSTR pColName,LPCTSTR pXmlProp)=0; //设置列整体XML属性
		virtual LPTSTR GetColumnProp(LPCTSTR pColName,LPCTSTR pItem)=0; //得到列属性
		virtual int SetColumnProp(LPCTSTR pColName,LPCTSTR pItem,LPCTSTR pProp)=0;//设置列属性

		virtual LPTSTR GetColumnProps(int nCol) = 0; //得到列整体XML属性
		virtual int SetColumnProps(int nCol,LPCTSTR pXmlProp) = 0; //设置列整体XML属性
		virtual LPTSTR GetColumnProp(int nCol,LPCTSTR pItem) = 0; //得到列属性
		virtual int SetColumnProp(int nCol,LPCTSTR pItem,LPCTSTR pProp) = 0;//设置列属性
	public:
		virtual int DataBI(LPCTSTR pSchema = NULL)=0; //数据BI分析
	public:
		virtual int chart(LPCTSTR pXmlArg = NULL)=0; //图形

	//send event message
		virtual int event_ItemFocusChanged(int nRow,int nCol)=0;
		virtual int event_RowFocusChanged(int nRow,int nOldRow)=0;
		virtual int event_GetFocus()=0;
		virtual int event_LoseFocus()=0;
		virtual int event_ItemChanged(int nRow,int nCol,LPCTSTR pData)=0;
		virtual int event_ItemChanging(int nRow,int nCol,LPCTSTR pData)=0;


	//follow function for cell table
	public:
		virtual int InsertRow(int nGroup,int nRow){return -1;} //插入组行
		virtual int DeleteRow(int nGroup,int nRow){return -1;} //删除组行
		virtual int InsertRows(int nGroup,int nRow,int nAmt){return -1;} //插入组多行
		virtual int DeleteRows(int nGroup,int nRow,int nAmt){return -1;}//删除组多行
	public:
		virtual int GetGroupStartRow(int nGroup){return -1;} //得到指定组的开始行
		virtual int GetGroupRowCount(int nGroup){return -1;} //得到指定组的结束行
		virtual int InsertGroupRow(int nGroup,int nRow){return -1;} //插入组行
	public:
		virtual int GetMaxDeep(){return -1;} //插入数据视图的最大深度
		virtual int GetLevel(){return -1;} //得到数据视图的级号
		virtual LPCTSTR GetDwXPath(){return NULL;} //得到数据视图的XPATH
	public:
		virtual IDwView * GetParent(){return NULL;} //得到父数据试图
	public:
		virtual int GetChildCount(){return -1;} //得到子数据视图数
		virtual IDwView * GetChild(int nIndex){return NULL;} //得到指定序号的子数据视图
	public:
		virtual LPCTSTR GetDwName(){return NULL;} //得到数据视图的名称
		virtual IDwView * FindDwTable(LPCTSTR pStrName,IDwView *pParent = NULL){return NULL;} //查找数据试图
		virtual IDwView * GetCurTable(int &nRow,int &nCol){return NULL;}//得到当前数据视图，并返回行列
		virtual IDwView * GetCurTable(){return NULL;}//得到当前数据视图
	public:
		//设置交易对象
		ITrans * m_pTrans;
		int SetTrans(ITrans * pTrans){m_pTrans = pTrans; return 1;}
		ITrans * GetTrans(){return m_pTrans;}
	public:
		virtual int GetDwType(){return DWT_CELL;}
	};
}}