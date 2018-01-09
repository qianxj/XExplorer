#include "stdafx.h"
#include "xcPropSheet.hpp"

namespace Hxsoft
{
	namespace XFrame 
	{

		xcPropSheetCtrl::xcPropSheetCtrl():m_pXml(new xbXml)
		{
			this->AttachListon(WM_PAINT,(FNEvent)&xcPropSheetCtrl::EvtPaint);
			this->AttachListon(WM_LBUTTONDOWN,(FNEvent)&xcPropSheetCtrl::EvtLButtonDown);
			m_pEditSvr = new XControl::CEditSvr;
			m_pEditSvr->m_pGetCellContent = &xcPropSheetCtrl::GetCellContent;
			m_pEditSvr->m_pSetCellContent = &xcPropSheetCtrl::SetCellContent;
			m_pEditSvr->m_pGetCellRect = &xcPropSheetCtrl::GetCellRect;
			m_pEditSvr->m_pGetEditType = &xcPropSheetCtrl::GetEditType;
			m_pEditSvr->m_pOwner = this;
			m_pEditSvr->m_pGetFont =  &xcPropSheetCtrl::GetEditFont;

			SetRect(&m_editRt,0,0,0,0);

			ib_sort=false;
			il_startx=0,il_starty=0;
			il_width,il_height;

			il_treebp = 17;
			il_treebox = 11;
			il_toolh = 24;
			il_labelsw = 100;
			il_startrow = 1; //from start 1
			il_cureditrow = 0;
			il_cellheight = 24;
			//long	il_row;
			il_startY = 1;

			HDC hdc;
			hdc = GetDC(NULL);

			LOGFONT	logFont;
			logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
			logFont.lfCharSet = DEFAULT_CHARSET ;
			logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logFont.lfEscapement = 0;
			_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
			logFont.lfItalic = 0;
			logFont.lfOrientation = 0;
			logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logFont.lfPitchAndFamily = DEFAULT_PITCH;
			logFont.lfQuality = CLEARTYPE_QUALITY;
			logFont.lfStrikeOut = 0;
			logFont.lfUnderline = 0;
			logFont.lfWeight = FW_NORMAL;
			logFont.lfWidth = 0;

			::ReleaseDC(NULL,hdc);


			m_hFont =::CreateFontIndirect(&logFont);		

		}
		xcPropSheetCtrl::~xcPropSheetCtrl()
		{
			if(m_pXml)delete m_pXml;
			if(m_pEditSvr) delete m_pEditSvr;

			if(m_hFont)delete m_hFont;
		}

