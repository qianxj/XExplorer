#include "StdAfx.h"
#include "..\include\xcOleControlContain.hpp"

namespace Hxsoft{ namespace XFrame
{
	xcOleControlContain::xcOleControlContain(void): m_pOleControl(NULL)
	{
		m_pClientSite		= new CComObject<COleClientSite>();
		m_pInPlaceSite		= new CComObject<COleInPlaceSite>();
		m_pInPlaceFrame		= new CComObject<COleInPlaceFrame>();
		m_pDocHostUIHandler = new CComObject<CDocHostUIHandler>();
		m_pServiceProvider = new CComObject<CServiceProvider>(); 
		m_pInternetSecurityManager = new CComObject<CInternetSecurityManager>(); 

		m_pClientSite->m_pT			= this;
		m_pInPlaceSite->m_pT		= this;
		m_pInPlaceFrame->m_pT		= this;
		m_pDocHostUIHandler->m_pT	= this;
		m_pServiceProvider->m_pT = this;
		m_pInternetSecurityManager->m_pT = this;
	}

	xcOleControlContain::~xcOleControlContain(void)
	{
		/*
		if(m_pClientSite)		delete m_pClientSite;
		if(m_pInPlaceSite)		delete m_pInPlaceSite;
		if(m_pInPlaceFrame)		delete m_pInPlaceFrame;
		if(m_pDocHostUIHandler) delete m_pDocHostUIHandler;
		if(m_pServiceProvider) delete m_pServiceProvider;
		if(m_pInternetSecurityManager) delete m_pInternetSecurityManager;
		*/
		
		//if(m_pClientSite)		delete m_pClientSite;
		//if(m_pInPlaceSite)		delete m_pInPlaceSite;
		//if(m_pInPlaceFrame)		delete m_pInPlaceFrame;
		//if(m_pDocHostUIHandler) delete m_pDocHostUIHandler;
		//if(m_pServiceProvider) delete m_pServiceProvider;
		//if(m_pInternetSecurityManager) delete m_pInternetSecurityManager;
		
	}

	int xcOleControlContain::OnClose()
	{
		if(m_pOleControl)
		{
			m_pOleControl->SetClientSite(NULL);
			m_pOleControl->Close(OLECLOSE_NOSAVE);
			m_pOleControl->Release();
			m_pOleControl = NULL;
		}
		return 1;
	}

	int xcOleControlContain::AdjustControlRect(RECT rect,bool redraw)
	{
		xfControl::AdjustControlRect(rect,redraw);
		
		RECT rc={0,0,0,0};
		rc.right = rect.right - rect.left ;
		rc.bottom = rect.bottom - rect.top;

		IOleInPlaceObject	*inplace;
		if(m_pOleControl)
		{
			if (!m_pOleControl->QueryInterface(IID_IOleInPlaceObject, (void**)&inplace))
			{
				inplace->SetObjectRects( &rc, &rc);
				inplace->Release();
			}
		}
		return 1;
	}

