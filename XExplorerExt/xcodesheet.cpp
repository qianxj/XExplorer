#include "StdAfx.h"
#include "xCodeSheet.h"
#include "xcControl.h"
#include <strstream>
using namespace std;
#include "Scintilla.h"

#include "wpp.h"
#include "wpp5.h"

#include "vl.h"
#include "parser/vlspacemgr.hpp"
#include "parser/vlarraymgr.hpp"
#include "parser/vlparser.hpp"
#include "parser/vlsym.hpp"
#include "parser/vlang.hpp"
#include "parser/vlbnf.hpp"
#include "parser/vlunit.hpp"

bool usevd = false;

#ifdef JS_V8
	#include <v8.h>
	using namespace v8;
#endif

#ifdef JS_SUPPER
/* 
 * This define is for Windows only, it is a work-around for bug 661663. 
 */  
#ifdef _MSC_VER  
#define XP_WIN  
#endif  
  
/* Include the JSAPI header file to get access to SpiderMonkey. */  
#include "jsapi.h"

#endif

bool FileExtIS(wchar_t * fileName,wchar_t* ext)
{
	if(!fileName || !ext && wcslen(fileName)< wcslen(ext) && ext[0]!='.') return false;
	return wcsstr(fileName + wcslen(fileName) - wcslen(ext),ext) ? true:false;
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
		nLenText = 99999999;
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
		nLenText = 99999999;
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
		nLenText = 99999999;
		SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
		SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
		SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

		return 1;
	}

	int output(const tchar* pFile,const tchar* pMsgText)
	{
		if(!m_pOutPut) return 0;

		int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
		nLenText = 99999999;
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

class vlerror : public vl::vlErr
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

xCodeSheet::xCodeSheet(void):/*m_cp(null),*/m_xcp(null),m_xsscp(null),m_xsmparser(null), beval(false),m_btpp(false),m_bvl(false),
 m_bwpp(false),m_wpp(null), m_bwpp5(true),m_wpp5(null)
{
	this->AttachListon(L"functions",CBN_SELCHANGE,(FNEvent)&xCodeSheet::EvtFctSelection);
	this->AttachListon(L"classes",CBN_SELCHANGE,(FNEvent)&xCodeSheet::EvtClsSelection);
	this->AttachListon(WM_NOTIFY,(FNEvent)&xCodeSheet::EvtNotify);
	this->AttachListon(L"command",EN_UPDATE,(FNEvent)&xCodeSheet::EvtEditChanged);
}


xCodeSheet::~xCodeSheet(void)
{
	if(m_xcp)
	{
		if(m_xcp->m_pxvm) delete m_xcp->m_pxvm;
		delete m_xcp;
	}
	//if(m_cp)delete m_cp; 

	if(m_xsscp)
	{
		if(m_xsscp->m_pxvm) delete m_xsscp->m_pxvm;
		delete m_xsscp;
	}
	if(m_xsmparser)delete m_xsmparser;
	if(m_wpp) delete (Wpp *)m_wpp;
}

bool xCodeSheet::EvtEmDblClicked(TEvent * pEvent, LPARAM lParam)
{
	xfControl* pEdit = GetControl(L"output");

	int row  = ::SendMessage(pEdit->GetHWND(),EM_LINEFROMCHAR,-1,0);
	TCHAR buf[255];
	buf[0] = (WORD)255;
	::SendMessage(pEdit->GetHWND(),EM_GETLINE,row,(LPARAM)buf);
	
	return true;
}

bool xCodeSheet::EvtNotify(TEvent* pEvent,LPARAM lParam)
{
	if(pEvent->notify.pnmh->code==SCN_UPDATEUI)
	{
		if(pEvent->notify.pnmh->hwndFrom == GetControl(L"codecontent")->GetHWND())
			return EvtCodeUpdateUI(pEvent,lParam);
	}
	return true;
}

bool xCodeSheet::EvtCodeUpdateUI(TEvent* pEvent,LPARAM lParam)
{
	int caret = SendMessage(pEvent->notify.pnmh->hwndFrom,SCI_GETCURRENTPOS,0,0);
	int line = SendMessage(pEvent->notify.pnmh->hwndFrom,SCI_LINEFROMPOSITION, caret,0);
	
	xfControl* xclist =  GetControl(_T("classes"));
	xfControl* xflist =  GetControl(_T("functions"));
	int count = ::SendMessage(xclist->GetHWND(),CB_GETCOUNT,0,0);
	int i=0;
	line++;
	for(i=0;i<count;i++)
	{
		TCHAR buf[255];
		::SendMessage(GetControl(L"classes")->GetHWND(),CB_GETLBTEXT,i,(LPARAM)buf);
		parser_cpp * xsmparser = (parser_cpp * )m_xsmparser;
		if(!xsmparser) return true;
		sym_class * pclass = xsmparser->m_pEnv->GetClass(buf);
		if(pclass->row<=line && pclass->erow>=line)
		{
			::SendMessage(xclist->GetHWND(),CB_SETCURSEL,i,0);
			ReFillFctInfo(pclass);

			sym_method * pmethod = pclass->pmethod;
			while(pmethod && pmethod->piror) pmethod= (sym_method *) pmethod->piror;
			int index = 0;
			while(pmethod)
			{
				if(pmethod->row<=line && pmethod->erow>=line)
				{
					SendMessage(xflist->GetHWND(),CB_SETCURSEL,index,0);
				}
				index++;
				pmethod = (sym_method *)pmethod->next;
			}
			break;
		}
	}
	if(i>=count)
	{
		::SendMessage(xclist->GetHWND(),WM_SETTEXT,0,NULL);
		::SendMessage(xflist->GetHWND(),WM_SETTEXT,0,NULL);
	}
	return true;
}

bool FetchData(const tchar * p,void* param)
{
	xfControl* pOutPut = (xfControl*)param;
	int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
	SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
	SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)p);
	SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

	return true;
}

bool FetchDataEx(const tchar * p,void* param)
{
	if(p)((std::wstring *)param)->append(p);
	return true;
}
bool xCodeSheet::EvtEditChanged(TEvent * pEvent, LPARAM lParam)
{
	TCHAR buf[255];
	buf[0] = (WORD)255;
	DWORD left, right;
	xfControl* pEdit = GetControl(L"command");
	::SendMessage(pEdit->GetHWND(),EM_GETSEL ,(WPARAM)&left,(LPARAM)&right);
	int row  = ::SendMessage(pEdit->GetHWND(),EM_LINEFROMCHAR,-1,0);
	int index = ::SendMessage(pEdit->GetHWND(),EM_LINEINDEX,row,0); 
	int nLen = ::SendMessage(pEdit->GetHWND(),EM_LINELENGTH,left,0);
	if(nLen==0 && index==left && index==right && row >0)
	{
		nLen = ::SendMessage(pEdit->GetHWND(),EM_LINELENGTH,left - 1,0);
		::SendMessage(pEdit->GetHWND(),EM_GETLINE,row - 1,(LPARAM)buf);
		buf[nLen] = 0;
		if(buf[0]=='?')
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return true;

			vlerror* perr = new vlerror();
			xfControl* pOutPut = m_pParentWin->GetControl(_T("output"));
			perr->m_pOutPut = pOutPut;
			
			/*
			vl::vlang3 vl;
			vl.setError(perr);
			vl.parse(pXml);
			*/

			::SendMessage(pOutPut->GetHWND(),EM_SETLIMITTEXT,-1,0);
			vl::vlang3 vl;
			vl.setError(perr);

			vl.GetSource = xfUriLoader::LoadScript;
			vl.FreeSource= xfUriLoader::FreeScript;
			vl.GetSourceDT = xfUriLoader::GetScriptDT;
			vl.GetSourceVER = xfUriLoader::GetScriptVER;

			vl.parse(pXml);
			vl.query(buf+1,pOutPut,FetchData,1);
			delete perr;

			::SysFreeString(pXml);
		}
	}
	
	return true;
}

bool xCodeSheet::EvtClsSelection(TEvent * pEvent, LPARAM lParam)
{
	TCHAR buf[255];
	int nIndex = (int)::SendMessage(GetControl(L"classes")->GetHWND(),CB_GETCURSEL,0,0);
	::SendMessage(GetControl(L"classes")->GetHWND(),CB_GETLBTEXT,nIndex,(LPARAM)buf);
	
	parser_cpp * xsmparser = (parser_cpp * )m_xsmparser;
	if(!xsmparser) return true;
	sym_class * pclass = xsmparser->m_pEnv->GetClass(buf);
	if(pclass) this->ReFillFctInfo(pclass);
	return true;
}
bool xCodeSheet::EvtFctSelection(TEvent * pEvent, LPARAM lParam)
{
	int nIndex = (int)::SendMessage(GetControl(L"functions")->GetHWND(),CB_GETCURSEL,0,0);
	if(nIndex >=0)
	{
		TCHAR clsname[255];
		int nIndex1 = (int)::SendMessage(GetControl(L"classes")->GetHWND(),CB_GETCURSEL,0,0);
		::SendMessage(GetControl(L"classes")->GetHWND(),CB_GETLBTEXT,nIndex1,(LPARAM)clsname);

		parser_cpp * xsmparser = (parser_cpp * )m_xsmparser;
		if(!xsmparser) return true;

		sym_class * pclass = xsmparser->m_pEnv->GetClass(clsname);
		if(!pclass) return true;

		sym_method * pmethod = pclass->pmethod;
		while(pmethod && pmethod->piror) pmethod = (sym_method *)pmethod->piror;
		while(pmethod && nIndex!=0)
		{
			nIndex--;
			pmethod = (sym_method *)pmethod->next;
		}

		if(!pmethod) return true;
		int sRow = pmethod->row;

		XSEdit * pedit = (XSEdit *)GetControl(_T("codecontent"));
		SendMessage(pedit->GetHWND(),SCI_GOTOLINE,sRow - 1,0);
		if(!SendMessage(pedit->GetHWND(),SCI_GETFOLDEXPANDED,sRow,0))
			SendMessage(pedit->GetHWND(),SCI_TOGGLEFOLD,sRow,0);
		SetFocus(pedit->GetHWND());
	}
	return true;
}

