#include "StdAfx.h"
#include "..\include\XGridDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XGridDataSvr::XGridDataSvr(void)
	{
	}

	XGridDataSvr::~XGridDataSvr(void) 
	{
		if(GetIAxisSvr())delete GetIAxisSvr();
		if(GetIContentSvr())delete GetIContentSvr();
	}
}}}