		bool xcPropSheetCtrl::EvtLButtonDown(TEvent* pEvent,LPARAM lParam)
		{
			int xpos = pEvent->mouse.point.x;
			int ypos = pEvent->mouse.point.y;

			long	ll_row,ll_col;
			long	ll_i = 0,ll_level;
			wstring	ls_expand;
			wstring	ls_value;
			long	ll_cntrow;
			wstring	ls_type,ls_colname;

			IXMLDOMNodeList* 	 luo_nodes;
			vector<IXMLDOMElement *> luo_items;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;

			ll_i++;
			long ll_len;
			m_pXml->m_pXmlDoc->getElementsByTagName(L"row",&luo_nodes);
			luo_nodes->get_length(&ll_len);
			if(ll_len < 1)
			{
				this->m_pxfNode->m_pElement->getElementsByTagName(L"row", &luo_nodes);
				luo_nodes->get_length(&ll_len);
			}
			while(ll_i<=ll_len)
			{
				luo_nodes->get_item(ll_i -1,&pNode);
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pElement->getAttribute(L"level",&var);
				ll_level = var;
				pNode->Release();

				pElement->getAttribute(L"expand",&var);
				ls_expand = var.bstrVal;

				luo_items.push_back(pElement);

				ll_i++ ;
				if(_tcsicmp(ls_expand.c_str(),L"false")==0)
				{
					while( ll_i <=ll_len)
					{
						luo_nodes->get_item(ll_i -1,&pNode);
						pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
						pElement->getAttribute(L"level",&var);
						int ll_level1 = var;
						pNode->Release();
						pElement->Release();
						if(ll_level >= ll_level1)break;
						ll_i++;
					}
				}
			}


			RECT rc;
			rc = m_pxfNode->m_ContentRect;
			rc.left -= m_pxfNode->m_AreaRect.left;
			rc.top -= m_pxfNode->m_AreaRect.top;
			rc.right  -= m_pxfNode->m_ContentRect.left - rc.left;
			rc.bottom -= m_pxfNode->m_ContentRect.top - rc.top;

			il_height = rc.bottom - rc.top - 10;
			il_width = rc.right - rc.left - 2;

			ll_cntrow = (int)luo_items.size();
			if(xpos > il_startx && xpos < il_startx + il_width)
			{
				if(ypos > il_starty && ypos < il_starty + il_height)
				{
					ll_row =( ypos - il_starty  +  1 )/il_cellheight  +  1;
					if(ll_row > ll_cntrow) return true;
					if(xpos <= il_startx + il_treebp)
						ll_col = 1;
					else if (xpos < il_startx + il_treebp + il_labelsw)
						ll_col = 2;
					else 
						ll_col = 3;
				}
				
				LPTSTR pText;
				pText = xbXml::GetStringAttribe(luo_items[ll_row - 1],L"expand");
				if(pText) ls_expand = pText ;
				pText = xbXml::GetStringAttribe(luo_items[ll_row - 1],L"colname");
				if(pText)  ls_colname = pText;
				pText = xbXml::GetStringAttribe(luo_items[ll_row - 1],L"type");
				if(pText) ls_type = pText;
				if(ll_col == 1)
				{
					//click flag
					if (_tcsicmp(ls_expand.c_str(),L"true")==0|| _tcsicmp(ls_expand.c_str(),L"false")==0)
					{
						if(xpos >= il_startx - 5  && xpos <= il_startx + 5 + il_treebox)
						{
							if(ypos > il_starty + (ll_row - 1) * il_cellheight + (il_cellheight - il_treebox)/2  &&
								ypos <  il_starty + (ll_row - 1) * il_cellheight + (il_cellheight + il_treebox)/2)
							{
								if (_tcsicmp(ls_expand.c_str(),L"true")==0)
									ls_expand = L"false" ;
								else
									ls_expand = L"true" ;
								
								luo_items[ll_row - 1]->setAttribute(L"expand",(_variant_t)ls_expand.c_str());
								if(il_cureditrow!=0)
								{
									this->m_pEditSvr->HiddenEditor();
									il_cureditrow = 0;
								}
								::InvalidateRect(m_hWnd,NULL,false);
							}
						}
					}
				}
				else if(ll_col == 3)
				{
					wstring	ls_category;
					pText = xbXml::GetStringAttribe(luo_items[ll_row - 1],L"category");
					if(pText) ls_category = pText ;
					if(_tcsicmp(ls_category.c_str(),L"group")!=0)
					{
						long ll_x1,ll_y1,ll_w,ll_h;
						ll_x1 = il_startx + il_treebp + il_labelsw ;
						ll_w =  il_width -  ll_x1 ;
						ll_y1 = il_starty + (ll_row - 1)*il_cellheight;
						ll_h = il_cellheight;

						::SetRect(&m_editRt,ll_x1+2,ll_y1 +2,ll_x1 + ll_w - 4,ll_y1 + ll_h - 4);
						if(il_cureditrow != ll_row)
						{
							m_pEditSvr->SelectCell(3,ll_row);
						}
						il_cureditrow = ll_row;
						::InvalidateRect(m_hWnd,NULL,false);
					}
					is_colname = ls_colname ;
				}
			}
			return true;
		}

		bool xcPropSheetCtrl::EvtPaint(TEvent* pEvent,LPARAM lParam)
		{
			HDC hdc;
			hdc = pEvent->paint.hdc;

			RECT	rc;
			HDC	hPaintDC;
			HBITMAP	hBmp;

			hPaintDC = ::CreateCompatibleDC(hdc);

			rc = m_pxfNode->m_ContentRect;
			rc.left -= m_pxfNode->m_AreaRect.left;
			rc.top -= m_pxfNode->m_AreaRect.top;
			rc.right  -= m_pxfNode->m_ContentRect.left - rc.left;
			rc.bottom -= m_pxfNode->m_ContentRect.top - rc.top;

			hBmp = ::CreateCompatibleBitmap(hdc,rc.right - rc.left + 1,rc.bottom - rc.top +1);
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);

			this->DoDraw(hPaintDC);

			::BitBlt(hdc,0,0,rc.right - rc.left + 1,rc.bottom - rc.top +1,hPaintDC,0,0,SRCCOPY);
			hBmp = (HBITMAP)::SelectObject(hPaintDC,hBmp);
			::DeleteObject(hBmp);
			::DeleteDC(hPaintDC); 

			return true;
		}

