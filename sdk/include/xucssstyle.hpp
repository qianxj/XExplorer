#pragma once
#include "xucsarray.hpp"
#include "xuvector.hpp"
#include <string>
#include <map>

namespace Hxsoft{ namespace XUtil
{
	namespace Css
	{
		#define cssMaskLeft		0x00000001
		#define cssMaskTop		0x00000002
		#define cssMaskRight	0x00000004
		#define cssMaskBottom	0x00000008

		#define cssMaskWidth	0x00000010
		#define cssMaskHeight	0x00000020

		#define ssMaskNone		0x00000000

		#define cssMaskColor	0x00010000
		#define cssMaskMargin	0x00080000
		#define cssMaskPadding	0x00100000

		#define cssMaskBackGround			0x00020000
		#define cssMaskBackGroundColor		0x00020010
		#define cssMaskBackGroundImage		0x00020020
		#define cssMaskBackGroundRepeat		0x00020040
		#define cssMaskBackGroundAttachment	0x00020080
		#define cssMaskBackGroundPosition	0x00020100

		#define cssMaskBorder		0x00040000
		#define cssMaskBorderType	0x00040010
		#define cssMaskBorderWidth	0x00040020
		#define cssMaskBorderColor	0x00040040

		#define cssMaskFont			0x00200000
		#define cssMaskFontFamily	0x00200010
		#define cssMaskFontFaceName	cssMaskFontFamily
		//#define cssMaskFontFaceName	0x00200020
		#define cssMaskFontStyle	0x00200040
		#define cssMaskFontVariant	0x00200080
		#define cssMaskFontWeight	0x00200100
		#define cssMaskFontSize		0x00200200
		#define cssMaskLineHeight	0x00200400

		#define cssMaskText				0x00400000
		#define cssMaskTextIndent		0x00400010
		#define cssMaskTextAlign		0x00400020
		#define cssMaskTextDecoration	0x00400040
		#define cssMaskLetterSpacing	0x00400080
		#define cssMaskWordSpacing		0x00400100
		#define cssMaskTextTransform	0x00400200
		#define cssMaskWhiteSpace		0x00400400

		enum Property
		{
			_undef,_refcolor,_refmeasure,_refvalue,_all,_none,_normal,_inherit,
			_cm,_mm,_pt,_px,_pc,_em,_ex,_en,_in,
			_hidden,_dotted,_dashed,_solid,_double,_groove,_ridge,_inset,_outset,
			_left,_top,_right,_bottom,_center,_justify,
			_transparent,
			_rgbcolor,_colorindex,_syscolor,
			_percentage,_amount,_auto,
			_scroll,_fixed,
			_repeat,_repeat_x,_repeat_y,_no_repeat,
			_uri,
			_thin,_medium,_thick,
			_serif,_sans_serif,_cursive,_fantasy,_monospace,
			_italic,_oblique,_small_caps,
			_bold ,_border ,_lighter,
			_xx_small,_x_small,_small,_large,_x_large,_xx_large,_larger,_smaller,
			_underline,_overline, _line_through, _blink,
			_capitalize,_uppercase ,_lowercase,
			_pre ,_nowrap ,_pre_wrap,_pre_line

		};
		typedef  Property Direct; //all|left|top|right|bottom
		typedef  Property BorderType; //none|hidden|dotted|dashed|solid|double|groove|ridge|inset|outset|inherit
		typedef  Property MeasureUnit;//cm|mm|pt|px|pc|em|ex|en|in
		typedef  Property Color; //none|refcolor|transparent|inherit
		typedef  Property ColorRef; //rgb|colorindex,syscolor,transparent
		typedef  Property Attachment; //scroll|fixed|inherit
		typedef  Property Repeat; //repeat|repeat_x|repeat_y|no_repeat|inherit
		typedef  Property Position; //none|refmeasure|left|top|right|bottom|center|inherit
		typedef Property FileRef; //uri|none|inherit
		typedef Property BorderWidth; //thin|medium|thick|refmeasure
		typedef Property FontFamily; //serif|sans_serif|cursive|fantasy|monospace
		typedef Property MeasureRef; //percentage|amount|auto
		typedef Property FontStyle; //normal|italic|oblique
		typedef Property FontVariant; //normal|small_caps
		typedef Property FontWeight; //	normal|bold|border|lighter|refmeasure|inherit
		typedef Property FontSize; //xx_small|x_small|small|medium|large|xx_large|x_large|larger|smaller|refmeasure|inherit

