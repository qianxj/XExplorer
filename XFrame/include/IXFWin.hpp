#pragma once

#include "xbxbase.h"
#include "xfAreaObject.hpp"

namespace Hxsoft{ namespace XFrame
{
	#define xfNodeAgentArea L"agentarea"
	#define xfNodeAgentToolArea L"agenttoolarea"
	#define xfNodeAgentExpanedArea L"agentexpandarea"

	class IXFControl;
	class IXPage;
	class IXNode;
	class xfWin;
	class xbXml;
	class ITrans;
	class XFRAME_API IXFWin : public xbObject
	{
	public:
		IXFWin(void);
		~IXFWin(void);
	public:
		HWND GetHWND();
	public:
		IXFWin* GetFrameWindow();
	public:
		int CloseWindow();
	public:
		int SetStyle(DWORD dwStyle);
		int SetStyleEx(DWORD dwStyleEx);
	public:
		static IXFWin * OpenWindow(LPCTSTR pServer,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx=0,DWORD DwStyle=0,LPCTSTR pCtrl=NULL);
		static IXFWin * OpenWindow(LPCTSTR pServer,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		static IXFWin * OpenWindow(LPCTSTR pServer,LPCTSTR pStrUrl,IXFWin* pParent=NULL,LPARAM lParam=NULL,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);

		static IXFWin * OpenWindow(ITrans * pTrans,LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		static IXFWin * OpenWindow(ITrans * pTrans,LPCTSTR pStrUrl,IXFWin* pParent=NULL,LPARAM lParam=NULL,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		
		IXFWin * OpenWindow(LPCTSTR pStrUrl,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		IXFWin * OpenWindow(LPCTSTR pStrUrl,IXFWin* pParent=NULL,LPARAM lParam=NULL,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);

		static IXFWin * OpenWindow(IXMLDOMElement * pElement,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		static IXFWin * OpenWindow(IXMLDOMElement *,IXFWin* pParent=NULL,LPARAM lParam=NULL,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		static IXFWin * OpenWindow(xbXml &xml,IXFWin* pParent=NULL,LPARAM lParam=NULL,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		static IXFWin * OpenWindow(IXPage * pPage,IXFWin* pParent,LPARAM lParam,IXFWin *pCmdWin,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
		static IXFWin * OpenWindow(IXPage * pPage,IXFWin* pParent=NULL,LPARAM lParam=NULL,DWORD dwStyleEx=0,DWORD DwStyle=0,LPRECT pRect=NULL);
	public:
		IXFControl * GetControl(LPCTSTR pControlName);
		IXFControl * GetControl(LPCTSTR pControlName,IXNode * pNode);
		IXFControl * GetControl(LPCTSTR pControlName,bool bInsUnlayered);
	public:
		int GetAttachObject();
		static int GetAttachObject(HWND hWnd);
	public:
		LPCTSTR GetServerUrl();
	public:
		int Redraw();
	public:
		IXNode * GetAgentNode(LPCTSTR pStrName = xfNodeAgentArea);

		IXNode* SetAgentNodeContent(IXNode* pAgentNode,IXMLDOMElement* pElement,IXNode * pOwnerNode);
		IXNode* SetAgentNodeContent(IXNode* pAgentNode,LPCTSTR pStrXml,IXNode * pOwnerNode);
		IXNode* SetAgentNodeContent(IXNode* pAgentNode,IXNode *Node,IXNode * pOwnerNode);

		IXNode* SetAgentNodeContent(IXNode* pAgentNode,LPCTSTR pStrXml);
		IXNode* SetAgentNodeContent(IXNode* pAgentNode,IXMLDOMElement* pElement);
		IXNode* SetAgentNodeContent(IXNode* pAgentNode,IXNode *pNode);

		int ClearAgentNodeContent(IXNode* pAgentNode = NULL,LPCTSTR pStrName = NULL);
		int ClearAgentNodeContentEx(IXNode* pAgentNode = NULL,LPCTSTR pStrName = NULL);
		int RemoveAgentNodeContent(IXNode* pAgentNode , IXNode *pNode);
	public:
		IXNode * GetXNode();
		IXPage * GetXPage();
		IXMLDOMElement * GetElement();
	public:
		void * GetParam();
		bool  SetParam(void * pParam);
		void * GetFrameParam();
		bool  SetFrameParam(void * pParam);
	public:
		void SwitchLayer(LPTSTR pStrLayerSheet,LPTSTR pStrLayerFrame);
	public:
		xfWin * m_pWin;
	public:
		bool AttachXvmEvent(LPTSTR msgid,int pObj, LPTSTR fctname);
		bool AttachXvmEvent(LPTSTR pCtlName,LPTSTR msgid,int pObj, LPTSTR fctname);
		bool AttachXvmEvent(HWND hWndControl,LPTSTR msgid,int pObj, LPTSTR fctname);
		bool AttachXvmEvent(IXFWin * pWin,LPTSTR pCtlName,LPTSTR msgid,int pObj, LPTSTR fctname);
	public:
		bool AttachEvent(LPTSTR msgid,FEvent pEvt,void * pThis);
		bool AttachEvent(LPTSTR pCtlName, LPTSTR msgid,FEvent pEvt,void * pThis);
	public:
		bool AttachVmEvent(LPTSTR msgid,int pEvt,void * pThis);
		bool AttachVmEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis);
	public:
		bool AttachWppEvent(LPTSTR msgid,int pEvt,void * pThis);
		bool AttachWppEvent(LPTSTR pCtlName, LPTSTR msgid,int pEvt,void * pThis);
	public:
		static IXFWin* CreateInstance();
		static int ReleaseInstance(IXFWin* pWin);
	public:
		xbObject * QueryItem(LPCTSTR pControlName,LPCTSTR pItem);
	};
}}
