#pragma once

#include "ixmlcontentsvr.hpp"
#include <vector>

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XXmlContentSvr :
		public IXmlContentSvr 
	{
	public:
		XXmlContentSvr(void);
		~XXmlContentSvr(void); 
	public:
		set<wstring>* m_pBuffers;
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
				XAPI STDMETHODIMP_(LPTSTR) GetContent();
		//end method for IContentDbSvr

		//method for IXmlContentSvr
		virtual XAPI STDMETHODIMP_(bool) Load(LPTSTR pStrFile);
		virtual XAPI STDMETHODIMP_(bool) LoadXml(LPTSTR pStrXml);
		virtual XAPI STDMETHODIMP_(bool) Load(IXMLDOMElement* pElement) ;

		virtual XAPI STDMETHODIMP_(bool) SetXPath(LPTSTR pXPath) ;
		virtual XAPI STDMETHODIMP_(LPTSTR) GetXPath() ;
		virtual XAPI STDMETHODIMP_(bool) SetRootElement(IXMLDOMElement  * pRoot) ;
		virtual XAPI STDMETHODIMP_(bool) Initial(IXMLDOMElement  * pElement) ;

		virtual XAPI STDMETHODIMP_(IXMLDOMNode*) GetItemNode(int ACol,int ARow);
		virtual XAPI STDMETHODIMP_(IXMLDOMElement*) GetRowElement(int ARow) ;
		//end method for IXmlContentSvr

		virtual XAPI STDMETHODIMP_(int) Reset();
	public:
		static XXmlContentSvr * CreateInstance();
	public:
		int ResetUpdateStatus();
	public:
		int GetBIData(IXMLDOMDocument2 * pXmlDoc);
	public:
		int Sort(vector<wstring>* m_pCols,vector<bool>* m_pOrders,vector<wstring>* m_pColTypes, bool bMovePositon = false);
		vector<int>* m_pRows;
		//Í³¼ÆÁÐ
	public:
		LPCTSTR GetTotalItemString(LPCTSTR pTotleColumn);
		LPCTSTR GetTotalItemString(LPCTSTR pTotleColumn,LPCTSTR pFormat);
	public:
		LPTSTR GetColumnName(int ACol);
		int  GetColumnIndex(LPCTSTR pColName); 
	public:
		std::vector<LPTSTR >*  m_pXPaths;
		std::vector<LPTSTR >*  m_pCols;
		std::map<wstring,wstring>* m_pDDLBData;
		std::vector<std::vector<LPTSTR> >*  m_pQueryStr;

		std::map<std::wstring,std::wstring>* m_pTotalStr;
	public:
		bool MoveRow(int row, int movecount);
		bool FShowOrigin;
		int SetShowOrigin(bool f);
		int ColumnFilter(LPTSTR pColumn, LPTSTR pStr); 

	protected:
		xbXml	m_xbXml;

		IXMLDOMNodeList * m_pLists;
		IXMLDOMElement  * m_pRoot;
		LPTSTR m_pStrRoot;

	public:
		LPTSTR m_pUri;
		LPTSTR m_pUrl;

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
