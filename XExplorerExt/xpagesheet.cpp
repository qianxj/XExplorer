#include "StdAfx.h"
#include "xPageSheet.h"
#include "xcControl.h"

#include "xscomplier.h"
#include "astnode.h"

using namespace xsharp;

#include "parse.h"
#include "xssparse.h"
using namespace  xcode;

#include "scintilla.h"
#include "SciLexer.h"

#include "xsm.h"
#include "parser/parser.hxx"
#include "parser/pcodes.hxx"
#include "tpp/parser_tpp.hxx"
#include "gen/gen_tpp.hxx"
using namespace xsm;

#include "vl.h"
#include "parser/vlspacemgr.hpp"
#include "parser/vlarraymgr.hpp"
#include "parser/vlparser.hpp"
#include "parser/vlsym.hpp"
#include "parser/vlang.hpp"
#include "parser/vlbnf.hpp"
#include "parser/vlunit.hpp"
#include "parser/vmlang.hpp"

#include "wpp.h"
#include "wpp5.h"
//using namespace Scintilla;

bool FetchDataEx1(const tchar * p,void* param)
{
	if(p)((std::wstring *)param)->append(p);
	return true;
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

		int nLenText = ::GetWindowTextLength(m_pDataOutPut->m_hWnd);
		SendMessage(m_pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
		SendMessage(m_pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)pMsgText);
		SendMessage(m_pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

		return 1;
	}
};

class myerrorTpp : public errobj
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

class myerrorVl : public vl::vlErr
{
public:
	xfControl* m_pOutPut;
	xfControl* m_pEdit;
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

xPageSheet::xPageSheet(void):m_pCodeAgent(NULL),m_pPageAgent(NULL),m_nSheet(0),m_xsscp(NULL),m_pvlang(NULL), beval(false),m_btpp(false),m_xsmparser(NULL),m_bvl(false),
 m_bwpp(false),m_wpp(0), m_bwpp5(false),m_wpp5(0)
{
	this->AttachListon(WM_SETFOCUS,(FNEvent)&xPageSheet::EvtSetFocus);
	this->AttachListon(WM_NOTIFY,(FNEvent)&xPageSheet::EvtNotify);
	this->AttachListon(L"functions",CBN_SELCHANGE,(FNEvent)&xPageSheet::EvtFctSelection);
	this->AttachListon(L"classes",CBN_SELCHANGE,(FNEvent)&xPageSheet::EvtClsSelection);
}

bool xPageSheet::EvtClsSelection(TEvent * pEvent, LPARAM lParam)
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
bool xPageSheet::EvtFctSelection(TEvent * pEvent, LPARAM lParam)
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

		XSEdit * pedit = (XSEdit *)GetControl(_T("xcodecontent"));
		SendMessage(pedit->GetHWND(),SCI_GOTOLINE,sRow - 1,0);
		if(!SendMessage(pedit->GetHWND(),SCI_GETFOLDEXPANDED,sRow,0))
			SendMessage(pedit->GetHWND(),SCI_TOGGLEFOLD,sRow,0);
		SetFocus(pedit->GetHWND());
	}
	return true;
}

xPageSheet::~xPageSheet(void)
{
	std::map<std::wstring,xbObjectMgrEvent *>::const_iterator tr;
	for(tr=m_mapObjectEvents.begin();tr!=m_mapObjectEvents.end();tr++)
	{
		if((*tr).second)delete (*tr).second;
	}
	if(m_xsscp)delete ((xsscomplier *)m_xsscp);
	if(m_xsmparser)delete ((parser_cpp *)m_xsmparser);
	if(m_wpp) delete (Wpp *)m_wpp;
}

bool xPageSheet::EvtNotify(TEvent* pEvent,LPARAM lParam)
{
	if(pEvent->notify.pnmh->code==SCN_UPDATEUI)
	{
		if(pEvent->notify.pnmh->hwndFrom == GetControl(L"xcodecontent")->GetHWND())
			return EvtCodeUpdateUI(pEvent,lParam);
		else if(pEvent->notify.pnmh->hwndFrom == GetControl(L"xpagecontent")->GetHWND())
			return EvtPageUpdateUI(pEvent,lParam);
	}

	return true;
}

bool xPageSheet::EvtCodeUpdateUI(TEvent* pEvent,LPARAM lParam)
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

