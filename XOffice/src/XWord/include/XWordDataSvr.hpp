#pragma once
#include "xofbase.h"
#include "xwordcontentsvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWordDataSvr :
		public XOfficeData
	{
	public:
		XWordDataSvr(void);
		~XWordDataSvr(void);
	public:
		XWordContentSvr*	m_pContentSvr;
	};
}}}}