		typedef Property TextIndent; //<length> |<percentage> | inherit
		typedef Property TextAlign; //	left | right | center | justify | inherit
		typedef Property TextDecoration; //	none | [ underline || overline || line-through || blink ] | inherit
		typedef Property LetterSpacing;//normal | <length> | inherit
		typedef Property WordSpacing; //normal | <length> | inherit
		typedef Property TextTransform; //capitalize | uppercase | lowercase | none | inherit
		typedef Property WhiteSpace; //	normal | pre | nowrap | pre-wrap | pre-line | inherit

		typedef Property BackGround; //normal|inherit
		typedef Property Border; //normal|inherit
		typedef Property Margin; //normal|inherit
		typedef Property Padding; //normal|inherit
		typedef Property Font; //normal|inherit
		typedef Property Text; //normal|inherit

		struct XUTIL_API tagCssData
		{
			int nXPixelPerInch;
			int nYPixelPerInch;
			tagCssData();
		};

		struct tagCssItem
		{
			LPTSTR name;
			Property item;
		};

		typedef struct XUTIL_API tagMeasure
		{
			MeasureRef measureRef;
			double	amount;
			double percentage;
			MeasureUnit unit;
			tagMeasure::tagMeasure(){::ZeroMemory(this,sizeof(tagMeasure));}
		} TMeasure;

		struct tagMeasureUnit
		{
			LPTSTR name;
			Css::MeasureUnit	eUnit;
			int MulToPt;
			int DivToPt;
		};


		typedef struct tagColor
		{
			DWORD		mask;
			ColorRef	colorRef[5];
			COLORREF	color[5];
			USHORT		gamma[5];
			tagColor::tagColor(){::ZeroMemory(this,sizeof(tagColor));}
		} TColor;

		typedef struct tagFileRef
		{
			FileRef		fileRef;
			TCHAR		url[255];
			tagFileRef::tagFileRef(){::ZeroMemory(this,sizeof(tagFileRef));}
		} TFileRef;

		typedef struct tagBackGround
		{
			DWORD mask;
			TColor	color;
			TFileRef image;
			Repeat repeat;
			Attachment attachment;
			Position position;
			TMeasure positionmesure;
			BackGround background;
			tagBackGround::tagBackGround(){::ZeroMemory(this,sizeof(tagBackGround));}
		} TBackGround ;

		typedef struct tagBorder
		{
			DWORD mask;
			TColor	color;
			BorderWidth	width;
			TMeasure widthmesure;
			BorderType bordertype;
			Border border;
			tagBorder::tagBorder(){::ZeroMemory(this,sizeof(tagBorder));}
		}TBorder ;

		typedef TMeasure  TMargin;
		typedef TMeasure  TPadding;

		typedef struct tagFont
		{
			DWORD mask;
			FontFamily fontfamily;
			TCHAR	fontfacename[LF_FACESIZE];
			FontStyle fontstyle;
			FontVariant fontvariant;
			FontWeight	fontweight;
			int	fontweightvalue;
			FontSize fontsize;
			TMeasure fontsizemeasure;
			TMeasure lineheightmeasure;
			Font	font;
			tagFont::tagFont(){::ZeroMemory(this,sizeof(tagFont));}
		} TFont;

		typedef struct tagText
		{
			DWORD mask;
			Text text;
			TextIndent textindent;
			TMeasure textindentmeasure;
			TextAlign textalign;
			TextDecoration textdecoration;
			LetterSpacing letterspacing;
			TMeasure letterspacingmeasure;
			WordSpacing wordspacing;
			TMeasure wordspacingmeasure;
			TextTransform texttransform;
			WhiteSpace  whitespace;
			tagText::tagText(){::ZeroMemory(this,sizeof(tagText));}
		}TText;

