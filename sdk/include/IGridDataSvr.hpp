#pragma once


#include "xofbase.h"
#include "iaxissvr.hpp"
#include "icontentsvr.hpp"
#include "iaxissvrimpl.hpp"
#include "icontentsvrimpl.hpp"
#include "iofficedatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API IGridDataSvr :
		public CSvrbase
	{
	public: 
		virtual XAPI STDMETHODIMP_(IAxisSvr*) GetIAxisSvr()=0; 
		virtual XAPI STDMETHODIMP_(void) SetIAxisSvr(IAxisSvr* value)=0; 
		virtual XAPI STDMETHODIMP_(IContentSvr*) GetIContentSvr()=0; 
		virtual XAPI STDMETHODIMP_(void) SetIContentSvr(IContentSvr* value)=0;
	};
}}}