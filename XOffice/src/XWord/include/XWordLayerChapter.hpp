#pragma once
#include "XWordLayerBase.hpp"
#include "XWordCursor.hpp"

class XWordLayerDocument;
class XWordLayerChapter : public XWordLayerBase
{
public:
	XWordLayerChapter(void);
	~XWordLayerChapter(void);
public:
	int DoDraw(XWordDataSvr* pDataSvr,XWordLayerDocument * pDocument,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
public:
	//获取设定高度和宽度信息
	virtual SIZE GetExtent();
	//获取实际占用的高度和宽度信息
	virtual SIZE GetRealExtent();
	SIZE CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC);
public:
	RECT GetImageRect(SIZE sz,RECT &rt);
	RECT GetImageBoundRect(SIZE sz,RECT &rt);
};
