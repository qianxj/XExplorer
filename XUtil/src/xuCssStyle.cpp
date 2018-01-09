#include "StdAfx.h"
#include "xucssstyle.hpp"
#include "shlwapi.h"
#include <string>
#include "xuXUtil.h"

namespace Hxsoft{ namespace XUtil
{
	XUTIL_API  Css::tagCssData 	_CssData;

	#define FindItemCssPropertyEx(x,y) FindItemCssProperty(x,sizeof(x)/sizeof(x[0]),y)
	Css::tagMeasureUnit MeasureUnits[] = 
	{
		{ _T("cm"),Css::_cm,7200,254  },  
		{ _T("em"),Css::_em,1,1  },
		{ _T("en"),Css::_en,1,1  },
		{ _T("ex"),Css::_ex,1,1 },
		{ _T("in"),Css::_in,72,1  },
		{ _T("mm"),Css::_mm,7200,2540  }, 
		{ _T("pc"),Css::_pc,1,12  },
		{ _T("pt"),Css::_pt,1,1  },
		{ _T("px"),Css::_px,1,1 }
	};

	Css::tagCssData::tagCssData()
	{
		HDC hdc = ::GetDC(NULL);
		nXPixelPerInch = GetDeviceCaps(hdc, LOGPIXELSX);
		nYPixelPerInch = GetDeviceCaps(hdc, LOGPIXELSY);
		::ReleaseDC(NULL,hdc);
	}

	struct tagColorNames
	{
		LPTSTR name;
		COLORREF	rgbValue;
	};

	const tagColorNames ColorNames[] =
	{
		{ _T("aliceblue"),              0xfff8f0 },
		{ _T("antiquewhite"),           0xd7ebfa },
		{ _T("aqua"),                   0xffff00 },
		{ _T("aquamarine"),             0xd4ff7f },
		{ _T("azure"),                  0xfffff0 },
		{ _T("beige"),                  0xdcf5f5 },
		{ _T("bisque"),                 0xc4e4ff },
		{ _T("black"),                  0x000000 },
		{ _T("blanchedalmond"),         0xcdebff },
		{ _T("blue"),                   0xff0000 },
		{ _T("blueviolet"),             0xe22b8a },
		{ _T("brown"),                  0x2a2aa5 },
		{ _T("burlywood"),              0x87b8de },
		{ _T("cadetblue"),              0xa09e5f },
		{ _T("chartreuse"),             0x00ff7f },
		{ _T("chocolate"),              0x1e69d2 },
		{ _T("coral"),                  0x507fff },
		{ _T("cornflowerblue"),         0xed9564 },
		{ _T("cornsilk"),               0xdcf8ff },
		{ _T("crimson"),                0x3c14dc },
		{ _T("cyan"),                   0xffff00 },
		{ _T("darkblue"),               0x8b0000 },
		{ _T("darkcyan"),               0x8b8b00 },
		{ _T("darkgoldenrod"),          0x0b86b8 },
		{ _T("darkgray"),               0xa9a9a9 },
		{ _T("darkgreen"),              0x006400 },
		{ _T("darkkhaki"),              0x6bb7bd },
		{ _T("darkmagenta"),            0x8b008b },
		{ _T("darkolivegreen"),         0x2f6b55 },
		{ _T("darkorange"),             0x008cff },
		{ _T("darkorchid"),             0xcc3299 },
		{ _T("darkred"),                0x00008b },
		{ _T("darksalmon"),             0x7a96e9 },
		{ _T("darkseagreen"),           0x8fbc8f },
		{ _T("darkslateblue"),          0x8b3d48 },
		{ _T("darkslategray"),          0x4f4f2f },
		{ _T("darkturquoise"),          0xd1ce00 },
		{ _T("darkviolet"),             0xd30094 },
		{ _T("deeppink"),               0x9314ff },
		{ _T("deepskyblue"),            0xffbf00 },
		{ _T("dimgray"),                0x696969 },
		{ _T("dodgerblue"),             0xff901e },
		{ _T("firebrick"),              0x2222b2 },
		{ _T("floralwhite"),            0xf0faff },
		{ _T("forestgreen"),            0x228b22 },
		{ _T("fuchsia"),                0xff00ff },
		{ _T("gainsboro"),              0xdcdcdc },
		{ _T("ghostwhite"),             0xfff8f8 },
		{ _T("gold"),                   0x00d7ff },
		{ _T("goldenrod"),              0x20a5da },
		{ _T("gray"),                   0x808080 },
		{ _T("green"),                  0x008000 },
		{ _T("greenyellow"),            0x2fffad },
		{ _T("honeydew"),               0xf0fff0 },
		{ _T("hotpink"),                0xb469ff },
		{ _T("indianred"),              0x5c5ccd },
		{ _T("indigo"),                 0x82004b },
		{ _T("ivory"),                  0xf0ffff },
		{ _T("khaki"),                  0x8ce6f0 },
		{ _T("lavender"),               0xfae6e6 },
		{ _T("lavenderblush"),          0xf5f0ff },
		{ _T("lawngreen"),              0x00fc7c },
		{ _T("lemonchiffon"),           0xcdfaff },
		{ _T("lightblue"),              0xe6d8ad },
		{ _T("lightcoral"),             0x8080f0 },
		{ _T("lightcyan"),              0xffffe0 },
		{ _T("lightgoldenrodyellow"),   0xd2fafa },
		{ _T("lightgreen"),             0x90ee90 },
		{ _T("lightgrey"),              0xd3d3d3 },
		{ _T("lightpink"),              0xc1b6ff },
		{ _T("lightsalmon"),            0x7aa0ff },
		{ _T("lightseagreen"),          0xaab220 },
		{ _T("lightskyblue"),           0xface87 },
		{ _T("lightslategray"),         0x998877 },
		{ _T("lightsteelblue"),         0xdec4b0 },
		{ _T("lightyellow"),            0xe0ffff },
		{ _T("lime"),                   0x00ff00 },
		{ _T("limegreen"),              0x32cd32 },
		{ _T("linen"),                  0xe6f0fa },
		{ _T("magenta"),                0xff00ff },
		{ _T("maroon"),                 0x000080 },
		{ _T("mediumaquamarine"),       0xaacd66 },
		{ _T("mediumblue"),             0xcd0000 },
		{ _T("mediumorchid"),           0xd355ba },
		{ _T("mediumpurple"),           0xdb7093 },
		{ _T("mediumseagreen"),         0x71b33c },
		{ _T("mediumslateblue"),        0xee687b },
		{ _T("mediumspringgreen"),      0x9afa00 },
		{ _T("mediumturquoise"),        0xccd148 },
		{ _T("mediumvioletred"),        0x8515c7 },
		{ _T("midnightblue"),           0x701919 },
		{ _T("mintcream"),              0xfafff5 },
		{ _T("mistyrose"),              0xe1e4ff },
		{ _T("moccasin"),               0xb5e4ff },
		{ _T("navajowhite"),            0xaddeff },
		{ _T("navy"),                   0x800000 },
		{ _T("oldlace"),                0xe6f5fd },
		{ _T("olive"),                  0x008080 },
		{ _T("olivedrab"),              0x238e6b },
		{ _T("orange"),                 0x00a5ff },
		{ _T("orangered"),              0x0045ff },
		{ _T("orchid"),                 0xd670da },
		{ _T("palegoldenrod"),          0xaae8ee },
		{ _T("palegreen"),              0x98fb98 },
		{ _T("paleturquoise"),          0xeeeeaf },
		{ _T("palevioletred"),          0x9370db },
		{ _T("papayawhip"),             0xd5efff },
		{ _T("peachpuff"),              0xb9daff },
		{ _T("peru"),                   0x3f85cd },
		{ _T("pink"),                   0xcbc0ff },
		{ _T("plum"),                   0xdda0dd },
		{ _T("powderblue"),             0xe6e0b0 },
		{ _T("purple"),                 0x800080 },
		{ _T("red"),                    0x0000ff },
		{ _T("rosybrown"),              0x8f8fbc },
		{ _T("royalblue"),              0xe16941 },
		{ _T("saddlebrown"),            0x13458b },
		{ _T("salmon"),                 0x7280fa },
		{ _T("sandybrown"),             0x60a4f4 },
		{ _T("seagreen"),               0x578b2e },
		{ _T("seashell"),               0xeef5ff },
		{ _T("sienna"),                 0x2d52a0 },
		{ _T("silver"),                 0xc0c0c0 },
		{ _T("skyblue"),                0xebce87 },
		{ _T("slateblue"),              0xcd5a6a },
		{ _T("slategray"),              0x908070 },
		{ _T("snow"),                   0xfafaff },
		{ _T("springgreen"),            0x7fff00 },
		{ _T("steelblue"),              0xb48246 },
		{ _T("tan"),                    0x8cb4d2 },
		{ _T("teal"),                   0x808000 },
		{ _T("thistle"),                0xd8bfd8 },
		{ _T("tomato"),                 0x4763ff },
		{ _T("turquoise"),              0xd0e040 },
		{ _T("violet"),                 0xee82ee },
		{ _T("wheat"),                  0xb3def5 },
		{ _T("white"),                  0xffffff },
		{ _T("whitesmoke"),             0xf5f5f5 },
		{ _T("yellow"),                 0x00ffff },
		{ _T("yellowgreen"),            0x32cd9a }
	};

	static  tagColorNames SystemColors[] =
	{
		{ _T("activeborder"),       COLOR_ACTIVEBORDER},    
		{ _T("activecaption"),      COLOR_ACTIVECAPTION},   
		{ _T("appworkspace"),       COLOR_APPWORKSPACE},    
		{ _T("background"),         COLOR_BACKGROUND},      
		{ _T("buttonface"),         COLOR_BTNFACE},         
		{ _T("buttonhighlight"),    COLOR_BTNHIGHLIGHT},    
		{ _T("buttonshadow"),       COLOR_BTNSHADOW},       
		{ _T("buttontext"),         COLOR_BTNTEXT},         
		{ _T("captiontext"),        COLOR_CAPTIONTEXT},     
		{ _T("graytext"),           COLOR_GRAYTEXT},        
		{ _T("highlight"),          COLOR_HIGHLIGHT},       
		{ _T("highlighttext"),      COLOR_HIGHLIGHTTEXT},   
		{ _T("inactiveborder"),     COLOR_INACTIVEBORDER},  
		{ _T("inactivecaption"),    COLOR_INACTIVECAPTION}, 
		{ _T("inactivecaptiontext"),COLOR_INACTIVECAPTIONTEXT}, 
		{ _T("infobackground"),     COLOR_INFOBK},          
		{ _T("infotext"),           COLOR_INFOTEXT},        
		{ _T("menu"),               COLOR_MENU},            
		{ _T("menutext"),           COLOR_MENUTEXT},        
		{ _T("scrollbar"),          COLOR_SCROLLBAR},       
		{ _T("threeddarkshadow"),   COLOR_3DDKSHADOW },     
		{ _T("threedface"),         COLOR_3DFACE},
		{ _T("threedhighlight"),    COLOR_3DHIGHLIGHT},     
		{ _T("threedlightshadow"),  COLOR_3DLIGHT},         
		{ _T("threedshadow"),       COLOR_3DSHADOW},        
		{ _T("window"),             COLOR_WINDOW},          
		{ _T("windowframe"),        COLOR_WINDOWFRAME},     
		{ _T("windowtext"),         COLOR_WINDOWTEXT},      
	};

	int CompareCssItem( const void * p1, const void * p2 )
	{
		return ::_tcsicmp( ((Css::tagCssItem *)p1)->name,
			((Css::tagCssItem *)p2)->name );
	}

	Css::Property FindItemCssProperty( Css::tagCssItem CssItems[],int nSize,LPCTSTR pItem)
	{
		Css::tagCssItem CssItem;
		CssItem.name = (LPTSTR)pItem;
		Css::tagCssItem * pCssItem = (Css::tagCssItem *)bsearch( &CssItem,
			CssItems,
			nSize,
			sizeof(Css::tagCssItem),
			CompareCssItem );

		if(pCssItem) return pCssItem->item;
		return Css::_undef;
	}

	int CompareColorName( const void * p1, const void * p2 )
	{
		return ::_tcsicmp( ((tagColorNames *)p1)->name,
			((tagColorNames *)p2)->name );
	}


	bool Css::ProcessCssColorEx(LPCTSTR pStr, Css::TColor &color,int nIndex)
	{
		if(::_tcsicmp(pStr,_T("none"))==0)
		{
			color.colorRef[nIndex] =Css::_undef;
			return true;
		}
		if( !pStr || pStr[0]=='\0' )
			return false;

		LPCTSTR pcszColour = pStr;
		if( *pcszColour == '#' )
		{
			pcszColour++;

			const int nLength = ::lstrlen(pStr) - 1;
			int nBytesPerColour = min( nLength / 3, 2 );
			int rgb[3] = { 0, 0, 0 };
			for( int nColour = 0; nColour < 3; nColour++ )
			{
				int val = 0;
				for( int nByte = 0; nByte < nBytesPerColour; nByte++ )
				{
					int c = 0;
					if( *pcszColour )
					{
						c = tolower( (TCHAR) *pcszColour );
						if( (c >= '0') && (c <= '9'))
						{
							c = c - '0';
						}
						else if( (c >= 'a') && ( c <= 'f') )
						{
							c = c - 'a' + 10;
						}
						else
						{
							c = 0;
						}
						val = (val << 4) + c;
						pcszColour++;
					}
				}
				rgb[ nColour ] = val;
			}
			color.colorRef[nIndex]=Css::_rgbcolor;
			color.color[nIndex] = RGB( rgb[0], rgb[1], rgb[2] );
			color.gamma[nIndex]=0xFF;
			return true;
		}else
		{
			tagColorNames ColorName;
			ColorName.name = (LPTSTR)pStr;

			tagColorNames * pColorName = (tagColorNames *)bsearch( &ColorName,
													  ColorNames,
													  sizeof(ColorNames)/sizeof(ColorNames[0]),
													  sizeof(tagColorNames),
													  CompareColorName );
			if(pColorName)
			{
				color.colorRef[nIndex]=Css::_colorindex;
				color.color[nIndex] = pColorName->rgbValue;
				color.gamma[nIndex]=0xFF;
				return true;
			}else
			{
				pColorName = (tagColorNames *)bsearch( &ColorName,
													  SystemColors,
													  sizeof(SystemColors)/sizeof(SystemColors[0]),
													  sizeof(tagColorNames),
													  CompareColorName );
				if(pColorName)
				{
					color.colorRef[nIndex]=Css::_syscolor;
					color.color[nIndex] = pColorName->rgbValue;
					color.gamma[nIndex]=0xFF;
					return true;
				}else if(::_tcsicmp(pStr,_T("transparent"))==0)
				{
					color.colorRef[nIndex]=Css::_transparent;
					color.color[nIndex] = pColorName->rgbValue;
				}
				else
					return false;
			}
		}
		return false;
	}
	bool Css::ProcessCssColor(LPCTSTR pStr, TColor &color)
	{
		vector<LPTSTR> vals;
		GetTokens(pStr,_T(" "),vals);
		if(vals.size()==1)
		{
			color.mask = cssMaskColor;
			return ProcessCssColorEx(vals[0],color,0);

		}else if(vals.size()==2 || vals.size()==3) //top right
		{
			color.mask |= cssMaskLeft;
			color.mask |= cssMaskRight;
			ProcessCssColorEx(vals[1],color,1);
			ProcessCssColorEx(vals[1],color,3);

			color.mask |= cssMaskTop;
			color.mask |= cssMaskBottom;
			ProcessCssColorEx(vals[0],color,2);
			ProcessCssColorEx(vals[0],color,4);
			return true;
		}else if(vals.size()>=4) //top right bottom left
		{
			color.mask |= cssMaskLeft;
			color.mask |= cssMaskRight;
			ProcessCssColorEx(vals[1],color,1);
			ProcessCssColorEx(vals[3],color,3);

			color.mask |= cssMaskTop;
			color.mask |= cssMaskBottom;
			ProcessCssColorEx(vals[0],color,2);
			ProcessCssColorEx(vals[2],color,4);
			return true;
		}else
			return false;
	}

	int CompareUnitName( const void * p1, const void * p2 )
	{
		return ::_tcsicmp( ((Css::tagMeasureUnit *)p1)->name,
			((Css::tagMeasureUnit *)p2)->name );
	}

	bool Css::ProcessCssMeasure(LPCTSTR pStrCss,TMeasure * pMeasure)
	{
		if(::_tcsicmp(pStrCss,_T("auto"))==0)
		{
			pMeasure->measureRef=Css::_auto;
			return true;
		}
		double value=0;
		TCHAR *endptr;
		value = ::_tcstod(pStrCss,&endptr);
		if(endptr==pStrCss)
		{
			pMeasure->measureRef = Css::_none;
			return false;
		}
		if(endptr && endptr[0]=='%')
		{
			pMeasure->measureRef = Css::_percentage;
			pMeasure->percentage = value;
			return true;
		}
		if(!endptr ||endptr[0]=='\0')
		{
			pMeasure->amount = value;
			pMeasure->unit = Css::_px;
			pMeasure->measureRef = Css::_amount;
			return true;
		}
		pMeasure->amount = value;
		pMeasure->measureRef = Css::_amount;

		tagMeasureUnit UnitName;
		UnitName.name = endptr;

		tagMeasureUnit * pUnitName = (tagMeasureUnit *)bsearch( &UnitName,
												  MeasureUnits,
												  sizeof(MeasureUnits)/sizeof(MeasureUnits[0]),
												  sizeof(tagMeasureUnit),
												  CompareUnitName );
		if(pUnitName)
			pMeasure->unit = pUnitName->eUnit;
		else
			pMeasure->unit = Css::_px;
		return true;
	}


