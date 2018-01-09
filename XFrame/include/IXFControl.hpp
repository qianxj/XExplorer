#pragma once

#include "xbxbase.h"

namespace Hxsoft{ namespace XFrame
{
	class IXNode;
	class IXFWin;
	class xfControl;
	class XFRAME_API IXFControl : public xbObject
	{
	public:
		IXFControl(void);
		~IXFControl(void);

	public:
		HWND GetHWND();
	public:
		IXFWin* GetWindow();
	public:
		IXNode * GetXNode();
		IXMLDOMElement * GetElement();
	public:
		RECT & GetAreaRect();
		RECT & GetContentRect();
	public:
		int SetAreaRect(RECT & rc);
		int SetContentRect(RECT & rc);
	public:
		RECT  GetClientRect();
	public:
		int Redraw();
	public:
		int DoAcceptText();
		virtual int AcceptText(){return 0;};
	public:
		xfControl * m_pControl;
	public:
		int AddObserver(xbObserver * pObservers);
		int RemoveObserver(xbObserver * pObservers);
		int ClearObserver();
	public:
		int SetStyle(DWORD dwStyle);
		int SetStyleEx(DWORD dwStyleEx);
	public:
		bool SetLayerFlow(LPCTSTR pFlow,bool bRedraw);
		bool SwitchLayer(LPTSTR pStrLayerSheet,LPTSTR pStrLayerFrame);
	public:
		xbObject * QueryItem(LPTSTR pItem);
	public:
		bool GetContentExtent(SIZE &sz);
	public:
		int addBindControl(UINT key, LPTSTR name, UINT ev=0x202);
		int fireBindControl(UINT key);
	public:
		int bindData(void * pcall,int vc,int tk);
		int bindData(void * pcall,int vc,int tk,const wchar_t* root);
	public:
		wchar_t* bindGetData(wchar_t* path, int row, wchar_t*col);
		wchar_t* bindGetData(wchar_t* path, wchar_t* id, wchar_t*col);
	public:
		int bindNotifyDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data);
		int bindNotifyDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data);
		int bindNotifyInsertRow(wchar_t* path,int row);
		int bindNotifyInsertRow(wchar_t* path,wchar_t* id);
		int bindNotifyDeleteRow(wchar_t* path,int row);
		int bindNotifyDeleteRow(wchar_t* path,wchar_t* id);
		int bindGetRowCount(wchar_t* path);
		int bindNotifyRemove();
	public:
		int bindUpdateDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data);
		int bindUpdateInsertRow(wchar_t* path,int row);
		int bindUpdateDeleteRow(wchar_t* path,int row);
		int bindUpdateDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data);
		int bindUpdateInsertRow(wchar_t* path,wchar_t* id);
		int bindUpdateDeleteRow(wchar_t* path,wchar_t* id);
	};
}}
