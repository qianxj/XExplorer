#pragma once

#include "xofbase.h"
#include "xofficefrmsvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOfficeFrmDataSvr :
		public XOfficeData
	{
	public:
		XOfficeFrmDataSvr(void);
		~XOfficeFrmDataSvr(void);
	public:
		XOfficeFrmSvr * m_pOfficeFramSvr;
	};
}}}
