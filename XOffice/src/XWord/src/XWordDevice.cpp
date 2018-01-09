#include "StdAfx.h"
#include "XWordDevice.hpp"

XWordDevice::XWordDevice(void):m_hDC(NULL),m_hPaintDC(NULL)
{
}

XWordDevice::~XWordDevice(void)
{
}

void XWordDevice::SetDC(HDC hDC)
{
	m_hDC = hDC;
}

HDC XWordDevice::GetDC()
{
	return m_hDC;
}

void XWordDevice::SetPaintDC(HDC hPaintDC)
{
	m_hPaintDC = hPaintDC;
}

HDC XWordDevice::GetPaintDC()
{
	return m_hPaintDC;
}
