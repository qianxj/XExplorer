#include "StdAfx.h"
#include "xcWCControl.hpp"
#include "commctrl.h"
#include "xuxutil.h"
#include "xsedit.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		xcWCControl::xcWCControl(void):m_fnCallBack(NULL)
		{
		}

		xcWCControl::~xcWCControl(void)
		{
			HFONT hFont = (HFONT)::SendMessage(m_hWnd,WM_GETFONT,0,0);
			if(hFont) ::DeleteObject(hFont);

			if(GetHWND() && m_fnCallBack)
			{
				SetWindowLongPtr(GetHWND(),GWLP_WNDPROC,(long)m_fnCallBack);
				::SetProp(GetHWND(),L"thisself",0);
			}
		}

		void xcWCControl::Initial()
		{
			//set Control Font;
			LOGFONT logFont;
			if(m_pxfNode)
			{
				XUtil::Css::GetFont(m_pxfNode->GetXPage()->m_pCCssStyle->GetBoxStyle(m_pxfNode->m_nCssIndex),logFont);
				HFONT hFont = ::CreateFontIndirect(&logFont);
				::SendMessage(m_hWnd,WM_SETFONT,(WPARAM)hFont,false);
			}

			if(::_tcsicmp(m_pStrClassName,_T("xdatetimepick"))==0)
			{
				::SendMessage(m_hWnd,DTM_SETSYSTEMTIME,GDT_NONE,0);
			}
			//process xcombobox
			if(::_tcsicmp(m_pStrClassName,_T("xcombobox"))==0)
			{
				IXMLDOMNodeListPtr	pNodeToolList;
				IXMLDOMNodePtr	pNodeTool;
				VARIANT	svalex;
				if(m_pxfNode)
				{

					m_pxfNode->m_pElement->selectNodes(L"item",&pNodeToolList);
					long toolen;

					pNodeToolList->get_length(&toolen);
					for( int l=0;l<toolen;l++)
					{
						(pNodeToolList)->get_item(l,&pNodeTool);
						((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"text",&svalex);
						if(svalex.vt==VT_BSTR && svalex.bstrVal)
						{
							LPTSTR pText;
							pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
							::SendMessage(m_hWnd,CB_ADDSTRING,0,(LPARAM)pText);
							delete pText;
						}else
						{
							BSTR bstr = NULL;
							((IXMLDOMElementPtr)pNodeTool)->get_text(&bstr);
							::SendMessage(m_hWnd,CB_ADDSTRING,0,(LPARAM)bstr);
							::SysFreeString(bstr);
						}
						::VariantClear(&svalex);
						VARIANT	svalex1;
						((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"data",&svalex1);
						if(svalex1.vt==VT_BSTR && svalex1.bstrVal)
						{
							LPTSTR pCombTextData;
							pCombTextData = XUtil::tcsdupFromBstr(svalex1.bstrVal);
							::SendMessage(m_hWnd,CB_SETITEMDATA,l,(LPARAM)pCombTextData);
							//delete pCombTextData;
						}
						::VariantClear(&svalex1);
					}
					m_pxfNode->m_pElement->getAttribute(L"text",&svalex);
					if(svalex.vt==VT_BSTR && svalex.bstrVal)
					{
						LPTSTR pText;
						pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
						int k = ::SendMessage(m_hWnd,CB_FINDSTRINGEXACT,0,(LPARAM)pText);
						if(k > -1)
							::SendMessage(m_hWnd,CB_SETCURSEL,(WPARAM)k,0);	
						else
							::SetWindowText(m_hWnd,pText);
					}
					::VariantClear(&svalex);
				}
			}
			if(::_tcsicmp(m_pStrClassName,_T("xcheckbox"))==0 || ::_tcsicmp(m_pStrClassName,_T("xradiobutton"))==0)
			{
				VARIANT	svalex;
				m_pxfNode->m_pElement->getAttribute(L"checked",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					LPTSTR pText;
					pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
					if(::_tcsicmp(pText,_T("Y"))==0 || ::_tcsicmp(pText,_T("true"))==0 || ::_tcsicmp(pText,_T("1"))==0)
					{
						::SendMessage(m_hWnd, BM_SETCHECK, BST_CHECKED,0);
					}
				}
				::VariantClear(&svalex);
			}
			//radiobutton
			if(::_tcsicmp(m_pStrClassName,_T("xradiobutton"))==0)
			{
				VARIANT	svalex;
				m_pxfNode->m_pElement->getAttribute(L"group",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					LPTSTR pText;
					pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
					if(::_tcsicmp(pText,_T("Y"))==0 || ::_tcsicmp(pText,_T("true"))==0 || ::_tcsicmp(pText,_T("1"))==0)
					{
						long style = ::GetWindowLong(m_hWnd, GWL_STYLE);
						::SetWindowLong(m_hWnd, GWL_STYLE, style | WS_GROUP);
					}
				}
				::VariantClear(&svalex);
			}

			//process xcolumn
			if(::_tcsicmp(m_pStrClassName,_T("xcolumn"))==0)
			{
				VARIANT	svalex;
				m_pxfNode->m_pElement->getAttribute(L"initial",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					LPTSTR pText;
					pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
					if(::_tcsicmp(pText,_T("today"))==0)
					{
						SYSTEMTIME	st;
						GetLocalTime(&st);
						TCHAR buf[80];
						::_stprintf_s(buf,80,_T("%i-%i-%i"),st.wYear,st.wMonth,st.wDay);
						::SetWindowText(m_hWnd,buf);
					}else
						::SetWindowText(m_hWnd,pText);
					delete pText;
				}
				::VariantClear(&svalex);
			}

			//process xedit
			if(::_tcsicmp(m_pStrClassName,_T("xedit"))==0 || ::_tcsicmp(m_pStrClassName,_T("xcolumn"))==0 || ::_tcsicmp(m_pStrClassName,_T("xmedit"))==0)
			{
				VARIANT	svalex;
				m_pxfNode->m_pElement->getAttribute(L"text",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					LPTSTR pText;
					pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
					::SetWindowText(m_hWnd,pText);
				}
				::VariantClear(&svalex);

				m_pxfNode->m_pElement->getAttribute(L"readonly",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					if(::_tcsicmp(svalex.bstrVal,_T("yes"))==0 || ::_tcsicmp(svalex.bstrVal,_T("true"))==0 ||
						::_tcsicmp(svalex.bstrVal,_T("1"))==0)
					{
						::SendMessage(m_hWnd,EM_SETREADONLY,true,0);
					}
				}
			}

			//process xlistview
			if(::_tcsicmp(m_pStrClassName,_T("xlistview"))==0)
			{
				IXMLDOMNodeListPtr	pNodeToolList;
				IXMLDOMNodePtr	pNodeTool;
				m_pxfNode->m_pElement->selectNodes(L"columns/column",&pNodeToolList);
				long toolen;

				LVCOLUMN	lvcolumn;
				VARIANT	svalex;
				pNodeToolList->get_length(&toolen);
				for( int l=0;l<toolen;l++)
				{
					(pNodeToolList)->get_item(l,&pNodeTool);
					
					((IXMLDOMElementPtr)pNodeTool)->getAttribute(L"caption",&svalex);
					if(svalex.vt==VT_BSTR && svalex.bstrVal)
					{
						LPTSTR pText;
						pText = XUtil::tcsdupFromBstr(svalex.bstrVal);
						
						lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT |LVCF_WIDTH ;
						lvcolumn.pszText = pText;
						lvcolumn.cx = 120; 
						lvcolumn.cchTextMax = (int)::_tcslen(pText);
						lvcolumn.fmt = 0; 
						lvcolumn.iSubItem = l;
						lvcolumn.iImage = 0;

						ListView_InsertColumn(m_hWnd,l,&lvcolumn);
						delete pText;
					}
					::VariantClear(&svalex);
				}
				m_pxfNode->m_pElement->getAttribute(L"imagelist",&svalex);
				if(!(svalex.vt==VT_BSTR && svalex.bstrVal))m_pxfNode->m_pElement->getAttribute(L"imagelist.large",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					WCHAR* pNextStr;
					int index = ::wcstol(svalex.bstrVal,&pNextStr,10);
					if(index >=0 && m_pxfNode->GetXPage()->m_pResourceMgr && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
					{
						::SendMessage(m_hWnd, LVM_SETIMAGELIST,LVSIL_NORMAL,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
					}else if(index >=0 && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
					{
						::SendMessage(m_hWnd, LVM_SETIMAGELIST,LVSIL_NORMAL,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
					}
				}
				::VariantClear(&svalex);
				m_pxfNode->m_pElement->getAttribute(L"imagelist.state",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					WCHAR* pNextStr;
					int index = ::wcstol(svalex.bstrVal,&pNextStr,10);
					if(index >=0 && m_pxfNode->GetXPage()->m_pResourceMgr && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
					{
						::SendMessage(m_hWnd, LVM_SETIMAGELIST,LVSIL_STATE,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
					}else if(index >=0 && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
					{
						::SendMessage(m_hWnd, LVM_SETIMAGELIST,LVSIL_STATE,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
					}
				}
				::VariantClear(&svalex);
				m_pxfNode->m_pElement->getAttribute(L"imagelist.small",&svalex);
				if(svalex.vt==VT_BSTR && svalex.bstrVal)
				{
					WCHAR* pNextStr;
					int index = ::wcstol(svalex.bstrVal,&pNextStr,10);
					if(index >=0 && m_pxfNode->GetXPage()->m_pResourceMgr && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
					{
						::SendMessage(m_hWnd, LVM_SETIMAGELIST,LVSIL_SMALL,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
					}else if(index >=0 && index < (int)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists.size())
					{
						::SendMessage(m_hWnd, LVM_SETIMAGELIST,LVSIL_SMALL,(LPARAM)m_pxfNode->GetXPage()->m_pResourceMgr->m_hImageLists[index]);
					}
				}
				::VariantClear(&svalex);

			}
			if(m_pxfNode && m_pxfNode->m_pElement)
			{
				VARIANT	bindXc;
				m_pxfNode->m_pElement->getAttribute(L"bind",&bindXc);
				if(bindXc.vt==VT_BSTR && bindXc.bstrVal)
				{
					LPTSTR pText;
					pText = XUtil::tcsdupFromBstr(bindXc.bstrVal);
					this->addBindControl(0xd, pText);

					m_fnCallBack = (WNDPROC)GetWindowLongPtr(GetHWND(),GWLP_WNDPROC);
					SetWindowLongPtr(GetHWND(),GWLP_WNDPROC,(long)&xcWCControl::fnCallBack);
					::SetProp(GetHWND(),L"thisself",(HANDLE)this);
				}
			}
		}
		LRESULT CALLBACK xcWCControl::fnCallBack(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
		{
			xcWCControl * pThis = ((xcWCControl *)GetProp(hwnd,L"thisself"));
			switch(uMsg)
			{
				case WM_KEYDOWN:
					{
						UINT nChar = (UINT)wParam;
						pThis->fireBindControl(nChar);
					}
				default:
					return CallWindowProc(pThis->m_fnCallBack,hwnd,uMsg,wParam,lParam);
			}
		}
		int xcWCControl::AdjustControlRect(RECT rect,bool redraw)
		{
			if(::_tcsicmp(m_pStrClassName,_T("xcombobox"))==0)
			{
				::MoveWindow(m_hWnd,rect.left,rect.top,rect.right - rect.left + 1,rect.bottom - rect.top +1 + 8*20,redraw);
				return 1;
			}
			else
				return xfControl::AdjustControlRect(rect,redraw);
		}
		
		int xcWCControl::GetXml(BSTR &bstr)
		{
			bstr = NULL;
			if(m_pxfNode && m_pxfNode->m_strNodeName)
			{
				if( ::IsWindow(m_hWnd))
				{
					int nLen = ::GetWindowTextLength(m_hWnd);
					if(nLen < 1)
					{
						bstr = ::SysAllocStringLen(NULL,(UINT)wcslen(m_pxfNode->m_strNodeName)*2+6);
						_stprintf_s(bstr,(UINT)wcslen(m_pxfNode->m_strNodeName)*2+6 + 2,_T("<%s></%s>"),m_pxfNode->m_strNodeName,m_pxfNode->m_strNodeName);
						return 1;
					}
					else
					{
						LPTSTR pText = new TCHAR[nLen + 1];
						::GetWindowText(m_hWnd,pText,nLen + 1);
						
						LPTSTR pXmlText = XUtil::ToXmlText(pText);
						if(pXmlText)
							delete pText;
						else 
							pXmlText = pText;
						bstr = ::SysAllocStringLen(NULL,(UINT)wcslen(m_pxfNode->m_strNodeName)*2+6 + wcslen(pXmlText));
						_stprintf_s(bstr,(UINT)wcslen(m_pxfNode->m_strNodeName)*2+6+ wcslen(pXmlText),_T("<%s>%s</%s>"),m_pxfNode->m_strNodeName,pXmlText,m_pxfNode->m_strNodeName);
						if(pXmlText)delete pXmlText;

						return 1;
					}
				}
			}
			return 0;
		}

	}
}
