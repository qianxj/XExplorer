#pragma once

#ifndef XGridDef_h__
#define XGridDef_h__

#define  EventSelectCell        1024
#define  EventSelectTempl       1025
#define  EventFind              1025
#define  EventReplace           1026
#define  EventdwCreate          1200
#define  EventbmpCreate         1201

#define  MaxListSize            32000
#define  MaxCustomExtents       MaxListSize
#define  MaxShortInt            32000

#define  False					FALSE
#define  True					TRUE

#define  TMessage               MSG
#define  POINTER                void *
#define  TPointer               POINTER
#define  Integer                int

#define  TBorderStyle           int
#define  TGridOptions           int //(goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine,
//    goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing, goRowMoving,
//    goColMoving, goEditing, goTabs, goRowSelect,
//    goAlwaysShowEditor, goThumbTracking,goDrawBound,goDrawFixed,goAutoSizeWidth,goAutoSizeHeight);

#define  TScrollStyle           int
#define  TRect                  RECT
#define  TGridState             int
#define  TCreateParams          int
#define  TShiftState            int 
#define  TMouseButton           int
#define  TColor                 COLORREF
#define  TPoint                 POINT
#define  TGridDrawState         TGridState      //set of (gdSelected, gdFocused, gdFixed);
#define  Cardinal               int

#define  TGridScrollDirection   int
#define  sdLeft                 0x0001
#define  sdRight                0x0002
#define  sdUp                   0x0004
#define  sdDown                 0x0008

#define  gdSelected             0x0001
#define  gdFocused              0x0002
#define  gdFixed                0x0004

#define  ssNone                 0
#define  ssHorizontal           1
#define  ssVertical             2
#define  ssBoth                 3

#define  ssCtrl                 0x0001
#define  ssShift                0x0008
#define  ssAlt                  0x0004

#define  clBtnFace              RGB(192,192,192)
#define  bsSingle               WS_BORDER
#define  clWindow               RGB(255,255,255)

#define  gsNormal       0x0001
#define  gsSelecting    0x0002
#define  gsRowSizing    0x0004
#define  gsColSizing    0x0008
#define  gsRowMoving    0x0010
#define  gsColMoving    0x0020
#define  gsDrawLining   0x0040
#define  gsDeleteLining 0x0080
#define  gsColsSizing	0x0100
#define  gsRowsSizing   0x0200
#define  gsFilling		0x0400
#define  gsDragColumn	0x0800
#define  gsEditing		0x1000

#define  goFixedVertLine        0x0001
#define  goFixedHorzLine        0x0002
#define  goVertLine             0x0004
#define  goHorzLine             0x0008
#define  goRangeSelect          0x0010
#define  goDrawFocusSelected    0x0020
#define  goRowSizing             0x0040
#define  goColSizing             0x0080
#define  goRowMoving             0x0100
#define  goColMoving             0x0200
#define  goEditing               0x0400
#define  goTabs                  0x0800
#define  goRowSelect             0x1000
#define  goAlwaysShowEditor      0x2000
#define  goAllowEdit		     0x4000
#define  goThumbTracking         0x8000
#define  goDrawBound	         0x10000
#define  goDrawFixed	         0x20000
#define  goAutoSizeWidth	     0x40000
#define  goAutoSizeHeight	     0x80000
#define  goDesignState		     0x100000
#define  goRunState			     0x200000
#define  goNormalState			 0x400000


#define ETEDIT		0x0000
#define ETCOMBO		0x0001
#define ETBUTTON	0x0002
#define ETCHECKBOX	0x0003
#define ETRADIO		0x0004
#define ETMASKEDIT	0x0005
#define ETDDDW		0x0006
#define ETDATE		0x0007
#define ETIAMGE		0x0008

struct  TGridCoord {
    int x;
    int y;
};

typedef  TRect  TXorRects[4] ;

TPoint  Point(int x,int y);

struct   TGridRect{
    union{
        struct {
        long left,top,right,bottom;
        };
        struct{
        TPoint TopLeft, BottomRight;
        };
    };
	TGridRect(){};
	TGridRect(TPoint aTopLeft,TPoint aBottomRight):TopLeft(aTopLeft),BottomRight(aBottomRight){};
	TGridRect(long aleft,long atop,long aright,long abottom):left(aleft),top(atop),right(aright),bottom(abottom){};
};


#define TGridDrawOption DWORD
#define gdoDrawSelection	0x0001
#define gdoDrawNote			0x0002
#define gdoDrawHeader		0x0010
#define gdoDrawRowSelector	0x0020
#define gdoDrawBand			0x0040
#define gdoDrawHScrollBar	0x1000
#define gdoDrawVScrollBar	0x2000
#define gdoDrawSheetPage	0x4000	
#define gdoDrawFillFlag		0x0100
#define gdoDrawFillRect		0x0200

