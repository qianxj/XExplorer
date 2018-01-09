#include "StdAfx.h"
#include "..\include\XShowSheetSvr.hpp"
#include "xshowserializesvr.hpp"
#include "xshowdatasvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XShow
{
	XShowSheetSvr::XShowSheetSvr(void)
	{
		m_pSerializeSvr = new XShowSerializeSvr;
		m_pDataSvr = new XShowDataSvr;
	}

	XShowSheetSvr::~XShowSheetSvr(void)
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
	}

	int XShowSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRec)
	{
		return 0;
	}
	int XShowSheetSvr::SetFRect(RECT rc)
	{
		XOfficeSheetSvr::SetFRect(rc);
		return 0;
	}

}}}}
