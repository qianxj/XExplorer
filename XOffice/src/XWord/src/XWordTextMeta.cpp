#include "StdAfx.h"
#include "XWordTextMeta.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XWordTextMeta::XWordTextMeta(void)
	{
	}

	XWordTextMeta::~XWordTextMeta(void)
	{

	}

	int XWordTextMeta::GetFitedText(HDC hdc,LPTSTR pStrText,int nWidth)
	{
		int nStrLen = _tcslen(pStrText);
		int * pDx = NULL;
		int nFit = 0;
		SIZE size ={0,0};
		GetTextExtentExPoint(hdc,pStrText,nStrLen,nWidth,&nFit,pDx,&size);
		
		return nFit;
	}

}}}}