bool OnCodeDwellEx(void * pOwner,Hxsoft::XFrame::XSEdit *pEdit,int pos,const char * word)
{
	int i=0;
	if(word)
	{
		/*
		for(i=0;i<(int)((xCodeSheet*)pOwner)->m_errors.size();i++)
		{
			if(((xCodeSheet*)pOwner)->m_errors[i].pt.x<=pos && 
				((xCodeSheet*)pOwner)->m_errors[i].pt.x + 
				((xCodeSheet*)pOwner)->m_errors[i].pt.y >=pos)break;
		}
		if(i<((int)(xCodeSheet*)pOwner)->m_errors.size())
		{
			LPCTSTR  pItems = ((xCodeSheet*)pOwner)->m_errors[i].errText.c_str(); 
			int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
			char *pszMulti = new char[cchMulti + 1];
			::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
			pszMulti[cchMulti] = 0;

			SendMessage(pEdit->GetHWND(),SCI_CALLTIPSHOW,pos,(sptr_t)pszMulti);

			delete pszMulti;
		}
		*/
	}else
		SendMessage(pEdit->GetHWND(),SCI_CALLTIPCANCEL,0,0);
	return true;
}
bool OnCodeEditCharEx(void * pOwner,Hxsoft::XFrame::XSEdit *pEdit,char ch)
{
	return false;

	TCHAR title[255];
	::GetWindowText(((xCodeSheet*)pOwner)->GetHWND(),title,255);
	if(FileExtIS(title,_T(".js"))) return false;
	//if(FileExtIS(title,_T(".vl"))) return false;
	if(FileExtIS(title,_T(".vm"))) return false;
	if(FileExtIS(title,_T(".vf"))) return false;
	if(FileExtIS(title,_T(".voc")))return false;
	if(FileExtIS(title,_T(".vc"))) return false;
	if(FileExtIS(title,_T(".vx"))) return false;
	if(!usevd && FileExtIS(title,_T(".vd"))) return false;
	
	//if(!FileExtIS(title,_T(".tpp"))) return false;
	if(!FileExtIS(title,_T(".vd")) && !FileExtIS(title,_T(".vl"))) return false;

	if((ch==';' || ch=='}' || ch=='\r'))
	{
		((xCodeSheet*)pOwner)->Eval(-1);
	}

	if(ch=='.' || ch==':')
	{
		int caret = SendMessage(pEdit->GetHWND(),SCI_GETCURRENTPOS,0,0);
		int line = SendMessage(pEdit->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
		int lineStart = SendMessage(pEdit->GetHWND(),SCI_POSITIONFROMLINE, line,0);

		int len = SendMessage(pEdit->GetHWND(),SCI_LINELENGTH,line,0);
		
		if(len > 0)
		{
			char* text = new char[len +1];
			char var[256];
			char var1[256];

			SendMessage(pEdit->GetHWND(),SCI_GETLINE,line,(LPARAM)text);
			text[len] = 0;

			int nPos = len -1;
			if(caret - lineStart < nPos)nPos = caret - lineStart - 1;
			int sPos = 0;
			if(nPos >=0 && text[nPos]!='.' && text[nPos]!=':')
				nPos--;
			if(nPos >=0 && text[nPos]=='.')
				nPos--;
			else if(nPos >=0 && text[nPos]==':')
			{
				nPos--;
				if(!(nPos >=0 && text[nPos]==':'))return true;
				nPos--;
			}
			while(nPos >=0 && ((text[nPos]>='a' && text[nPos]<='z') || (text[nPos]>='A' && text[nPos]<='Z') ||
				(text[nPos]>='0' && text[nPos]<='9')||text[nPos]=='_'))
			{
				if(sPos<254) var1[sPos++] = text[nPos--];
			}
			var1[sPos] = 0;
			for(int i=strlen(var1) -1;i>=0;i--)
				var[strlen(var1) -1  - i]=var1[i];
			var[sPos] = 0;

			if(!var[0]) strcat_s(var,256, "_global");
			if(var[0])
			{
				wpp::wpp_runtime runtime;
				wpp::wpp_context context(&runtime);
				if(!((xCodeSheet*)pOwner)->m_wpp)	((xCodeSheet*)pOwner)->m_wpp = new Wpp;
				context.set_wpp_ptr((Wpp*)((xCodeSheet*)pOwner)->m_wpp);
				((xCodeSheet*)pOwner)->Eval(-99,&context);

				BSTR bstr;
				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]=0;
				::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);

				LPTSTR pItems = context.getVarAutoString(bstr);
				if(pItems)
				{
					int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
					char *pszMulti = new char[cchMulti + 1];
					::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
					pszMulti[cchMulti] = 0;
					SendMessage(pEdit->GetHWND(),SCI_AUTOCSHOW,0,(sptr_t)pszMulti);
					delete pszMulti;
					context.freeAutoString(pItems);
				}
				::SysFreeString(bstr);
			}
		}
		return true;
	}
	
	if(ch=='(')
	{
		int caret = SendMessage(pEdit->GetHWND(),SCI_GETCURRENTPOS,0,0);
		int line = SendMessage(pEdit->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
		int lineStart = SendMessage(pEdit->GetHWND(),SCI_POSITIONFROMLINE, line,0);

		int len = SendMessage(pEdit->GetHWND(),SCI_LINELENGTH,line,0);
		
		if(len > 0)
		{
			char* text = new char[len +1];
			SendMessage(pEdit->GetHWND(),SCI_GETLINE,line,(LPARAM)text);
			text[len] = 0;

			char var[256];
			char var1[256];
			int nPos = len -1;
			if(caret - lineStart < nPos)nPos = caret - lineStart - 1;
			int sPos = 0;
			if(nPos >=0 && text[nPos]!='(')nPos--;
			if(nPos >=0 && text[nPos]=='(')nPos--;
			while(nPos >=0 && ((text[nPos]>='a' && text[nPos]<='z') || (text[nPos]>='A' && text[nPos]<='Z') ||
				(text[nPos]>='0' && text[nPos]<='9')||text[nPos]=='_' || text[nPos]=='.' || text[nPos]==':'))
			{
				if(sPos<254) var1[sPos++] = text[nPos--];
			}
			var1[sPos] = 0;
			for(int i=strlen(var1) -1;i>=0;i--)
				var[strlen(var1) -1  - i]=var1[i];
			var[sPos++] = '(';
			var[sPos] = 0;

			const char *pos1 = strchr(var,'.');
			const char *pos2 = strchr(var,'(');
			if(pos1 > 0 && pos1 < pos2)
			{
				wpp::wpp_runtime runtime;
				wpp::wpp_context context(&runtime);
				if(!((xCodeSheet*)pOwner)->m_wpp)
					((xCodeSheet*)pOwner)->m_wpp = new Wpp;
				context.set_wpp_ptr((Wpp*)((xCodeSheet*)pOwner)->m_wpp);
				((xCodeSheet*)pOwner)->Eval(-99,&context);

				BSTR bstr;
				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]=0;
				::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);

				LPTSTR pItems = context.getFctTipString(bstr);
				if(pItems)
				{
					int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
					char *pszMulti = new char[cchMulti + 1];
					::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
					pszMulti[cchMulti] = 0;
					SendMessage(pEdit->GetHWND(),SCI_CALLTIPSHOW,caret,(sptr_t)pszMulti);
					delete pszMulti;
					context.freeAutoString(pItems);
				}
				::SysFreeString(bstr);
			}
		}
	}

