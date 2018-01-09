#pragma once
#include <xframe.h>

namespace Hxsoft
{
	namespace XFrame
	{
		class XCONTROL_API xcFile:
			public xfControl
		{
		public:
			xcFile();
			~xcFile();
		public:
			bool EvtEraseBkgn(TEvent* pEvent,LPARAM lParam);
			bool EvtPaint(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseMove(TEvent* pEvent,LPARAM lParam);
			bool EvtMouseLeave(TEvent* pEvent,LPARAM lParam);
			bool EvtLButtonUp(TEvent* pEvent,LPARAM lParam);
			bool EvtTimer(TEvent* pEvent,LPARAM lParam);
			bool EvtSetFocus(TEvent* pEvent,LPARAM lParam);
			bool EvtKillFocus(TEvent* pEvent,LPARAM lParam);
			bool EvtKeyDown(TEvent* pEvent,LPARAM lParam);
		public:
			bool	m_bHover;
			LPTSTR	m_sPath;
			LPTSTR	m_sFolder;
			bool	m_bIsUpload;
			bool	m_bAutoUpload;
			int		m_sTxtWidth;
			bool	m_bFocusRect;
		public:
			int SetFilePath(LPTSTR szPath);
			int SetFileFolder(LPTSTR szFolder);

		public:
			enum EImageFlow{ImageFlow_Left,ImageFlow_Right,ImageFlow_Top,ImageFlow_Bottom,ImageFlow_None,
				ImageFlow_NoneText,ImageFlow_Background};
		public:
			virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		public:
			void Initial();
		public:
			xfNode * m_pAgentNodeTool;
		};
	}
}