		typedef struct XUTIL_API tagBoxStyle
		{
			DWORD mask;
			TColor *pColor;
			TText*	pText;
			TFont*	pFont;
			TBackGround * pBackGround;

			TBorder * pBorder[5];
			TMargin* pMargin[5];
			TPadding* pPadding[5];

			TMeasure* pWidth;
			TMeasure* pHeight;

			bool SharedProperty;

			tagBoxStyle(void);
			~tagBoxStyle(void);
			bool operator==(const tagBoxStyle &boxstyle);
			tagBoxStyle & operator=(const tagBoxStyle &boxstyle);
			tagBoxStyle & operator + (const tagBoxStyle &boxstyle);
			int Release(){delete this; return 1;}

		}TBoxStyle;

		bool IsSameStyle(const TBoxStyle & style1,const TBoxStyle & style2);
		bool StyleCopyTo(TBoxStyle * pDestStyle,const TBoxStyle * pSrcStyle);
		bool MergeStyle(TBoxStyle * pDestStyle,const TBoxStyle * pSrcStyle);

		inline bool TBoxStyle::operator==(const tagBoxStyle &boxstyle)
		{
			return IsSameStyle(*this, boxstyle);
		}
		inline tagBoxStyle & TBoxStyle::operator=(const tagBoxStyle &boxstyle)
		{
			StyleCopyTo(this,&boxstyle);
			return *this;
		}
		inline tagBoxStyle & TBoxStyle::operator+(const tagBoxStyle &boxstyle)
		{
			MergeStyle(this,&boxstyle);
			return *this;
		}

		DWORD GetCssMask(LPCTSTR pStrCssItem);
		bool XUTIL_API ProcessCssMargin(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle);
		bool XUTIL_API ProcessCssPadding(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle);
		bool XUTIL_API ProcessCssBorder(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle);
		bool XUTIL_API ProcessCssBackground(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle);
		bool XUTIL_API ProcessCssFont(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle);
		bool XUTIL_API ProcessCssText(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle);
		bool XUTIL_API ProcessCssMeasure(LPCTSTR pStrCssValue,TMeasure * pMeasure);
		bool XUTIL_API ProcessCssColor(LPCTSTR pStrCssValue, TColor &color);
		bool XUTIL_API ProcessCssColorEx(LPCTSTR pStr, Css::TColor &color,int nIndex);
		bool XUTIL_API ProcessCssBorder(LPCTSTR pStrCssValue,TBorder * pBorder);
		bool XUTIL_API ProcessCssBorderType(LPCTSTR pStrCssValue,TBorder * pBorder);
		bool XUTIL_API ProcessCssBorderWidth(LPCTSTR pStrCssValue,TBorder * pBorder);

		//Draw CssStyle
		bool XUTIL_API DrawCssFrame(HDC hPaintDC,RECT *pRect,const TBoxStyle* pCssStyle);

		XUTIL_API  int  GetDefaultFont(LOGFONT & logFont);
		XUTIL_API  int GetFont(const TBoxStyle * pBoxStyle,LOGFONT & logFont);
		XUTIL_API  int SetFont(TBoxStyle * pBoxStyle,LOGFONT & logFont);
		XUTIL_API  double	GetPixelExtent(TMeasure * pMeasure,bool bHorz = false);
		XUTIL_API  double	GetPtExtent(TMeasure * pMeasure,bool bHorz=false);
		XUTIL_API  double	GetBorderExtent(TBorder * pBorder,bool bHorz=false);
		
		typedef struct XUTIL_API tagCssClassItem
		{
			LPTSTR pStrClass;
			int	nIndex;
		}CssClassItem;
	}

	class XUTIL_API  xuCCssStyle 
	{
	public:
		typedef struct tagCellStyle
		{
			DWORD mask;
			int nFont;
			int nText;
			int nBackGround;
			int nMargin;
			int nPadding;
			int nColor;
			int nBorder;
			int nHeight;
			int nWidth;
			
           Css::TBoxStyle* m_pBoxStyle;    
		   tagCellStyle():nFont(-1),nText(-1),nBackGround(-1),nMargin(-1),nPadding(-1),nColor(-1),nBorder(-1),nWidth(-1),nHeight(-1)
		     {
                 m_pBoxStyle=NULL;
		     }
            ~tagCellStyle()
		     {
                 if (m_pBoxStyle) delete m_pBoxStyle;                
				 m_pBoxStyle=NULL;
		     }
		}TCssCellStyle;
		typedef struct tagTCssMargins
		{
			Css::TMargin * pMargin[5];
			tagTCssMargins()
			{
				for(int i=0;i<5;i++)pMargin[i]=NULL;
			}
			~tagTCssMargins()
			{
				for(int i=0;i<5;i++)if(pMargin[i]) delete pMargin[i];
			}
		}TCssMargins;
		typedef struct tagTCssPaddings
		{
			Css::TPadding * pPadding[5];
			tagTCssPaddings()
			{
				for(int i=0;i<5;i++)pPadding[i]=NULL;
			}
			~tagTCssPaddings()
			{
				for(int i=0;i<5;i++)if(pPadding[i]) delete pPadding[i];
			}
		}TCssPaddings;
		typedef struct tagTCssBorders
		{
			Css::TBorder * pBorder[5];
			tagTCssBorders()
			{
				for(int i=0;i<5;i++)pBorder[i]=NULL;
			}
			~tagTCssBorders()
			{
				for(int i=0;i<5;i++)if(pBorder[i]) delete pBorder[i];
			}
		}TCssBorders;

