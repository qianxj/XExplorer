#pragma once

#include "xbxbase.h"
#include "ixnode.hpp"
namespace Hxsoft{ namespace XFrame
{
	class IXFControl;
	class IXFWin;
	class IXNode;
	class xfXPage;
	class ITrans;
	class XFRAME_API IXPage : public xbObject
	{
	public:
		IXPage(void);
		~IXPage(void);
	public:
		bool Load(LPCTSTR pStrXPageFile);
		bool Load(IXMLDOMElement* pElement);
		bool LoadXPage(LPCTSTR pServer,LPCTSTR pStrUrl);
		bool LoadXPage(ITrans *pTrans,LPCTSTR pStrUrl);
		bool LoadXPage(LPCTSTR pStrPage);
	public:
		IXFControl * GetControl();
		IXFWin *	 GetWin();
	public:
		LPCTSTR GetStrLocal();
		LPCTSTR GetUrl();
		LPCTSTR GetWkUrl();
	public:
		int SetStrLocal(LPCTSTR pStr);
		int SetUrl(LPCTSTR pStr);
		int SetWkUrl(LPCTSTR pStr);
	public:
		int SetXNode(IXNode * pNode,bool bReCeate = true);
		int SetElement(IXNode * pNode,bool bReCeate = false);
	public:
		int RemoveControl();
		int CreateNodeControl(HWND hWnd);
	public:
		int RemoveChild();
		int RemoveChild(IXNode * pParentNode);
	public:
		static IXNode * CreateNode(IXMLDOMElement * pElement,bool setClassName = true);
	public:
		static int TransToControl(IXNode * pNode,LPCTSTR pControlClass);
	public:
		BOOL IsControlNode(IXNode *pNode);
	public:
		DWORD GetLayerFlow();
		int SetLayerFlow(DWORD dwLayerFlow);
		DWORD GetLayerFlow(IXNode * pNode);
		int SetLayerFlow(IXNode * pNode,DWORD dwLayerFlow);
	public:
		//draw 
		int DrawNode(HDC hDC);
	public:
		static IXPage* CreateInstance();
		static int ReleaseInstance(IXPage* pPage);
	public:
		xfXPage* m_pXPage;
	};
}}
