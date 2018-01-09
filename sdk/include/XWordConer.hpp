#pragma once

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWordConer
	{
	public:
		XWordConer(void);
		~XWordConer(void);
	public:
		RECT FRect;
		int SetFRect(RECT &rc);
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	};
}}}}
