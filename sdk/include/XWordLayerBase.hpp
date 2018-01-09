#pragma once
#include "xuxutil.h"
#include "xwordblock.hpp"
#include "XWordDataSvr.hpp"

using namespace Hxsoft::XFrame;
using namespace Hxsoft::XUtil;
using namespace Hxsoft::XFrame::XOffice::XWord;

//文本块布局信息
class XWordBlock;
struct BlockMetaInfo
{
	int * pDx;
	LPTSTR pStr;
	int nLenStr;
	Css::TBoxStyle * pBoxStyle;
	BOOL bDeleteBoxStyle;
	SIZE ContentExtent;
	XWordBlock * pBlock;
};

class XWordLayerBase
{
public:
	XWordLayerBase(void);
	XWordLayerBase(XWordBlock * m_pBlock);
	~XWordLayerBase(void);
public:
	//外部区域
	RECT FRect;
	//设置外部区域
	inline void SetFRect(RECT &rc){ FRect = rc;}
	//获取外部区域
	inline const RECT & GetFRect(){return FRect;}
public:
	//内容区域
	RECT FContentRect;
	//设置内容区域
	inline void SetContentRect(RECT &rc){ FContentRect = rc;}
	//获取内容区域
	inline const RECT & GetContentRect(){return FContentRect;}
public:
	//获取设定高度和宽度信息
	virtual SIZE GetExtent();
	//获取实际占用的高度和宽度信息
	virtual SIZE GetRealExtent();
	//重新计算位置信息
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XWordBlock * pBlock,Css::TBoxStyle * pParentBoxStyle, HDC hDC);
private:
	XWordBlock * _pBlock;
public:
	void  SetWordBlock(XWordBlock * pBlock);
	XWordBlock *  GetWordBlock();
};
