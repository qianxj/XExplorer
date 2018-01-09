#pragma once
#include "xuxutil.h"
#include "xwordblock.hpp"
#include "XWordDataSvr.hpp"
#include "wpp4.h"

using namespace Hxsoft::XFrame;
using namespace Hxsoft::XUtil;
using namespace Hxsoft::XFrame::XOffice::XWord;

//�ı��鲼����Ϣ
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
	//�ⲿ����
	RECT FRect;
	//�����ⲿ����
	inline void SetFRect(RECT &rc){ FRect = rc;}
	//��ȡ�ⲿ����
	inline const RECT & GetFRect(){return FRect;}
public:
	//��������
	RECT FContentRect;
	//������������
	inline void SetContentRect(RECT &rc){ FContentRect = rc;}
	//��ȡ��������
	inline const RECT & GetContentRect(){return FContentRect;}
public:
	//��ȡ�趨�߶ȺͿ����Ϣ
	virtual SIZE GetExtent();
	//��ȡʵ��ռ�õĸ߶ȺͿ����Ϣ
	virtual SIZE GetRealExtent();
#ifdef V3
	//���¼���λ����Ϣ
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
