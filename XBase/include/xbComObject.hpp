#pragma once

#include "unknwn.h"
#include "xbobject.hpp"

namespace Hxsoft{ namespace XFrame
{
	template<class T>
	class  xbComObject : public T
	{
	public:
		xbComObject():m_nRef(0)
		{
			AddRef();
		}
		~xbComObject()
		{
		}
		long m_nRef;
	public:
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void** ppvObject)
		{
				*ppvObject = this;
				return S_OK;
		}
		virtual ULONG STDMETHODCALLTYPE AddRef( void)
		{
			return InterlockedIncrement(&m_nRef);
		}
		virtual ULONG STDMETHODCALLTYPE Release( void)
		{
			if (InterlockedDecrement(&m_nRef) == 0)
			{
				delete this;
				return 0;
			}
			return m_nRef;
		}
	};
}}
