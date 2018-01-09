#include "StdAfx.h"
#include "XWordLayerBase.hpp"

#if V3
XWordLayerBase::XWordLayerBase(void):_pBlock(NULL)
{
}

XWordLayerBase::XWordLayerBase(XWordBlock * pBlock):_pBlock(pBlock)
{
}
#else
XWordLayerBase::XWordLayerBase(void)
{

}

#endif


XWordLayerBase::~XWordLayerBase(void)
{
}

//获取设定高度和宽度信息
SIZE XWordLayerBase::GetExtent()
{
	SIZE sz={0,0};
	return sz;
}
//获取实际占用的高度和宽度信息
SIZE XWordLayerBase::GetRealExtent()
{
	SIZE sz={0,0};
	return sz;
}

#if V3
//重新计算位置信息
SIZE XWordLayerBase::CalcExtent(XWordDataSvr* pDataSvr,XWordBlock * pBlock,Css::TBoxStyle * pParentBoxStyle, HDC hDC)
{
	SIZE sz={0,0};
	return sz;
}

void  XWordLayerBase::SetWordBlock(XWordBlock * pBlock)
{
	_pBlock = pBlock;
}

XWordBlock *  XWordLayerBase::GetWordBlock()
{
	return _pBlock;
}
#else
//重新计算位置信息
SIZE XWordLayerBase::CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC)
{
	SIZE sz={0,0};
	return sz;
}

void  XWordLayerBase::set_block(vord_t & v)
{
	block_ = v;
}

vord_t&  XWordLayerBase::get_block()
{
	return block_;
}
#endif


