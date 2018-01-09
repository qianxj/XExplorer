#include "StdAfx.h"
#include "..\include\XWordDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XWordDataSvr::XWordDataSvr(void):m_pContentSvr(NULL)
	{
	}

	XWordDataSvr::~XWordDataSvr(void)
	{
		if(m_pContentSvr) delete m_pContentSvr;
	}
}}}}