#if 0
	TCHAR title[255];
	::GetWindowText(((xCodeSheet*)pOwner)->GetHWND(),title,255);
	if(FileExtIS(title,_T(".js"))) return false;
	if(FileExtIS(title,_T(".vl"))) return false;
	if(FileExtIS(title,_T(".vm"))) return false;
	if(FileExtIS(title,_T(".vx"))) return false;
	if(FileExtIS(title,_T(".vd"))) return false;
	if(FileExtIS(title,_T(".vf"))) return false;
	if(FileExtIS(title,_T(".voc"))) return false;
	if(FileExtIS(title,_T(".vc"))) return false;

	if(!FileExtIS(title,_T(".tpp"))) return false;

	if((ch==';' || ch=='}' || ch=='\r') && ((xCodeSheet*)pOwner)->m_btpp)
	{
		((xCodeSheet*)pOwner)->Eval(-1);
	}

	if(ch=='.' || ch==':')
	{
		int caret = SendMessage(pEdit->GetHWND(),SCI_GETCURRENTPOS,0,0);
		int line = SendMessage(pEdit->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
		int lineStart = SendMessage(pEdit->GetHWND(),SCI_POSITIONFROMLINE, line,0);

		int len = SendMessage(pEdit->GetHWND(),SCI_LINELENGTH,line,0);
		
		if(len > 0)
		{
			char* text = new char[len +1];
			char var[256];
			char var1[256];

			SendMessage(pEdit->GetHWND(),SCI_GETLINE,line,(LPARAM)text);
			text[len] = 0;


			int nPos = len -1;
			if(caret - lineStart < nPos)nPos = caret - lineStart - 1;
			int sPos = 0;
			if(nPos >=0 && text[nPos]=='.')
				nPos--;
			else if(nPos >=0 && text[nPos]==':')
			{
				nPos--;
				if(!(nPos >=0 && text[nPos]==':'))return true;
				nPos--;
			}
			while(nPos >=0 && ((text[nPos]>='a' && text[nPos]<='z') || (text[nPos]>='A' && text[nPos]<='Z') ||
				(text[nPos]>='0' && text[nPos]<='9')||text[nPos]=='_'))
			{
				if(sPos<254) var1[sPos++] = text[nPos--];
			}
			var1[sPos] = 0;
			for(int i=strlen(var1) -1;i>=0;i--)
				var[strlen(var1) -1  - i]=var1[i];
			var[sPos] = 0;

			if(!var[0]) strcat_s(var,256, "_global");
			if(var[0])
			{
				if(!((xCodeSheet*)pOwner)->m_btpp)
				{
					xsscomplier * xsscp = (xsscomplier *)((xCodeSheet*)pOwner)->m_xsscp;
					if(!xsscp)
					{
						((xCodeSheet*)pOwner)->InitXssComplier();
						((xCodeSheet*)pOwner)->Eval(-99);
						xsscp = (xsscomplier *)((xCodeSheet*)pOwner)->m_xsscp;
					}else
						((xCodeSheet*)pOwner)->Eval(-99);

					BSTR bstr;
					int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
					bstr = SysAllocStringLen(NULL,cchWide + 1);
					bstr[cchWide]=0;
					::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);

					LPTSTR pItems = xsscp->getVarAutoString(bstr);

					if(pItems)
					{
						int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
						char *pszMulti = new char[cchMulti + 1];
						::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
						pszMulti[cchMulti] = 0;
						SendMessage(pEdit->GetHWND(),SCI_AUTOCSHOW,0,(sptr_t)pszMulti);
						delete pszMulti;
						delete pItems;
					}
					::SysFreeString(bstr);
				}else
				{
					parser_cpp * xsmparser = new parser_cpp();
					((xCodeSheet*)pOwner)->Eval(-99,xsmparser);

					BSTR bstr;
					int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
					bstr = SysAllocStringLen(NULL,cchWide + 1);
					bstr[cchWide]=0;
					::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);

					LPTSTR pItems = xsmparser->m_pEnv->getVarAutoString(bstr);
					if(pItems)
					{
						int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
						char *pszMulti = new char[cchMulti + 1];
						::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
						pszMulti[cchMulti] = 0;
						SendMessage(pEdit->GetHWND(),SCI_AUTOCSHOW,0,(sptr_t)pszMulti);
						delete pszMulti;
						xsmparser->m_pEnv->Free(pItems);
					}
					::SysFreeString(bstr);
					delete xsmparser;
				}
			}
		}
		return true;
	}
	
	if(ch=='(')
	{
		int caret = SendMessage(pEdit->GetHWND(),SCI_GETCURRENTPOS,0,0);
		int line = SendMessage(pEdit->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
		int lineStart = SendMessage(pEdit->GetHWND(),SCI_POSITIONFROMLINE, line,0);

		int len = SendMessage(pEdit->GetHWND(),SCI_LINELENGTH,line,0);
		
		if(len > 0)
		{
			char* text = new char[len +1];
			SendMessage(pEdit->GetHWND(),SCI_GETLINE,line,(LPARAM)text);
			text[len] = 0;

			char var[256];
			char var1[256];
			int nPos = len -1;
			if(caret - lineStart < nPos)nPos = caret - lineStart - 1;
			int sPos = 0;
			if(nPos >=0 && text[nPos]=='(')nPos--;
			while(nPos >=0 && ((text[nPos]>='a' && text[nPos]<='z') || (text[nPos]>='A' && text[nPos]<='Z') ||
				(text[nPos]>='0' && text[nPos]<='9')||text[nPos]=='_' || text[nPos]=='.' || text[nPos]==':'))
			{
				if(sPos<254) var1[sPos++] = text[nPos--];
			}
			var1[sPos] = 0;
			for(int i=strlen(var1) -1;i>=0;i--)
				var[strlen(var1) -1  - i]=var1[i];
			var[sPos++] = '(';
			var[sPos] = 0;

			const char *pos1 = strchr(var,'.');
			const char *pos2 = strchr(var,'(');
			if(pos1 > 0 && pos1 < pos2)
			{
				if(!((xCodeSheet*)pOwner)->m_btpp)
				{
					xsscomplier * xsscp = (xsscomplier *)((xCodeSheet*)pOwner)->m_xsscp;
					if(!xsscp)
					{
						((xCodeSheet*)pOwner)->InitXssComplier();
						((xCodeSheet*)pOwner)->Eval(-99);
						xsscp = (xsscomplier *)((xCodeSheet*)pOwner)->m_xsscp;
					}else
						((xCodeSheet*)pOwner)->Eval(-99);

					BSTR bstr;
					int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
					bstr = SysAllocStringLen(NULL,cchWide + 1);
					bstr[cchWide]=0;
					::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);
				
					LPTSTR pItems = xsscp->getFctTipString(bstr);
					if(pItems)
					{
						int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
						char *pszMulti = new char[cchMulti + 1];
						::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
						pszMulti[cchMulti] = 0;

						SendMessage(pEdit->GetHWND(),SCI_CALLTIPSHOW,caret,(sptr_t)pszMulti);

						delete pszMulti;
						delete pItems;
					}
					::SysFreeString(bstr);
				}else
				{
					parser_cpp * xsmparser = new parser_cpp();
					((xCodeSheet*)pOwner)->Eval(-99,xsmparser);

					BSTR bstr;
					int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
					bstr = SysAllocStringLen(NULL,cchWide + 1);
					bstr[cchWide]=0;
					::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);
				
					LPTSTR pItems= xsmparser->m_pEnv->getFctTipString(bstr);
					if(pItems)
					{
						int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
						char *pszMulti = new char[cchMulti + 1];
						::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
						pszMulti[cchMulti] = 0;

						SendMessage(pEdit->GetHWND(),SCI_CALLTIPSHOW,caret,(sptr_t)pszMulti);

						delete pszMulti;
						xsmparser->m_pEnv->Free(pItems);
					}
					::SysFreeString(bstr);
					delete xsmparser;
				}
			}
		}
	}
#endif 

	return false;
}

int xCodeSheet::OnOpen()
{
	xfControl * pEdit = GetControl(L"codecontent");
	if(pEdit)
	{
		((XSEdit *)pEdit)->AddExtent(this,(XSEdit *)pEdit,OnCodeEditCharEx,OnCodeDwellEx);
		SendMessage(pEdit->GetHWND(),SCI_AUTOCSETMAXHEIGHT,14,0);

	}
	return 1;
	
}


/*
class myerror : public xerror
{
public:
	xfControl* m_pOutPut;
	int adderror(lptxstr pFile,int nrow,int ncol,int nError,lptxstr pErrText)
	{
		int nLen = (int)strlen(pErrText);
		int _convert = MultiByteToWideChar(CP_ACP, 0, pErrText, nLen, NULL, 0);
		TCHAR * psz = new TCHAR[_convert + 1];
		int nRet = MultiByteToWideChar(CP_ACP, 0,pErrText, nLen, psz, _convert);
		psz[_convert]='\0';

		int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
		SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
		SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
		SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

		delete psz;
		return xerror::adderror(pFile,nrow,ncol,nError,pErrText);
	}
	int addwarn(lptxstr pFile,int nrow,int ncol,int nError,lptxstr pErrText)
	{
		int nLen = (int)strlen(pErrText);
		int _convert = MultiByteToWideChar(CP_ACP, 0, pErrText, nLen, NULL, 0);
		TCHAR * psz = new TCHAR[_convert + 1];
		int nRet = MultiByteToWideChar(CP_ACP, 0,pErrText, nLen, psz, _convert);
		psz[_convert]='\0';

		int nLenText = ::GetWindowTextLength(m_pOutPut->m_hWnd);
		SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
		SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)psz);
		SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

		delete psz;
		return xerror::addwarn(pFile,nrow,ncol,nError,pErrText);
	}
};
*/

class myerror1 : public xvmerror
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

class myerror2 : public errobj
{
public:
	xfControl* m_pOutPut;
	xfControl* m_pEdit;
	//xCodeSheet * m_psheet;
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
		SendMessage(m_pEdit->m_hWnd,SCI_INDICSETSTYLE,9+i, i);
		SendMessage(m_pEdit->m_hWnd,SCI_INDICSETFORE,9+i, 0x0000ff);
		SendMessage(m_pEdit->m_hWnd,SCI_SETINDICATORCURRENT,9+i, 0);

