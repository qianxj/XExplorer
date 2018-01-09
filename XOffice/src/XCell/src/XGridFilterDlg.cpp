#include "StdAfx.h"
#include "XGridFilterDlg.hpp"
#include "gridframesvr.hpp"
#include "xofobject.h"
#include "XGridHeaderEx.hpp"
#include "XGridReport.hpp"
#include "XGridSheetSvrEx.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	XGridFilterDlg::XGridFilterDlg(void)
	{
		SetEventListons();
	}

	XGridFilterDlg::~XGridFilterDlg(void)
	{
		UnSetEventListons();
	}

	bool XGridFilterDlg::SetEventListons()
	{
		this->AttachListon(_T("join1"),BN_CLICKED,(FNEvent)&XGridFilterDlg::EvtRadioClick);
		this->AttachListon(_T("join2"),BN_CLICKED,(FNEvent)&XGridFilterDlg::EvtRadioClick);
		return true;
	}

	bool XGridFilterDlg::UnSetEventListons()
	{
		this->ClearAllListonsTo();
		return true;
	}

	bool XGridFilterDlg::EvtRadioClick(TEvent* pEvent,LPARAM lParam)
	{
		Hxsoft::XFrame::xfControl * xcJoin1 = GetControl(_T("join1"));
		Hxsoft::XFrame::xfControl * xcJoin2 = GetControl(_T("join2"));

		::SendMessage(xcJoin1->GetHWND(), BM_SETCHECK, BST_UNCHECKED, NULL);
		::SendMessage(xcJoin2->GetHWND(), BM_SETCHECK, BST_UNCHECKED, NULL);

		::SendMessage((HWND)pEvent->lParam, BM_SETCHECK, BST_CHECKED, NULL);
		return true;
	}


	LPTSTR XGridFilterDlg::SetFilterStr(LPTSTR pColumn, LPTSTR pOp, LPTSTR pText,bool bind)
	{
		if(!pText || !pText[0]) return NULL;
		if(!pOp) return NULL;


		LPTSTR pStr = new TCHAR[1024];
		XGridHeaderEx* pHeader = (XGridHeaderEx *)m_pParam;
		LPTSTR celltype = pHeader->m_pGridReportSvr->GetColumnProp(pHeader->m_nQueryColIndex,L"celltype");
		bool bNumber = false;
		bool bInteger = false;
		bool bDouble = false;
		if(celltype && _tcscmp(celltype,L"xs:decimal")==0) bDouble = true;
		if(celltype && _tcscmp(celltype,L"xs:integer")==0) bInteger = true;
		if(bInteger || bDouble) bNumber = true;
		if(!bind)
		{
			if(_tcscmp(pOp, L"等于") == 0)
				_stprintf_s(pStr,1024,_T("(%s='%s' or (count(%s/@_displaystring)=1 and %s/@_displaystring='%s'))") ,pColumn, pText, pColumn, pColumn, pText);
			else if(_tcscmp(pOp, L"不等于") == 0)
				_stprintf_s(pStr,1024,_T("(count(%s)=0 or (count(%s/@_displaystring)=0 and %s!='%s') or (count(%s/@_displaystring)=1 and %s/@_displaystring!='%s'))") ,pColumn,pColumn,pColumn, pText, pColumn, pColumn, pText);
			else if(_tcscmp(pOp, L"大于") == 0)
			{
				if(bNumber)
				{
					_stprintf_s(pStr,1024,_T("(number(%s)>%s or (count(%s/@_displaystring)=1 and number(%s/@_displaystring) > %s ))") ,pColumn, pText, pColumn, pColumn, pText);
				}else
					_stprintf_s(pStr,1024,_T("(ms:string-compare(%s,'%s', 'en-US', 'i')>0 or (count(%s/@_displaystring)=1 and ms:string-compare(%s/@_displaystring,'%s', 'en-US', 'i')>0))") ,pColumn, pText, pColumn, pColumn, pText);
			}
			else if(_tcscmp(pOp, L"大于等于") == 0)
			{
				if(bNumber)
				{
					_stprintf_s(pStr,1024,_T("(number(%s)>=%s or (count(%s/@_displaystring)=1 and number(%s/@_displaystring) >= %s ))") ,pColumn, pText, pColumn, pColumn, pText);
				}else
					_stprintf_s(pStr,1024,_T("(ms:string-compare(%s,'%s')>=0 or (count(%s/@_displaystring)=1 and ms:string-compare(%s/@_displaystring,'%s')>=0))") ,pColumn, pText, pColumn, pColumn, pText);
			}
			else if(_tcscmp(pOp, L"小于") == 0)
			{
				if(bNumber)
				{
					_stprintf_s(pStr,1024,_T("(number(%s)<%s or (count(%s/@_displaystring)=1 and number(%s/@_displaystring) < %s ))") ,pColumn, pText, pColumn, pColumn, pText);
				}else
					_stprintf_s(pStr,1024,_T("(ms:string-compare(%s,'%s')<0 or (count(%s/@_displaystring)=1 and ms:string-compare(%s/@_displaystring,'%s')<0))") ,pColumn, pText, pColumn, pColumn, pText);
			}
			else if(_tcscmp(pOp, L"小于等于") == 0)
			{
				if(bNumber)
				{
					_stprintf_s(pStr,1024,_T("(number(%s)<=%s or (count(%s/@_displaystring)=1 and number(%s/@_displaystring) <= %s ))") ,pColumn, pText, pColumn, pColumn, pText);
				}else
					_stprintf_s(pStr,1024,_T("(ms:string-compare(%s,'%s')<=0 or (count(%s/@_displaystring)=1 and ms:string-compare(%s/@_displaystring,'%s')<=0))") ,pColumn, pText, pColumn, pColumn, pText);
			}
			else if(_tcscmp(pOp, L"包含") == 0)
				_stprintf_s(pStr,1024,_T("(contains(%s, '%s') or (count(%s/@_displaystring)=1 and contains(%s/@_displaystring,'%s')))") ,pColumn, pText, pColumn, pColumn, pText);
			else if(_tcscmp(pOp, L"不包含") == 0)
				_stprintf_s(pStr,1024,_T("(not(contains(%s, '%s')) or (count(%s/@_displaystring)=1 and not(contains(%s/@_displaystring,'%s'))))") ,pColumn, pText, pColumn, pColumn, pText);
			else
				pStr = NULL;
		}else
		{
			if(_tcscmp(pOp, L"等于") == 0)
				_stprintf_s(pStr,1024,_T("eq %s") ,pText);
			else if(_tcscmp(pOp, L"不等于") == 0)
				_stprintf_s(pStr,1024,_T("neq %s") ,pText);
			else if(_tcscmp(pOp, L"大于") == 0)
			{
				if(bInteger)
					_stprintf_s(pStr,1024,_T("igt %s") ,pText);
				else if(bDouble)
					_stprintf_s(pStr,1024,_T("fgt %s") ,pText);
				else
					_stprintf_s(pStr,1024,_T("gt %s") ,pText);
			}
			else if(_tcscmp(pOp, L"大于等于") == 0)
			{
				if(bInteger)
					_stprintf_s(pStr,1024,_T("igte %s") ,pText);
				else if(bDouble)
					_stprintf_s(pStr,1024,_T("fgte %s") ,pText);
				else
					_stprintf_s(pStr,1024,_T("gte %s") ,pText);
			}
			else if(_tcscmp(pOp, L"小于") == 0)
			{
				if(bInteger)
					_stprintf_s(pStr,1024,_T("ilt %s") ,pText);
				else if(bDouble)
					_stprintf_s(pStr,1024,_T("flt %s") ,pText);
				else
					_stprintf_s(pStr,1024,_T("lt %s") ,pText);
			}
			else if(_tcscmp(pOp, L"小于等于") == 0)
			{
				if(bInteger)
					_stprintf_s(pStr,1024,_T("ilte %s") ,pText);
				else if(bDouble)
					_stprintf_s(pStr,1024,_T("flte %s") ,pText);
				else
					_stprintf_s(pStr,1024,_T("lte %s") ,pText);
			}
			else if(_tcscmp(pOp, L"包含") == 0)
				_stprintf_s(pStr,1024,_T("con %s") ,pText);
			else if(_tcscmp(pOp, L"不包含") == 0)
				_stprintf_s(pStr,1024,_T("nocon %s") ,pText);
			else
				pStr = NULL;
		}

		return pStr;
	}

	int XGridFilterDlg::OnXCommand(LPCTSTR	pStrID, Hxsoft::XFrame::xfControl * pControl)
	{
		if(!pStrID) return 0;
		
		if(_tcscmp(pStrID, L"xmConcel") == 0)
		{
			m_dwStatus &= ~WST_RESPONSELOOP;
			return 0;
		}
		else if(_tcscmp(pStrID, L"xmOk") == 0)
		{
			XGridHeaderEx* pHeader = (XGridHeaderEx *)m_pParam;
			//2017.8.23 LPTSTR pColumn = (LPTSTR)pHeader->m_pGridReportSvr->GetColumnName(pHeader->m_nQueryColIndex + 1);
			LPTSTR pColumn = (LPTSTR)pHeader->m_pGridReportSvr->GetColumnName(pHeader->m_nQueryColIndex);
			
			bool bind = false;
			if(pHeader->m_pGridReportSvr->m_pSheetSvr->m_pSheetCtrl->hasBindData()) bind = true;

			Hxsoft::XFrame::xfControl * xcOp1 = GetControl(_T("op1"));
			Hxsoft::XFrame::xfControl * xcValue1 = GetControl(_T("value1"));
			LPTSTR pOp1 = new TCHAR[1024];
			LPTSTR pValue1 = new TCHAR[1024];
			::GetWindowText(xcOp1->GetHWND(), pOp1, 1024);
			::GetWindowText(xcValue1->GetHWND(), pValue1, 1024);
			LPTSTR pStr1 = SetFilterStr(pColumn, pOp1, pValue1,bind);

			Hxsoft::XFrame::xfControl * xcOp2 = GetControl(_T("op2"));
			LPTSTR pOp2 = new TCHAR[1024];
			::GetWindowText(xcOp2->GetHWND(), pOp2, 1024);
			Hxsoft::XFrame::xfControl * xcValue2 = GetControl(_T("value2"));
			LPTSTR pValue2 = new TCHAR[1024];
			::GetWindowText(xcValue2->GetHWND(), pValue2, 1024);
			LPTSTR pStr2 = SetFilterStr(pColumn, pOp2, pValue2,bind);

			Hxsoft::XFrame::xfControl * xcOp3 = GetControl(_T("op3"));
			LPTSTR pOp3 = new TCHAR[1024];
			::GetWindowText(xcOp2->GetHWND(), pOp3, 1024);
			Hxsoft::XFrame::xfControl * xcValue3 = GetControl(_T("value3"));
			LPTSTR pValue3 = new TCHAR[1024];
			::GetWindowText(xcValue3->GetHWND(), pValue3, 1024);
			LPTSTR pStr3 = SetFilterStr(pColumn, pOp3, pValue3,bind);

			Hxsoft::XFrame::xfControl * xcOp4 = GetControl(_T("op4"));
			LPTSTR pOp4 = new TCHAR[1024];
			::GetWindowText(xcOp2->GetHWND(), pOp4, 1024);
			Hxsoft::XFrame::xfControl * xcValue4 = GetControl(_T("value4"));
			LPTSTR pValue4 = new TCHAR[1024];
			::GetWindowText(xcValue4->GetHWND(), pValue4, 1024);
			LPTSTR pStr4 = SetFilterStr(pColumn, pOp4, pValue4,bind);

			wchar_t* join = L"or";
			Hxsoft::XFrame::xfControl * xcJoin1 = GetControl(_T("join1"));
			int nRet = ::SendMessage(xcJoin1->GetHWND(), BM_GETCHECK, NULL, NULL);
			if(nRet == BST_CHECKED) join = L"and";
			std::wstring str;
			int cnt = 0;
			if(pStr1 && pStr1[0])
			{
				str = pStr1;
				cnt++;
			}
			if(pStr2 && pStr2[0])
			{
				cnt++;
				if(str== L"")
					str = pStr2;
				else
				{
					if(bind)
						str = str + L"!#!"+ pStr2;
					else
						str = str + L" "+ join + L" " + pStr2;
				}
			}
			if(pStr3 && pStr3[0])
			{
				cnt++;
				if(str== L"")
					str = pStr3;
				else
				{
					if(bind)
						str = str + L"!#!"+ pStr3;
					else
						str = str + L" "+ join + L" " + pStr3;
				}
			}
			if(pStr4 && pStr4[0])
			{
				cnt++;
				if(str== L"")
					str = pStr4;
				else
				{
					if(bind)
						str = str + L"!#!"+ pStr4;
					else
						str = str + L" "+ join + L" " + pStr4;
				}
			}

			if(pStr1) delete pStr1;
			if(pStr2) delete pStr2;
			if(pStr3) delete pStr3;
			if(pStr4) delete pStr4;

			if(bind && cnt >= 1)str = (std::wstring)L"!!!"+ join + L" "+ str;

			if(bind)
				pHeader->m_pGridReportSvr->m_pSheetSvr->m_pSheetCtrl->bindGetData(pHeader->m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->GetXPath(), -1, (wchar_t*)((std::wstring)L"filter:"+ pColumn+L","+str).c_str());
			else
				pHeader->m_pGridReportSvr->GetGridDataSvr()->m_pContentSvr->ColumnFilter(pColumn, wcsdup(str.c_str()));
			::InvalidateRect(pHeader->GetHWND(),NULL,false);

			m_dwStatus &= ~WST_RESPONSELOOP;
		}

		return 1;
	}

	struct  StringLess
	{
		bool operator()(const wstring & lhs, const wstring &rhs) const
		{	
			return _tcsicoll(lhs.c_str(), rhs.c_str())<0?true:false;
		}
	};

	struct  DoubleLess
	{
		bool operator()(const wstring & lhs, const wstring &rhs) const
		{	
			return wcstod(lhs.c_str(),0) < wcstod(rhs.c_str(),0);
		}
	};

	struct  IntLess
	{
		bool operator()(const wstring & lhs, const wstring &rhs) const
		{	
			return wcstol(lhs.c_str(),0,10) < wcstol(rhs.c_str(),0,10);
		}
	};

	int XGridFilterDlg::OnOpen()
	{
		XGridHeaderEx* pHeader = (XGridHeaderEx *)m_pParam;
		//2017.8.23 LPTSTR pColumn = (LPTSTR)pHeader->m_pGridReportSvr->GetColumnName(pHeader->m_nQueryColIndex + 1);
		LPTSTR pColumn = (LPTSTR)pHeader->m_pGridReportSvr->GetColumnName(pHeader->m_nQueryColIndex);
		LPTSTR pColumnName = (LPTSTR)pHeader->m_pGridReportSvr->GetTitleName(pHeader->m_nQueryColIndex);

		Hxsoft::XFrame::xfControl * xcName = GetControl(_T("columnName"));
		::SetWindowText(xcName->GetHWND(), pColumnName);
		::SendMessage(xcName->GetHWND(), EM_SETREADONLY, (LPARAM)true, NULL);

		Hxsoft::XFrame::xfControl * xcValue1 = GetControl(_T("value1"));
		Hxsoft::XFrame::xfControl * xcValue2 = GetControl(_T("value2"));
		Hxsoft::XFrame::xfControl * xcValue3 = GetControl(_T("value3"));
		Hxsoft::XFrame::xfControl * xcValue4 = GetControl(_T("value4"));
		int counts = pHeader->m_pGridReportSvr->GetRowCount();

		LPTSTR celltype = pHeader->m_pGridReportSvr->GetColumnProp(pHeader->m_nQueryColIndex,L"celltype");
		if(celltype && _tcscmp(celltype,L"xs:decimal")==0)
		{
			std::set<wstring,DoubleLess> values;
			for(int i=1;i<=counts; i++)
			{
				//2017.8.23 LPTSTR pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemDisplayString(i, pHeader->m_nQueryColIndex + 1);
				LPTSTR pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemDisplayString(i, pHeader->m_nQueryColIndex);
				//2017.8.23 if(!pText) pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemString(i, pHeader->m_nQueryColIndex + 1);
				if(!pText) pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemString(i, pHeader->m_nQueryColIndex);
				if(pText && _tcscmp(pText, L"") != 0) values.insert(pText);
			}
			for(std::set<wstring,DoubleLess>::const_iterator tr=values.begin();tr!=values.end();tr++)
			{
				::SendMessage(xcValue1->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue2->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue3->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue4->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
			}
		}
		else if(celltype && _tcscmp(celltype,L"xs:integer")==0)
		{
			std::set<wstring,IntLess> values;
			for(int i=1;i<=counts; i++)
			{
				//2017.8.23 LPTSTR pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemDisplayString(i, pHeader->m_nQueryColIndex + 1);
				LPTSTR pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemDisplayString(i, pHeader->m_nQueryColIndex);
				//2017.8.23 if(!pText) pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemString(i, pHeader->m_nQueryColIndex + 1);
				if(!pText) pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemString(i, pHeader->m_nQueryColIndex);
				if(pText && _tcscmp(pText, L"") != 0) values.insert(pText);
			}
			for(std::set<wstring,IntLess>::const_iterator tr=values.begin();tr!=values.end();tr++)
			{
				::SendMessage(xcValue1->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue2->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue3->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue4->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
			}
		}
		else
		{
			std::set<wstring,StringLess> values;
			for(int i=1;i<=counts; i++)
			{
				//2017.8.23 LPTSTR pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemDisplayString(i, pHeader->m_nQueryColIndex + 1);
				LPTSTR pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemDisplayString(i, pHeader->m_nQueryColIndex);
				//2017.8.23 if(!pText) pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemString(i, pHeader->m_nQueryColIndex + 1);
				if(!pText)pText = (LPTSTR)pHeader->m_pGridReportSvr->GetItemString(i, pHeader->m_nQueryColIndex);
				if(pText && _tcscmp(pText, L"") != 0) values.insert(pText);
			}
			for(std::set<wstring,StringLess>::const_iterator tr=values.begin();tr!=values.end();tr++)
			{
				::SendMessage(xcValue1->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue2->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue3->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
				::SendMessage(xcValue4->GetHWND(), CB_ADDSTRING, NULL, (WPARAM)(*tr).c_str());
			}
		}

		Hxsoft::XFrame::xfControl * xcJoin1 = GetControl(_T("join1"));
		::SendMessage(xcJoin1->GetHWND(), BM_SETCHECK, BST_CHECKED, NULL);

		return 1;
	}

}}}}