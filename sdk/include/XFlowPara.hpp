#pragma once
#include <vector>
#include "XFlowItem.hpp"
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
		XWordDataBase * pBlock;
	};

	/**
		��ʾ�����������Ű�Ŀռ�
	*/
	class XFlowPara : public XFlowItem
	{
	public:
		XFlowPara(void);
		~XFlowPara(void);
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
		virtual SIZE CalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC);
	public:
		bool CalcCursorPositionAtPoint(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,POINT pt,XWordCursorInfoEx & CursorInfo);
		bool CalcCursorPositionAtPoint(BlockLayerInfo pBlockLayerInfo[],int nLen,POINT pt,XWordCursorInfoEx & CursorInfo);
		bool CalcCursorPositionAtPoint(BlockLayerInfo* pBlockLayerInfo,int nStart,int nBefore,int nPosLeft,int nPosTop,POINT pt,XWordCursorInfoEx & CursorInfo);
	public:
		bool CalcCursorPosition(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordDataTextBlock *pBlock,int nTextIndex,XWordCursorInfoEx & CursorInfo);
		bool CalcCursorPosition(BlockLayerInfo pBlockLayerInfo[],int nLen,XWordDataTextBlock *pBlock,int nTextIndex,XWordCursorInfoEx & CursorInfo);
		bool CalcCursorPosition(BlockLayerInfo* pBlockLayerInfo,int nStart, int nTextIndex, int nPosLeft,int nPosTop,XWordCursorInfoEx & CursorInfo);
	public:
		//����ռ�õĸ߶ȺͿ����Ϣ
		SIZE ReCalcExtent(BlockLayerInfo pBlockLayerInfo[],int nLen);
	public:
		//�������ݿ�Ĳ�����Ϣ
		SIZE CalcBlockLayerInfo(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordDataTextBlock* pBlock,BlockLayerInfo * pBlockLayerInfo);
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
		int DoDraw(XWordDataSvr* pDataSvr,XFlowDocument * pDocument,XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
		int DoDraw(XFlowDocument * pDocument,BlockLayerInfo pBlockLayerInfo[], int nLen, HDC hPaintDC);
	public:
		int DrawContent(XFlowDocument * pDocument, BlockLayerInfo * pBlockLayerInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen);
	public:
		int GetIndexByBlock(XWordDataBase *pBlock);
	public:
		XWordDataBase* GetControlBlockAtPoint(XWordDataSvr* pDataSvr,POINT pt);
	};
}}}}