bool xPageSheet::EvtPageUpdateUI(TEvent* pEvent,LPARAM lParam)
{
	int caret = SendMessage(pEvent->notify.pnmh->hwndFrom,SCI_GETCURRENTPOS,0,0);
	int line = SendMessage(pEvent->notify.pnmh->hwndFrom,SCI_LINEFROMPOSITION, caret,0);
	int lineStart = SendMessage(pEvent->notify.pnmh->hwndFrom,SCI_POSITIONFROMLINE, line,0);

	int len = SendMessage(pEvent->notify.pnmh->hwndFrom,SCI_LINELENGTH,line,0);
	
	if(len > 0)
	{
		char* text = new char[len +1];
		char tag[64];

		SendMessage(pEvent->notify.pnmh->hwndFrom,SCI_GETLINE,line,(LPARAM)text);
		text[len] = 0;
		int nPos = len -1;
		if(caret - lineStart < nPos)nPos = caret - lineStart;
		int sPos = 0;
		while(text[nPos]!='<' && nPos>=0) nPos--;
		if(nPos>=0)
		{
			nPos++;
			while(nPos < len && !(text[nPos]=='\t' || text[nPos]=='\r'|| text[nPos]=='\n' || text[nPos]==' ' || text[nPos]=='/' ||
					text[nPos]=='>'))
			{
				tag[sPos++] = text[nPos++];
			}
		}
		tag[sPos] = 0;
		if(tag[0] && tag[0]!='?')
		{
			xfNode* pAgentNode = this->m_pXPage->GetAgentNode();
			if(pAgentNode)
			{
				xfControl * pControl = pAgentNode->GetXPage()->m_pWin->GetControl(_T("item"),pAgentNode);
				if(pControl)SetWindowTextA(pControl->m_hWnd,tag);


				int _convert = MultiByteToWideChar(CP_UTF8, 0, tag, strlen(tag), NULL, 0);
				TCHAR * psz = new TCHAR[_convert + 1];
				int nRet = MultiByteToWideChar(CP_UTF8, 0,tag, strlen(tag), psz, _convert);
				psz[_convert]='\0';

				IXMLDOMElement *pElement;
				IXMLDOMElement *pElementEx;
				IXMLDOMElement* pDocElement;
				IXMLDOMNode *pNode;

				xcPropSheetCtrl * pPropSheet = (xcPropSheetCtrl*)pAgentNode->GetXPage()->m_pWin->GetControl(_T("propsheet"),pAgentNode);
				if(!pPropSheet) return true;
				pPropSheet->m_pXml->m_pXmlDoc->get_documentElement(&pDocElement);
				if(!pDocElement)
					pDocElement = pPropSheet->m_pxfNode->m_pElement;

				pDocElement->selectSingleNode(L"//row[@category='items' and @label='events:']",&pNode);
				
				if(pNode)
				{
					IXMLDOMNode *pNodeEx;
					pDocElement->selectSingleNode(L"//rows",&pNodeEx);
					if(pNodeEx)
					{
						pNodeEx->removeChild(pNode,NULL);
						pNodeEx->Release();
					}
					pNode->Release();

				}
					
				if(xfApp::GetApp()->m_ObjectMgr.IsControl(NULL,psz))
				{
					std::map<std::wstring,xbObjectMgrEvent *>::const_iterator tr=m_mapObjectEvents.find(psz);
					if(tr==m_mapObjectEvents.end())
					{
						TCHAR path[MAX_PATH];
						::GetModuleFileName(NULL,path,MAX_PATH);
						::PathRemoveFileSpec(path);
						TCHAR file[MAX_PATH];
						::_stprintf_s(file,MAX_PATH,_T("%s\\lib\\events\\xframe\\%s.xml"),path,psz);
						if(PathFileExists(file))
						{
							xbXml xml;
							if(xml.Load(file))
							{
								IXMLDOMNodeList * pEventList;
								long nLenEvents;
								IXMLDOMNode * pNode;
								IXMLDOMElement * pElement;

								xml.m_pXmlDoc->selectNodes(L"//events/event",&pEventList);
								pEventList->get_length(&nLenEvents);

								xbObjectMgrEvent * pEvent = NULL;
								if(nLenEvents>0)  pEvent = new xbObjectMgrEvent ;
								for(int m=0;m<nLenEvents;m++)
								{
									pEventList->get_item(m,&pNode);
									pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);

									LPTSTR pAttr =xbXml::GetStringAttribe(pElement,L"name");
									pEvent->m_eventName.push_back(pAttr);
									delete pAttr;

									pAttr =xbXml::GetStringAttribe(pElement,L"evtid");
									pEvent->m_eventID.push_back(pAttr);
									delete pAttr;

									pNode->Release();
									pElement->Release();
								}
								if(pEvent)
								{
									std::pair<std::wstring,xbObjectMgrEvent *> pr;
									pr.first = psz;
									pr.second = pEvent;
									this->m_mapObjectEvents.insert(pr);
								}
							}
						}
					}
				
					tr=m_mapObjectEvents.find(psz);
					if(tr!=m_mapObjectEvents.end())
					{
						pPropSheet->m_pXml->m_pXmlDoc->createElement(L"row",&pElement);

						pElement->setAttribute(L"category",(_variant_t)L"items");
						pElement->setAttribute(L"level",(_variant_t)L"2");
						pElement->setAttribute(L"label",(_variant_t)L"events:");
						pElement->setAttribute(L"type",(_variant_t)L"rcolumn");
						pElement->setAttribute(L"expand",(_variant_t)L"true");

						for(int n=0;n<(int)(*tr).second->m_eventName.size();n++)
						{
							pPropSheet->m_pXml->m_pXmlDoc->createElement(L"row",&pElementEx);
							pElementEx->setAttribute(L"category",(_variant_t)L"item");
							pElementEx->setAttribute(L"level",(_variant_t)L"3");
							pElementEx->setAttribute(L"label",(_variant_t)(*tr).second->m_eventName[n].c_str());
							pElementEx->setAttribute(L"type",(_variant_t)L"rcolumn");
							pElementEx->setAttribute(L"expand",(_variant_t)L"false");
							pElement->appendChild(pElementEx,NULL);
							pElementEx->Release();
						}
						pDocElement->selectSingleNode(L"//rows",&pNode);
						if(pNode)
						{
							pNode->appendChild(pElement,NULL);
							pNode->Release();
						}

						pElement->Release();
					}
				}
				delete psz;
				::InvalidateRect(pPropSheet->m_hWnd,NULL,false);
			}
		}
		delete text;
	}
	return true;
}

