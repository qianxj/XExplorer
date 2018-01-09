#pragma once
#include "xframe.h"
#include "xshape.h"

namespace Hxsoft{namespace XFrame
{
	class XCONTROL_API xcShapeIcon :
		public xfControl
	{
	public:
		xcShapeIcon(void);
		~xcShapeIcon(void);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * rc);
		virtual	void Initial();
	public:
		virtual bool EvtPaint(TEvent* pEvent,LPARAM lParam);
		virtual bool EvtLButtonDown(TEvent* pEvent,LPARAM lParam);
	public:
		int GetItemRect(int nIndex,RECT &rect);
	private:
		TCHAR * m_pStrShapeUri;
		XShapeObjectSet * m_pShapes;
	public:
		int m_nIndex;
	};
}}