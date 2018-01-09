#pragma once

#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame
{
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

	class IXFControl;
	class IXPage;
	class xfNode;
	class XFRAME_API IXNode : public xbObject
	{
	public:
		IXNode(void);
		~IXNode(void);
	public:
		LPCTSTR GetNodeName();
		LPCTSTR GetNodeID();
	public:
		int SetNodeName(LPCTSTR pNodeName);
		int SetNodeID(LPCTSTR pNodeID);
	public:
		LPCTSTR GetCaption();
		int SetCaption(LPCTSTR pCaption);
	public:
		IXNode * GetNodeByID(LPCTSTR pStrNodeID);
		IXNode * GetNodeByName(LPCTSTR pStrName);
	public:
		IXNode * GetXNode();
		IXPage * GetXPage();
		IXMLDOMElement * GetElement();
	public:
		IXNode * GetRootNode();
		BOOL IsControlNode();
		BOOL IsControlNode(IXNode *pNode);
	public:
		IXFControl * GetControl();
	public:
		int SetXNode(IXNode * pNode,bool bReCeate = true);
		int SetElement(IXNode * pNode,bool bReCeate = false);
	public:
		int RemoveControl();
		int CreateControl(HWND hWnd);
	public:
		int RemoveChild();
		int RemoveChild(IXNode * pParentNode);
	public:
		IXNode * CreateNode(IXMLDOMElement * pElement,bool setClassName = true);
	public:
		int TransToControl(LPCTSTR pControlClass);
		int TransToControl(IXNode * pNode,LPCTSTR pControlClass);
	public:
		RECT & GetAreaRect();
		RECT & GetContentRect();
	public:
		int SetAreaRect(RECT & rc);
		int SetContentRect(RECT & rc);
	public:
		//adjust or size rect
		int AdjustRect(); //adjust content rect,adjust child node or brother node
		int AdjustRect(RECT rect); //adjust content rect,adjust child node or brother node
		int AdjustRect(int dx); //adjust content rect,adjust child node or brother node
		int CalcContentRect();	//calc content rect

		int CreateNodeControl(HWND	hWnd);
		int AdjustCtrlRect();
	public:
		//draw 
		int DrawNode(HDC hDC);
	public:
		IXNode * GetParentNode();
		int GetChildNodesCount();
		IXNode* GetChild(int nItem); //from 0
		bool RemoveChildNode(int nIndex);
		IXNode* InsertChildNode(int nIndex, LPTSTR pXml);
		IXNode* InsertChildNode(int nIndex, IXMLDOMElement * pElement );
	public:
		DWORD GetLayerFlow();
		int SetLayerFlow(DWORD dwLayerFlow);
		DWORD GetLayerFlow(IXNode * pNode);
		int SetLayerFlow(IXNode * pNode,DWORD dwLayerFlow);

	public:
		static IXNode * CreateInstance();
		static int ReleaseInstance(IXNode * pNode);
	public:
		LPTSTR GetXml();
	public:
		HWND GetHWND();

	public:
		xfNode * m_pNode;

	public:
		int SetWidthHeight(int width, int height);
		int MoveTo(LPTSTR Name);
	};
}}