bool OnCodeEditChar(void * pOwner,Hxsoft::XFrame::XSEdit *pEdit,char ch)
{
	return false;

	if((ch==';' || ch=='}' || ch=='\r') && ((xPageSheet*)pOwner)->m_btpp)
	{
		((xPageSheet*)pOwner)->Eval(-1);
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
				if(!((xPageSheet*)pOwner)->m_btpp && !((xPageSheet*)pOwner)->m_bvl &&
					!((xPageSheet*)pOwner)->m_bwpp && !((xPageSheet*)pOwner)->m_bwpp5)
				{
					xsscomplier * xsscp = (xsscomplier *)((xPageSheet*)pOwner)->m_xsscp;
					if(!xsscp)
					{
						((xPageSheet*)pOwner)->InitXssComplier();
						((xPageSheet*)pOwner)->Eval(-99);
						xsscp = (xsscomplier *)((xPageSheet*)pOwner)->m_xsscp;
					}else
						((xPageSheet*)pOwner)->Eval(-99);

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
				}else if(((xPageSheet*)pOwner)->m_btpp)
				{
					parser_cpp * xsmparser = new parser_cpp();
					((xPageSheet*)pOwner)->Eval(-99,xsmparser);

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
				}else if(((xPageSheet*)pOwner)->m_bvl)
				{
					vl::vlang3 * vp = new vl::vlang3();
					((xPageSheet*)pOwner)->Eval(-99,vp);

					BSTR bstr;
					int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
					bstr = SysAllocStringLen(NULL,cchWide + 1);
					bstr[cchWide]=0;
					::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);

					vl::vmlang3* vm = (vl::vmlang3*)vp->getLang(L"tpp");
					LPTSTR pItems = vm->getVarAutoString(bstr);
					if(pItems)
					{
						int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
						char *pszMulti = new char[cchMulti + 1];
						::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
						pszMulti[cchMulti] = 0;
						SendMessage(pEdit->GetHWND(),SCI_AUTOCSHOW,0,(sptr_t)pszMulti);
						delete pszMulti;
						vm->freeAutoString(pItems);
					}
					::SysFreeString(bstr);
					delete vp;
				}else if(((xPageSheet*)pOwner)->m_bwpp)
				{
					wpp::wpp_runtime runtime;
					wpp::wpp_context context(&runtime);
					if(!((xPageSheet*)pOwner)->m_wpp)
						((xPageSheet*)pOwner)->m_wpp = new Wpp;
					context.set_wpp_ptr((Wpp *)((xPageSheet*)pOwner)->m_wpp);

					((xPageSheet*)pOwner)->Eval(-99,&context);

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
				}else if(((xPageSheet*)pOwner)->m_bwpp5)
				{
					xlang::wpp_runtime runtime;
					xlang::wpp_context context(&runtime);
					if(!((xPageSheet*)pOwner)->m_wpp5)
						((xPageSheet*)pOwner)->m_wpp5 = new xlang::Wpp5;
					context.set_wpp_ptr((xlang::Wpp5 *)((xPageSheet*)pOwner)->m_wpp5);

					((xPageSheet*)pOwner)->Eval(-99,&context);

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
				if(!((xPageSheet*)pOwner)->m_btpp  && !((xPageSheet*)pOwner)->m_bvl &&
					 !((xPageSheet*)pOwner)->m_bwpp && !((xPageSheet*)pOwner)->m_bwpp5)
				{
					xsscomplier * xsscp = (xsscomplier *)((xPageSheet*)pOwner)->m_xsscp;
					if(!xsscp)
					{
						((xPageSheet*)pOwner)->InitXssComplier();
						((xPageSheet*)pOwner)->Eval(-99);
						xsscp = (xsscomplier *)((xPageSheet*)pOwner)->m_xsscp;
					}else
						((xPageSheet*)pOwner)->Eval(-99);

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
				}else if(((xPageSheet*)pOwner)->m_btpp)
				{
					parser_cpp * xsmparser = new parser_cpp();
					((xPageSheet*)pOwner)->Eval(-99,xsmparser);

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
				}else if(((xPageSheet*)pOwner)->m_bvl)
				{
					vl::vlang3 * vp = new vl::vlang3();
					((xPageSheet*)pOwner)->Eval(-99,vp);

					BSTR bstr;
					int cchWide = ::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), NULL, 0);
					bstr = SysAllocStringLen(NULL,cchWide + 1);
					bstr[cchWide]=0;
					::MultiByteToWideChar(CP_UTF8, 0, var, (int)strlen(var), bstr, cchWide + 1);

					vl::vmlang3* vm = (vl::vmlang3*)vp->getLang(L"tpp");
					LPTSTR pItems = vm->getFctTipString(bstr);
					if(pItems)
					{
						int cchMulti = ::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), NULL, 0, NULL, NULL);
						char *pszMulti = new char[cchMulti + 1];
						::WideCharToMultiByte(CP_UTF8, 0, pItems, _tcslen(pItems), pszMulti, cchMulti + 1, NULL, NULL);
						pszMulti[cchMulti] = 0;
						SendMessage(pEdit->GetHWND(),SCI_CALLTIPSHOW,caret,(sptr_t)pszMulti);
						delete pszMulti;
						vm->freeAutoString(pItems);
					}
					::SysFreeString(bstr);
					delete vp;
				}else if(((xPageSheet*)pOwner)->m_bwpp)
				{
					wpp::wpp_runtime runtime;
					wpp::wpp_context context(&runtime);
					if(!((xPageSheet*)pOwner)->m_wpp)
						((xPageSheet*)pOwner)->m_wpp = new Wpp;
					context.set_wpp_ptr((Wpp*)((xPageSheet*)pOwner)->m_wpp);
					((xPageSheet*)pOwner)->Eval(-99,&context);

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
				}else if(((xPageSheet*)pOwner)->m_bwpp5)
				{
					xlang::wpp_runtime runtime;
					xlang::wpp_context context(&runtime);
					if(!((xPageSheet*)pOwner)->m_wpp5)
						((xPageSheet*)pOwner)->m_wpp5 = new xlang::Wpp5;
					context.set_wpp_ptr((xlang::Wpp5*)((xPageSheet*)pOwner)->m_wpp5);
					((xPageSheet*)pOwner)->Eval(-99,&context);

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
	}

	return false;
}

