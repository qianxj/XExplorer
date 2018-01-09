#pragma once
#include <vector>
#include "XFlowSpaceItem.hpp"
#include "XWordSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	//�ı��鲼����Ϣ
	struct BlockLayerInfo
	{
		int * pDx;
		LPTSTR pStr;
		int nLenStr;
		XUtil::Css::TBoxStyle * pBoxStyle;
		BOOL bDeleteBoxStyle;
		SIZE ContentExtent;
	};

	/**
		��ʾ�����������Ű�Ŀռ�
	*/
	class XFlowSpacePara : public XFlowSpaceItem
	{
	public:
		XFlowSpacePara(void);
		~XFlowSpacePara(void);
	public:
		XWordDataContentParagraph * m_pContent;
	public:
		//��ʾ���ռ������
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		//��ȡ�趨�߶ȺͿ����Ϣ
		virtual POINT GetExtent();
		//��ȡʵ��ռ�õĸ߶ȺͿ����Ϣ
		virtual POINT GetRealExtent();
		//���¼���λ����Ϣ
		virtual SIZE RecalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC);
	public:
		//����ռ�õĸ߶ȺͿ����Ϣ
		SIZE ReCalcExtent(BlockLayerInfo pBlockLayerInfo[],int nLen);
	public:
		//�������ݿ�Ĳ�����Ϣ
		SIZE CalcBlockLayerInfo(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordDataSpanBlock* pBlock,BlockLayerInfo * pBlockLayerInfo);
		//������ݿ鲼�ֶ�����
		int  ClearLayerInfo(BlockLayerInfo pBlockLayerInfo[],int nCount);
	private:
		//�õ��ཻ���ų�����
		BOOL GetUnionExRect(std::vector<RECT> &vRect, RECT &rtBound);
		//�����ұ߿����ɾ���Ŀ�ȣ�����ʱ��Ҫ�ų�ExRect
		int GetLayerRight(BlockLayerInfo pBlockLayerInfo[],int nHeight,int nMinWidth, POINT ptOffset);
		//�����ұ߿����ɾ���ľ��󣬼���ʱ��Ҫ�ų�ExRect
		int GetLayerRightRects(std::vector<RECT> &rRect,BlockLayerInfo pBlockLayerInfo[],int nHeight,int nMinWidth, POINT ptOffset);
		//���մ��������������
		int SortRects(std::vector<RECT> &vRect);
	public:
		int DoDraw(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,RECT * pDrawRect=NULL);
		int DoDraw(BlockLayerInfo pBlockLayerInfo[], int nLen, HDC hPaintDC);
	public:
		int DrawText(BlockLayerInfo * pBlockLayerInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen);
	public:
		bool CalcCursorInfoAtPoint(POINT pt,XWordCursorInfoEx & CursorInfo);
	};
}}}}