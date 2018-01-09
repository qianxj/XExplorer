#pragma once
#include <vector>
#include "XFlowItem.hpp"
#include "XWordSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	/**
		表示正常数据流排版的空间
	*/
	class XFlowPara;
	class XFlowChapter : public XFlowItem
	{
	public:
		XFlowChapter(void);
		~XFlowChapter(void);
	public:
		XWordDataChapter * m_pContent;
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
		int DoDraw(XWordDataSvr* pDataSvr,XFlowDocument* pDocument, XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
	public:
		XFlowPara  * GetParaAtPointer(XWordDataSvr* pDataSvr,POINT pt);
	public:
		int GetIndexByBlock(XWordDataBase *pBlock);
		XFlowPara  * GetParaByBlock(XWordDataSvr* pDataSvr,XWordDataBase *pBlock);
	};
}}}}