		int lineStart = SendMessage(m_pEdit->m_hWnd,SCI_POSITIONFROMLINE, nrow - 1,0);
		//SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,lineStart + ncol - 3,5);
		SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,nstart,nlen + 1);
		
		/*xCodeSheet::TError te;
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
		SendMessage(m_pEdit->m_hWnd,SCI_INDICSETSTYLE,9+i, i);
		SendMessage(m_pEdit->m_hWnd,SCI_INDICSETFORE,9+i, 0xff00ff);
		SendMessage(m_pEdit->m_hWnd,SCI_SETINDICATORCURRENT,9+i, 0);

		int lineStart = SendMessage(m_pEdit->m_hWnd,SCI_POSITIONFROMLINE, nrow - 1,0);
		//SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,lineStart + ncol - 3,5);
		SendMessage(m_pEdit->m_hWnd,SCI_INDICATORFILLRANGE,nstart,nlen + 1);

		/*xCodeSheet::TError te;
		te.pt.x = nstart;
		te.pt.y = nlen;
		te.errText = pErrText;
		m_psheet->m_errors.push_back(te);*/

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
/*
int ProcessNode(complier &cp, astnode * pNode,HTREEITEM hItem,xfControl * pTree)
{
	LPSTR pStr = null;
	if(pNode)
		pStr = (LPSTR)cp.GetLabel(pNode);

	if(pStr)
	{
		TVINSERTSTRUCTA tvins;
		tvins.hInsertAfter = TVI_LAST;
		tvins.hParent = hItem;
		if(pNode)
			tvins.item.pszText = pStr;
		else
			tvins.item.pszText = "null";
		tvins.item.cchTextMax = 255;
		tvins.item.lParam = (LPARAM)pNode;
		tvins.item.iImage =1;
		tvins.item.iSelectedImage =1;
		tvins.item.mask = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		hItem = (HTREEITEM)SNDMSG(pTree->m_hWnd, TVM_INSERTITEMA, 0, (LPARAM)(LPTV_INSERTSTRUCTA)(&tvins));
	}
	if(pNode)
	{
		if(pNode->m_pchilds)
		{
			for(int i=0;i<(int)pNode->m_pchilds->size();i++)
			{
				if((*pNode->m_pchilds)[i])
					ProcessNode(cp,(*pNode->m_pchilds)[i],hItem,pTree);
			}
		}
		if(pStr) delete pStr;

		if(pNode->m_pNestNameSpec)ProcessNode(cp,pNode->m_pNestNameSpec,hItem,pTree);
	}
	return 1;
}
*/
int xCodeSheet::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmFileSave"))==0)
	{
		m_pParentWin->OnXCommand(pStrID,pControl);
		//OnXCommand(L"xmEval",NULL);
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmEditFind"))==0)
	{

		OnCodeFind();
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmEditReplace"))==0)
	{
		OnCodeReplace();
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmExec"))==0)
	{
		TCHAR title[255];
		::GetWindowText(m_hWnd,title,255);
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);
#ifdef JS_V8
		if(FileExtIS(title,_T(".js")))
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在V8编译...\r\n");

			// Create a stack-allocated handle scope.
			HandleScope handle_scope;

			// Create a new context.
			Persistent<Context> context = Context::New();

			// Enter the created context for compiling and
			// running the hello world script. 
			Context::Scope context_scope(context);

			// Create a string containing the JavaScript source code.
			Handle<String> source = String::New((const uint16_t *)pXml);

			// Compile the source code.
			Handle<Script> script = Script::Compile(source);

			// Run the script to get the result.
			Handle<Value> result = script->Run();

			// Dispose the persistent context.
			context.Dispose();

			// Convert the result to an ASCII string and print it.
			String::AsciiValue ascii(result);

			if(*ascii) 
			{
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessageA(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)*ascii);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}

			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)L"\r\n运行完成\r\n");
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}
		else
#endif
		if(FileExtIS(title,_T(".tpp")))
		{
			if(!m_xsmparser) return 0;
			gener_tpp* gt = new gener_tpp;
			gt->m_pEnv = ((parser_cpp *)m_xsmparser)->m_pEnv;

			myerror2* perr = new myerror2();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			perr->m_pEdit = GetControl(L"codecontent");
			m_errors.clear();
			gt->m_perror = perr;
			gt->m_pParser = (parser_cpp *)m_xsmparser;

			HWND hWnd = pOutPut->m_hWnd;
			BSTR bstr = NULL ;
			bstr = gt->Exec();
			::SetWindowText(pOutPut->m_hWnd,L"正在运行...\r\n");
			delete gt;
			int nLenText = ::GetWindowTextLength(hWnd);
			if(bstr)
			{
				SendMessage(hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(hWnd,EM_REPLACESEL,false,(LPARAM)bstr);
				SendMessage(hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}

			if(bstr)::SysFreeString(bstr);
			nLenText = ::GetWindowTextLength(hWnd);
			SendMessage(hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(hWnd,EM_REPLACESEL,false,(LPARAM)L"\r\n运行完成\r\n");
			SendMessage(hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}
		else if(FileExtIS(title,_T(".xss")))
		{
			if(!m_xsscp) return 0;
			if(!m_xsscp->m_pxvm) return 0;
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在运行...\r\n");
			
			/*
			if(m_xsscp->m_pxvm->m_initsstate)
			{
				for(int i=0;i<(int)m_xsscp->m_pCodes->size();i++)
				{
					xcomplier::parsepcode(m_xsscp->m_pxvm,(*m_xsscp->m_pCodes)[i]);
				}
			}*/

			//m_xsscp->m_pxvm->reset();
			//void * p = m_xsscp->m_pxvm->m_codes;

			//m_xsscp->m_pxvm->m_codes =  m_xsscp->m_pxvm->m_codesEx;
			m_xsscp->m_pxvm->m_error = m_xsscp->m_error;

			m_xsscp->m_pxvm->m_pc = 0;
			m_xsscp->m_pxvm->exec();

			//m_xsscp->m_pxvm->m_codes = (xcode::xarrays<xcode::code *,xcode::code> *)p;

			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)L"运行完成\r\n");
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}else if(/*FileExtIS(title,_T(".vl")) ||*/ FileExtIS(title,_T(".vm")) ||  FileExtIS(title,_T(".vf")) ||
			FileExtIS(title,_T(".vx"))||/* FileExtIS(title,_T(".vd")) ||*/ FileExtIS(title,_T(".voc")) ||  FileExtIS(title,_T(".vc")))
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return true;

			vlerror* perr = new vlerror();
			xfControl* pOutPut = m_pParentWin->GetControl(_T("output"));
			perr->m_pOutPut = pOutPut;
			
			/*
			vl::vlang3 vl;
			vl.setError(perr);
			vl.parse(pXml);
			*/

			vl::vlang3 vl;
			vl.setError(perr);

			vl.GetSource = xfUriLoader::LoadScript;
			vl.FreeSource= xfUriLoader::FreeScript;
			vl.GetSourceDT = xfUriLoader::GetScriptDT;
			vl.GetSourceVER = xfUriLoader::GetScriptVER;

			vl.parse(pXml);
			vl.query(L"exec main",pOutPut,FetchData,1);
			delete perr;

			::SysFreeString(pXml);
		}else if(( FileExtIS(title,_T(".vd"))) || FileExtIS(title,_T(".vl")))
		{
			if(GetKeyState(VK_CONTROL) & 0x8000)
			{
				m_bwpp= true;
				m_bwpp5= false;
			}
			else
			{
				m_bwpp5= true;
				m_bwpp = false;
			}
			if(m_bwpp)
			{
				BSTR pXml = NULL;
				XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
				pControl->GetContent(&pXml);
				if(!pXml) return true;

				wlerror* perr = new wlerror();
				xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
				
				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
				perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
				
				::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
				::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");
				
				::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

				wpp::wpp_runtime runtime;
				wpp::wpp_context context(&runtime);
				if(!m_wpp)	m_wpp = new Wpp;
				context.set_wpp_ptr((Wpp*)m_wpp);

				runtime.GetSource	 = xfUriLoader::LoadScript;
				runtime.FreeSource	 = xfUriLoader::FreeScript;
				runtime.GetSourceDT  = xfUriLoader::GetScriptDT;
				runtime.GetSourceVER = xfUriLoader::GetScriptVER;

				context.set_error(perr);
				context.parse(pXml);

				TCHAR buf[255];
				_stprintf_s(buf,80,L"编译完成: \r\n");
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				int fn = runtime.find_method(&context,L"test",L"main");
				if(fn)runtime.call_method(&context,fn);
				delete perr;

				buf[255];
				_stprintf_s(buf,80,L"\r\n执行完成: ");
				nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				::SysFreeString(pXml);
			}
			else if(m_bwpp5)
			{
				BSTR pXml = NULL;
				XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
				pControl->GetContent(&pXml);
				if(!pXml) return true;

				wlerror5* perr = new wlerror5();
				xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
				
				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
				perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
				
				::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
				::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");
				
				::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

				xlang::wpp_runtime runtime;
				xlang::wpp_context context(&runtime);
				if(!m_wpp5)	m_wpp5 = new xlang::Wpp5;
				context.set_wpp_ptr((xlang::Wpp5*)m_wpp);

				runtime.GetSource	 = xfUriLoader::LoadScript;
				runtime.FreeSource	 = xfUriLoader::FreeScript;
				runtime.GetSourceDT  = xfUriLoader::GetScriptDT;
				runtime.GetSourceVER = xfUriLoader::GetScriptVER;

				context.set_error(perr);
				context.parse(pXml);

				TCHAR buf[255];
				_stprintf_s(buf,80,L"编译完成: \r\n");
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				int fn = runtime.find_method(&context,L"test",L"main");
				if(fn)runtime.call_method(&context,fn);
				delete perr;

				buf[255];
				_stprintf_s(buf,80,L"\r\n执行完成: ");
				nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				wchar_t* text = new wchar_t[nLenText + 1];
				GetWindowText(pOutPut->GetHWND(),text,nLenText + 1);
				XSEdit * pControlEx = (XSEdit *)m_pParentWin->GetControl(_T("vdata"));
				pControlEx->SetContent((BSTR)text,L".vl");
				delete text;

				::SysFreeString(pXml);
			}
		}/*else
			if(m_cp)m_cp->exec();*/
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
		return 1;
	}
	else if(pStrID && _tcsicmp(pStrID,_T("xmEval"))==0)
	{

		TCHAR title[255];
		::GetWindowText(m_hWnd,title,255);
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;

#ifdef JS_SUPPER
		if(FileExtIS(title,_T(".js")))
		{
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在FF编译...\r\n");

			jsval rval;  
			JSString *str;  
			JSBool ok;  
			const char *filename = "noname";  
			uintN lineno = 0;  
		  
			JS_BeginRequest(xfApp::GetApp()->m_jscx);
			ok =  JS_EvaluateUCScript(xfApp::GetApp()->m_jscx, xfApp::GetApp()->m_jsglobal, pXml, _tcslen(pXml),  
				  				   filename, lineno, &rval);  
			if(! ((rval == NULL) | (rval == JS_FALSE))) 
			{
				str = JS_ValueToString(xfApp::GetApp()->m_jscx, rval);  
				//printf("%s\n", JS_EncodeString(xfApp::GetApp()->m_jscx, str));

				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessageA(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)JS_EncodeString(xfApp::GetApp()->m_jscx, str));
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}
			JS_EndRequest(xfApp::GetApp()->m_jscx);
			
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)L"\r\n运行完成\r\n");
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}

		else
