#pragma once
#include "XWordLayerBase.hpp"
#include <vector>
#include <xtree>

class XWordLayerDocument;
class XWordLayerPara : public XWordLayerBase
{
public:
	XWordLayerPara(void);
	~XWordLayerPara(void);
public:
	//ֱ���и߶�
	int m_nRowHeight;
	//�м��
	vector<int> m_spRowHeights;
	//�о�
	vector<int> m_RowHeights;
	//�õ�����ʼλ��
	POINT GetRowPosition(int nRow);
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
	int DoDraw(XWordLayerDocument * pDocument,BlockMetaInfo pBlockMetaInfo[], int nLen, HDC hPaintDC);
public:
	int DrawContent(XWordLayerDocument * pDocument, BlockMetaInfo * pBlockMetaInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen);
public:
	//��ȡ�趨�߶ȺͿ����Ϣ
	//virtual SIZE GetExtent();
	//��ȡʵ��ռ�õĸ߶ȺͿ����Ϣ
	//virtual SIZE GetRealExtent();
	//���¼���λ����Ϣ
	virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XWordBlock * pBlock,Css::TBoxStyle * pParentBoxStyle, HDC hDC);
public:
	//����ռ�õĸ߶ȺͿ����Ϣ
	SIZE ReCalcExtent(BlockMetaInfo pBlockMetaInfo[],int nLen);

public:
	//�������ݿ�Ĳ�����Ϣ
	SIZE CalcBlockMetaInfo(XWordDataSvr* pDataSvr,Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordBlock* pBlock,BlockMetaInfo * pBlockMetaInfo);
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
};
