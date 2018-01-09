#pragma once

#include "unknwn.h"

#define XINTERFACE_(x)  class __declspec(novtable) x
#define XAPI
#define XEVENT
#define XIMETHOD virtual STDMETHODIMP

namespace Hxsoft{ namespace XFrame
{
	XINTERFACE_(IObject): public IUnknown
	{
	public:
		virtual XAPI STDMETHODIMP_(bool) SetClassName(LPTSTR pStrUri,LPTSTR pStrClassName)=0; 
		virtual XAPI STDMETHODIMP_(LPCTSTR) GetClassName(LPCTSTR pStrUri)=0; 
	};
}}