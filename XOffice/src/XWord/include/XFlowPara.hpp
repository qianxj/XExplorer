#pragma once
#include <vector>
#include "XFlowItem.hpp"
#include "XWordSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	//文本块布局信息
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
		表示正常数据流排版的空间
	*/
	class XFlowPara : public XFlowItem
	{
	public:
		XFlowPara(void);
		~XFlowPara(void);
	public:
		XWordDataContentParagraph * m_pContent;
	public:
		//显示流空间的内容
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	public:
		//获取设定高度和宽度信息
		virtual POINT GetExtent();
		//获取实际占用的高度和宽度信息
		virtual POINT GetRealExtent();
		//重新计算位置信息
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
		//计算占用的高度和宽度信息
		SIZE ReCalcExtent(BlockLayerInfo pBlockLayerInfo[],int nLen);
	public:
		//计算数据块的布局信息
		SIZE CalcBlockLayerInfo(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordDataTextBlock* pBlock,BlockLayerInfo * pBlockLayerInfo);
		//清除数据块布局对象组
		int  ClearLayerInfo(BlockLayerInfo pBlockLayerInfo[],int nCount);
	private:
		//得到相交的排除矩阵
		BOOL GetUnionExRect(std::vector<RECT> &vRect, RECT &rtBound);
		//计算右边可容纳矩阵的宽度，计算时需要排除ExRect
		int GetLayerRight(BlockLayerInfo pBlockLayerInfo[],int nHeight,int nMinWidth, POINT ptOffset);
		//计算右边可容纳矩阵的矩阵，计算时需要排除ExRect
		int GetLayerRightRects(std::vector<RECT> &rRect,BlockLayerInfo pBlockLayerInfo[],int nHeight,int nMinWidth, POINT ptOffset);
		//按照从左到右排序矩形组
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