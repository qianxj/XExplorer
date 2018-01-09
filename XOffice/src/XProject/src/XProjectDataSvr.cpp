#include "StdAfx.h"
#include "..\include\XProjectDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	XProjectDataSvr::XProjectDataSvr(void):m_pTaskSvr(NULL)
	{
	}

	XProjectDataSvr::~XProjectDataSvr(void)
	{
		if(m_pTaskSvr) delete m_pTaskSvr;
	}
}}}}