	HWND xcOleControlContain::CreateControl(LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu,LPVOID lpParam )
	{
		HWND m_hWnd = xfControl::CreateControl(pszWndTitle,  rtPos, hWndParent,uID,hInstance,hMenu,lpParam);
		if(!m_hWnd) return NULL;

		_variant_t var;
		m_pxfNode->m_pElement->getAttribute(L"clsid",&var);
		if(var.bstrVal)
		{
			CLSID clsid;
			HRESULT hr;
			if(var.bstrVal[0]=='{')
				hr = CLSIDFromString(var.bstrVal,&clsid);
			else
				hr = CLSIDFromProgIDEx(var.bstrVal,&clsid);
			if(hr ==S_OK)
			{
				LPCLASSFACTORY		pClassFactory;
				IWebBrowser2		*webBrowser2;
				RECT				rect;
				pClassFactory = 0;

				if (!CoGetClassObject(clsid/*CLSID_WebBrowser*/, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER, NULL, IID_IClassFactory, (void **)&pClassFactory) && pClassFactory)
				{
					if (!pClassFactory->CreateInstance(0, IID_IOleObject, (void **)&m_pOleControl))
					{
						pClassFactory->Release();

						if (!m_pOleControl->SetClientSite(m_pClientSite))
						{
							m_pOleControl->SetHostNames(L"XExplorer", 0);
							::GetClientRect(m_hWnd, &rect);

							if (!OleSetContainedObject((struct IUnknown *)m_pOleControl, TRUE) &&
								!m_pOleControl->DoVerb(OLEIVERB_SHOW, NULL, m_pClientSite, -1, m_hWnd, &rect) &&
								!m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
							{
								webBrowser2->put_Left(0);
								webBrowser2->put_Top(0);
								webBrowser2->put_Width(rect.right);
								webBrowser2->put_Height(rect.bottom);

								VARIANT myURL;							
								VariantInit(&myURL);
								myURL.vt = VT_BSTR;

								
								this->m_pxfNode->m_pElement->getAttribute(L"url",&myURL);
								if(!myURL.bstrVal)
								{
									myURL.vt = VT_BSTR;
									myURL.bstrVal = SysAllocString(L"about:blank");
								}

								webBrowser2->Navigate2( &myURL, 0, 0, 0, 0);
								VariantClear(&myURL);
								webBrowser2->Release();

								// Success
								return m_hWnd;
							}
						}
					}
				}
			}
		}
		return m_hWnd;
	}

	int xcOleControlContain::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		return 1;
	}

