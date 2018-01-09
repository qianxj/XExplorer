#pragma once

#include "xwin.hpp"
#include <vector>
#include "xcontrol.hpp"

using namespace std;
namespace Hxsoft
{
	namespace XFrame
	{
		#define MarginLeft	0x1
		#define MarginRight 0x2
		#define MarginTop	0x4
		#define MarginBottom 0x8

		#define PaddingLeft	0x1
		#define PaddingRight 0x2
		#define PaddingTop	0x4
		#define PaddingBottom 0x8

		#define BorderLeft	0x1
		#define BorderRight 0x2
		#define BorderTop	0x4
		#define BorderBottom 0x8

		#define Background	0x1

		//spliter status
		#define SP_LEFT		0x01
		#define SP_RIGHT	0x02
		#define SP_VERT		0x04
		#define SP_HORZ		0x08

		#define AR_NS		0x01	//vert
		#define AR_WE		0x02	//horz
		#define AR_ABS		0x04	//abs position

		class XNode;
		class XWinbase;
		class XWin;
		class XWinDesign;

		class XEditTemplate
		{
		public:
			XEditTemplate();
			~XEditTemplate();
		public:
			vector<IXMLDOMElement *>* 	m_pTemplateElemnt;
			vector<LPTSTR> * m_pTempletCategory;
			vector<LPTSTR> * m_pTempletName;
		public:
			int AddXmlTemplate(LPTSTR pStrName, IXMLDOMElement * pElement);
			int AddXmlTemplate(LPTSTR pStrName, LPTSTR pXmlStr);
			int AddXmlFileTemplate(LPTSTR pStrName, LPTSTR pStrFileName);
			int AddXmlTemplate(LPTSTR pCategory, LPTSTR pStrName, IXMLDOMElement * pElement);
			int AddXmlTemplate(LPTSTR pCategory,LPTSTR pStrName, LPTSTR pXmlStr);
			int AddXmlFileTemplate(LPTSTR pCategory,LPTSTR pStrName, LPTSTR pStrFileName);
			int AddXmlTemplateParseFile(LPTSTR pCategory,LPTSTR pStrFileName);
		public:
			IXMLDOMElement * FindTemplate(LPTSTR pStrName);
			IXMLDOMElement * FindTemplate(LPTSTR pCategory, LPTSTR pStrName);
			int CreateXmlDoc(IXMLDOMDocument * &pXmlDocument);
		};

		class	XLayers
		{
		public:
			XLayers();
			~XLayers();
		public:
			int	nCurLayer;
			vector<XNode *>* 	pLayers;
			vector<bool> *		pCtrlWinCreated;
			XNode *	m_pNode;
			int m_bTopArrow;
		public:
			int ParseNode(XNode * pParentNode,IXMLDOMElement * pElement);
			int SwitchTo(int nIndex);
			int AddSheet(IXMLDOMElement * pElement);
			int AddXPageSheet(LPTSTR pStrCaption);
		public:
			void * GetLayerSheetCtrl();
		};

		class XSpliterBar
		{
		public:

			bool	m_bIsVertbar;	//is vert bar
			XNode * m_pLeftNode;	//left node
			XNode * m_pRightNode;	//right node
			int spBarWidth;
			XSpliterBar():m_bIsVertbar(true),m_pLeftNode(NULL),m_pRightNode(NULL),spBarWidth(4){};

			int OnPaint(HDC hdc);

			//spliter operator
			BOOL IsHited(POINT pt){return ::PtInRect(&GetBarRect(),pt);}	//this is hited
			void DrawSizeBar(HWND hWnd,POINT pt);	//draw sizebar
			int AdjustSplitePos(POINT pt); //adjust splite pos
			int GetBarWidth(){return spBarWidth;};
			RECT GetBarRect();
		};

