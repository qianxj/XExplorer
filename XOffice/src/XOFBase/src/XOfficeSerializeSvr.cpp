#include "StdAfx.h"
#include "..\include\XOfficeSerializeSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeSerializeSvr::XOfficeSerializeSvr(void):m_pStrGuid(NULL),m_Ver(10000)
	{

	}

	XOfficeSerializeSvr::~XOfficeSerializeSvr(void)
	{
		if(m_pStrGuid)delete m_pStrGuid;
	}
}}}
