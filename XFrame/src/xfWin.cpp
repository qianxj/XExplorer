#include "StdAfx.h"
#include "xuXUtil.h"
#include "xfnode.hpp"
#include "xfxpage.hpp"
#include "xfwinbase.hpp"
#include "xfWin.hpp"
#include "xfapp.hpp"
#include "xfmdiframe.hpp"
#include "shlwapi.h"
#include "xfuriloader.hpp"
#include "atlbase.h"
#include "xfcontrol.hpp"
#include "xfnodecontrol.hpp"
#include "ixfwin.hpp"
#include "xfNodeScript.hpp"

#include "xssparse.h"
using namespace  xcode;

#include "vlang.hpp"
#include "vlparser.hpp"
#include "vmlang.hpp"
#include "vmparser.hpp"
#include "vmtype.hpp"
#include "vmtuple.hpp"

#include "xsm.h"
#include "parser/parser.hxx"
#include "parser/pcodes.hxx"
#include "tpp/parser_tpp.hxx"
#include "gen/gen_tpp.hxx"
using namespace xsm;

#include "wpp.h"
#include "wpp5.h"


#define CALC_X(w,h) (h*h*w*2/(w*w+h*h))
#define CALC_Y(w,h) (w*w*h*2/(w*w+h*h))

namespace Hxsoft{ namespace XFrame
{
	extern int Filter1(unsigned int code, struct _EXCEPTION_POINTERS *ep);
	bool FetchData1(const tchar * p,void* param)
	{
		xfControl* pOutPut = (xfControl*)param;
		if(param)
		{
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)p);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}

