#pragma once

#include "vxIObject.hxx"
namespace Hxsoft{ namespace XFrame
{
	class  XBASE_API xbObject
	{
	public:
		xbObject(void);
		virtual ~xbObject(void);
	public:
		LPTSTR	m_pStrUriObject;
		LPTSTR	m_pStrClassName;
		virtual bool SetClassName(LPTSTR pStrUri,LPTSTR pStrClassName); 
		virtual LPCTSTR GetClassName(LPCTSTR pStrUri); 
	public:
		xbObject * m_pInterFace;
		virtual xbObject * GetInterface(); 
	public:
		virtual int GetXml(BSTR &bstr);
		virtual int SetXml(LPTSTR pStrXml);
	public:
		long m_nRef;
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
