#pragma once

#include "vldef.hpp"
 
namespace vl
{
	class VL_API vlErr
	{
	public:
		virtual int adderror(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)=0;
		virtual int addwarn(const tchar* pFile,int nrow,int ncol,int nError,const tchar* pErrText,int nstart,int nlen)=0;
	};
}