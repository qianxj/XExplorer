#ifndef WPP_WERR_H
#define WPP_WERR_H

#include "vlang3.h"

namespace wpp {
namespace how {

class VLANG3_API wErr
{
public:
	virtual int adderror(const wchar_t* pFile,int nrow,int ncol,int nError,const wchar_t* pErrText,int nstart,int nlen)=0;
	virtual int addwarn(const wchar_t* pFile,int nrow,int ncol,int nError,const wchar_t* pErrText,int nstart,int nlen)=0;
	virtual int print(wchar_t* format,...)=0;
	virtual int output(const wchar_t* pFile,const wchar_t* pMsgText)=0;
};

}} //// namespace wpp::how

#endif //WPP_WERR_H