	struct CssMaskItem
	{
		std::map<wstring,DWORD> mapMaskItem;
		CssMaskItem(void)
		{
			//color
			mapMaskItem[_T("color")]= cssMaskColor;

			mapMaskItem[_T("margin")]= cssMaskMargin;
			mapMaskItem[_T("margin-left")]= cssMaskMargin|cssMaskLeft;
			mapMaskItem[_T("margin-top")]= cssMaskMargin|cssMaskTop;
			mapMaskItem[_T("margin-right")]= cssMaskMargin|cssMaskRight;
			mapMaskItem[_T("margin-bottom")]= cssMaskMargin|cssMaskBottom;

			mapMaskItem[_T("padding")]= cssMaskPadding;
			mapMaskItem[_T("padding-left")]= cssMaskPadding|cssMaskLeft;
			mapMaskItem[_T("padding-top")]= cssMaskPadding|cssMaskTop;
			mapMaskItem[_T("padding-right")]= cssMaskPadding|cssMaskRight;
			mapMaskItem[_T("padding-bottom")]= cssMaskPadding|cssMaskBottom;

			mapMaskItem[_T("background")]= cssMaskBackGround;
			mapMaskItem[_T("background-color")]= cssMaskBackGroundColor;
			mapMaskItem[_T("background-image")]= cssMaskBackGroundImage;
			mapMaskItem[_T("background-repeat")]= cssMaskBackGroundRepeat;
			mapMaskItem[_T("background-attachment")]= cssMaskBackGroundAttachment;
			mapMaskItem[_T("background-position")]= cssMaskBackGroundPosition;

			mapMaskItem[_T("width")]= cssMaskWidth;
			mapMaskItem[_T("height")]= cssMaskHeight;

			mapMaskItem[_T("border")]= cssMaskBorder;
			mapMaskItem[_T("border-style")]= cssMaskBorderType;
			mapMaskItem[_T("border-width")]= cssMaskBorderWidth;
			mapMaskItem[_T("border-color")]= cssMaskBorderColor;

			mapMaskItem[_T("border-left")]= cssMaskBorder|cssMaskLeft;
			mapMaskItem[_T("border-left-style")]= cssMaskBorderType|cssMaskLeft;
			mapMaskItem[_T("border-left-width")]= cssMaskBorderWidth|cssMaskLeft;
			mapMaskItem[_T("border-left-color")]= cssMaskBorderColor|cssMaskLeft;

			mapMaskItem[_T("border-top")]= cssMaskBorder|cssMaskTop;
			mapMaskItem[_T("border-top-style")]= cssMaskBorderType|cssMaskTop;
			mapMaskItem[_T("border-top-width")]= cssMaskBorderWidth|cssMaskTop;
			mapMaskItem[_T("border-top-color")]= cssMaskBorderColor|cssMaskTop;

			mapMaskItem[_T("border-right")]= cssMaskBorder|cssMaskRight;
			mapMaskItem[_T("border-right-style")]= cssMaskBorderType|cssMaskRight;
			mapMaskItem[_T("border-right-width")]= cssMaskBorderWidth|cssMaskRight;
			mapMaskItem[_T("border-right-color")]= cssMaskBorderColor|cssMaskRight;

			mapMaskItem[_T("border-bottom")]= cssMaskBorder|cssMaskBottom;
			mapMaskItem[_T("border-bottom-style")]= cssMaskBorderType|cssMaskBottom;
			mapMaskItem[_T("border-bottom-width")]= cssMaskBorderWidth|cssMaskBottom;
			mapMaskItem[_T("border-bottom-color")]= cssMaskBorderColor|cssMaskBottom;

			mapMaskItem[_T("font")]= cssMaskFont;
			mapMaskItem[_T("font-family")]= cssMaskFontFamily;
			mapMaskItem[_T("font-facename")]= cssMaskFontFaceName;
			mapMaskItem[_T("font-style")]= cssMaskFontStyle;
			mapMaskItem[_T("font-variant")]= cssMaskFontVariant;
			mapMaskItem[_T("font-weight")]= cssMaskFontWeight;
			mapMaskItem[_T("font-size")]= cssMaskFontSize;
			mapMaskItem[_T("line-height")]= cssMaskLineHeight;

			mapMaskItem[_T("text")]= cssMaskText;
			mapMaskItem[_T("text-indent")]= cssMaskTextIndent;
			mapMaskItem[_T("text-align")]= cssMaskTextAlign;
			mapMaskItem[_T("text-decoration")]= cssMaskTextDecoration;
			mapMaskItem[_T("letter-spacing")]= cssMaskLetterSpacing;
			mapMaskItem[_T("word-spacing")]= cssMaskWordSpacing;
			mapMaskItem[_T("text-transform")]= cssMaskTextTransform;
			mapMaskItem[_T("white-space")]= cssMaskWhiteSpace;
		}
		~CssMaskItem(void)
		{
			mapMaskItem.clear();
		}
	};

	static CssMaskItem g_CssMaskItem;

	DWORD Css::GetCssMask(LPCTSTR pStrCssItem)
	{
		std::map<wstring,DWORD>::const_iterator iterator ;
		iterator = g_CssMaskItem.mapMaskItem.find(pStrCssItem);
		if(iterator != g_CssMaskItem.mapMaskItem.end())
		{
			return iterator->second;
		}else
			return ssMaskNone;
	}

