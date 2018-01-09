#pragma once
#include "xuxutil.h"
#include "xwordblock.hpp"
#include "XWordDataSvr.hpp"

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
	XWordBlock * pBlock;
};

class XWordLayerBase
{
public:
	XWordLayerBase(void);
	XWordLayerBase(XWordBlock * m_pBlock);
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
	//���¼���λ����Ϣ
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XWordBlock * pBlock,Css::TBoxStyle * pParentBoxStyle, HDC hDC);
private:
	XWordBlock * _pBlock;
public:
	void  SetWordBlock(XWordBlock * pBlock);
	XWordBlock *  GetWordBlock();
};
