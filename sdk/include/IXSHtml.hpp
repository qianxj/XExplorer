#pragma once

#include "xframe.h"
#include "xcImage.hpp"
#include "xcSHtml.hpp"
namespace Hxsoft{ namespace XFrame
{
	class XCONTROL_API IXSHtmlItem
	{
	public:
		LPCTSTR GetUrl(xcSHtml::htBlock*  pBlock);
		LPCTSTR GetData(xcSHtml::htBlock* pBlock);
		LPCTSTR GetData(xcSHtml::htBlock* pBlock, int nRow, int nCol);
		int GetRowColumn(xcSHtml::htBlock* pBlock, int &Row, int &Col);
		int RenameID(xcSHtml::htBlock* pBlock,LPTSTR ID);
		int SetData(xcSHtml::htBlock* pBlock,LPTSTR strText);

		int SetStyle(xcSHtml::htBlock* pBlock,LPTSTR cssStyle);
		int SetStyleClass(xcSHtml::htBlock* pBlock,LPTSTR cssStyleClass);

		LPCTSTR GetText(xcSHtml::htBlock* pBlock);
		int SetText(xcSHtml::htBlock* pBlock,LPTSTR strText);

		LPCTSTR GetID(xcSHtml::htBlock* pBlock);
		int SetID(xcSHtml::htBlock* pBlock,LPTSTR newID);
	};

	class XCONTROL_API IXSHtml : public IXFControl
	{
	public:
		IXSHtml(void);
		~IXSHtml(void);
	public:
		int SetContent(LPCTSTR pStr);
	public:
		int DrawHtml(HDC hPaintDC,LPTSTR pHtml,RECT &rt);
	public:
		int SetElementVisible(LPTSTR ID, bool bVisible);
	public:
		bool IsVisible(LPTSTR ID);
		bool SwitchState(LPTSTR ID);
	public:
		xcSHtml::htBlock* FoundBlockByID(LPTSTR ID);
		xcSHtml::htBlock* FoundBlockByID(xcSHtml::htBlock* pBlock, LPTSTR ID);
	};
}}