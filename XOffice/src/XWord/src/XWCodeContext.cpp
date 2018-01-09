#include "StdAfx.h"
#include "XWCodeContext.hpp"

XWCodePaintContext::XWCodePaintContext():pDevice_(NULL),pDataSvr(NULL),fullpage(false),pSelection(0),
	nRow(-1),piror_ext(0)
{
	ptPoint.x = 0;
	ptPoint.y = 0;
}

XWCodePaintContext::XWCodePaintContext(XWordDevice* pDevice):pDevice_(pDevice),fullpage(false),pSelection(0),
	nRow(-1),piror_ext(0)
{
	ptPoint.x = 0;
	ptPoint.y = 0;
}

XWCodePaintContext::~XWCodePaintContext()
{
}

XWordDevice* XWCodePaintContext::GetDevice()
{
	return pDevice_;
}

void XWCodePaintContext::SetDevice(XWordDevice* pDevice)
{
	pDevice_ = pDevice;
}