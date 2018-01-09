#pragma once
namespace Hxsoft{ namespace XFrame
{
	class XUTIL_API XByteStream : 
		public IStream
	{
	public:
		XByteStream(BYTE *pBytes,ULONG nByteSize) throw()
		{
			//ATLASSERT(pBytes);
			m_pArray = pBytes;
			m_dwRead = 0;
			m_nRef = 1;
			m_nByteSize = nByteSize;
		}

		~XByteStream()
		{
			if(m_pArray)delete m_pArray;
		}
	public:
		ULONG m_nRef;
	public:
		BYTE *m_pArray;
		ULONG m_nByteSize;
		DWORD m_dwRead;


		STDMETHOD(Read)(void *pv, ULONG cb, ULONG *pcbRead) throw()
		{
			if (!pv)
				return E_INVALIDARG;

			if (cb == 0)
				return S_OK;

			if (!m_pArray)
				return E_UNEXPECTED;

			BYTE *pCurr  = m_pArray;
			pCurr += m_dwRead;
			if(m_dwRead + cb > m_nByteSize) cb = m_nByteSize - m_dwRead;
			if(cb > 0) memcpy_s(pv, cb, pCurr, cb);
			if (pcbRead)
				*pcbRead = cb;
			m_dwRead += cb;
			return S_OK;
		}

		STDMETHOD(Write)(const void* , ULONG , ULONG* ) throw()
		{
			return E_UNEXPECTED;
		}

		STDMETHOD(Seek)(LARGE_INTEGER dlibMove , DWORD dwOrigin, ULARGE_INTEGER * plibNewPosition) throw()
		{
			switch(dwOrigin)
			{
			case STREAM_SEEK_SET :
				m_dwRead = dlibMove.LowPart;
				break;
			case STREAM_SEEK_CUR:
				m_dwRead += dlibMove.LowPart;
				break;
			case STREAM_SEEK_END:
				m_dwRead = m_nByteSize -  dlibMove.LowPart;
				break;
			}
			if(m_dwRead <0) m_dwRead=0;
			if(m_dwRead > m_nByteSize) m_dwRead = m_nByteSize;
			if(plibNewPosition)
			{
				plibNewPosition->LowPart = m_dwRead;
				plibNewPosition->HighPart = 0;
			}
			return S_OK;
		}

		STDMETHOD(SetSize)(ULARGE_INTEGER ) throw()
		{
			return E_NOTIMPL;
		}

		STDMETHOD(CopyTo)(IStream *, ULARGE_INTEGER , ULARGE_INTEGER *,
			ULARGE_INTEGER *) throw()
		{
			return E_NOTIMPL;
		}

		STDMETHOD(Commit)(DWORD ) throw()
		{
			return E_NOTIMPL;
		}

		STDMETHOD(Revert)( void) throw()
		{
			return E_NOTIMPL;
		}

		STDMETHOD(LockRegion)(ULARGE_INTEGER , ULARGE_INTEGER , DWORD ) throw()
		{
			return E_NOTIMPL;
		}

		STDMETHOD(UnlockRegion)(ULARGE_INTEGER , ULARGE_INTEGER ,
			DWORD ) throw()
		{
			return E_NOTIMPL;
		}

		STDMETHOD(Stat)(STATSTG * pStat, DWORD grfStatFlag) throw()
		{
			if(!pStat) return STG_E_INVALIDPOINTER;

			memset(pStat,0,sizeof(STATSTG));
			pStat->cbSize.HighPart = 0;
			pStat->cbSize.LowPart = this->m_nByteSize;
			pStat->type = STGTY_STREAM;
			return S_OK;
		}

		STDMETHOD(Clone)(IStream **) throw()
		{
			return E_NOTIMPL;
		}

		STDMETHOD(QueryInterface)(REFIID iid, void **ppUnk) throw()
		{
			*ppUnk = NULL;
			if (::InlineIsEqualGUID(iid, IID_IUnknown) ||
				::InlineIsEqualGUID(iid, IID_ISequentialStream) ||
				::InlineIsEqualGUID(iid, IID_IStream))
			{
				*ppUnk = (void*)(IStream*)this;
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		ULONG STDMETHODCALLTYPE AddRef( void)  throw()
		{
			return (ULONG)m_nRef++;
		}

		ULONG STDMETHODCALLTYPE Release( void)  throw()
		{
			m_nRef--;
			if(m_nRef==0)
				delete this;
			return (ULONG)m_nRef;
		}
	};
}}