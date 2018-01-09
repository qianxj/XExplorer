#pragma once

#include "xofbase.h"
#include "iboundItem.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class  XOFOBJECT_API IHeaderSelector :
		public IBoundItem
	{
	public:
		virtual XAPI STDMETHODIMP_(void) OnDraw(HDC hdc,RECT &rect) = 0;
	};
}}}