		return true;
	}

	int xfWin::SetEventListions()
	{ 
		this->AttachListon(WM_SIZE,  (FNEvent)&xfWin::EvtSize );
		this->AttachListon(WM_PAINT, (FNEvent)&xfWin::EvtPaint);
		this->AttachListon(WM_XCOMMAND, (FNEvent)&xfWin::EvtXCommand);
		this->AttachListon(WM_DESTROY, (FNEvent)&xfWin::EvtDestroy);
		this->AttachListon(WM_KILLFOCUS, (FNEvent)&xfWin::EvtKillFocus);
		this->AttachListon(WM_ACTIVATE, (FNEvent)&xfWin::EvtActivate);
		this->AttachListon(WM_MDIACTIVATE, (FNEvent)&xfWin::EvtMdiActivate);
		this->AttachListon(WM_FOCUSNEXT, (FNEvent)&xfWin::EvtFocusNext);
		this->AttachListon(WM_FOCUSPREV, (FNEvent)&xfWin::EvtFocusPrev);
		this->AttachListon(WM_IDLEUPDATECMDUI, (FNEvent)&xfWin::EvtUpdateCmdUI);
		this->AttachListon(WM_ERASEBKGND,(FNEvent)&xfWin::EvtEraseBkgn);
		
		this->AttachListon(WM_SETCURSOR,(FNEvent)&xfWin::EvtSetCursor);
		this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xfWin::EvtLButtonDown);
		this->AttachListon(WM_LBUTTONUP,(FNEvent)&xfWin::EvtLButtonUp);

		return 0; 
	}
	int xfWin::UnSetEventListions()
	{ 
		this->ClearAllListonsTo();
		return 0;
	}

	HWND  xfWin::hHookMenu = NULL;
	HHOOK xfWin::hHook = NULL;
	xfWin::xfWin(void):m_hDLL(NULL),m_pXPage(NULL),m_pParam(NULL),m_pOwnerWin(NULL),m_pCmdWin(NULL),m_pParentWin(NULL),m_bAutoDeleted(true),m_pFrameParam(NULL),m_pPaper(NULL)
	{
		m_MousePt.x=0;
		m_MousePt.y=0;
		SetEventListions();
	}

	xfWin::~xfWin(void)
	{
		if(m_pXPage)delete m_pXPage;
		if(m_pPaper)delete m_pPaper;
		DetachListon();
	}

	int xfWin::OnOpen()
	{
		return 0; 
	}

	int xfWin::OnPostOpen()
	{
		return 0; 
	}

	int xfWin::OnPreOpen()
	{
		return 0; 
	}
	
	int xfWin::OnClose()
	{
		if(m_pXPage && m_pXPage->m_pxfNode)
		{
			m_pXPage->m_pxfNode->OnClose();
		}
		return 0;
	}
	int xfWin::OnCloseQuery()
	{
		return 0;
	}

	int xfWin::OnOpen(xfWin * pWin)
	{
		return 1;
	}
	
	int xfWin::Construct()
	{
		return 1;
	}


	xfWin* xfWin::LoadXPage(LPCTSTR pStrUri,LPCTSTR pStrUrl,DWORD dwExStyle,DWORD dwStyle, RECT *pBoundRect,HWND hParent,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		BYTE* pData = NULL;
		xfWin*  pWinEx = NULL;
		int nLen = 0;

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		LPTSTR  pStrRealPath = (LPTSTR)pStrUrl;

		if(hParent)
		{
			xfWin * pParentWin = (xfWin *)::GetProp(hParent,_T("this"));
			if(pParentWin && pParentWin->m_pXPage && pParentWin->m_pXPage->m_pStrUrl)
			{
				if(!wcsstr(pStrRealPath,L"dev:"))
				{
					xfUriLoader::UrlCombineW(pParentWin->m_pXPage->m_pStrUrl,pStrUrl,strUrl,&dwSize,0);
					pStrRealPath = strUrl;
				}
				xfApp::GetApp()->m_pFrame = pParentWin->GetFrameWindow();
			}
		}
		if(xfUriLoader::Load(pStrRealPath, pStrUri,pData,nLen)==1)
		{
			TCHAR Path[MAX_PATH];
			xfUriLoader::GetCacheFileName(pStrRealPath,Path);

			xfXPage * pXPage = new xfXPage;
			pXPage->m_pStrUrl = _tcsdup(pStrRealPath);
			//::_tcscpy_s(pXPage->m_pStrUrl,INTERNET_MAX_URL_LENGTH,pStrRealPath);
			pXPage->Load(Path);
			
			if(!pXPage->m_pxfNode)
			{
				delete pXPage;
				return NULL;
			}
			IXMLDOMElementPtr pNodeEx ;
			if(pXPage->m_pElement)
				pNodeEx = pXPage->m_pElement;
			else
				pXPage->m_pxbXml->m_pXmlDoc->get_documentElement(&pNodeEx);

			pWinEx = CreateWinObject(pNodeEx,pXPage->m_pStrUrl);
			if(!pWinEx) pWinEx = new xfWin();
			if(pWinEx)pWinEx->Open(pXPage,dwExStyle, dwStyle, pBoundRect,hParent,hInst,pParam,pCmdWin);
		}

		if(pData)
			xfUriLoader::FreeData(pData);

		return pWinEx;
	}


	xfWin* xfWin::LoadXPage(LPCTSTR pStrUri,LPCTSTR pStrUrl,HWND hParent,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		BYTE* pData = NULL;
		xfWin*  pWinEx = NULL;
		int nLen = 0;

		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;
		LPTSTR pStrRealPath = (LPTSTR)pStrUrl;

		if(hParent)
		{
			xfWin * pParentWin = (xfWin *)::GetProp(hParent,_T("this"));
			if(pParentWin)
			{
				xfUriLoader::UrlCombineW(pParentWin->m_pXPage->m_pStrUrl,pStrUrl,strUrl,&dwSize,0);
				pStrRealPath = strUrl;
				xfApp::GetApp()->m_pFrame = pParentWin->GetFrameWindow();
			}
		}
		if(xfUriLoader::Load(pStrRealPath, pStrUri,pData,nLen)==1)
		{
			TCHAR Path[MAX_PATH];
			xfUriLoader::GetCacheFileName(pStrRealPath,Path);

			xfXPage * pXPage = new xfXPage;
			//::_tcscpy_s(pXPage->m_pStrUrl,INTERNET_MAX_URL_LENGTH,pStrRealPath);
			pXPage->m_pStrUrl = _tcsdup(pStrRealPath);
			pXPage->Load(Path);
			pWinEx = LoadXPageXml(pXPage, hParent,hInst,pParam,pCmdWin);
		}
		if(pData)
			xfUriLoader::FreeData(pData);

		return pWinEx;
	}
	
	xfWin* xfWin::LoadXPage(IXMLDOMElement * pElement,HWND hParent,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		xfXPage * pXPage = new xfXPage;
		pXPage->Load(pElement);
		return LoadXPageXml(pXPage, hParent,hInst,pParam,pCmdWin);
	}

	xfWin* xfWin::LoadXPage(LPCTSTR pXPageFile,HWND hParent,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		xfXPage * pXPage = new xfXPage;
		pXPage->Load(pXPageFile);
		return LoadXPageXml(pXPage, hParent,hInst,pParam,pCmdWin);
	}
	xfWin* xfWin::LoadXPageXml(LPCTSTR pXPageXml,HWND hParent,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		xfXPage * pXPage = new xfXPage;
		pXPage->LoadXPage(pXPageXml);
		return LoadXPageXml(pXPage, hParent,hInst,pParam,pCmdWin);
	}

	xfWin * CreateWinObjectFromHandle(HMODULE hDLL,LPTSTR pGuid,LPTSTR pWinID ,LPTSTR pWinClass,LPTSTR strWinType)
	{
		if(!hDLL) return NULL;
		if(!pGuid && !pWinID && !pWinClass && !strWinType) return NULL;

		xfWin * pWin = NULL;
		//init window
		xfWin *  (WINAPI * fnCreateInstance)(LPTSTR pWinClass,LPTSTR pWinName) = NULL;
		fnCreateInstance = (xfWin *  (WINAPI * )(LPTSTR pWinClass,LPTSTR pWinName))::GetProcAddress(hDLL,"CreateInstance");
		if(fnCreateInstance)
		{
			if(pGuid)pWin = fnCreateInstance(NULL,pGuid);
			if(pWin) return pWin;
			if(strWinType && ::_tcscmp(_T("mdiframe"),strWinType)==0 && !pWinClass)
			{
				pWin = fnCreateInstance(_T("mdiframe"),pWinID);
			}
			else
			{
				pWin = fnCreateInstance(pWinClass,pWinID);
			}
		}
		return pWin;
	}
	
	HMODULE LoadScriptDll(LPCTSTR pStrUrl,LPCTSTR pstrWorkGuid)
	{
		return NULL;
		if(!pstrWorkGuid) return NULL;

		TCHAR Path[MAX_PATH];
		TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
		DWORD dwSize = INTERNET_MAX_URL_LENGTH;

		LPBYTE pData=NULL;
		int len;

		TCHAR sModula[MAX_PATH];
		TCHAR sModulaEx[MAX_PATH];
		::GetModuleFileName(xfApp::GetApp()->GetInstance(),sModula,255);
		int pos = (int)::_tcslen(sModula);
		while(pos>=0 && sModula[pos]!='\\')pos--;
		if(pos >=0)sModula[pos] = 0;

		_stprintf_s(sModulaEx,MAX_PATH,_T("%s\\ext\\%s.dll"),sModula,pstrWorkGuid);
		if(!PathFileExists(sModulaEx))
		{
			_stprintf_s(sModula,MAX_PATH,_T("%s\\ext\\%sT%u.dll"),sModula,pstrWorkGuid,GetTickCount()%0x80);

			_stprintf_s(Path,MAX_PATH,_T("/developer/make/dll/%s.dll"),pstrWorkGuid);
			xfUriLoader::UrlCombineW(pStrUrl,Path,strUrl,&dwSize,0);
			if(xfUriLoader::Load(strUrl,NULL,pData,len,false)>=0)
			{
				if(PathFileExists(sModula))::DeleteFile(sModula);
				DWORD dwSize;
				HANDLE fp1 = CreateFile(sModula,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
				WriteFile(fp1,pData,len,&dwSize,NULL);
				::CloseHandle(fp1);
			}
			if(pData)xfUriLoader::FreeData(pData);
			return ::LoadLibrary(sModula);
		}else
			return ::LoadLibrary(sModulaEx);

	}

	xfWin * xfWin::CreateWinObject(IXMLDOMElement * pNodeEx,LPCTSTR pStrUrl)
	{
		xfWin * pWin = NULL; 
		VARIANT	val;
		LPTSTR strWinType = NULL;
		LPTSTR pXScript = NULL;
		LPTSTR pXDll = NULL;
		LPTSTR pstrGuid = NULL;
		LPTSTR pstrWorkGuid = NULL;
		LPTSTR pWinID = NULL;
		LPTSTR pWinClass = NULL;

		pNodeEx->getAttribute(L"type",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			strWinType = XUtil::tcsdupFromBstr(val.bstrVal); 
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"xscript",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pXScript = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"xdll",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pXDll = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"name",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pWinID = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"windowclass",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pWinClass = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"workguid",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pstrWorkGuid = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"guid",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pstrGuid = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		HMODULE hDLL = NULL;
		if(pXScript && pstrWorkGuid) hDLL = LoadScriptDll(pStrUrl, pstrWorkGuid);
		if(hDLL) pWin = CreateWinObjectFromHandle(hDLL,pstrGuid,pWinID,pWinClass,strWinType);
		if(!pWin)
		{
			if(hDLL)::FreeModule(hDLL);
			hDLL = NULL;
			if(pXDll)
			{
				hDLL = ::LoadLibrary(pXDll);
				if(!hDLL)
				{
					TCHAR path[MAX_PATH];
					::GetModuleFileName(NULL,path,MAX_PATH);
					::PathRemoveFileSpec(path);
					::PathCombine(path,path,pXDll);
					hDLL = ::LoadLibrary(path); 
					if(!hDLL)
					{
						//MessageBox(0,pXDll,L"Error",MB_OK);
						TCHAR szBuf[80]; 
						LPVOID lpMsgBuf;
						DWORD dw = GetLastError(); 

						FormatMessage(
							FORMAT_MESSAGE_ALLOCATE_BUFFER | 
							FORMAT_MESSAGE_FROM_SYSTEM,
							NULL,
							dw,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
							(LPTSTR) &lpMsgBuf,
							0, NULL );

						wsprintf(szBuf, 
							L"%s %s failed with error %d: %s", 
							L"LoadLibrary",path, dw, lpMsgBuf); 
					 
						MessageBox(NULL, szBuf, L"Error", MB_OK); 

						LocalFree(lpMsgBuf);
					}
				}
			}
			if(hDLL) pWin = CreateWinObjectFromHandle(hDLL,pstrGuid,pWinID,pWinClass,strWinType);
			if(!pWin && hDLL) ::FreeModule(hDLL);
		}

		if(hDLL && pWin)pWin->m_hDLL = hDLL;
		if(pXScript) delete  pXScript; 
		if(pXDll) delete pXDll;
		if(pstrGuid) delete pstrGuid;
		if(pstrWorkGuid) delete pstrWorkGuid;
		if(pWinID) delete pWinID;
		if(pWinClass) delete pWinClass;

		

		return pWin;
	}

	xfWin* xfWin::LoadXPageXml(xfXPage * pXPage,HWND hParent,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		if(!pXPage->m_pxfNode)
		{
			delete pXPage;
			return NULL;
		}
		IXMLDOMElementPtr pNodeEx ;
		if(pXPage->m_pElement)
			pNodeEx = pXPage->m_pElement;
		else
			pXPage->m_pxbXml->m_pXmlDoc->get_documentElement(&pNodeEx);

		xfWin * pWin = CreateWinObject(pNodeEx,pXPage->m_pStrUrl);
		if(!pWin) pWin = new xfWin();
		pWin->Open(pXPage,hParent,hInst,pParam,pCmdWin);
		return pWin;
	}

	bool xfWin::Open(xfXPage * pXPage,HWND hParent,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		return Open(pXPage,0,WS_OVERLAPPEDWINDOW,NULL,hParent,hInst,pParam,pCmdWin);		
	}

	class wlerror5 : public xlang::wErr
	{
	public:
		xfControl* m_pOutPut;
		xfControl* m_pDataOutPut;
		xfControl* m_pPCodeOutPut;
		int adderror(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;

			const TCHAR * psz = pErrText;

			tchar buf[255];
			if(pFile)
				_stprintf_s(buf,255,L"%s(%d,%d)  错误: 0x%x %s\r\n",pFile,nrow,ncol,nError ,pErrText);
			else
				_stprintf_s(buf,255,L"第%d行 第%d列  错误: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}

		int print(wchar_t* format,...)
		{
			m_pDataOutPut= m_pOutPut;
			if(!m_pDataOutPut) return 0;

			wchar_t* arg;
			int nLen = wcslen(format);

			int argCount = 0;
			for(int i=0; i<nLen; i++)
			{
				wchar_t tmp = format[i];
				if(tmp == '%') argCount++;
			}

			va_list args;
			va_start(args, format);
			char * bk = args;
			while(argCount > 0)
			{
				argCount--;
				arg = va_arg(args, wchar_t*);
				if(!arg) continue;
				nLen += wcslen(arg);
			}
			TCHAR* psz = new TCHAR[nLen];
			int len = vswprintf_s(psz,nLen,format,bk);
			va_end(args);
	/*
			TCHAR psz[327680];

			va_list args;
			va_start(args, format);
			int len = vswprintf_s(psz,327680,format,args);
			va_end(args);
	*/
			int nLenText = ::GetWindowTextLength(m_pDataOutPut->m_hWnd);
			SendMessage(m_pDataOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pDataOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pDataOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}

		int addwarn(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;

			const TCHAR * psz = pErrText;

			tchar buf[255];
			if(pFile)
				_stprintf_s(buf,255,L"%s(%d,%d)  提示: 0x%x %s\r\n",pFile,nrow,ncol,nError ,pErrText);
			else
				_stprintf_s(buf,255,L"第%d行 第%d列  提示: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}

		int output(const tchar* pFile,const tchar* pMsgText)
		{
			if(!m_pOutPut) return 0;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)pMsgText);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}
	};

	class wlerror : public wpp::how::wErr
	{
	public:
		xfControl* m_pOutPut;
		xfControl* m_pDataOutPut;
		xfControl* m_pPCodeOutPut;
		int adderror(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;

			const TCHAR * psz = pErrText;

			tchar buf[255];
			if(pFile)
				_stprintf_s(buf,255,L"%s(%d,%d)  错误: 0x%x %s\r\n",pFile,nrow,ncol,nError ,pErrText);
			else
				_stprintf_s(buf,255,L"第%d行 第%d列  错误: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}

		int print(wchar_t* format,...)
		{
			if(!m_pDataOutPut) return 0;

			wchar_t* arg;
			int nLen = wcslen(format);

			int argCount = 0;
			for(int i=0; i<nLen; i++)
			{
				wchar_t tmp = format[i];
				if(tmp == '%') argCount++;
			}

			va_list args;
			va_start(args, format);
			char * bk = args;
			while(argCount > 0)
			{
				argCount--;
				arg = va_arg(args, wchar_t*);
				if(!arg) continue;
				nLen += wcslen(arg);
			}
			TCHAR* psz = new TCHAR[nLen];
			int len = vswprintf_s(psz,nLen,format,bk);
			va_end(args);
	/*
			TCHAR psz[327680];

			va_list args;
			va_start(args, format);
			int len = vswprintf_s(psz,327680,format,args);
			va_end(args);
	*/
			int nLenText = ::GetWindowTextLength(m_pDataOutPut->m_hWnd);
			SendMessage(m_pDataOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pDataOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pDataOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}

		int addwarn(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;

			const TCHAR * psz = pErrText;

			tchar buf[255];
			if(pFile)
				_stprintf_s(buf,255,L"%s(%d,%d)  提示: 0x%x %s\r\n",pFile,nrow,ncol,nError ,pErrText);
			else
				_stprintf_s(buf,255,L"第%d行 第%d列  提示: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}

		int output(const tchar* pFile,const tchar* pMsgText)
		{
			if(!m_pOutPut) return 0;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)pMsgText);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}
	};

	class myerror2 : public xvmerror
	{
	public:
		xfControl* m_pOutPut;
		int adderror(lptstr pFile,int nrow,int ncol,int nError,lptstr pErrText)
		{
			if(!m_pOutPut) return 0;

			TCHAR * psz = pErrText;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return xvmerror::adderror(pFile,nrow,ncol,nError,pErrText);
		}
		int addwarn(lptstr pFile,int nrow,int ncol,int nError,lptstr pErrText)
		{
			if(!m_pOutPut) return 0;

			TCHAR * psz = pErrText;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			delete psz;
			return xvmerror::addwarn(pFile,nrow,ncol,nError,pErrText);
		}
	};
	class xsmerror : public errobj
	{
	public:
		xfControl* m_pOutPut;
		xfControl* m_pEdit;
		int adderror(lptstr pFile,int nrow,int ncol,int nError,lptstr pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;
			TCHAR * psz = pErrText;

			tchar buf[255];
			if(pFile)
				_stprintf_s(buf,255,L"%s(%d) : error C0x%x: %s\r\n",pFile,nrow,nError,pErrText);	
			else
				_stprintf_s(buf,255,L"第%d行 第%d列  错误: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			int i= 1;
			//~SendMessage(m_pEdit->m_hWnd,SCI_INDICSETSTYLE,9+i, i);
			//~SendMessage(m_pEdit->m_hWnd,SCI_INDICSETFORE,9+i, 0x0000ff);
			//~SendMessage(m_pEdit->m_hWnd,SCI_SETINDICATORCURRENT,9+i, 0);

			//~int lineStart = SendMessage(m_pEdit->m_hWnd,SCI_POSITIONFROMLINE, nrow - 1,0);
			//SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,lineStart + ncol - 3,5);
			//~SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,nstart,nlen + 1);
			/*
			xCodeSheet::TError te;
			te.pt.x = nstart;
			te.pt.y = nlen;
			te.errText = pErrText;
			m_psheet->m_errors.push_back(te);*/
			return errobj::adderror(pFile,nrow,ncol,nError,pErrText,nstart,nlen);
		}
		int addwarn(lptstr pFile,int nrow,int ncol,int nError,lptstr pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;

			TCHAR * psz = pErrText;

			tchar buf[255];
			if(pFile)
				_stprintf_s(buf,255,L"%s(%d) : error C0x%x: %s\r\n",pFile,nrow,nError,pErrText);	
			else
				_stprintf_s(buf,255,L"第%d行 第%d列  错误: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			int i= 1;
			//~SendMessage(m_pEdit->m_hWnd,SCI_INDICSETSTYLE,9+i, i);
			//~SendMessage(m_pEdit->m_hWnd,SCI_INDICSETFORE,9+i, 0xff00ff);
			//~SendMessage(m_pEdit->m_hWnd,SCI_SETINDICATORCURRENT,9+i, 0);

			//~int lineStart = SendMessage(m_pEdit->m_hWnd,SCI_POSITIONFROMLINE, nrow - 1,0);
			//SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,lineStart + ncol - 3,5);
			//~SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,nstart,nlen + 1);

			/*xCodeSheet::TError te;
			te.pt.x = nstart;
			te.pt.y = nlen;
			te.errText = pErrText;
			m_psheet->m_errors.push_back(te);
			*/
			return errobj::addwarn(pFile,nrow,ncol,nError,pErrText,nstart,nlen);
		}
		int addtip(lptstr pstr)
		{
			if(!m_pOutPut) return 0;

			TCHAR * psz = pstr;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return errobj::addtip(pstr);
		}
		
		int addfct(lptstr pstr,int srow,int erow)
		{
			/*if(!m_pOutPut) return 0;

			TCHAR * psz = pstr;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);*/

			return errobj::addfct(pstr,srow,erow);
		}
	};

	class vlerror3 : public vl::vlErr
	{
	public:
		xfControl* m_pOutPut;
		int adderror(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;

			const TCHAR * psz = pErrText;

			tchar buf[255];
			//if(pFile)
			//	_stprintf_s(buf,255,L"%s(%d) : error C0x%x: %s\r\n",pFile,nrow,nError,pErrText);	
			//else
			_stprintf_s(buf,255,L"第%d行 第%d列  错误: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}

		int addwarn(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)
		{
			if(!m_pOutPut) return 0;

			const TCHAR * psz = pErrText;

			tchar buf[255];
			if(pFile)
				_stprintf_s(buf,255,L"%s(%d) : error C0x%x: %s\r\n",pFile,nrow,nError,pErrText);	
			else
				_stprintf_s(buf,255,L"第%d行 第%d列  错误: 0x%x %s\r\n",nrow,ncol,nError ,pErrText);
			psz = buf;

			int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
			SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
			SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			return 1;
		}
	};

	int ExecCallEx5(xlang::wpp_context* ptr_context,int f,int pthis,xfControl* pOutPut)
	{
		__try
		{
			ptr_context->get_runtime()->call_method(ptr_context,f,pthis);
		}__except(Filter1(GetExceptionCode(), GetExceptionInformation()))
		{
			wprintf(L"one except has happen.\r\n");
		} 
		return 1;
	}

	int ExecCallEx_(wpp::wpp_context* ptr_context,int f,int pthis,xfControl* pOutPut)
	{
		__try
		{
			ptr_context->get_runtime()->call_method(ptr_context,CWord(f,0),pthis);
		}__except(Filter1(GetExceptionCode(), GetExceptionInformation()))
		{
			wprintf(L"one except has happen.\r\n");
		} 
		return 1;
	}

	int ExecCall_(vl::vmlang3* v,int f,int tr,xfControl* pOutPut)
	{
		__try
		{
			v->call(f,0,tr,FetchData1,pOutPut);
		}__except(Filter1(GetExceptionCode(), GetExceptionInformation()))
		{
			wprintf(L"one except has happen.\r\n");
		} 
		return 1;
	}

	bool xfWin::Open(xfXPage * pXPage,DWORD dwExStyle,DWORD dwStyle, RECT *pBoundRect ,HWND hParent ,HINSTANCE hInst,LPVOID pParam,xfWin* pCmdWin)
	{
		//if(!dwStyle && !pBoundRect)dwStyle = WS_OVERLAPPEDWINDOW;

		xfWin * pWin = this;
		if(!hInst)hInst = xfApp::GetApp()->GetInstance();

		if(!pXPage->m_pxfNode) return false;
		IXMLDOMElementPtr pNodeEx ;
		if(pXPage->m_pElement)
			pNodeEx = pXPage->m_pElement;
		else
			pXPage->m_pxbXml->m_pXmlDoc->get_documentElement(&pNodeEx);

		VARIANT	val;
		LPTSTR strWinType = NULL;
		LPTSTR pXScript = NULL;
		LPTSTR pXDll = NULL;
		LPTSTR pstrGuid = NULL;
		LPTSTR pstrWorkGuid = NULL;
		LPTSTR pWinID = NULL;
		LPTSTR pWinClass = NULL;

		LPTSTR pScriptUrl = NULL;
		LPTSTR pScriptClass = NULL;
		LPTSTR pScriptUrlEx = NULL;
		LPTSTR pClassUrl = NULL;
		LPTSTR pClassUrl_ = NULL;
		LPTSTR pClassUrl5 = NULL;
		
		if(dwStyle & WS_CHILD)
		{
			strWinType = _tcsdup(L"child");
		}else
		{
			pNodeEx->getAttribute(L"type",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				strWinType = XUtil::tcsdupFromBstr(val.bstrVal); 
			}
			::VariantClear(&val);
		}
		
		pNodeEx->getAttribute(L"xscript",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pXScript = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"xdll",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pXDll = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"name",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pWinID = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"windowclass",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pWinClass = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"guid",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pstrGuid = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"script_class",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pScriptClass = XUtil::tcsdupFromBstr(val.bstrVal);
		}
		::VariantClear(&val);

		pNodeEx->getAttribute(L"tpp_url",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pScriptUrlEx = XUtil::tcsdupFromBstr(val.bstrVal);
			::VariantClear(&val);
		}else
		{
			pNodeEx->getAttribute(L"tpp_guid",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				pScriptUrlEx = new TCHAR[255];
				_stprintf_s(pScriptUrlEx,255,_T("dev:code%s"),val.bstrVal);
				::VariantClear(&val);
			}
		}

		pNodeEx->getAttribute(L"script_url",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			pScriptUrl = XUtil::tcsdupFromBstr(val.bstrVal);
			::VariantClear(&val);
		}else
		{
			pNodeEx->getAttribute(L"script_guid",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				pScriptUrl = new TCHAR[255];
				_stprintf_s(pScriptUrl,255,_T("dev:code%s"),val.bstrVal);
				::VariantClear(&val);
			}else
			{
				pNodeEx->getAttribute(L"classurl",&val);
				if(val.vt==VT_BSTR && val.bstrVal)
				{
					pClassUrl = new TCHAR[255];
					_stprintf_s(pClassUrl,255,_T("%s"),val.bstrVal);
					::VariantClear(&val);
				}
				else
				{
					pNodeEx->getAttribute(L"class_url",&val);
					if(val.vt==VT_BSTR && val.bstrVal)
					{
						pClassUrl_ = new TCHAR[255];
						_stprintf_s(pClassUrl_,255,_T("%s"),val.bstrVal);
						::VariantClear(&val);
					}else
					{
						pNodeEx->getAttribute(L"class_url5",&val);
						if(val.vt==VT_BSTR && val.bstrVal)
						{
							pClassUrl5 = new TCHAR[255];
							_stprintf_s(pClassUrl5,255,_T("%s"),val.bstrVal);
							::VariantClear(&val);
						}
					}
				}
			}
		}
		

		HMODULE hDLL = this->m_hDLL;
		if(!hDLL && pXDll)hDLL = ::LoadLibrary(pXDll);

		pWin->m_pXPage =  pXPage;
		pXPage->m_pWin = pWin;
		pWin->m_pCmdWin = pCmdWin;

		RECT rc;
		::SetRect(&rc,0,0,0,0);
		int width=400,height=600;
		
		pNodeEx->getAttribute(L"rect",&val);
		if(val.vt==VT_BSTR && val.bstrVal)
		{
			vector<LPTSTR> vals;
			LPTSTR pstrVal;
			pstrVal = XUtil::tcsdupFromBstr(val.bstrVal);
			if(XUtil::GetTokens(pstrVal,_T(","),vals)==4)
			{
				SetRect(&rc,XUtil::CalcCssStrToNumber(vals[0]),XUtil::CalcCssStrToNumber(vals[1]),
					XUtil::CalcCssStrToNumber(vals[2]),XUtil::CalcCssStrToNumber(vals[3]));
			}
			if(pstrVal) delete pstrVal;
			::VariantClear(&val);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
		}else
		{
			pNodeEx->getAttribute(L"width",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				LPTSTR pstrVal = XUtil::tcsdupFromBstr(val.bstrVal);
				if(pstrVal)delete pstrVal;
				width = XUtil::CalcCssStrToNumber(pstrVal);
				::VariantClear(&val);
			}
			pNodeEx->getAttribute(L"height",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				LPTSTR pstrVal = XUtil::tcsdupFromBstr(val.bstrVal);
				if(pstrVal)delete pstrVal;
				height = XUtil::CalcCssStrToNumber(pstrVal);
				::VariantClear(&val);
			}
			::SetRect(&rc,0,0,width,height);
		}

		if(pBoundRect && !(pBoundRect->right==0 && pBoundRect->bottom==0))
		{
			int scWidth,scHeight;
			scWidth = GetSystemMetrics(SM_CXSCREEN);
			scHeight = GetSystemMetrics(SM_CYSCREEN) - 36;

			RECT	rt;
			RECT& bound = *pBoundRect;

			//左右
			if(bound.left + width <= scWidth)
			{
				rt.left = bound.left;
				rt.right = rt.left + width;
			}
			else if(bound.right - width >= 0)
			{
				rt.left = bound.right - width;
				rt.right = bound.right;
			}
			else
			{
				rt.left = scWidth - width;
				rt.right = scWidth;
			}
			//上下
			if(bound.bottom + height <= scHeight)
			{
				rt.top = bound.bottom;
				rt.bottom = rt.top + height;
			}
			else if(bound.top - height >= 0)
			{
				rt.top = bound.top - height;
				rt.bottom = bound.top;
			}
			else
			{
				rt.top = scHeight - height;
				rt.bottom = scHeight;
				if(rt.left == bound.left && bound.right + width <= scWidth)
				{
					rt.left = bound.right;
					rt.right = rt.left + width;
				}
				else if(rt.right == bound.right && bound.left - width >= 0)
				{
					rt.left = bound.left - width;
					rt.right = bound.left;
				}
			}
			/*
			if(bound.bottom +  height< scHeight && bound.left + width < scWidth)
			{
				rt.left =  bound.left;
				rt.top  =  bound.bottom;
				rt.right = rt.left + width;
				rt.bottom = rt.top + height;
			}else if(bound.top +  height< scHeight && bound.right + width < scWidth)
			{
				rt.left =  bound.right;
				rt.top  =  bound.top;
				rt.right = rt.left + width;
				rt.bottom = rt.top + height;
			}else if(bound.top +  height< scHeight && bound.left - width > 0)
			{
				rt.right =  bound.left;
				rt.top  =  bound.top;
				rt.left = rt.right - width;
				rt.bottom = rt.top + height;
			}else if(bound.bottom +  height< scHeight)
			{
				rt.right =  scWidth;
				rt.top  =  bound.bottom;
				rt.left = rt.right - width;
				rt.bottom = rt.top + height;
			}else if(bound.bottom -  height>0 && bound.right + width < scWidth)
			{
				rt.left =  bound.right;
				rt.bottom =  bound.bottom;
				rt.right = rt.left + width;
				rt.top = rt.bottom - height;
			}else if(bound.bottom -  height>0 && bound.left - width > 0)
			{
				rt.right =  bound.left;
				rt.bottom =  bound.bottom;
				rt.left = rt.right - width;
				rt.top = rt.bottom - height;
			}else if(bound.bottom -  height>0)
			{
				rt.right =  scWidth;
				rt.top  =  bound.bottom;
				rt.left = rt.right - width;
				rt.bottom = rt.top + height;
			}else
			{
				if(bound.top + height > scHeight)
					rt.top = scHeight - height;
				else
					rt.top = bound.top;
				rt.left  = bound.left;
				rt.right = rt.left + width;
				rt.bottom = rt.top + height;
				//rt = rc;
			}*/
			rc = rt;
		}

		xfWin * pParentWin = (xfWin *)::GetProp(hParent,_T("this"));
		if(strWinType) 
		{
			if(::_tcscmp(_T("mdiframe"),strWinType)==0)
			{
				pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
			}else if(::_tcscmp(_T("sheet"),strWinType)==0)
			{
				if(pParentWin)
					pParentWin = pParentWin->GetFrameWindow();
				xfMdiFrame * pFrame = dynamic_cast<xfMdiFrame *>(pParentWin);
				if(!pFrame)
				{
					xfWin * xWin = (xfWin *)::GetProp(hParent,_T("this__"));
					if(xWin)
					{
						xfWin * xpWin = xWin->GetFrameWindow();
						pFrame = dynamic_cast<xfMdiFrame *>(xpWin);
						pParentWin = xpWin;
					}
				}
				if(pFrame)
				{
					pWin->CreateEx(WS_EX_MDICHILD|dwExStyle,WS_CHILD|dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,pFrame->m_hMdiClient,0,hInst);
					pFrame->AddSheet(pWin);
				}
				else
					pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;
			}else if(::_tcsstr(strWinType,_T("sheet.paper"))==strWinType)
			{
				if(pParentWin)
					pParentWin = pParentWin->GetFrameWindow();
				xfMdiFrame * pFrame = dynamic_cast<xfMdiFrame *>(pParentWin);
				if(pFrame)
				{
					pWin->CreateEx(WS_EX_MDICHILD|dwExStyle,WS_CHILD|dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,pFrame->m_hMdiClient,0,hInst);
					pFrame->AddSheet(pWin);
				}
				else
					pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;

				int nStyle = 0;
				if(strWinType[_tcslen(_T("sheet.paper"))]=='.')
					nStyle = _ttol(strWinType + _tcslen(_T("sheet.paper")) + 1);
				pWin->SetPaperState(nStyle);
			}else if(::_tcscmp(_T("main"),strWinType)==0)
			{
				pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;
			}else if(::_tcsstr(strWinType,_T("main.paper"))==strWinType)
			{
				pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;

				int nStyle = 0;
				if(strWinType[_tcslen(_T("main.paper"))]=='.')
					nStyle = _ttol(strWinType + _tcslen(_T("main.paper")) + 1);
				pWin->SetPaperState(nStyle);
			}else if(::_tcscmp(_T("popup"),strWinType)==0)
			{
				HWND hWndParent = NULL;
				if( pParentWin) hWndParent = pParentWin->GetHWND();
				pWin->CreateEx(dwExStyle,WS_POPUP|dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,hWndParent,0,hInst);
				xfWin * xWin = (xfWin *)::GetProp(hParent,_T("this__"));
				if(xWin) //pParentWin = xWin->GetFrameWindow();
					::SetProp(pWin->GetHWND(),_T("this__"),xWin);
				if(pWin)pWin->m_pParentWin = pParentWin;
			}else if(::_tcsstr(strWinType,_T("popup.paper"))==strWinType)
			{
				HWND hWndParent = NULL;
				if(pWin) hWndParent = pParentWin->GetHWND();
				pWin->CreateEx(dwExStyle,WS_POPUP|dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,hWndParent,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;
				
				int nStyle = 0;
				if(strWinType[_tcslen(_T("popup.paper"))]=='.')
					nStyle = _ttol(strWinType + _tcslen(_T("popup.paper")) + 1);
				pWin->SetPaperState(nStyle);
			}else if(::_tcscmp(_T("child"),strWinType)==0)
			{
				pWin->CreateEx(dwExStyle,WS_CHILD|dwStyle,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)hParent,0,hInst);
				if(pWin)
				{
					pWin->m_pParentWin = pParentWin;
					pWin->m_pFrameParam = pParentWin->m_pParam;
				}
			}else if(::_tcscmp(_T("response"),strWinType)==0)
			{
				pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;
				pWin->m_bAutoDeleted = false;

			}else if(::_tcsstr(strWinType,_T("response.paper"))==strWinType)
			{
				pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;
				pWin->m_bAutoDeleted = false;

				int nStyle = 0;
				if(strWinType[_tcslen(_T("response.paper"))]=='.')
					nStyle = _ttol(strWinType + _tcslen(_T("response.paper")) + 1);
				pWin->SetPaperState(nStyle);

			}else
			{
				pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
				if(pWin)pWin->m_pParentWin = pParentWin;
			}
		}else
		{
			pWin->CreateEx(dwExStyle,dwStyle|WS_CLIPCHILDREN,XFrameWndClass,pXPage->m_pxfNode->m_pCaption,rc,(HWND)NULL,0,hInst);
		}

		pWin->m_hDLL = hDLL;
		if(pParam)pWin->m_pParam = pParam;

		if(pXScript) delete  pXScript; 
		if(pXDll) delete pXDll;
		//if(pstrGuid) delete pstrGuid;
		//if(pstrWorkGuid) delete pstrWorkGuid;
		if(pWinID) delete pWinID;
		if(pWinClass) delete pWinClass;


		pWin->m_pXPage = pXPage;
		if(pXPage->m_pxfNode)
		{
			pXPage->m_pxfNode->m_pxfPage = pXPage;
			pXPage->m_pxfNode->CreateNodeControl(pWin->m_hWnd);
		}

		vl::vmlang3 * pm = 0;
		int cf = 0;
		int cls = 0;
		wpp::wpp_context* ptr_context = 0;
		xlang::wpp_context* ptr_context5 = 0;
		if(pClassUrl_)
		{
			xfControl* pOutPut = pWin->GetControl(_T("output"));
			if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
			if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;
			ptr_context =  (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
			if(!ptr_context)
			{
				ptr_context = new wpp::wpp_context(new wpp::wpp_runtime);
				ptr_context->set_wpp_ptr(new Wpp);

				wlerror* perr = new wlerror();
				
				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pOutPut;
				perr->m_pPCodeOutPut = 0;
				
				ptr_context->set_error(perr);

				ptr_context->get_runtime()->GetSource	 = xfUriLoader::LoadScript;
				ptr_context->get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
				ptr_context->get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
				ptr_context->get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

				xfApp::GetApp()->m_wpp = ptr_context;
			}else if(ptr_context->get_rrror())
			{
				wlerror* perr = (wlerror*)ptr_context->get_rrror();
				if(!perr->m_pOutPut)perr->m_pOutPut =  pOutPut;

			}
			cf =  ptr_context->get_runtime()->load_class(ptr_context,pClassUrl_);
			if(cf)
			{
				int f =   ptr_context->get_runtime()->find_method(ptr_context,cf,L"onload");
				cls = ptr_context->get_runtime()->create_class(ptr_context,cf);
				ptr_context->get_runtime()->set_nativePointer(ptr_context,cls,pWin->GetInterface());
				
				if(f) ExecCallEx_(ptr_context,f,cls,pOutPut);
				//ptr_context->get_runtime()->call_method(ptr_context,CWord(f,0),cls);

				 ::SetProp(this->GetHWND(),L"AttachObject",(HANDLE)cls);
			}
		}
		else if(pClassUrl5)
		{
			xfControl* pOutPut = pWin->GetControl(_T("output"));
			if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
			if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;
			ptr_context5 =  (xlang::wpp_context*)xfApp::GetApp()->m_wpp5;
			if(!ptr_context5)
			{
				ptr_context5 = new xlang::wpp_context(new xlang::wpp_runtime);
				ptr_context5->set_wpp_ptr(new xlang::Wpp5);

				wlerror5* perr = new wlerror5();
				
				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pOutPut;
				perr->m_pPCodeOutPut = 0;
				
				ptr_context5->set_error(perr);

				ptr_context5->get_runtime()->GetSource	 = xfUriLoader::LoadScript;
				ptr_context5->get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
				ptr_context5->get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
				ptr_context5->get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

				xfApp::GetApp()->m_wpp5 = ptr_context5;
			}else if(ptr_context5->get_rrror())
			{
				wlerror5* perr = (wlerror5*)ptr_context5->get_rrror();
				if(!perr->m_pOutPut)perr->m_pOutPut =  pOutPut;

			}
			cf =  ptr_context5->get_runtime()->load_class(ptr_context5,pClassUrl5);
			if(cf)
			{
				int f =   ptr_context5->get_runtime()->find_method(ptr_context5,cf,L"onload");
				cls = ptr_context5->get_runtime()->create_class(ptr_context5,cf);
				ptr_context5->get_runtime()->set_nativePointer(ptr_context5,cls,pWin->GetInterface());
				
				if(f) ExecCallEx5(ptr_context5,f,cls,pOutPut);
				//ptr_context5->get_runtime()->call_method(ptr_context,CWord(f,0),cls);

				 ::SetProp(this->GetHWND(),L"AttachObject",(HANDLE)cls);
			}
		}
		else if(pClassUrl)
		{
			/*vl::vmlang3 * */pm = (vl::vmlang3 *)xfApp::GetApp()->m_vmlang;
			if(!pm)
			{
				vl::vlang3 * pv =  new vl::vlang3;
				//pm = new vl::vmlang3();
				//pm->setVLang(pv);
				//pm->setArrayMgr(pv->getArrayMgr());

				vlerror3* perr = new vlerror3();
				xfControl* pOutPut = pWin->GetControl(_T("output"));
				if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
				if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;
				perr->m_pOutPut = pOutPut;
				pv->setError(perr);

				pv->GetSource = xfUriLoader::LoadScript;
				pv->FreeSource= xfUriLoader::FreeScript;
				pv->GetSourceDT = xfUriLoader::GetScriptDT;
				pv->GetSourceVER = xfUriLoader::GetScriptVER;
				
				xfApp::GetApp()->m_vmlang = pv->getLang(L"tpp");
				 pm = (vl::vmlang3 *)xfApp::GetApp()->m_vmlang;
			}
			/*int*/ cf = pm->loadClass(pClassUrl);
			if(cf >0)
			{
				xfControl* pOutPut = pWin->GetControl(_T("output"));
				if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
				if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;

				DWORD dwStart =  GetTickCount();
				/*int*/ cls = pm->createClass(cf);
				pm->setNativePointer(cls,pWin->GetInterface());
				int f = pm->findClassMethod(cf,L"onload");
				int tr = pm->getTr();
				vl::vmtuple::push(pm->getArrayMgr(),tr,vl::vmtype::classT|(cf<<8),cls);

				ExecCall_(pm,f,pm->getTr(),pOutPut);
				//pm->call(f,0,pm->getTr(),FetchData1,pOutPut);

				::SetProp(this->GetHWND(),L"AttachObject",(HANDLE)cls);

				/*
				DWORD dwRun =  GetTickCount() - dwStart;

				tchar buf[255];
				_stprintf_s(buf,255,L"\r\ncomp file: %d",pm->getVLang()->dwTimeCompFile);
				FetchData1(buf,pOutPut);
				_stprintf_s(buf,255,L"\r\nread file: %d",pm->getVLang()->dwTimeGetFile);
				FetchData1(buf,pOutPut);

				int dwCompiler = 0;
				for(int i=0;i<pm->getVLang()->dwTimeFiles.size();i++)
				{
					_stprintf_s(buf,255,L"\r\ncomplier %s use  %d",
						pm->getVLang()->dwTimeFiles[i].c_str(),
						pm->getVLang()->dwTimeComplier[i]);
					FetchData1(buf,pOutPut);
					dwCompiler += pm->getVLang()->dwTimeComplier[i];
				}
				_stprintf_s(buf,255,L"\r\ntotal complier  used  %d",dwCompiler);
				FetchData1(buf,pOutPut);
				_stprintf_s(buf,255,L"\r\nFound Token Hited used  %d",pm->getVLang()->dwTimeFounded);
				FetchData1(buf,pOutPut);
				_stprintf_s(buf,255,L"\r\nFound Token  used  %d",pm->getVLang()->dwTimeUnFound);
				FetchData1(buf,pOutPut);
				_stprintf_s(buf,255,L"\r\nrun  used  %d",dwRun);
				FetchData1(buf,pOutPut);

				pm->getVLang()->dwTimeCompFile = 0;
				pm->getVLang()->dwTimeGetFile = 0;
				pm->getVLang()->dwTimeFiles.clear();
				pm->getVLang()->dwTimeComplier.clear();
				pm->getVLang()->dwTimeUnFound = 0;
				pm->getVLang()->dwTimeFounded = 0;*/
			}
		}else if(pScriptUrl && pScriptClass)
		{
			LPTSTR pStr;
			BYTE * pData=NULL;
			int len;
			if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,pScriptUrl,NULL,pData,len)>=0 && pData)
			{
				if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
				{
					pStr = (LPTSTR)(pData + 2);
				}else
					pStr = (LPTSTR)pData;
				
				/*if(((xsscomplier*)xfApp::GetApp()->m_xsscp))
				{
					if(((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm) delete ((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm;
					delete (xsscomplier*)xfApp::GetApp()->m_xsscp;
					xfApp::GetApp()->m_xsscp = NULL;
				}*/

				/*xsscomplier* pxsscp;
				if(!(::GetKeyState(VK_CONTROL)<0))
				{
					pxsscp = xfApp::GetApp()->m_xsscp;
					if(!xfApp::GetApp()->m_xsscp)
				}*/

				if(!((xsscomplier*)xfApp::GetApp()->m_xsscp))
				{
					xfApp::GetApp()->m_xsscp = new xsscomplier();
					((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm = new xvmcore;
				}

				{
					myerror2* perr = new myerror2();
					xfControl* pOutPut = pWin->GetControl(_T("output"));
					if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
					if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;
					perr->m_pOutPut = pOutPut;
					
					((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_error = perr;
					((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_error = perr;
				}


				((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_error = ((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_error;
				

				int pc = ((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_pc;
				((xsscomplier*)xfApp::GetApp()->m_xsscp)->eval(pStr);
				((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_pc = pc;

				//void * p = ((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_codes;

				//((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_codes =  ((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_codesEx;
				//((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_error = ((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_error;

				
				code cd;
				memset(&cd,0,sizeof(code));
				/*cd.order = XC_PUSH;
				cd.h = xa_int;
				cd.n = (int)pWin->GetInterface();
				((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->eval(cd);*/

				int pClass = ((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->newXvmClass(pScriptClass,(int)pWin->GetInterface());

				cd.order = XC_CALL;
				tchar buf[255];
				_stprintf_s(buf,255,_T("%s::onload"),pScriptClass);
				cd.p[0] = _tcsdup(buf);
				cd.h = 0;
				((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->eval(cd,pClass);
				delete cd.p[0];

				/*cd.order = XC_POP;
				cd.h = 1;
				((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->eval(cd);*/

				((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_pc = pc;
				//((xsscomplier*)xfApp::GetApp()->m_xsscp)->m_pxvm->m_codes = (xcode::xarrays<xcode::code *,xcode::code> *)p;
			}
			if(pData) xfUriLoader::FreeData(pData);
		}
		else if(pScriptUrlEx && pScriptClass)
		{
			LPTSTR pStr = NULL;
			//BYTE * pData=NULL;
			//int	   len;
			bool   ok = false;

			if(!(parser_cpp*)xfApp::GetApp()->m_xsmparser)
			{
				//	delete (parser_cpp*)xfApp::GetApp()->m_xsmparser;
				xfApp::GetApp()->m_xsmparser = new parser_cpp();

				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->GetSource = xfUriLoader::LoadScript;
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->FreeSource= xfUriLoader::FreeScript;
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->GetSourceDT = xfUriLoader::GetScriptDT;
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->GetSourceVER = xfUriLoader::GetScriptVER;
			/*if(!((parser_cpp*)xfApp::GetApp()->m_xsmparser))
			{
				xfApp::GetApp()->m_xsmparser = new parser_cpp();
			}*/

				xsmerror* perr = new xsmerror();
				xfControl* pOutPut = pWin->GetControl(_T("output"));
				if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
				if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;
				perr->m_pOutPut = pOutPut;
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_perror = perr;
			}
			/*
			for(int i=0;m_pXPage->m_pScript && i<(int)m_pXPage->m_pScript->size();i++)
			{
				pStr = NULL;
				if((*m_pXPage->m_pScript)[i]->m_pSrc && (*m_pXPage->m_pScript)[i]->m_pSrc[0])
				{
					if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,(*m_pXPage->m_pScript)[i]->m_pSrc,NULL,pData,len)>=0 && pData)
					{
						if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
						{
							pStr = (LPTSTR)(pData + 2);
						}else
							pStr = (LPTSTR)pData;
					}
					if(pStr)
					{
						lexer* cp = new lexer;
						((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp = cp;
						((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp->setsource(pStr,null);
						((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp->insymbol();

						if(((parser_cpp*)xfApp::GetApp()->m_xsmparser)->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
						((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp->source = NULL;
						delete 	((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp;
					}
					if(pData) xfUriLoader::FreeData(pData);
				}
			}*/
			/*
			if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,pScriptUrlEx,NULL,pData,len)>=0 && pData)
			{
				if((pData[0]==0xff &&  pData[1]==0xfe)||(pData[0]==0xfe &&  pData[1]==0xff))
				{
					pStr = (LPTSTR)(pData + 2);
				}else
					pStr = (LPTSTR)pData;
			}
			if(pStr)
			{
				lexer* cp = new lexer;
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp = cp;
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp->setsource(pStr,null);
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp->insymbol();

				if(((parser_cpp*)xfApp::GetApp()->m_xsmparser)->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp->source = NULL;
				delete 	((parser_cpp*)xfApp::GetApp()->m_xsmparser)->cp;
			}
			*/
			((parser_cpp*)xfApp::GetApp()->m_xsmparser)->ProcessFile(pScriptUrlEx);
			if(((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_pEnv->m_units.size()<1) return false;
			gener_tpp* gt = (gener_tpp*)((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_pEnv->m_pgc;
			if(!gt)
			{
				//((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_pEnv->FreeGC(gt);
				((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_pEnv->m_pgc = 
					((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_pEnv->AllocGC();
				gt = (gener_tpp*)((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_pEnv->m_pgc;
				
			}
			gt->m_pParser =(parser_cpp*)xfApp::GetApp()->m_xsmparser;
			gt->m_pEnv = ((parser_cpp*)xfApp::GetApp()->m_xsmparser)->m_pEnv;
			gt->Exec(pScriptClass,L"onload",this->GetInterface());
			//delete gt;

			//if(pData) xfUriLoader::FreeData(pData);
		}

		if(pScriptUrl)	delete pScriptUrl;
		if(pScriptUrlEx)delete pScriptUrlEx;
		if(pScriptClass)delete pScriptClass;
		if(pClassUrl)delete pClassUrl;
		if(pClassUrl_)delete pClassUrl_;
		if(pClassUrl5)delete pClassUrl5;

		pWin->Construct();
		pWin->OnPreOpen();
		pWin->OnOpen();
		pWin->OnPostOpen();
		if(pWin->m_pCmdWin)
			pWin->m_pCmdWin->OnOpen(pWin);

		pNodeEx->getAttribute(L"visible",&val);
		bool bShow = true;
		if(val.vt==VT_BSTR && val.bstrVal && _tcsicmp(val.bstrVal,L"hidden")==0) bShow = false;
		if(!pBoundRect || (pBoundRect->right==0 && pBoundRect->bottom==0))
		{
			if(!pBoundRect || !(pBoundRect->right==0 && pBoundRect->bottom==0))
			{

				pNodeEx->getAttribute(L"winstate",&val);
				if(val.vt==VT_BSTR && val.bstrVal)
				{
					if(::_wcsicmp(val.bstrVal,L"maximize")==0)
					{
						if(bShow)::ShowWindow(pWin->m_hWnd,SW_MAXIMIZE);
						::GetClientRect(pWin->m_hWnd,&rc);
						if(pXPage->m_pxfNode)
						{
							pXPage->m_pxfNode->AdjustRect(rc);
							pXPage->m_pxfNode->AdjustCtrlRect(); 
						}
					}
					else if(::_wcsicmp(val.bstrVal,L"center")==0)
					{
						MONITORINFO mi;
						mi.cbSize = sizeof(mi);
						GetMonitorInfo(MonitorFromWindow(pWin->m_hWnd, MONITOR_DEFAULTTOPRIMARY), &mi);
						RECT rcCenter = mi.rcWork;
						RECT rcArea = mi.rcWork;
						RECT rcDlg = rc;

						// find dialog's upper left based on rcCenter
						int xLeft = (rcCenter.left + rcCenter.right) / 2 - (rcDlg.right - rcDlg.left) / 2;
						int yTop = (rcCenter.top + rcCenter.bottom) / 2 - (rcDlg.bottom - rcDlg.top) / 2;

						// if the dialog is outside the screen, move it inside
						if (xLeft < rcArea.left)
							xLeft = rcArea.left;
						else if (xLeft + (rcDlg.right - rcDlg.left) > rcArea.right)
							xLeft = rcArea.right - (rcDlg.right - rcDlg.left);

						if (yTop < rcArea.top)
							yTop = rcArea.top;
						else if (yTop + (rcDlg.bottom - rcDlg.top) > rcArea.bottom)
							yTop = rcArea.bottom - (rcDlg.bottom - rcDlg.top);

						// map screen coordinates to child coordinates
						SetWindowPos(pWin->m_hWnd,NULL, xLeft, yTop, -1, -1,
							SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

						::GetClientRect(pWin->m_hWnd,&rc);
						if(pXPage->m_pxfNode)
						{
							pXPage->m_pxfNode->AdjustRect(rc);
							pXPage->m_pxfNode->AdjustCtrlRect(); 
						}
						if(bShow) ShowWindow(pWin->m_hWnd,SW_SHOW);
					}else
					{
						::GetClientRect(pWin->m_hWnd,&rc);
						pXPage->m_pxfNode->AdjustRect(rc);
						pXPage->m_pxfNode->AdjustCtrlRect(); 
						if(bShow)::ShowWindow(pWin->m_hWnd,SW_SHOW);
					}
					::VariantClear(&val);
				}
			}
		}else
		{
			::GetClientRect(pWin->m_hWnd,&rc);
			pXPage->m_pxfNode->AdjustRect(rc);
			pXPage->m_pxfNode->AdjustCtrlRect(); 
			if(bShow)::ShowWindow(pWin->m_hWnd,SW_SHOW);
		}
		::UpdateWindow(pWin->m_hWnd);

		if(strWinType && (::_tcscmp(_T("response"),strWinType)==0 ||::_tcsstr(strWinType,_T("response.paper"))==strWinType) && ::IsWindow(pWin->m_hWnd))
		{
			bool bEnableParent = false;
			pWin->m_dwStatus |= WST_RESPONSELOOP + WST_RESPONSEWIN;
			if (hParent != NULL && ::IsWindowEnabled(hParent))
			{
				HWND hFrame;
				hFrame = hParent;
				while(GetParent(hFrame))
				{
					hFrame = GetParent(hFrame);
				}
				//::EnableWindow(hParent, FALSE);
				::EnableWindow(hFrame, FALSE);

				bEnableParent = TRUE;
			}
			
			MSG msg;
			while(true)
			{
				//if(PeekMessage(&msg,NULL,NULL,NULL,PM_NOREMOVE))
				//{
					GetMessage(&msg,NULL,NULL,NULL);
					if(msg.message ==WM_KEYDOWN )
					{
						if(msg.wParam == VK_TAB)
						{
							HWND hWnd = msg.hwnd;
							xfWinbase*  pThis = (xfWinbase*)::GetProp(hWnd,_T("this"));
							while(!pThis || !dynamic_cast<xfWin* >(pThis))
							{
								hWnd  = ::GetParent(hWnd);
								if(!hWnd)break;
								pThis = (xfWinbase*)::GetProp(hWnd,_T("this"));
							}
							if(pThis)
							{
								if((GetKeyState(VK_SHIFT) & 0x8000 ? TRUE : FALSE))
								{
									if(::SendMessage(pThis->m_hWnd,WM_FOCUSPREV,0,0)==1) continue;  
								}else
								{
									if(::SendMessage(pThis->m_hWnd,WM_FOCUSNEXT,0,0)==1) continue;
								}
							}
						}
					}
					TranslateMessage(&msg); 
					DispatchMessage(&msg);
				//}
				if(!(pWin->m_dwStatus & WST_RESPONSELOOP))break;
			}

			if (pWin->m_hWnd != NULL)
			{
				SetWindowPos(pWin->m_hWnd,NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
			}
			if (bEnableParent)
			{
				HWND hFrame;
				hFrame = hParent;
				while(GetParent(hFrame))
				{
					hFrame = GetParent(hFrame);
				}
				::EnableWindow(hFrame, TRUE);
			}
			if (hParent != NULL && ::GetActiveWindow() == pWin->m_hWnd)
			{
				::SetActiveWindow(hParent);
			}

			// cleanup
			DestroyWindow(pWin->m_hWnd);
		}
		/*
		if(::_tcscmp(_T("sheet"),strWinType)==0)
		{
			if(pParentWin)
				pParentWin = pParentWin->GetFrameWindow();
			xfMdiFrame * pFrame = dynamic_cast<xfMdiFrame *>(pParentWin);
			if(pFrame)
			{
				::SetActiveWindow(pFrame->GetHWND());
				::SetActiveWindow(pFrame->m_hMdiClient);
				::SetActiveWindow(pWin->m_hWnd);
				::SetFocus(pWin->m_hWnd);
				::SetFocus(pFrame->m_hMdiClient);
				
				::PostMessage(pFrame->m_hMdiClient,WM_MDINEXT,(LPARAM)pWin->m_hWnd,0L);
				::PostMessage(pFrame->m_hMdiClient,WM_MDIACTIVATE,(LPARAM)pWin->m_hWnd,0L);
			}
		}
		*/
		if(strWinType)delete strWinType;

		if(pClassUrl_ && ptr_context && cls)
		{
			xfControl* pOutPut = pWin->GetControl(_T("output"));
			if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
			if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;

			int f =   ptr_context->get_runtime()->find_method(ptr_context,cf,L"onloaded");
			if(f) ExecCallEx_(ptr_context,f,cls,pOutPut);
		}
		else if(pClassUrl5 && ptr_context5 && cls)
		{
			xfControl* pOutPut = pWin->GetControl(_T("output"));
			if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
			if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;

			int f =   ptr_context5->get_runtime()->find_method(ptr_context5,cf,L"onloaded");
			if(f) ExecCallEx5(ptr_context5,f,cls,pOutPut);
		}
		else if(pm && cf)
		{
			xfControl* pOutPut = pWin->GetControl(_T("output"));
			if(!pOutPut) pWin->GetFrameWindow()->GetControl(_T("output"));
			if(!pOutPut && xfApp::GetApp()->m_pOutPut) pOutPut = xfApp::GetApp()->m_pOutPut;
				
			int f = pm->findClassMethod(cf,L"onloaded");
			if(f > 0)
			{
				int tr = pm->getTr();
				vl::vmtuple::push(pm->getArrayMgr(),tr,vl::vmtype::classT|(cf<<8),cls);
				ExecCall_(pm,f,pm->getTr(),pOutPut);
			}
		}

		return true;
	}

	bool xfWin::EvtSetCursor(TEvent * pEvent, LPARAM lParam)
	{
		POINT point = pEvent->pt;
		ScreenToClient(GetHWND(),&point);

		int xPos = point.x;
		int yPos = point.y;

		if(m_pPaper)
		{
			RECT rt;
			::GetClientRect(GetHWND(),&rt);
			::InflateRect(&rt,-1,-1);

			HRGN hrgn = NULL;
			if(m_pPaper->nStyle==0)
			{
				HRGN hrgn1;
				POINT pt[3];
				pt[0].x = rt.right - m_pPaper->pt[1].x;
				pt[0].y = rt.top;
				pt[1].x = rt.right - CALC_X(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
				pt[1].y = rt.top   + CALC_Y(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
				pt[2].x = rt.right;
				pt[2].y = rt.top + m_pPaper->pt[1].y;

				hrgn = ::CreatePolygonRgn(pt,3,ALTERNATE);

				pt[0].x = rt.left + m_pPaper->pt[3].x;
				pt[0].y = rt.bottom;
				pt[1].x = rt.left +  CALC_X(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
				pt[1].y = rt.bottom  -  CALC_Y(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
				pt[2].x = rt.left;
				pt[2].y = rt.bottom - m_pPaper->pt[3].y;
				hrgn1 = ::CreatePolygonRgn(pt,3,ALTERNATE);

				CombineRgn(hrgn,hrgn,hrgn1,RGN_OR);
				::DeleteObject(hrgn1);

				if(::PtInRegion(hrgn,xPos,yPos))
				{
					HCURSOR hCursor = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL));
					hCursor = ::SetCursor(hCursor);
					pEvent->dwRet = 1;
				}
				::DeleteObject(hrgn);
			}else if(m_pPaper->nStyle==1)
			{
				HDC hPaintDC = GetDC(GetHWND());
				BeginPath(hPaintDC);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				::InflateRect(&rt,-1,-1);
			
				POINT pt[3];
				int startY = 18;
				rt.top += startY;
				pt[0].x = rt.left + (rt.right - rt.left)/4;
				pt[0].y = rt.top - 60;

				pt[1].x = rt.left + (rt.right - rt.left)*3/4;
				pt[1].y = rt.top + 60;

				pt[2].x = rt.right;
				pt[2].y = rt.top;

				::MoveToEx(hPaintDC,rt.left,rt.top,NULL);
				::PolyBezierTo(hPaintDC,pt,3);
				
				::LineTo(hPaintDC,rt.right,rt.top + 18);
				::LineTo(hPaintDC,rt.left, rt.top + 18);
				::LineTo(hPaintDC,rt.left, rt.top);

				EndPath(hPaintDC);
				hrgn = PathToRegion(hPaintDC);
				if(::PtInRegion(hrgn,xPos,yPos))
				{
					HCURSOR hCursor = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL));
					hCursor = ::SetCursor(hCursor);
					pEvent->dwRet = 1;
				}
				::ReleaseDC(GetHWND(),hPaintDC);
				::DeleteObject(hrgn);
			}else if(m_pPaper->nStyle==2)
			{
				HDC hPaintDC = GetDC(GetHWND());
				BeginPath(hPaintDC);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				::InflateRect(&rt,-1,-1);
			
				POINT pt[5];
				pt[0].x = rt.left + m_pPaper->pt[1].x;
				pt[0].y = rt.top;
				pt[1].x = rt.right - m_pPaper->pt[1].x;
				pt[1].y = rt.top   ;
				pt[2].x = rt.right;
				pt[2].y = rt.top + m_pPaper->pt[0].y;
				pt[3].x = rt.left;
				pt[3].y = rt.top + m_pPaper->pt[1].y;
				pt[4].x = pt[0].x;
				pt[4].y = pt[0].y;

				hrgn = ::CreatePolygonRgn(pt,5,ALTERNATE);

				if(::PtInRegion(hrgn,xPos,yPos))
				{
					HCURSOR hCursor = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL));
					hCursor = ::SetCursor(hCursor);
					pEvent->dwRet = 1;
				}
				::ReleaseDC(GetHWND(),hPaintDC);
				::DeleteObject(hrgn);
			}else if(m_pPaper->nStyle==3)
			{
				HDC hPaintDC = GetDC(GetHWND());
				BeginPath(hPaintDC);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				::InflateRect(&rt,-1,-1);
			
				POINT pt[3];
				int startY = 5;
				rt.top += startY;
				pt[0].x = rt.left + (rt.right - rt.left)/4;
				pt[0].y = rt.top - 10;

				pt[1].x = rt.left + (rt.right - rt.left)*3/4;
				pt[1].y = rt.top + 10;

				pt[2].x = rt.right;
				pt[2].y = rt.top;

				::MoveToEx(hPaintDC,rt.left,rt.top,NULL);
				::PolyBezierTo(hPaintDC,pt,3);
				
				::LineTo(hPaintDC,rt.right,rt.top + 18);
				::LineTo(hPaintDC,rt.left, rt.top + 18);
				::LineTo(hPaintDC,rt.left, rt.top);

				EndPath(hPaintDC);
				hrgn = PathToRegion(hPaintDC);
				if(::PtInRegion(hrgn,xPos,yPos))
				{
					HCURSOR hCursor = LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL));
					hCursor = ::SetCursor(hCursor);
					pEvent->dwRet = 1;
				}

				::ReleaseDC(GetHWND(),hPaintDC);
				::DeleteObject(hrgn);
			}
		}

		return true; 
	}

	bool xfWin::EvtDestroy(TEvent *pEvent,LPARAM lparam)
	{
		::UnhookWindowsHookEx(hHook); 
		hHookMenu = NULL;
		return true;
	}
	bool xfWin::EvtKillFocus(TEvent *pEvent,LPARAM lparam)
	{
		if(hHookMenu)
		{
			if( GetForegroundWindow() != hHookMenu && ::GetParent(GetForegroundWindow()) != hHookMenu)
			{
				::DestroyWindow(hHookMenu);  
			}
		}
		return true;
	}

	bool xfWin::EvtActivate(TEvent *pEvent,LPARAM lparam)
	{
		/*
		if(LOWORD(pEvent->wParam) != WA_INACTIVE)
		{
			if(m_pXPage && m_pXPage->m_pFocusNodeControl)
			{
				if(m_pXPage->m_pFocusNodeControl->m_pxfControl )
					::SetFocus(m_pXPage->m_pFocusNodeControl->m_pxfControl->GetHWND());
			}else if(m_pXPage)
			{
				std::set<xfNode *,xfNode::nodetab_less> nodes;
				m_pXPage->m_pxfNode->GetNodesByTab(nodes);
				std::set<xfNode *,xfNode::nodetab_less>::const_iterator tr;
				tr = nodes.begin();
				while(tr != nodes.end())
				{
					xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*tr);
					if(pNodeControl && pNodeControl->m_pxfControl)
					{
						::SetFocus(pNodeControl->m_pxfControl->m_hWnd);
						break;
					}
					tr++;
				}
			}
		}*/
		return true;
	}
	bool xfWin::EvtMdiActivate(TEvent *pEvent,LPARAM lparam)
	{
		/*
		if(LOWORD(pEvent->wParam) != WA_INACTIVE)
		{
			if(m_pXPage && m_pXPage->m_pFocusNodeControl)
			{
				::SetFocus(m_pXPage->m_pFocusNodeControl->m_pxfControl->GetHWND());
			}else if(m_pXPage)
			{
				std::set<xfNode *,xfNode::nodetab_less> nodes;
				m_pXPage->m_pxfNode->GetNodesByTab(nodes);
				std::set<xfNode *,xfNode::nodetab_less>::const_iterator tr;
				tr = nodes.begin();
				while(tr != nodes.end())
				{
					xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*tr);
					if(pNodeControl && pNodeControl->m_pxfControl)
					{
						::SetFocus(pNodeControl->m_pxfControl->m_hWnd);
						break;
					}
					tr++;
				}
			}
		}*/
		return true;
	}

	bool xfWin::EvtEraseBkgn(TEvent* pEvent,LPARAM lParam)
	{

		pEvent->dwRet = 1;
		return true;
	}

	
	bool xfWin::EvtPaint(TEvent *pEvent,LPARAM lparam) 
	{
		RECT	rc;
		HDC	hPaintDC;
		HBITMAP	hBmp;

		HDC hDC = (HDC)pEvent->paint.hdc;
		GetClientRect(&rc);
		hPaintDC = ::CreateCompatibleDC(hDC);
		hBmp = ::CreateCompatibleBitmap(hDC,rc.right - rc.left + 1,rc.bottom - rc.top +1);
		hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

		HBRUSH	hBrush = ::CreateSolidBrush(RGB(255,255,255));
		::FillRect(hPaintDC,&rc,hBrush);
		::DeleteObject(hBrush);

		if(m_pXPage)m_pXPage->DoDraw(hPaintDC);

		if(m_pPaper)
		{
			RECT rt;
			POINT pt[9];

			::GetClientRect(GetHWND(),&rt);

			if(m_pPaper->nStyle == 0)
			{
				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				::InflateRect(&rt,-1,-1);

				pt[0].x = rt.left + m_pPaper->pt[0].x;
				pt[0].y = rt.top;

				pt[1].x = rt.right - m_pPaper->pt[1].x;
				pt[1].y = rt.top;
				pt[2].x = rt.right;
				pt[2].y = rt.top + m_pPaper->pt[1].y;

				pt[3].x = rt.right;
				pt[3].y = rt.bottom - m_pPaper->pt[2].x;
				pt[4].x = rt.right -  m_pPaper->pt[2].y;
				pt[4].y = rt.bottom;

				pt[5].x = rt.left + m_pPaper->pt[3].x;
				pt[5].y = rt.bottom;
				pt[6].x = rt.left ;
				pt[6].y = rt.bottom - m_pPaper->pt[3].y ;

				pt[7].x = rt.left ;
				pt[7].y = rt.top + m_pPaper->pt[0].y ;
				pt[8].x = pt[0].x;
				pt[8].y = pt[0].y;

				Polyline(hPaintDC,pt,9);

				pt[0].x = rt.right - m_pPaper->pt[1].x;
				pt[0].y = rt.top;
				pt[1].x = rt.right - CALC_X(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
				pt[1].y = rt.top   + CALC_Y(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
				pt[2].x = rt.right;
				pt[2].y = rt.top + m_pPaper->pt[1].y;
				::Polygon(hPaintDC,pt,3);

				pt[0].x = rt.left + m_pPaper->pt[3].x;
				pt[0].y = rt.bottom;
				pt[1].x = rt.left +  CALC_X(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
				pt[1].y = rt.bottom  -  CALC_Y(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
				pt[2].x = rt.left;
				pt[2].y = rt.bottom - m_pPaper->pt[3].y;
				::Polygon(hPaintDC,pt,3);				

				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}else if(m_pPaper->nStyle == 1)
			{
				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
				hPen = (HPEN)::SelectObject(hPaintDC,hPen);

				::InflateRect(&rt,-1,-1);
				
				POINT pt[3];
				int startY = 18;
				rt.top += startY;
				pt[0].x = rt.left + (rt.right - rt.left)/4;
				pt[0].y = rt.top - 60;

				pt[1].x = rt.left + (rt.right - rt.left)*3/4;
				pt[1].y = rt.top + 60;

				pt[2].x = rt.right;
				pt[2].y = rt.top;

				::MoveToEx(hPaintDC,rt.left,rt.top,NULL);
				::PolyBezierTo(hPaintDC,pt,3);
				::LineTo(hPaintDC,rt.right,rt.bottom);
				::LineTo(hPaintDC,rt.left,rt.bottom);
				::LineTo(hPaintDC,rt.left,rt.top);

				hPen = (HPEN)::SelectObject(hPaintDC,hPen);
				::DeleteObject(hPen);
			}
		}

		::BitBlt(hDC,0,0,rc.right - rc.left + 1,rc.bottom - rc.top +1,hPaintDC,0,0,SRCCOPY);
		hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
		::DeleteObject(hBmp);
		::DeleteDC(hPaintDC);

		return true;
	}

	bool xfWin::EvtSize(TEvent *pEvent,LPARAM lparam)
	{
		RECT	rc;
		UINT nFlags = pEvent->size.nFlags;
		POINT point = pEvent->size.point;
		if(nFlags !=SIZE_MINIMIZED)
		{
			if(this->GetHWND())
			{
				GetClientRect(&rc);
				if(m_pXPage->m_pxfNode)
				{
					m_pXPage->m_pxfNode->AdjustRect(rc);
					m_pXPage->m_pxfNode->AdjustCtrlRect();
				}
			}
			if(this->m_pPaper)
			{
				int nStyle = m_pPaper->nStyle;
				delete this->m_pPaper;
				SetPaperState(nStyle);
			}
		}
		return true;
	}

	bool xfWin::EvtXCommand(TEvent *pEvent,LPARAM lparam)
	{
		if(hHookMenu)
		{
			::DestroyWindow(hHookMenu);
			hHookMenu = NULL;
		}
		if(m_pCmdWin)
		{
			::SendMessage(m_pCmdWin->GetHWND(),WM_XCOMMAND,pEvent->wParam,pEvent->lParam);
			pEvent->dwRet = 1;
			return true;
		}
		if((m_pCmdWin?m_pCmdWin->OnXCommandProcess(pEvent->wParam, pEvent->lParam):OnXCommandProcess(pEvent->wParam, pEvent->lParam))<1)
		{
			pEvent->dwRet = DefCmdProcess(pEvent->message, pEvent->wParam, pEvent->lParam);
		}else
			pEvent->dwRet = 1;
		return true;
	}

	LRESULT CALLBACK MouseProc( int nCode, WPARAM wParam,LPARAM lParam)
	{
		if(nCode < 0) return CallNextHookEx(NULL,nCode,wParam,lParam);
		MOUSEHOOKSTRUCT *  pMsg = (MOUSEHOOKSTRUCT *)lParam; 
		if(pMsg)   
		{
			if(wParam==WM_LBUTTONDOWN || wParam== WM_NCLBUTTONDOWN)
			{
				if(xfWin::hHookMenu && (pMsg->hwnd != xfWin::hHookMenu) &&  ::GetParent(pMsg->hwnd) != xfWin::hHookMenu)  
				{
					::DestroyWindow(xfWin::hHookMenu);  
					return CallNextHookEx(NULL,nCode,wParam,lParam);
				}
			}
		}
		return CallNextHookEx(NULL,nCode,wParam,lParam);
	}

	xfControl * xfWin::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
	{
		return NULL;
	}

	int xfWin::OnXCommand(LPCTSTR	pStrID, xfControl * pControl)
	{
		return 0;
	}

	int xfWin::OnXCommandProcess(WPARAM wParam, LPARAM lParam)
	{
		LPTSTR	pStrID = (LPTSTR)wParam;
		xfControl * pControl = (xfControl *)lParam; 

		
		if(this->OnXCommand(pStrID,pControl)==1) return 1; 
		if(pControl && pControl->m_pxfNode && !GetParent(GetHWND()))
		{
			xfNodeControl* pFoucsNode = pControl->m_pxfNode->GetXPage()->GetFocusNodeControl();
			if(pFoucsNode && pFoucsNode->m_pxfControl  )
			{
				int nResult = 0;
				nResult = (int)SendMessage(pFoucsNode->m_pxfControl->m_hWnd,WM_XCOMMAND,wParam,lParam);
				if(nResult ==1) return 1;
				if(pFoucsNode->m_pxfControl->GetWin() != this)
					nResult = pFoucsNode->m_pxfControl->GetWin()->OnXCommand(pStrID,pControl);
				if(nResult ==1) return 1;
				return 0;
			}
		}
		return 0;//OnXCommand(pStrID,pControl);
	}

	int xfWin::DefCmdProcess(UINT message, WPARAM wParam, LPARAM lParam)
	{
		xfControl * pCtrlWin = (xfControl *)lParam;
		if(pCtrlWin && pCtrlWin->m_pxfNode &&!GetParent(GetHWND()))
		{

			IXMLDOMNode *pNode ;
			BSTR	bstr;
			pCtrlWin->m_pxfNode->m_pElement->selectSingleNode(L"box|xframe",&pNode);
			if(!pNode) return 0;

			IXMLDOMNodeList *pNodeList ;
			pCtrlWin->m_pxfNode->m_pElement->selectNodes(L"box|xframe",&pNodeList);
			long nLen;
			pNodeList->get_length(&nLen);
			if(nLen==1)
			{
				IXMLDOMElement * pElement;
				pElement = xbXml::GetElement(pNode);
				BSTR bstrTag;
				pElement->get_tagName(&bstrTag);
				if(::_wcsicmp(bstrTag,L"help")==0)
				{
					::SysFreeString(bstrTag);
					pElement->Release();
					pNodeList->Release();
					pNode->Release();
					return 0;
				}
				::SysFreeString(bstrTag);
				pElement->Release();
			}
			pNodeList->Release();


			xfWin* pPopupWin =  new xfWin();
			xfXPage * pPage = new xfXPage();
			pPopupWin->m_bAutoDeleted = false;
			pPage->m_pWin = pPopupWin;
			pPopupWin->m_pXPage = pPage;
			pNode->get_xml(&bstr);
			if(bstr)
			{
				pPage->LoadXPage(bstr);
				::SysFreeString(bstr);
			}

			RECT rect;
			::SetRect(&rect,0,0,0,0);
			
			int nWidth=0,nHeight=0;
			VARIANT val;
			IXMLDOMElement* pElement = 	xbXml::GetElement(pNode);
			pElement->getAttribute(L"rect",&val);
			if(val.vt==VT_BSTR && val.bstrVal)
			{
				vector<LPTSTR> vals;
				LPTSTR pstrVal;
				pstrVal = XUtil::tcsdupFromBstr(val.bstrVal);
				if(XUtil::GetTokens(pstrVal,_T(","),vals)==4)
				{
					SetRect(&rect,XUtil::CalcCssStrToNumber(vals[0]),XUtil::CalcCssStrToNumber(vals[1]),
						XUtil::CalcCssStrToNumber(vals[2]),XUtil::CalcCssStrToNumber(vals[3]));
				}
				if(pstrVal) delete pstrVal;
				nWidth = rect.right - rect.left;
				nHeight = rect.bottom - rect.top;
			}else
			{
				::VariantClear(&val);
				pElement->getAttribute(L"width",&val);
				if(val.vt==VT_BSTR && val.bstrVal)
					nWidth = XUtil::CalcCssStrToNumber(val.bstrVal);
				::VariantClear(&val);
				pElement->getAttribute(L"height",&val);
				if(val.vt==VT_BSTR && val.bstrVal)
					nHeight = XUtil::CalcCssStrToNumber(val.bstrVal);
			}
			::VariantClear(&val);
			pElement->Release();
			pNode->Release();
		
			RECT	rc;
			POINT	pt;
			pt.x = pt.y = 0;
			rc =  pCtrlWin->m_pxfNode->m_AreaRect;
			ClientToScreen(m_hWnd,&pt);
			rc.left = rc.left + pt.x;
			rc.top = rc.bottom + pt.y ;
			if(nWidth >0)
				rc.right = rc.left + nWidth;
			else
				rc.right = rc.left + 120;
			if(nHeight > 0)
				rc.bottom = rc.top + nHeight;
			else
				rc.bottom = rc.top + 240;

			int scWidth,scHeight;
			scWidth = GetSystemMetrics(SM_CXSCREEN);
			scHeight = GetSystemMetrics(SM_CYSCREEN);

			if(rc.right > scWidth)
				::OffsetRect(&rc,- rc.right + scWidth - 15,0);
			if(rc.bottom > scHeight)
			{
				rc.bottom = rc.top - pCtrlWin->m_pxfNode->m_AreaRect.bottom + pCtrlWin->m_pxfNode->m_AreaRect.top;
				rc.top -= 240;
			}
			pPopupWin->CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_NOACTIVATE,WS_BORDER|WS_POPUP,XFrameWndClass,NULL,rc,
				GetHWND()/*(HWND)pCtrlWin->m_pxfNode->GetXPage()->m_pWin->m_hWnd*/,0,xfApp::GetApp()->m_hInst);
			::GetClientRect(pPopupWin->m_hWnd,&rc);
			pPopupWin->m_hDLL = m_hDLL;

			pPage->m_pResourceMgr = this->m_pXPage->m_pResourceMgr;
			pPage->m_pFocusNodeControl = pCtrlWin->m_pxfNode->GetXPage()->m_pFocusNodeControl;
			if(pPage->m_pxfNode)
			{
				pPage->m_pxfNode->AdjustRect(rc);
				pPage->m_pxfNode->CreateNodeControl(pPopupWin->m_hWnd);
				pPage->m_pxfNode->AdjustCtrlRect();
			}
			pPopupWin->m_pCmdWin = this;
			::ShowWindow(pPopupWin->m_hWnd,SW_SHOWNOACTIVATE);
			::InvalidateRect(pPopupWin->m_hWnd,NULL,true);
			hHookMenu = pPopupWin->m_hWnd;
			hHook = SetWindowsHookEx(WH_MOUSE,MouseProc,xfApp::GetApp()->m_hInst,0);
			if(!hHook)hHook = SetWindowsHookEx(WH_MOUSE,MouseProc,0,0);
			if(!hHook)
			{
				TCHAR szBuf[80]; 
				LPVOID lpMsgBuf;
				DWORD dw = GetLastError(); 

				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM,
					NULL,
					dw,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					(LPTSTR) &lpMsgBuf,
					0, NULL );
				wsprintf(szBuf, 
					_T("%s failed with error %d: %s"), 
					_T("SetWindowsHookEx"), dw, lpMsgBuf); 

				//MessageBox(NULL, szBuf, _T("Error"), MB_OK); 
				LocalFree(lpMsgBuf);
			}
			
			return 1;
		}
		return 0;
	}
	
	xfControl * xfWin::GetControl(LPCTSTR pStrName,bool bInsUnlayered)
	{
		if(_tcsstr(pStrName,_T("frame:")) && GetFrameWindow() !=this)
			return GetFrameWindow()->GetControl(pStrName + _tcslen(_T("frame")) + 1,bInsUnlayered);
		if(m_pXPage->m_pxfNode && m_pXPage->m_pxfNode->GetNodeByName(pStrName,bInsUnlayered))
			return ((xfNodeControl *)m_pXPage->m_pxfNode->GetNodeByName(pStrName,bInsUnlayered))->m_pxfControl;
		return NULL;
	}

	xfControl * xfWin::GetControl(LPCTSTR pStrName,xfNode * pNode)
	{
		if(pNode && pNode->GetNodeByName(pStrName))
			return ((xfNodeControl *)pNode->GetNodeByName(pStrName))->m_pxfControl;
		else
			return NULL;
	}

	xfControl * xfWin::GetControl(LPCTSTR pStrName)
	{
		xfControl * pControl = GetControl(pStrName,false);
		if(pControl) return pControl;
		return GetControl(pStrName,true);
	}
	void * xfWin::GetIControl(LPCTSTR pStrName)
	{
		xfControl * pControl = GetControl(pStrName);
		if(pControl)
			return pControl->GetInterface();
		else
			return NULL;
	}
	xfWin * xfWin::GetFrameWindow()
	{
		if(this->m_pParentWin) return m_pParentWin->GetFrameWindow();

		HWND hParent = m_hWnd;
		while(::GetParent(hParent))
		{
			hParent = ::GetParent(hParent);
		}
		
		if(hParent == m_hWnd) return this;

		xfWin * pWin = (xfWin *)::GetProp(hParent,_T("this"));

		return pWin;
	}

	bool xfWin::EvtFocusNext(TEvent *pEvent,LPARAM lparam)
	{
		std::set<xfNode *,xfNode::nodetab_less> nodes;
		m_pXPage->m_pxfNode->GetNodesByTab(nodes);
		std::set<xfNode *,xfNode::nodetab_less>::const_iterator tr;
		tr = nodes.begin();
		HWND hWnd = ::GetFocus();
		while(tr != nodes.end())
		{
			xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*tr);
			tr++;
			if(pNodeControl && (pNodeControl->GetHWND() == hWnd || pNodeControl->GetHWND() == ::GetParent(hWnd)))break;
		}
		if(tr != nodes.end())
		{
			xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*tr);
			if(pNodeControl)
			{
				SetFocus(pNodeControl->GetHWND());
				pEvent->dwRet = 1;
			}
		}else if((tr = nodes.begin()) != nodes.end())
		{
			xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*tr);
			if(pNodeControl)
			{
				SetFocus(pNodeControl->GetHWND());
			}
		}
		
		return true;
	}

	bool xfWin::EvtUpdateCmdUI(TEvent *pEvent,LPARAM lparam)
	{
		return true;
	}

	bool xfWin::EvtFocusPrev(TEvent *pEvent,LPARAM lparam)
	{
		std::set<xfNode *,xfNode::nodetab_less> nodes;
		m_pXPage->m_pxfNode->GetNodesByTab(nodes);
		std::set<xfNode *,xfNode::nodetab_less>::const_iterator tr;
		tr = nodes.begin();
		HWND hWnd = ::GetFocus();

		int nIndex = 0;
		while(tr != nodes.end())
		{
			xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*tr);
			if(pNodeControl && (pNodeControl->GetHWND() == hWnd || pNodeControl->GetHWND() == ::GetParent(hWnd)))break;
			nIndex++;
			tr++;
		}
		if(nIndex >0)
		{
			tr--;
			xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*tr);
			if(pNodeControl)
			{
				SetFocus(pNodeControl->GetHWND());
			}
		}else
		{
			if(nodes.size() > 0)
			{
				xfNodeControl * pNodeControl = dynamic_cast<xfNodeControl *>(*nodes.rbegin());
				if(pNodeControl)
				{
					SetFocus(pNodeControl->GetHWND());
				}
			}
		}
		
		return true;
	}
	int xfWin::PreSwitchLayer(xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame)
	{
		return 0;
	}
	
	xbObject * xfWin::GetInterface()
	{
		if(m_pInterFace) return m_pInterFace;
		m_pInterFace = (xbObject *)new IXFWin();
		((IXFWin *)m_pInterFace)->m_pWin = this;
		return m_pInterFace;
	}
	int xfWin::SetPaperState(int nStyle)
	{
		m_pPaper = new Paper();
		::memset(m_pPaper,0,sizeof(Paper));
		m_pPaper->nStyle = nStyle;

		m_pPaper->crBack = RGB(0xce,0xde,0xce);
		m_pPaper->crLine = RGB(64,64,64);

		RECT rt;
		POINT ptc;

		POINT pt[9];
		::GetWindowRect(GetHWND(),&rt);
		ptc.x = rt.left;
		ptc.y = rt.top;
		::ScreenToClient(GetHWND(),&ptc);
		::GetClientRect(GetHWND(),&rt);

		::OffsetRect(&rt,-ptc.x,-ptc.y);

		HRGN hrgn = NULL;
		if(nStyle==0)
		{
			m_pPaper->pt[0].x = 10 , m_pPaper->pt[0].y = 10;
			m_pPaper->pt[1].x = 40 , m_pPaper->pt[1].y = 160;
			m_pPaper->pt[2].x = 10 , m_pPaper->pt[2].y = 10;
			m_pPaper->pt[3].x = 40 , m_pPaper->pt[3].y = 160;

			pt[0].x = rt.left + m_pPaper->pt[0].x;
			pt[0].y = rt.top;

			pt[1].x = rt.right - m_pPaper->pt[1].x;
			pt[1].y = rt.top;
			pt[2].x = rt.right;
			pt[2].y = rt.top + m_pPaper->pt[1].y;

			pt[3].x = rt.right;
			pt[3].y = rt.bottom - m_pPaper->pt[2].x;
			pt[4].x = rt.right -  m_pPaper->pt[2].y;
			pt[4].y = rt.bottom;

			pt[5].x = rt.left + m_pPaper->pt[3].x;
			pt[5].y = rt.bottom;
			pt[6].x = rt.left ;
			pt[6].y = rt.bottom - m_pPaper->pt[3].y ;

			pt[7].x = rt.left ;
			pt[7].y = rt.top + m_pPaper->pt[0].y ;
			pt[8].x = pt[0].x;
			pt[8].y = pt[0].y;

			HRGN rgn = ::CreatePolygonRgn(pt,9,ALTERNATE);
			::SetWindowRgn(GetHWND(),rgn,false);
			
			HRGN hrgn1;
			pt[0].x = rt.right - m_pPaper->pt[1].x;
			pt[0].y = rt.top;
			pt[1].x = rt.right - CALC_X(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
			pt[1].y = rt.top   + CALC_Y(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
			pt[2].x = rt.right;
			pt[2].y = rt.top + m_pPaper->pt[1].y;
			hrgn = ::CreatePolygonRgn(pt,3,ALTERNATE);

			pt[0].x = rt.left + m_pPaper->pt[3].x;
			pt[0].y = rt.bottom;
			pt[1].x = rt.left +  CALC_X(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
			pt[1].y = rt.bottom  -  CALC_Y(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
			pt[2].x = rt.left;
			pt[2].y = rt.bottom - m_pPaper->pt[3].y;
			hrgn1 = ::CreatePolygonRgn(pt,3,ALTERNATE);

			CombineRgn(hrgn,hrgn,hrgn1,RGN_OR);
			::DeleteObject(hrgn1);
		}else if(nStyle==1)
		{
			HDC hPaintDC = GetDC(GetHWND());
			BeginPath(hPaintDC);

			HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
			hPen = (HPEN)::SelectObject(hPaintDC,hPen);

			::InflateRect(&rt,1,1);
			
			POINT pt[3];
			int startY = 18;
			rt.top += startY;
			pt[0].x = rt.left + (rt.right - rt.left)/4;
			pt[0].y = rt.top - 60;

			pt[1].x = rt.left + (rt.right - rt.left)*3/4;
			pt[1].y = rt.top + 60;

			pt[2].x = rt.right;
			pt[2].y = rt.top;

			::MoveToEx(hPaintDC,rt.left,rt.top,NULL);
			::PolyBezierTo(hPaintDC,pt,3);
			
			::LineTo(hPaintDC,rt.right,rt.bottom);
			::LineTo(hPaintDC,rt.left,rt.bottom);
			::LineTo(hPaintDC,rt.left,rt.top);

			EndPath(hPaintDC);

			//WidenPath(hPaintDC);
			hrgn = PathToRegion(hPaintDC);
			::SetWindowRgn(GetHWND(),hrgn,false);

			::ReleaseDC(GetHWND(),hPaintDC);
			::DeleteObject(hrgn);
		}else if(nStyle==2)
		{
			m_pPaper->pt[0].x = 10 , m_pPaper->pt[0].y = 10;
			m_pPaper->pt[1].x = 10 , m_pPaper->pt[1].y = 10;
			m_pPaper->pt[2].x = 0 , m_pPaper->pt[2].y = 0;
			m_pPaper->pt[3].x = 0 , m_pPaper->pt[3].y = 0;

			pt[0].x = rt.left + m_pPaper->pt[0].x;
			pt[0].y = rt.top;

			pt[1].x = rt.right - m_pPaper->pt[1].x;
			pt[1].y = rt.top;
			pt[2].x = rt.right;
			pt[2].y = rt.top + m_pPaper->pt[1].y;

			pt[3].x = rt.right;
			pt[3].y = rt.bottom - m_pPaper->pt[2].y;
			pt[4].x = rt.right  - m_pPaper->pt[2].x;
			pt[4].y = rt.bottom ;

			pt[5].x = rt.left + m_pPaper->pt[3].x;
			pt[5].y = rt.bottom;
			pt[6].x = rt.left ;
			pt[6].y = rt.bottom - m_pPaper->pt[3].y;

			pt[7].x = rt.left;
			pt[7].y = rt.top + m_pPaper->pt[1].y;
			pt[8].x = pt[0].x;
			pt[8].y = pt[0].y;

			HRGN rgn = ::CreatePolygonRgn(pt,9,ALTERNATE);
			::SetWindowRgn(GetHWND(),rgn,false);
		}else if(nStyle==3)
		{
			HDC hPaintDC = GetDC(GetHWND());
			BeginPath(hPaintDC);

			HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
			hPen = (HPEN)::SelectObject(hPaintDC,hPen);

			::InflateRect(&rt,1,1);
			
			POINT pt[3];
			int startY = 5;
			rt.top += startY;
			pt[0].x = rt.left + (rt.right - rt.left)/4;
			pt[0].y = rt.top - 10;

			pt[1].x = rt.left + (rt.right - rt.left)*3/4;
			pt[1].y = rt.top + 10;

			pt[2].x = rt.right;
			pt[2].y = rt.top;

			::MoveToEx(hPaintDC,rt.left,rt.top,NULL);
			::PolyBezierTo(hPaintDC,pt,3);
			
			::LineTo(hPaintDC,rt.right,rt.bottom);
			::LineTo(hPaintDC,rt.left,rt.bottom);
			::LineTo(hPaintDC,rt.left,rt.top);

			EndPath(hPaintDC);

			//WidenPath(hPaintDC);
			hrgn = PathToRegion(hPaintDC);
			::SetWindowRgn(GetHWND(),hrgn,false);

			::ReleaseDC(GetHWND(),hPaintDC);
			::DeleteObject(hrgn);
		}
		m_pPaper->rgn = hrgn;
	
		return 1;
	}

	#ifndef GET_X_LPARAM
	#define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
	#endif
	#ifndef GET_Y_LPARAM
	#define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
	#endif

	bool xfWin::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
	{
			if(m_pPaper)
			{
				int xPos = GET_X_LPARAM(pEvent->lParam); 
				int yPos = GET_Y_LPARAM(pEvent->lParam); 

				RECT rt;
				::GetClientRect(GetHWND(),&rt);
				::InflateRect(&rt,-1,-1);

				HRGN hrgn = NULL;
				if(m_pPaper->nStyle==0)
				{
					HRGN hrgn1;
					POINT pt[3];
					pt[0].x = rt.right - m_pPaper->pt[1].x;
					pt[0].y = rt.top;
					pt[1].x = rt.right - CALC_X(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
					pt[1].y = rt.top   + CALC_Y(m_pPaper->pt[1].x,m_pPaper->pt[1].y);
					pt[2].x = rt.right;
					pt[2].y = rt.top + m_pPaper->pt[1].y;

					hrgn = ::CreatePolygonRgn(pt,3,ALTERNATE);

					pt[0].x = rt.left + m_pPaper->pt[3].x;
					pt[0].y = rt.bottom;
					pt[1].x = rt.left +  CALC_X(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
					pt[1].y = rt.bottom  -  CALC_Y(m_pPaper->pt[3].x,m_pPaper->pt[3].y);
					pt[2].x = rt.left;
					pt[2].y = rt.bottom - m_pPaper->pt[3].y;
					hrgn1 = ::CreatePolygonRgn(pt,3,ALTERNATE);

					CombineRgn(hrgn,hrgn,hrgn1,RGN_OR);
					::DeleteObject(hrgn1);

					if(::PtInRegion(hrgn,xPos,yPos))
					{
						m_pPaper->state = st_move;
						m_MousePt.x = xPos;
						m_MousePt.y = yPos;
						this->SetCapture();
					}
					::DeleteObject(hrgn);
				}else if(m_pPaper->nStyle==1)
				{
					HDC hPaintDC = GetDC(GetHWND());
					BeginPath(hPaintDC);

					HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
					hPen = (HPEN)::SelectObject(hPaintDC,hPen);

					::InflateRect(&rt,-1,-1);
				
					POINT pt[3];
					int startY = 18;
					rt.top += startY;
					pt[0].x = rt.left + (rt.right - rt.left)/4;
					pt[0].y = rt.top - 60;

					pt[1].x = rt.left + (rt.right - rt.left)*3/4;
					pt[1].y = rt.top + 60;

					pt[2].x = rt.right;
					pt[2].y = rt.top;

					::MoveToEx(hPaintDC,rt.left,rt.top,NULL);
					::PolyBezierTo(hPaintDC,pt,3);
					
					::LineTo(hPaintDC,rt.right,rt.top + 18);
					::LineTo(hPaintDC,rt.left, rt.top + 18);
					::LineTo(hPaintDC,rt.left, rt.top);

					EndPath(hPaintDC);
					hrgn = PathToRegion(hPaintDC);
					if(::PtInRegion(hrgn,xPos,yPos))
					{
						m_pPaper->state = st_move;
						m_MousePt.x = xPos;
						m_MousePt.y = yPos;
						this->SetCapture();
					}

					::ReleaseDC(GetHWND(),hPaintDC);
					::DeleteObject(hrgn);
				}else if(m_pPaper->nStyle==2)
				{
					//HRGN hrgn1;
					POINT pt[3];
					pt[0].x = rt.left + m_pPaper->pt[1].x;
					pt[0].y = rt.top;
					pt[1].x = rt.right - m_pPaper->pt[1].x;
					pt[1].y = rt.top   ;
					pt[2].x = rt.right;
					pt[2].y = rt.top + m_pPaper->pt[0].y;
					pt[3].x = rt.left;
					pt[3].y = rt.top + m_pPaper->pt[1].y;
					pt[4].x = pt[0].x;
					pt[4].y = pt[0].y;

					hrgn = ::CreatePolygonRgn(pt,5,ALTERNATE);

					if(::PtInRegion(hrgn,xPos,yPos))
					{
						m_pPaper->state = st_move;
						m_MousePt.x = xPos;
						m_MousePt.y = yPos;
						this->SetCapture();
					}
					::DeleteObject(hrgn);
				}else if(m_pPaper->nStyle==3)
				{
					HDC hPaintDC = GetDC(GetHWND());
					BeginPath(hPaintDC);

					HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(64,64,0));
					hPen = (HPEN)::SelectObject(hPaintDC,hPen);

					::InflateRect(&rt,-1,-1);
				
					POINT pt[3];
					int startY = 5;
					rt.top += startY;
					pt[0].x = rt.left + (rt.right - rt.left)/4;
					pt[0].y = rt.top - 10;

					pt[1].x = rt.left + (rt.right - rt.left)*3/4;
					pt[1].y = rt.top + 10;

					pt[2].x = rt.right;
					pt[2].y = rt.top;

					::MoveToEx(hPaintDC,rt.left,rt.top,NULL);
					::PolyBezierTo(hPaintDC,pt,3);
					
					::LineTo(hPaintDC,rt.right,rt.top + 18);
					::LineTo(hPaintDC,rt.left, rt.top + 18);
					::LineTo(hPaintDC,rt.left, rt.top);

					EndPath(hPaintDC);
					hrgn = PathToRegion(hPaintDC);
					if(::PtInRegion(hrgn,xPos,yPos))
					{
						m_pPaper->state = st_move;
						m_MousePt.x = xPos;
						m_MousePt.y = yPos;
						this->SetCapture();
					}

					::ReleaseDC(GetHWND(),hPaintDC);
					::DeleteObject(hrgn);
				}
			}
			return true;
	}
	bool xfWin::EvtLButtonUp(TEvent* pEvent,LPARAM lParam)
	{
		if(m_pPaper)
		{
			int xPos = GET_X_LPARAM(pEvent->lParam); 
			int yPos = GET_Y_LPARAM(pEvent->lParam); 
			if(m_pPaper->state == st_move)
			{
				m_pPaper->state = st_normal;
				int dx = xPos - m_MousePt.x;
				int dy = yPos - m_MousePt.y;

				RECT rt;
				::GetWindowRect(GetHWND(),&rt);
				::OffsetRect(&rt,dx,dy);
				::MoveWindow(GetHWND(),rt.left,rt.top,rt.right - rt.left,rt.bottom - rt.top,true);
				this->ReleaseCapture();
			}
			
		}
		return true;
	}

}}
