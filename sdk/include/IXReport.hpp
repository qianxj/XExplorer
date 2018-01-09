#pragma once

#include "xframe.h"
#include "xofobject.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XReport
{
	class XReportSheetSvr;
	class XREPORT_API  IXReport :
		public IDwView
	{
	public:
		IXReport(void);
		~IXReport(void);
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
		LPCTSTR GetNote(int nRow,int nColumn){return NULL;}//�õ���ע
		bool SetNote(int nRow,int nColumn,LPCTSTR pStrNote){return true;} //���ñ�ע
		bool AddNote(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL){return true;} //���ñ�ע
		
		LPCTSTR GetNote1(int nRow,int nColumn){return NULL;}//�õ��޸�
		bool SetNote1(int nRow,int nColumn,LPCTSTR pStrNote){return true;} //�����޸�
		bool AddNote1(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL) {return true;} //�����޸�

		LPCTSTR GetNote2(int nRow,int nColumn){return NULL;}//�õ���ע
		bool SetNote2(int nRow,int nColumn,LPCTSTR pStrNote) {return true;} //������ע
		bool AddNote2(int nRow,int nColumn,LPCTSTR pStrNote,LPCTSTR pUser=NULL,LPCTSTR pData=NULL){return true;} //������ע
	public:
		LPCTSTR GetItemString(int nRow,int nColumn,DWBuffer dwBuffer=dbuf_normal);
		LPCTSTR GetItemString(int nRow,LPCTSTR ColumnName,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemString(int nRow,int nColumn,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
		bool SetItemString(int nRow,LPCTSTR ColumnName,LPCTSTR lptStr,DWBuffer dwBuffer=dbuf_normal);
	public:
		bool SetItemStatus(int nRow,int nColumn,RowStatus status,DWBuffer dwBuffer=dbuf_normal){return false;}
		RowStatus GetItemStatus(int nRow,int nColumn, DWBuffer dwBuffer=dbuf_normal){return (RowStatus)-1;}
	public:
		int InsertRow(int nRow);
		int DeleteRow(int nRow);
	public:
		int InsertRows(int nRow,int nAmt){return -99;}//�������
		int DeleteRows(int nRow,int nAmt){return -99;};//ɾ������

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
	public:
		virtual int Retrieve(LPCTSTR pServer,LPCTSTR pDataUrl, LPCTSTR pArgStr) ;//��ȡ����
		virtual int Retrieve(ITrans *pTrans,LPCTSTR pDataUrl, LPCTSTR pArgStr);//��ȡ����
		virtual int Retrieve(LPCTSTR pDataUrl, LPCTSTR pArgStr) ;//��ȡ����

		virtual int Retrieve(IXMLDOMElement * pElement);//��ȡ����
		virtual int Retrieve(xbXml &xml);//��ȡ����

		virtual int RetrieveEx(LPCTSTR pServer,LPCTSTR pEntity, LPCTSTR pEntityKey){return -99;}//��ȡ����
		virtual int RetrieveEx(ITrans *pTrans,LPCTSTR pEntity, LPCTSTR pEntityKey) {return -99;}//��ȡ����
		virtual int RetrieveEx(LPCTSTR pEntity, LPCTSTR pEntityKey){return -99;}//��ȡ����
	public:
		virtual int SetDataObject(LPCTSTR pServer,LPCTSTR pUrl) ;//����ģ�����
		virtual int SetDataObject(ITrans *pTrans,LPCTSTR pUrl) ;//����ģ�����
		virtual int SetDataObject(LPCTSTR pUrl) ;//����ģ�����

		virtual int SetDataObject(IXMLDOMElement * pElement) ;//����ģ�����
		virtual int SetDataObject(IXMLDOMDocument * pDoc);
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
		int SetValues(LPCTSTR pColName,LPCTSTR pExpr){return -99;} //��������ֵ
		int AddCalcColumn(LPCTSTR pColName,LPCTSTR pExpr){return -99;}//���ù���������
		int ClearCalcColumn(LPCTSTR pColName = NULL){return -99;}//�������������
	public:
		int GetNextModifyRow(int nStart){return -99;} //�õ���һ�޸���
	public:
		int Modify(LPCTSTR pStr); //�﷨�޸�
		int Evaluate(int nRow,LPCTSTR pExpr){return -99;} //�������ʽ
	public:
		int SetAutoFilter(bool bAutoFilter){return -99;} //�����Զ�����
		int SetFilter(LPCTSTR pStrFilter){return -99;} //���ù�������
		int Filter(){return -99;} //����
		int SetSort(LPCTSTR pStrSort){return -99;} //�����������
		int Sort(){return -99;} //����
		int Find(LPCTSTR pCondition = NULL){return -99;} //����
		int FindNext(){return -99;} //����
	public:
		int GetFirstVisibleRow(){return -99;} //�õ���ǰ��ʾ�ĵ�һ��
		int Expand(bool bExpand){return -99;}//������չ��������ͼ
	public:
		int Print(LPCTSTR pXmlArg = NULL){return -99;} //��ӡ
		int PrintPreview(LPCTSTR pXmlArg = NULL){return -99;} //��ӡԤ��
	public:
		int SaveAs(LPCTSTR pXmlArg = NULL){return -99;} //���Ϊ
		int SaveAsEx(){return -99;} //�������Ϊ
		int SaveAsEx(LPCTSTR pStrFile){return -99;} //�������Ϊ
	public:
		LPTSTR GetColumnProps(LPCTSTR pColName){return NULL;} //�õ�������XML����
		int SetColumnProps(LPCTSTR pColName,LPCTSTR pXmlProp){return -99;} //����������XML����
		LPTSTR GetColumnProp(LPCTSTR pColName,LPCTSTR pItem){return NULL;} //�õ�������
		int SetColumnProp(LPCTSTR pColName,LPCTSTR pItem,LPCTSTR pProp){return -99;}//����������

		LPTSTR GetColumnProps(int nCol){return NULL;}; //�õ�������XML����
		int SetColumnProps(int nCol,LPCTSTR pXmlProp){return -99;}; //����������XML����
		LPTSTR GetColumnProp(int nCol,LPCTSTR pItem){return NULL;}; //�õ�������
		int SetColumnProp(int nCol,LPCTSTR pItem,LPCTSTR pProp){return -99;};//����������
	public:
		int DataBI(LPCTSTR pSchema = NULL){return -99;} //����BI����
	public:
		int chart(LPCTSTR pXmlArg = NULL){return -99;} //ͼ��

	public:
		int event_ItemFocusChanged(int nRow,int nCol);
		int event_RowFocusChanged(int nRow,int nOldRow);
		int event_GetFocus();
		int event_LoseFocus();
		int event_ItemChanged(int nRow,int nCol,LPCTSTR pData);
		int event_ItemChanging(int nRow,int nCol,LPCTSTR pData);
	public:
		int GetDwType();
	public:
		XReportSheetSvr * m_pSheetSvr;
		XReportSheetSvr * GetReportSheetSvr();
	public:
		IXMLDOMElement * GetRowElement(int nRow);
	public:
		int InsertCol(int nCol,LPCTSTR pCaption,LPCTSTR pColName,int nWidth,LPCTSTR pColPath = NULL,LPCTSTR pStyleClass=NULL,
			LPCTSTR pEditClass=NULL,LPCTSTR pFormatClass=NULL);
		int RemoveCol(int nCol);
		int RemoveVCol(int nCol);
		int RemoveCol(LPCTSTR pColName);

		int AddEditStyle(LPCTSTR pName, LPCTSTR pStr);
		int RemoveEditStyle(LPCTSTR pName, LPCTSTR pStr);
		int SetColEditStyle(LPCTSTR pColName,LPCTSTR pClassName);
		int SetDDLBData(LPCTSTR pColName,LPCTSTR pData);
		LPCTSTR GetDDLBData(LPCTSTR pColName);
	public:
		int GetContentRow(int nRow);
		int CreateTree(LPCTSTR pPath,int nCol, int nImage1,int nImage2);
	};
}}}}
