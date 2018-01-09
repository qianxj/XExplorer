#pragma once

#include "xofbase.h"
#include "xuxutil.h"
#include "XProjectTaskSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	class XProjectDataSvr :
		public XOfficeData
	{
	public:
		XProjectDataSvr(void);
		~XProjectDataSvr(void);
	public:
		XProjectTaskSvr*	m_pTaskSvr;
	};
}}}}
