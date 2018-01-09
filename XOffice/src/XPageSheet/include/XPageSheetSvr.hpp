#pragma once

#include "xofbase.h"
#include "xframe.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XPage
{
	class XPAGESHEET_API XPageSheetSvr :
		public XOfficeSheetSvr
	{
	public:
		XPageSheetSvr(void);
		~XPageSheetSvr(void);
	public:
		bool	m_ControlCreated;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		virtual int SetFRect(RECT rc);
		virtual int ShowSheet(bool bShow);
	public:
		bool EvtXCommand(TEvent *pEvent ,LPARAM lParam);

	};
}}}}
