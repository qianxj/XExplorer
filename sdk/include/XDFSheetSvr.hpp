#pragma once
#include "xofbase.h"
#include "xdfdatasvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	class XDF_API XDFSheetSvr :
		public XOffice::XOfficeSheetSvr
	{
	public:
		XDFSheetSvr(void);
		~XDFSheetSvr(void);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
		virtual int SetFRect(RECT rc);
	public:
		virtual void OnLButtonDown( UINT nFlags, POINT point ) ;
	};
}}}}