#endif
		/*
		if((GetKeyState(VK_CONTROL) & 0x8000))
		{
			LPTSTR bstr = pXml;
			int _convert = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);
			char* pszA = new char[_convert];
			int nRet = WideCharToMultiByte(CP_ACP, 0, bstr, -1, pszA, _convert, NULL, NULL);
			istrstream source(pszA); 

			ostrstream out;

			if(m_cp)delete m_cp;
			m_cp = new complier; 
			complier & cp = *m_cp;
			myerror* perr = new myerror();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			
			cp.m_error = perr;
			cp.eval(&source,&out,true); 

			TCHAR buf[80];
			_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",perr->m_pErrors->size(),perr->m_pWarns->size());

			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			
			xfControl* pTree = pControl->GetWin()->m_pParentWin->GetControl(_T("asttree"));
			TreeView_DeleteAllItems(pTree->m_hWnd);
			ProcessNode(cp,cp.m_prootnode,TVI_ROOT, pTree);

		}else*/ if(FileExtIS(title,_T(".xss")))
		{
			if(m_xsscp)
			{
				if(m_xsscp->m_pxvm) delete m_xsscp->m_pxvm;
				delete m_xsscp;
			}
		
			m_xsscp = new xsscomplier();
			m_xsscp->m_pxvm = new xvmcore;

			myerror1* perr = new myerror1();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			
			m_xsscp->m_error = perr;
			m_xsscp->eval(pXml);

			TCHAR buf[255];
			TCHAR buf1[255];

			xfControl* PCode = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
			::SetWindowText(PCode->m_hWnd,L"");

			int nlevel = 0;

			buf1[0] = 0;
			std::vector<int> seqs;
			int index = 0;  
			seqs.push_back(0);
			bool bdesc = false;
			for(int i=0;i<(int)m_xsscp->m_pCodes->size();i++)
			{
				int nLenText = ::GetWindowTextLength(PCode->m_hWnd);
				SendMessage(PCode->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				if((*m_xsscp->m_pCodes)[i])
				{
					if(::_tcsstr((*m_xsscp->m_pCodes)[i],_T("blkstart"))== (*m_xsscp->m_pCodes)[i])
					{

						_stprintf_s(buf,255,_T("%s{ %s\r\n"),buf1,(*m_xsscp->m_pCodes)[i] + _tcslen(_T("blkstart")));
						buf1[nlevel*3] = ' ';
						buf1[nlevel*3 + 1] = ' ';
						buf1[nlevel*3 + 2] = ' ';
						nlevel++;
						buf1[nlevel*3] = 0;
					}else if(::_tcsstr((*m_xsscp->m_pCodes)[i],_T("blkend"))== (*m_xsscp->m_pCodes)[i])
					{
						if(nlevel == index) index --;
						nlevel--;
						buf1[nlevel*3] = 0;
						_stprintf_s(buf,255,_T("%s}\r\n"),buf1);
					}else
					{
						if((*m_xsscp->m_pCodes)[i][0]=='.')
						{
							index++;
							seqs.push_back(0);
							_stprintf_s(buf,255,_T("%s%s"),buf1,(*m_xsscp->m_pCodes)[i]);
						}else
						{
							_stprintf_s(buf,255,_T("%s%d  %s"),buf1,seqs[index],(*m_xsscp->m_pCodes)[i]);
							seqs[index]++;

						}
					}
				}else
				{
					_stprintf_s(buf,80,_T("#invalid\r\n"));
				}
				SendMessage(PCode->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(PCode->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}

			_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",perr->m_pErrors->size(),perr->m_pWarns->size());

			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}else if(FileExtIS(title,_T(".xsm")) || FileExtIS(title,_T(".tpp")))
		{
			if(m_xsmparser) delete m_xsmparser;
		
			m_xsmparser = new parser_cpp();
			((parser_cpp*)m_xsmparser)->GetSource = xfUriLoader::LoadScript;
			((parser_cpp*)m_xsmparser)->FreeSource= xfUriLoader::FreeScript;
			((parser_cpp*)m_xsmparser)->GetSourceDT = xfUriLoader::GetScriptDT;
			((parser_cpp*)m_xsmparser)->GetSourceVER = xfUriLoader::GetScriptVER;

			myerror2* perr = new myerror2();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			perr->m_pEdit = GetControl(L"codecontent");
			m_errors.clear();

			((parser_cpp *)m_xsmparser)->m_perror = perr;
			SendMessage(perr->m_pEdit->m_hWnd,SCI_SETMOUSEDWELLTIME,1000,0);
			int len =SendMessage(perr->m_pEdit->m_hWnd,SCI_GETTEXTLENGTH,0,0);
			SendMessage(perr->m_pEdit->m_hWnd,SCI_INDICATORCLEARRANGE,0,len);
			
			TCHAR buf[255];
			
			int startIndex = 0;

			lexer* cp = new lexer;
			((parser_cpp *)m_xsmparser)->cp = cp;
			cp->setsource(pXml,null);
			cp->insymbol();
			bool ok = false;
			if(((parser_cpp *)m_xsmparser)->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
			((parser_cpp *)m_xsmparser)->cp = null;
			cp->source = null;
			startIndex = cp->startIndex;
			delete cp;

			int endIndex =((parser_cpp *)m_xsmparser)->m_pEnv->GetCurMetaTable()->pTableSeq[__atom_sym_meta_typedef].size();
			this->ReFillClassInfo(startIndex,endIndex);

			/*
			xfControl* PCode = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
			::SetWindowText(PCode->m_hWnd,L"");
			gener_tpp* gt = new gener_tpp;
			gt->m_pEnv = m_xsmparser->m_pEnv;

			BSTR bstr = NULL ;
			bstr = gt->DeComplier();
			SetWindowText(PCode->m_hWnd,bstr);
			delete gt;
			if(bstr)::SysFreeString(bstr);
			*/

			_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",perr->m_pErrors->size(),perr->m_pWarns->size());
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

		}
		else if(/*FileExtIS(title,_T(".vl")) ||*/ FileExtIS(title,_T(".vm")) ||  FileExtIS(title,_T(".vf")) ||
			FileExtIS(title,_T(".vx"))|| FileExtIS(title,_T(".voc")) ||  FileExtIS(title,_T(".vc")))
		{
			/*if(m_xsmparser) delete m_xsmparser;
		
			m_xsmparser = new parser_cpp();
			((parser_cpp*)m_xsmparser)->GetSource = xfUriLoader::LoadScript;
			((parser_cpp*)m_xsmparser)->FreeSource= xfUriLoader::FreeScript;
			((parser_cpp*)m_xsmparser)->GetSourceDT = xfUriLoader::GetScriptDT;
			((parser_cpp*)m_xsmparser)->GetSourceVER = xfUriLoader::GetScriptVER;
			*/

			vlerror* perr = new vlerror();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			//perr->m_pEdit = GetControl(L"codecontent");

			//SendMessage(perr->m_pEdit->m_hWnd,SCI_SETMOUSEDWELLTIME,1000,0);
			//int len =SendMessage(perr->m_pEdit->m_hWnd,SCI_GETTEXTLENGTH,0,0);
			//SendMessage(perr->m_pEdit->m_hWnd,SCI_INDICATORCLEARRANGE,0,len);
			
			TCHAR buf[255];
			
			int startIndex = 0;

			vl::vlang3 vl;

			vl.GetSource = xfUriLoader::LoadScript;
			vl.FreeSource= xfUriLoader::FreeScript;
			vl.GetSourceDT = xfUriLoader::GetScriptDT;
			vl.GetSourceVER = xfUriLoader::GetScriptVER;

			vl.setError(perr);
			vl.parse(pXml);

			std::wstring str;
			//vl.traceTable(FetchData,pOutPut);
			vl.traceTable(FetchDataEx,&str);

			XSEdit * pControl = (XSEdit *)m_pParentWin->GetControl(_T("vdata"));
			pControl->SetContent((BSTR)str.c_str(),L".vl");
			
			/*
			int nLenText1 = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText1 + 1,nLenText1 + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)str.c_str());
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			*/

			delete perr;

			//_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",perr->m_pErrors->size(),perr->m_pWarns->size());
			_stprintf_s(buf,80,L"编译完成: ");
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}
		else if(FileExtIS(title,_T(".vl")) || (FileExtIS(title,_T(".vd"))))
		{
			if(GetKeyState(VK_CONTROL) & 0x8000)
			{
				m_bwpp= true;
				m_bwpp5= false;
			}
			else
			{
				m_bwpp5= true;
				m_bwpp = false;
			}
			if(m_bwpp)
			{
				wlerror* perr = new wlerror(); 
				xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
				
				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
				perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
				
				::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
				::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");
				
				::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

				wpp::wpp_runtime runtime;
				wpp::wpp_context context(&runtime);
				if(!m_wpp)	m_wpp = new Wpp;
				context.set_wpp_ptr((Wpp*)m_wpp);

				runtime.GetSource	 = xfUriLoader::LoadScript;
				runtime.FreeSource	 = xfUriLoader::FreeScript;
				runtime.GetSourceDT  = xfUriLoader::GetScriptDT;
				runtime.GetSourceVER = xfUriLoader::GetScriptVER;

				context.set_error(perr);
				context.parse(pXml);
				delete perr;

				TCHAR buf[255];
				_stprintf_s(buf,80,L"\r\n编译完成: ");
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				wchar_t* text = new wchar_t[nLenText + 1];
				GetWindowText(pOutPut->GetHWND(),text,nLenText + 1);
				XSEdit * pControl = (XSEdit *)m_pParentWin->GetControl(_T("vdata"));
				pControl->SetContent((BSTR)text,L".vl");
				delete text;
			}else
			{
				wlerror5* perr = new wlerror5(); 
				xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
				
				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
				perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
				
				::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
				::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");
				
				::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

				xlang::wpp_runtime runtime;
				xlang::wpp_context context(&runtime);
				if(!m_wpp5)	m_wpp5 = new xlang::Wpp5;
				context.set_wpp_ptr((xlang::Wpp5*)m_wpp5);

				runtime.GetSource	 = xfUriLoader::LoadScript;
				runtime.FreeSource	 = xfUriLoader::FreeScript;
				runtime.GetSourceDT  = xfUriLoader::GetScriptDT;
				runtime.GetSourceVER = xfUriLoader::GetScriptVER;

				context.set_error(perr);
				context.parse(pXml);
				delete perr;

				TCHAR buf[255];
				_stprintf_s(buf,80,L"\r\n编译完成: ");
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				wchar_t* text = new wchar_t[nLenText + 1];
				GetWindowText(pOutPut->GetHWND(),text,nLenText + 1);
				XSEdit * pControl = (XSEdit *)m_pParentWin->GetControl(_T("vdata"));
				pControl->SetContent((BSTR)text,L".vl");
				delete text;
			}
		}
		else
		{
			if(m_xcp)
			{
				if(m_xcp->m_pxvm) delete m_xcp->m_pxvm;
				delete m_xcp;
			}
		
			m_xcp = new xcomplier();
			m_xcp->m_pxvm = new xvmcore;

			myerror1* perr = new myerror1();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			
			m_xcp->m_error = perr;
			m_xcp->eval(pXml);

			TCHAR buf[80];
			TCHAR buf1[80];

			xfControl* PCode = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
			::SetWindowText(PCode->m_hWnd,L"");

			int nlevel = 0;

			buf1[0] = 0;
			int seqs[256];
			int index = 0;
			seqs[index] = 0;
			bool bdesc = false;
			for(int i=0;i<(int)m_xcp->m_pCodes->size();i++)
			{
				int nLenText = ::GetWindowTextLength(PCode->m_hWnd);
				SendMessage(PCode->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				if(::_tcsstr((*m_xcp->m_pCodes)[i],_T("blkstart"))== (*m_xcp->m_pCodes)[i])
				{
					_stprintf_s(buf,80,_T("%s{ %s\r\n"),buf1,(*m_xcp->m_pCodes)[i] + _tcslen(_T("blkstart")) );
					buf1[nlevel*3] = ' ';
					buf1[nlevel*3 + 1] = ' ';
					buf1[nlevel*3 + 2] = ' ';
					nlevel++;
					buf1[nlevel*3] = 0;
				}else if(::_tcsstr((*m_xcp->m_pCodes)[i],_T("blkend"))== (*m_xcp->m_pCodes)[i])
				{
					if(nlevel == index) index --;
					nlevel--;
					buf1[nlevel*3] = 0;
					_stprintf_s(buf,80,_T("%s}\r\n"),buf1);
				}else
				{
					if((*m_xcp->m_pCodes)[i][0]=='.')
					{
						index++;
						seqs[index] = 0;
						_stprintf_s(buf,80,_T("%s%s"),buf1,(*m_xcp->m_pCodes)[i]);
					}else
					{
						_stprintf_s(buf,80,_T("%s%d  %s"),buf1,seqs[index],(*m_xcp->m_pCodes)[i]);
						seqs[index]++;

					}
				}
				SendMessage(PCode->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(PCode->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}

			_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",perr->m_pErrors->size(),perr->m_pWarns->size());

			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}
		
		hCursor = ::SetCursor(hCursor);
		DestroyCursor(hCursor);
	}
	return 0;
}
xfControl * xCodeSheet::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int xCodeSheet::PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame)
{
	return 1;
}

int xCodeSheet::OnCodeFind()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("codecontent"))->GetInterface();
	pEdit->OnFindDlg();
	return 1;
}
int xCodeSheet::OnCodeReplace()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("codecontent"))->GetInterface();
	pEdit->OnReplaceDlg();
	return 1;
}