#define gdoDrawSheetDefault	gdoDrawHeader|gdoDrawRowSelector
#define gdoDrawFrameDefault gdoDrawHScrollBar|gdoDrawVScrollBar|gdoDrawSheetPage
#define gdoDrawFillDefault  gdoDrawFillFlag|gdoDrawFillRect
#define gdoDrawGridDefault	gdoDrawSelection|gdoDrawNote
#define gdoDrawDefault		gdoDrawGridDefault|gdoDrawSheetDefault|gdoDrawFrameDefault|gdoDrawGridDefault

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XGridSvr;
	struct  TGridAxisDrawInfo1{
		int         EffectiveLineWidth;
		int         FixedBoundary;
		int         GridBoundary;
		int         GridExtent;
		int	        LastFullVisibleCell;
		int         FullVisBoundary;
		int         FixedCellCount;
		int         FirstGridCell;
		int         GridCellCount;
		int         (XGridSvr::* GetExtent)(int index);
	};
	 
	struct TGridDrawInfo1{
			TGridAxisDrawInfo1 Horz;
			TGridAxisDrawInfo1 Vert;
	};
}}}

#define ABSSCOL 0x01;
#define ABSSROW 0x02;
#define ABSECOL 0x04;
#define ABSEROW 0x08;

struct TRCellRect
{
	INT_PTR col;
	INT_PTR row;
	RECT	rcell;
	DWORD bAbs;
};

inline  BOOL PointInGridRect(int Col, int Row,  TGridRect Rect)
{
     return  (Col >= Rect.left) && (Col <= Rect.right) && (Row >= Rect.top)
            && (Row <= Rect.bottom);
};

inline BOOL PtInRect(int X, int Y ,TRect Rect)
  {
     return  (X >= Rect.left) && (X <= Rect.right) && (Y >= Rect.top) &&
      (Y <= Rect.bottom);
  };

inline int intMulDiv(int Mult1, int Mult2, int Div1)
{
        return MulDiv(Mult1,Mult2,Div1);
};

inline RECT GetRect(int left,int top,int right,int bottom)
{
    RECT        rect;
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    return rect;
};

bool RectIsNull(RECT &rect);
BOOL InflateRect(LPRECT pRect,int dx0,int dy0,int dx1,int dy1);
RECT NormalizeRect(RECT &rect);

inline bool RectIsNull(RECT &rect)
{
	return rect.left==0 && rect.right==0 && rect.top==0 && rect.bottom==0?true:false;
}

inline BOOL InflateRect(LPRECT pRect,int dx0,int dy0,int dx1,int dy1)
{
	pRect->left -= dx0;
	pRect->top  -= dy0;
	pRect->right  += dx1;
	pRect->bottom += dy1;
	return TRUE;
}

inline RECT NormalizeRect(RECT &rect)
{
	int nTemp;
	if (rect.left > rect.right)
	{
		nTemp = rect.left;
		rect.left = rect.right;
		rect.right = nTemp;
	}
	if (rect.top > rect.bottom)
	{
		nTemp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = nTemp;
	}
	return rect;
};

/*
int IMin(int A, int B);
int IMax(int A, int B);

int ReadAttrib(LPTSTR src,int istart,int  iend , LPTSTR * & pAttribs,LPTSTR * & pValues);
*/

//#endif

//mask flag
#define	STI_ENTRY		0x00000000
#define	STI_STRING		0x00000001
#define	STI_FORMULA		0x00000002
#define	STI_BORDER		0x00000004
#define	STI_FORMAT		0x00000008
#define	STI_NOTE		0x00000010
#define	STI_NAME		0x00000020
#define	STI_SPAN		0x00000040
#define	STI_FONTINDEX	0x00000080
#define	STI_OBJECT		0x00000100
#define	STI_TEXTCOLOR	0x00000200
#define	STI_TEXTBKCOLOR	0x00000400
#define	STI_BKCOLOR		0x00000800
#define	STI_ALIGN		0x00001000
#define	STI_RTF			0x00002000
#define	STI_AUTOWIDTH	0x00004000
#define	STI_AUTOHEIGHT	0x00008000
#define	STI_PRESTR		0x00010000
#define	STI_LASTPRE		0x00020000
#define	STI_LINK		0x00040000
#define	STI_XML			0x00080000
#define	STI_TABLE		0x00100000
#define	STI_ATTRIB		0x00200000
#define	STI_EDITTYPE	0x00400000
#define	STI_CELLTYPE	0x00800000

#define	STI_DATADEFXML	0X01000000
#define	STI_EDITDEFXML	0X02000000
#define	STI_SCRIPTXML	0X04000000

#define STI_STYLE  STI_BORDER|STI_FONTINDEX|STI_TEXTCOLOR|STI_TEXTBKCOLOR|STI_BKCOLOR|STI_ALIGN|STI_RTF|STI_AUTOWIDTH|STI_AUTOHEIGHT

#define BORDER_LEFT			0x0001
#define BORDER_TOP			0x0002
#define BORDER_RIGHT		0x0004
#define BORDER_BOTTOM		0x0008
#define BORDER_INNERVLINE	0x0010
#define BORDER_INNERHLINE	0x0020
#define BORDER_INNERLBLINE	0x0040
#define BORDER_INNERBRLINE	0x0080

#define BORDER_NONE			0x0000
#define BORDER_OUTER		(BORDER_LEFT|BORDER_TOP|BORDER_RIGHT|BORDER_BOTTOM)
#define BORDER_INNER		(BORDER_INNERVLINE|BORDER_INNERHLINE)
#define BORDER_ALL			(BORDER_OUTER|BORDER_INNER)
#endif