	template<class T>
	HRESULT STDMETHODCALLTYPE xcOleControlContain::CComObject<T>::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
	{
		if (InlineIsEqualGUID(riid, IID_IUnknown)||InlineIsEqualGUID(riid,IID_IOleClientSite))
			*ppvObj = m_pT->m_pClientSite;

		else if (InlineIsEqualGUID(riid, IID_IOleInPlaceSite))
			*ppvObj = m_pT->m_pInPlaceSite;

		else if (InlineIsEqualGUID(riid, IID_IDocHostUIHandler))
			*ppvObj =  m_pT->m_pDocHostUIHandler;
		else if (InlineIsEqualGUID(riid, IID_IInternetSecurityManager))
			*ppvObj =  m_pT->m_pInternetSecurityManager;
		else if (InlineIsEqualGUID(riid, IID_IServiceProvider))
			*ppvObj =  m_pT->m_pServiceProvider;
		else
		{
			*ppvObj = 0;
			return(E_NOINTERFACE);
		}

		return(S_OK);
	}
	template<class T>
	ULONG STDMETHODCALLTYPE xcOleControlContain::CComObject<T>::AddRef( )
	{
		return 1;
	}
	template<class T>
	ULONG STDMETHODCALLTYPE xcOleControlContain::CComObject<T>::Release( )
	{
		return 1;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::ShowContextMenu( DWORD dwID, POINT __RPC_FAR *ppt, IUnknown __RPC_FAR *pcmdtReserved, IDispatch __RPC_FAR *pdispReserved)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::GetHostInfo( DOCHOSTUIINFO __RPC_FAR *pInfo)
	{
		pInfo->cbSize = sizeof(DOCHOSTUIINFO);
		pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER|DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE|DOCHOSTUIFLAG_LOCAL_MACHINE_ACCESS_CHECK|DOCHOSTUIFLAG_DISABLE_UNTRUSTEDPROTOCOL;
		pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::ShowUI( DWORD dwID, IOleInPlaceActiveObject __RPC_FAR *pActiveObject, IOleCommandTarget __RPC_FAR *pCommandTarget, IOleInPlaceFrame __RPC_FAR *pFrame, IOleInPlaceUIWindow __RPC_FAR *pDoc)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::HideUI()
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::UpdateUI()
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::EnableModeless( BOOL fEnable)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::OnDocWindowActivate( BOOL fActivate)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::OnFrameWindowActivate( BOOL fActivate)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::ResizeBorder( LPCRECT prcBorder, IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow)
	{
		return(S_OK);
	}

	// Called from the browser object's TranslateAccelerator routines to translate key strokes to commands.
	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::TranslateAccelerator( LPMSG lpMsg, const GUID __RPC_FAR *pguidCmdGroup, DWORD nCmdID)
	{
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::GetOptionKeyPath( LPOLESTR __RPC_FAR *pchKey, DWORD dw)
	{
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::GetDropTarget( IDropTarget __RPC_FAR *pDropTarget, IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget)
	{
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::GetExternal( IDispatch __RPC_FAR *__RPC_FAR *ppDispatch)
	{
		*ppDispatch = 0;
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::TranslateUrl( DWORD dwTranslate, OLECHAR __RPC_FAR *pchURLIn, OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut)
	{
		*ppchURLOut = 0;
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::CDocHostUIHandler::FilterDataObject( IDataObject __RPC_FAR *pDO, IDataObject __RPC_FAR *__RPC_FAR *ppDORet)
	{
		*ppDORet = 0;
		return(S_FALSE);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleClientSite::SaveObject()
	{
		return(E_NOINTERFACE);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleClientSite::GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk)
	{
		return(E_NOINTERFACE);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleClientSite::GetContainer( LPOLECONTAINER FAR* ppContainer)
	{
		*ppContainer = 0;

		return(E_NOINTERFACE);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleClientSite::ShowObject()
	{
		return(NOERROR);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleClientSite::OnShowWindow( BOOL fShow)
	{
		return(E_NOINTERFACE);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleClientSite::RequestNewObjectLayout()
	{
		return(E_NOINTERFACE);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::GetWindow( HWND FAR* lphwnd)
	{
		*lphwnd = m_pT->m_hWnd;

		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::ContextSensitiveHelp( BOOL fEnterMode)
	{
		return(E_NOINTERFACE);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::CanInPlaceActivate()
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::OnInPlaceActivate()
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::OnUIActivate()
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::GetWindowContext( LPOLEINPLACEFRAME FAR* lplpFrame, LPOLEINPLACEUIWINDOW FAR* lplpDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
	{
		*lplpFrame = m_pT->m_pInPlaceFrame;

		*lplpDoc = 0;

		if(lpFrameInfo)
		{
			lpFrameInfo->fMDIApp = FALSE;
			lpFrameInfo->hwndFrame = m_pT->GetWin()->m_hWnd;;
			lpFrameInfo->haccel = 0;
			lpFrameInfo->cAccelEntries = 0; 
		}

		//GetClientRect(lpFrameInfo->hwndFrame, lprcPosRect);
		//GetClientRect(lpFrameInfo->hwndFrame, lprcClipRect);


		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::Scroll( SIZE scrollExtent)
	{
		NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::OnUIDeactivate( BOOL fUndoable)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::OnInPlaceDeactivate()
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::DiscardUndoState()
	{
		return(E_NOINTERFACE);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::DeactivateAndUndo()
	{
		return(E_NOINTERFACE);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceSite::OnPosRectChange( LPCRECT lprcPosRect)
	{
		IOleInPlaceObject	*inplace;

		if(m_pT->m_pOleControl)
		{
			if (!m_pT->m_pOleControl->QueryInterface(IID_IOleInPlaceObject, (void**)&inplace))
			{
				inplace->SetObjectRects( lprcPosRect, lprcPosRect);
				inplace->Release();
			}
		}
		
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::GetWindow( HWND FAR* lphwnd)
	{
		*lphwnd = m_pT->GetWin()->m_hWnd;
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::ContextSensitiveHelp( BOOL fEnterMode)
	{
		NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::GetBorder( LPRECT lprectBorder)
	{
		NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::RequestBorderSpace( LPCBORDERWIDTHS pborderwidths)
	{
		return(E_NOTIMPL);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::SetBorderSpace( LPCBORDERWIDTHS pborderwidths)
	{
		return(E_NOTIMPL);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::SetActiveObject( IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::InsertMenus( HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths)
	{
		return(E_NOTIMPL);
		//NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::SetMenu( HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::RemoveMenus( HMENU hmenuShared)
	{
		NOTIMPLEMENTED;
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::SetStatusText( LPCOLESTR pszStatusText)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::EnableModeless( BOOL fEnable)
	{
		return(S_OK);
	}

	HRESULT STDMETHODCALLTYPE xcOleControlContain::COleInPlaceFrame::TranslateAccelerator( LPMSG lpmsg, WORD wID)
	{
		NOTIMPLEMENTED;
	}


	HRESULT xcOleControlContain::CInternetSecurityManager::SetSecuritySite (IInternetSecurityMgrSite *pSite)
	{
		return INET_E_DEFAULT_ACTION;
	}

	HRESULT xcOleControlContain::CInternetSecurityManager::GetSecuritySite(IInternetSecurityMgrSite **ppSite)
	{
		return INET_E_DEFAULT_ACTION;
	}

	HRESULT xcOleControlContain::CInternetSecurityManager::MapUrlToZone(LPCWSTR pwszUrl,DWORD *pdwZone,DWORD dwFlags)
	{
		return INET_E_DEFAULT_ACTION;
	}


	HRESULT   xcOleControlContain::CInternetSecurityManager::GetSecurityId(LPCWSTR pwszUrl,
		BYTE *pbSecurityId,
		DWORD *pcbSecurityId, 
		DWORD dwReserved)
	{

		return INET_E_DEFAULT_ACTION;
	}


	STDMETHODIMP xcOleControlContain::CInternetSecurityManager
		::ProcessUrlAction(	/* [in] */ LPCWSTR pwszUrl,
		/* [in] */ DWORD dwAction,
		/* [size_is][out] */ BYTE __RPC_FAR *pPolicy,
		/* [in] */ DWORD cbPolicy,
		/* [in] */ BYTE __RPC_FAR *pContext,
		/* [in] */ DWORD cbContext,
		/* [in] */ DWORD dwFlags,
		/* [in] */ DWORD dwReserved)
	{

		DWORD dwPolicy=URLPOLICY_ALLOW;

		if (dwAction <= URLACTION_ACTIVEX_MAX && dwAction >= URLACTION_ACTIVEX_MIN)
			dwPolicy = false ? URLPOLICY_DISALLOW : URLPOLICY_ALLOW;
		else if ((dwAction <= URLACTION_JAVA_MAX && dwAction >= URLACTION_JAVA_MIN) ||
			URLACTION_HTML_JAVA_RUN == dwAction)
			if (true)
				dwPolicy = URLPOLICY_JAVA_PROHIBIT;
			else
				return INET_E_DEFAULT_ACTION;
		else if (dwAction <= URLACTION_SCRIPT_MAX && dwAction >= URLACTION_SCRIPT_MIN)
			dwPolicy = false ? URLPOLICY_DISALLOW : URLPOLICY_ALLOW;
		else if (URLACTION_CROSS_DOMAIN_DATA == dwAction)
			dwPolicy = false ? URLPOLICY_ALLOW : URLPOLICY_DISALLOW;
		else
			return INET_E_DEFAULT_ACTION;

		if ( cbPolicy >= sizeof (DWORD))
		{
			*(DWORD*) pPolicy = dwPolicy;
			return S_OK;
		} 
		else 
		{
			return S_FALSE;
		}
	}

	HRESULT xcOleControlContain::CInternetSecurityManager
		::QueryCustomPolicy(LPCWSTR pwszUrl,
		REFGUID guidKey,
		BYTE **ppPolicy,
		DWORD *pcbPolicy,
		BYTE *pContext,
		DWORD cbContext,
		DWORD dwReserved)
	{
		return INET_E_DEFAULT_ACTION;
	}

	HRESULT xcOleControlContain::CInternetSecurityManager
		::SetZoneMapping(DWORD dwZone,
		LPCWSTR lpszPattern,
		DWORD dwFlags)
	{
		return INET_E_DEFAULT_ACTION;
	}

	HRESULT xcOleControlContain::CInternetSecurityManager
		::GetZoneMappings(DWORD dwZone,
		IEnumString **ppenumString, 
		DWORD dwFlags)
	{
		return INET_E_DEFAULT_ACTION;
	}

	/////////////////////////////////////////////////////////////
	STDMETHODIMP xcOleControlContain::CServiceProvider
		::QueryService(REFGUID guidService,  
		REFIID riid,
		void** ppvObject)
	{
		if (guidService == SID_SInternetSecurityManager && 
			riid == IID_IInternetSecurityManager)
		{
			HRESULT hr = QueryInterface(riid, ppvObject);
			return hr;
		} 
		else 
		{
			*ppvObject = NULL;

		}
		return E_NOINTERFACE;
	}

	int xcOleControlContain::ExecWB(int cmdID, int cmdexecopt)
	{
		IWebBrowser2	*webBrowser2;
		xcOleControlContain*  pControl = this;
		if(!pControl->m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
		{
			webBrowser2->ExecWB((OLECMDID)cmdID, (OLECMDEXECOPT)cmdexecopt, NULL, NULL);
		}
		return 1;
	}

	int xcOleControlContain::LoadHtml(LPTSTR pStrHtml)
	{
		IWebBrowser2	*webBrowser2;
		LPDISPATCH		lpDispatch;
		IHTMLDocument2	*htmlDoc2;
		SAFEARRAY		*sfArray;
		VARIANT			*pVar;
		BSTR			bstr = NULL;
		SAFEARRAYBOUND ArrayBound = {1, 0};


		xcOleControlContain*  pControl = this;
		if(!pControl->m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
		{
			if (!webBrowser2->get_Document(&lpDispatch))
			{
				if (!lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&htmlDoc2))
				{
					//htmlDoc2->put_designMode(L"on");
					if ((sfArray = SafeArrayCreate(VT_VARIANT, 1, (SAFEARRAYBOUND *)&ArrayBound)))
					{
						if (!SafeArrayAccessData(sfArray, (void**)&pVar))
						{
							pVar->vt = VT_BSTR;
							bstr = SysAllocString(pStrHtml);

							if ((pVar->bstrVal = bstr))
							{
								htmlDoc2->write(sfArray);
								htmlDoc2->close();
							}
						}
						SafeArrayDestroy(sfArray);
					}
					IHTMLElement *pElement;
					htmlDoc2->get_body(&pElement);
					if(pElement)
					{
						BSTR str;
						pElement->get_innerText(&str);
						int i=1;
						pElement->Release();
					}

					htmlDoc2->Release();
				}
				lpDispatch->Release();
			}
		}
		return 1;
	}

	int xcOleControlContain::LoadText(LPTSTR pStrText)
	{
		IWebBrowser2	*webBrowser2;
		LPDISPATCH		lpDispatch;
		IHTMLDocument2	*htmlDoc2;

		xcOleControlContain*  pControl = this;
		if(!pControl->m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
		{
			if (!webBrowser2->get_Document(&lpDispatch))
			{
				if (!lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&htmlDoc2))
				{
					IHTMLElement *pElement;
					htmlDoc2->get_body(&pElement);
					if(pElement)
					{
						pElement->put_innerText(pStrText);
						pElement->Release(); 
					}
					
				}
			}
		}
		return 1;
	}


	IHTMLDocument2 * xcOleControlContain::GetHtmlDocument()
	{
		IWebBrowser2	*webBrowser2;
		LPDISPATCH		lpDispatch;
		IHTMLDocument2	*htmlDoc2;
		xcOleControlContain*  pControl = this;
		if(!pControl->m_pOleControl->QueryInterface(IID_IWebBrowser2, (void**)&webBrowser2))
		{
			if (!webBrowser2->get_Document(&lpDispatch))
			{
				if (!lpDispatch->QueryInterface(IID_IHTMLDocument2, (void**)&htmlDoc2))
				{
					return htmlDoc2;
				}
				lpDispatch->Release();
			}
			webBrowser2->Release();
		}
		return NULL;
	}
}}
