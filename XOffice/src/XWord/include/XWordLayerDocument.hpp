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
	//��ȡ�趨�߶ȺͿ����Ϣ
	virtual SIZE GetExtent();
	//��ȡʵ��ռ�õĸ߶ȺͿ����Ϣ
	virtual SIZE GetRealExtent();
	//���¼���λ����Ϣ
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC);
public:
	int DoDraw(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
};
