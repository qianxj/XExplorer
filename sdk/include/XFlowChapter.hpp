#pragma once
#include <vector>
#include "XFlowItem.hpp"
#include "XWordSheetSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	/**
		��ʾ�����������Ű�Ŀռ�
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
		int DoDraw(XWordDataSvr* pDataSvr,XFlowDocument* pDocument, XUtil::Css::TBoxStyle * pParentBoxStyle,HDC hPaintDC,POINT ptOffset, RECT * pDrawRect,RECT * pUpdateRect);
	public:
		XFlowPara  * GetParaAtPointer(XWordDataSvr* pDataSvr,POINT pt);
	public:
		int GetIndexByBlock(XWordDataBase *pBlock);
		XFlowPara  * GetParaByBlock(XWordDataSvr* pDataSvr,XWordDataBase *pBlock);
	};
}}}}