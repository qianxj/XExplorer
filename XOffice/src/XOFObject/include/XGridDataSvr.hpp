#pragma once
#include "xofbase.h"
#include "igriddatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XGridDataSvr :
		public IGridDataSvr,
		public IAxisSvrImpl<XGridDataSvr>,
		public IContentSvrImpl<XGridDataSvr>,
		public XOfficeData
	{
	public:
		XGridDataSvr(void);
		~XGridDataSvr(void);
	public:
		//method for IGridDataSvr
		inline virtual XAPI STDMETHODIMP_(IAxisSvr*) GetIAxisSvr(){return m_pAxisSvr;}
		inline virtual XAPI STDMETHODIMP_(void) SetIAxisSvr(IAxisSvr* value){m_pAxisSvr = value;}
		inline virtual XAPI STDMETHODIMP_(IContentSvr*) GetIContentSvr(){return  m_pContentSvr;}
		inline virtual XAPI STDMETHODIMP_(void) SetIContentSvr(IContentSvr* value){ m_pContentSvr = value;}
	protected:
		IAxisSvr *	  m_pAxisSvr;
		IContentSvr*  m_pContentSvr;
	};
}}}
