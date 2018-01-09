#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	template<class T>
	class  XOFOBJECT_API IAxisSvrImpl :
		public IAxisSvr
	{
	public:
		inline virtual XAPI STDMETHODIMP_(int) GetColWidth(int column)
		{
			return dynamic_cast<T*>(this)->GetIAxisSvr()->GetColWidth(column); 
		}
		inline virtual XAPI STDMETHODIMP_(int) GetRowHeight(int row)
		{
			return dynamic_cast<T*>(this)->GetIAxisSvr()->GetRowHeight(row);
		}
		inline virtual XAPI STDMETHODIMP_(int) SetColWidth(int column,int value)
		{ 
			return dynamic_cast<T*>(this)->GetIAxisSvr()->SetColWidth(column,value);
		}
		inline virtual XAPI STDMETHODIMP_(int) SetRowHeight(int row ,int value)
		{
			return dynamic_cast<T*>(this)->GetIAxisSvr()->SetRowHeight(row,value); 
		}
		inline virtual XAPI STDMETHODIMP_(int) SetColWidth(int startcolumn,int endcolumn,int value) 
		{
			return dynamic_cast<T*>(this)->GetIAxisSvr()->SetColWidth(startcolumn,endcolumn,value);
		}
		inline virtual XAPI STDMETHODIMP_(int) SetRowHeight(int startrow ,int endrow, int value)
		{
			return dynamic_cast<T*>(this)->GetIAxisSvr()-> SetRowHeight(startrow,endrow,value);
		}

		inline virtual XAPI STDMETHODIMP_(int) GetSumRowHeight(int startrow,int endrow,int EffectiveLineHeight)
		{
			return dynamic_cast<T*>(this)->GetIAxisSvr()->GetSumRowHeight(startrow,endrow,EffectiveLineHeight);
		}
		inline virtual XAPI STDMETHODIMP_(int) GetSumColWidth(int  startcol,int endcol,int EffectiveLineWidth)
		{
			return dynamic_cast<T*>(this)->GetIAxisSvr()->GetSumRowHeight(startcol,endcol,EffectiveLineWidth);
		}
	};
}}}