		class XControl;
		class XNode
		{
		public:
			XNode(void);
		public:
			~XNode(void);
		public:
			LPTSTR	m_strCtlName;
			LPTSTR  m_strCtlID;
		public:
			XControl * GetControlByName(LPTSTR pStrName);
			XControl * GetControlByID(LPTSTR pStrID);
			XNode * GetNodeByID(LPTSTR pStrID);
			XNode * GetNodeByName(LPTSTR pStrID);

		public:
			LPTSTR	m_pCaption;		//title
			LPTSTR	m_pXScript;		//xscript
			LPTSTR	m_pXDll;		//xdll
			LPTSTR	m_pstrGuid;		//xguid

			//link objs
			XNode *		m_pParentNode;	//parent node
			XNode *		m_pLeftNode;	//left child node
			XNode*		m_pRightNode;	//right child node
			DWORD		m_BoundStatus;	//bound status
			DWORD		m_AlignArrow;	//child control align arrow
			XLayers*	m_pLayers;	//layers sheets
		public:
			XWin * m_pOwnerWin;	//owner window	
			XControl * m_pOwerControl;	//owner window	
			vector<XControl *>	m_pControlWins;	//Control group
			IXMLDOMElementPtr m_pElement;	//xml element
		public:
			//css style attribute
			//margin attribute
			DWORD	m_FMargin;			//Margin Status
			int		m_MarginWidths[4];	//Margin Width

			//padding attribute
			DWORD	m_FPadding;			//Padding status
			int		m_PaddingWidths[4];	//Padding width

			//border attribute
			DWORD		m_FBorder;			//Border status
			int			m_BorderWidths[4];	//Border width
			COLORREF	m_BorderColors[4];	//Border color
			int			m_BorderType[4];	//Border type

			//background attribute
			UINT m_FBackground;				//backgroud status
			TCHAR	m_BackgroundUrl[255];		//backgroud url
			double  m_BackgroundRate;	//backgroud position
			COLORREF m_BackgroundColor;	//background color
			DWORD	m_BackGroundRepeate;	//background repeat

			//css string;
			LPTSTR		m_pCssStyle;			//css style string

			//
		public:
			//spliter attrib
			DWORD	m_spStatus;			//spliter status
			DOUBLE	m_spRate;			//box spliter rate
			int		m_width;				//box width
			int		m_height;				//box height
			XSpliterBar*	m_pSpliterBar;
			int		m_minWidth;
			int		m_hopeWidth;

		public:
			RECT	m_AreaRect;	//Entry Area Rect
			RECT	m_ContentRect;	//content Rect
		public:
			vector<HIMAGELIST> & GetImageList();
		public:
			//css style operator
			int ClearCssStyle();  //clear magin border background padding flag to zero
			int ParseCssStyle();	//parse cssstyle string
			int SetCssStyle(LPCTSTR	cssStyle); //set cssstyle string ,next parse cssstyle,adjust child node or brother node
			LPTSTR GetCssStyle(){return this->m_pCssStyle; };

			//adjust or size rect
			int AdjustRect(); //adjust content rect,adjust child node or brother node
			int AdjustRect(RECT rect); //adjust content rect,adjust child node or brother node
			int AdjustRect(int dx); //adjust content rect,adjust child node or brother node
			int CalcContentRect();	//calc content rect

			//draw 
			int DrawNode(HDC hDC);
			int DrawCtrlWin(XControl * pControl, HDC hDC);
			int DoDraw(HDC hDC);
			int ClearChild();
			int ClearChild( bool bClearControl);
			bool	HasChild();
			
			//root node
			XNode * GetRootNode();

			//link object
			XNode * GetBrotherNode();
			//spliterbar
			XSpliterBar* GetSpliterbar();
			int GetMinWidth(){return this->m_minWidth;};
			int GetHopeWidth(){return this->m_hopeWidth;};

			int ParseNode(IXMLDOMElementPtr pNodeEx);
			int CreateNodeControl(HWND	hWnd);
			int AdjustCtrlRect();

			int HideCtrlWin();
			int ShowCtrlWin();
	
			XSpliterBar* GetSpliterBarAtPointer(POINT pt);


		};
	}
}