bool xCodeSheet::ReFillClassInfo(int startIndex,int endIndex)
{
	parser_cpp * xsmparser = (parser_cpp * )m_xsmparser;
	if(!xsmparser) return false;

	xfControl* xclist =  GetControl(_T("classes"));
	::SendMessage(xclist->GetHWND(),CB_RESETCONTENT,0,0);
	for(int i=startIndex;i<endIndex;i++)
	{
		sym_object * pobject = xsmparser->m_pEnv->GetCurMetaTable()->pTableSeq[__atom_sym_meta_typedef][i];
		if(pobject->at==__atom_sym_class)
		{
			sym_class * pclass = (sym_class *)pobject;
			if(!pclass->bole)
			{
				LPTSTR pname = xsmparser->m_pEnv->GetTokenName(pclass->name);
				SendMessage(xclist->GetHWND(),CB_ADDSTRING,0,(LPARAM)pname);
			}
		}
	}
	return true;
}
bool xCodeSheet::ReFillFctInfo(void * pobject,bool bfield)
{
	parser_cpp * xsmparser = (parser_cpp * )m_xsmparser;
	if(!xsmparser) return false;

	xfControl* xflist =  GetControl(_T("functions"));
	::SendMessage(xflist->GetHWND(),CB_RESETCONTENT,0,0);
	sym_class * pclass = (sym_class *)pobject;
	if(!pclass->bole)
	{
		if(bfield)
		{
			sym_field * pfield = pclass->pfield;
			while(pfield && pfield->piror)pfield = (sym_field *) pfield->piror;
			while(pfield)
			{
				LPTSTR pename = xsmparser->m_pEnv->GetTokenName(pfield->name);
				LPTSTR ptype  = xsmparser->m_pEnv->GetTypeName(pfield->ptype);
				TCHAR buf[255];
				_stprintf_s(buf,255,L"%s %s",ptype,pename);
				SendMessage(xflist->GetHWND(),CB_ADDSTRING,0,(LPARAM)buf);
				pfield = (sym_field *)pfield->next;
			}
		}
		sym_method * pmethod = pclass->pmethod;
		while(pmethod && pmethod->piror)pmethod = (sym_method *) pmethod->piror;
		while(pmethod)
		{
			LPTSTR pmname = xsmparser->m_pEnv->GetTokenName(pmethod->name);
			LPTSTR ptype  = xsmparser->m_pEnv->GetTypeName(pmethod->ptype);
			TCHAR buf[255];
			_stprintf_s(buf,255,L"%s %s(",ptype,pmname);
			sym_param * parg = pmethod->param;
			while(parg && parg->piror) parg =(sym_param *) parg->piror;
			bool bf = true;
			while(parg)
			{
				LPTSTR paname = xsmparser->m_pEnv->GetTokenName(parg->name);
				LPTSTR ptype  = xsmparser->m_pEnv->GetTypeName(parg->ptype);
				TCHAR buf1[255];
				if(bf)
				{
					_stprintf_s(buf1,255,L"%s %s",ptype,paname);
					bf = false;
				}else
					_stprintf_s(buf1,255,L", %s %s",ptype,paname);
				_tcscat_s(buf,255,buf1);

				parg = (sym_param *)parg->next;
			}
			_tcscat_s(buf,255,L")");
			SendMessage(xflist->GetHWND(),CB_ADDSTRING,0,(LPARAM)buf);
			pmethod = (sym_method *)pmethod->next;
		}
	}
	return true;
}

