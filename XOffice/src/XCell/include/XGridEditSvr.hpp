#pragma once
#include "xofbase.h"
#include "xframe.h"
#include "xuxutil.h"
#include "XGridReport.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class CEditSvrEx;
	class CInplaceEditEx : public Hxsoft::XFrame::xfControl
	{
	public:
		enum eEditType{_null,_edit,_combobox,_date,_datetime,_time,_comboboex,_radio,_checkbox,_autofill,_control,_flowchart};
	public:
		LPTSTR	m_szCotrolClass;
	public:
		CInplaceEditEx(void):m_szCotrolClass(NULL){};
		~CInplaceEditEx(void){if(m_szCotrolClass) delete m_szCotrolClass;}
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
		static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	public:
		int AttachProcEdit();
		int DettachProcEdit();
	public:
		static BOOL RightSide(HWND handle);
		static BOOL LeftSide(HWND handle);
	public:
		WNDPROC m_PrevWndFunc;
	private:		
		static bool ToLeft(HWND handle);
		static bool ToRight(HWND handle);
		static bool ToUp(HWND handle);
		static bool ToDown(HWND handle);
	public:
		CEditSvrEx * m_pEditSvr;
	};

	class CCellEditerEx : public CInplaceEditEx
	{
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData(){return NULL;};
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};

	/*
	class CMaskEditer : public CInplaceEditEx
	{
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData(){return NULL;};
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};
	*/

	class xfControl;
	class CControlEditerEx : public CInplaceEditEx
	{
	public:
		CControlEditerEx();
		~CControlEditerEx();
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData(){return NULL;};
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	public:
		int InstanceControl(HWND hParent,LPTSTR xClass);
	public:
		xfControl * m_pxfControl;	
	};

	class CCellCheckBoxEditerEx : public CInplaceEditEx
	{
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};
	class CCellComboBoxEditerEx : public CInplaceEditEx
	{
	public:
		virtual LPTSTR	GetText();
		virtual LPTSTR 	GetTextData();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};

	class CGridFrameSvr;
	class XCELL_API CEditSvrEx :public CSvrbase
	{
	public:
		struct tagEditCtlBuff
		{
			CInplaceEditEx::eEditType	editType;
			CInplaceEditEx* pEdit; 
			tagEditCtlBuff():editType(CInplaceEditEx::_null),pEdit(NULL){};
			~tagEditCtlBuff(){if(pEdit) delete pEdit;}
		};
	public:
		template class XCELL_API XUtil::xuVector<tagEditCtlBuff* >;
		XUtil::xuVector<tagEditCtlBuff* > m_EditBuffer;
	public:
		CEditSvrEx(void);
	public:
		~CEditSvrEx(void);
	public:
		class XGridReport * m_pGridReport;
	public:
		CInplaceEditEx * m_pEdit;
		int m_ACol;
		int m_ARow;
	public:
		int MoveLoc(int x,int y,int cx,int cy,int nCmdShow );
		int UpdateEditContent(int ACol,int ARow);
		int UpdateCellContent(int ACol,int ARow);
		LPTSTR GetEditContent();
		CInplaceEditEx * GetEditCtl(int ACol,int ARow);
		int Draw(int ACol,int ARow,HDC hdc,RECT rect);
		int	SelectCell(int ACol,int ARow);
		int HiddenEditor();
		int ShowEditor(int ACol,int ARow);
	};
}}}}
