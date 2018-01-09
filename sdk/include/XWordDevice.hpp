#pragma once

class XWordBlock;
class XWordDevice
{
public:
	XWordDevice(void);
	~XWordDevice(void);
public:
	HDC m_hDC;
public:
	void SetHDC(HDC hDC);
	HDC GetHDC();
public:
	int DrawBlock(HDC hDC,XWordBlock * pBlock);
	int DrawRow(HDC hDC,LPTSTR pStrText,int nRowWidth,int uFormat);
};
