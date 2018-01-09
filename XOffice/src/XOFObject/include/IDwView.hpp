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
		virtual int GetRow() = 0;//�õ���
		virtual int GetColumn() = 0;//�õ���
		virtual int GetRowCount() = 0; //�õ�����
		virtual int GetColumnCount() =0; //�õ�����
	public:
		virtual LPCTSTR GetNote(int nRow,int nColumn) = 0;//�õ���ע
		virtual bool SetNote(int nRow,int nColumn,LPCTSTR pStrNote) = 0; //���ñ�ע
		virtual bool AddNote(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) = 0; //���ñ�ע
		
		virtual LPCTSTR GetNote1(int nRow,int nColumn) = 0;//�õ��޸�
		virtual bool SetNote1(int nRow,int nColumn,LPCTSTR pStrNote) = 0; //�����޸�
		virtual bool AddNote1(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) = 0; //�����޸�

		virtual LPCTSTR GetNote2(int nRow,int nColumn) = 0;//�õ���ע
		virtual bool SetNote2(int nRow,int nColumn,LPCTSTR pStrNote) = 0; //������ע
		virtual bool AddNote2(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) = 0; //������ע

	public:
		virtual int InsertRow(int nRow) = 0;//������
		virtual int DeleteRow(int nRow) = 0;//ɾ����
		virtual int InsertRows(int nRow,int nAmt) = 0;//�������
		virtual int DeleteRows(int nRow,int nAmt) = 0;//ɾ������
	public:
		virtual LPCTSTR GetColumnName(int nColumn) = 0;//�õ�����
		virtual int GetColumnIndex(LPCTSTR pColumn) = 0; //�õ��к�
	public:
		virtual LPCTSTR GetItemString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal) = 0;//�õ�����ֵ
		virtual LPCTSTR GetItemString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer=dbuf_normal) = 0;//�õ�����ֵ
		virtual bool SetItemString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal) = 0;//��������ֵ
		virtual bool SetItemString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal) = 0;//��������ֵ
	public:
		virtual bool SetItemStatus(int nRow,int nColumn,RowStatus status,DWBuffer dwBuffer=dbuf_normal) = 0;//��������״̬
		virtual RowStatus GetItemStatus(int nRow,int nColumn, DWBuffer dwBuffer=dbuf_normal) = 0;//�õ�����״̬

	public:
		virtual int	SetKeyValue(int nRow,int nKeyValue) = 0;//���������ؽ�ֵ
		virtual int GetKeyValue(int nRow) = 0;//�õ������ؽ�ֵ
		virtual bool GetGuid(int nRow,TCHAR Guid[],int nMax) = 0;//����guid
		virtual bool SetGuid(int nRow,LPCTSTR pStrGuid) = 0; //����guid

	public:
		int SetReadOnly(bool bReadOnly=true); //����ֻ��
	public:
		int SelectRow(int nRow,bool bSelect=true);//ѡ�е���
		int SelectRow(int sRow,int eRow, bool bSelect=true);//ѡ�ж���
		int GetNextSelectRow(int nStartRow); //�õ������޸���
		bool IsRowSelected(int nRow); //�ж����Ƿ�ѡ��
		int SetSelectBkColor(COLORREF color); //����ѡ��ı�����ɫ
		int SetSelectionMode(int nMode); //����ѡ���ģʽ//0 unselection 1 singleSelection 2 multiselection 3 multiselectionex 

	public:
		virtual int	DeletedCount() = 0; //�õ�ɾ��������
		virtual int	ModifiedCount() = 0; //�õ��޸�������
	public:
		virtual int	Reset() = 0; //�������
		virtual int	ResetUpdateStatus() = 0;//����޸ı��
	public:
		virtual BSTR GetXml(LPCTSTR pRoot, bool bIncludeChild ){return NULL;} //��ȡXML����
		int  GetXml(BSTR &bstr){bstr = NULL; return -1;}//��ȡXML����
		int  GetXml(BSTR &bstr,LPCTSTR pGuid){bstr = NULL; return -1;} //��ȡXML����
		virtual int Load(IXMLDOMElement * pElement) = 0;//��ȡ����
		virtual int  LoadXml(BSTR bstr)=0;//��ȡ����
	public:
		virtual int AddObserver(xbObserver * pObserver) = 0; //�����¼���������
		virtual int RemoveObserver(xbObserver * pObserver) = 0;//ȥ���¼���������
		virtual int ClearObserver() = 0;//����¼���������
	public:
		virtual int DwUpdateTo(IXMLDOMDocument2 * pXmlDoc)=0;//�����ݸ��µ�XML������
		virtual int DwUpdateAllTo(IXMLDOMDocument2 * pXmlDoc)=0;//�����ݸ��µ�XML������
	public:
		virtual int Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr) = 0;//��ȡ����
		virtual int Retrieve(ITrans *pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr) = 0;//��ȡ����
		virtual int Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr) = 0;//��ȡ����

		virtual int Retrieve(IXMLDOMElement * pElement)=0;//��ȡ����
		virtual int Retrieve(xbXml &xml)=0;//��ȡ����

		virtual int RetrieveEx(LPCTSTR pServer,LPCTSTR pEntity, LPCTSTR pEntityKey) = 0;//��ȡ����
		virtual int RetrieveEx(ITrans *pTrans,LPCTSTR pEntity, LPCTSTR pEntityKey) = 0;//��ȡ����
		virtual int RetrieveEx(LPCTSTR pEntity, LPCTSTR pEntityKey) = 0;//��ȡ����
	public:
		virtual int SetDataObject(LPCTSTR pServer,LPCTSTR pUrl) = 0;//����ģ�����
		virtual int SetDataObject(ITrans *pTrans,LPCTSTR pUrl) = 0;//����ģ�����
		virtual int SetDataObject(LPCTSTR pUrl) = 0;//����ģ�����

		virtual int SetDataObject(IXMLDOMElement * pElement) = 0;//����ģ�����
		virtual int SetDataObject(IXMLDOMDocument * pDoc)=0;
	public:
		virtual int ImportFile()=0 ; //ת������
		virtual int ImportFile(LPCTSTR pStrFile,LPCTSTR pType=NULL,bool Header = false)=0; //ת������
		virtual int ImportString(LPCTSTR pStr,LPCTSTR pType=NULL,bool Header = false)=0; //ת���ı�����
	public:
		virtual int SetValues(LPCTSTR pColName,LPCTSTR pExpr)=0; //��������ֵ
		virtual int AddCalcColumn(LPCTSTR pColName,LPCTSTR pExpr)=0;//���ù���������
		virtual int ClearCalcColumn(LPCTSTR pColName = NULL)=0;//�������������
	public:
		virtual int GetNextModifyRow(int nStart)=0; //�õ���һ�޸���
	public:
		virtual int Modify(LPCTSTR pStr)=0; //�﷨�޸�
		virtual int Evaluate(int nRow,LPCTSTR pExpr)=0; //�������ʽ
	public:
		virtual int SetAutoFilter(bool bAutoFilter)=0; //�����Զ�����
		virtual int SetFilter(LPCTSTR pStrFilter)=0; //���ù�������
		virtual int Filter()=0; //����
		virtual int SetSort(LPCTSTR pStrSort)=0; //�����������
		virtual int Sort()=0; //����
		virtual int Find(LPCTSTR pCondition = NULL)=0; //����
		virtual int FindNext()=0; //����
	public:
		virtual int GetFirstVisibleRow()=0; //�õ���ǰ��ʾ�ĵ�һ��
		virtual int Expand(bool bExpand)=0;//������չ��������ͼ
	public:
		virtual int Print(LPCTSTR pXmlArg = NULL)=0; //��ӡ
		virtual int PrintPreview(LPCTSTR pXmlArg = NULL)=0; //��ӡԤ��
	public:
		virtual int SaveAs(LPCTSTR pXmlArg = NULL)=0; //���Ϊ
		virtual int SaveAsEx()=0; //�������Ϊ
		virtual int SaveAsEx(LPCTSTR pStrFile)=0; //�������Ϊ
	public:
		virtual LPTSTR GetColumnProps(LPCTSTR pColName)=0; //�õ�������XML����
		virtual int SetColumnProps(LPCTSTR pColName,LPCTSTR pXmlProp)=0; //����������XML����
		virtual LPTSTR GetColumnProp(LPCTSTR pColName,LPCTSTR pItem)=0; //�õ�������
		virtual int SetColumnProp(LPCTSTR pColName,LPCTSTR pItem,LPCTSTR pProp)=0;//����������

		virtual LPTSTR GetColumnProps(int nCol) = 0; //�õ�������XML����
		virtual int SetColumnProps(int nCol,LPCTSTR pXmlProp) = 0; //����������XML����
		virtual LPTSTR GetColumnProp(int nCol,LPCTSTR pItem) = 0; //�õ�������
		virtual int SetColumnProp(int nCol,LPCTSTR pItem,LPCTSTR pProp) = 0;//����������
	public:
		virtual int DataBI(LPCTSTR pSchema = NULL)=0; //����BI����
	public:
		virtual int chart(LPCTSTR pXmlArg = NULL)=0; //ͼ��

	//send event message
		virtual int event_ItemFocusChanged(int nRow,int nCol)=0;
		virtual int event_RowFocusChanged(int nRow,int nOldRow)=0;
		virtual int event_GetFocus()=0;
		virtual int event_LoseFocus()=0;
		virtual int event_ItemChanged(int nRow,int nCol,LPCTSTR pData)=0;
		virtual int event_ItemChanging(int nRow,int nCol,LPCTSTR pData)=0;


	//follow function for cell table
	public:
		virtual int InsertRow(int nGroup,int nRow){return -1;} //��������
		virtual int DeleteRow(int nGroup,int nRow){return -1;} //ɾ������
		virtual int InsertRows(int nGroup,int nRow,int nAmt){return -1;} //���������
		virtual int DeleteRows(int nGroup,int nRow,int nAmt){return -1;}//ɾ�������
	public:
		virtual int GetGroupStartRow(int nGroup){return -1;} //�õ�ָ����Ŀ�ʼ��
		virtual int GetGroupRowCount(int nGroup){return -1;} //�õ�ָ����Ľ�����
		virtual int InsertGroupRow(int nGroup,int nRow){return -1;} //��������
	public:
		virtual int GetMaxDeep(){return -1;} //����������ͼ��������
		virtual int GetLevel(){return -1;} //�õ�������ͼ�ļ���
		virtual LPCTSTR GetDwXPath(){return NULL;} //�õ�������ͼ��XPATH
	public:
		virtual IDwView * GetParent(){return NULL;} //�õ���������ͼ
	public:
		virtual int GetChildCount(){return -1;} //�õ���������ͼ��
		virtual IDwView * GetChild(int nIndex){return NULL;} //�õ�ָ����ŵ���������ͼ
	public:
		virtual LPCTSTR GetDwName(){return NULL;} //�õ�������ͼ������
		virtual IDwView * FindDwTable(LPCTSTR pStrName,IDwView *pParent = NULL){return NULL;} //����������ͼ
		virtual IDwView * GetCurTable(int &nRow,int &nCol){return NULL;}//�õ���ǰ������ͼ������������
		virtual IDwView * GetCurTable(){return NULL;}//�õ���ǰ������ͼ
	public:
		//���ý��׶���
		ITrans * m_pTrans;
		int SetTrans(ITrans * pTrans){m_pTrans = pTrans; return 1;}
		ITrans * GetTrans(){return m_pTrans;}
	public:
		virtual int GetDwType(){return DWT_CELL;}
	};
}}