	bool Css::ProcessCssMargin(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle)
	{
		if(mask & cssMaskMargin)
		{
			mask &= ~cssMaskMargin;
			pStyle->mask |= cssMaskMargin;
			if(mask & cssMaskLeft)
			{
				if(!pStyle->pMargin[1]) pStyle->pMargin[1] = new TMargin;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pMargin[1]);

			}else if(mask & cssMaskRight)
			{
				if(!pStyle->pMargin[3]) pStyle->pMargin[3] = new TMargin;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pMargin[3]);
			}else if(mask & cssMaskTop)
			{
				if(!pStyle->pMargin[2]) pStyle->pMargin[2] = new TMargin;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pMargin[2]);

			}else if(mask & cssMaskBottom)
			{
				if(!pStyle->pMargin[4]) pStyle->pMargin[4] = new TMargin;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pMargin[4]);
			}
			else
			{
				vector<LPTSTR> vals;
				GetTokens(pStrCssValue,_T(" "),vals);
				if(vals.size()==1)
				{
					if(!pStyle->pMargin[0]) pStyle->pMargin[0] = new TMargin;
					Css::ProcessCssMeasure(vals[0], pStyle->pMargin[0]);
				}else if(vals.size()==2) 
				{
					if(!pStyle->pMargin[2]) pStyle->pMargin[2] = new TMargin;
					Css::ProcessCssMeasure(vals[0], pStyle->pMargin[2]);
					if(!pStyle->pMargin[3]) pStyle->pMargin[3] = new TMargin;
					Css::ProcessCssMeasure(vals[1], pStyle->pMargin[3]);
					if(!pStyle->pMargin[4]) pStyle->pMargin[4] = new TMargin;
					Css::ProcessCssMeasure(vals[0], pStyle->pMargin[4]);
					if(!pStyle->pMargin[1]) pStyle->pMargin[1] = new TMargin;
					Css::ProcessCssMeasure(vals[1], pStyle->pMargin[1]);

				}else if(vals.size()==4)
				{
					if(!pStyle->pMargin[2]) pStyle->pMargin[2] = new TMargin;
					Css::ProcessCssMeasure(vals[0], pStyle->pMargin[2]);
					if(!pStyle->pMargin[3]) pStyle->pMargin[3] = new TMargin;
					Css::ProcessCssMeasure(vals[1], pStyle->pMargin[3]);
					if(!pStyle->pMargin[4]) pStyle->pMargin[4] = new TMargin;
					Css::ProcessCssMeasure(vals[2], pStyle->pMargin[4]);
					if(!pStyle->pMargin[1]) pStyle->pMargin[1] = new TMargin;
					Css::ProcessCssMeasure(vals[3], pStyle->pMargin[1]);
				}
			}
		}
		return true;
	}
	bool Css::ProcessCssPadding(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle)
	{
		if(mask & cssMaskPadding)
		{
			mask &= ~cssMaskPadding;
			pStyle->mask |= cssMaskPadding;
			if(mask & cssMaskLeft)
			{
				if(!pStyle->pPadding[1]) pStyle->pPadding[1] = new TPadding;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pPadding[1]);

			}else if(mask & cssMaskRight)
			{
				if(!pStyle->pPadding[3]) pStyle->pPadding[3] = new TPadding;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pPadding[3]);
			}else if(mask & cssMaskTop)
			{
				if(!pStyle->pPadding[2]) pStyle->pPadding[2] = new TPadding;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pPadding[2]);

			}else if(mask & cssMaskBottom)
			{
				if(!pStyle->pPadding[4]) pStyle->pPadding[4] = new TPadding;
				Css::ProcessCssMeasure(pStrCssValue, pStyle->pPadding[4]);
			}
			else
			{
				vector<LPTSTR> vals;
				GetTokens(pStrCssValue,_T(" "),vals);
				if(vals.size()==1)
				{
					if(!pStyle->pPadding[0]) pStyle->pPadding[0] = new TPadding;
					Css::ProcessCssMeasure(vals[0], pStyle->pPadding[0]);
				}else if(vals.size()==2) 
				{
					if(!pStyle->pPadding[2]) pStyle->pPadding[2] = new TPadding;
					Css::ProcessCssMeasure(vals[0], pStyle->pPadding[2]);
					if(!pStyle->pPadding[3]) pStyle->pPadding[3] = new TPadding;
					Css::ProcessCssMeasure(vals[1], pStyle->pPadding[3]);
					if(!pStyle->pPadding[4]) pStyle->pPadding[4] = new TPadding;
					Css::ProcessCssMeasure(vals[0], pStyle->pPadding[4]);
					if(!pStyle->pPadding[1]) pStyle->pPadding[1] = new TPadding;
					Css::ProcessCssMeasure(vals[1], pStyle->pPadding[1]);

				}else if(vals.size()==4)
				{
					if(!pStyle->pPadding[2]) pStyle->pPadding[2] = new TPadding;
					Css::ProcessCssMeasure(vals[0], pStyle->pPadding[2]);
					if(!pStyle->pPadding[3]) pStyle->pPadding[3] = new TPadding;
					Css::ProcessCssMeasure(vals[1], pStyle->pPadding[3]);
					if(!pStyle->pPadding[4]) pStyle->pPadding[4] = new TPadding;
					Css::ProcessCssMeasure(vals[2], pStyle->pPadding[4]);
					if(!pStyle->pPadding[1]) pStyle->pPadding[1] = new TPadding;
					Css::ProcessCssMeasure(vals[3], pStyle->pPadding[1]);
				}
			}
		}
		return true;
	}

	Css::tagCssItem	CssBorderTypes[]={
		{_T("dashed"), Css::_dashed},
		{_T("double"), Css::_double},
		{_T("dotted"), Css::_dotted},
		{_T("groove"), Css::_groove},
		{_T("hidden"), Css::_hidden},
		{_T("inherit"), Css::_inherit},
		{_T("inset"), Css::_inset},
		{_T("none"), Css::_none},
		{_T("outset"), Css::_outset},
		{_T("ridge"), Css::_ridge},
		{_T("solid"), Css::_solid}
	};
	Css::tagCssItem	CssBorderWidths[]={
		{_T("medium"), Css::_medium},
		{_T("thick"), Css::_thick},
		{_T("thin"), Css::_thin}
	};

	bool Css::ProcessCssBorderType(LPCTSTR pStrCssValue,TBorder * pBorder)
	{
		tagCssItem ItemName;
		tagCssItem * pItemName;
		ItemName.name = (LPTSTR)pStrCssValue;
		pItemName = (tagCssItem *)bsearch( &ItemName,
			CssBorderTypes,
			sizeof(CssBorderTypes)/sizeof(CssBorderTypes[0]),
			sizeof(tagCssItem),
			CompareCssItem );
		if(pItemName)
		{
			//is border type
			pBorder->mask |= cssMaskBorderType;
			pBorder->bordertype = pItemName->item;
			return true;
		}
		return false;
	}

	bool Css::ProcessCssBorderWidth(LPCTSTR pStrCssValue,TBorder * pBorder)
	{
		tagCssItem ItemName;
		tagCssItem * pItemName;
		ItemName.name = (LPTSTR)pStrCssValue;
		pItemName = (tagCssItem *)bsearch( &ItemName,
			CssBorderWidths,
			sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]),
			sizeof(tagCssItem),
			CompareCssItem );
		if(pItemName)
		{
			//is border width
			pBorder->mask |= cssMaskBorderWidth;
			pBorder->width = pItemName->item;
			return true;
		}
		if(Css::ProcessCssMeasure(pStrCssValue,&pBorder->widthmesure))
		{
			//is border width float
			pBorder->mask |= cssMaskBorderWidth;
			pBorder->width = _refmeasure;
			return true;
		}
		return false;
	}

	bool Css::ProcessCssBorder(LPCTSTR pStrCssValue,TBorder * pBorder)
	{
		tagCssItem ItemName;
		tagCssItem * pItemName;
		vector<LPTSTR> vals;
		GetTokens(pStrCssValue,_T(" "),vals);
		for(int i=0;i<(3 > (int)vals.size()?(int)vals.size():3);i++)
		{
			ItemName.name = vals[i];
			pItemName = (tagCssItem *)bsearch( &ItemName,
				CssBorderTypes,
				sizeof(CssBorderTypes)/sizeof(CssBorderTypes[0]),
				sizeof(tagCssItem),
				CompareCssItem );
			if(pItemName)
			{
				//is border type
				pBorder->mask |= cssMaskBorderType;
				pBorder->bordertype = pItemName->item;
				continue;
			}
			pItemName = (tagCssItem *)bsearch( &ItemName,
				CssBorderWidths,
				sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]),
				sizeof(tagCssItem),
				CompareCssItem );
			if(pItemName)
			{
				//is border width
				pBorder->mask |= cssMaskBorderWidth;
				pBorder->width = pItemName->item;
				continue;
			}
			if(Css::ProcessCssMeasure(vals[i],&pBorder->widthmesure))
			{
				//is border width float
				pBorder->mask |= cssMaskBorderWidth;
				pBorder->width = _refmeasure;
				continue;
			}
			//color
			if(Css::ProcessCssColor(vals[i],pBorder->color))
			{
				pBorder->mask |= cssMaskBorderColor;
			}
		}

		return true;
	}
	bool Css::ProcessCssBorder(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle)
	{
		if(mask & cssMaskBorder)
		{
			mask &= ~cssMaskBorder;
			pStyle->mask |= cssMaskBorder;
			if(mask & cssMaskLeft)
			{
				if(!pStyle->pBorder[1]) pStyle->pBorder[1] = new TBorder;
				if(mask & cssMaskBorderType)
					Css::ProcessCssBorderType(pStrCssValue, pStyle->pBorder[1]);
				else if(mask & cssMaskBorderWidth)
					Css::ProcessCssBorderWidth(pStrCssValue, pStyle->pBorder[1]);
				else if(mask & cssMaskBorderColor)
				{
					if(Css::ProcessCssColor(pStrCssValue, pStyle->pBorder[1]->color))
						 pStyle->pBorder[1]->mask |= cssMaskBorderColor;
				}else
					Css::ProcessCssBorder(pStrCssValue,pStyle->pBorder[1]);
			}else if(mask & cssMaskRight)
			{
				if(!pStyle->pBorder[3]) pStyle->pBorder[3] = new TBorder;
				if(mask & cssMaskBorderType)
					Css::ProcessCssBorderType(pStrCssValue, pStyle->pBorder[3]);
				else if(mask & cssMaskBorderWidth)
					Css::ProcessCssBorderWidth(pStrCssValue, pStyle->pBorder[3]);
				else if(mask & cssMaskBorderColor)
				{
					if(Css::ProcessCssColor(pStrCssValue, pStyle->pBorder[3]->color))
						 pStyle->pBorder[3]->mask |= cssMaskBorderColor;
				}else
					Css::ProcessCssBorder(pStrCssValue,pStyle->pBorder[3]);
			}else if(mask & cssMaskTop)
			{
				if(!pStyle->pBorder[2]) pStyle->pBorder[2] = new TBorder;
				if(mask & cssMaskBorderType)
					Css::ProcessCssBorderType(pStrCssValue, pStyle->pBorder[2]);
				else if(mask & cssMaskBorderWidth)
					Css::ProcessCssBorderWidth(pStrCssValue, pStyle->pBorder[2]);
				else if(mask & cssMaskBorderColor)
				{
					if(Css::ProcessCssColor(pStrCssValue, pStyle->pBorder[2]->color))
						 pStyle->pBorder[2]->mask |= cssMaskBorderColor;
				}else
					Css::ProcessCssBorder(pStrCssValue,pStyle->pBorder[2]);

			}else if(mask & cssMaskBottom)
			{
				if(!pStyle->pBorder[4]) pStyle->pBorder[4] = new TBorder;
				if(mask & cssMaskBorderType)
					Css::ProcessCssBorderType(pStrCssValue, pStyle->pBorder[4]);
				else if(mask & cssMaskBorderWidth)
					Css::ProcessCssBorderWidth(pStrCssValue, pStyle->pBorder[4]);
				else if(mask & cssMaskBorderColor)
				{
					if(Css::ProcessCssColor(pStrCssValue, pStyle->pBorder[4]->color))
						 pStyle->pBorder[4]->mask |= cssMaskBorderColor;
				}else
					Css::ProcessCssBorder(pStrCssValue,pStyle->pBorder[4]);
			}
			else
			{
				if(!pStyle->pBorder[0]) pStyle->pBorder[0] = new TBorder;
				if(mask & cssMaskBorderType)
					Css::ProcessCssBorderType(pStrCssValue, pStyle->pBorder[0]);
				else if(mask & cssMaskBorderWidth)
					Css::ProcessCssBorderWidth(pStrCssValue, pStyle->pBorder[0]);
				else if(mask & cssMaskBorderColor)
				{
					if(Css::ProcessCssColor(pStrCssValue, pStyle->pBorder[0]->color))
						 pStyle->pBorder[0]->mask |= cssMaskBorderColor;
				}else
					Css::ProcessCssBorder(pStrCssValue,pStyle->pBorder[0]);
			}
		}
		return true;
	}

	Css::tagCssItem	CssBackgroundRepeats[]={
		{_T("inherit"), Css::_inherit},
		{_T("no-repeat"), Css::_no_repeat},
		{_T("repeat"), Css::_repeat},
		{_T("repeat-x"), Css::_repeat_x},
		{_T("repeat-y"), Css::_repeat_y}
	};

	Css::tagCssItem	CssBackgroundAttachment[]={
		{_T("fixed"), Css::_fixed},
		{_T("inherit"), Css::_inherit},
		{_T("scroll"), Css::_scroll}
	};

	Css::tagCssItem	CssBackgroundPosition[]={
		{_T("bottom"), Css::_bottom},
		{_T("center"), Css::_center},
		{_T("inherit"), Css::_inherit},
		{_T("left"), Css::_left},
		{_T("right"), Css::_right},
		{_T("top"), Css::_top},
		{_T("none"), Css::_none}
	};

	bool Css::ProcessCssBackground(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle)
	{
		if(mask & cssMaskBackGround)
		{
			mask &= ~cssMaskBackGround;
			pStyle->mask |= cssMaskBackGround;
			if(!pStyle->pBackGround) pStyle->pBackGround = new TBackGround;

			if(mask & cssMaskBackGroundColor)
			{
				pStyle->pBackGround->mask |= cssMaskBackGroundColor;
				if(Css::ProcessCssColor(pStrCssValue,pStyle->pBackGround->color))
				{
					pStyle->pBackGround->mask |= cssMaskBackGroundColor;
					return true;
				}
				else
					return false;
			}else if(mask & cssMaskBackGroundImage)
			{
				pStyle->pBackGround->mask |= cssMaskBackGroundImage;
				if(::_tcsstr(pStrCssValue,_T("url(")))
				{
					LPTSTR	strPosStart,strPosEnd;
					strPosStart = ::_tcsstr((LPTSTR)pStrCssValue,_T("url(")) + ::lstrlen(_T("url("));
					strPosEnd = ::_tcsstr(strPosStart,_T(")"));
					pStyle->pBackGround->image.fileRef=_uri;
					::lstrcpyn(pStyle->pBackGround->image.url ,strPosStart, (int)(strPosEnd - strPosStart + 1));
				}else if(_tcsicmp(pStrCssValue,_T("none"))==0)
					pStyle->pBackGround->image.fileRef=_none;
				else if(_tcsicmp(pStrCssValue,_T("inherit"))==0)
					pStyle->pBackGround->image.fileRef=_inherit;
				else
				{
					pStyle->pBackGround->image.fileRef =_undef;
					return false;
				}
				pStyle->pBackGround->mask |= cssMaskBackGroundImage;
				return true;
			}else if(mask & cssMaskBackGroundRepeat)
			{
				pStyle->pBackGround->repeat = FindItemCssPropertyEx(CssBackgroundRepeats,(LPTSTR)pStrCssValue);
				if(pStyle->pBackGround->repeat !=_undef)
				{
					pStyle->pBackGround->mask |= cssMaskBackGroundRepeat;
					return true;
				}else
					return false;
			}else if(mask & cssMaskBackGroundAttachment)
			{
				pStyle->pBackGround->attachment = FindItemCssPropertyEx(CssBackgroundAttachment,(LPTSTR)pStrCssValue);
				if(pStyle->pBackGround->attachment !=_undef)
				{
					pStyle->pBackGround->mask |= cssMaskBackGroundAttachment;
					return true;
				}else
					return false;
			}else if(mask & cssMaskBackGroundPosition)
			{
				pStyle->pBackGround->position = FindItemCssPropertyEx(CssBackgroundPosition,(LPTSTR)pStrCssValue);
				if(pStyle->pBackGround->position==_undef)
				{
					if(ProcessCssMeasure(pStrCssValue,&pStyle->pBackGround->positionmesure))
						pStyle->pBackGround->position = _refmeasure;
				}
				if(pStyle->pBackGround->position !=_undef)
				{
					pStyle->pBackGround->mask |= cssMaskBackGroundPosition;
					return true;
				}else
					return false;
			}else
			{
				if(_tcsicmp(pStrCssValue,_T("inherit"))==0)
				{
					pStyle->pBackGround->mask |= cssMaskBackGround;
					pStyle->pBackGround->background = _inherit;
					return true;
				}

				if(_tcsicmp(pStrCssValue,_T("normal"))==0)
				{
					pStyle->pBackGround->mask |= cssMaskBackGround;
					pStyle->pBackGround->background = _normal;
					return true;
				}

				vector<LPTSTR> vals;
				GetTokens(pStrCssValue,_T(" "),vals);
				for(int i=0;i<(int)vals.size();i++)
				{
					if(Css::ProcessCssColor(vals[i],pStyle->pBackGround->color))
					{
						pStyle->pBackGround->mask |= cssMaskBackGroundColor;
						continue;
					}

					if(::_tcsstr(vals[i],_T("url(")))
					{
						LPTSTR	strPosStart,strPosEnd;
						strPosStart = ::_tcsstr(vals[i],_T("url(")) + ::lstrlen(_T("url("));
						strPosEnd = ::_tcsstr(strPosStart,_T(")"));
						pStyle->pBackGround->image.fileRef=_uri;
						::lstrcpyn(pStyle->pBackGround->image.url ,strPosStart, (int)(strPosEnd - strPosStart + 1));
						pStyle->pBackGround->mask |= cssMaskBackGroundImage;
					}else if(_tcsicmp(vals[i],_T("none"))==0)
					{
						pStyle->pBackGround->image.fileRef=_none;
						pStyle->pBackGround->mask |= cssMaskBackGroundImage;
					}
					else if(_tcsicmp(vals[i],_T("inherit"))==0)
					{
						pStyle->pBackGround->image.fileRef=_inherit;
						pStyle->pBackGround->mask |= cssMaskBackGroundImage;
					}
					if(pStyle->pBackGround->mask & cssMaskBackGroundImage) continue;

					pStyle->pBackGround->repeat = FindItemCssPropertyEx(CssBackgroundRepeats,vals[i]);
					if(pStyle->pBackGround->repeat !=_undef)
					{
						pStyle->pBackGround->mask |= cssMaskBackGroundRepeat;
						continue;
					}

					pStyle->pBackGround->attachment = FindItemCssPropertyEx(CssBackgroundAttachment,vals[i]);
					if(pStyle->pBackGround->attachment !=_undef)
					{
						pStyle->pBackGround->mask |= cssMaskBackGroundAttachment;
						continue;
					}

					pStyle->pBackGround->position = FindItemCssPropertyEx(CssBackgroundPosition,vals[i]);
					if(pStyle->pBackGround->position==_undef)
					{
						if(ProcessCssMeasure(vals[i],&pStyle->pBackGround->positionmesure))
							pStyle->pBackGround->position = _refmeasure;
					}
					if(pStyle->pBackGround->position !=_undef)
					{
						pStyle->pBackGround->mask |= cssMaskBackGroundPosition;
						continue;
					}
				}
				return true;
			}
		}
		return true;
	}

	Css::tagCssItem	CssFontFamily[]={
		{_T("cursive"), Css::_cursive},
		{_T("fantasy"), Css::_fantasy},
		{_T("monospace"), Css::_monospace},
		{_T("sans_serif"), Css::_sans_serif},
		{_T("serif"), Css::_serif}
	};

	Css::tagCssItem	CssFontStyle[]={
		{_T("italic"), Css::_italic},
		{_T("normal"), Css::_normal},
		{_T("oblique"), Css::_oblique}
	};

	Css::tagCssItem	CssFontVariant[]={
		{_T("normal"), Css::_normal},
		{_T("small_caps"), Css::_small_caps}
	};

	Css::tagCssItem	CssFontWeight[]={
		{_T("bold"), Css::_bold},
		{_T("border"), Css::_border},
		{_T("inherit"), Css::_inherit},
		{_T("lighter"), Css::_lighter},
		{_T("normal"), Css::_normal}
	};

	Css::tagCssItem	CssFontSize[]={
		{_T("inherit"), Css::_inherit},
		{_T("large"), Css::_large},
		{_T("larger"), Css::_larger},
		{_T("medium"), Css::_medium},
		{_T("small"), Css::_small},
		{_T("smaller"), Css::_smaller},
		{_T("xx_large"), Css::_xx_large},
		{_T("xx_small"), Css::_xx_small},
		{_T("x_large"), Css::_x_large},
		{_T("x_small"), Css::_x_small}
	};

	bool Css::ProcessCssFont(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle)
	{
		if(mask & cssMaskFont)
		{
			mask &= ~cssMaskFont;
			pStyle->mask |= cssMaskFont;
			if(!pStyle->pFont) pStyle->pFont = new TFont;
			if(mask & cssMaskFontFamily)
			{
				//pStyle->pFont->mask |= cssMaskFontFamily;
				//pStyle->pFont->fontfamily = FindItemCssPropertyEx(CssFontFamily,pStrCssValue);
				pStyle->pFont->mask |= cssMaskFontFaceName;
				::_tcscpy_s(pStyle->pFont->fontfacename,LF_FACESIZE, pStrCssValue);
				return true;
			}else if(mask & cssMaskFontFaceName)
			{
				pStyle->pFont->mask |= cssMaskFontFaceName;
				::_tcscpy_s(pStyle->pFont->fontfacename,LF_FACESIZE, pStrCssValue);
				return true;
			}else if(mask & cssMaskFontStyle)
			{
				pStyle->pFont->mask |= cssMaskFontStyle;
				pStyle->pFont->fontstyle = FindItemCssPropertyEx(CssFontStyle,pStrCssValue);
				return true;
			}else if(mask & cssMaskFontVariant)
			{
				pStyle->pFont->mask |= cssMaskFontVariant;
				pStyle->pFont->fontvariant = FindItemCssPropertyEx(CssFontVariant,pStrCssValue);
				return true;
			}else if(mask & cssMaskFontWeight)
			{
				pStyle->pFont->mask |= cssMaskFontWeight;
				pStyle->pFont->fontweight = FindItemCssPropertyEx(CssFontWeight,pStrCssValue);
				if(pStyle->pFont->fontweight ==_undef)
				{
					LPTSTR pStrNext;
					pStyle->pFont->fontweightvalue = ::_tcstol(pStrCssValue,&pStrNext,10);
					pStyle->pFont->fontweight = _refmeasure;
				}
				return true;
			}else if(mask & cssMaskFontSize)
			{
				pStyle->pFont->mask |= cssMaskFontSize;
				pStyle->pFont->fontsize = FindItemCssPropertyEx(CssFontSize,pStrCssValue);
				if(pStyle->pFont->fontsize ==_undef)
				{
					 Css::ProcessCssMeasure(pStrCssValue,&pStyle->pFont->fontsizemeasure);
					 pStyle->pFont->fontsize = _refmeasure;
					 return true;
				}
			}else if(mask & cssMaskLineHeight)
			{
				pStyle->pFont->mask |= cssMaskLineHeight;
				Css::ProcessCssMeasure(pStrCssValue,&pStyle->pFont->lineheightmeasure);
				return true;
			}else
			{
				if(_tcsicmp(pStrCssValue,_T("inherit"))==0)
				{
					pStyle->pFont->font =_inherit;
					pStyle->pFont->mask = cssMaskFont;
				}
				if(_tcsicmp(pStrCssValue,_T("normal"))==0)
				{
					pStyle->pFont->font =_normal;
					pStyle->pFont->mask = cssMaskFont;
				}
			}

		}
		return true;
	}

	Css::tagCssItem	CssTextAlign[]={
		{_T("center"), Css::_center},
		{_T("inherit"), Css::_inherit},
		{_T("justify"), Css::_justify},
		{_T("left"), Css::_left},
		{_T("right"), Css::_right}
	};

		Css::tagCssItem	CssTextDecoration[]={
		{_T("blink "), Css::_blink },
		{_T("inherit"), Css::_inherit},
		{_T("line-through"), Css::_line_through},
		{_T("none"), Css::_none},
		{_T("overline"), Css::_overline},
		{_T("underline"), Css::_underline}
	};

		Css::tagCssItem	CssWordSpacing[]={
		{_T("inherit"), Css::_inherit},
		{_T("normal"), Css::_normal}
	};

		Css::tagCssItem	CssLetterSpacing[]={
		{_T("inherit"), Css::_inherit},
		{_T("normal"), Css::_normal}
	};

		Css::tagCssItem	CssTextTransform[]={
		{_T("capitalize"), Css::_capitalize},
		{_T("inherit"), Css::_inherit},
		{_T("lowercase"), Css::_lowercase},
		{_T("none"), Css::_none},
		{_T("uppercase"), Css::_uppercase}
	};

		Css::tagCssItem	CssWhiteSpace[]={
		{_T("inherit"), Css::_inherit},
		{_T("normal"), Css::_normal},
		{_T("nowrap"), Css::_nowrap},
		{_T("pre"), Css::_pre},
		{_T("pre-line"), Css::_pre_line},
		{_T("pre-wrap"), Css::_pre_wrap}
	};

	bool Css::ProcessCssText(LPCTSTR pStrCssValue,DWORD mask,TBoxStyle * pStyle)
	{
		if(mask & cssMaskText)
		{
			mask &= ~cssMaskText;
			pStyle->mask |= cssMaskText;
			if(!pStyle->pText) pStyle->pText = new TText;
			if(mask & cssMaskTextIndent)
			{
				pStyle->pText->mask |= cssMaskTextIndent;
				pStyle->pText->textindent = _refmeasure;
				Css::ProcessCssMeasure(pStrCssValue,&pStyle->pText->textindentmeasure);
			}else if(mask & cssMaskTextAlign)
			{
				pStyle->pText->mask |= cssMaskTextAlign;
				pStyle->pText->textalign = FindItemCssPropertyEx(CssTextAlign,pStrCssValue);
				return true;
			}else if(mask & cssMaskTextDecoration)
			{
				pStyle->pText->mask |= cssMaskTextDecoration;
				pStyle->pText->textdecoration = FindItemCssPropertyEx(CssTextDecoration,pStrCssValue);
				return true;
			}else if(mask & cssMaskWordSpacing)
			{
				pStyle->pText->mask |= cssMaskWordSpacing;
				pStyle->pText->wordspacing = FindItemCssPropertyEx(CssWordSpacing,pStrCssValue);
				if(pStyle->pText->wordspacing ==_undef)
				{

					pStyle->pText->wordspacing = _refmeasure;
					Css::ProcessCssMeasure(pStrCssValue,&pStyle->pText->wordspacingmeasure);
				}
				return true;
			}else if(mask & cssMaskLetterSpacing)
			{
				pStyle->pText->mask |= cssMaskLetterSpacing;
				pStyle->pText->letterspacing = FindItemCssPropertyEx(CssLetterSpacing,pStrCssValue);
				if(pStyle->pText->letterspacing ==_undef)
				{

					pStyle->pText->letterspacing = _refmeasure;
					Css::ProcessCssMeasure(pStrCssValue,&pStyle->pText->letterspacingmeasure);
				}
				return true;
			}else if(mask & cssMaskTextTransform)
			{
				pStyle->pText->mask |= cssMaskTextTransform;
				pStyle->pText->texttransform = FindItemCssPropertyEx(CssTextTransform,pStrCssValue);
				return true;
			}else if(mask & cssMaskWhiteSpace)
			{
				pStyle->pText->mask |= cssMaskWhiteSpace;
				pStyle->pText->whitespace = FindItemCssPropertyEx(CssWhiteSpace,pStrCssValue);
				return true;
			}else
			{
				if(_tcsicmp(pStrCssValue,_T("inherit"))==0)
				{
					pStyle->pText->text =_inherit;
					pStyle->pText->mask = cssMaskText;
				}
				if(_tcsicmp(pStrCssValue,_T("normal"))==0)
				{
					pStyle->pText->text =_normal;
					pStyle->pText->mask = cssMaskText;
				}
			}
		}
		return true;
	}

	Css::tagBoxStyle::tagBoxStyle(void)
	{
		/*
		pColor = NULL;
		pText = NULL;
		pFont = NULL;
		pBackGround = NULL;
		for(int i=0;i<5;i++)
		{
			pMargin[i] =  NULL;
			pPadding[i] = NULL;
			pBorder[i] = NULL;
		};
		SharedProperty = false;
		*/
		::ZeroMemory(this,sizeof(tagBoxStyle));
	}

	Css::tagBoxStyle::~tagBoxStyle(void)
	{
		if(!SharedProperty)
		{
			if(pColor) delete pColor;
			if(pText) delete pText;
			if(pFont) delete pFont;
			if(pBackGround)delete pBackGround;
			for(int i=0;i<5;i++)
			{
				if(pMargin[i]) delete pMargin[i];
				if(pPadding[i]) delete  pPadding[i];
				if(pBorder[i]) delete  pBorder[i];
			}
		}
	}

	bool Css::IsSameStyle(const TBoxStyle & style1,const TBoxStyle & style2)
	{
		//cmp color
		if(style1.pColor &&  style2.pColor)
		{
			if(memcmp(style1.pColor,style2.pColor,sizeof(TColor))) return false;
		}else 
			if(style1.pColor ||  style2.pColor) return false;

		//cmp background
		if(style1.pBackGround &&  style2.pBackGround)
		{
			if(memcmp(style1.pBackGround,style2.pBackGround,sizeof(TBackGround))) return false;
		}else 
			if(style1.pBackGround ||  style2.pBackGround) return false;

		//cmp font
		if(style1.pFont &&  style2.pFont)
		{
			if(memcmp(style1.pFont,style2.pFont,sizeof(TFont))) return false;
		}else 
			if(style1.pFont ||  style2.pFont) return false;

		//cmp background
		if(style1.pText &&  style2.pText)
		{
			if(memcmp(style1.pText,style2.pText,sizeof(TText))) return false;
		}else 
			if(style1.pText ||  style2.pText) return false;

		for(int i=0;i<5;i++)
		{
			//cmp border
			if(style1.pBorder[i] &&  style2.pBorder[i])
			{
				if(memcmp(style1.pBorder[i],style2.pBorder[i],sizeof(TBorder))) return false;
			}else 
				if(style1.pBorder[i] ||  style2.pBorder[i]) return false;

			//cmp padding
			if(style1.pPadding[i] &&  style2.pPadding[i])
			{
				if(memcmp(style1.pPadding[i],style2.pPadding[i],sizeof(TPadding))) return false;
			}else 
				if(style1.pPadding[i] ||  style2.pPadding[i]) return false;

			//cmp margin
			if(style1.pMargin[i] &&  style2.pMargin[i])
			{
				if(memcmp(style1.pMargin[i],style2.pMargin[i],sizeof(TMargin))) return false;
			}else 
				if(style1.pMargin[i] ||  style2.pMargin[i]) return false;
		}
		return true;
	}
	bool Css::StyleCopyTo(TBoxStyle * pDestStyle,const TBoxStyle * pSrcStyle)
	{
		if(!pSrcStyle) return false;
		if(!pDestStyle) pDestStyle = new TBoxStyle();

		pDestStyle->mask = pSrcStyle->mask;
		//copy color
		if(pSrcStyle->pColor)
		{
			if(!pDestStyle->pColor) pDestStyle->pColor = new TColor;
			memcpy(pDestStyle->pColor,pSrcStyle->pColor,sizeof(TColor));
		}else if(pDestStyle->pColor)
		{
			delete pDestStyle->pColor;
			pDestStyle->pColor = NULL;
		}

		//copy background
		if(pSrcStyle->pBackGround)
		{
			if(!pDestStyle->pBackGround) pDestStyle->pBackGround = new TBackGround;
			memcpy(pDestStyle->pBackGround,pSrcStyle->pBackGround,sizeof(TBackGround));
		}else if(pDestStyle->pBackGround)
		{
			delete pDestStyle->pBackGround;
			pDestStyle->pBackGround = NULL;
		}

		//copy font
		if(pSrcStyle->pFont)
		{
			if(!pDestStyle->pFont) pDestStyle->pFont = new TFont;
			memcpy(pDestStyle->pFont,pSrcStyle->pFont,sizeof(TFont));
		}else if(pDestStyle->pFont)
		{
			delete pDestStyle->pFont;
			pDestStyle->pFont = NULL;
		}

		//copy text
		if(pSrcStyle->pText)
		{
			if(!pDestStyle->pText) pDestStyle->pText = new TText;
			memcpy(pDestStyle->pText,pSrcStyle->pText,sizeof(TText));
		}else if(pDestStyle->pText)
		{
			delete pDestStyle->pText;
			pDestStyle->pText = NULL;
		}

		for(int i=0;i<5;i++)
		{
			//copy border
			if(pSrcStyle->pBorder[i])
			{
				if(!pDestStyle->pBorder[i]) pDestStyle->pBorder[i] = new TBorder;
				memcpy(pDestStyle->pBorder[i],pSrcStyle->pBorder[i],sizeof(TBorder));
			}else if(pDestStyle->pBorder[i])
			{
				delete pDestStyle->pBorder[i];
				pDestStyle->pBorder[i] = NULL;
			}

			//copy padding
			if(pSrcStyle->pPadding[i])
			{
				if(!pDestStyle->pPadding[i]) pDestStyle->pPadding[i] = new TPadding;
				memcpy(pDestStyle->pPadding[i],pSrcStyle->pPadding[i],sizeof(TPadding));
			}else if(pDestStyle->pPadding[i])
			{
				delete pDestStyle->pPadding[i];
				pDestStyle->pPadding[i] = NULL;
			}

			//cmp margin
			if(pSrcStyle->pMargin[i])
			{
				if(!pDestStyle->pMargin[i]) pDestStyle->pMargin[i] = new TMargin;
				memcpy(pDestStyle->pMargin[i],pSrcStyle->pMargin[i],sizeof(TMargin));
			}else if(pDestStyle->pMargin[i])
			{
				delete pDestStyle->pMargin[i];
				pDestStyle->pMargin[i] = NULL;
			}
		}

		//merge width
		if(pSrcStyle->mask & cssMaskWidth)
		{
			if( pSrcStyle->pWidth)
			{
				if(!pDestStyle->pWidth) pDestStyle->pWidth = new TMeasure;
				memcpy(pDestStyle->pWidth,pSrcStyle->pWidth,sizeof(TMeasure));
			}
			pDestStyle->mask |= cssMaskWidth;
		}

		//merge height
		if(pSrcStyle->mask & cssMaskHeight)
		{
			if( pSrcStyle->pHeight)
			{
				if(!pDestStyle->pHeight) pDestStyle->pHeight = new TMeasure;
				memcpy(pDestStyle->pHeight,pSrcStyle->pHeight,sizeof(TMeasure));
			}
			pDestStyle->mask |= cssMaskHeight;
		}

		return true;
	}

	bool Css::DrawCssFrame(HDC hPaintDC,RECT *pRect,const TBoxStyle* pCssStyle)
	{
		RECT rect = *pRect;
		if(pCssStyle)
		{
			if(pCssStyle->mask & cssMaskBackGround)
			{
				if(pCssStyle->pBackGround->mask & cssMaskBackGroundColor)
				{
					if(pCssStyle->pBackGround->color.mask & cssMaskColor)
					{
						COLORREF color = pCssStyle->pBackGround->color.color[0];
						HBRUSH hBrush = ::CreateSolidBrush(color);
						::FillRect(hPaintDC,&rect,hBrush);
						::DeleteObject(hBrush);
					}else if(pCssStyle->pBackGround->color.mask & cssMaskLeft && pCssStyle->pBackGround->color.colorRef[1]!=XUtil::Css::_undef && 
						pCssStyle->pBackGround->color.mask & cssMaskRight && pCssStyle->pBackGround->color.colorRef[3]!=XUtil::Css::_undef &&
						pCssStyle->pBackGround->color.colorRef[2]==XUtil::Css::_undef && pCssStyle->pBackGround->color.colorRef[4]==XUtil::Css::_undef)
					{
						TRIVERTEX	vertex[2];
						GRADIENT_RECT grect[1];

						vertex[0].x = rect.left;
						vertex[0].y = rect.top;
						vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[1])*256;
						vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[1])*256;
						vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[1])*256;

						vertex[1].x = rect.right;
						vertex[1].y = rect.bottom;
						vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[3])*256;
						vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[3])*256;
						vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[3])*256;

						grect[0].UpperLeft = 1;
						grect[0].LowerRight = 0;
						GradientFill(hPaintDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
					}else if(pCssStyle->pBackGround->color.mask & cssMaskTop && pCssStyle->pBackGround->color.colorRef[2]!=XUtil::Css::_undef && 
						pCssStyle->pBackGround->color.mask & cssMaskBottom && pCssStyle->pBackGround->color.colorRef[4]!=XUtil::Css::_undef  &&
						pCssStyle->pBackGround->color.colorRef[1]==XUtil::Css::_undef && pCssStyle->pBackGround->color.colorRef[3]==XUtil::Css::_undef)
					{
						TRIVERTEX	vertex[2];
						GRADIENT_RECT grect[1];

						vertex[0].x = rect.left;
						vertex[0].y = rect.top;
						vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[2])*256;
						vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[2])*256;
						vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[2])*256;

						vertex[1].x = rect.right;
						vertex[1].y = rect.bottom;
						vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[4])*256;
						vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[4])*256;
						vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[4])*256;

						grect[0].UpperLeft = 1;
						grect[0].LowerRight = 0;
						GradientFill(hPaintDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);
					}else
					{

						TRIVERTEX        vert [4] ;
						GRADIENT_TRIANGLE    gTRi[2];

						vert [0] .x       =  rect.left;
						vert [0] .y       =  rect.top ;
						vert [0].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[1])*256;
						vert [0].Green    = GetGValue( pCssStyle->pBackGround->color.color[1])*256;
						vert [0].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[1])*256;
						vert [0].Alpha    =  0x0000;

						vert [1] .x       =  rect.right;
						vert [1] .y       =  rect.top;
						vert [1].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[2])*256;
						vert [1].Green    = GetGValue( pCssStyle->pBackGround->color.color[2])*256;
						vert [1].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[2])*256;
						vert [1].Alpha    =  0x0000;

						vert [2] .x       =  rect.right;
						vert [2] .y       =  rect.bottom; 
						vert [2].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[3])*256;
						vert [2].Green    =  GetGValue( pCssStyle->pBackGround->color.color[3])*256;
						vert [2].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[3])*256;
						vert [2].Alpha    =  0x0000;


						vert [3] .x       =  rect.left;
						vert [3] .y       =  rect.bottom;
						vert [3].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[4])*256;
						vert [3].Green    =  GetGValue( pCssStyle->pBackGround->color.color[4])*256;
						vert [3].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[4])*256;
						vert [3].Alpha    =  0x0000;

						gTRi[0].Vertex1   = 0;
						gTRi[0].Vertex2   = 1;
						gTRi[0].Vertex3   = 3;

						gTRi[1].Vertex1   = 1;
						gTRi[1].Vertex2   = 2;
						gTRi[1].Vertex3   = 3;
						GradientFill(hPaintDC,vert,4,&gTRi,2,GRADIENT_FILL_TRIANGLE);
					}
				}
			}
			if(pCssStyle->mask & cssMaskBorder)
			{
				//if(pCssBoxStyle->pBorder[1])m_ContentRect.left += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[1]);
				//if(pCssBoxStyle->pBorder[2])m_ContentRect.top += (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[2],false);
				//if(pCssBoxStyle->pBorder[3])m_ContentRect.right -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[3]);
				//if(pCssBoxStyle->pBorder[4])m_ContentRect.bottom -= (int) XUtil::Css::GetBorderExtent(pCssBoxStyle->pBorder[4],false);
				
				if(pCssStyle->pBorder[1])
				{
					int width = (int) XUtil::Css::GetBorderExtent(pCssStyle->pBorder[1]);
					RECT r = rect;
					r.right = r.left + width - 1;
					if(pCssStyle->pBorder[1]->mask & cssMaskBorderColor)
					{
						COLORREF color = pCssStyle->pBorder[1]->color.color[0];
						 if(width==1)
						 {
							HPEN hPen = CreatePen(PS_SOLID, width, RGB(255,0,0));
							::MoveToEx(hPaintDC,r.left,r.top,0);
							::LineTo(hPaintDC,r.right + 1,r.bottom +1);
							::DeleteObject(hPen);
						 }else
						 {
							HBRUSH hBrush = ::CreateSolidBrush(color);
							::FillRect(hPaintDC,&r,hBrush);
							::DeleteObject(hBrush);
						 }
					}
				}
				if(pCssStyle->pBorder[2])
				{
					int height = (int) XUtil::Css::GetBorderExtent(pCssStyle->pBorder[1],false);
					RECT r = rect;
					r.bottom = r.top+ height - 1;
					if(pCssStyle->pBorder[2]->mask & cssMaskBorderColor)
					{
						COLORREF color = pCssStyle->pBorder[1]->color.color[0];
						 if(height==1)
						 {
							HPEN hPen = CreatePen(PS_SOLID, height, RGB(255,0,0));
							::MoveToEx(hPaintDC,r.left,r.top,0);
							::LineTo(hPaintDC,r.right + 1,r.bottom +1);
							::DeleteObject(hPen);
						 }else
						 {
							HBRUSH hBrush = ::CreateSolidBrush(color);
							::FillRect(hPaintDC,&r,hBrush);
							::DeleteObject(hBrush);
						 }
					}
				}
				if(pCssStyle->pBorder[3])
				{
					int width = (int) XUtil::Css::GetBorderExtent(pCssStyle->pBorder[1]);
					RECT r = rect;
					r.left = r.right - width + 1;
					if(pCssStyle->pBorder[3]->mask & cssMaskBorderColor)
					{
						COLORREF color = pCssStyle->pBorder[3]->color.color[0];
						 if(width==1)
						 {
							HPEN hPen = CreatePen(PS_SOLID, width, RGB(255,0,0));
							::MoveToEx(hPaintDC,r.left,r.top,0);
							::LineTo(hPaintDC,r.right + 1,r.bottom +1);
							::DeleteObject(hPen);
						 }else
						 {
							HBRUSH hBrush = ::CreateSolidBrush(color);
							::FillRect(hPaintDC,&r,hBrush);
							::DeleteObject(hBrush);
						 }
					}
				}

				if(pCssStyle->pBorder[4])
				{
					int height = (int) XUtil::Css::GetBorderExtent(pCssStyle->pBorder[1],false);
					RECT r = rect;
					r.top = r.bottom - height + 1;
					if(pCssStyle->pBorder[4]->mask & cssMaskBorderColor)
					{
						COLORREF color = pCssStyle->pBorder[1]->color.color[0];
						 if(height==1)
						 {
							HPEN hPen = CreatePen(PS_SOLID, height, RGB(255,0,0));
							::MoveToEx(hPaintDC,r.left,r.top,0);
							::LineTo(hPaintDC,r.right + 1,r.bottom +1);
							::DeleteObject(hPen);
						 }else
						 {
							HBRUSH hBrush = ::CreateSolidBrush(color);
							::FillRect(hPaintDC,&r,hBrush);
							::DeleteObject(hBrush);
						 }
					}
				}

					/*
					if(pCssStyle->pBorder[0]->color.mask & cssMaskColor)
					{
						COLORREF color = pCssStyle->pBorder[0]->color.color[0];
						HBRUSH hBrush = ::CreateSolidBrush(color);
						int width = 1;
						if(mask & cssMaskBorderWidth)
						::FillRect(hPaintDC,&rect,hBrush);
						::DeleteObject(hBrush);
					}else if(pCssStyle->pBackGround->color.mask & cssMaskLeft && pCssStyle->pBackGround->color.colorRef[1]!=XUtil::Css::_undef && 
						pCssStyle->pBackGround->color.mask & cssMaskRight && pCssStyle->pBackGround->color.colorRef[3]!=XUtil::Css::_undef &&
						pCssStyle->pBackGround->color.colorRef[2]==XUtil::Css::_undef && pCssStyle->pBackGround->color.colorRef[4]==XUtil::Css::_undef)
					{
						TRIVERTEX	vertex[2];
						GRADIENT_RECT grect[1];

						vertex[0].x = rect.left;
						vertex[0].y = rect.top;
						vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[1])*256;
						vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[1])*256;
						vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[1])*256;

						vertex[1].x = rect.right;
						vertex[1].y = rect.bottom;
						vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[3])*256;
						vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[3])*256;
						vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[3])*256;

						grect[0].UpperLeft = 1;
						grect[0].LowerRight = 0;
						GradientFill(hPaintDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_H);
					}else if(pCssStyle->pBackGround->color.mask & cssMaskTop && pCssStyle->pBackGround->color.colorRef[2]!=XUtil::Css::_undef && 
						pCssStyle->pBackGround->color.mask & cssMaskBottom && pCssStyle->pBackGround->color.colorRef[4]!=XUtil::Css::_undef  &&
						pCssStyle->pBackGround->color.colorRef[1]==XUtil::Css::_undef && pCssStyle->pBackGround->color.colorRef[3]==XUtil::Css::_undef)
					{
						TRIVERTEX	vertex[2];
						GRADIENT_RECT grect[1];

						vertex[0].x = rect.left;
						vertex[0].y = rect.top;
						vertex[0].Red  =  GetRValue( pCssStyle->pBackGround->color.color[2])*256;
						vertex[0].Green = GetGValue( pCssStyle->pBackGround->color.color[2])*256;
						vertex[0].Blue =  GetBValue( pCssStyle->pBackGround->color.color[2])*256;

						vertex[1].x = rect.right;
						vertex[1].y = rect.bottom;
						vertex[1].Red  =  GetRValue( pCssStyle->pBackGround->color.color[4])*256;
						vertex[1].Green = GetGValue( pCssStyle->pBackGround->color.color[4])*256;
						vertex[1].Blue =  GetBValue( pCssStyle->pBackGround->color.color[4])*256;

						grect[0].UpperLeft = 1;
						grect[0].LowerRight = 0;
						GradientFill(hPaintDC, vertex, 2, grect, 1,GRADIENT_FILL_RECT_V);
					}else
					{

						TRIVERTEX        vert [4] ;
						GRADIENT_TRIANGLE    gTRi[2];

						vert [0] .x       =  rect.left;
						vert [0] .y       =  rect.top ;
						vert [0].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[1])*256;
						vert [0].Green    = GetGValue( pCssStyle->pBackGround->color.color[1])*256;
						vert [0].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[1])*256;
						vert [0].Alpha    =  0x0000;

						vert [1] .x       =  rect.right;
						vert [1] .y       =  rect.top;
						vert [1].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[2])*256;
						vert [1].Green    = GetGValue( pCssStyle->pBackGround->color.color[2])*256;
						vert [1].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[2])*256;
						vert [1].Alpha    =  0x0000;

						vert [2] .x       =  rect.right;
						vert [2] .y       =  rect.bottom; 
						vert [2].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[3])*256;
						vert [2].Green    =  GetGValue( pCssStyle->pBackGround->color.color[3])*256;
						vert [2].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[3])*256;
						vert [2].Alpha    =  0x0000;


						vert [3] .x       =  rect.left;
						vert [3] .y       =  rect.bottom;
						vert [3].Red	  =  GetRValue( pCssStyle->pBackGround->color.color[4])*256;
						vert [3].Green    =  GetGValue( pCssStyle->pBackGround->color.color[4])*256;
						vert [3].Blue     =  GetBValue( pCssStyle->pBackGround->color.color[4])*256;
						vert [3].Alpha    =  0x0000;

						gTRi[0].Vertex1   = 0;
						gTRi[0].Vertex2   = 1;
						gTRi[0].Vertex3   = 3;

						gTRi[1].Vertex1   = 1;
						gTRi[1].Vertex2   = 2;
						gTRi[1].Vertex3   = 3;
						GradientFill(hPaintDC,vert,4,&gTRi,2,GRADIENT_FILL_TRIANGLE);
					}
				}
				*/
			}
		}
		return true;
	}

	bool Css::MergeStyle(TBoxStyle * pDestStyle,const TBoxStyle * pSrcStyle)
	{

		if(!pSrcStyle) return true;
		if(!pDestStyle) pDestStyle = new TBoxStyle();
		//merge color
		if(pSrcStyle->mask & cssMaskColor)
		{
			if( pSrcStyle->pColor)
			{
				if(!pDestStyle->pColor) pDestStyle->pColor = new TColor;
				memcpy(pDestStyle->pColor,pSrcStyle->pColor,sizeof(TColor));
			}else if(pDestStyle->pColor)
			{
				delete pDestStyle->pColor;
				pDestStyle->pColor = NULL;
			}
			if(pDestStyle->pColor)
				pDestStyle->mask |= cssMaskColor;
			else
				pDestStyle->mask = pDestStyle->mask & ~cssMaskColor;
		}

		//merge background
		if(pSrcStyle->mask & cssMaskBackGround)
		{
			if(pSrcStyle->pBackGround)
			{
				if(!pDestStyle->pBackGround)
				{
					pDestStyle->pBackGround = new TBackGround;
					memcpy(pDestStyle->pBackGround,pSrcStyle->pBackGround,sizeof(TBackGround));
				}else
				{
					if(pSrcStyle->pBackGround->mask & cssMaskBackGround)
						pDestStyle->pBackGround->background = pSrcStyle->pBackGround->background;
					if(pSrcStyle->pBackGround->mask & cssMaskBackGroundColor)
						pDestStyle->pBackGround->color = pSrcStyle->pBackGround->color;
					if(pSrcStyle->pBackGround->mask & cssMaskBackGroundImage)
					{
						pDestStyle->pBackGround->image = pSrcStyle->pBackGround->image;
						::memcpy(pDestStyle->pBackGround->image.url,pSrcStyle->pBackGround->image.url,sizeof(TCHAR)*255);
					}
					if(pSrcStyle->pBackGround->mask & cssMaskBackGroundAttachment)
						pDestStyle->pBackGround->attachment = pSrcStyle->pBackGround->attachment;
					if(pSrcStyle->pBackGround->mask & cssMaskBackGroundRepeat)
						pDestStyle->pBackGround->repeat= pSrcStyle->pBackGround->repeat;
					if(pSrcStyle->pBackGround->mask & cssMaskBackGroundPosition)
					{
						pDestStyle->pBackGround->position= pSrcStyle->pBackGround->position;
						pDestStyle->pBackGround->positionmesure= pSrcStyle->pBackGround->positionmesure;
					}
				}
				pDestStyle->pBackGround->mask |= pSrcStyle->pBackGround->mask;

			}else if(pDestStyle->pBackGround)
			{
				delete pDestStyle->pBackGround;
				pDestStyle->pBackGround = NULL;
			}
			if(pDestStyle->pBackGround)
				pDestStyle->mask |= cssMaskBackGround;
			else
				pDestStyle->mask = pDestStyle->mask & ~cssMaskBackGround;
		}

		//merge font
		if(pSrcStyle->mask & cssMaskFont)
		{
			if(pSrcStyle->pFont)
			{
				if(!pDestStyle->pFont)
				{
					pDestStyle->pFont = new TFont;
					memcpy(pDestStyle->pFont,pSrcStyle->pFont,sizeof(TFont));
				}else
				{
					if(pSrcStyle->pFont->mask & cssMaskFont)
						pDestStyle->pFont->font = pSrcStyle->pFont->font;
					//if(pSrcStyle->pFont->mask & cssMaskFontFamily)
					//	pDestStyle->pFont->fontfamily = pSrcStyle->pFont->fontfamily;
					if(pSrcStyle->pFont->mask & cssMaskFontFaceName)
					{
						::memcpy(pDestStyle->pFont->fontfacename ,pSrcStyle->pFont->fontfacename,sizeof(TCHAR)*LF_FACESIZE);
					}
					if(pSrcStyle->pFont->mask & cssMaskFontStyle)
						pDestStyle->pFont->fontstyle = pSrcStyle->pFont->fontstyle;
					if(pSrcStyle->pFont->mask & cssMaskFontVariant)
						pDestStyle->pFont->fontvariant= pSrcStyle->pFont->fontvariant;
					if(pSrcStyle->pFont->mask & cssMaskFontSize)
					{
						pDestStyle->pFont->fontsize= pSrcStyle->pFont->fontsize;
						pDestStyle->pFont->fontsizemeasure= pSrcStyle->pFont->fontsizemeasure;
					}
					if(pSrcStyle->pFont->mask & cssMaskFontWeight)
					{
						pDestStyle->pFont->fontweight= pSrcStyle->pFont->fontweight;
						pDestStyle->pFont->fontweightvalue= pSrcStyle->pFont->fontweightvalue;
					}
					if(pSrcStyle->pFont->mask & cssMaskLineHeight)
						pDestStyle->pFont->lineheightmeasure= pSrcStyle->pFont->lineheightmeasure;
				}
				pDestStyle->pFont->mask |= pSrcStyle->pFont->mask;

			}else if(pDestStyle->pFont)
			{
				delete pDestStyle->pFont;
				pDestStyle->pFont = NULL;
			}
			if(pDestStyle->pFont)
				pDestStyle->mask |= cssMaskFont;
			else
				pDestStyle->mask = pDestStyle->mask & ~cssMaskFont;
		}

		//merge text
		if(pSrcStyle->mask & cssMaskText)
		{
			if(pSrcStyle->pText)
			{
				if(!pDestStyle->pText)
				{
					pDestStyle->pText = new TText;
					memcpy(pDestStyle->pText,pSrcStyle->pText,sizeof(TText));
				}else
				{
					if(pSrcStyle->pText->mask & cssMaskText)
						pDestStyle->pText->text = pSrcStyle->pText->text;
					if(pSrcStyle->pText->mask & cssMaskTextAlign)
						pDestStyle->pText->textalign= pSrcStyle->pText->textalign;
					if(pSrcStyle->pText->mask & cssMaskTextIndent)
					{
						pDestStyle->pText->textindent = pSrcStyle->pText->textindent;
						pDestStyle->pText->textindentmeasure = pSrcStyle->pText->textindentmeasure;
					}
					if(pSrcStyle->pText->mask & cssMaskTextDecoration)
						pDestStyle->pText->textdecoration = pSrcStyle->pText->textdecoration;
					if(pSrcStyle->pText->mask & cssMaskTextTransform)
						pDestStyle->pText->texttransform= pSrcStyle->pText->texttransform;
					if(pSrcStyle->pText->mask & cssMaskLetterSpacing)
					{
						pDestStyle->pText->letterspacing= pSrcStyle->pText->letterspacing;
						pDestStyle->pText->letterspacingmeasure= pSrcStyle->pText->letterspacingmeasure;
					}
					if(pSrcStyle->pText->mask & cssMaskWordSpacing)
					{
						pDestStyle->pText->wordspacing= pSrcStyle->pText->wordspacing;
						pDestStyle->pText->wordspacingmeasure= pSrcStyle->pText->wordspacingmeasure;
					}
					if(pSrcStyle->pText->mask & cssMaskWhiteSpace)
						pDestStyle->pText->whitespace= pSrcStyle->pText->whitespace;
				}
				pDestStyle->pText->mask |= pSrcStyle->pText->mask;

			}else if(pDestStyle->pText)
			{
				delete pDestStyle->pText;
				pDestStyle->pText = NULL;
			}
			if(pDestStyle->pText)
				pDestStyle->mask |= cssMaskText;
			else
				pDestStyle->mask = pDestStyle->mask & ~cssMaskText;
		}

		for(int i=0;i<5;i++)
		{
			//merge border
			if(pSrcStyle->mask & cssMaskBorder)
			{
				if(pSrcStyle->pBorder[i])
				{
					if(!pDestStyle->pBorder[i])
					{
						pDestStyle->pBorder[i] = new TBorder;
						memcpy(pDestStyle->pBorder[i],pSrcStyle->pBorder[i],sizeof(TBorder));
					}else
					{
						if(pSrcStyle->pBorder[i]->mask & cssMaskBorder)
							pDestStyle->pBorder[i]->border = pSrcStyle->pBorder[i]->border;
						if(pSrcStyle->pBorder[i]->mask & cssMaskBorderColor)
							pDestStyle->pBorder[i]->color = pSrcStyle->pBorder[i]->color;
						if(pSrcStyle->pBorder[i]->mask & cssMaskBorderWidth)
						{
							pDestStyle->pBorder[i]->width = pSrcStyle->pBorder[i]->width;
							pDestStyle->pBorder[i]->widthmesure = pSrcStyle->pBorder[i]->widthmesure;
						}
						if(pSrcStyle->pBorder[i]->mask & cssMaskBorderType)
							pDestStyle->pBorder[i]->bordertype = pSrcStyle->pBorder[i]->bordertype;
					}
					pDestStyle->pBorder[i]->mask |= pSrcStyle->pBorder[i]->mask;

				}else if(pDestStyle->pBorder[i])
				{
					delete pDestStyle->pBorder[i];
					pDestStyle->pBorder[i] = NULL;
				}
				if(pDestStyle->pBorder[i])
					pDestStyle->mask |= cssMaskBorder;
			}

			//merge padding
			if(pSrcStyle->mask & cssMaskPadding)
			{
				if( pSrcStyle->pPadding[i])
				{
					if(!pDestStyle->pPadding[i]) pDestStyle->pPadding[i] = new TPadding;
					memcpy(pDestStyle->pPadding[i],pSrcStyle->pPadding[i],sizeof(TPadding));
				}else if(pDestStyle->pPadding[i])
				{
					delete pDestStyle->pPadding[i];
					pDestStyle->pPadding[i] = NULL;
				}
				if(pDestStyle->pPadding[i])
					pDestStyle->mask |= cssMaskPadding;
			}

			//merge margin
			if(pSrcStyle->mask & cssMaskMargin)
			{
				if( pSrcStyle->pMargin[i])
				{
					if(!pDestStyle->pMargin[i]) pDestStyle->pMargin[i] = new TMargin;
					memcpy(pDestStyle->pMargin[i],pSrcStyle->pMargin[i],sizeof(TMargin));
				}else if(pDestStyle->pMargin[i])
				{
					delete pDestStyle->pMargin[i];
					pDestStyle->pMargin[i] = NULL;
				}
				if(pDestStyle->pMargin[i])
					pDestStyle->mask |= cssMaskMargin;
			}

		}

		//merge width
		if(pSrcStyle->mask & cssMaskWidth)
		{
			if( pSrcStyle->pWidth)
			{
				if(!pDestStyle->pWidth) pDestStyle->pWidth = new TMeasure;
				memcpy(pDestStyle->pWidth,pSrcStyle->pWidth,sizeof(TMeasure));
			}
			pDestStyle->mask |= cssMaskWidth;
		}

		//merge height
		if(pSrcStyle->mask & cssMaskHeight)
		{
			if( pSrcStyle->pHeight)
			{
				if(!pDestStyle->pHeight) pDestStyle->pHeight = new TMeasure;
				memcpy(pDestStyle->pHeight,pSrcStyle->pHeight,sizeof(TMeasure));
			}
			pDestStyle->mask |= cssMaskHeight;
		}

		return true;
	}

	xuCCssStyle::xuCCssStyle(void)
	{
		m_pCellStyle = new xuVector<TCssCellStyle* >;
		m_pCssFont = new xuVector<Css::TFont *> ;
		m_pCssText = new xuVector<Css::TText *> ;
		m_pCssBackGround = new xuVector<Css::TBackGround *>;
		m_pCssMargin = new xuVector<TCssMargins *>;
		m_pCssPadding = new xuVector<TCssPaddings *>;
		m_pCssColor = new xuVector<Css::TColor *>;
		m_pCssBorder = new xuVector<TCssBorders *> ;

		m_pCssWidth = new xuVector<Css::TMeasure *> ;
		m_pCssHeight = new xuVector<Css::TMeasure *> ;

	}

	xuCCssStyle::~xuCCssStyle(void)
	{
		if(m_pCellStyle)
		{
			for(int i=0;i<m_pCellStyle->size();i++)
			{
				if((*m_pCellStyle)[i])delete (*m_pCellStyle)[i];
			}
			delete m_pCellStyle;
			m_pCellStyle = NULL;
		}

		xuVector<void *>* pVoid[9];
		pVoid[0] = (xuVector<void *>*)m_pCssFont;
		pVoid[1] = (xuVector<void *>*)m_pCssText;
		pVoid[2] = (xuVector<void *>*)m_pCssBackGround;
		pVoid[3] = (xuVector<void *>*)m_pCssMargin;
		pVoid[4] = (xuVector<void *>*)m_pCssPadding;
		pVoid[5] = (xuVector<void *>*)m_pCssColor;
		pVoid[6] = (xuVector<void *>*)m_pCssBorder;
		pVoid[7] = (xuVector<void *>*)m_pCssWidth;
		pVoid[8] = (xuVector<void *>*)m_pCssHeight;
		for(int i=0;i<9;i++)
		{
			if(pVoid[i])
			{
				for(int j=0;i<pVoid[i]->size();i++)
				{
					if((*pVoid[i])[j])
					{
						if(i==0)delete (*m_pCssFont)[j];
						else if(i==1)delete (*m_pCssText)[j];
						else if(i==2)delete (*m_pCssBackGround)[j];
						else if(i==3)delete (*m_pCssMargin)[j];
						else if(i==4)delete (*m_pCssPadding)[j];
						else if(i==5)delete (*m_pCssColor)[j];
						else if(i==6)delete (*m_pCssBorder)[j];
						else if(i==7)delete (*m_pCssWidth)[j];
						else if(i==8)delete (*m_pCssHeight)[j];
					}
				}
			}
			if(i==0)delete m_pCssFont;
			else if(i==1)delete m_pCssText;
			else if(i==2)delete m_pCssBackGround;
			else if(i==3)delete m_pCssMargin;
			else if(i==4)delete m_pCssPadding;
			else if(i==5)delete m_pCssColor;
			else if(i==6)delete m_pCssBorder;
			else if(i==6)delete m_pCssWidth;
			else if(i==6)delete m_pCssHeight;
			pVoid[i] = NULL;
		}
	}

	const Css::TBoxStyle *  xuCCssStyle::GetBoxCssStyleFromCssCellStyle(TCssCellStyle * pCellStyle)
	{
		if (pCellStyle->m_pBoxStyle) return pCellStyle->m_pBoxStyle;

		Css::TBoxStyle * pBoxStyle = new Css::TBoxStyle();
		pBoxStyle->mask = pCellStyle->mask;
		if(pCellStyle->mask & cssMaskColor)
		{
			pBoxStyle->pColor = new Css::TColor;
			::memcpy(pBoxStyle->pColor,(*m_pCssColor)[pCellStyle->nColor],sizeof(Css::TColor));
		}
		if(pCellStyle->mask & cssMaskFont)
		{
			pBoxStyle->pFont= new Css::TFont;
			::memcpy(pBoxStyle->pFont,(*m_pCssFont)[pCellStyle->nFont],sizeof(Css::TFont));
			::memcpy(pBoxStyle->pFont->fontfacename,(*m_pCssFont)[pCellStyle->nFont]->fontfacename,sizeof(TCHAR)*LF_FACESIZE);
		}

		if(pCellStyle->mask & cssMaskWidth)
		{
			pBoxStyle->pWidth= new Css::TMeasure;
			::memcpy(pBoxStyle->pWidth,(*m_pCssWidth)[pCellStyle->nWidth],sizeof(Css::TMeasure));
		}
		if(pCellStyle->mask & cssMaskHeight)
		{
			pBoxStyle->pHeight= new Css::TMeasure;
			::memcpy(pBoxStyle->pHeight,(*m_pCssHeight)[pCellStyle->nHeight],sizeof(Css::TMeasure));
		}

		if(pCellStyle->mask & cssMaskText)
		{
			pBoxStyle->pText= new Css::TText;
			::memcpy(pBoxStyle->pText,(*m_pCssText)[pCellStyle->nText],sizeof(Css::TText));
		}
		if(pCellStyle->mask & cssMaskBackGround)
		{
			pBoxStyle->pBackGround= new Css::TBackGround;
			::memcpy(pBoxStyle->pBackGround,(*m_pCssBackGround)[pCellStyle->nBackGround],sizeof(Css::TBackGround));
		}
		if(pCellStyle->mask & cssMaskBorder)
		{
			for(int i=0;i<5;i++)
			{
				if((*m_pCssBorder)[pCellStyle->nBorder]->pBorder[i])
				{
					pBoxStyle->pBorder[i]= new Css::TBorder;
					::memcpy(pBoxStyle->pBorder[i],(*m_pCssBorder)[pCellStyle->nBorder]->pBorder[i],sizeof(Css::TBorder));
				}
			}
		}
		if(pCellStyle->mask & cssMaskMargin)
		{
			for(int i=0;i<5;i++)
			{
				if((*m_pCssMargin)[pCellStyle->nMargin]->pMargin[i])
				{
					pBoxStyle->pMargin[i]= new Css::TMargin;
					::memcpy(pBoxStyle->pMargin[i],(*m_pCssMargin)[pCellStyle->nMargin]->pMargin[i],sizeof(Css::TMargin));
				}
			}
		}
		if(pCellStyle->mask & cssMaskPadding)
		{
			for(int i=0;i<5;i++)
			{
				if((*m_pCssPadding)[pCellStyle->nPadding]->pPadding[i])
				{
					pBoxStyle->pPadding[i]= new Css::TPadding;
					::memcpy(pBoxStyle->pPadding[i],(*m_pCssPadding)[pCellStyle->nPadding]->pPadding[i],sizeof(Css::TPadding));
				}
			}
		}

		pCellStyle->m_pBoxStyle=pBoxStyle;
		return pBoxStyle;
	}
	
	int Css::SetFont(TBoxStyle * pBoxStyle,LOGFONT & logFont)
	{
		if(!pBoxStyle) 
			pBoxStyle = new TBoxStyle;
		pBoxStyle->mask |= cssMaskFont;
		pBoxStyle->mask |= cssMaskText;

		if(!pBoxStyle->pFont)pBoxStyle->pFont = new TFont;
		if(!pBoxStyle->pText)pBoxStyle->pText = new TText;
		pBoxStyle->pFont->mask |= cssMaskFontSize;
		pBoxStyle->pFont->fontsize = _refmeasure;
		pBoxStyle->pFont->fontsizemeasure.measureRef =_amount;
		pBoxStyle->pFont->fontsizemeasure.amount = - logFont.lfHeight;
		pBoxStyle->pFont->fontsizemeasure.unit = _px;

		pBoxStyle->pFont->mask |= cssMaskFontFaceName;
		_tcscpy_s(pBoxStyle->pFont->fontfacename ,_tcslen(logFont.lfFaceName) + 1, logFont.lfFaceName);
		pBoxStyle->pFont->mask |= cssMaskFontStyle;
		if(logFont.lfItalic == 1)
		{
			pBoxStyle->pFont->fontstyle=_italic;
		}else
		{
			pBoxStyle->pFont->fontstyle=_normal;
		}
		pBoxStyle->pText->mask |= cssMaskTextDecoration;
		if(logFont.lfUnderline == 1)
			pBoxStyle->pText->textdecoration=_underline;
		else if(logFont.lfStrikeOut == 1)
			pBoxStyle->pText->textdecoration=_line_through;
		else
			pBoxStyle->pText->textdecoration=_normal;

		pBoxStyle->pFont->mask |= cssMaskFontWeight;
		switch(logFont.lfWeight)
		{
		case FW_NORMAL:
			pBoxStyle->pFont->fontweight = _normal;
		case FW_BOLD:
			pBoxStyle->pFont->fontweight = _bold;
		case FW_MEDIUM:
			pBoxStyle->pFont->fontweight = _medium;
		case FW_LIGHT:
			pBoxStyle->pFont->fontweight = _lighter;
		default:
			{
				pBoxStyle->pFont->fontweight = _refmeasure;
				pBoxStyle->pFont->fontweightvalue = logFont.lfWeight;
			}
		}
		return 1;
	}

	int Css::GetDefaultFont(LOGFONT & logFont)
	{
		logFont.lfHeight = - MulDiv(9, _CssData.nYPixelPerInch, 72);
		logFont.lfCharSet = DEFAULT_CHARSET ;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfEscapement = 0;
		_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
		logFont.lfItalic = 0;
		logFont.lfOrientation = 0;
		logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logFont.lfPitchAndFamily = DEFAULT_PITCH;
		logFont.lfQuality = CLEARTYPE_QUALITY;
		logFont.lfStrikeOut = 0;
		logFont.lfUnderline = 0;
		logFont.lfWeight = FW_NORMAL;
		logFont.lfWidth = 0;
		return 1;
	}
	int Css::GetFont(const TBoxStyle * pBoxStyle,LOGFONT & logFont)
	{
		
		//default;
		GetDefaultFont(logFont);

		if(pBoxStyle && (pBoxStyle->mask & cssMaskFont ||pBoxStyle->mask & cssMaskText))
		{
			if(pBoxStyle->mask & cssMaskFont && pBoxStyle->pFont && pBoxStyle->pFont->mask & cssMaskFontSize)
			{
				double	nMediumFontSize = -12;
				double nFontSize = -9;
				switch(pBoxStyle->pFont->fontsize)
				{
					case _medium:
						nFontSize = nMediumFontSize ;
						break;
					case _small:
						nFontSize = nMediumFontSize /1.2;
						break;
					case _x_small:
						nFontSize = nMediumFontSize /1.2/1.2;
						break;
					case _xx_small:
						nFontSize = nMediumFontSize /1.2/1.2/1.2;
						break;
					case _large:
						nFontSize = nMediumFontSize *1.2;
						break;
					case _x_large:
						nFontSize = nMediumFontSize  *1.2*1.2;
						break;
					case _xx_large:
						nFontSize = nMediumFontSize *1.2*1.2*1.2;
						break;
					case _refmeasure :
						nFontSize = - GetPtExtent( &pBoxStyle->pFont->fontsizemeasure);
						break;
					default:
						nFontSize = -8;
				}
				logFont.lfHeight =  MulDiv((int)nFontSize, _CssData.nYPixelPerInch, 72);
			}
			logFont.lfCharSet = DEFAULT_CHARSET ;
			logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logFont.lfEscapement = 0;
			if(pBoxStyle->mask & cssMaskFont && pBoxStyle->pFont && pBoxStyle->pFont->mask & cssMaskFontFaceName && pBoxStyle->pFont->fontfacename)
				_tcscpy_s(logFont.lfFaceName ,_tcslen(pBoxStyle->pFont->fontfacename) + 1, pBoxStyle->pFont->fontfacename);
			else
				_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
			if(pBoxStyle->pFont && pBoxStyle->pFont->fontstyle==_italic)
				logFont.lfItalic = 1;
			else
				logFont.lfItalic = 0;
			logFont.lfOrientation = 0;
			logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logFont.lfPitchAndFamily = DEFAULT_PITCH;
			logFont.lfQuality = CLEARTYPE_QUALITY;
			if(pBoxStyle->mask & cssMaskText && pBoxStyle->pText && pBoxStyle->pText->mask & cssMaskTextDecoration)
			{
				if(pBoxStyle->pText && pBoxStyle->pText->textdecoration==_underline)
					logFont.lfUnderline = 1;
				else
					logFont.lfUnderline = 0;

				if(pBoxStyle->pText && pBoxStyle->pText->textdecoration==_line_through)
					logFont.lfStrikeOut = 1;
				else
					logFont.lfStrikeOut = 0;
			}
			if(pBoxStyle->mask & cssMaskFont && pBoxStyle->pFont && pBoxStyle->pFont->mask & cssMaskFontWeight && pBoxStyle->pFont->fontweight)
			{
				switch(pBoxStyle->pFont->fontweight)
				{
				case _normal:
					logFont.lfWeight = FW_NORMAL;
					break;
				case _bold:
					logFont.lfWeight = FW_BOLD;
					break;
				case _border:
					logFont.lfWeight = FW_MEDIUM;
					break;
				case _lighter:
					logFont.lfWeight =  FW_LIGHT;
					break;
				case _refmeasure:
					logFont.lfWeight = pBoxStyle->pFont->fontweightvalue;
					break;
				default:
					logFont.lfWeight = FW_NORMAL;
				}
				//logFont.lfWeight = FW_NORMAL;
			}
			logFont.lfWidth = 0;
		}

		return 0;
	}

	double	Css::GetPixelExtent(TMeasure * pMeasure,bool bHorz)
	{
		if(pMeasure->measureRef==_amount)
		{
			if(pMeasure->unit==_px) return pMeasure->amount; 
			return GetPtExtent(pMeasure,bHorz) * (bHorz?_CssData.nXPixelPerInch:_CssData.nYPixelPerInch) /72;
		}
		return 0;
	}

	double	Css::GetPtExtent(TMeasure * pMeasure,bool bHorz)
	{
		if(pMeasure->measureRef==_amount)
		{
			if(pMeasure->unit==_pt) return pMeasure->amount;
			if(pMeasure->unit==_px) return pMeasure->amount * 72 / (bHorz?_CssData.nXPixelPerInch:_CssData.nYPixelPerInch); 
			for(int i=0;i< sizeof(MeasureUnits)/sizeof(MeasureUnits[0]);i++)
			{	
				if(MeasureUnits[i].eUnit ==pMeasure->unit)
				{
					return pMeasure->amount * MeasureUnits[i].MulToPt/MeasureUnits[i].DivToPt;
				}
			}
		}
		return 0;
	}
	double	Css::GetBorderExtent(TBorder * pBorder,bool bHorz)
	{
		if(!pBorder) return 0;
		if(pBorder->mask & cssMaskBorderWidth)
		{
			switch(pBorder->width)
			{
			case _thin:
				return 1;
			case _medium:
				return 2;
			case _thick:
				return 3;
			case _refmeasure:
				return GetPixelExtent(&pBorder->widthmesure,bHorz);
			}
		}
		return 0;
	}


	bool ReleaseCellCssStyle(Css::TBoxStyle * pBoxStyle)
	{
		if(pBoxStyle)delete pBoxStyle;
		return true;
	}

	int xuCCssStyle::AddCssFont(Css::TFont * F)
	{
		for(int i=0;i<m_pCssFont->size();i++)
		{
			if(memcmp(F,(*m_pCssFont)[i],sizeof(Css::TFont))==0) return i;		
		}
		Css::TFont * pnF;
		pnF=new Css::TFont;
		memcpy(pnF,F,sizeof(Css::TFont));
		return  m_pCssFont->push_back(pnF);
	}

	int xuCCssStyle::AddCssWidth(Css::TMeasure* w)
	{
		for(int i=0;i<m_pCssWidth->size();i++)
		{
			if(memcmp(w,(*m_pCssWidth)[i],sizeof(Css::TMeasure))==0) return i;		
		}
		Css::TMeasure * pnF;
		pnF=new Css::TMeasure;
		memcpy(pnF,w,sizeof(Css::TMeasure));
		return  m_pCssWidth->push_back(pnF);
	}

	int xuCCssStyle::AddCssHeight(Css::TMeasure* h)
	{
		for(int i=0;i<m_pCssHeight->size();i++)
		{
			if(memcmp(h,(*m_pCssHeight)[i],sizeof(Css::TMeasure))==0) return i;		
		}
		Css::TMeasure * pnF;
		pnF=new Css::TMeasure;
		memcpy(pnF,h,sizeof(Css::TMeasure));
		return  m_pCssHeight->push_back(pnF);
	}



	int xuCCssStyle::AddCssText(Css::TText * T)
	{
		for(int i=0;i<m_pCssText->size();i++)
		{
			if(memcmp(T,(*m_pCssText)[i],sizeof(Css::TText))==0) return i;		
		}
		Css::TText * pnT;
		pnT=new Css::TText;
		memcpy(pnT,T,sizeof(Css::TText));
		return  m_pCssText->push_back(pnT);
	}

	int xuCCssStyle::AddCssBackGround( Css::TBackGround * B)
	{
		for(int i=0;i<m_pCssBackGround->size();i++)
		{
			if(memcmp(B,(*m_pCssBackGround)[i],sizeof(Css::TBackGround))==0) return i;		
		}
		Css::TBackGround * pnB;
		pnB=new Css::TBackGround;
		memcpy(pnB,B,sizeof(Css::TBackGround));
		return  m_pCssBackGround->push_back(pnB);
	}

	int xuCCssStyle::AddCssMargin(TCssMargins * M)
	{
		int j;
		for(int i=0;i<m_pCssMargin->size();i++)
		{ 
			for (j=0 ;j<5;j++)
			{
				if(M->pMargin[j] && ((*m_pCssMargin)[i]->pMargin)[j])
				{
					if(memcmp(M->pMargin[j],((*m_pCssMargin)[i]->pMargin)[j],sizeof(Css::TMargin)))break;
				}
				else if(M->pMargin[j] != ((*m_pCssMargin)[i]->pMargin)[j])break;
			}
			if(j>=5) return i;
		}

		TCssMargins * pnM;
		pnM=new TCssMargins;
		Css::TMargin *pnMa;
		for (int j=0;j<5;j++)
		{
			if(M->pMargin[j])
			{
				pnMa=new Css::TMargin;
				memcpy(pnMa,M->pMargin[j],sizeof(Css::TMargin));
				pnM->pMargin[j]=pnMa;
			}
		}
		return  m_pCssMargin->push_back(pnM);

	}
	int xuCCssStyle::AddCssPadding(TCssPaddings * P)
	{
		int j;
		for(int i=0;i<m_pCssPadding->size();i++)
		{ 
			for (j=0 ;j<5;j++)
			{
				if(P->pPadding[j] && ((*m_pCssPadding)[i]->pPadding)[j])
				{
					if(memcmp(P->pPadding[j],((*m_pCssPadding)[i]->pPadding)[j],sizeof(Css::TPadding)))break;
				}
				else if(P->pPadding[j] != ((*m_pCssPadding)[i]->pPadding)[j])break;
			}
			if(j>=5) return i;
		}

		TCssPaddings * pnP;
		pnP=new TCssPaddings;

		Css::TPadding *pnPa;
		for (int j=0;j<5;j++)
		{
			if(P->pPadding[j]) 
			{
				pnPa=new Css::TPadding;
				memcpy(pnPa,P->pPadding[j],sizeof(Css::TPadding));
				pnP->pPadding[j]=pnPa;
			}
		}
		return  m_pCssPadding->push_back(pnP);
	}
	int xuCCssStyle::AddCssColor(Css::TColor * C)
	{
		for(int i=0;i<m_pCssColor->size();i++)
		{
			if((*m_pCssColor)[i] && C)
			{
				if(memcmp(C,(*m_pCssColor)[i],sizeof(Css::TColor))==0) return i;
			}
			if(C != (*m_pCssColor)[i])
				break;
			else
				return i;
		}

		Css::TColor * pnC;
		pnC=new Css::TColor;
		memcpy(pnC,C,sizeof(Css::TColor));
		return  m_pCssColor->push_back(pnC);
	}


	int xuCCssStyle::AddCssBorder(TCssBorders * B)
	{
		int j;
		for(int i=0;i<m_pCssBorder->size();i++)
		{ 
			for (j=0 ;j<5;j++)
			{
				if(B->pBorder[j] && ((*m_pCssBorder)[i]->pBorder)[j])
				{
					if(memcmp(B->pBorder[j],((*m_pCssBorder)[i]->pBorder)[j],sizeof(Css::TBorder)))break;
				}
				else if(B->pBorder[j] != ((*m_pCssBorder)[i]->pBorder)[j])break;
			}
			if(j>=5) return i;
		}

		TCssBorders * pnB;
		pnB=new TCssBorders;

		Css::TBorder *pnBa;
		for (int j=0;j<5;j++)
		{
			if(B->pBorder[j])
			{
				pnBa=new Css::TBorder;
				memcpy(pnBa,B->pBorder[j],sizeof(Css::TBorder));
				pnB->pBorder[j]=pnBa;
			}
		}
		return  m_pCssBorder->push_back(pnB);
	}

	int xuCCssStyle::AddCssCellStyle( TCssCellStyle * pCellStyle)
	{
		for(int i=0;i<m_pCellStyle->size();i++)
		{
			if ((pCellStyle->mask ==(*m_pCellStyle)[i]->mask) &&
				(pCellStyle->nFont ==(*m_pCellStyle)[i]->nFont)&&
				(pCellStyle->nText ==(*m_pCellStyle)[i]->nText)&&
				(pCellStyle->nBackGround ==(*m_pCellStyle)[i]->nBackGround)&&
				(pCellStyle->nMargin ==(*m_pCellStyle)[i]->nMargin)&&
				(pCellStyle->nPadding ==(*m_pCellStyle)[i]->nPadding)&&
				(pCellStyle->nColor ==(*m_pCellStyle)[i]->nColor)&&
				(pCellStyle->nBorder ==(*m_pCellStyle)[i]->nBorder) &&
				(pCellStyle->nWidth ==(*m_pCellStyle)[i]->nWidth) &&
				(pCellStyle->nHeight ==(*m_pCellStyle)[i]->nHeight)
			 )
			 return i;
		}

		TCssCellStyle * pnC;
		pnC=new TCssCellStyle;
		pnC->mask =pCellStyle->mask;
		pnC->nFont =pCellStyle->nFont;
		pnC->nText =pCellStyle->nText;
		pnC->nBackGround =pCellStyle->nBackGround;
		pnC->nMargin =pCellStyle->nMargin;
		pnC->nPadding =pCellStyle->nPadding;
		pnC->nColor =pCellStyle->nColor;
		pnC->nBorder =pCellStyle->nBorder;
		pnC->nWidth=pCellStyle->nWidth;
		pnC->nHeight =pCellStyle->nHeight;

		return  m_pCellStyle->push_back(pnC);

	}

	int xuCCssStyle::AddCssCellStyle( const Css::TBoxStyle * pBoxStyle)
	{
		TCssCellStyle * pnC;
		pnC=new TCssCellStyle;

		pnC->mask =pBoxStyle->mask;
		if(pBoxStyle->mask &cssMaskFont)
			pnC->nFont =AddCssFont(pBoxStyle->pFont);

		if(pBoxStyle->mask &cssMaskWidth)
			pnC->nWidth =AddCssWidth(pBoxStyle->pWidth);

		if(pBoxStyle->mask &cssMaskHeight)
			pnC->nHeight =AddCssHeight(pBoxStyle->pHeight);

		if(pBoxStyle->mask &cssMaskText)
			pnC->nText =AddCssText(pBoxStyle->pText);
		if(pBoxStyle->mask &cssMaskBackGround)
			pnC->nBackGround =AddCssBackGround(pBoxStyle->pBackGround);

		if(pBoxStyle->mask &cssMaskMargin)
		{
			TCssMargins tm;	 
			for (int i=0;i<5;i++)
				if(pBoxStyle->pMargin[i] )
				{
					tm.pMargin[i] = new Css::TMargin();
					memcpy(tm.pMargin[i],pBoxStyle->pMargin[i],sizeof(Css::TMargin));
				}
			pnC->nMargin =AddCssMargin(&tm);
		}

		if(pBoxStyle->mask &cssMaskPadding)
		{
			TCssPaddings tp;	 
			for (int i=0;i<5;i++)
			{
				if(pBoxStyle->pPadding[i] )
				{
					tp.pPadding[i] = new Css::TPadding();
					memcpy(tp.pPadding[i],pBoxStyle->pPadding[i],sizeof(Css::TPadding));
				}
			}
			pnC->nPadding =AddCssPadding(&tp);
		}

		if(pBoxStyle->mask &cssMaskColor)

			pnC->nColor =AddCssColor(pBoxStyle->pColor );

		if(pBoxStyle->mask &cssMaskBorder)
		{
			TCssBorders tb;	 
			for (int i=0;i<5;i++)
			{
				if(pBoxStyle->pBorder[i] )
				{
					tb.pBorder[i] = new Css::TBorder();
					memcpy(tb.pBorder[i],pBoxStyle->pBorder[i],sizeof(Css::TBorder));
				}
			}
			pnC->nBorder=AddCssBorder(&tb);
		}
		return  AddCssCellStyle(pnC);
	}

	int xuCCssStyle::GetCssStyles(LPCTSTR cssStyleStr,xuVector<LPTSTR> &cssStyles, xuVector<LPTSTR> &cssValues)
	{

		LPTSTR csStr;
		LPTSTR csPos;
		LPTSTR csStyle,csValue;
		TCHAR	ch;

		csStr = (LPTSTR)cssStyleStr;
		if(!csStr) return 0;
		while(csStr[0]==' '||csStr[0]=='\r'||csStr[0]=='\n'||csStr[0]=='\t'||csStr[0]==';')csStr++;

		csPos = ::_tcsstr(csStr,_T(":"));
		while(csPos)
		{
			csStyle = new TCHAR[csPos - csStr + 1];
			ch = csPos[0];
			csPos[0]='\0';
			::lstrcpy(csStyle,csStr);
			csPos[0] = ch;
			::_tcslwr_s(csStyle,_tcslen(csStyle)+1);
			cssStyles.push_back(csStyle);
			csStr = csPos + 1;
			csPos = ::_tcsstr(csStr,_T(";"));
			if(csPos)
			{
				ch = csPos[0];
				csPos[0]='\0';
				csValue = new TCHAR[csPos - csStr + 1];
				::lstrcpy(csValue,csStr);
				csPos[0] = ch;
				cssValues.push_back(csValue);
				csStr = csPos + 1;
			}else
			{
				csValue = new TCHAR[::lstrlen(csStr) + 1];
				::lstrcpy(csValue,csStr);
				::_tcslwr_s(csValue,_tcslen(csValue)+1);
				cssValues.push_back(csValue);
				break;
			}
			while(csStr[0]==' '||csStr[0]=='\r'||csStr[0]=='\n'||csStr[0]=='\t')csStr++;
			csPos = ::_tcsstr(csStr,_T(":"));
		}
		return 1;
	}

	const Css::TBoxStyle * xuCCssStyle::GetBoxStyle(int nIndex)
	{
		if(!m_pCellStyle || nIndex <0 || nIndex >=this->m_pCellStyle->size()) return NULL;
		return GetBoxCssStyleFromCssCellStyle((*m_pCellStyle)[nIndex]);
	}

	Css::TBoxStyle * xuCCssStyle::ParseCssStyle(LPCTSTR cssStyleStr)
	{
		int start = 0;
		xuVector<LPTSTR> cssStyles;
		xuVector<LPTSTR> cssValue;
		GetCssStyles(cssStyleStr,cssStyles,cssValue);

		Css::TBoxStyle * pBoxStyle = new Css::TBoxStyle();

		DWORD dwMask;
		for(int i= (int)cssStyles.size() - 1;i>=start;i--)
		{
			dwMask = Css::GetCssMask(cssStyles[i]);
			if(dwMask & cssMaskMargin) Css::ProcessCssMargin(cssValue[i],dwMask,pBoxStyle);
			if(dwMask & cssMaskPadding) Css::ProcessCssPadding(cssValue[i],dwMask,pBoxStyle);
			if(dwMask & cssMaskBorder) Css::ProcessCssBorder(cssValue[i],dwMask,pBoxStyle);
			if(dwMask & cssMaskBackGround) Css::ProcessCssBackground(cssValue[i],dwMask,pBoxStyle);
			if(dwMask & cssMaskFont) Css::ProcessCssFont(cssValue[i],dwMask,pBoxStyle);
			if(dwMask & cssMaskText) Css::ProcessCssText(cssValue[i],dwMask,pBoxStyle);
			if(dwMask == cssMaskWidth)
			{
				if(!pBoxStyle->pWidth)pBoxStyle->pWidth = new Css::TMeasure;
				Css::ProcessCssMeasure(cssValue[i],pBoxStyle->pWidth);
				pBoxStyle->mask |= cssMaskWidth;
			}
			if(dwMask == cssMaskHeight)
			{
				if(!pBoxStyle->pHeight)pBoxStyle->pHeight = new Css::TMeasure;
				Css::ProcessCssMeasure(cssValue[i],pBoxStyle->pHeight);
				pBoxStyle->mask |= cssMaskHeight; 
			}
			
			if(dwMask == cssMaskColor)
			{
				if(!pBoxStyle->pColor) pBoxStyle->pColor = new Css::TColor;
				Css::ProcessCssColor(cssValue[i],*pBoxStyle->pColor);
				pBoxStyle->mask |= cssMaskColor;
			}
		}

		return pBoxStyle;
	}

	int GetMeasureStyleStr( Css::TMeasure & measure,LPTSTR pCssStr)
	{
		if(measure.measureRef==Css::_auto)
		{
			::_stprintf_s(pCssStr,50,_T("auto"));
			return 1;
		}
		if(measure.measureRef==Css::_percentage)
		{
			::_stprintf_s(pCssStr,50,_T("%f%s"),measure.percentage,_T("%"));
			return 1;
		}
		::_stprintf_s(pCssStr,50,_T("%f"),measure.amount);
		
		int cnt = sizeof(MeasureUnits)/sizeof(MeasureUnits[0]);
		for(int m=0;m<cnt;m++)
		{
			if(MeasureUnits[m].eUnit== measure.unit)
			{
				::_stprintf_s(pCssStr,50,_T("%s%s"),pCssStr,MeasureUnits[m].name);
				break;
			}
		}

		return 1;
	}

	void xuCCssStyle::GetStyles(std::map<std::wstring,std::wstring> & items)
	{
		//std::wstring styles;
		std::wstring str;

		TCHAR StrName[30];
		TCHAR StrBuf[255];
		TCHAR StrMeasure[50];
		//font
		for(int i=0;i<m_pCssFont->size();i++)
		{
			::_stprintf_s(StrName,30,_T("f%i"),i);
			StrBuf[0]='\0';
			if((*m_pCssFont)[i]->mask & cssMaskFontFamily)
			{
				int cnt = sizeof(CssFontFamily)/sizeof(CssFontFamily[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontFamily[m].item== (*m_pCssFont)[i]->fontfamily)
					{
						::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("font-family"),CssFontFamily[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontFaceName)
			{
				//::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("font-facename"),(*m_pCssFont)[i]->fontfacename);
				::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("font-family"),(*m_pCssFont)[i]->fontfacename);
			
			}
			
			if((*m_pCssFont)[i]->mask & cssMaskFontStyle)
			{
				int cnt = sizeof(CssFontStyle)/sizeof(CssFontStyle[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontStyle[m].item== (*m_pCssFont)[i]->fontstyle)
					{
						::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("font-style"),CssFontStyle[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontVariant)
			{
				int cnt = sizeof(CssFontVariant)/sizeof(CssFontVariant[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontVariant[m].item== (*m_pCssFont)[i]->fontvariant)
					{
						::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("font-variant"),CssFontVariant[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontWeight)
			{
				if((*m_pCssFont)[i]->fontweight==Css::_refmeasure)
				{
					::_stprintf_s(StrBuf,255,_T("%s;%s:%i"),StrBuf,_T("font-weight"),(*m_pCssFont)[i]->fontweightvalue);
				}else
				{
					int cnt = sizeof(CssFontWeight)/sizeof(CssFontWeight[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssFontWeight[m].item== (*m_pCssFont)[i]->fontweight)
						{
							::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("font-weight"),CssFontWeight[m].name);
							break;
						}
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontSize)
			{
				if((*m_pCssFont)[i]->fontsize==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssFont)[i]->fontsizemeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s%s:%s;"),StrBuf,_T("font-size"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssFontSize)/sizeof(CssFontSize[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssFontSize[m].item== (*m_pCssFont)[i]->fontsize)
						{
							::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("font-size"),CssFontSize[m].name);
							break;
						}
					}
				}
			}
			items.insert(std::pair<std::wstring,std::wstring>(StrName,StrBuf));
		}

		//text
		for(int i=0;i<m_pCssText->size();i++)
		{
			::_stprintf_s(StrName,30,_T("t%i"),i);
			
			StrBuf[0]='\0';
			if((*m_pCssText)[i]->mask & cssMaskTextIndent)
			{
				GetMeasureStyleStr( (*m_pCssText)[i]->textindentmeasure,StrMeasure);
				::_stprintf_s(StrBuf ,255,_T("%s%s:%s;"),StrBuf,_T("text-indent"),StrMeasure);
			}
			if((*m_pCssText)[i]->mask & cssMaskTextAlign)
			{
				int cnt = sizeof(CssTextAlign)/sizeof(CssTextAlign[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextAlign[m].item== (*m_pCssText)[i]->textalign)
					{
						::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("text-align"),CssTextAlign[m].name);
						break;
					}
				}
			}
			
			if((*m_pCssText)[i]->mask & cssMaskTextDecoration)
			{
				int cnt = sizeof(CssTextDecoration)/sizeof(CssTextDecoration[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextDecoration[m].item== (*m_pCssText)[i]->textdecoration)
					{
						::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("text-decoration"),CssTextDecoration[m].name);
						break;
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskLetterSpacing)
			{
				if((*m_pCssText)[i]->letterspacing==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssText)[i]->letterspacingmeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s%s:%s;"),StrBuf,_T("letter-spacing"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssLetterSpacing)/sizeof(CssLetterSpacing[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssLetterSpacing[m].item== (*m_pCssText)[i]->letterspacing)
						{
							::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("letter-spacing"),CssLetterSpacing[m].name);
							break;
						}
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskWordSpacing)
			{
				if((*m_pCssText)[i]->wordspacing==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssText)[i]->wordspacingmeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s%s:%s;"),StrBuf,_T("word-spacing"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssWordSpacing)/sizeof(CssWordSpacing[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssWordSpacing[m].item== (*m_pCssText)[i]->wordspacing)
						{
							::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("word-spacing"),CssWordSpacing[m].name);
							break;
						}
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskWhiteSpace)
			{
				int cnt = sizeof(CssWhiteSpace)/sizeof(CssWhiteSpace[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssWhiteSpace[m].item== (*m_pCssText)[i]->whitespace)
					{
						::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("white-space"),CssWhiteSpace[m].name);
						break;
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskTextTransform)
			{
				int cnt = sizeof(CssTextTransform)/sizeof(CssTextTransform[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextTransform[m].item== (*m_pCssText)[i]->texttransform)
					{
						::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("text-transform"),CssTextTransform[m].name);
						break;
					}
				}
			}
			items.insert(std::pair<std::wstring,std::wstring>(StrName,StrBuf));
		}

		//border
		for(int i=0;i<this->m_pCssBorder->size();i++)
		{
			::_stprintf_s(StrName,30,_T("b%i"),i);
			
			StrBuf[0]='\0';
			for(int k=0;k<5;k++)
			{
				if((*m_pCssBorder)[i]->pBorder[k])
				{
					Css::TBorder* pBorder = (*m_pCssBorder)[i]->pBorder[k];
					if(pBorder->mask & cssMaskBorderType)
					{
						int cnt = sizeof(CssBorderTypes)/sizeof(CssBorderTypes[0]);
						for(int m=0;m<cnt;m++)
						{
							if(CssBorderTypes[m].item== pBorder->bordertype)
							{
								if(k==0)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-style"),CssBorderTypes[m].name);
								if(k==1)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-left-style"),CssBorderTypes[m].name);
								if(k==2)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-top-style"),CssBorderTypes[m].name);
								if(k==3)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-right-style"),CssBorderTypes[m].name);
								if(k==4)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-bottom-style"),CssBorderTypes[m].name);
								break;
							}
						}
					}

					if(pBorder->mask & cssMaskBorderWidth)
					{
						if(pBorder->width==Css::_refmeasure)
						{
							GetMeasureStyleStr( pBorder->widthmesure,StrMeasure);
							if(k==0)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-width"),StrMeasure);
							if(k==1)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-left-width"),StrMeasure);
							if(k==2)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-top-width"),StrMeasure);
							if(k==3)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-right-width"),StrMeasure);
							if(k==4)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-bottom-width"),StrMeasure);
						}else
						{
							int cnt = sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]);
							for(int m=0;m<cnt;m++)
							{
								if(CssBorderWidths[m].item== pBorder->width)
								{
									if(k==0)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-width"),CssBorderWidths[m].name);
									if(k==1)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-left-width"),CssBorderWidths[m].name);
									if(k==2)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-top-width"),CssBorderWidths[m].name);
									if(k==3)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-right-width"),CssBorderWidths[m].name);
									if(k==4)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-bottom-width"),CssBorderWidths[m].name);
									break;
								}
							}
						}
					}
					if(pBorder->mask & cssMaskBorderColor)
					{
						/*
						if(pBorder->width==Css::_refmeasure)
						{
							GetMeasureStyleStr( pBorder->widthmesure,StrMeasure);
							if(k==0)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-width"),StrMeasure);
							if(k==1)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-left-width"),StrMeasure);
							if(k==2)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-top-width"),StrMeasure);
							if(k==3)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-right-width"),StrMeasure);
							if(k==4)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-bottom-width"),StrMeasure);
						}else
						{
							int cnt = sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]);
							for(int m=0;m<cnt;m++)
							{
								if(CssBorderWidths[m].item== pBorder->width)
								{
									if(k==0)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-width"),CssBorderWidths[m].name);
									if(k==1)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-left-width"),CssBorderWidths[m].name);
									if(k==2)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-top-width"),CssBorderWidths[m].name);
									if(k==3)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-right-width"),CssBorderWidths[m].name);
									if(k==4)::_stprintf_s(StrBuf,255,_T("%s%s:%s;"),StrBuf,_T("border-bottom-width"),CssBorderWidths[m].name);
									break;
								}
							}
						}
						*/
					}
				}
			}
			items.insert(std::pair<std::wstring,std::wstring>(StrName,StrBuf));
		}

		//color
		for(int i=0;i<this->m_pCssColor->size();i++)
		{
			::_stprintf_s(StrName,30,_T("c%i"),i);
			
			StrBuf[0]='\0';
			::_stprintf_s(StrBuf,255,_T("%s%s:#%02x%02x%02x;"),StrBuf,_T("color"),(*m_pCssColor)[i]->color[0]&0xff,(*m_pCssColor)[i]->color[0]>>8&0xff,(*m_pCssColor)[i]->color[0]>>16&0xff);

			items.insert(std::pair<std::wstring,std::wstring>(StrName,StrBuf));
		}

		//background
		for(int i=0;i<this->m_pCssBackGround->size();i++)
		{
			::_stprintf_s(StrName,30,_T("g%i"),i);
			
			StrBuf[0]='\0';
			if((*m_pCssBackGround)[i]->color.mask & cssMaskColor)
			{
				::_stprintf_s(StrBuf,255,_T("%s%s:#%02x%02x%02x;"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[0]&0xff,(*m_pCssBackGround)[i]->color.color[0]>>8&0xff,(*m_pCssBackGround)[i]->color.color[0]>>16&0xff);
			}
			if((*m_pCssBackGround)[i]->color.mask & cssMaskLeft && (*m_pCssBackGround)[i]->color.colorRef[1]!=XUtil::Css::_undef && 
				(*m_pCssBackGround)[i]->color.mask & cssMaskRight && (*m_pCssBackGround)[i]->color.colorRef[3]!=XUtil::Css::_undef)
			{
				::_stprintf_s(StrBuf,255,_T("%s%s:none #%02x%02x%02x none #%02x%02x%02x;"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[3]&0xff,(*m_pCssBackGround)[i]->color.color[3]>>8&0xff,(*m_pCssBackGround)[i]->color.color[3]>>16&0xff,
					(*m_pCssBackGround)[i]->color.color[1],(*m_pCssBackGround)[i]->color.color[1]>>8,(*m_pCssBackGround)[i]->color.color[1]>>16);

			}else if((*m_pCssBackGround)[i]->color.mask & cssMaskTop && (*m_pCssBackGround)[i]->color.colorRef[2]!=XUtil::Css::_undef && 
				(*m_pCssBackGround)[i]->color.mask & cssMaskBottom && (*m_pCssBackGround)[i]->color.colorRef[4]!=XUtil::Css::_undef)
			{
				::_stprintf_s(StrBuf,255,_T("%s%s:#%02x%02x%02x none #%02x%02x%02x none;"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[2]&0xff,(*m_pCssBackGround)[i]->color.color[2]>>8&0xff,(*m_pCssBackGround)[i]->color.color[2]>>16&0xff,
					(*m_pCssBackGround)[i]->color.color[4]&0xff,(*m_pCssBackGround)[i]->color.color[4]>>8&0xff,(*m_pCssBackGround)[i]->color.color[4]>>16&0xff);
			}
					
			items.insert(std::pair<std::wstring,std::wstring>(StrName,StrBuf));
		}

		return ;
	}

	std::wstring xuCCssStyle::GetStylesString()
	{
		std::wstring styles;
		std::wstring str;

		TCHAR StrName[30];
		TCHAR StrBuf[255];
		TCHAR StrMeasure[50];
		//font
		for(int i=0;i<m_pCssFont->size();i++)
		{
			::_stprintf_s(StrName,30,_T("f%i"),i);
			StrBuf[0]='\0';
			if((*m_pCssFont)[i]->mask & cssMaskFontFamily)
			{
				int cnt = sizeof(CssFontFamily)/sizeof(CssFontFamily[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontFamily[m].item== (*m_pCssFont)[i]->fontfamily)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-family"),CssFontFamily[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontFaceName)
			{
				//::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-facename"),(*m_pCssFont)[i]->fontfacename);
				::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-family"),(*m_pCssFont)[i]->fontfacename);
			
			}
			
			if((*m_pCssFont)[i]->mask & cssMaskFontStyle)
			{
				int cnt = sizeof(CssFontStyle)/sizeof(CssFontStyle[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontStyle[m].item== (*m_pCssFont)[i]->fontstyle)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-style"),CssFontStyle[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontVariant)
			{
				int cnt = sizeof(CssFontVariant)/sizeof(CssFontVariant[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontVariant[m].item== (*m_pCssFont)[i]->fontvariant)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-variant"),CssFontVariant[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontWeight)
			{
				if((*m_pCssFont)[i]->fontweight==Css::_refmeasure)
				{
					::_stprintf_s(StrBuf,255,_T("%s;%s:%i"),StrBuf,_T("font-weight"),(*m_pCssFont)[i]->fontweightvalue);
				}else
				{
					int cnt = sizeof(CssFontWeight)/sizeof(CssFontWeight[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssFontWeight[m].item== (*m_pCssFont)[i]->fontweight)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-weight"),CssFontWeight[m].name);
							break;
						}
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontSize)
			{
				if((*m_pCssFont)[i]->fontsize==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssFont)[i]->fontsizemeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("font-size"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssFontSize)/sizeof(CssFontSize[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssFontSize[m].item== (*m_pCssFont)[i]->fontsize)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-size"),CssFontSize[m].name);
							break;
						}
					}
				}
			}
			if(StrBuf[0]!=';')
				styles += (std::wstring)L"." + StrName + L"\r\n{" +  StrBuf + L";}\r\n";
			else
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ (StrBuf+1) + L";}\r\n";
		}

		//text
		for(int i=0;i<m_pCssText->size();i++)
		{
			::_stprintf_s(StrName,30,_T("t%i"),i);
			
			StrBuf[0]='\0';
			if((*m_pCssText)[i]->mask & cssMaskTextIndent)
			{
				GetMeasureStyleStr( (*m_pCssText)[i]->textindentmeasure,StrMeasure);
				::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("text-indent"),StrMeasure);
			}
			if((*m_pCssText)[i]->mask & cssMaskTextAlign)
			{
				int cnt = sizeof(CssTextAlign)/sizeof(CssTextAlign[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextAlign[m].item== (*m_pCssText)[i]->textalign)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("text-align"),CssTextAlign[m].name);
						break;
					}
				}
			}
			
			if((*m_pCssText)[i]->mask & cssMaskTextDecoration)
			{
				int cnt = sizeof(CssTextDecoration)/sizeof(CssTextDecoration[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextDecoration[m].item== (*m_pCssText)[i]->textdecoration)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("text-decoration"),CssTextDecoration[m].name);
						break;
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskLetterSpacing)
			{
				if((*m_pCssText)[i]->letterspacing==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssText)[i]->letterspacingmeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("letter-spacing"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssLetterSpacing)/sizeof(CssLetterSpacing[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssLetterSpacing[m].item== (*m_pCssText)[i]->letterspacing)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("letter-spacing"),CssLetterSpacing[m].name);
							break;
						}
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskWordSpacing)
			{
				if((*m_pCssText)[i]->wordspacing==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssText)[i]->wordspacingmeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("word-spacing"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssWordSpacing)/sizeof(CssWordSpacing[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssWordSpacing[m].item== (*m_pCssText)[i]->wordspacing)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("word-spacing"),CssWordSpacing[m].name);
							break;
						}
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskWhiteSpace)
			{
				int cnt = sizeof(CssWhiteSpace)/sizeof(CssWhiteSpace[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssWhiteSpace[m].item== (*m_pCssText)[i]->whitespace)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("white-space"),CssWhiteSpace[m].name);
						break;
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskTextTransform)
			{
				int cnt = sizeof(CssTextTransform)/sizeof(CssTextTransform[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextTransform[m].item== (*m_pCssText)[i]->texttransform)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("text-transform"),CssTextTransform[m].name);
						break;
					}
				}
			}
			if(StrBuf[0]!=';')
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ StrBuf+L";}\r\n";
			else
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ (StrBuf+1) + L";}\r\n";
		}

		//border
		for(int i=0;i<this->m_pCssBorder->size();i++)
		{
			::_stprintf_s(StrName,30,_T("b%i"),i);
			
			StrBuf[0]='\0';
			for(int k=0;k<5;k++)
			{
				if((*m_pCssBorder)[i]->pBorder[k])
				{
					Css::TBorder* pBorder = (*m_pCssBorder)[i]->pBorder[k];
					if(pBorder->mask & cssMaskBorderType)
					{
						int cnt = sizeof(CssBorderTypes)/sizeof(CssBorderTypes[0]);
						for(int m=0;m<cnt;m++)
						{
							if(CssBorderTypes[m].item== pBorder->bordertype)
							{
								if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-style"),CssBorderTypes[m].name);
								if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-style"),CssBorderTypes[m].name);
								if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-style"),CssBorderTypes[m].name);
								if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-style"),CssBorderTypes[m].name);
								if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-style"),CssBorderTypes[m].name);
								break;
							}
						}
					}

					if(pBorder->mask & cssMaskBorderWidth)
					{
						if(pBorder->width==Css::_refmeasure)
						{
							GetMeasureStyleStr( pBorder->widthmesure,StrMeasure);
							if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),StrMeasure);
							if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),StrMeasure);
							if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),StrMeasure);
							if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),StrMeasure);
							if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),StrMeasure);
						}else
						{
							int cnt = sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]);
							for(int m=0;m<cnt;m++)
							{
								if(CssBorderWidths[m].item== pBorder->width)
								{
									if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),CssBorderWidths[m].name);
									if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),CssBorderWidths[m].name);
									if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),CssBorderWidths[m].name);
									if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),CssBorderWidths[m].name);
									if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),CssBorderWidths[m].name);
									break;
								}
							}
						}
					}
					if(pBorder->mask & cssMaskBorderColor)
					{
						/*
						if(pBorder->width==Css::_refmeasure)
						{
							GetMeasureStyleStr( pBorder->widthmesure,StrMeasure);
							if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),StrMeasure);
							if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),StrMeasure);
							if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),StrMeasure);
							if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),StrMeasure);
							if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),StrMeasure);
						}else
						{
							int cnt = sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]);
							for(int m=0;m<cnt;m++)
							{
								if(CssBorderWidths[m].item== pBorder->width)
								{
									if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),CssBorderWidths[m].name);
									if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),CssBorderWidths[m].name);
									if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),CssBorderWidths[m].name);
									if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),CssBorderWidths[m].name);
									if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),CssBorderWidths[m].name);
									break;
								}
							}
						}
						*/
					}
				}
			}
			if(StrBuf[0]!=';')
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ StrBuf+L";}\r\n";
			else
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ (StrBuf+1) + L";}\r\n";
		}

		//color
		for(int i=0;i<this->m_pCssColor->size();i++)
		{
			::_stprintf_s(StrName,30,_T("c%i"),i);
			
			StrBuf[0]='\0';
			::_stprintf_s(StrBuf,255,_T("%s;%s:#%02x%02x%02x"),StrBuf,_T("color"),(*m_pCssColor)[i]->color[0]&0xff,(*m_pCssColor)[i]->color[0]>>8&0xff,(*m_pCssColor)[i]->color[0]>>16&0xff);

			if(StrBuf[0]!=';')
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ StrBuf+L";}\r\n";
			else
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ (StrBuf+1) + L";}\r\n";
		}

		//background
		for(int i=0;i<this->m_pCssBackGround->size();i++)
		{
			::_stprintf_s(StrName,30,_T("g%i"),i);
			
			StrBuf[0]='\0';
			if((*m_pCssBackGround)[i]->color.mask & cssMaskColor)
			{
				::_stprintf_s(StrBuf,255,_T("%s;%s:#%02x%02x%02x"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[0]&0xff,(*m_pCssBackGround)[i]->color.color[0]>>8&0xff,(*m_pCssBackGround)[i]->color.color[0]>>16&0xff);
			}
			if((*m_pCssBackGround)[i]->color.mask & cssMaskLeft && (*m_pCssBackGround)[i]->color.colorRef[1]!=XUtil::Css::_undef && 
				(*m_pCssBackGround)[i]->color.mask & cssMaskRight && (*m_pCssBackGround)[i]->color.colorRef[3]!=XUtil::Css::_undef)
			{
				::_stprintf_s(StrBuf,255,_T("%s;%s:none #%02x%02x%02x none #%02x%02x%02x"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[3]&0xff,(*m_pCssBackGround)[i]->color.color[3]>>8&0xff,(*m_pCssBackGround)[i]->color.color[3]>>16&0xff,
					(*m_pCssBackGround)[i]->color.color[1],(*m_pCssBackGround)[i]->color.color[1]>>8,(*m_pCssBackGround)[i]->color.color[1]>>16);

			}else if((*m_pCssBackGround)[i]->color.mask & cssMaskTop && (*m_pCssBackGround)[i]->color.colorRef[2]!=XUtil::Css::_undef && 
				(*m_pCssBackGround)[i]->color.mask & cssMaskBottom && (*m_pCssBackGround)[i]->color.colorRef[4]!=XUtil::Css::_undef)
			{
				::_stprintf_s(StrBuf,255,_T("%s;%s:#%02x%02x%02x none #%02x%02x%02x none"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[2]&0xff,(*m_pCssBackGround)[i]->color.color[2]>>8&0xff,(*m_pCssBackGround)[i]->color.color[2]>>16&0xff,
					(*m_pCssBackGround)[i]->color.color[4]&0xff,(*m_pCssBackGround)[i]->color.color[4]>>8&0xff,(*m_pCssBackGround)[i]->color.color[4]>>16&0xff);
			}
					
			if(StrBuf[0]!=';')
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ StrBuf+L";}\r\n";
			else
				styles += (std::wstring)L"." + StrName + L"\r\n{"+ (StrBuf+1) + L";}\r\n";
		}

		return styles;
	}

	int xuCCssStyle::SetStyleSheet(IXMLDOMElement * pElement,BSTR pTagName)
	{
		IXMLDOMDocument * pXmlDoc;
		IXMLDOMElement * pItemElement;
		TCHAR StrName[30];
		TCHAR StrBuf[255];
		TCHAR StrMeasure[50];

		pElement->get_ownerDocument(&pXmlDoc);
		//font
		for(int i=0;i<m_pCssFont->size();i++)
		{
			pXmlDoc->createElement(pTagName,&pItemElement);
			::_stprintf_s(StrName,30,_T("f%i"),i);
			pItemElement->setAttribute(L"name",(_variant_t)StrName);
			
			StrBuf[0]='\0';
			if((*m_pCssFont)[i]->mask & cssMaskFontFamily)
			{
				int cnt = sizeof(CssFontFamily)/sizeof(CssFontFamily[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontFamily[m].item== (*m_pCssFont)[i]->fontfamily)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-family"),CssFontFamily[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontFaceName)
			{
				//::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-facename"),(*m_pCssFont)[i]->fontfacename);
				::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-family"),(*m_pCssFont)[i]->fontfacename);
			
			}
			
			if((*m_pCssFont)[i]->mask & cssMaskFontStyle)
			{
				int cnt = sizeof(CssFontStyle)/sizeof(CssFontStyle[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontStyle[m].item== (*m_pCssFont)[i]->fontstyle)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-style"),CssFontStyle[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontVariant)
			{
				int cnt = sizeof(CssFontVariant)/sizeof(CssFontVariant[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssFontVariant[m].item== (*m_pCssFont)[i]->fontvariant)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-variant"),CssFontVariant[m].name);
						break;
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontWeight)
			{
				if((*m_pCssFont)[i]->fontweight==Css::_refmeasure)
				{
					::_stprintf_s(StrBuf,255,_T("%s;%s:%i"),StrBuf,_T("font-weight"),(*m_pCssFont)[i]->fontweightvalue);
				}else
				{
					int cnt = sizeof(CssFontWeight)/sizeof(CssFontWeight[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssFontWeight[m].item== (*m_pCssFont)[i]->fontweight)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-weight"),CssFontWeight[m].name);
							break;
						}
					}
				}
			}
			if((*m_pCssFont)[i]->mask & cssMaskFontSize)
			{
				if((*m_pCssFont)[i]->fontsize==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssFont)[i]->fontsizemeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("font-size"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssFontSize)/sizeof(CssFontSize[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssFontSize[m].item== (*m_pCssFont)[i]->fontsize)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("font-size"),CssFontSize[m].name);
							break;
						}
					}
				}
			}
			if(StrBuf[0]!=';')
				pItemElement->put_text(StrBuf);
			else
				pItemElement->put_text(StrBuf + 1);
			pElement->appendChild(pItemElement,NULL);
			pItemElement->Release();
		}

		//text
		for(int i=0;i<m_pCssText->size();i++)
		{
			pXmlDoc->createElement(pTagName,&pItemElement);
			::_stprintf_s(StrName,30,_T("t%i"),i);
			pItemElement->setAttribute(L"name",(_variant_t)StrName);
			
			StrBuf[0]='\0';
			if((*m_pCssText)[i]->mask & cssMaskTextIndent)
			{
				GetMeasureStyleStr( (*m_pCssText)[i]->textindentmeasure,StrMeasure);
				::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("text-indent"),StrMeasure);
			}
			if((*m_pCssText)[i]->mask & cssMaskTextAlign)
			{
				int cnt = sizeof(CssTextAlign)/sizeof(CssTextAlign[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextAlign[m].item== (*m_pCssText)[i]->textalign)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("text-align"),CssTextAlign[m].name);
						break;
					}
				}
			}
			
			if((*m_pCssText)[i]->mask & cssMaskTextDecoration)
			{
				int cnt = sizeof(CssTextDecoration)/sizeof(CssTextDecoration[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextDecoration[m].item== (*m_pCssText)[i]->textdecoration)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("text-decoration"),CssTextDecoration[m].name);
						break;
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskLetterSpacing)
			{
				if((*m_pCssText)[i]->letterspacing==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssText)[i]->letterspacingmeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("letter-spacing"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssLetterSpacing)/sizeof(CssLetterSpacing[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssLetterSpacing[m].item== (*m_pCssText)[i]->letterspacing)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("letter-spacing"),CssLetterSpacing[m].name);
							break;
						}
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskWordSpacing)
			{
				if((*m_pCssText)[i]->wordspacing==Css::_refmeasure)
				{
					GetMeasureStyleStr( (*m_pCssText)[i]->wordspacingmeasure,StrMeasure);
					::_stprintf_s(StrBuf ,255,_T("%s;%s:%s"),StrBuf,_T("word-spacing"),StrMeasure);
				}else
				{
					int cnt = sizeof(CssWordSpacing)/sizeof(CssWordSpacing[0]);
					for(int m=0;m<cnt;m++)
					{
						if(CssWordSpacing[m].item== (*m_pCssText)[i]->wordspacing)
						{
							::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("word-spacing"),CssWordSpacing[m].name);
							break;
						}
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskWhiteSpace)
			{
				int cnt = sizeof(CssWhiteSpace)/sizeof(CssWhiteSpace[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssWhiteSpace[m].item== (*m_pCssText)[i]->whitespace)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("white-space"),CssWhiteSpace[m].name);
						break;
					}
				}
			}

			if((*m_pCssText)[i]->mask & cssMaskTextTransform)
			{
				int cnt = sizeof(CssTextTransform)/sizeof(CssTextTransform[0]);
				for(int m=0;m<cnt;m++)
				{
					if(CssTextTransform[m].item== (*m_pCssText)[i]->texttransform)
					{
						::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("text-transform"),CssTextTransform[m].name);
						break;
					}
				}
			}
			if(StrBuf[0]!=';')
				pItemElement->put_text(StrBuf);
			else
				pItemElement->put_text(StrBuf + 1);
			pElement->appendChild(pItemElement,NULL);
			pItemElement->Release();
		}

		//border
		for(int i=0;i<this->m_pCssBorder->size();i++)
		{
			pXmlDoc->createElement(pTagName,&pItemElement);
			::_stprintf_s(StrName,30,_T("b%i"),i);
			pItemElement->setAttribute(L"name",(_variant_t)StrName);
			
			StrBuf[0]='\0';
			for(int k=0;k<5;k++)
			{
				if((*m_pCssBorder)[i]->pBorder[k])
				{
					Css::TBorder* pBorder = (*m_pCssBorder)[i]->pBorder[k];
					if(pBorder->mask & cssMaskBorderType)
					{
						int cnt = sizeof(CssBorderTypes)/sizeof(CssBorderTypes[0]);
						for(int m=0;m<cnt;m++)
						{
							if(CssBorderTypes[m].item== pBorder->bordertype)
							{
								if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-style"),CssBorderTypes[m].name);
								if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-style"),CssBorderTypes[m].name);
								if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-style"),CssBorderTypes[m].name);
								if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-style"),CssBorderTypes[m].name);
								if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-style"),CssBorderTypes[m].name);
								break;
							}
						}
					}

					if(pBorder->mask & cssMaskBorderWidth)
					{
						if(pBorder->width==Css::_refmeasure)
						{
							GetMeasureStyleStr( pBorder->widthmesure,StrMeasure);
							if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),StrMeasure);
							if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),StrMeasure);
							if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),StrMeasure);
							if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),StrMeasure);
							if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),StrMeasure);
						}else
						{
							int cnt = sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]);
							for(int m=0;m<cnt;m++)
							{
								if(CssBorderWidths[m].item== pBorder->width)
								{
									if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),CssBorderWidths[m].name);
									if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),CssBorderWidths[m].name);
									if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),CssBorderWidths[m].name);
									if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),CssBorderWidths[m].name);
									if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),CssBorderWidths[m].name);
									break;
								}
							}
						}
					}
					if(pBorder->mask & cssMaskBorderColor)
					{
						/*
						if(pBorder->width==Css::_refmeasure)
						{
							GetMeasureStyleStr( pBorder->widthmesure,StrMeasure);
							if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),StrMeasure);
							if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),StrMeasure);
							if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),StrMeasure);
							if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),StrMeasure);
							if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),StrMeasure);
						}else
						{
							int cnt = sizeof(CssBorderWidths)/sizeof(CssBorderWidths[0]);
							for(int m=0;m<cnt;m++)
							{
								if(CssBorderWidths[m].item== pBorder->width)
								{
									if(k==0)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-width"),CssBorderWidths[m].name);
									if(k==1)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-left-width"),CssBorderWidths[m].name);
									if(k==2)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-top-width"),CssBorderWidths[m].name);
									if(k==3)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-right-width"),CssBorderWidths[m].name);
									if(k==4)::_stprintf_s(StrBuf,255,_T("%s;%s:%s"),StrBuf,_T("border-bottom-width"),CssBorderWidths[m].name);
									break;
								}
							}
						}
						*/
					}
				}
			}
			if(StrBuf[0]!=';')
				pItemElement->put_text(StrBuf);
			else
				pItemElement->put_text(StrBuf + 1);
			pElement->appendChild(pItemElement,NULL);
			pItemElement->Release();
		}

		//color
		for(int i=0;i<this->m_pCssColor->size();i++)
		{
			pXmlDoc->createElement(pTagName,&pItemElement);
			::_stprintf_s(StrName,30,_T("c%i"),i);
			pItemElement->setAttribute(L"name",(_variant_t)StrName);
			
			StrBuf[0]='\0';
			::_stprintf_s(StrBuf,255,_T("%s;%s:#%02x%02x%02x"),StrBuf,_T("color"),(*m_pCssColor)[i]->color[0]&0xff,(*m_pCssColor)[i]->color[0]>>8&0xff,(*m_pCssColor)[i]->color[0]>>16&0xff);

			if(StrBuf[0]!=';')
				pItemElement->put_text(StrBuf);
			else
				pItemElement->put_text(StrBuf + 1);
			pElement->appendChild(pItemElement,NULL);
			pItemElement->Release();
		}

		//background
		for(int i=0;i<this->m_pCssBackGround->size();i++)
		{
			pXmlDoc->createElement(pTagName,&pItemElement);
			::_stprintf_s(StrName,30,_T("g%i"),i);
			pItemElement->setAttribute(L"name",(_variant_t)StrName);
			
			StrBuf[0]='\0';
			if((*m_pCssBackGround)[i]->color.mask & cssMaskColor)
			{
				::_stprintf_s(StrBuf,255,_T("%s;%s:#%02x%02x%02x"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[0]&0xff,(*m_pCssBackGround)[i]->color.color[0]>>8&0xff,(*m_pCssBackGround)[i]->color.color[0]>>16&0xff);
			}
			if((*m_pCssBackGround)[i]->color.mask & cssMaskLeft && (*m_pCssBackGround)[i]->color.colorRef[1]!=XUtil::Css::_undef && 
				(*m_pCssBackGround)[i]->color.mask & cssMaskRight && (*m_pCssBackGround)[i]->color.colorRef[3]!=XUtil::Css::_undef)
			{
				::_stprintf_s(StrBuf,255,_T("%s;%s:none #%02x%02x%02x none #%02x%02x%02x"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[3]&0xff,(*m_pCssBackGround)[i]->color.color[3]>>8&0xff,(*m_pCssBackGround)[i]->color.color[3]>>16&0xff,
					(*m_pCssBackGround)[i]->color.color[1],(*m_pCssBackGround)[i]->color.color[1]>>8,(*m_pCssBackGround)[i]->color.color[1]>>16);

			}else if((*m_pCssBackGround)[i]->color.mask & cssMaskTop && (*m_pCssBackGround)[i]->color.colorRef[2]!=XUtil::Css::_undef && 
				(*m_pCssBackGround)[i]->color.mask & cssMaskBottom && (*m_pCssBackGround)[i]->color.colorRef[4]!=XUtil::Css::_undef)
			{
				::_stprintf_s(StrBuf,255,_T("%s;%s:#%02x%02x%02x none #%02x%02x%02x none"),StrBuf,_T("background-color"),(*m_pCssBackGround)[i]->color.color[2]&0xff,(*m_pCssBackGround)[i]->color.color[2]>>8&0xff,(*m_pCssBackGround)[i]->color.color[2]>>16&0xff,
					(*m_pCssBackGround)[i]->color.color[4]&0xff,(*m_pCssBackGround)[i]->color.color[4]>>8&0xff,(*m_pCssBackGround)[i]->color.color[4]>>16&0xff);
			}
					
			if(StrBuf[0]!=';')
				pItemElement->put_text(StrBuf);
			else
				pItemElement->put_text(StrBuf + 1);
			pElement->appendChild(pItemElement,NULL);
			pItemElement->Release();
		}

		return 1;
	}

	int xuCCssStyle::GetClassNameByIndex(int nIndex,LPTSTR className)
	{
		if(nIndex <0)
		{
			className[0]='\0';
			return 1;
		}
		className[0]='\0';
		TCssCellStyle  * pCellStyle = (*m_pCellStyle)[nIndex];
		if(pCellStyle->mask & cssMaskFont)
			if(pCellStyle->nFont>=0)::_stprintf_s(className,255,_T("%s f%i"),className,pCellStyle->nFont);
		if(pCellStyle->mask & cssMaskText)
			if(pCellStyle->nText>=0)::_stprintf_s(className,255,_T("%s t%i"),className,pCellStyle->nText);
		if(pCellStyle->mask & cssMaskBackGround)
			if(pCellStyle->nBackGround>=0)::_stprintf_s(className,255,_T("%s g%i"),className,pCellStyle->nBackGround);
		if(pCellStyle->mask & cssMaskMargin)
			if(pCellStyle->nMargin>=0)::_stprintf_s(className,255,_T("%s m%i"),className,pCellStyle->nMargin);
		if(pCellStyle->mask & cssMaskPadding)
			if(pCellStyle->nPadding>=0)::_stprintf_s(className,255,_T("%s p%i"),className,pCellStyle->nPadding);
		if(pCellStyle->mask & cssMaskColor)
			if(pCellStyle->nColor>=0)::_stprintf_s(className,255,_T("%s c%i"),className,pCellStyle->nColor);
		if(pCellStyle->mask & cssMaskBorder)
			if(pCellStyle->nBorder>=0)::_stprintf_s(className,255,_T("%s b%i"),className,pCellStyle->nBorder);
		return 1;
	}

	xuCssStyleSheet::xuCssStyleSheet(void)//:m_pCssStyle(new xuCCssStyle)
	{
	}

	xuCssStyleSheet::~xuCssStyleSheet(void)
	{
		//if(m_pCssStyle)delete m_pCssStyle;
		for(int i=0;i<(int)m_CssClass.size();i++)
		{
			if(m_CssClass[i].pStrClass) delete m_CssClass[i].pStrClass;
		}
	}

	int xuCssStyleSheet::AddCssClass(LPTSTR className,LPTSTR cssStr)
	{
		XUtil::Css::TBoxStyle * pBoxStyle = m_pCssStyle->ParseCssStyle(cssStr);
		int nIndex = -1;
		nIndex = m_pCssStyle->AddCssCellStyle(pBoxStyle);
		if(nIndex >=0)
		{
			XUtil::Css::CssClassItem cssItem;
			cssItem.pStrClass = ::_tcsdup(className);
			cssItem.nIndex = nIndex;
			this->m_CssClass.push_back(cssItem);
		}
		return nIndex;
	}
	
	const XUtil::Css::TBoxStyle* xuCssStyleSheet::GetBoxStyle(int nIndex)
	{
		return m_pCssStyle->GetBoxStyle(nIndex);
	}

	int xuCssStyleSheet::SetCssStyle(const XUtil::Css::TBoxStyle* pBoxStyle)
	{
		return m_pCssStyle->AddCssCellStyle(pBoxStyle);
	}

	int xuCssStyleSheet::GetBoxStyleIndexByClass(LPTSTR pStrClass)
	{
		int nIndex = -1;
		XUtil::Css::TBoxStyle* pBoxStyleOne = NULL;
		const XUtil::Css::TBoxStyle* pBoxStyleTwo;

		vector<LPTSTR> vals;
		XUtil::GetTokens(pStrClass,_T(" "),vals);
		for(int k=0;k<(int)vals.size();k++)
		{
			for(int i=0;i<m_CssClass.size();i++)
			{
				if(_tcsicmp(vals[k],m_CssClass[i].pStrClass)==0)
				{
					pBoxStyleTwo =  GetBoxStyle(m_CssClass[i].nIndex);
					if(pBoxStyleTwo)
					{
						if(!pBoxStyleOne) pBoxStyleOne = new XUtil::Css::TBoxStyle();
						*pBoxStyleOne = *pBoxStyleOne + *pBoxStyleTwo; 
					}
					break;
				}
			}
		}
		if(pBoxStyleOne)
		{
			nIndex = SetCssStyle(pBoxStyleOne);
			delete pBoxStyleOne;
		}
		return nIndex;
	}

	int xuCssStyleSheet::ParseStyleSheet(const LPTSTR pStyle)
	{
		int nPos = 0;
		int sPos = 0;
		TCHAR buf[255];
		TCHAR css[1024];
		bool bBrach = true;
		
		if(pStyle)
		{
			while(true)
			{
				css[0]=0;
				buf[0]=0;
				while(pStyle[nPos] && (pStyle[nPos]==' '||pStyle[nPos]=='\t'||pStyle[nPos]=='\r'|| pStyle[nPos]=='\n'))nPos++;
				if(!pStyle[nPos])break;
				
				//parse css name
				sPos=0;
				while(pStyle[nPos] && !(pStyle[nPos]==' '||pStyle[nPos]=='\t'||pStyle[nPos]=='\r'|| pStyle[nPos]=='\n'))
				{
					if(sPos < 253) buf[sPos++] = pStyle[nPos++];
				}
				buf[sPos++] = 0;
				nPos++;

				//parse css content
				sPos=0;
				while(pStyle[nPos] && !(pStyle[nPos]=='{'))nPos++;
				if(!pStyle[nPos])break;
				if(pStyle[nPos]=='{') nPos++;
				css[0] = 0;
				while(pStyle[nPos] && !(pStyle[nPos]=='}'))
				{
					if(sPos < 1023) css[sPos++] = pStyle[nPos++];
				}
				css[sPos++] = 0;
				nPos++;

				AddCssClass(buf,css);
			}
		}
		return 1;
	}

	int xuCssStyleSheet::GetClassStyle(LPCTSTR pStrName,LPCTSTR pStrID,LPCTSTR pCssClass,LPCTSTR pCssStyle,eCssItemType et)
	{
		XUtil::Css::TBoxStyle * pStyle = NULL;
		XUtil::Css::TBoxStyle * pStyleEx = NULL;

		//parse class boxstyle;
		if(pCssClass)
		{
			TCHAR cssClass[255];
			int nPos = 0;
			int sPos = 0;

			while(true)
			{
				while(pCssClass[nPos] && (pCssClass[nPos]==' '||pCssClass[nPos]=='\t'||pCssClass[nPos]=='\r'|| pCssClass[nPos]=='\n'))nPos++;
				if(!pCssClass[nPos])break;
				
				sPos = 1;
				cssClass[0] ='.';
				while(nPos <(int)_tcslen(pCssClass) &&  pCssClass[nPos] && !(pCssClass[nPos]==' '||pCssClass[nPos]=='\t'||pCssClass[nPos]=='\r'|| pCssClass[nPos]=='\n'))
				{
					if(sPos<254) cssClass[sPos++] = pCssClass[nPos++];
				}
				cssClass[sPos] = 0;

				if(cssClass[0])
				{
					int nCssIndex =-1;
					nCssIndex = GetBoxStyleIndexByClass(cssClass);
					if(nCssIndex >=0 )
					{
						pStyleEx = (XUtil::Css::TBoxStyle *)GetBoxStyle(nCssIndex);
						if(!pStyle) 
						{
							pStyle = new XUtil::Css::TBoxStyle;
							*pStyle = * pStyleEx;
						}else
							pStyle = &(*pStyle + * pStyleEx);
					}
				}

				if(!cssClass[nPos])break;
				nPos++;
			}
		}else if(pStrID) //css for id
		{
			TCHAR cssClass[255];
			int nPos = 0;
			int sPos = 1;

			while(pStrID[nPos] && (pStrID[nPos]==' '||pStrID[nPos]=='\t'||pStrID[nPos]=='\r'|| pStrID[nPos]=='\n'))nPos++;
			if(pStrID[nPos])
			{
				sPos = 1;
				cssClass[0] ='#';
				while(pStrID[nPos] && !(pStrID[nPos]==' '||pStrID[nPos]=='\t'||pStrID[nPos]=='\r'|| pStrID[nPos]=='\n'))
				{
					if(sPos<254) cssClass[sPos++] = pStrID[nPos++];
				}
				cssClass[sPos] = 0;

				if(cssClass[1])
				{
					int nCssIndex = -1;
					nCssIndex = GetBoxStyleIndexByClass(cssClass);
					if(nCssIndex >=0 )
					{
						pStyleEx = (XUtil::Css::TBoxStyle *)GetBoxStyle(nCssIndex);
						if(!pStyle) 
						{
							pStyle = new XUtil::Css::TBoxStyle;
							*pStyle = * pStyleEx;
						}else
							pStyle = &(*pStyle + * pStyleEx);
					}
				}
			}
		}else
		{
			if(pStrName)
			{
				TCHAR cssClass[255];
				cssClass[0] =0;
				_tcscat_s(cssClass,255,pStrName);

				if(cssClass[1])
				{
					int nCssIndex = -1;
					nCssIndex = GetBoxStyleIndexByClass(cssClass);
					if(nCssIndex >=0 )
					{
						pStyleEx = (XUtil::Css::TBoxStyle *)GetBoxStyle(nCssIndex);
						if(!pStyle) 
						{
							pStyle = new XUtil::Css::TBoxStyle;
							*pStyle = * pStyleEx;
						}else
							pStyle = &(*pStyle + * pStyleEx);
					}
				}
			}
		}

		//direct css style
		if(pCssStyle)
		{
			pStyleEx = XUtil::xuCCssStyle::ParseCssStyle(pCssStyle);
			if(pStyle && pStyleEx)
				pStyle = &(*pStyle + * pStyleEx);
			else if(pStyleEx) pStyle = pStyleEx;
		}
		if(!pStyle) return -1;
		int nIndex = SetCssStyle(pStyle);
		delete pStyle;
		return nIndex;
	}

}}
