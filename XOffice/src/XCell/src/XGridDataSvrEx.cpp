#include "StdAfx.h"
#include "..\include\XGridDataSvrEx.hpp"
#include "xofobject.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridDataSvrEx::XGridDataSvrEx(void)
	{
		m_pAxisSvr = new CAxisSvr();
		m_pContentSvr = new XXmlContentSvr();
		m_bShared = false;
	}

	XGridDataSvrEx::~XGridDataSvrEx(void)
	{
		if(m_pAxisSvr)delete m_pAxisSvr;
		if(m_pContentSvr && !m_bShared)  delete m_pContentSvr;
	}
}}}}