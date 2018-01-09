#ifndef WPP7_WERR_H
#define WPP7_WERR_H

#include "vlang7.h"

namespace xlang {

class VLANG_API vlError
{
public:
	virtual int adderror(const wchar_t* pFile,int nrow,int ncol,int nError,const wchar_t* pErrText,int nstart,int nlen)=0;
	virtual int addwarn(const wchar_t* pFile,int nrow,int ncol,int nError,const wchar_t* pErrText,int nstart,int nlen)=0;
	virtual int print(wchar_t* format,...)=0;
	virtual int output(const wchar_t* pFile,const wchar_t* pMsgText)=0;
};

} //// namespace xlang

#endif //WPP7_WERR_H