int xCodeSheet::Eval(int nLine,void * parser)
{
	if(GetKeyState(VK_CONTROL) & 0x8000)
	{
		m_bwpp= true;
		m_bwpp5= false;
	}
	else
	{
		m_bwpp5= true;
		m_bwpp = false;
	}
	if(!m_btpp && !m_bvl && !m_bwpp &&!m_bwpp5)
	{
		InitXssComplier();
		xsscomplier * xsscp = (xsscomplier *)m_xsscp;
		if(nLine ==-1)
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"compiler...\r\n");

			xsscp->eval(pXml);

			::SetWindowText(pOutPut->m_hWnd,L"compiler finished...\r\n");

			::SysFreeString(pXml);
		}else
		{
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			int caret ;
			int line;
			if(nLine==-99)
			{
				caret = SendMessage(pControl->GetHWND(),SCI_GETCURRENTPOS,0,0);
				line = SendMessage(pControl->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
			}else
				line = nLine;

			int lineStart = SendMessage(pControl->GetHWND(),SCI_POSITIONFROMLINE, line,0);

			if(lineStart > 0)
			{
				char* pContent = new char[lineStart];
				BSTR bstr;
				SendMessage(pControl->GetHWND(),SCI_GETTEXT,lineStart - 1,(LPARAM)pContent);

				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]='\0';
				::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), bstr, cchWide + 1);


				xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
				::SetWindowText(pOutPut->m_hWnd,L"compiler...\r\n");

				xsscp->eval(bstr);

				::SetWindowText(pOutPut->m_hWnd,L"compiler finished...\r\n");

				::SysFreeString(bstr);
			}
		}
	}else if(m_btpp)
	{
		if(nLine ==-1)
		{
			InitXssComplier();
			parser_cpp * xsmparser = (parser_cpp * )m_xsmparser;

			int startIndex = 0;
			//if(xsmparser->m_pEnv->m_units.size()>0)
			//	startIndex = xsmparser->m_pEnv->GetCurMetaTable()->pTableSeq[__atom_sym_meta_typedef].size();
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));

			lexer* cp = new lexer;
			xsmparser->cp = cp;
			cp->setsource(pXml,null);
			cp->insymbol();
			bool ok = false;
			if(xsmparser->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
			xsmparser->cp = null;
			cp->source = null;
			startIndex = cp->startIndex;
			delete cp;

			TCHAR buf[255];
			_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",xsmparser->m_perror->m_pErrors->size(),xsmparser->m_perror->m_pWarns->size());
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			::SysFreeString(pXml);
			int endIndex = xsmparser->m_pEnv->GetCurMetaTable()->pTableSeq[__atom_sym_meta_typedef].size();
			this->ReFillClassInfo(startIndex,endIndex);
		}else
		{
			parser_cpp * xsmparser = null;
			if(parser)
				xsmparser = (parser_cpp *)parser;
			else 
				xsmparser = new parser_cpp;
			InitXssComplier(xsmparser);

			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			int caret ;
			int line;
			if(nLine==-99)
			{
				caret = SendMessage(pControl->GetHWND(),SCI_GETCURRENTPOS,0,0);
				line = SendMessage(pControl->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
			}else
				line = nLine;

			int lineStart = SendMessage(pControl->GetHWND(),SCI_POSITIONFROMLINE, line,0);

			if(lineStart > 0)
			{
				char* pContent = new char[lineStart];
				BSTR bstr;
				SendMessage(pControl->GetHWND(),SCI_GETTEXT,lineStart - 1,(LPARAM)pContent);

				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]='\0';
				::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), bstr, cchWide + 1);


				xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));

				lexer* cp = new lexer;
				xsmparser->cp = cp;
				cp->setsource(bstr,null);
				cp->insymbol();
				bool ok = false;
				if(xsmparser->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
				xsmparser->cp = null;
				cp->source = null;
				delete cp;

				TCHAR buf[255];
				_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",xsmparser->m_perror->m_pErrors->size(),xsmparser->m_perror->m_pWarns->size());
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				::SysFreeString(bstr);
				if(!parser) delete xsmparser;
			}
		}
	}else if(m_bwpp)
	{
		if(nLine ==-1)
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			wlerror* perr = new wlerror();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			
			perr->m_pOutPut = pOutPut;
			perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
			perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
			
			::SendMessage(perr->m_pDataOutPut->GetHWND(),EM_SETLIMITTEXT,-1,0);

			::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
			::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");
			
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

			/*
			wpp::wpp_context& context = *(wpp::wpp_context *)parser ;

			context.get_runtime()->GetSource	 = xfUriLoader::LoadScript;
			context.get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
			context.get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
			context.get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

			context.set_error(perr);
			context.parse(pXml);
			delete perr;
			::SysFreeString(pXml); */
			
			wpp::wpp_runtime runtime;
			wpp::wpp_context context(&runtime);
			if(!m_wpp)	m_wpp = new Wpp;
			context.set_wpp_ptr((Wpp*)m_wpp);

			runtime.GetSource	 = xfUriLoader::LoadScript;
			runtime.FreeSource	 = xfUriLoader::FreeScript;
			runtime.GetSourceDT  = xfUriLoader::GetScriptDT;
			runtime.GetSourceVER = xfUriLoader::GetScriptVER;

			context.set_error(perr);
			context.parse(pXml);
			delete perr;
			::SysFreeString(pXml);

			TCHAR buf[255];
			_stprintf_s(buf,80,L"编译完成: ");
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}else
		{
			wpp::wpp_context& context = *(wpp::wpp_context *)parser ;
			context.get_runtime()->GetSource	 = xfUriLoader::LoadScript;
			context.get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
			context.get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
			context.get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			int caret ;
			int line;
			if(nLine==-99)
			{
				caret = SendMessage(pControl->GetHWND(),SCI_GETCURRENTPOS,0,0);
				line = SendMessage(pControl->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
			}else
				line = nLine;

			int lineStart = SendMessage(pControl->GetHWND(),SCI_POSITIONFROMLINE, line,0);

			if(lineStart > 0)
			{
				char* pContent = new char[lineStart + 1];
				BSTR bstr;
				SendMessage(pControl->GetHWND(),SCI_GETTEXT,lineStart,(LPARAM)pContent);

				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]='\0';
				::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), bstr, cchWide + 1);


				wlerror* perr = new wlerror();
				xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
				
				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
				perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
				
				::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
				::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");

				::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

				context.set_error(perr);
				context.parse(bstr);
				delete perr;
				::SysFreeString(bstr);

				TCHAR buf[255];
				_stprintf_s(buf,80,L"编译完成: ");
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}
		}
	}
	else if(m_bwpp5)
	{
		if(nLine ==-1)
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));

			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			wlerror5* perr = new wlerror5();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));

			perr->m_pOutPut = pOutPut;
			perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
			perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));

			::SendMessage(perr->m_pDataOutPut->GetHWND(),EM_SETLIMITTEXT,-1,0);

			::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
			::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");

			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

			/*
			wpp::wpp_context& context = *(wpp::wpp_context *)parser ;

			context.get_runtime()->GetSource	 = xfUriLoader::LoadScript;
			context.get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
			context.get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
			context.get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

			context.set_error(perr);
			context.parse(pXml);
			delete perr;
			::SysFreeString(pXml); */

			xlang::wpp_runtime runtime;
			xlang::wpp_context context(&runtime);
			if(!m_wpp5)	m_wpp5 = new xlang::Wpp5;
			context.set_wpp_ptr((xlang::Wpp5*)m_wpp);

			runtime.GetSource	 = xfUriLoader::LoadScript;
			runtime.FreeSource	 = xfUriLoader::FreeScript;
			runtime.GetSourceDT  = xfUriLoader::GetScriptDT;
			runtime.GetSourceVER = xfUriLoader::GetScriptVER;

			context.set_error(perr);
			context.parse(pXml);
			delete perr;
			::SysFreeString(pXml);

			TCHAR buf[255];
			_stprintf_s(buf,80,L"编译完成: ");
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}else
		{
			xlang::wpp_context& context = *(xlang::wpp_context *)parser ;
			context.get_runtime()->GetSource	 = xfUriLoader::LoadScript;
			context.get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
			context.get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
			context.get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			int caret ;
			int line;
			if(nLine==-99)
			{
				caret = SendMessage(pControl->GetHWND(),SCI_GETCURRENTPOS,0,0);
				line = SendMessage(pControl->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
			}else
				line = nLine;

			int lineStart = SendMessage(pControl->GetHWND(),SCI_POSITIONFROMLINE, line,0);

			if(lineStart > 0)
			{
				char* pContent = new char[lineStart + 1];
				BSTR bstr;
				SendMessage(pControl->GetHWND(),SCI_GETTEXT,lineStart,(LPARAM)pContent);

				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]='\0';
				::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), bstr, cchWide + 1);


				wlerror5* perr = new wlerror5();
				xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));

				perr->m_pOutPut = pOutPut;
				perr->m_pDataOutPut  = pControl->GetWin()->m_pParentWin->GetControl(_T("data"));
				perr->m_pPCodeOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));

				::SetWindowText(perr->m_pDataOutPut->m_hWnd,L"");
				::SetWindowText(perr->m_pPCodeOutPut->m_hWnd,L"");

				::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");

				context.set_error(perr);
				context.parse(bstr);
				delete perr;
				::SysFreeString(bstr);

				TCHAR buf[255];
				_stprintf_s(buf,80,L"编译完成: ");
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}
		}
	}
	else if(m_bvl)
	{
		if(nLine ==-1)
		{
			/*
			InitXssComplier();
			vl::vlang3 * pvlang = (vl::vlang3 * )m_pvlang;

			int startIndex = 0;
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			myerrorVl* perr = new myerrorVl();
			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			perr->m_pEdit = GetControl(L"codecontent");

			pvlang->setError(perr);
			pvlang->parse(pXml);
			delete perr;
			pvlang->setError(NULL);

			std::wstring str;
			pvlang->traceTable(FetchDataEx1,&str);

			XSEdit * pe = (XSEdit *)m_pParentWin->GetControl(_T("vdata"));
			pe->SetContent((BSTR)str.c_str(),L".vl");

			TCHAR buf[255];
			_stprintf_s(buf,80,L"编译完成");
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);*/
		}else
		{
			/*
			vl::vlang3 * xsmparser = null;
			if(parser)
				xsmparser = (vl::vlang3 *)parser;
			else 
				xsmparser = new vl::vlang3;
			xsmparser->GetSource = xfUriLoader::LoadScript;
			xsmparser->FreeSource= xfUriLoader::FreeScript;
			xsmparser->GetSourceDT = xfUriLoader::GetScriptDT;
			xsmparser->GetSourceVER = xfUriLoader::GetScriptVER;

			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			int caret ;
			int line;
			if(nLine==-99)
			{
				caret = SendMessage(pControl->GetHWND(),SCI_GETCURRENTPOS,0,0);
				line = SendMessage(pControl->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
			}else
				line = nLine;

			int lineStart = SendMessage(pControl->GetHWND(),SCI_POSITIONFROMLINE, line,0);

			if(lineStart > 0)
			{
				char* pContent = new char[lineStart];
				BSTR bstr;
				SendMessage(pControl->GetHWND(),SCI_GETTEXT,lineStart - 1,(LPARAM)pContent);

				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]='\0';
				::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), bstr, cchWide + 1);


				xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));

				myerrorVl* perr = new myerrorVl();
				::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
				perr->m_pOutPut = pOutPut;
				perr->m_pEdit = GetControl(L"codecontent");

				xsmparser->setError(perr);
				xsmparser->parse(bstr);
				delete perr;
				xsmparser->setError(NULL);

				TCHAR buf[255];
				_stprintf_s(buf,80,L"编译完成");
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				::SysFreeString(bstr);
				if(!parser) delete xsmparser;
			}*/
		}
	}
	return 1;
}

