#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class  XOFOBJECT_API IBoundItem :
		public CSvrbase
	{
	public:
		virtual XAPI STDMETHODIMP_(TRect &) GetFRect()=0;
		virtual XAPI STDMETHODIMP_(int) SetFRect(TRect &rect)=0;
	};
}}}