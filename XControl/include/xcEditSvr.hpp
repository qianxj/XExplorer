#pragma once

#include "xframe.h"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class CInplaceEdit : public xfControl
	{
	public:
		enum eEditType{_null,_edit,_combobox,_date,_datetime,_time,_comboboex,_radio,_checkbox,_autofill,_control,_flowchart};
	public:
		LPTSTR	m_szCotrolClass;
	public:
		CInplaceEdit(void):m_szCotrolClass(NULL){};
		~CInplaceEdit(void){if(m_szCotrolClass) delete m_szCotrolClass;}
	public:
		virtual LPTSTR	GetText()=0;
		virtual int	SetText(LPTSTR pStrContent)=0;
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow)=0;
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText)=0;
	public:
		virtual void Invalidate(){::InvalidateRect(this->m_hWnd,NULL,TRUE);}
		virtual void SetFont(LOGFONT &logFont,bool bRedraw){SendMessage(m_hWnd,WM_SETFONT,(WPARAM)&logFont,bRedraw);}
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL){return 0;}
	};

	class CCellEditer : public CInplaceEdit
	{
	public:
		virtual LPTSTR	GetText();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};

	class xfControl;
	class CControlEditer : public CInplaceEdit
	{
	public:
		CControlEditer();
		~CControlEditer();
	public:
		virtual LPTSTR	GetText();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	public:
		int InstanceControl(HWND hParent,LPTSTR xClass);
	public:
		xfControl * m_pxfControl;	
	};


	class CCellComboBoxEditer : public CInplaceEdit
	{
	public:
		virtual LPTSTR	GetText();
		virtual int	SetText(LPTSTR pStrContent);
		virtual int MoveLoc(int x,int y,int cx,int cy,int nCmdShow);
		virtual int Draw(HDC hdc,RECT rect,LPTSTR pStrText);
	};

	class XCONTROL_API CEditSvr :public xfAreaObject
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
		template class XCONTROL_API XUtil::xuVector<tagEditCtlBuff* >;
		XUtil::xuVector<tagEditCtlBuff* > m_EditBuffer;
	public:
		CEditSvr(void);
	public:
		~CEditSvr(void);
	public:
		Hxsoft::XFrame::xfControl * m_pOwner;
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
		LPTSTR GetCellContent(int ACol,int ARow);
		int  SetCellContent(int ACol,int ARow,LPTSTR pStr);
		LPTSTR  GetEditType(int ACol,int ARow);
		int  GetCellRect(int ACol,int ARow,RECT &rt);
	public:
		LPTSTR  (* m_pGetCellContent)(Hxsoft::XFrame::xfControl * pOwner,int ACol,int ARow) ; 
		int		(*m_pSetCellContent)(Hxsoft::XFrame::xfControl * pOwner,int ACol,int ARow,LPTSTR pStr);
		LPTSTR  (*m_pGetEditType)(Hxsoft::XFrame::xfControl * pOwner,int ACol,int ARow);
		int		(*m_pGetCellRect)(Hxsoft::XFrame::xfControl * pOwner,int ACol,int ARow,RECT &rt);
		HFONT   (*m_pGetFont)(Hxsoft::XFrame::xfControl * pOwner,int ACol,int ARow);
	};
}}}