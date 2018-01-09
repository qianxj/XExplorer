#pragma once
#include "xuxutil.h"
#include "xwordblock.hpp"
#include "XWordDataSvr.hpp"
#include "wpp4.h"

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
#ifdef V3
	XWordBlock * pBlock;
#else
	vord_t  pBlock;
#endif

#if 0
	BlockMetaInfo()
	{
	}
	BlockMetaInfo( const BlockMetaInfo &b)
	{
	}
	BlockMetaInfo &  operator = ( const BlockMetaInfo &b)
	{
		pDx = b.pDx;
		/*
	LPTSTR pStr;
	int nLenStr;
	Css::TBoxStyle * pBoxStyle;
	BOOL bDeleteBoxStyle;
	SIZE ContentExtent;
#ifdef V3
	XWordBlock * pBlock;
#else
	vord_t  pBlock;*/

	}
#endif
};

class XWordLayerBase
{
public:
	XWordLayerBase(void);
#if V3
	XWordLayerBase(XWordBlock * m_pBlock);
#endif
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
#ifdef V3
	//重新计算位置信息
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XWordBlock * pBlock,Css::TBoxStyle * pParentBoxStyle, HDC hDC);
private:
	XWordBlock * _pBlock;
public:
	void  SetWordBlock(XWordBlock * pBlock);
	XWordBlock *  GetWordBlock();
#else
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC);
#endif

private:
	vord_t block_;
public:
	void  set_block(vord_t &v);
	vord_t&  get_block();

};