		int xcPropSheetCtrl::DoDraw(HDC hPaintDC,RECT * pDrawRect)
		{
			RECT rc;
			if(pDrawRect)
				rc = *pDrawRect;
			else
			{
				rc = m_pxfNode->m_ContentRect;
				rc.left -= m_pxfNode->m_AreaRect.left;
				rc.top -= m_pxfNode->m_AreaRect.top;
				rc.right  -= m_pxfNode->m_ContentRect.left - rc.left;
				rc.bottom -= m_pxfNode->m_ContentRect.top - rc.top;
			}

			vector<IXMLDOMElement*> pElementsLists;
			IXMLDOMNodeListPtr	pList;
			IXMLDOMNodePtr pNode;
			IXMLDOMElementPtr pElement;
			long	len;
			long	ll_level;
			_variant_t	val;
			m_pXml->m_pXmlDoc->getElementsByTagName(L"row", &pList);
			pList->get_length(&len);
			if(len < 1)
			{
				this->m_pxfNode->m_pElement->getElementsByTagName(L"row", &pList);
				pList->get_length(&len);
			}
			int i=0;
			while(i<len)
			{
				pList->get_item(i,&pNode);
				pElement = pNode;
				pElement->getAttribute(L"level",&val);
				ll_level = val;
				pElement->getAttribute(L"expand",&val);
				pElement->AddRef();
				pElementsLists.push_back(pElement);
				i++;
				if(wcscmp(val.bstrVal,L"false")==0)
				{
					while(i<len)
					{
						pList->get_item(i,&pNode);
						pElement = pNode;
						pElement->getAttribute(L"level",&val);
						if(ll_level >= (int)val)break;
						i++;
					}
				}
			}


			COLORREF color=RGB(0xdc,0xec,0xdc);

			TRIVERTEX	pVertex[2];
			GRADIENT_RECT pGRect[1];

			pVertex[0].x = 0;
			pVertex[0].y = 0;
			pVertex[0].Red  = (GetRValue(color)>15? GetRValue(color) + 15:0)*256;
			pVertex[0].Green = (GetGValue(color)>15? GetGValue(color) + 15:0)*256;
			pVertex[0].Blue = (GetBValue(color)>15? GetBValue(color) + 15:0)*256;

			pVertex[1].x = rc.right - rc.left;
			pVertex[1].y = rc.bottom - rc.top;
			pVertex[1].Red= (GetRValue(color)>230? 255:GetRValue(color) - 5)*256;
			pVertex[1].Green =(GetGValue(color)>230?255:GetGValue(color) - 5)*256;
			pVertex[1].Blue = (GetBValue(color)>230?255:GetGValue(color) - 5)*256;

			pGRect[0].UpperLeft = 0;
			pGRect[0].LowerRight = 1;

			GradientFill(hPaintDC, pVertex,2,pGRect , 1,GRADIENT_FILL_RECT_H);

			il_height = rc.bottom - rc.top - 10;
			il_width = rc.right - rc.left - 2;

			HBRUSH	hbrush;
			HBRUSH	hbrushw;
			HPEN	hpen,hpen1;
			//hbrush = ::CreateSolidBrush( ::GetSysColor( 15) );
			hbrush = ::CreateSolidBrush( RGB(0xe0,0xe8,0xe0) );
			hbrushw = ::CreateSolidBrush( RGB(255,255,255) );
			hpen = ::CreatePen(0, 1, ::GetSysColor(16));
			hpen = (HPEN)::SelectObject( hPaintDC,hpen);

			LOGFONT	logFont;
			logFont.lfHeight = - MulDiv(9, GetDeviceCaps(hPaintDC, LOGPIXELSY), 72);
			logFont.lfCharSet = DEFAULT_CHARSET ;
			logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logFont.lfEscapement = 0;
			_tcscpy_s(logFont.lfFaceName ,_tcslen( _T("MS Sans Serif")) + 1, _T("MS Sans Serif"));
			logFont.lfItalic = 0;
			logFont.lfOrientation = 0;
			logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logFont.lfPitchAndFamily = DEFAULT_PITCH;
			logFont.lfQuality = CLEARTYPE_QUALITY;
			logFont.lfStrikeOut = 0;
			logFont.lfUnderline = 0;
			logFont.lfWeight = FW_NORMAL;
			logFont.lfWidth = 0;


			HFONT	hFont =::CreateFontIndirect(&logFont);
			logFont.lfWeight = FW_BOLD;
			HFONT	hFontB =::CreateFontIndirect(&logFont);

			hFont = (HFONT)::SelectObject(hPaintDC,hFont);
			hpen1 = ::CreatePen(0, 1, RGB(0,0,0));

			hbrushw = (HBRUSH)SelectObject( hPaintDC,hbrushw);
			Rectangle( hPaintDC,il_startx,il_starty,il_startx + il_width ,il_starty + il_height);

			RECT	frect;
			::SetRect(&frect,il_startx + 1,il_starty + 1,il_startx + il_treebp ,il_starty + il_height);
			FillRect( hPaintDC,&frect,hbrush );

			long ll_y1,ll_y2;
			ll_y1 = il_startY;
			ll_y2 = ll_y1 +  il_height;
			if(((long)pElementsLists.size() - il_startrow + 1)* il_cellheight < il_height)
				ll_y2 =  ll_y1 + ((long)pElementsLists.size() - il_startrow + 1)* il_cellheight;
			::SetBkMode(hPaintDC,1);

			::MoveToEx(hPaintDC,il_startx + il_treebp + il_labelsw, ll_y1,NULL);
			::LineTo(hPaintDC,il_startx + il_treebp+  il_labelsw,ll_y2);

			long	ll_y;
			long	ll_x1,ll_x2;
			ll_y = il_startY;
			for( int ll_i = il_startrow - 1; ll_i< (int)pElementsLists.size();ll_i++)
			{
				ll_y1 = ll_y;
				ll_y2 = ll_y1 + il_cellheight - 1 ;
				ll_x1 = il_startx +  il_treebp;
				ll_x2 = il_startx + il_width - 1;

				_variant_t	val;
				LPWSTR ls_type = NULL;
				LPWSTR ls_category = NULL;
				LPWSTR ls_label = NULL;
				LPWSTR ls_expand = NULL;
				LPWSTR ls_colname = NULL;
				LPWSTR ls_display = NULL;
				BSTR	bstrDisplay = NULL;

				long	ll_level;

				pElementsLists[ll_i]->getAttribute(L"type",&val);
				ls_type = ::_wcsdup(val.bstrVal);
				pElementsLists[ll_i]->getAttribute(L"category",&val);
				ls_category = ::_wcsdup(val.bstrVal);
				pElementsLists[ll_i]->getAttribute(L"label",&val);
				ls_label = ::_wcsdup(val.bstrVal);
				pElementsLists[ll_i]->getAttribute(L"expand",&val);
				ls_expand = ::_wcsdup(val.bstrVal);
				pElementsLists[ll_i]->getAttribute(L"colname",&val);
				ls_colname = ::_wcsdup(val.bstrVal);
				pElementsLists[ll_i]->getAttribute(L"level",&val);
				ll_level = val;
				ls_display =  xbXml::GetStringAttribe(pElementsLists[ll_i],L"value");

				::MoveToEx(hPaintDC,ll_x1,ll_y2,NULL);
				::LineTo(hPaintDC,il_startx + il_width,ll_y2);

				if(wcscmp(ls_category ,L"item")!=0)
				{
					//draw flag bitmap
					hpen1 = (HPEN)SelectObject(hPaintDC, hpen1);
					Rectangle(hPaintDC,ll_x1 - 2 - il_treebox ,(ll_y1 + ll_y2 )/2   -   il_treebox/2 + 1  ,ll_x1 - 4  ,(ll_y1 + ll_y2 )/2 +  il_treebox/2);
					::MoveToEx(hPaintDC,ll_x1 - 2 - il_treebox + 2 ,(ll_y1 + ll_y2 )/2,NULL);
					::LineTo(hPaintDC,ll_x1 - 6  ,(ll_y1 + ll_y2 )/2);
					if(wcscmp(ls_expand,L"false")==0)
					{
						::MoveToEx(hPaintDC,ll_x1 - 4 - il_treebox /2 ,(ll_y1 + ll_y2 )/2   -   il_treebox/2 + 1+2,NULL);
						::LineTo(hPaintDC,ll_x1 - 4 - il_treebox /2 ,(ll_y1 + ll_y2 )/2 +  il_treebox/2 - 2);
					}
					hpen1 = (HPEN)SelectObject( hPaintDC,hpen1);
				}
				if(wcscmp(ls_category ,L"group")==0)
				{
					hFontB = (HFONT)::SelectObject(hPaintDC,hFontB);
					//draw group band
					::SetRect(&frect,ll_x1,ll_y1 + 1,ll_x2,ll_y2);
					FillRect( hPaintDC,&frect, hbrush );
					::SetRect(&frect,ll_x1,ll_y1,ll_x2,ll_y2);
					::DrawTextW( hPaintDC, ls_label,(int)wcslen(ls_label),&frect ,4 + 32);
					hFontB = (HFONT)::SelectObject(hPaintDC,hFontB);
				}else
				{
					//draw column data
					ll_x2 = ll_x1 + il_labelsw - 1;
					::SetRect(&frect,ll_x1 + 2 + (ll_level - 2) * 8  ,ll_y1,ll_x2,ll_y2);
					DrawText( hPaintDC, ls_label,(int)wcslen(ls_label),&frect ,4 + 32);
					ll_x1 = ll_x2 + 2;
					ll_x2 =  il_startx + il_width;
					if(::wcscmp(ls_type,L"column")==0)
					{
						
					}
					ll_x1 += 2;
					::SetRect(&frect,ll_x1,ll_y1,ll_x2,ll_y2);
					if(ls_display)::DrawTextW( hPaintDC, ls_display,(int)wcslen(ls_display),&frect ,4 + 32);
				}
				ll_y += il_cellheight;

				if(ls_type) delete ls_type;
				if(ls_category) delete  ls_category;
				if(ls_label) delete ls_label;
				if(ls_expand) delete  ls_expand;
				if(ls_colname) delete ls_colname;
				if(ls_display) delete ls_display;

				if(ll_y >= il_startY + il_height)break;
			}
			SetBkMode( hPaintDC, 0);

			hpen = (HPEN)SelectObject( hPaintDC,hpen);
			DeleteObject( hpen);
			DeleteObject( hbrush);
			hbrushw = (HBRUSH)SelectObject( hPaintDC,hbrushw);
			DeleteObject( hbrushw);
			DeleteObject( hpen1);
			hFont = (HFONT)::SelectObject(hPaintDC,hFont);
			::DeleteObject(hFont);
			::DeleteObject(hFontB);

			for(int i=0;i<(int)pElementsLists.size();i++)
			{
				pElementsLists[i]->Release();
			}

			pElementsLists.clear();
			return 0;
		}

