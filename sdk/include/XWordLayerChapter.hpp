#pragma once
#include "XWordLayerBase.hpp"

class XWordLayerDocument;
class XWordLayerChapter : public XWordLayerBase
{
public:
	XWordLayerChapter(void);
	~XWordLayerChapter(void);
public:
	int DoDraw(XWordDataSvr* pDataSvr,XWordLayerDocument * pDocument,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
};
