#include "StdAfx.h"
#include "XOfficeFrmSvr.hpp"
#include "XOfficeFrmSerializeSvr.hpp"
#include "XOfficeFrmDataSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOfficeFrmSvr::XOfficeFrmSvr()
	{
		m_pSerializeSvr = new XOfficeFrmSerializeSvr;
		m_pDataSvr = new XOfficeFrmDataSvr;
		((XOfficeFrmDataSvr*)m_pDataSvr)->m_pOfficeFramSvr = this;
	}

	XOfficeFrmSvr::~XOfficeFrmSvr(void)
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
	}  
	
}}}
