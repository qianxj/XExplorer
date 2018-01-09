#pragma once

#include "xfnode.hpp"
#include "xfwinbase.hpp"
namespace Hxsoft{ namespace XFrame
{
	typedef struct tagBindControl
	{
		UINT key; 
		LPTSTR name;
		UINT fireEvent;
	}TBindControl;

	class XFRAME_API xfControl :
		public xfWinbase
	{
	public:
		xfControl(void);
		~xfControl(void);
	public:
		xfNode * m_pxfNode;
		xfNode * GetXfNode(){ return m_pxfNode;}
		bool SetXfNode(xfNode * pNode){ m_pxfNode = pNode; return true;}
	public:
		DWORD	m_dwExStyle;
		DWORD	m_dwStyle;
		LPTSTR	m_pszWinClass;
		bool	m_bMdiClient;
	public:
		virtual	HWND CreateControl(LPTSTR pszWndTitle,  RECT & rtPos, HWND hWndParent, UINT uID,HINSTANCE hInstance,HMENU hMenu = NULL,LPVOID lpParam = NULL);
		virtual	void Initial();
		virtual	void Initial(IXMLDOMElement *pElement);
	public:
		bool m_bTransparent;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect = NULL)=0;
		RECT	m_DrawRect;
	public:
		virtual int AdjustControlRect(RECT rect,bool redraw = true);
	public:
		void SwitchLayer(LPTSTR pStrLayerSheet =NULL,LPTSTR pStrLayerFrame=NULL);
	public:
		virtual class xfWin* GetWin();
	public:
		virtual int ControlCreated();
	public:
		bool EvtLButton(TEvent * pEvent,LPARAM lPara);
	public:
		std::vector<xbObserver *> * m_pObservers;
		int AddObserver(xbObserver * pObservers);
		int RemoveObserver(xbObserver * pObservers);
		int ClearObserver();
	public:
		virtual int OnClose();
	public:
		virtual xbObject * GetInterface(); 
	public:
		bool SetLayerFlow(LPCTSTR pFlow,bool bRedraw);
	public:
		virtual xbObject * QueryItem(LPTSTR pItem){return NULL;}
	public:
		virtual SIZE GetContentExtent();
	
	//data bind operate
	private:
		struct TBindDataCall
		{
			int getdata;
			int getdata1;
			int datachanged;
			int datachanged1;
			int insertrow;
			int insertrow1;
			int deleterow;
			int deleterow1;
			int rowcount;
			int rowchanged;
			int remove;
		};

		struct TBindData
		{
			int vc;
			int tk;
			wchar_t* root;
			int hrow;
			int hcol;

			int getdata;
			int getdata1;
			int datachanged;
			int datachanged1;
			int insertrow;
			int insertrow1;
			int deleterow;
			int deleterow1;
			int rowcount;
			int rowchanged;
			int remove;

			TBindData(); 
		};
		TBindData * m_pBindData;
	public:
		bool hasBindData();
		int BindDataVC();
	public:
		int bindData(void * pcall,int vc,int tk);
		int bindData(void * pcall,int vc,int tk,const wchar_t* root);
	private:
		int manageCall(void * context, void * p, int fn);
	public:
		int bindNotifyDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data);
		int bindNotifyDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data);
		int bindNotifyInsertRow(wchar_t* path,int row);
		int bindNotifyInsertRow(wchar_t* path,wchar_t *id);
		int bindNotifyDeleteRow(wchar_t* path,int row);
		int bindNotifyDeleteRow(wchar_t* path,wchar_t *id);
		int bindNotifyGetRowCount(wchar_t* path);
		int bindNotifyRowChanged(wchar_t* path,int row);
		int bindNotifyRemove();
	public:
		wchar_t* bindGetData(wchar_t* path, int row, wchar_t*col);
		wchar_t* bindGetData(wchar_t* path, wchar_t* id, wchar_t*col);
	public:
		virtual int bindUpdateDataChanged(wchar_t* path,int row,wchar_t* col, wchar_t* data);
		virtual int bindUpdateInsertRow(wchar_t* path,int row);
		virtual int bindUpdateDeleteRow(wchar_t* path,int row);
		virtual int bindUpdateDataChanged(wchar_t* path,wchar_t* id,wchar_t* col, wchar_t* data);
		virtual int bindUpdateInsertRow(wchar_t* path,wchar_t* id);
		virtual int bindUpdateDeleteRow(wchar_t* path,wchar_t* id);
	public:
		virtual int addBindControl(UINT key, LPTSTR name, UINT ev=0x202);
		virtual int fireBindControl(UINT key);
		vector<TBindControl> *m_BindControl;
		virtual bool EvtKeyDown(TEvent * pEvent, LPARAM lParam);

	};
}}