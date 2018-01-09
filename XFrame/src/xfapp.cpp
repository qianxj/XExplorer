#include "StdAfx.h"
#include "xfApp.hpp"
#include "shlwapi.h"
#include "xfwinbase.hpp"
#include "xfNetwork.hpp"
#include "xshape.h"
#include "xfconnect.hpp"
#include "xfcachemgr.hpp"
#include "xfWin.hpp"
#include "xfControl.hpp"
#include "shellapi.h"
#include "wininet.h"
#include "xfuriloader.hpp"
#include "xfxpage.hpp"
#include "xfControl.hpp"
#include "resource.h"

#include "xssparse.h"
using namespace  xcode;

#include "xsm.h"
#include "parser/parser.hxx"
#include "parser/pcodes.hxx"
#include "tpp/parser_tpp.hxx"
#include "gen/gen_tpp.hxx"


#include "vlang.hpp"
#include "vlparser.hpp"
#include "vmlang.hpp"
#include "vmparser.hpp"

#include "wpp.h"
#include "wpp5.h"

XFRAME_API Hxsoft::XFrame::xfApp*  g_pXApp = NULL;


#ifdef JS_SUPPER
	/* The class of the global object. */  
	static JSClass global_class = {  
			"global", JSCLASS_GLOBAL_FLAGS,  
			JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,  
			JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,  
			JSCLASS_NO_OPTIONAL_MEMBERS  
		}; 

	/* The error reporter callback. */  
	void reportError(JSContext *cx, const char *message, JSErrorReport *report)  
	{  
		Hxsoft::XFrame::xfControl* pOutPut = Hxsoft::XFrame::xfApp::GetApp()->m_pOutPut;
		if(pOutPut)
		{
			HWND hWnd = pOutPut->GetHWND();
			int nLenText = ::GetWindowTextLength(hWnd);
			char buf[512];
			sprintf(buf, "%s:%u:%s\n",  
					report->filename ? report->filename : "<no filename>",  
					(unsigned int) report->lineno,  
					message);
			SendMessage(hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessageA(hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}else
		{
			::SetWindowText(pOutPut->m_hWnd,L"ÕýÔÚ±àÒë...\r\n");

					fprintf(stderr, "%s:%u:%s\n",  
					report->filename ? report->filename : "<no filename>",  
					(unsigned int) report->lineno,  
					message); 
		}
	}

	static void
	ReportException(JSContext *cx)
	{
		if (JS_IsExceptionPending(cx)) {
			if (!JS_ReportPendingException(cx))
				JS_ClearPendingException(cx);
		}
	}

	#include "jsatom.h"
	FILE *gOutFile=stderr;
	class ToString {
	  public:
		ToString(JSContext *aCx, jsval v, JSBool aThrow = JS_FALSE)
		  : cx(aCx), mThrow(aThrow)
		{
			mStr = JS_ValueToString(cx, v);
			if (!aThrow && !mStr)
				ReportException(cx);
			JS_AddNamedStringRoot(cx, &mStr, "Value ToString helper");
		}
		~ToString() {
			JS_RemoveStringRoot(cx, &mStr);
		}
		JSBool threw() { return !mStr; }
		jsval getJSVal() { return STRING_TO_JSVAL(mStr); }
		const char *getBytes() {
			if (mStr && (mBytes.ptr() || mBytes.encode(cx, mStr)))
				return mBytes.ptr();
			return "(error converting value)";
		}
	  private:
		JSContext *cx;
		JSString *mStr;
		JSBool mThrow;
		JSAutoByteString mBytes;
	};

	class IdStringifier : public ToString {
	public:
		IdStringifier(JSContext *cx, jsid id, JSBool aThrow = JS_FALSE)
			: ToString(cx, js::IdToJsval(id), aThrow)
		{ }
	};
	/*
	 * Define a JS object called "it".  Give it class operations that printf why
	 * they're being called for tutorial purposes.
	 */
	enum its_tinyid {
		ITS_COLOR, ITS_HEIGHT, ITS_WIDTH, ITS_FUNNY, ITS_ARRAY, ITS_RDONLY,
		ITS_CUSTOM, ITS_CUSTOMRDONLY
	};

	static JSBool
	its_getter(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
	  jsval *val = (jsval *) JS_GetPrivate(cx, obj);
	  *vp = val ? *val : JSVAL_VOID;
	  return JS_TRUE;
	}

	static JSBool
	its_setter(JSContext *cx, JSObject *obj, jsid id, JSBool strict, jsval *vp)
	{
	  jsval *val = (jsval *) JS_GetPrivate(cx, obj);
	  if (val) {
		  *val = *vp;
		  return JS_TRUE;
	  }

	  val = new jsval;
	  if (!val) {
		  JS_ReportOutOfMemory(cx);
		  return JS_FALSE;
	  }

	  if (!JS_AddValueRoot(cx, val)) {
		  delete val;
		  return JS_FALSE;
	  }

	  if (!JS_SetPrivate(cx, obj, (void*)val)) {
		  JS_RemoveValueRoot(cx, val);
		  delete val;
		  return JS_FALSE;
	  }

	  *val = *vp;
	  return JS_TRUE;
	}

	static JSPropertySpec its_props[] = {
		{"color",           ITS_COLOR,      JSPROP_ENUMERATE,       NULL, NULL},
		{"height",          ITS_HEIGHT,     JSPROP_ENUMERATE,       NULL, NULL},
		{"width",           ITS_WIDTH,      JSPROP_ENUMERATE,       NULL, NULL},
		{"funny",           ITS_FUNNY,      JSPROP_ENUMERATE,       NULL, NULL},
		{"array",           ITS_ARRAY,      JSPROP_ENUMERATE,       NULL, NULL},
		{"rdonly",          ITS_RDONLY,     JSPROP_READONLY,        NULL, NULL},
		{"custom",          ITS_CUSTOM,     JSPROP_ENUMERATE,
							its_getter,     its_setter},
		{"customRdOnly",    ITS_CUSTOMRDONLY, JSPROP_ENUMERATE | JSPROP_READONLY,
							its_getter,     its_setter},
		{NULL,0,0,NULL,NULL}
	};

	static JSBool
	its_bindMethod(JSContext *cx, uintN argc, jsval *vp)
	{
		JSString *name;
		JSObject *method;

		JSObject *thisobj = JS_THIS_OBJECT(cx, vp);

		if (!JS_ConvertArguments(cx, argc, JS_ARGV(cx, vp), "So", &name, &method))
			return JS_FALSE;

		*vp = OBJECT_TO_JSVAL(method);

		if (JS_TypeOfValue(cx, *vp) != JSTYPE_FUNCTION) {
			JSAutoByteString nameBytes(cx, name);
			if (!!nameBytes) {
				JSString *valstr = JS_ValueToString(cx, *vp);
				if (valstr) {
					JSAutoByteString valBytes(cx, valstr);
					if (!!valBytes) {
						JS_ReportError(cx, "can't bind method %s to non-callable object %s",
									   nameBytes.ptr(), valBytes.ptr());
					}
				}
			}
			return JS_FALSE;
		}

		jsid id;
		if (!JS_ValueToId(cx, STRING_TO_JSVAL(name), &id))
			return JS_FALSE;

		if (!JS_DefinePropertyById(cx, thisobj, id, *vp, NULL, NULL, JSPROP_ENUMERATE))
			return JS_FALSE;

		return JS_SetParent(cx, method, thisobj);
	}

	static JSFunctionSpec its_methods[] = {
		{"bindMethod",      its_bindMethod, 2,0},
		{NULL,NULL,0,0}
	};

	#ifdef JSD_LOWLEVEL_SOURCE
	/*
	 * This facilitates sending source to JSD (the debugger system) in the shell
	 * where the source is loaded using the JSFILE hack in jsscan. The function
	 * below is used as a callback for the jsdbgapi JS_SetSourceHandler hook.
	 * A more normal embedding (e.g. mozilla) loads source itself and can send
	 * source directly to JSD without using this hook scheme.
	 */
	static void
	SendSourceToJSDebugger(const char *filename, uintN lineno,
						   jschar *str, size_t length,
						   void **listenerTSData, JSDContext* jsdc)
	{
		JSDSourceText *jsdsrc = (JSDSourceText *) *listenerTSData;

		if (!jsdsrc) {
			if (!filename)
				filename = "typein";
			if (1 == lineno) {
				jsdsrc = JSD_NewSourceText(jsdc, filename);
			} else {
				jsdsrc = JSD_FindSourceForURL(jsdc, filename);
				if (jsdsrc && JSD_SOURCE_PARTIAL !=
					JSD_GetSourceStatus(jsdc, jsdsrc)) {
					jsdsrc = NULL;
				}
			}
		}
		if (jsdsrc) {
			jsdsrc = JSD_AppendUCSourceText(jsdc,jsdsrc, str, length,
											JSD_SOURCE_PARTIAL);
		}
		*listenerTSData = jsdsrc;
	}
	#endif /* JSD_LOWLEVEL_SOURCE */

	static JSBool its_noisy;    /* whether to be noisy when finalizing it */
	static JSBool its_enum_fail;/* whether to fail when enumerating it */

	static JSBool
	its_addProperty(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
		if (!its_noisy)
			return JS_TRUE;

		IdStringifier idString(cx, id);
		fprintf(gOutFile, "adding its property %s,", idString.getBytes());
		ToString valueString(cx, *vp);
		fprintf(gOutFile, " initial value %s\n", valueString.getBytes());
		return JS_TRUE;
	}

	static JSBool
	its_delProperty(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
		if (!its_noisy)
			return JS_TRUE;

		IdStringifier idString(cx, id);
		fprintf(gOutFile, "deleting its property %s,", idString.getBytes());
		ToString valueString(cx, *vp);
		fprintf(gOutFile, " initial value %s\n", valueString.getBytes());
		return JS_TRUE;
	}

	static JSBool
	its_getProperty(JSContext *cx, JSObject *obj, jsid id, jsval *vp)
	{
		if (!its_noisy)
			return JS_TRUE;

		IdStringifier idString(cx, id);
		fprintf(gOutFile, "getting its property %s,", idString.getBytes());
		ToString valueString(cx, *vp);
		fprintf(gOutFile, " initial value %s\n", valueString.getBytes());
		return JS_TRUE;
	}

	static JSBool
	its_setProperty(JSContext *cx, JSObject *obj, jsid id, JSBool strict, jsval *vp)
	{
		IdStringifier idString(cx, id);
		if (its_noisy) {
			fprintf(gOutFile, "setting its property %s,", idString.getBytes());
			ToString valueString(cx, *vp);
			fprintf(gOutFile, " new value %s\n", valueString.getBytes());
		}

		if (!JSID_IS_ATOM(id))
			return JS_TRUE;

		if (!strcmp(idString.getBytes(), "noisy"))
			JS_ValueToBoolean(cx, *vp, &its_noisy);
		else if (!strcmp(idString.getBytes(), "enum_fail"))
			JS_ValueToBoolean(cx, *vp, &its_enum_fail);

		return JS_TRUE;
	}

	/*
	 * Its enumerator, implemented using the "new" enumerate API,
	 * see class flags.
	 */
	static JSBool
	its_enumerate(JSContext *cx, JSObject *obj, JSIterateOp enum_op,
				  jsval *statep, jsid *idp)
	{
		JSObject *iterator;

		switch (enum_op) {
		  case JSENUMERATE_INIT:
		  case JSENUMERATE_INIT_ALL:
			if (its_noisy)
				fprintf(gOutFile, "enumerate its properties\n");

			iterator = JS_NewPropertyIterator(cx, obj);
			if (!iterator)
				return JS_FALSE;

			*statep = OBJECT_TO_JSVAL(iterator);
			if (idp)
				*idp = INT_TO_JSID(0);
			break;

		  case JSENUMERATE_NEXT:
			if (its_enum_fail) {
				JS_ReportError(cx, "its enumeration failed");
				return JS_FALSE;
			}

			iterator = (JSObject *) JSVAL_TO_OBJECT(*statep);
			if (!JS_NextProperty(cx, iterator, idp))
				return JS_FALSE;

			if (!JSID_IS_VOID(*idp))
				break;
			/* Fall through. */

		  case JSENUMERATE_DESTROY:
			/* Allow our iterator object to be GC'd. */
			*statep = JSVAL_NULL;
			break;
		}

		return JS_TRUE;
	}

	static JSBool
	its_resolve(JSContext *cx, JSObject *obj, jsid id, uintN flags,
				JSObject **objp)
	{
		if (its_noisy) {
			IdStringifier idString(cx, id);
			fprintf(gOutFile, "resolving its property %s, flags {%s,%s,%s}\n",
				   idString.getBytes(),
				   (flags & JSRESOLVE_QUALIFIED) ? "qualified" : "",
				   (flags & JSRESOLVE_ASSIGNING) ? "assigning" : "",
				   (flags & JSRESOLVE_DETECTING) ? "detecting" : "");
		}
		return JS_TRUE;
	}

	static JSBool
	its_convert(JSContext *cx, JSObject *obj, JSType type, jsval *vp)
	{
		if (its_noisy)
			fprintf(gOutFile, "converting it to %s type\n", JS_GetTypeName(cx, type));
		return JS_TRUE;
	}

	static void
	its_finalize(JSContext *cx, JSObject *obj)
	{
		jsval *rootedVal;
		if (its_noisy)
			fprintf(gOutFile, "finalizing it\n");
		rootedVal = (jsval *) JS_GetPrivate(cx, obj);
		if (rootedVal) {
		  JS_RemoveValueRoot(cx, rootedVal);
		  JS_SetPrivate(cx, obj, NULL);
		  delete rootedVal;
		}
	}

	static JSClass its_class = {
		"It", JSCLASS_NEW_RESOLVE | JSCLASS_NEW_ENUMERATE | JSCLASS_HAS_PRIVATE,
		its_addProperty,  its_delProperty,  its_getProperty,  its_setProperty,
		(JSEnumerateOp)its_enumerate, (JSResolveOp)its_resolve,
		its_convert,      its_finalize,
		JSCLASS_NO_OPTIONAL_MEMBERS
	};
#endif //JS_SUPPER

namespace Hxsoft{ namespace XFrame
{
	bool xfApp::m_bShutDown = false;
	xfApp::xfApp(void):m_hInst(NULL),m_hWnd(NULL),m_hMdiClient(NULL),m_pNetwork(new xfNetwork),
		m_pShapeMgr(NULL),m_pConnect(NULL),m_pFrame(NULL),m_bIdling(false),m_pUser(NULL),m_pTrans(NULL),
		m_xsscp(NULL),m_pDevFrame(NULL),m_pOutPut(NULL),m_tipcount(-1),m_xsmparser(NULL),m_vmlang(NULL),
		m_wpp(NULL),m_wpp5(NULL)
#ifdef JS_SUPPER
		,m_jsrt(NULL),m_jscx(NULL),m_jsglobal(NULL)
#endif
	{
		m_WorkEnv = _tcsdup(_T("office"));
		InitializeCriticalSection(&m_csCapture);
		m_pBrowerModulaPath = new TCHAR[MAX_PATH];
		m_pXConfigPath = new TCHAR[MAX_PATH];

		::GetModuleFileName(NULL,m_pBrowerModulaPath,MAX_PATH);
		PathRemoveFileSpec(m_pBrowerModulaPath);
		m_pXConfigPath[0]='\0';
		_tcscat_s(m_pXConfigPath,MAX_PATH,m_pBrowerModulaPath);
		_tcscat_s(m_pXConfigPath,MAX_PATH,_T("\\xconfig"));
		::ZeroMemory(&guid,sizeof(GUID));

		m_pShapeMgr = new XShapeMgr();
		 m_pCacheMgr = new xfCacheMgr();

		 m_pWins = new std::vector<HWND>;
#ifdef JS_SUPPER
		/* Create a JS runtime. You always need at least one runtime per process. */  
		m_jsrt = JS_NewRuntime(8 * 1024 * 1024);  
		if (m_jsrt == NULL)  
			return ;  
	  
		/*  
		 * Create a context. You always need a context per thread. 
		 * Note that this program is not multi-threaded. 
		 */  
		m_jscx = JS_NewContext(m_jsrt, 8192);  
		if (m_jscx == NULL)  
			return ;  
		JS_SetOptions(m_jscx, JSOPTION_VAROBJFIX | JSOPTION_JIT | JSOPTION_METHODJIT);  
		JS_SetVersion(m_jscx, JSVERSION_LATEST);  
		JS_SetErrorReporter(m_jscx, reportError);  


		JS_SetContextThread(m_jscx);
		/* 
		 * Create the global object in a new compartment. 
		 * You always need a global object per context. 
		 */
		JS_BeginRequest(m_jscx);
		m_jsglobal = JS_NewCompartmentAndGlobalObject(m_jscx, &global_class, NULL);  
		if (m_jsglobal == NULL)
		{
			JS_EndRequest(m_jscx);
			return ; 
		}
		/* 
		 * Populate the global object with the standard JavaScript 
		 * function and object classes, such as Object, Array, Date. 
		 */  
		if (!JS_InitStandardClasses(m_jscx, m_jsglobal)) 
		{
			JS_EndRequest(m_jscx);
			return ; 
		}

		JSObject *it = JS_DefineObject(m_jscx, m_jsglobal, "it", &its_class, NULL, 0);
        if (it)
		{
 			JS_EndRequest(m_jscx);
			return ; 
		}
        if (!JS_DefineProperties(m_jscx, it, its_props))
		{
 			JS_EndRequest(m_jscx);
			return ; 
		}
        if (!JS_DefineFunctions(m_jscx, it, its_methods))
		{
 			JS_EndRequest(m_jscx);
			return ; 
		}

        if (!JS_DefineProperty(m_jscx, m_jsglobal, "custom", JSVAL_VOID, its_getter,
                               its_setter, 0))
		{
 			JS_EndRequest(m_jscx);
			return ; 
		}
        if (!JS_DefineProperty(m_jscx, m_jsglobal, "customRdOnly", JSVAL_VOID, its_getter,
                               its_setter, JSPROP_READONLY))
		{
 			JS_EndRequest(m_jscx);
			return ; 
		}

		JS_EndRequest(m_jscx);
#endif 

	}

	xfApp::~xfApp(void)
	{
		DeleteCriticalSection(&m_csCapture);
		if(m_pBrowerModulaPath) delete m_pBrowerModulaPath;
		if(m_pXConfigPath)delete m_pXConfigPath;
		if(m_pNetwork)delete m_pNetwork;
		if(m_pShapeMgr) delete m_pShapeMgr;
		if(m_pWins) delete m_pWins;
		if(m_WorkEnv)delete m_WorkEnv;
		if(m_pUser)delete m_pUser;

		if(m_pTrans)delete m_pTrans;

		if(m_xsscp)
		{
			if(((xsscomplier*)m_xsscp)->m_pxvm) delete ((xsscomplier*)m_xsscp)->m_pxvm;
			delete ((xsscomplier*)m_xsscp);
		}
		if(m_xsmparser) delete (parser_cpp*)m_xsmparser;
		//if(m_wpp) delete (Wpp*)m_wpp;
		//if(m_wpp5) delete (xlang::Wpp5*)m_wpp;

#ifdef JS_SUPPER
	JS_ClearContextThread(m_jscx);
	//JS_DestroyContext(m_jscx);  
    JS_DestroyRuntime(m_jsrt);  
    JS_ShutDown();  
#endif
	}

	std::vector<HWND>* m_pWins;
	int xfApp::AddhWnd(HWND hWnd)
	{
		m_pWins->push_back(hWnd);
		return 1;
	}
	int xfApp::RemovehWnd(HWND hWnd)
	{
		for(int i=0;i<(int)m_pWins->size();i++)
		{
			if((*m_pWins)[i]==hWnd)
				m_pWins->erase(m_pWins->begin() + i);
		}
		return 1;
	}


	int xfApp::OnIdle(int nLevel)
	{
		/*
		if(this->m_pConnect)
		{
			TCHAR strCmd[512];
			::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/xchat/ackmsg"));
			if(!m_pConnect->SendCommand(m_pConnect->m_socket,strCmd)) return false;

			LPBYTE pData = NULL;
			int nLen;
			if(m_pConnect->ResponseCommand(m_pConnect->m_socket,pData,nLen)>=0)
			{
				if(nLen >=2)
				{
				}
			}
			if(pData)delete pData;
			
		}*/
		return false; //not process other
	}

	int xfApp::Idle(DWORD dwTime)
	{
		if(dwTime >= 100)
		{

			for(int i=0;i<(int)m_pWins->size();i++)
			{
				if(::IsWindow((*m_pWins)[i]) && ::IsWindowVisible((*m_pWins)[i]))
				{
					xfWin * pWin = dynamic_cast<xfWin *>((xfWinbase *)::GetProp((*m_pWins)[i],_T("this")));
					if(pWin)
					{
						pWin->ProcessMessage(pWin->m_hWnd,WM_IDLEUPDATECMDUI,0,0);
						xfWin::SendMessageToChilds(pWin->m_hWnd,WM_IDLEUPDATECMDUI,0,0,TRUE);
					}
				}
			}

			if(/*false*/ this->m_pConnect && this->m_pConnect->IsConnected())
			{
				return false;
				TCHAR strCmd[512];
				//::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/xchat/ackmsg"));
				::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/xquery:[test4.xq]"));
				if(!m_pConnect->SendCommand(m_pConnect->m_socket,strCmd)) return false;

				LPBYTE pData = NULL;
				int nLen;
				if(m_pConnect->ResponseCommand(m_pConnect->m_socket,pData,nLen)>=0)
				{
					if(nLen >=1 && !(pData[0]=='<' && pData[1]=='?'))
					{
						if(_tcslen((LPTSTR)pData) < 127)
						{
							NOTIFYICONDATA nid = {0};
							nid.cbSize   =   sizeof(nid);     
							nid.hWnd   =   m_hWnd;       
							nid.uID   =  1;       
							nid.uTimeout =0;

							//Update the icon of the notification
							nid.uFlags = NIF_ICON;
							m_tipcount ++;
							if(m_tipcount %6 < 3)
								nid.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(132));
							else
								nid.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(133));

							nid.uFlags |=  NIF_TIP;     
							_tcscpy_s(nid.szTip,128,(LPTSTR)pData);
							if(m_tipcount==0)
							{
								nid.uFlags |= NIF_INFO;
								nid.uTimeout = 60000;
								_tcscpy_s(nid.szInfo,sizeof(nid.szInfo)/sizeof(nid.szInfo[0]),(LPTSTR)pData);
							}else
							{
								nid.szInfo[0]=0;
							}

							Shell_NotifyIcon(NIM_MODIFY, &nid);
							if(m_tipcount %120==0 && m_tipcount<300)
								Beep(120, 120 );
							else if(m_tipcount %600==0 && m_tipcount<3600)
								Beep(120, 15 );
							else if(m_tipcount %1200==0)
								Beep(120, 15 );
						}

					}else
					{
						NOTIFYICONDATA nid = {0};
						nid.cbSize   =   sizeof(nid);     
						nid.hWnd   =   m_hWnd;       
						nid.uID   =  1;       

						//Update the icon of the notification
						nid.uFlags = NIF_ICON;
						nid.uFlags |= NIF_INFO;
						nid.szInfo[0]=0;

						nid.uFlags |=  NIF_TIP;     
						_tcscpy_s(nid.szTip,128,_T("this is application Explorer"));

						nid.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(133));
						Shell_NotifyIcon(NIM_MODIFY, &nid);
						m_tipcount = -1; 
					}

				}
				if(pData)delete pData;
			
			}
		}
		return 1;
	}
	
	int xfApp::InitApp()
	{
		return 1;
	}

	int xfApp::ExitApp()
	{
		return 1;
	}

	LRESULT CALLBACK xfApp::XFrameDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_INITDIALOG:
			return true;
		case WM_CLOSE:
			::EndDialog(hWnd,1);
			return true;
		default:
			return false;
		}
		return false;
	}

	LRESULT CALLBACK xfApp::XFrameWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		xfWinbase*  pThis;

