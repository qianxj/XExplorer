#pragma once

#include "gridframesvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{

	class XCELL_API CDwTable :public xbObject
	{
	public:
		CDwTable(void);
	public:
		~CDwTable(void);
	public:
		//数据组操作
		int insertGroup(int group);	//增加组
		int deleteGroup(int group);	//删除组
		int DoSetGroupPreRow(int group); //执行组预留行
		int getGroupStartRow(int group); //得到组的起始行
		int getGroupStartCellRow(int group); //得到组的起始表格行
		int getGroupFromRow(int row); //从数据行得到组
		int getGroupFromCellRow(int cellRow); //得到组的起始表格行
		bool getGroupCellArea(int group, int &sCellRow,int &eCellRow); //得到组的表格行区间
		bool getGroupRowArea(int group, int &sRow,int &eRow); //得到组的数据行区间
		int getGroupRowCount(int group);//得到组的行数

		//数据操作
		int getColumns(vector<LPTSTR> &dataColumns,vector<POINT> &ColPoses); //得到数据列
		int getRowColumnFromCell(int ACol,int ARow,int &row,int &col); //得到行列
		int getCellFromRowColumn(int row,int col,int &ACol,int &ARow); //得到行列
		PStrItem getDwItem(int row,int col); //得到行列项
		int setDwItem(int row,int col, PStrItem pValue); //设置行列项
		LPTSTR getDwItemText(int row,int col); //得到行列项
		int setDwItemText(int row,int col,LPTSTR strValue); //设置行列项
		int getColumnCount(); //得到列数
		LPTSTR getColumnName(int col); //得到列名
		int getColumnIndex(LPTSTR pColName);//得到列号

		LPTSTR getDwItemDisplayText(int row,int col); //得到行列项
		int setDwItemDisplayText(int row,int col,LPTSTR strValue); //设置行列项

		//数据表整体特性
		bool GetTableCellArea(int &sCellRow, int &eCellRow); //得到数据表的表格行区间

		//数据行操作
		int SetRow(int row);//设置焦点
		int GetRow();// 得到当前行
		int GetRowFromCell(int cellRow);// 插入新行
		int GetRowArea(int row, int& sCellRow,int& eCellRow); //得到行的表格区域
		int DoSetPreRows();//执行预留的行
		int GetRowCount();//得到行数
		int DeleteRow(); //删除行
		int DeleteRow(int row); //删除行
		int DeleteGRow(int group ,int row); //删除行
		int InsertRow(); // 插入行
		int InsertRow(int row); // 插入行
		int InsertGRow(int group, int row); // 插入行
		int InsertGRow(int group, int row,int amt,bool redraw = true); // 插入行
		int AppendRow(void); //扩展行
		int AppendGRow(int group); //扩展行

		int AdjustDwTables(int sCellPos,int Number);
		static int AdjustDwTables(CDwTable * pDwTable,int sCellPos,int Number);
		static int AdjustDwTables(CGridFrameSvr* pGrid, int sCellPos,int Number);
		//数据行操作结束

		//数据操作声明结束

		//模板操作
	public:
		bool LoadTemplData(int row, LPTSTR szGroupStr,CGridDataSvr * m_pData); //读入模板数据
		int GetBandInfo(vector<POINT> &pts,vector<int> &levels, vector<wstring> &Bands,vector<CDwTable *> &pDws);//得到Band信息
		int GetMaxDeep(); //返回当前的最大深度
		int GetLevel(); //返回当前的级次


	private:
		int GetRowGroupStartTempl(LPCTSTR szGroup,CGridDataSvr * m_pData); //得到模板组起始表格行
		int GetRowGroupEndTempl(LPCTSTR szGroup,CGridDataSvr * m_pData); //得到模板组的结束表格行
		LPTSTR GetMatchGroupTempl(LPCTSTR szGroup,LPTSTR szGroupStr); //从模板串中匹配模板组，返回对应的指针
		LPTSTR TrimLeftGroupTempl(LPTSTR szGroupStr); //从模板串中去除左边的模板组
		//模板操作结束

	public:
		int GetTemplateColumnCell(int nCol,int &tCol,int &tRow);
		int GetTemplateColumnCell(LPTSTR pColName,int &tRow,int &tCol);
		LPTSTR GetColumnProp(int nCol,LPCTSTR pAttribute);
		LPTSTR GetColumnProp(LPCTSTR pcolName,LPCTSTR pAttribute);
		LPTSTR GetColumnProps(LPCTSTR pcolName);
		LPTSTR GetColumnProps(int nCol);

		int SetColumnProp(int nCol,LPCTSTR pAttribute,LPCTSTR pProp);
		int SetColumnProp(LPCTSTR pcolName,LPCTSTR pAttribute,LPCTSTR pProp);
		int SetColumnProps(LPCTSTR pcolName,LPCTSTR pProps);
		int SetColumnProps(int nCol,LPCTSTR  pProps);
	public:
		//关键字和行状态
		bool	SetStringKeyValue(int row,LPTSTR keyValue,DWBuffer dwBuffer=dbuf_normal);
		bool	SetKeyvalue(int row,int keyValue,DWBuffer dwBuffer=dbuf_normal);
		LPCTSTR	GetStringKeyvalue(int row,DWBuffer dwBuffer=dbuf_normal);
		int		GetKeyvalue(int row,DWBuffer dwBuffer=dbuf_normal);
		bool	SetGuid(int row,LPTSTR guid,DWBuffer dwBuffer=dbuf_normal);
		LPCTSTR	GetGuid(int row,DWBuffer dwBuffer=dbuf_normal);
		RowStatus	GetItemStatus(int row,int col,DWBuffer dwBuffer=dbuf_normal);	
		bool	SetItemStatus(int row,int col,RowStatus status,DWBuffer dwBuffer=dbuf_normal);	
		bool	SetXmlRowId(int row,int xmlRowId,DWBuffer dwBuffer=dbuf_normal);	
		int 	GetXmlRowId(int row,DWBuffer dwBuffer=dbuf_normal);	
		int		GetDeletedCount();
		int		GetModifiedCount();
		int		Reset();
		int		ResetUpdateStatus();
		int		SetItemModified(int row,int col);
	public:
		//模板变量
		bool				m_bDestroyTemplData;	//是否要删除模板数据对象
		int					m_RowGroupStartTempl;	//对象在模板数据对象中的起始行
		int					m_RowGroupEndTempl;	//对象在模板数据中的结束行
		CGridDataSvr*		m_pTemplGridData; //模板数据
		LPTSTR				m_strBoundGroup; //对象绑定的模板组

		#pragma warning(disable: 4251)

		//数据变量
		CGridFrameSvr* m_pGrid;	//操作的表格
		vector<int> m_pRowCells; //每一行包含的表格行数量
		int m_preRows; //初始预留行数

		//数据组变量
		vector<int>			m_groups;		//每一组包含的行数
		vector<int>			m_groupstart;	//每一组的起始单元格，相对于父对象

		//关联的对象
		CDwTable *			m_pParent;   //父对象
		vector<CDwTable*>	m_pChilds;	//子对象

		//列变量
		vector<LPTSTR> m_dbcols;
		vector<POINT>  m_dbcolsPos;

		//附加变量
		vector<int> m_rowsIndex;
		vector<int> m_delrowsIndex;
		vector<IXMLDOMElement *> m_delXml;  //删除的xml
		vector<int> m_delGroup;  //删除的xml对应父对象的group

		vector<LPTSTR> m_StrKeys;
		vector<int> m_Keys;
		vector<LPTSTR> m_guidKeys;
		vector<int> m_XmlRowNos;
		vector<RowStatus> m_RowsStatus;
	public:
		std::vector<xbObserver * > m_pObServers;
		int AddObserver(xbObserver * pObserver);
		int RemoveObserver(xbObserver * pObserver);
		int ClearObserver();
	public:
		virtual xbObject*  GetInterface();
	public:
		int event_ItemFocusChanged(int nRow,int nCol);
		int event_RowFocusChanged(int nRow,int nOldRow);
		int event_GetFocus( CDwTable * pLoseDwTable);
		int event_LoseFocus(CDwTable * pFocusedDwTable);
		int event_ItemChanged(int nRow,int nCol,LPTSTR pData,LPTSTR pDisplayData=NULL);
		int event_ItemChanging(int nRow,int nCol,LPTSTR pData);
		int event_EditorLoseFocus();
	public:
		int AcceptText();
	
	public:
		int getNextAllowEditColumn(int nRow,int nCol);
		int getPirorAllowEditColumn(int nRow,int nCol);
		int getUpAllowEditColumn(int nRow,int nCol);
		int getDownAllowEditColumn(int nRow,int nCol);

		int getNullCell(POINT &pt);
		int setDWTableDelElement(CDwTable * pCDwTable, IXMLDOMElement * pElement);
		LPTSTR _tcstrim(const LPTSTR lpszStr);
		int ExpandRow();
		int ExpandRow(int sRow, int eRow);
		int ExpandMarginRow(int sRow, int eRow);
		int ExpandCellRow(int sRow, int eRow);

	#pragma   warning(default:4251) 
		
	};
}}}}

