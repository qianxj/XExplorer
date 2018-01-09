#pragma once
#include "xofbase.h"
#include "xframe.h"
#include "xuxutil.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class CEditSvr;
	class CInplaceEdit : public Hxsoft::XFrame::xfControl
	{
	public:
		enum eEditType{_null,_edit,_combobox,_date,_datetime,_time,_comboboex,_radio,_checkbox,_autofill,_control,_flowchart,_listbox};
	public:
		LPTSTR	m_szCotrolClass;
	public:
		CInplaceEdit(void):m_szCotrolClass(NULL),m_pEditSvr(NULL){};
		~CInplaceEdit(void){if(m_szCotrolClass) delete m_szCotrolClass;}
	public:
		virtual LPTSTR	GetText()=0;
		virtual LPTSTR	GetTextData()=0;
		virtual int	SetText(LPTSTR pStrContent)=0;
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow)=0;
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText)=0;
	public:
		virtual void Invalidate(){if(m_hWnd) ::InvalidateRect(this->m_hWnd,NULL,TRUE);}
		virtual void SetFont(LOGFONT &logFont,bool bRedraw){SendMessage(m_hWnd,WM_SETFONT,(WPARAM)&logFont,bRedraw);}
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
	public:
		CEditSvr * m_pEditSvr;
	};

	class CCellEditer : public CInplaceEdit
	{
	public:
		~CCellEditer();
	public: 
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData(){return NULL;}
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	private:
		BOOL RightSide(HWND handle);
		BOOL LeftSide(HWND handle);
	private:
		void Deselect();
		void SelectAll();
	public:
		int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		int OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		int OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	public:
		static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	public:
		int AttachProcEdit();
		int DettachProcEdit();
	public:
		 WNDPROC m_PrevWndFunc;
	};


	class CMaskEditer :  public CInplaceEdit
	{
	public:
		~CMaskEditer();
	public: 
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData(){return NULL;}
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	private:
		BOOL RightSide(HWND handle);
		BOOL LeftSide(HWND handle);
	private:
		void Deselect();
		void SelectAll();
	public:
		int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		int OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
		int OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	public:
		static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	public:
		int AttachProcEdit();
		int DettachProcEdit();
	public:
		 WNDPROC m_PrevWndFunc;
	};

	class CControlEditer : public CInplaceEdit
	{
	public:
		CControlEditer();
		~CControlEditer();
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData(){return NULL;}
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	public:
		int InstanceControl(HWND hParent,LPTSTR xClass);
	public:
		xfControl * m_pxfControl;	
	};

	class CCellCheckBoxEditer : public CInplaceEdit
	{
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};

	class CCellComboBoxEditer : public CInplaceEdit
	{
	public:
		~CCellComboBoxEditer();
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	public:
		int OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
		static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		int AttachProcEdit();
		int DettachProcEdit();
		WNDPROC m_PrevWndFunc;
	};

	class CCellListBoxEditer : public CInplaceEdit
	{
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};

	class CGridFrameSvr;
	class XCELL_API CEditSvr :public CSvrbase
	{
	public:
		struct tagEditCtlBuff
		{
			CInplaceEdit::eEditType	editType;
			CInplaceEdit* pEdit; 
			tagEditCtlBuff():editType(CInplaceEdit::_null),pEdit(NULL){};
			~tagEditCtlBuff(){if(pEdit) delete pEdit;}
		};
	public:
		template class XCELL_API XUtil::xuVector<tagEditCtlBuff* >;
		XUtil::xuVector<tagEditCtlBuff* > m_EditBuffer;
	public:
		CEditSvr(void);
	public:
		~CEditSvr(void);
	public:
		CGridFrameSvr * m_pGridSvr;
	public:
		CInplaceEdit * m_pEdit;
		int m_ACol;
		int m_ARow;
	public:
		int MoveLoc(int x,int y,int cx,int cy,int nCmdShow );
		int UpdateEditContent(int ACol,int ARow);
		int UpdateCellContent(int ACol,int ARow);
		LPTSTR GetEditContent();
		CInplaceEdit * GetEditCtl(int ACol,int ARow);
		int Draw(int ACol,int ARow,HDC hdc,RECT rect);
		int	SelectCell(int ACol,int ARow);
		int HiddenEditor();
	public:
		HWND TipHWMD;
		int ShowTip(int ARow, int ACol, bool f=true);
	};
}}}}