int xPageSheet::OnOpen()
{
	SetAgent();
	xfControl * pEdit = GetControl(L"xcodecontent");
	if(pEdit)
	{
		((XSEdit *)pEdit)->AddExtent(this,(XSEdit *)pEdit,OnCodeEditChar);
		SendMessage(pEdit->GetHWND(),SCI_AUTOCSETMAXHEIGHT,14,0);

	}
	return 1;
	
}

bool xPageSheet::EvtSetFocus(TEvent * pEvent, LPARAM lParam)
{
	//if((HWND)pEvent->wParam != m_hWnd);
	SetAgent();
	return true;
}

int xPageSheet::SetAgent()
{
	xfNode * pAgentNode = this->m_pXPage->GetAgentNode();
	if(!pAgentNode) return 1;

	if(m_nSheet==2)
	{
		if(m_pCodeAgent)
		{
			m_pXPage->SetAgentNodeContent(pAgentNode,m_pCodeAgent,NULL);
			return 1;
		}

		IXMLDOMNode * pNode;
		this->m_pXPage->m_pxfNode->m_pElement->selectSingleNode(L"agent/agentarea[@for='code']/*[1]",&pNode);
		if(pNode)
		{
			BSTR bstr;
			pNode->get_xml(&bstr);

			if(pAgentNode)
			{
				m_pCodeAgent = m_pXPage->SetAgentNodeContent(pAgentNode,bstr,NULL);
			}
			::SysFreeString(bstr);
			pNode->Release();
		}
	}else
	{
		if(m_pPageAgent)
		{
			m_pXPage->SetAgentNodeContent(pAgentNode,m_pPageAgent,NULL);
			return 1;
		}

		IXMLDOMNode * pNode;
		this->m_pXPage->m_pxfNode->m_pElement->selectSingleNode(L"agent/agentarea[@for='xpage']/*[1]",&pNode);
		if(pNode)
		{
			BSTR bstr;
			pNode->get_xml(&bstr);

			if(pAgentNode)
			{
				m_pPageAgent = m_pXPage->SetAgentNodeContent(pAgentNode,bstr,NULL);
			}
			::SysFreeString(bstr);
			pNode->Release();
		}
	}
	return 1;
}

class myerror9 : public xvmerror
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

bool xPageSheet::ReFillClassInfo(int startIndex,int endIndex)
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
bool xPageSheet::ReFillFctInfo(void * pobject,bool bfield)
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

int xPageSheet::Eval(int nLine, void * parser)
{
	xbXml xml;
	BSTR pXml = NULL;
	XSEdit * pControl = (XSEdit *)GetControl(_T("xpagecontent"));
	pControl->GetContent(&pXml);
	if(!pXml) return 1;
	if(xml.LoadXml(pXml))
	{
		VARIANT var;
		IXMLDOMElement * pElement;
		xml.GetXmlDoc()->get_documentElement(&pElement);
		if(pElement)
		{
			pElement->getAttribute(L"tpp_guid",&var);
			if(var.bstrVal && var.bstrVal[0])
			{
				m_btpp = true;
				::VariantClear(&var);
			}else
				m_btpp = false;
			pElement->getAttribute(L"classurl",&var);
			if(var.bstrVal && var.bstrVal[0])
			{
				m_bvl = true;
				::VariantClear(&var);
			}else
				m_bvl = false;
			pElement->getAttribute(L"class_url",&var);
			if(var.bstrVal && var.bstrVal[0])
			{
				m_bwpp= true;
				::VariantClear(&var);
			}else
				m_bwpp = false;
			pElement->getAttribute(L"class_url5",&var);
			if(var.bstrVal && var.bstrVal[0])
			{
				m_bwpp5= true;
				::VariantClear(&var);
			}else
				m_bwpp5 = false;
			pElement->Release();
		}
	}
	::SysFreeString(pXml);

	if(!m_btpp && !m_bvl && !m_bwpp && !m_bwpp5)
	{
		InitXssComplier();
		xsscomplier * xsscp = (xsscomplier *)m_xsscp;
		if(nLine ==-1)
		{
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"compiler...\r\n");

			xsscp->eval(pXml);

			::SetWindowText(pOutPut->m_hWnd,L"compiler finished...\r\n");

			::SysFreeString(pXml);
		}else
		{
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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

			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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

			wpp::wpp_context& context = *(wpp::wpp_context *)parser ;

			context.get_runtime()->GetSource	 = xfUriLoader::LoadScript;
			context.get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
			context.get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
			context.get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

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

			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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

			xlang::wpp_context& context = *(xlang::wpp_context *)parser ;

			context.get_runtime()->GetSource	 = xfUriLoader::LoadScript;
			context.get_runtime()->FreeSource	 = xfUriLoader::FreeScript;
			context.get_runtime()->GetSourceDT  = xfUriLoader::GetScriptDT;
			context.get_runtime()->GetSourceVER = xfUriLoader::GetScriptVER;

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

			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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
			InitXssComplier();
			vl::vlang3 * pvlang = (vl::vlang3 * )m_pvlang;

			int startIndex = 0;
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			myerrorVl* perr = new myerrorVl();
			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;
			perr->m_pEdit = GetControl(L"xcodecontent");

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
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
		}else
		{
			
			vl::vlang3 * xsmparser = null;
			if(parser)
				xsmparser = (vl::vlang3 *)parser;
			else 
				xsmparser = new vl::vlang3;
			xsmparser->GetSource = xfUriLoader::LoadScript;
			xsmparser->FreeSource= xfUriLoader::FreeScript;
			xsmparser->GetSourceDT = xfUriLoader::GetScriptDT;
			xsmparser->GetSourceVER = xfUriLoader::GetScriptVER;

			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
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
				perr->m_pEdit = GetControl(L"xcodecontent");

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
			}
		}
	}
	return 1;
}

