#pragma once
#include "XWordLayerBase.hpp"
#include "XWordCursor.hpp"
#include "XWordDevice.hpp"

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
	//获取设定高度和宽度信息
	virtual SIZE GetExtent();
	//获取实际占用的高度和宽度信息
	virtual SIZE GetRealExtent();
	//重新计算位置信息
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC);
public:
	int DoDraw(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
};