#define   UM_ICONNOTIFY   WM_USER+100
		if(message==UM_ICONNOTIFY)
		{
			if(lParam==WM_LBUTTONDOWN && xfApp::GetApp()->m_pFrame)
			{
				/*
				TCHAR strCmd[512];
				::_stprintf_s(strCmd,512,_T("uri.hxsoft.com/xaserver/xchat/ackmsg"));
				if(!xfApp::GetApp()->m_pConnect->SendCommand(xfApp::GetApp()->m_pConnect->m_socket,strCmd)) return 0;

				LPBYTE pData = NULL;
				int nLen;
				if(xfApp::GetApp()->m_pConnect->ResponseCommand(xfApp::GetApp()->m_pConnect->m_socket,pData,nLen)>=0)
				{
					if(nLen >=1 && !(pData[0]=='<' && pData[1]=='?'))
					{
						TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
						DWORD dwSize = INTERNET_MAX_URL_LENGTH;
						xfUriLoader::UrlCombineW(xfApp::GetApp()->m_pFrame->m_pXPage->m_pStrUrl,_T("/business/xchat.xml"),strUrl,&dwSize,0);

						TCHAR name[255];
						int npos=0;
						int spos=0;
						while(npos<nLen)
						{
							
						
							xfWin*  pWinEx = xfWin::LoadXPage(NULL,strUrl,xfApp::GetApp()->m_pFrame->m_hWnd,xfApp::GetApp()->GetInstance(),_tcsdup(name));
						}
	
					}
				}
				if(pData)delete pData;
				*/
			}
		}
		if(message==WM_CREATE)
		{
			LPCREATESTRUCT lpcs = (LPCREATESTRUCT) lParam;
			if(lpcs->dwExStyle & WS_EX_MDICHILD)
			{
				MDICREATESTRUCTA * pMdics = (MDICREATESTRUCTA *)lpcs->lpCreateParams;
				if(pMdics)
				{
					if(pMdics->lParam)
					{
						::SetProp(hWnd,_T("this"),(HANDLE)pMdics->lParam);
						
					}
				}
			}
			else
			{
				if(lpcs->lpCreateParams)
				{
					::SetProp(hWnd,_T("this"),lpcs->lpCreateParams);
				}
			}
			if(!::GetParent(hWnd)) xfApp::GetApp()->AddhWnd(hWnd);
		}

		
		pThis = (xfWinbase *)(::GetProp(hWnd,_T("this")));
		if(pThis  && pThis->ProcessMessage(hWnd,message,wParam,lParam))
		{
			if(message==WM_ERASEBKGND) return true;
			if(message==WM_SETCURSOR) return true;
			return 0;
		}

		switch (message)
		{
		case WM_CLOSE:
			if(pThis)
			{
				xfWinbase * pBase = pThis;
				xfWin* pWin = dynamic_cast<xfWin * >(pBase);
				if(pWin)
				{
					if(pWin->OnCloseQuery()!=0) return 0;
				}
			}
			if(pThis && pThis->m_dwStatus & WST_RESPONSEWIN && pThis->m_dwStatus & WST_RESPONSELOOP)
			{
				pThis->m_dwStatus &= ~WST_RESPONSELOOP;
				return 0;
			}
				return DefWindowProc(hWnd, message, wParam, lParam);
		case WM_DESTROY:
			if(pThis)
			{
				xfWin * pWin = dynamic_cast<xfWin *>(pThis);
				if(pWin)pWin->OnClose();

				if(pWin && pWin->m_bAutoDeleted)
				{
					HMODULE hDLL = pWin->m_hDLL;

					//delete pWin;
					if(hDLL)FreeLibrary(hDLL);
				}
			}
			if(xfApp::GetApp()->m_pFrame && xfApp::GetApp()->m_pFrame==pThis)xfApp::GetApp()->m_pFrame = NULL;
			pThis = NULL;
			::RemoveProp(hWnd,_T("this"));
			if(!::GetParent(hWnd)) xfApp::GetApp()->RemovehWnd(hWnd);
			if(xfApp::GetApp()->m_hWnd == hWnd)::PostQuitMessage(0);
			break;
		default:
			{
				if(GetWindowLong(hWnd,GWL_EXSTYLE) & WS_EX_MDICHILD)
					return DefMDIChildProc(hWnd,message, wParam, lParam);
				else 
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		return 0;
	}

	int xfApp::XFrameStartUp(HINSTANCE hInstance,DWORD dwIcon,DWORD dwIconSm)
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW |CS_DBLCLKS ;
		wcex.lpfnWndProc	= XFrameWndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XEXPLORER));//dwIcon));
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= XFrameWndClass;
		wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));//dwIconSm));

		RegisterClassEx(&wcex);

		InitObjectMgr();

		return 1;
	}

	int xfApp::XFrameShutDown(HINSTANCE hInstance)
	{
		if(m_pConnect)
		{
			m_pConnect->CloseSocket(m_pConnect->m_socket);
			delete m_pConnect;
		}

		UnregisterClass(XFrameWndClass,hInstance);
		return 1;
	}

	HINSTANCE xfApp::GetInstance()
	{
		return m_hInst;
	}

	const wchar_t* xfApp::GetServerUrl()
	{
		if(!m_pDefaultView) return L"";
		if(!m_pDefaultView->m_pXPage) return L"";
		return m_pDefaultView->m_pXPage->m_pStrUrl;
	}

	xfApp * xfApp::GetApp()
	{
		return g_pXApp;
	}

	void xfApp::InitObjectMgr()
	{
		LPTSTR pFileObjects = new TCHAR[MAX_PATH];
		::_stprintf_s(pFileObjects,MAX_PATH,_T("%s\\objects.xml"),m_pXConfigPath);
		m_ObjectMgr.RegisterObjectsByXmlFile(pFileObjects);
		
		::_stprintf_s(pFileObjects,MAX_PATH,_T("%s\\shapes.xml"),m_pXConfigPath);
		m_pShapeMgr->RegisterShapesByFile(pFileObjects);
	}
	
	LPCTSTR xfApp::GetUser()
	{
		return m_pUser;
	}
	LPCTSTR xfApp::GetPassWord()
	{
		return m_pPassWord;
	}
	
	ITrans * xfApp::GetTrans(LPCTSTR pServerUrl,LPCTSTR pUser,LPCTSTR pAuthor)
	{
		if(m_pTrans) return m_pTrans;
		m_pTrans = new ITrans();
		m_pTrans->m_pServerUrl = _tcsdup(pServerUrl);
		return m_pTrans;
	}

	int xfApp::trace(LPCTSTR pstr)
	{
		if(!pstr) return 1;
		xfControl * pOut = xfApp::GetApp()->m_pOutPut;
		if(pOut && ::IsWindow(pOut->GetHWND()))
		{
			int nLenText = ::GetWindowTextLength(pOut->m_hWnd);
			SendMessage(pOut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOut->m_hWnd,EM_REPLACESEL,false,(LPARAM)pstr);
			SendMessage(pOut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}
		return 1;
	}
}}