int xPageSheet::InitXssComplier()
{
	if(!m_btpp && !m_bvl && !m_bwpp && !m_bwpp5)
	{
		xsscomplier * xsscp = (xsscomplier *)m_xsscp;;
		if(!xsscp)
		{
			xsscp = new xsscomplier();
			xsscp->m_pxvm = new xvmcore;

			myerror9* perr = new myerror9();

			xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
			perr->m_pOutPut = pOutPut;

			xsscp->m_error = perr;
			m_xsscp = xsscp;
		}
	}else if(m_btpp)
	{
		if(m_xsmparser) delete (parser_cpp * )m_xsmparser;
		m_xsmparser = new parser_cpp();
		((parser_cpp*)m_xsmparser)->GetSource = xfUriLoader::LoadScript;
		((parser_cpp*)m_xsmparser)->FreeSource= xfUriLoader::FreeScript;
		((parser_cpp*)m_xsmparser)->GetSourceDT = xfUriLoader::GetScriptDT;
		((parser_cpp*)m_xsmparser)->GetSourceVER = xfUriLoader::GetScriptVER;

		InitXssComplier(m_xsmparser);
	}else if(m_bvl)
	{
		if(m_pvlang) delete (vl::vlang3 * )m_pvlang;
		m_pvlang = new vl::vlang3();
		((vl::vlang3*)m_pvlang)->GetSource = xfUriLoader::LoadScript;
		((vl::vlang3*)m_pvlang)->FreeSource= xfUriLoader::FreeScript;
		((vl::vlang3*)m_pvlang)->GetSourceDT = xfUriLoader::GetScriptDT;
		((vl::vlang3*)m_pvlang)->GetSourceVER = xfUriLoader::GetScriptVER;

		//InitXssComplier(m_pvlang);
	}
	return 1;
}
int xPageSheet::InitXssComplier(void *parser)
{
	parser_cpp * xsmparser = (parser_cpp * )parser;
	((parser_cpp*)xsmparser)->GetSource = xfUriLoader::LoadScript;
	((parser_cpp*)xsmparser)->FreeSource= xfUriLoader::FreeScript;
	((parser_cpp*)xsmparser)->GetSourceDT = xfUriLoader::GetScriptDT;
	((parser_cpp*)xsmparser)->GetSourceVER = xfUriLoader::GetScriptVER;

	myerrorTpp* perr = new myerrorTpp();
	xfControl* pOutPut = GetFrameWindow()->GetControl(_T("output"));
	::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
	perr->m_pOutPut = pOutPut;
	perr->m_pEdit = GetControl(L"xcodecontent");

	xsmparser->m_perror = perr;
	int len =SendMessage(perr->m_pEdit->m_hWnd,SCI_GETTEXTLENGTH,0,0);
	SendMessage(perr->m_pEdit->m_hWnd,SCI_INDICATORCLEARRANGE,0,len);
	
	xbXml xml;
	BSTR pXml = NULL;
	XSEdit * pControl = (XSEdit *)GetControl(_T("xpagecontent"));
	pControl->GetContent(&pXml);
	if(pXml)
	{
		if(xml.LoadXml(pXml))
		{
			IXMLDOMElement * pDocElement = NULL;
			IXMLDOMNodeList * pList;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			LPTSTR pStr = NULL;
			BYTE * pData=NULL;
			int	   len;
			pStr = NULL;
			long nLen = 0;
			bool ok = false;
			VARIANT var;
			xml.GetXmlDoc()->get_documentElement(&pDocElement);
			pDocElement->selectNodes(L"script",&pList);
			pList->get_length(&nLen);
			for(int i=0;i<nLen;i++)
			{
				ok = false;
				pList->get_item(i,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pNode->Release();
				pElement->getAttribute(L"src",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					if(xfUriLoader::ProcessCmd(GetFrameWindow()->m_pXPage->m_pStrUrl,var.bstrVal,NULL,pData,len)>=0 && pData)
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
						xsmparser->cp = cp;
						xsmparser->cp->setsource(pStr,null);
						xsmparser->cp->insymbol();

						if(xsmparser->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
						xsmparser->cp->source = NULL;
						delete 	cp;
					}
					if(pData) xfUriLoader::FreeData(pData);
				}
				::VariantClear(&var);
			}
			pDocElement->Release();
		}
		::SysFreeString(pXml);
	}
	return 1;
}
int xPageSheet::OnXCommand(LPCTSTR	pStrID, class xfControl * pControl)
{
	if(pStrID && _tcsicmp(pStrID,_T("xmFileSave"))==0)
	{
		m_pParentWin->OnXCommand(pStrID,pControl);
		if(GetKeyState(VK_CONTROL)& 0x8000)
		{
			if(!(parser_cpp*)xfApp::GetApp()->m_xsmparser) return 1;
			parser_cpp * xsmparser = (parser_cpp*)xfApp::GetApp()->m_xsmparser;
			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;
			lexer* cp = new lexer;
			xsmparser->cp = cp;
			cp->setsource(pXml,null);
			cp->insymbol();
			bool ok = false;
			if(xsmparser->parse_trans_unit()==__atom_cp_parse_ok) ok = true;
			xsmparser->cp = null;
			cp->source = null;
			delete cp;
			if(pXml)::SysFreeString(pXml);
		}
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

	if(pStrID && _tcsicmp(pStrID,_T("xmRun"))==0)
	{
		HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
		hCursor = ::SetCursor(hCursor);

		if(GetKeyState(VK_CONTROL) & 0x8000 ? TRUE : FALSE) 
		{
			/*if(xfApp::GetApp()->m_wpp)
				delete (wpp::wpp_context*)xfApp::GetApp()->m_wpp;
			xfApp::GetApp()->m_wpp = 0;*/

			if(xfApp::GetApp()->m_wpp5)
				delete (xlang::wpp_context*)xfApp::GetApp()->m_wpp5;
			xfApp::GetApp()->m_wpp5 = 0;
		}

		xbXml xml;

		/*
		long nLen;
		LPTSTR pXml;
		nLen = ::GetWindowTextLength(GetControl(_T("xpagecontent"))->m_hWnd);
		pXml = new TCHAR[nLen +1];
		::GetWindowText(GetControl(_T("xpagecontent"))->m_hWnd,pXml,nLen+1);
		*/
		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xpagecontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;

		xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
		::SetWindowText(pOutPut->m_hWnd,_T(""));
		if(!xml.LoadXml(pXml))
		{
			IXMLDOMParseError* pError;
			xml.m_pXmlDoc->get_parseError(&pError);
			if(pError)
			{
				TCHAR ErrText[4096]={0};

				long nLine;
				long nLinePos;
				long nErrCode;
				pError->get_line(&nLine);
				pError->get_linepos(&nLinePos);
				pError->get_errorCode(&nErrCode);

				BSTR bstr;
				pError->get_reason(&bstr);
				_stprintf_s(ErrText,4096,_T("XPage 错误:%s\r\n     代码: 0x%x\r\n     位置: 第 %i 行 第 %i 列\r\n     原因: %s"),ErrText,nErrCode,nLine,nLinePos,bstr);
				::SysFreeString(bstr);
				::SetWindowText(pOutPut->m_hWnd,ErrText);

				int nIndex = (int)SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_LINEINDEX,nLine - 1,0);
				nIndex += nLinePos - 1;
				SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_SETSEL,nIndex,nIndex);
				SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_SCROLLCARET,0,0);
			}
			::SysFreeString(pXml);
			return 1;
		}
		::SysFreeString(pXml);

		IXMLDOMElement * pElementEx;
		xml.m_pXmlDoc->get_documentElement(&pElementEx);
		if(pElementEx)
		{
			TCHAR strUrl[INTERNET_MAX_URL_LENGTH];
			DWORD dwSize = INTERNET_MAX_URL_LENGTH;
			xfUriLoader::UrlCombineW(m_pXPage->m_pStrUrl,_T("/developer/xpage.xml"),strUrl,&dwSize,0);

			xfXPage * pXPage = new xfXPage();
			pXPage->m_pStrUrl = _tcsdup(strUrl);
			pXPage->Load(pElementEx);

			//set use guid dll
			HTREEITEM hItem = (HTREEITEM)this->m_pParam;
			if(hItem)
			{
				TVITEM tvItem;
				xfControl * pDevTreeCtrl = this->m_pParentWin->GetControl(_T("DevExplorer"));

				BSTR bstrTagName;
				HTREEITEM hpItem = hItem;
				IXMLDOMElement* pWorkElementEx;
findworkitems:	hpItem = TreeView_GetParent(pDevTreeCtrl->m_hWnd, hpItem);
				tvItem.mask = TVIF_PARAM|TVIF_HANDLE;
				tvItem.hItem = hpItem;
				TreeView_GetItem(pDevTreeCtrl->m_hWnd,&tvItem);
				pWorkElementEx = (IXMLDOMElement* )(tvItem.lParam);
				pWorkElementEx->get_tagName(&bstrTagName);
				if(_tcsicmp(bstrTagName,_T("work"))!=0 && hpItem) goto findworkitems;

				IXMLDOMElement * pItemElement = (IXMLDOMElement* )(tvItem.lParam);

				_variant_t var,var1;
				pItemElement->getAttribute(L"guid",&var);
				
				pElementEx->getAttribute(L"workguid",&var1);
				if(!var1.bstrVal)
				{
					pElementEx->setAttribute(L"workguid",var);
					pElementEx->setAttribute(L"xscript",(_variant_t)_T("yes"));
				}
			}
			pElementEx->Release();

			xfWin* pWin = xfWin::LoadXPageXml(pXPage,m_hWnd,xfApp::GetApp()->GetInstance());
		}
		hCursor = ::SetCursor(hCursor);
		return 1;
	}

	if(pStrID && _tcsicmp(pStrID,_T("xmCompile"))==0)
	{
		xbXml xml;
		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xpagecontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;
		if(xml.LoadXml(pXml))
		{
			VARIANT var;
			IXMLDOMElement * pElement;
			xml.GetXmlDoc()->get_documentElement(&pElement);
			if(pElement)
			{
				pElement->getAttribute(L"tpp_guid",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_btpp = true;
					::VariantClear(&var);
				}else
					m_btpp = false;
				pElement->getAttribute(L"classurl",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_bvl = true;
					::VariantClear(&var);
				}else
					m_bvl = false;
				pElement->getAttribute(L"class_url",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_bwpp = true;
					::VariantClear(&var);
				}else
					m_bwpp = false;
				pElement->getAttribute(L"class_url5",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_bwpp5 = true;
					::VariantClear(&var);
				}else
					m_bwpp5 = false;
				pElement->Release();
			}
		}
		::SysFreeString(pXml);
		if(!m_btpp && !m_bvl && !m_bwpp && !m_bwpp5)
		{
			TCHAR title[255];
			::GetWindowText(m_hWnd,title,255);
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			BSTR pXml = NULL;
			XSEdit * pControl = (XSEdit *)GetControl(_T("xcodecontent"));
			pControl->GetContent(&pXml);
			if(!pXml) return 1;

			xsscomplier * xsscp;

			xsscp = new xsscomplier();
			xsscp->m_pxvm = new xvmcore;

			myerror9* perr = new myerror9();
			xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
			::SetWindowText(pOutPut->m_hWnd,L"正在编译...\r\n");
			perr->m_pOutPut = pOutPut;

			xsscp->m_error = perr;
			xsscp->eval(pXml);

			TCHAR buf[1024];
			TCHAR buf1[1024];

			xfControl* PCode = pControl->GetWin()->m_pParentWin->GetControl(_T("pcode"));
			::SetWindowText(PCode->m_hWnd,L"");

			int nlevel = 0;

			buf1[0] = 0;
			std::vector<int> seqs;
			int index = 0;  
			seqs.push_back(0);
			bool bdesc = false;
			for(int i=0;i<(int)xsscp->m_pCodes->size();i++)
			{
				int nLenText = ::GetWindowTextLength(PCode->m_hWnd);
				SendMessage(PCode->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
				if((*xsscp->m_pCodes)[i])
				{
					if(::_tcsstr((*xsscp->m_pCodes)[i],_T("blkstart"))== (*xsscp->m_pCodes)[i])
					{

						_stprintf_s(buf,1024,_T("%s{ %s\r\n"),buf1,(*xsscp->m_pCodes)[i] + _tcslen(_T("blkstart")));
						buf1[nlevel*3] = ' ';
						buf1[nlevel*3 + 1] = ' ';
						buf1[nlevel*3 + 2] = ' ';
						nlevel++;
						buf1[nlevel*3] = 0;
					}else if(::_tcsstr((*xsscp->m_pCodes)[i],_T("blkend"))== (*xsscp->m_pCodes)[i])
					{
						if(nlevel == index) index --;
						nlevel--;
						buf1[nlevel*3] = 0;
						_stprintf_s(buf,1024,_T("%s}\r\n"),buf1);
					}else
					{
						if((*xsscp->m_pCodes)[i][0]=='.')
						{
							index++;
							seqs.push_back(0);
							_stprintf_s(buf,1024,_T("%s%s"),buf1,(*xsscp->m_pCodes)[i]);
						}else
						{
							_stprintf_s(buf,1024,_T("%s%d  %s"),buf1,seqs[index],(*xsscp->m_pCodes)[i]);
							seqs[index]++;

						}
					}
				}else
				{
					_stprintf_s(buf,1024,_T("#invalid\r\n"));
				}
				SendMessage(PCode->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
				SendMessage(PCode->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);
			}

			_stprintf_s(buf,1024,L"编译完成: - %i 个错误  %i 个警告",perr->m_pErrors->size(),perr->m_pWarns->size());

			int nLenText = ::GetWindowTextLength(pOutPut->m_hWnd);
			SendMessage(pOutPut->m_hWnd,EM_SETSEL,nLenText + 1,nLenText + 1);
			SendMessage(pOutPut->m_hWnd,EM_REPLACESEL,false,(LPARAM)buf);
			SendMessage(pOutPut->m_hWnd,EM_SCROLL,SB_LINEDOWN,0);

			if(xsscp)
			{
				if(xsscp->m_pxvm) delete xsscp->m_pxvm;
				delete xsscp;
			}

			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
		}
		else if(m_bwpp)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			wpp::wpp_runtime runtime;
			wpp::wpp_context context(&runtime);
			if(!m_wpp) 
				m_wpp = new Wpp;
			context.set_wpp_ptr((Wpp*)m_wpp);

			Eval(-1,&context);

			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
		}
		else if(m_bwpp5)
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			xlang::wpp_runtime runtime;
			xlang::wpp_context context(&runtime);
			if(!m_wpp5) 
				m_wpp5 = new xlang::Wpp5;
			context.set_wpp_ptr((xlang::Wpp5*)m_wpp5);

			Eval(-1,&context);

			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
		}
		else
		{
			HCURSOR hCursor = LoadCursor(NULL,IDC_WAIT);
			hCursor = ::SetCursor(hCursor);

			Eval(-1);
			hCursor = ::SetCursor(hCursor);
			DestroyCursor(hCursor);
		}
	}
		
	if(pStrID && _tcsicmp(pStrID,_T("xmValid"))==0)
	{
		xbXml xml;
		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xpagecontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;

		xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
		::SetWindowText(pOutPut->m_hWnd,_T(""));
		if(!xml.LoadXml(pXml))
		{
			IXMLDOMParseError* pError;
			xml.m_pXmlDoc->get_parseError(&pError);
			if(pError)
			{
				TCHAR ErrText[4096]={0};

				long nLine;
				long nLinePos;
				long nErrCode;
				pError->get_line(&nLine);
				pError->get_linepos(&nLinePos);
				pError->get_errorCode(&nErrCode);

				BSTR bstr;
				pError->get_reason(&bstr);
				_stprintf_s(ErrText,4096,_T("XPage 错误:%s\r\n     代码: 0x%x\r\n     位置: 第 %i 行 第 %i 列\r\n     原因: %s"),ErrText,nErrCode,nLine,nLinePos,bstr);
				::SysFreeString(bstr);
				::SetWindowText(pOutPut->m_hWnd,ErrText);

				int nIndex = (int)SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_LINEINDEX,nLine - 1,0);
				nIndex += nLinePos - 1;
				SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_SETSEL,nIndex,nIndex);
				SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_SCROLLCARET,0,0);
			}
		}
		::SysFreeString(pXml);
		return 1;
	}
	return 0;
}
xfControl * xPageSheet::CreateControl(LPCTSTR pCtrlID,LPCTSTR pStrClass,IXMLDOMElement *)
{
	return NULL;
}

