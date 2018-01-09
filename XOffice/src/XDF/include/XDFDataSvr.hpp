#pragma once

#include "xofbase.h"
#include "XDFpagessvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	class XDF_API XDFDataSvr :
		public XOffice::XOfficeData
	{
	public:
		XDFDataSvr(void);
		~XDFDataSvr(void);
	public:
		XDFPagesSvr*	m_pPagesSvr;
	};
}}}}
