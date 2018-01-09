#pragma once

#include "idatastoresvr.hpp"
#include <vector>

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XDataStoreSvr :
		public IDataStoreSvr
	{
	public:
		XDataStoreSvr(void);
		~XDataStoreSvr(void); 
	public:
		//method for IContentCountSvr
		virtual XAPI STDMETHODIMP_(int) GetRowCount();
		virtual XAPI STDMETHODIMP_(int) GetColCount();
		//end for IContentSvr

		//method for IContentObjectSvr
		virtual XAPI STDMETHODIMP_(void *) GetItemData(int ACol,int ARow);
		virtual XAPI STDMETHODIMP_(bool)  SetItemData(int ACol,int ARow,void * pData);
		virtual XAPI STDMETHODIMP_(eDataType) GetItemType(int ACol,int ARow);
		virtual XAPI STDMETHODIMP_(bool) SetItemType(int ACol,int ARow, eDataType dt);

		//method for IContentStringSvr
		virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemString(int ACol,int ARow);
		virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemString(LPTSTR pColName,int ARow);
		virtual XAPI STDMETHODIMP_(bool) SetItemString(int ACol,int ARow, LPTSTR pData);
		virtual XAPI STDMETHODIMP_(bool) SetItemString(LPTSTR pColName,int ARow, LPTSTR pData);
		        XAPI STDMETHODIMP_(bool) SetItemStringEx(LPTSTR pColName,int ARow, LPTSTR pData);

		virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemDisplayString(int ACol,int ARow);
		virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemDisplayString(LPTSTR pColName,int ARow);
		virtual XAPI STDMETHODIMP_(bool) SetItemDisplayString(int ACol,int ARow, LPTSTR pData);
		virtual XAPI STDMETHODIMP_(bool) SetItemDisplayString(LPTSTR pColName,int ARow, LPTSTR pData);
				XAPI STDMETHODIMP_(bool) SetItemDisplayStringEx(LPTSTR pColName,int ARow, LPTSTR pData);
		//end method for IStringContentSvr

		virtual XAPI STDMETHODIMP_(int) InsertRow(int nRow);
		virtual XAPI STDMETHODIMP_(int) DeleteRow(int nRow);

		//method for IContentDbSvr
		virtual XAPI STDMETHODIMP_(int) Retrieve(LPTSTR pXmlStr = NULL);
		virtual XAPI STDMETHODIMP_(int) Update();
		virtual XAPI STDMETHODIMP_(int) GetContent(BSTR &bstr);
		virtual XAPI STDMETHODIMP_(int) GetContent(BSTR &bstr,LPTSTR pGuid);
		virtual	XAPI STDMETHODIMP_(LPTSTR) GetContent();
		//end method for IContentDbSvr

		//method for IDatastoreSvr
		virtual	XAPI STDMETHODIMP_(bool) LoadData(unsigned char * pData);
		virtual	XAPI STDMETHODIMP_(bool) AppendData(unsigned char * pData);
		virtual	XAPI STDMETHODIMP_(bool) Reset();
	public:
			//原始数据结构
			/*
			struct TCOLINFo
			{
				ULONG size;
				ULONG columnCount;
				ULONG column offset[column count]
				struct ColumnTerms
				{
					ULONG dbtype;
					wchar_t* ColumnName;
				} columnTerms[column count]
			};
			*/
			/* 
			struct TROWINFO
			{
				ULONG size;
				ULONG state;
				ULONG follow;
				ULONG Column Offset[column count];
				Value value[column count];
			};
			*/
		struct TDatastore
		{
			std::vector<unsigned char* > pDatas; //origindata
			std::map<std::wstring /*colum name */, int/*column index*/> mCols;
			std::vector<wchar_t* >  m_cols;
			std::vector<WORD /*DBYPE*/> colsDBType;
			std::vector<int /*row data offset*/> rowOffset;
			std::vector<int /*row data source*/> rowOffsetSource;
			inline bool DestoryData()
			{
				for(std::size_t i=0;i<pDatas.size();i++) delete pDatas[i];
				return true;
			}
			inline bool Reset(bool DestroyOrigData = true)
			{
				if(DestroyOrigData) DestoryData();
				pDatas.clear();
				mCols.clear();
				m_cols.clear();
				colsDBType.clear();
				rowOffset.clear();
				rowOffsetSource.clear();
				return true;
			}
		};
		TDatastore* m_pDatastore;
	private:

	public:
		static XDataStoreSvr * CreateInstance();
	public:
		int ResetUpdateStatus();
	public:
		int GetBIData(IXMLDOMDocument2 * pXmlDoc);
		wchar_t* GetBIData(wchar_t* columns);
	public:
		int Sort(wchar_t* sortString );
		int Sort(vector<wstring>* m_pCols,vector<bool>* m_pOrders,vector<wstring>* m_pColTypes, bool bMovePositon = false);
		vector<int>* m_pRows;
		//统计列
	public:
		LPCTSTR GetTotalItemString(LPCTSTR pTotalColumn);
		LPCTSTR GetTotalItemString(LPCTSTR pTotalColumn,LPCTSTR pFormat);
	public:
		LPTSTR GetColumnName(int ACol);
		int  GetColumnIndex(LPCTSTR pColName); 
	public:
		std::vector<std::vector<std::wstring> >*  m_pQueryStr;

		std::map<std::wstring,std::wstring>* m_pTotalStr;
	public:
		bool MoveRow(int row, int movecount);
		bool FShowOrigin;
		int SetShowOrigin(bool f);
		int ColumnFilter(LPTSTR pColumn, LPTSTR pStr); 
		int Filter(); 

	public:
		typedef struct tagTreeItem
		{
			int nLevel;
			int nImageList;
			int nImage;
			int nSelectedImage;
			bool children;
			bool expanded;
			int nContentRow;
			int nContentCol;
			LPTSTR pGuid;
		}TTreeItem;

		vector<TTreeItem>* m_pvcTreeData;
		static  int GetTreeIndex(vector<TTreeItem>* pTreeData,int tRow); 
		virtual int GetTreeData(vector<TTreeItem>* & pvcTreeData){pvcTreeData = m_pvcTreeData; return m_pvcTreeData?1:-1;}
		int SetTreeData(vector<TTreeItem>* pTreeData);
		static int GetTreeNextItem(vector<TTreeItem>* pTreeData,int nIndex);
		static int GetTreeParentItem(vector<TTreeItem>* pTreeData,int nIndex);
		int GetContentRow(int nRow);
	};
}}}