		LPTSTR xcPropSheetCtrl::GetCellContent(xfControl* m_pOwner, int ACol,int ARow)
		{
			xcPropSheetCtrl * pThis =(xcPropSheetCtrl *) m_pOwner;
			IXMLDOMNodeList* 	 luo_nodes;
			vector<IXMLDOMElement *> luo_items;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;
			LPTSTR pStr = NULL;

			long ll_len;
			pThis->m_pXml->m_pXmlDoc->getElementsByTagName(L"row",&luo_nodes);
			luo_nodes->get_length(&ll_len);
			if(ll_len < 1)
			{
				pThis->m_pxfNode->m_pElement->getElementsByTagName(L"row", &luo_nodes);
				luo_nodes->get_length(&ll_len);
			}
			luo_nodes->get_length(&ll_len);
			if(ll_len < 1) return NULL;
			if(ll_len <=ARow) return NULL;

			luo_nodes->get_item(ARow -1, &pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				pStr = xbXml::GetStringAttribe(pElement,L"value");

				pElement->Release();
				pNode->Release();
			}
			luo_nodes->Release();
			return pStr;
		}
		int xcPropSheetCtrl::SetCellContent(xfControl* m_pOwner, int ACol,int ARow, LPTSTR pStr)
		{
			xcPropSheetCtrl * pThis =(xcPropSheetCtrl *) m_pOwner;
			IXMLDOMNodeList* 	 luo_nodes;
			vector<IXMLDOMElement *> luo_items;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElement;
			_variant_t var;

			long ll_len;
			pThis->m_pXml->m_pXmlDoc->getElementsByTagName(L"row",&luo_nodes);
			luo_nodes->get_length(&ll_len);
			if(ll_len < 1)
			{
				pThis->m_pxfNode->m_pElement->getElementsByTagName(L"row", &luo_nodes);
				luo_nodes->get_length(&ll_len);
			}
			luo_nodes->get_length(&ll_len);
			if(ll_len < 1) return NULL;
			if(ll_len <=ARow) return NULL;

			luo_nodes->get_item(ARow -1, &pNode);
			if(pNode)
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElement);
				if(pStr)
					pElement->setAttribute(L"value",(_variant_t)pStr);
				else
					pElement->setAttribute(L"value",(_variant_t)L"");
				pElement->Release();
				pNode->Release();
			}
			luo_nodes->Release();
			return 1;
		}
		int xcPropSheetCtrl::GetCellRect(xfControl* m_pOwner, int ACol,int ARow,RECT &rt)
		{
			rt = (( xcPropSheetCtrl*)m_pOwner)->m_editRt;
			return 1;
		}
		LPTSTR xcPropSheetCtrl::GetEditType(xfControl* m_pOwner, int ACol,int ARow)
		{
			return _tcsdup(L"ddlb");
			//return _tcsdup(L"edit");
		}
		HFONT xcPropSheetCtrl::GetEditFont(xfControl* m_pOwner,int ACol,int ARow)
		{
			return (( xcPropSheetCtrl*)m_pOwner)->m_hFont;
		}
	}
}	
