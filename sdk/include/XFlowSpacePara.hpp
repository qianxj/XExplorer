#pragma once
#include <vector>
#include "XFlowSpaceItem.hpp"
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
	};

	/**
		表示正常数据流排版的空间
	*/
	class XFlowSpacePara : public XFlowSpaceItem
	{
	public:
		XFlowSpacePara(void);
		~XFlowSpacePara(void);
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
		virtual SIZE RecalcExtent(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC);
	public:
		//计算占用的高度和宽度信息
		SIZE ReCalcExtent(BlockLayerInfo pBlockLayerInfo[],int nLen);
	public:
		//计算数据块的布局信息
		SIZE CalcBlockLayerInfo(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle, HDC hDC,XWordDataSpanBlock* pBlock,BlockLayerInfo * pBlockLayerInfo);
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
		int DoDraw(XWordDataSvr* pDataSvr,XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,RECT * pDrawRect=NULL);
		int DoDraw(BlockLayerInfo pBlockLayerInfo[], int nLen, HDC hPaintDC);
	public:
		int DrawText(BlockLayerInfo * pBlockLayerInfo,RECT &rt,HDC hPaintDC,int nStart ,int nLen);
	public:
		bool CalcCursorInfoAtPoint(POINT pt,XWordCursorInfoEx & CursorInfo);
	};
}}}}