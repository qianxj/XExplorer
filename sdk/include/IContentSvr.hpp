#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{

	class XOFOBJECT_API IContentSvr :
		public CSvrbase
	{
	public:
		virtual XAPI STDMETHODIMP_(int) GetRowCount()=0;
		virtual XAPI STDMETHODIMP_(int) GetColCount()=0;

	public:
		enum eDataType{none,null,string,integer,decimal,datatime,time,date,boolean,image,object,xml,rtf,mail,report,xcell,pointer};
		virtual XAPI STDMETHODIMP_(void *) GetItemData(int ACol,int ARow)=0;
		virtual XAPI STDMETHODIMP_(bool)  SetItemData(int ACol,int ARow,void * pData)=0;
		virtual XAPI STDMETHODIMP_(eDataType) GetItemType(int ACol,int ARow)=0;
		virtual XAPI STDMETHODIMP_(bool) SetItemType(int ACol,int ARow, eDataType dt)=0;

	public:
		virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemString(int ACol,int ARow)=0;
		virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemString(LPTSTR pColName,int ARow)=0;
		virtual XAPI STDMETHODIMP_(bool)  SetItemString(int ACol,int ARow,LPTSTR pStr)=0;
		virtual XAPI STDMETHODIMP_(bool)  SetItemString(LPTSTR pColName,int ARow,LPTSTR pStr)=0;
	
		virtual XAPI STDMETHODIMP_(int) Retrieve(LPTSTR pXmlStr = NULL) =0;
		virtual XAPI STDMETHODIMP_(int) Update() = 0;

	};
}}}