		xuVector<Css::TFont *> *m_pCssFont;
		xuVector<Css::TText *> *m_pCssText;
		xuVector<Css::TBackGround *> * m_pCssBackGround;
		xuVector<TCssMargins *> *  m_pCssMargin;
		xuVector<TCssPaddings *> * m_pCssPadding;
		xuVector<Css::TColor *> *   m_pCssColor;
		xuVector<TCssBorders * > * m_pCssBorder;
		xuVector<Css::TMeasure* > * m_pCssWidth;
		xuVector<Css::TMeasure* > * m_pCssHeight;
	public:
		xuCCssStyle(void);
		~xuCCssStyle(void);
	public:
		xuVector<TCssCellStyle *> *m_pCellStyle;
	public:
		const Css::TBoxStyle *  GetBoxCssStyleFromCssCellStyle(TCssCellStyle * pCellStyle);
		bool ReleaseCellCssStyle(Css::TBoxStyle * pBoxStyle);
	
		int AddCssFont(Css::TFont * F);
		int AddCssText(Css::TText * T);
		int AddCssBackGround( Css::TBackGround * B);
		int AddCssMargin(TCssMargins * M);
		int AddCssPadding(TCssPaddings * P);
		int AddCssColor(Css::TColor * C);
		int AddCssBorder(TCssBorders * B);
		
		int AddCssWidth(Css::TMeasure* w);
		int AddCssHeight(Css::TMeasure* h);


		int AddCssCellStyle( TCssCellStyle * pCellStyle);
		int AddCssCellStyle(const Css::TBoxStyle * pBoxStyle);

        static int GetCssStyles(LPCTSTR cssStyleStr,xuVector<LPTSTR> &cssStyles, xuVector<LPTSTR> &cssValues);
		static Css::TBoxStyle * ParseCssStyle(LPCTSTR cssStyleStr);
		const Css::TBoxStyle * GetBoxStyle(int nIndex);
	public:
		int SetStyleSheet(IXMLDOMElement * pElement,BSTR pTagName);
		int GetClassNameByIndex(int nIndex,LPTSTR className);
		std::wstring GetStylesString();
		void GetStyles(std::map<std::wstring,std::wstring> & items);

	};

	class XUTIL_API  xuCssStyleSheet
	{
	public:
		xuCssStyleSheet(void);
		~xuCssStyleSheet(void);
	public:
		XUtil::xuCCssStyle * m_pCssStyle;
		template class XUTIL_API xuVector<XUtil::Css::CssClassItem>;
		XUtil::xuVector<XUtil::Css::CssClassItem> m_CssClass;  
	public:
		int AddCssClass(LPTSTR className,LPTSTR cssStr);
		const Css::TBoxStyle* GetBoxStyle(int nIndex);
		int SetCssStyle(const Css::TBoxStyle* pBoxStyle);
		int GetBoxStyleIndexByClass(LPTSTR pStrClass);
	public:
		int ParseStyleSheet(const LPTSTR  pStyle);
		enum eCssItemType
		{
			etCssNormal = 0,
			etCssSelection=1,
			etCssHover=2
		};
		int GetClassStyle(LPCTSTR pStrName,LPCTSTR pStrID,LPCTSTR pCssClass=NULL,LPCTSTR pStyle=NULL,eCssItemType et=etCssNormal);
	};

}}
