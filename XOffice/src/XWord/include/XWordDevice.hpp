#pragma once
#include <vector>

class XWordBlock;
class XWordDevice
{
public:
	XWordDevice(void);
	~XWordDevice(void);
public:
	void SetDC(HDC hDC);
	HDC GetDC();
public:
	void SetPaintDC(HDC hDC);
	HDC GetPaintDC();
private:
	HDC m_hDC;
	HDC m_hPaintDC;
public:
	RECT * pDrawRect;
	RECT * pUpdateRect;
	RECT * rcPage;
	RECT * rcContentPage;
public:
	int	startRow;
	int endRow;
};
