#pragma once
#include "xofbase.h"
#include "xframe.h"
#include "xuxutil.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	class XFlowChartObject;
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

	class CEditSvr :public CSvrbase
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
		//template class XUtil::xuVector<tagEditCtlBuff* >;
		XUtil::xuVector<tagEditCtlBuff* > m_EditBuffer;
	public:
		CEditSvr(void);
	public:
		~CEditSvr(void);
	public:
		class XOfficeSheetSvr * m_pSheetSvr;
	public:
		CInplaceEdit * m_pEdit;
	public:
		XFlowChartObject * m_pObject;
	public:
		int MoveLoc(int x,int y,int cx,int cy,int nCmdShow );
		int UpdateEditContent(XFlowChartObject * pObject);
		int UpdateContent(XFlowChartObject * pObject);
		LPTSTR GetEditContent();

		CInplaceEdit * GetEditCtl(XFlowChartObject * pObject);
		
		int Draw(int ACol,int ARow,HDC hdc,RECT rect);
		
		int	SelectObject(XFlowChartObject * pObject);
		int HiddenEditor();
	};
}}}}
