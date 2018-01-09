#pragma once

#include "xbxbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWordVRuler:
		public xbObject
	{
	public:
		XWordVRuler(void);
		~XWordVRuler(void);
	public:
		RECT FRect;
		int SetFRect(RECT &rc);
		static int m_nWidth;
	public:
		class XFlowDocument * m_pFlowDocument;
		class XWordLayer * m_pLayer;
		class XWordSheetSvr* m_pSheetSvr;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	};
}}}}
