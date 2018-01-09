#pragma once
#include "XWordLayerBase.hpp"
class XWordLayerDocument : public XWordLayerBase
{
public:
	XWordLayerDocument(void);
	~XWordLayerDocument(void);
private:
	int _pageCount;
public:
	int getPageCount();
	int setPageCount(int count,bool redraw = false);
public:
	int DoDraw(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
};
