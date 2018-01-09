#pragma once
#include "XWordLayerBase.hpp"
#include "XWordDevice.hpp"
#include "XWCodeAsm.hpp"
#include "wpp4.h"
#include <vector>

class XWordLayerDocument;
class XWordLayerPara : public XWordLayerBase
{
public:
	XWordLayerPara(void);
	~XWordLayerPara(void);
public:
	//�����ų�����ľ�����
	std::vector<RECT> m_exRects;
	//�����ų�����
	int AddExRect(RECT &rc){m_exRects.push_back(rc); return (int)m_exRects.size();}
	//����ų�����
	int ClearExRect();
	//�õ��ų�����,��Ҫ�ϲ����������ĺ�ҳ���ų�����.
	int GetExceptRects(std::vector<RECT> & exRects);

public:
	//��ʾ���ռ������
	virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);

public:
	int DoDraw(XWordDataSvr* pDataSvr,XWordLayerDocument * pDocument,Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
	int DoDraw(XWordLayerDocument * pDocument,BlockMetaInfo pBlockMetaInfo[], int nLen, HDC hPaintDC,POINT & ptOffset);
public:
	int DrawContent(XWordLayerDocument * pDocument, BlockMetaInfo * pBlockMetaInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen);
public:
	//��ȡ�趨�߶ȺͿ����Ϣ
	virtual SIZE GetExtent();
	//��ȡʵ��ռ�õĸ߶ȺͿ����Ϣ
	virtual SIZE GetRealExtent();
	//���¼���λ����Ϣ
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,vord_t& pBlock,const Css::TBoxStyle * pParentBoxStyle, HDC hDC);
public:
	//����ռ�õĸ߶ȺͿ����Ϣ
	SIZE ReCalcExtent(BlockMetaInfo pBlockMetaInfo[],int nLen);

public:
	//�������ݿ�Ĳ�����Ϣ
	SIZE CalcBlockMetaInfo(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle, HDC hDC,vord_t& pBlock,BlockMetaInfo * pBlockMetaInfo);
	//������ݿ鲼�ֶ�����
	int  ClearMetaInfo(BlockMetaInfo pBlockMetaInfo[],int nCount);
private:
	//�����ұ߿����ɾ���Ŀ�ȣ�����ʱ��Ҫ�ų�ExRect
	int GetLayerRight(BlockMetaInfo pBlockMetaInfo[],int nHeight,int nMinWidth, POINT ptOffset);
	//�����ұ߿����ɾ���ľ��󣬼���ʱ��Ҫ�ų�ExRect
	int GetLayerRightRects(std::vector<RECT> &rRect,BlockMetaInfo pBlockMetaInfo[],int nHeight,int nMinWidth, POINT ptOffset);
	//�õ��ཻ���ų�����
	BOOL GetUnionExRect(std::vector<RECT> &vRect, RECT &rtBound);
	//���մ��������������
	int SortRects(std::vector<RECT> &vRect);
//layer
public:
	const Css::TBoxStyle *  GetBoxStyle(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,vord_t pBlock);
	int CalcLayer(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle,XWCodeAsm  * pAsm, XWordDevice * pDevice);
	int CalcLayer(XWCodeAsm  * pAsm, BlockMetaInfo pBlockMetaInfo[], int nLen);
};
