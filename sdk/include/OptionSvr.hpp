#pragma once
#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	struct 	DWNMHDR : public NMHDR
	{
		void * DwTable;
		int row;
		int col;
		LPTSTR data;
		int oldrow;
		int oldcol;
		LPTSTR colname;
		LPTSTR oldcolname;
	};

	struct 	CELLNMHDR : public NMHDR
	{
		int currentrow;
		int currentcol;
		LPTSTR data;
		int anchorrow;
		int anchorcol;
	};

	struct 	CELLTIPNMHDR : public NMHDR
	{
		int currentrow;
		int currentcol;
		int anchorrow;
		int anchorcol;
		int tipType;
		LPTSTR pTipStr;
	};

#define	DMS_INDENT				0x1
#define	DMS_IAMGERIGHTINDENT	( 0x1<<1)
#define	DMS_IMAGE			( 0x1<<2)
#define	DMS_CSSSTYLE		( 0x1<<3)
#define	DMS_CSSCLASS		( 0x1<<4)
#define	DMS_TEXT			( 0x1<<5)
#define	DMS_RECT			( 0x1<<6)

	struct 	DWASKSTYLENMHDR : public NMHDR
	{
		int row;
		int col;
		LPTSTR colname;
		int mask;
		int indent;
		int imagelist;
		int image;
		int cximage;
		int cyimage;
		int indent1;
		LPTSTR pCssStyle;
		LPTSTR pCssClass;
		RECT rt;
		LPTSTR pStr;
	};

	class XCELL_API COptionSvr :public CSvrbase
	{
	public:
		COptionSvr(void){};
		~COptionSvr(void){};
	public:
		static COLORREF stdTextColor;
		static int stdRowHeight;
		static int stdColumnWidth;
		static int stdColHeaderHeight;
		static int stdRowSelectorWidth;
		static COLORREF stdConerColor;
		static COLORREF stdBorderLineColor;
		static int stdGridLineWidth;

		static COLORREF stdColHeaderColor0;
		static COLORREF stdColHeaderColor1;
		static COLORREF stdRowSelecterColor0;
		static COLORREF stdRowSelecterColor1;

		static COLORREF stdGridColor0;
		static COLORREF stdGridColor1;
		static COLORREF stdGridColor2;
		static COLORREF stdGridColor3;
	};
}}}}
