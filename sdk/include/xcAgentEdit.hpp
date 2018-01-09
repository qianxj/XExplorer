#pragma once

#include "commctrl.h"
#include "xframe.h"
#include "vector"

namespace Hxsoft{ namespace XFrame
{
	class xcAgentEdit :
		public xfControl
	{
	public:
		xcAgentEdit(void);
		~xcAgentEdit(void);
	public:
		virtual void Initial(); 
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
	public:
		bool EvtChange(TEvent* pEvent,LPARAM lParam);
	private:
		class xcListWin * m_pListWin;
		int CalcListRect(int nWidth,int nHeight,RECT &rc);
	};
}}