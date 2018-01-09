#pragma once

#include <exdisp.h>		// Defines of stuff like IWebBrowser2. This is an include file with Visual C 6 and above
#include <mshtml.h>		// Defines of stuff like IHTMLDocument2. This is an include file with Visual C 6 and above
#include <mshtmhst.h>	// Defines of stuff like IDocHostUIHandler. This is an include file with Visual C 6 and above
#include <crtdbg.h>		// for _ASSERT()
#include "xframe.h"

//#define NOTIMPLEMENTED _ASSERT(0); return(E_NOTIMPL)
#define NOTIMPLEMENTED return(E_NOTIMPL);

namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API xcOleControlContain : public xfControl
	{
	public:
		xcOleControlContain(void);
		~xcOleControlContain(void);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		HWND CreateControl(LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL);
		int AdjustControlRect(RECT rect,bool redraw);
		int OnClose();
	public:
		template<class T>
		class CComObject : public T
		{
			//IUnkown
		public:
			HRESULT STDMETHODCALLTYPE QueryInterface(  REFIID riid, LPVOID FAR* ppvObj);
			ULONG STDMETHODCALLTYPE AddRef();
			ULONG STDMETHODCALLTYPE Release();
		};

		//IOleInPlaceFrame
		class COleInPlaceFrame : public IOleInPlaceFrame
		{
		public:
			xcOleControlContain * m_pT;
		public:
			HRESULT STDMETHODCALLTYPE GetWindow(  HWND FAR* lphwnd);
			HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(  BOOL fEnterMode);
			HRESULT STDMETHODCALLTYPE GetBorder(  LPRECT lprectBorder);
			HRESULT STDMETHODCALLTYPE RequestBorderSpace(  LPCBORDERWIDTHS pborderwidths);
			HRESULT STDMETHODCALLTYPE SetBorderSpace(  LPCBORDERWIDTHS pborderwidths);
			HRESULT STDMETHODCALLTYPE SetActiveObject(  IOleInPlaceActiveObject *pActiveObject, LPCOLESTR pszObjName);
			HRESULT STDMETHODCALLTYPE InsertMenus(  HMENU hmenuShared, LPOLEMENUGROUPWIDTHS lpMenuWidths);
			HRESULT STDMETHODCALLTYPE SetMenu(  HMENU hmenuShared, HOLEMENU holemenu, HWND hwndActiveObject);
			HRESULT STDMETHODCALLTYPE RemoveMenus(  HMENU hmenuShared);
			HRESULT STDMETHODCALLTYPE SetStatusText(  LPCOLESTR pszStatusText);
			HRESULT STDMETHODCALLTYPE EnableModeless(  BOOL fEnable);
			HRESULT STDMETHODCALLTYPE TranslateAccelerator(  LPMSG lpmsg, WORD wID);
		};

		//IOleClientSite
		class COleClientSite : public IOleClientSite
		{
		public:
			xcOleControlContain * m_pT;
		public:
			HRESULT STDMETHODCALLTYPE SaveObject( );
			HRESULT STDMETHODCALLTYPE GetMoniker(  DWORD dwAssign, DWORD dwWhichMoniker, IMoniker ** ppmk);
			HRESULT STDMETHODCALLTYPE GetContainer(  LPOLECONTAINER FAR* ppContainer);
			HRESULT STDMETHODCALLTYPE ShowObject( );
			HRESULT STDMETHODCALLTYPE OnShowWindow(  BOOL fShow);
			HRESULT STDMETHODCALLTYPE RequestNewObjectLayout( );
		};

		//IDocHostUIHandler
		class CDocHostUIHandler  : public IDocHostUIHandler
		{
		public:
			xcOleControlContain * m_pT;
		public:
			HRESULT STDMETHODCALLTYPE ShowContextMenu(  DWORD dwID, POINT __RPC_FAR *ppt, IUnknown __RPC_FAR *pcmdtReserved, IDispatch __RPC_FAR *pdispReserved);
			HRESULT STDMETHODCALLTYPE GetHostInfo(  DOCHOSTUIINFO __RPC_FAR *pInfo);
			HRESULT STDMETHODCALLTYPE ShowUI(  DWORD dwID, IOleInPlaceActiveObject __RPC_FAR *pActiveObject, IOleCommandTarget __RPC_FAR *pCommandTarget, IOleInPlaceFrame __RPC_FAR *pFrame, IOleInPlaceUIWindow __RPC_FAR *pDoc);
			HRESULT STDMETHODCALLTYPE HideUI( );
			HRESULT STDMETHODCALLTYPE UpdateUI( );
			HRESULT STDMETHODCALLTYPE EnableModeless(  BOOL fEnable);
			HRESULT STDMETHODCALLTYPE OnDocWindowActivate(  BOOL fActivate);
			HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(  BOOL fActivate);
			HRESULT STDMETHODCALLTYPE ResizeBorder(  LPCRECT prcBorder, IOleInPlaceUIWindow __RPC_FAR *pUIWindow, BOOL fRameWindow);
			HRESULT STDMETHODCALLTYPE TranslateAccelerator(  LPMSG lpMsg, const GUID __RPC_FAR *pguidCmdGroup, DWORD nCmdID);
			HRESULT STDMETHODCALLTYPE GetOptionKeyPath(  LPOLESTR __RPC_FAR *pchKey, DWORD dw);
			HRESULT STDMETHODCALLTYPE GetDropTarget(  IDropTarget __RPC_FAR *pDropTarget, IDropTarget __RPC_FAR *__RPC_FAR *ppDropTarget);
			HRESULT STDMETHODCALLTYPE GetExternal(  IDispatch __RPC_FAR *__RPC_FAR *ppDispatch);
			HRESULT STDMETHODCALLTYPE TranslateUrl(  DWORD dwTranslate, OLECHAR __RPC_FAR *pchURLIn, OLECHAR __RPC_FAR *__RPC_FAR *ppchURLOut);
			HRESULT STDMETHODCALLTYPE FilterDataObject(  IDataObject __RPC_FAR *pDO, IDataObject __RPC_FAR *__RPC_FAR *ppDORet);
		};
		
		//IOleInPlaceSite
		class COleInPlaceSite :public IOleInPlaceSite
		{
		public:
			xcOleControlContain * m_pT;
		public:
			HRESULT STDMETHODCALLTYPE GetWindow(  HWND FAR* lphwnd);
			HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(  BOOL fEnterMode);
			HRESULT STDMETHODCALLTYPE CanInPlaceActivate( );
			HRESULT STDMETHODCALLTYPE OnInPlaceActivate( );
			HRESULT STDMETHODCALLTYPE OnUIActivate( );
			HRESULT STDMETHODCALLTYPE GetWindowContext(  LPOLEINPLACEFRAME FAR* lplpFrame,LPOLEINPLACEUIWINDOW FAR* lplpDoc,LPRECT lprcPosRect,LPRECT lprcClipRect,LPOLEINPLACEFRAMEINFO lpFrameInfo);
			HRESULT STDMETHODCALLTYPE Scroll(  SIZE scrollExtent);
			HRESULT STDMETHODCALLTYPE OnUIDeactivate(  BOOL fUndoable);
			HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate( );
			HRESULT STDMETHODCALLTYPE DiscardUndoState( );
			HRESULT STDMETHODCALLTYPE DeactivateAndUndo( );
			HRESULT STDMETHODCALLTYPE OnPosRectChange(  LPCRECT lprcPosRect);
		};

		class CServiceProvider :public IServiceProvider
		{
		public:
			xcOleControlContain * m_pT;
		public:
			STDMETHOD(QueryService)(REFGUID,REFIID,void**);
		};

		///////////////////////////////////////
		class CInternetSecurityManager :public IInternetSecurityManager
		{
		public:
			xcOleControlContain * m_pT;
		public:
			STDMETHOD(SetSecuritySite)(IInternetSecurityMgrSite*);
			STDMETHOD(GetSecuritySite)(IInternetSecurityMgrSite**);
			STDMETHOD(MapUrlToZone)(LPCWSTR,DWORD*,DWORD);
			STDMETHOD(GetSecurityId)(LPCWSTR,BYTE*,DWORD*,DWORD);
			STDMETHOD(ProcessUrlAction)(
				/* [in] */ LPCWSTR pwszUrl,
				/* [in] */ DWORD dwAction,
				/* [size_is][out] */ BYTE __RPC_FAR *pPolicy,
				/* [in] */ DWORD cbPolicy,
				/* [in] */ BYTE __RPC_FAR *pContext,
				/* [in] */ DWORD cbContext,
				/* [in] */ DWORD dwFlags,
				/* [in] */ DWORD dwReserved = 0);
			STDMETHOD(QueryCustomPolicy)(LPCWSTR,REFGUID,BYTE**,DWORD*,BYTE*,DWORD,DWORD);
			STDMETHOD(SetZoneMapping)(DWORD,LPCWSTR,DWORD);
			STDMETHOD(GetZoneMappings)(DWORD,IEnumString**,DWORD);
		};


	public:
		COleClientSite	  *  m_pClientSite;
		COleInPlaceSite   *  m_pInPlaceSite;
		COleInPlaceFrame  *  m_pInPlaceFrame;
		CDocHostUIHandler *  m_pDocHostUIHandler;
		CServiceProvider *  m_pServiceProvider;
		CInternetSecurityManager * m_pInternetSecurityManager;
	public:
		IOleObject		  *  m_pOleControl;
	public:
		xbObject * GetInterface(){return this;} 
	public:
		int LoadHtml(LPTSTR pStrHtml);
		int LoadText(LPTSTR pStrText);
		IHTMLDocument2 * GetHtmlDocument();
	public:
		int ExecWB(int cmdID, int cmdexecopt);
	};
}}