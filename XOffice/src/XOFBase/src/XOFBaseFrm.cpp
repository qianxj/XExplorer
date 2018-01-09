#include "stdafx.h"
#include "XOFBaseFrm.hpp"


namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	XOFBaseFrm::XOFBaseFrm():m_pFrameSvr(NULL)
	{
	}

	XOFBaseFrm::~XOFBaseFrm()
	{
		if(m_pFrameSvr) delete m_pFrameSvr;
	}

	int XOFBaseFrm::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		if(m_pFrameSvr)
		{
			if(pDrawRect)m_DrawRect  = *pDrawRect;
			return m_pFrameSvr->DoDraw(hPaintDC,pDrawRect);
		}
		else
			return 0;
	}

}}}
