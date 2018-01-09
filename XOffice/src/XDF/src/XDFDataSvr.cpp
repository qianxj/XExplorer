#include "StdAfx.h"
#include "..\include\XDFDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	XDFDataSvr::XDFDataSvr(void):m_pPagesSvr(NULL)
	{
	}

	XDFDataSvr::~XDFDataSvr(void)
	{
		if(m_pPagesSvr) delete m_pPagesSvr;
	}
}}}}
