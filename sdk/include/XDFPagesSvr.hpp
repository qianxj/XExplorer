#pragma once

#include "xofbase.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XDF
{
	enum eXDFDrawObject{_none,_line,_rect,_round,_image,_text,_control,_shape};
	struct tagDrawObjectInfo
	{
		LPTSTR pStrItemName;
		eXDFDrawObject eItem;
	};

	eXDFDrawObject FindDrawObject(LPTSTR pstrItem);
	struct XDFPageItem;
	struct XDFDrawObject
	{
		XUtil::Css::TMeasure startx;
		XUtil::Css::TMeasure starty;
		XUtil::Css::TMeasure endx;
		XUtil::Css::TMeasure endy;
		XUtil::Css::TMeasure width;
		XUtil::Css::TMeasure height;

		LPTSTR strStartX;
		LPTSTR strStartY;
		LPTSTR strEndX;
		LPTSTR strEndY;
		LPTSTR strWidth;
		LPTSTR strHeight;

		int m_nIndexStyle;
		eXDFDrawObject eObject;
		LPTSTR pStrText;
		LPTSTR pStrUrl;

		int id;
		LPTSTR name;
		int refid;
		LPTSTR refname;

		XDFDrawObject(void):pStrText(NULL),pStrUrl(NULL),
				eObject(_none),m_nIndexStyle(-1),
				strStartX(NULL),strStartY(NULL),strEndX(NULL),strEndY(NULL),
				strWidth(NULL),strHeight(NULL){};
		~XDFDrawObject(void)
		{
			if(pStrText)delete pStrText;
			if(pStrUrl)delete pStrUrl;

			if(name)delete name;
			if(refname)delete refname;

			if(strStartX) delete strStartX;
			if(strStartY) delete strStartY;
			if(strEndX) delete strEndX;
			if(strEndY) delete strEndY;
			if(strWidth) delete strWidth;
			if(strHeight) delete strHeight;
		}
	public:
		int DoDraw(HDC hPaintDC,RECT rtPage,XOfficeStyleSvr * pStyleSvr,XDFPageItem * pItem=NULL);
	};

	struct tagShape
	{
		int id;
		LPTSTR name;
		XUtil::xuVector<XDFDrawObject *> m_DrawObjs;
	};

	class XDFPagesSvr;
	struct XDFPageItem
	{
	public:
		int pagew;
		int pageh;
		RECT AreaRect;
		RECT ContentRect;
	public:
		int id;
	public:
		RECT srect;
		RECT prect;
	public:
		XDFPagesSvr * m_pXDFPagesSvr;
	public:
		int m_nIndexStyle;
	public:
		XUtil::xuVector<XDFDrawObject *> m_DrawObjs;
		XUtil::xuVector<tagShape *> m_Shapes;

		XDFPageItem():m_nIndexStyle(-1),m_pXDFPagesSvr(NULL)
		{
			::SetRect(&srect,0,0,0,0);
			::SetRect(&prect,0,0,0,0);
		};
		~XDFPageItem()
		{
			for(int i=0; i<m_DrawObjs.size();i++)
			{
				if(m_DrawObjs[i]) delete m_DrawObjs[i];
			}
			for(int i=0; i<m_Shapes.size();i++)
			{
				if(m_Shapes[i]) delete m_Shapes[i];
			}
		}
	};

	tagShape* FindShapeByName(XDFPageItem* pItem,LPTSTR pstrName);
	tagShape* FindShapeByID(XDFPageItem* pItem,int ShapeID);

	class XDFPagesSvr :
		public CSvrbase
	{
	public:
		XDFPagesSvr(void);
		~XDFPagesSvr(void);
	public:
		int m_nIndexStyle;
	public:
		int m_nPageBarWidth;
		int m_curPage;
	public:
		XUtil::xuVector<XDFPageItem *> m_Pages;
		XUtil::xuVector<tagShape *> m_Shapes;
	};
}}}}
