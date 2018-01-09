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
		//���������
		int insertGroup(int group);	//������
		int deleteGroup(int group);	//ɾ����
		int DoSetGroupPreRow(int group); //ִ����Ԥ����
		int getGroupStartRow(int group); //�õ������ʼ��
		int getGroupStartCellRow(int group); //�õ������ʼ�����
		int getGroupFromRow(int row); //�������еõ���
		int getGroupFromCellRow(int cellRow); //�õ������ʼ�����
		bool getGroupCellArea(int group, int &sCellRow,int &eCellRow); //�õ���ı��������
		bool getGroupRowArea(int group, int &sRow,int &eRow); //�õ��������������
		int getGroupRowCount(int group);//�õ��������

		//���ݲ���
		int getColumns(vector<LPTSTR> &dataColumns,vector<POINT> &ColPoses); //�õ�������
		int getRowColumnFromCell(int ACol,int ARow,int &row,int &col); //�õ�����
		int getCellFromRowColumn(int row,int col,int &ACol,int &ARow); //�õ�����
		PStrItem getDwItem(int row,int col); //�õ�������
		int setDwItem(int row,int col, PStrItem pValue); //����������
		LPTSTR getDwItemText(int row,int col); //�õ�������
		int setDwItemText(int row,int col,LPTSTR strValue); //����������
		int getColumnCount(); //�õ�����
		LPTSTR getColumnName(int col); //�õ�����
		int getColumnIndex(LPTSTR pColName);//�õ��к�

		LPTSTR getDwItemDisplayText(int row,int col); //�õ�������
		int setDwItemDisplayText(int row,int col,LPTSTR strValue); //����������

		//���ݱ���������
		bool GetTableCellArea(int &sCellRow, int &eCellRow); //�õ����ݱ�ı��������

		//�����в���
		int SetRow(int row);//���ý���
		int GetRow();// �õ���ǰ��
		int GetRowFromCell(int cellRow);// ��������
		int GetRowArea(int row, int& sCellRow,int& eCellRow); //�õ��еı������
		int DoSetPreRows();//ִ��Ԥ������
		int GetRowCount();//�õ�����
		int DeleteRow(); //ɾ����
		int DeleteRow(int row); //ɾ����
		int DeleteGRow(int group ,int row); //ɾ����
		int InsertRow(); // ������
		int InsertRow(int row); // ������
		int InsertGRow(int group, int row); // ������
		int InsertGRow(int group, int row,int amt,bool redraw = true); // ������
		int AppendRow(void); //��չ��
		int AppendGRow(int group); //��չ��

		int AdjustDwTables(int sCellPos,int Number);
		static int AdjustDwTables(CDwTable * pDwTable,int sCellPos,int Number);
		static int AdjustDwTables(CGridFrameSvr* pGrid, int sCellPos,int Number);
		//�����в�������

		//���ݲ�����������

		//ģ�����
	public:
		bool LoadTemplData(int row, LPTSTR szGroupStr,CGridDataSvr * m_pData); //����ģ������
		int GetBandInfo(vector<POINT> &pts,vector<int> &levels, vector<wstring> &Bands,vector<CDwTable *> &pDws);//�õ�Band��Ϣ
		int GetMaxDeep(); //���ص�ǰ��������
		int GetLevel(); //���ص�ǰ�ļ���


	private:
		int GetRowGroupStartTempl(LPCTSTR szGroup,CGridDataSvr * m_pData); //�õ�ģ������ʼ�����
		int GetRowGroupEndTempl(LPCTSTR szGroup,CGridDataSvr * m_pData); //�õ�ģ����Ľ��������
		LPTSTR GetMatchGroupTempl(LPCTSTR szGroup,LPTSTR szGroupStr); //��ģ�崮��ƥ��ģ���飬���ض�Ӧ��ָ��
		LPTSTR TrimLeftGroupTempl(LPTSTR szGroupStr); //��ģ�崮��ȥ����ߵ�ģ����
		//ģ���������

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
		//�ؼ��ֺ���״̬
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
		//ģ�����
		bool				m_bDestroyTemplData;	//�Ƿ�Ҫɾ��ģ�����ݶ���
		int					m_RowGroupStartTempl;	//������ģ�����ݶ����е���ʼ��
		int					m_RowGroupEndTempl;	//������ģ�������еĽ�����
		CGridDataSvr*		m_pTemplGridData; //ģ������
		LPTSTR				m_strBoundGroup; //����󶨵�ģ����

		#pragma warning(disable: 4251)

		//���ݱ���
		CGridFrameSvr* m_pGrid;	//�����ı��
		vector<int> m_pRowCells; //ÿһ�а����ı��������
		int m_preRows; //��ʼԤ������

		//���������
		vector<int>			m_groups;		//ÿһ�����������
		vector<int>			m_groupstart;	//ÿһ�����ʼ��Ԫ������ڸ�����

		//�����Ķ���
		CDwTable *			m_pParent;   //������
		vector<CDwTable*>	m_pChilds;	//�Ӷ���

		//�б���
		vector<LPTSTR> m_dbcols;
		vector<POINT>  m_dbcolsPos;

		//���ӱ���
		vector<int> m_rowsIndex;
		vector<int> m_delrowsIndex;
		vector<IXMLDOMElement *> m_delXml;  //ɾ����xml
		vector<int> m_delGroup;  //ɾ����xml��Ӧ�������group

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

