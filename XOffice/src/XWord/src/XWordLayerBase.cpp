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

//��ȡ�趨�߶ȺͿ����Ϣ
SIZE XWordLayerBase::GetExtent()
{
	SIZE sz={0,0};
	return sz;
}
//��ȡʵ��ռ�õĸ߶ȺͿ����Ϣ
SIZE XWordLayerBase::GetRealExtent()
{
	SIZE sz={0,0};
	return sz;
}

#if V3
//���¼���λ����Ϣ
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
//���¼���λ����Ϣ
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


