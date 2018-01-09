#pragma once

#include "xbxbase.h"
#include <vector>
#include "xfareaobject.hpp"
#include <functional>
#include "xuXUtil.h"

using namespace std;
namespace Hxsoft{ namespace XFrame
{

	//LayerFlow
	typedef int	TLayerFlow;
	typedef int	TZOrder;
	#define LayerFlow_None			0x00000001		//not layer space
	#define LayerFlow_WE			0x00000002
	#define LayerFlow_NS			0x00000004
	#define LayerFlow_FreeForm		0x00000008
	#define LayerFlow_Control		0x00000010
	#define LayerFlow_HtmlFlow		0x00000020
	#define LayerFlow_Sheets		0x00000040
	#define LayerFlow_Hidden		0x00000080		//not show but layer space
	#define LayerFlow_UnLayer		0x00000100		//not show and not layer space
	
	#define	LayerFlow_NotLayerNode	(LayerFlow_None | LayerFlow_UnLayer)
	#define LayerFlow_LayerChild	(LayerFlow_WE|LayerFlow_NS|LayerFlow_FreeForm|LayerFlow_Sheets| LayerFlow_HtmlFlow )

	typedef int TPosStatus;
	#define PosStatus_nX			0x0001
	#define PosStatus_nY			0x0002
	#define PosStatus_nRX			0x0004
	#define PosStatus_nRY			0x0008
	#define PosStatus_Paragraph		0x0100
	#define PosStatus_Row			0x0200
	#define PosStatus_Page			0x0400
	

	class xfXPage;
	class XFRAME_API xfNode : public xbObject
	{
	public:
		xfNode(void);
	public:
		~xfNode(void);

	public:
		LPTSTR	m_strNodeName;
		LPTSTR  m_strNodeID;
		LPTSTR  m_strClass;
		LPTSTR  m_strTagName;
	public:
		xfNode * GetNodeByID(LPCTSTR pStrNodeID,bool bIncUnLayered = true);
		xfNode * GetNodeByName(LPCTSTR pStrName,bool bIncUnLayered = true);

	public:
		LPTSTR	m_pCaption;		//title
		LPTSTR	m_pCaptionEx;	//title

		//link objs
		xfNode *	m_pParentNode;	//parent node
		xfXPage *	m_pxfPage;
	public:
		std::vector<xfNode *> * m_pChilds;
	public:
		IXMLDOMElement* m_pElement;	//xml element
	public:
		//css string;
		LPTSTR		m_pCssStyle;			//css style string
		int			m_nCssIndex;			//css style string
		int			m_nCssIndexSelection;	//css style string
		int			m_nCssIndexHover;		//css style string

	public:
		virtual int ShowControl(bool bShow);
	public:
		void * m_pData;
		void * m_pParam;
		void * m_pAgentOwner;

	public:
		DOUBLE	m_hExtRate;			//box spliter rate
		DOUBLE	m_vExtRate;			//box spliter rate
		int		m_width;				//box width
		int		m_height;				//box height
		int		m_minWidth;
		int		m_hopeWidth;
		int		m_nX;
		int		m_nY;
		int		m_nRX;
		int		m_nRY;
		TPosStatus	m_posStatus;
	public:
		RECT	m_AreaRect;	//Entry Area Rect
		RECT	m_ContentRect;	//content Rect
	public:
		//css style operator
		int ClearCssStyle();  //clear magin border background padding flag to zero
		int ParseCssStyle(LPCTSTR pStrCssStyle, XUtil::xuCssStyleSheet::eCssItemType et=XUtil::xuCssStyleSheet::etCssNormal);	//parse cssstyle string
		int SetCssStyle(LPCTSTR	cssStyle); //set cssstyle string ,next parse cssstyle,adjust child node or brother node
		LPCTSTR GetCssStyle(){return this->m_pCssStyle; };

		//adjust or size rect
		virtual int AdjustRect(); //adjust content rect,adjust child node or brother node
		virtual int AdjustRect(RECT rect); //adjust content rect,adjust child node or brother node
		virtual int AdjustRect(int dx); //adjust content rect,adjust child node or brother node
		virtual int CalcContentRect();	//calc content rect

		virtual int CreateNodeControl(HWND	hWnd);
		virtual int AdjustCtrlRect();

		//draw 
		virtual int DrawNode(HDC hDC);

		//root node
		xfNode * GetRootNode();
		xfXPage * GetXPage();

		int GetMinWidth(){return this->m_minWidth;};
		int GetHopeWidth(){return this->m_hopeWidth;};
		int SetWidthHeight(int width, int height);
		int MoveTo(LPTSTR Name);

		int RemoveChild();
		static int RemoveChild(xfNode * pParentNode);
		virtual int RemoveControl();
		virtual int OnClose();

		virtual int ParseNode(IXMLDOMElement* pNodeEx,bool ParseChild = true) ;
		static xfNode * CreateNode(IXMLDOMElement * pElement,bool setClassName = true);
	public:
		bool m_bSelection;
		bool m_bHover;
		virtual xfNode * GetLabelNode(bool bSelection ,LPCTSTR pLayerFrame,LPCTSTR pLayerSheet);
	public:
		TLayerFlow	m_LayerFlow;
		TZOrder		m_zOrder;
		bool	IsLayerNode();
		bool	ShouldLayerChild();
	
	public:
		virtual HWND GetHWND();
		virtual bool GetClientRect(RECT *pRect);
	public:
		int m_pTabOrder;
		struct nodetab_less:public std::binary_function<xfNode * , xfNode *  , bool>
		{
			bool operator()(const xfNode *  pNode1, const xfNode * pNode2)const
			{
				return pNode1->m_pTabOrder < pNode2->m_pTabOrder;
			}
		};
		virtual int GetNodesByTab(std::set<xfNode *,nodetab_less> & pSetNode);
	public:
		virtual int GetXml(BSTR &bstr);
	public:
		virtual xbObject * GetInterface();
	public:
		bool m_bDeleteing;
	};


}}