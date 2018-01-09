#pragma once

#include "xofbase.h"
#include "icontentsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	template<class T>
	class XOFOBJECT_API IContentSvrImpl:
		public IContentSvr
	{
	public:
		//interface for IContentCountSvr
		inline virtual XAPI STDMETHODIMP_(int) GetRowCount()
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetRowCount();
		}
		inline virtual XAPI STDMETHODIMP_(int) GetColCount()
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetColCount();
		}
	public:
		//interface for IObjectContentSvr
		enum eDataType{none,null,string,integer,decimal,datatime,time,date,boolean,image,object,xml,rtf,mail,report,xcell,pointer};
		inline virtual XAPI STDMETHODIMP_(void *) GetItemData(int ACol,int ARow)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetItemData(ACol,ARow);
		}
		inline virtual XAPI STDMETHODIMP_(bool)  SetItemData(int ACol,int ARow,void * pData)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->SetItemData(ACol,ARow,pData);
		}
		inline virtual XAPI STDMETHODIMP_(IContentSvr::eDataType) GetItemType(int ACol,int ARow)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetItemType(ACol,ARow);

		}
		inline virtual XAPI STDMETHODIMP_(bool) SetItemType(int ACol,int ARow, IContentSvr::eDataType dt)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->SetItemType(ACol,ARow,dt);
		}
	public:
		//interface for IStringContentSvr
		inline virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemString(int ACol,int ARow)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetItemString(ACol,ARow);

		}
		inline virtual XAPI STDMETHODIMP_(LPCTSTR) GetItemString(LPTSTR pColName,int ARow)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->GetItemString(pColName,ARow);

		}
		inline virtual XAPI STDMETHODIMP_(bool)  SetItemString(int ACol,int ARow,LPTSTR pStr)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->SetItemString(ACol,ARow,pStr);
		}
		inline virtual XAPI STDMETHODIMP_(bool)  SetItemString(LPTSTR pColName,int ARow,LPTSTR pStr)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->SetItemString(pColName,ARow,pStr);
		}
	
	public:
		//interface for IContentDBSvr
		inline virtual XAPI STDMETHODIMP_(int) Retrieve(LPTSTR pXmlStr = NULL)
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->Retrieve(pXmlStr);
		}
		inline virtual XAPI STDMETHODIMP_(int) Update()
		{
			return dynamic_cast<T*>(this)->GetIContentSvr()->Update();
		}
	};
}}}