int xPageSheet::PreSwitchLayer(class xfControl * pControl,LPCTSTR pStrLayerSheet,LPCTSTR pStrLayerFrame)
{
	if(_tcsicmp(pStrLayerSheet,_T("sheet1"))==0)
		m_nSheet = 0;
	else if(_tcsicmp(pStrLayerSheet,_T("sheet2"))==0)
		m_nSheet = 1;
	else if(_tcsicmp(pStrLayerSheet,_T("sheet3"))==0)
		m_nSheet = 2;

	SetAgent();
	if(_tcsicmp(pStrLayerSheet,_T("sheet3"))==0)
	{
		xbXml xml;
		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xpagecontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;
		if(xml.LoadXml(pXml))
		{
			VARIANT var;
			IXMLDOMElement * pElement;
			xml.GetXmlDoc()->get_documentElement(&pElement);
			if(pElement)
			{
				pElement->getAttribute(L"tpp_guid",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_btpp = true;
					::VariantClear(&var);
				}else
					m_btpp = false;
				pElement->getAttribute(L"classurl",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_bvl = true;
					::VariantClear(&var);
				}else
					m_bvl = false;
				pElement->getAttribute(L"class_url",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_bwpp = true;
					::VariantClear(&var);
				}else
					m_bwpp = false;
				pElement->getAttribute(L"class_url5",&var);
				if(var.bstrVal && var.bstrVal[0])
				{
					m_bwpp5 = true;
					::VariantClear(&var);
				}else
					m_bwpp5 = false;
				pElement->Release();
			}
		}
		::SysFreeString(pXml);
		return 1;
	}
	else if(_tcsicmp(pStrLayerSheet,_T("sheet2"))==0)
	{
		

		xbXml xml;
		long nLen;
		/*LPTSTR pXml;
		nLen = ::GetWindowTextLength(GetControl(_T("xpagecontent"))->m_hWnd);
		pXml = new TCHAR[nLen +1];
		::GetWindowText(GetControl(_T("xpagecontent"))->m_hWnd,pXml,nLen+1);*/

		BSTR pXml = NULL;
		XSEdit * pControl = (XSEdit *)GetControl(_T("xpagecontent"));
		pControl->GetContent(&pXml);
		if(!pXml) return 1;

		xfControl* pOutPut = pControl->GetWin()->m_pParentWin->GetControl(_T("output"));
		::SetWindowText(pOutPut->m_hWnd,_T(""));
		if(!xml.LoadXml(pXml))
		{
			IXMLDOMParseError* pError;
			xml.m_pXmlDoc->get_parseError(&pError);
			if(pError)
			{
				TCHAR ErrText[4096]={0};

				long nLine;
				long nLinePos;
				long nErrCode;
				pError->get_line(&nLine);
				pError->get_linepos(&nLinePos);
				pError->get_errorCode(&nErrCode);

				BSTR bstr;
				pError->get_reason(&bstr);
				_stprintf_s(ErrText,4096,_T("XPage 错误:%s\r\n     代码: 0x%x\r\n     位置: 第 %i 行 第 %i 列\r\n     原因: %s"),ErrText,nErrCode,nLine,nLinePos,bstr);
				::SysFreeString(bstr);
				::SetWindowText(pOutPut->m_hWnd,ErrText);

				int nIndex = (int)SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_LINEINDEX,nLine - 1,0);
				nIndex += nLinePos - 1;
				SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_SETSEL,nIndex,nIndex);
				SendMessage(GetControl(_T("xpagecontent"))->m_hWnd,EM_SCROLLCARET,0,0);
			}
			::SysFreeString(pXml);
			return -1;
		}
		::SysFreeString(pXml);

		xfNode* pNode = m_pXPage->m_pxfNode->GetNodeByName(_T("formarea"));
		for(int i=0;i<(int)pNode->m_pChilds->size();i++)
		{
			pNode->m_pElement->removeChild((*pNode->m_pChilds)[i]->m_pElement,NULL);
		}
		pNode->RemoveChild();

		IXMLDOMElement* pDocElement;
		IXMLDOMNode* pXmlNode;
		IXMLDOMNodeList* pNodeList;
		IXMLDOMElement* pElement;
		xml.m_pXmlDoc->get_documentElement(&pDocElement);
		pDocElement->selectNodes(L"*",&pNodeList);
		pNodeList->get_length(&nLen);
		for(int k=0;k<nLen;k++)
		{
			pNodeList->get_item(k,&pXmlNode);
			pXmlNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
			pXmlNode->Release();
			pNode->m_pElement->appendChild(pElement,NULL);
			pElement->Release();
		}
		pNodeList->Release();
		pNode->ParseNode(pNode->m_pElement,true);
		pNode->CreateNodeControl(m_hWnd);
		return 2;
	}
	return 1;
}

int  xPageSheet::OnCodeFind()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcodecontent"))->GetInterface();
	pEdit->OnFindDlg();
	return 1;
}

int  xPageSheet::OnCodeReplace()
{
	IXSEdit*  pEdit = (IXSEdit*)this->GetControl(_T("xcodecontent"))->GetInterface();
	pEdit->OnReplaceDlg();
	 return 1;
}