int xCodeSheet::Eval1(int nLine, void * parser)
{
	if(!m_btpp)
	{
		InitXssComplier();
		xsscomplier * xsscp = (xsscomplier *)m_xsscp;
		if(nLine ==-1)
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"compiler...\r\n");

			xsscp->eval(pXml);

			::SetWindowText(pOutPut->m_hWnd,L"compiler finished...\r\n");

			::SysFreeString(pXml);
		}else
		{
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			int caret ;
			int line;
			if(nLine==-99)
			{
				caret = SendMessage(pControl->GetHWND(),SCI_GETCURRENTPOS,0,0);
				line = SendMessage(pControl->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
			}else
				line = nLine;

			int lineStart = SendMessage(pControl->GetHWND(),SCI_POSITIONFROMLINE, line,0);

			if(lineStart > 0)
			{
				char* pContent = new char[lineStart];
				BSTR bstr;
				SendMessage(pControl->GetHWND(),SCI_GETTEXT,lineStart - 1,(LPARAM)pContent);

				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]='\0';
				::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), bstr, cchWide + 1);


				xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
				::SetWindowText(pOutPut->m_hWnd,L"compiler...\r\n");

				xsscp->eval(bstr);

				::SetWindowText(pOutPut->m_hWnd,L"compiler finished...\r\n");

				::SysFreeString(bstr);
			}
		}
	}else
	{
		if(nLine ==-1)
		{
			InitXssComplier();
			parser_cpp * xsmparser = (parser_cpp * )m_xsmparser;

			int startIndex = 0;
			//if(xsmparser->m_pEnv->m_units.size()>0)
			//	startIndex = xsmparser->m_pEnv->GetCurMetaTable()->pTableSeq[__atom_sym_meta_typedef].size();
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));

			lexer* cp = new lexer;
			xsmparser->cp = cp;
			cp->setsource(pXml,null);
			cp->insymbol();
			bool ok = false;
			if(xsmparser->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
			xsmparser->cp = null;
			cp->source = null;
			startIndex = cp->startIndex;
			delete cp;

			TCHAR buf[255];
			_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",xsmparser->m_perror->m_pErrors->size(),xsmparser->m_perror->m_pWarns->size());
			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			::SysFreeString(pXml);
			int endIndex = xsmparser->m_pEnv->GetCurMetaTable()->pTableSeq[__atom_sym_meta_typedef].size();
			this->ReFillClassInfo(startIndex,endIndex);
		}else
		{
			parser_cpp * xsmparser = null;
			if(parser)
				xsmparser = (parser_cpp *)parser;
			else 
				xsmparser = new parser_cpp;
			InitXssComplier(xsmparser);

			XSEdit * pControl = (XSEdit *)GetControl(_T("codecontent"));
			int caret ;
			int line;
			if(nLine==-99)
			{
				caret = SendMessage(pControl->GetHWND(),SCI_GETCURRENTPOS,0,0);
				line = SendMessage(pControl->GetHWND(),SCI_LINEFROMPOSITION, caret,0);
			}else
				line = nLine;

			int lineStart = SendMessage(pControl->GetHWND(),SCI_POSITIONFROMLINE, line,0);

			if(lineStart > 0)
			{
				char* pContent = new char[lineStart];
				BSTR bstr;
				SendMessage(pControl->GetHWND(),SCI_GETTEXT,lineStart - 1,(LPARAM)pContent);

				int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), NULL, 0);
				bstr = SysAllocStringLen(NULL,cchWide + 1);
				bstr[cchWide]='\0';
				::MultiByteToWideChar(CP_UTF8, 0, pContent, (int)strlen(pContent), bstr, cchWide + 1);


				xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));

				lexer* cp = new lexer;
				xsmparser->cp = cp;
				cp->setsource(bstr,null);
				cp->insymbol();
				bool ok = false;
				if(xsmparser->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
				xsmparser->cp = null;
				cp->source = null;
				delete cp;

				TCHAR buf[255];
				_stprintf_s(buf,80,L"编译完成: - %i 个错误  %i 个警告",xsmparser->m_perror->m_pErrors->size(),xsmparser->m_perror->m_pWarns->size());
				int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
				SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

				::SysFreeString(bstr);
				if(!parser) delete xsmparser;
			}
		}
	}
	return 1;
}
int xCodeSheet::InitXssComplier()
{
	if(!m_btpp)
	{
		xsscomplier * xsscp = (xsscomplier *)m_xsscp;;
		if(!xsscp)
		{
			xsscp = new xsscomplier();
			xsscp->m_pxvm = new xvmcore;

			myerror1* perr = new myerror1();

			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
			perr->m_pOutPut = pOutPut;

			xsscp->m_error = perr;
			m_xsscp = xsscp;
		}
	}else
	{
		if(m_xsmparser) delete (parser_cpp * )m_xsmparser;
		m_xsmparser = new parser_cpp();
		((parser_cpp*)m_xsmparser)->GetSource = xfUriLoader::LoadScript;
		((parser_cpp*)m_xsmparser)->FreeSource= xfUriLoader::FreeScript;
		((parser_cpp*)m_xsmparser)->GetSourceDT = xfUriLoader::GetScriptDT;
		((parser_cpp*)m_xsmparser)->GetSourceVER = xfUriLoader::GetScriptVER;

		InitXssComplier(m_xsmparser);
	}
	return 1;
}
int xCodeSheet::InitXssComplier(void *parser)
{
	parser_cpp * xsmparser = (parser_cpp * )parser;
	((parser_cpp*)xsmparser)->GetSource = xfUriLoader::LoadScript;
	((parser_cpp*)xsmparser)->FreeSource= xfUriLoader::FreeScript;
	((parser_cpp*)xsmparser)->GetSourceDT = xfUriLoader::GetScriptDT;
	((parser_cpp*)xsmparser)->GetSourceVER = xfUriLoader::GetScriptVER;

	myerror2* perr = new myerror2();
	xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
	::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
	perr->m_pOutPut = pOutPut;
	perr->m_pEdit = GetControl(L"codecontent");

	xsmparser->m_perror = perr;
	int len =SendMessage(perr->m_pEdit->m_hWnd,SCI_GETTEXTLENGTH,0,0);
	SendMessage(perr->m_pEdit->m_hWnd,SCI_INDICATORCLEARRANGE,0,len);
	
	return 1;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              