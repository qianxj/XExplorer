#pragma once

#include "xofbase.h"
#include "griddatasvr.hpp"
#include "xofobject.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class  XCELL_API  XGridDataSvrEx : public XOfficeData
	{
	public:
		XGridDataSvrEx(void);
		~XGridDataSvrEx(void);
	public:
		CAxisSvr*				m_pAxisSvr;
		class XXmlContentSvr*	m_pContentSvr;
	public:
		bool m_bShared;
		
	};
}}}}
