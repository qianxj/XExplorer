#include "stdafx.h"
#include "XFlowchartobject.hpp"
#include "xflowchartdatasvr.hpp"
#include "XFlowChartContentSvr.hpp"
#include <time.h>

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XFlowChart
{
	XFlowChartObject::XFlowChartObject():m_pCssStyle(NULL),m_pRefName(NULL),m_pRefUri(NULL),m_pDataSvr(NULL),m_FState(XFONORMAL),m_pStrCaption(NULL),m_nCssStyle(0),
		m_phtPoints(new vector<POINT>),m_pstPoints(new vector<POINT>),m_phtArrows(new vector<int>), m_pElement(NULL),m_pGuid(NULL),m_pDataElement(NULL),m_pStrID(NULL)
	{
		::SetRect(&FRect,0,0,0,0);
	}
	XFlowChartObject::~XFlowChartObject()
	{
		if(m_phtPoints)delete m_phtPoints;
		if(m_pstPoints)delete m_pstPoints;
		if(m_pCssStyle) delete m_pCssStyle;
		if(m_pRefName)  delete m_pRefName;
		if(m_pRefUri)delete m_pRefUri;
		if(m_pStrCaption) delete m_pStrCaption;
		if(m_phtArrows) delete m_phtArrows;
		if(m_pElement)m_pElement->Release();
		if(m_pDataElement)m_pDataElement->Release();
		if(m_pGuid)delete m_pGuid;
		if(m_pStrID)delete m_pStrID;
	}

	void XFlowChartObject::SetFRect(RECT rect)
	{
		FRect = rect;
		m_ptTopLeft.mask = TDrawPointMask_X|TDrawPointMask_Y;
		m_ptBottomRight.mask = TDrawPointMask_X|TDrawPointMask_Y;
		
		m_ptTopLeft.x.measureRef = XUtil::Css::_amount;
		m_ptTopLeft.x.amount = rect.left;
		m_ptTopLeft.x.unit=XUtil::Css::_px;
		m_ptTopLeft.y.measureRef = XUtil::Css::_amount;
		m_ptTopLeft.y.amount = rect.top;
		m_ptTopLeft.y.unit=XUtil::Css::_px;

		m_ptBottomRight.x.measureRef = XUtil::Css::_amount;
		m_ptBottomRight.x.amount = rect.right;
		m_ptBottomRight.x.unit=XUtil::Css::_px;
		m_ptBottomRight.y.measureRef = XUtil::Css::_amount;
		m_ptBottomRight.y.amount = rect.bottom;
		m_ptBottomRight.y.unit=XUtil::Css::_px;

		if(m_pRefName)
		{
			XShapeObject * pShapeObject = NULL;
			if(!m_pRefUri)pShapeObject=m_pDataSvr->m_pContentSvr->m_pShapeSetItem->Find(m_pRefName);
			if(!pShapeObject)
			{
				xfApp::GetApp()->m_pShapeMgr->CreateShape(m_pRefUri,m_pRefName,pShapeObject);
			}
			if(pShapeObject)
			{
				this->m_phtPoints->clear();
				pShapeObject->GetHotedPoints(rect,this->m_phtPoints);
				this->m_pstPoints->clear();
				pShapeObject->GetSelectPoints(rect,this->m_pstPoints);
				this->m_phtArrows->clear();
				pShapeObject->GetHotedArrows(m_phtArrows);
			}
		}
	}

	int XFlowChartObject::LoadObject(IXMLDOMElement * pElement)
	{
		LPTSTR pStr;
		this->m_pElement = pElement;
		pElement->AddRef();

		this->m_pStrID = xbXml::GetStringAttribe(pElement,_T("ID"));
		pStr = xbXml::GetStringAttribe(pElement,_T("startx"));
		if(pStr)
		{
			if(pStr[0]=='.' || pStr[0]=='@')
			{
				m_ptTopLeft.mask |= TDrawPointMask_STRX;
				m_ptTopLeft.pStrX = ::_tcsdup(_T("."));
			}else if(pStr[0]=='E')
			{
				m_ptTopLeft.mask |= TDrawPointMask_CX;
				XUtil::Css::ProcessCssMeasure(pStr + 1,&m_ptTopLeft.x);
			}else
			{
				m_ptTopLeft.mask |= TDrawPointMask_X;
				XUtil::Css::ProcessCssMeasure(pStr,&m_ptTopLeft.x);
			}
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("starty"));
		if(pStr)
		{
			if(pStr[0]=='.' || pStr[0]=='@')
			{
				m_ptTopLeft.mask |= TDrawPointMask_STRY;
				m_ptTopLeft.pStrY = ::_tcsdup(_T("."));
			}else if(pStr[0]=='E')
			{
				m_ptTopLeft.mask |= TDrawPointMask_CY;
				XUtil::Css::ProcessCssMeasure(pStr + 1,&m_ptTopLeft.y);
			}else
			{
				m_ptTopLeft.mask |= TDrawPointMask_Y;
				XUtil::Css::ProcessCssMeasure(pStr,&m_ptTopLeft.y);
			}
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("endx"));
		if(pStr)
		{
			if(pStr[0]=='.' || pStr[0]=='@')
			{
				m_ptBottomRight.mask |= TDrawPointMask_STRX;
				m_ptBottomRight.pStrX = ::_tcsdup(_T("."));
			}else if(pStr[0]=='E')
			{
				m_ptBottomRight.mask |= TDrawPointMask_CX;
				XUtil::Css::ProcessCssMeasure(pStr + 1,&m_ptBottomRight.x);
			}else
			{
				m_ptBottomRight.mask |= TDrawPointMask_X;
				XUtil::Css::ProcessCssMeasure(pStr,&m_ptBottomRight.x);
			}
			delete pStr;
		}
		pStr = xbXml::GetStringAttribe(pElement,_T("endy"));
		if(pStr)
		{
			if(pStr[0]=='.' || pStr[0]=='@')
			{
				m_ptBottomRight.mask |= TDrawPointMask_STRY;
				m_ptBottomRight.pStrY = ::_tcsdup(_T("."));
			}else if(pStr[0]=='E')
			{
				m_ptBottomRight.mask |= TDrawPointMask_CY;
				XUtil::Css::ProcessCssMeasure(pStr + 1,&m_ptBottomRight.y);
			}else
			{
				m_ptBottomRight.mask |= TDrawPointMask_Y;
				XUtil::Css::ProcessCssMeasure(pStr,&m_ptBottomRight.y);
			}
			delete pStr;
		}
		m_pRefName = xbXml::GetStringAttribe(pElement,_T("refname"));
		if(_tcschr(m_pRefName,':'))
		{
			m_pRefUri = new TCHAR[_tcschr(m_pRefName,':') - m_pRefName + 1];
			_tcsncpy_s(m_pRefUri,_tcschr(m_pRefName,':') - m_pRefName + 1,m_pRefName,_tcschr(m_pRefName,':') - m_pRefName);
			m_pRefUri[_tcschr(m_pRefName,':') - m_pRefName]='\0';
			LPTSTR pTemp = ::_tcsdup(_tcschr(m_pRefName,':') + 1);
			delete m_pRefName;
			m_pRefName = pTemp;
		}
		m_pStrCaption = xbXml::GetStringAttribe(pElement,_T("caption"));

		IXMLDOMNode * pNode;
		IXMLDOMNode * pNodeEx;
		pElement->selectSingleNode(L"data",&pNode);
		if(pNode)
		{
			pNode->cloneNode(VARIANT_TRUE,&pNodeEx);
			pNode->Release();
			pNodeEx->QueryInterface(IID_IXMLDOMElement,(void **)&m_pDataElement);
		}
		return 1;
	}
	int XFlowChartObject::LoadObjectXml(LPTSTR pStrXml)
	{
		xbXml	xml;
		xml.LoadXml(pStrXml);
		IXMLDOMElement *pElement;
		xml.m_pXmlDoc->get_documentElement(&pElement);
		int result = LoadObject(pElement);
		pElement->Release();
		return result;
	}

	int GetExtent(XUtil::Css::TMeasure &mes,int extent,bool bHorz)
	{
		if(mes.measureRef==XUtil::Css::_percentage)
			return (int)(extent*mes.percentage/100.0);
		else
			return  (int)XUtil::Css::GetPixelExtent(&mes,bHorz); 
		return 0;
	}

	XFOHitPosition XFlowChartObject::GetHitResizePat(POINT pt)
	{
		int swidth = (FRect.right - FRect.left + 1)/2;
		int sheight = (FRect.bottom - FRect.top + 1)/2;

		if(pt.x>=FRect.left - 2 && pt.x<= FRect.left + 2 && pt.y >=FRect.top - 2 && pt.y<= FRect.top + 2) return XFOHITLEFTTOP;
		if(pt.x>=FRect.right - 2 && pt.x<= FRect.right + 2 && pt.y >=FRect.top - 2 && pt.y<= FRect.top + 2) return XFOHITRIGHTTOP;
		if(pt.x>=FRect.right - 2 && pt.x<= FRect.right + 2 && pt.y >=FRect.bottom - 2 && pt.y<= FRect.bottom + 2) return XFOHITRIGHTBOTTOM;
		if(pt.x>=FRect.left - 2 && pt.x<= FRect.left + 2 && pt.y >=FRect.bottom - 2 && pt.y<= FRect.bottom + 2) return XFOHITLEFTBOTTOM;

		if(pt.x>=FRect.left - 2 + swidth && pt.x<= FRect.left + 2 +swidth && pt.y >=FRect.top - 2 && pt.y<= FRect.top + 2) return XFOHITTOPCENTER;
		if(pt.x>=FRect.right - 2 && pt.x<= FRect.right + 2 && pt.y >=FRect.top - 2 + sheight && pt.y<= FRect.top + 2 + sheight) return XFOHITRIGHTCENTER;
		if(pt.x>=FRect.left - 2 + swidth && pt.x<= FRect.left + 2 + swidth && pt.y >=FRect.bottom - 2 && pt.y<= FRect.bottom + 2) return XFOHITBOTTOMCENTER;
		if(pt.x>=FRect.left - 2 && pt.x<= FRect.left + 2 && pt.y >=FRect.top - 2 +sheight&& pt.y<= FRect.top + 2 +sheight) return XFOHITLEFTCENTER;

		return XFOHITNONE;
	}

	void XFlowChartObject::InvalidatePatRgn(HWND hWnd,POINT & ptOffset)
	{
		HRGN	hRgn = ::CreateRectRgn(0,0,0,0);
		POINT	pt;
		HRGN	hRgnEx;
		RECT	rect;

		for(int i=0;i<(int)m_pstPoints->size();i++)
		{
			pt = (*m_pstPoints)[i];
			SetRect(&rect,pt.x - 5 ,pt.y - 5, pt.x + 5, pt.y + 5);
			::OffsetRect(&rect,- ptOffset.x,- ptOffset.y);
			hRgnEx = CreateRectRgnIndirect(&rect);
			CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
			DeleteObject(hRgnEx);
		}
		/*
		HRGN	hRgnEx;
		int swidth = (rect.right - rect.left + 1)/2;
		int sheight = (rect.bottom - rect.top + 1)/2;

		::SetRect(&rect,FRect.left - 3 ,FRect.top - 3,
			FRect.left + 3, FRect.top + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.right - 3 ,FRect.top - 3,
			FRect.right + 3, FRect.top + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.right - 3 ,FRect.bottom - 3,
			FRect.right + 3, FRect.bottom + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.left - 3 ,FRect.bottom - 3,
			FRect.left + 3, FRect.bottom + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);


		::SetRect(&rect,FRect.left - 3 + swidth,FRect.top - 3,
			FRect.left + 3+swidth, FRect.top + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.right - 3 ,FRect.top - 3 + sheight,
			FRect.right + 3, FRect.top + 3+ sheight);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.left - 3 + swidth ,FRect.bottom - 3,
			FRect.left + 3 + swidth, FRect.bottom + 3);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);

		::SetRect(&rect,FRect.left - 3 ,FRect.top - 3 + sheight,
			FRect.left + 3, FRect.top + 3 +  sheight);
		hRgnEx = ::CreateRectRgnIndirect(&rect);
		CombineRgn(hRgn,hRgn,hRgnEx,RGN_OR);
		::DeleteObject(hRgnEx);
		*/

		::InvalidateRgn(hWnd,hRgn,false);
		::DeleteObject(hRgn);
	}

	void XFlowChartObject::HitAdjust(XFOHitPosition htpos,int cx,int cy)
	{
		if(htpos==XFOHITLEFTTOP)
		{
			this->FRect.left += cx;
			this->FRect.top += cy;
		}

		if(htpos==XFOHITRIGHTTOP)
		{
			this->FRect.right += cx;
			this->FRect.top += cy;
		}
		if(htpos==XFOHITRIGHTBOTTOM)
		{
			this->FRect.right += cx;
			this->FRect.bottom += cy;
		}
		if(htpos==XFOHITLEFTBOTTOM)
		{
			this->FRect.left += cx;
			this->FRect.bottom += cy;
		}

		if(htpos==XFOHITTOPCENTER)
		{
			this->FRect.top += cy;
		}
		if(htpos==XFOHITRIGHTCENTER)
		{
			this->FRect.right += cx;
		}
		if(htpos==XFOHITBOTTOMCENTER)
		{
			this->FRect.bottom += cy;
		}
		if(htpos==XFOHITLEFTCENTER)
		{
			this->FRect.left += cx;
		}
		this->SetFRect(FRect);
	}

	void XFlowChartObject::DoDraw(HDC hPaintDC,RECT rc)
	{
		RECT	rect;
		rect = rc;

		POINT ptCur;
		srand( (unsigned)time( NULL ) );
		GetCurrentPositionEx(hPaintDC,&ptCur);
		if(m_ptTopLeft.mask & TDrawPointMask_STRX && ::_tcsicmp(m_ptTopLeft.pStrX,_T("."))==0)
			rect.left = ptCur.x;
		else if(m_ptTopLeft.mask & TDrawPointMask_CX )
			rect.left = ptCur.x +  GetExtent(m_ptTopLeft.x,rc.right - rc.left + 1,true);
		else
		{
			rect.left = rc.left + GetExtent(m_ptTopLeft.x,rc.right - rc.left + 1,true);
			//rect.left = GetExtent(m_ptTopLeft.x,rc.right - rc.left + 1,true);
		}

		if(m_ptTopLeft.mask & TDrawPointMask_STRY && ::_tcsicmp(m_ptTopLeft.pStrY,_T("."))==0)
			rect.top = ptCur.y;
		else if(m_ptTopLeft.mask & TDrawPointMask_CY )
			rect.top = ptCur.y +  GetExtent(m_ptTopLeft.y,rc.bottom - rc.top + 1,false);
		else
		{
			rect.top = rc.top + GetExtent(m_ptTopLeft.y,rc.bottom - rc.top + 1,false);
			//rect.top = GetExtent(m_ptTopLeft.y,rc.bottom - rc.top + 1,false);
		}

		if(m_ptBottomRight.mask & TDrawPointMask_STRX && ::_tcsicmp(m_ptBottomRight.pStrX,_T("."))==0)
			rect.right = rect.left;
		else if(m_ptBottomRight.mask & TDrawPointMask_CX )
			rect.right = rect.left +  GetExtent(m_ptBottomRight.x,rc.right - rc.left + 1,true);
		else
		{
			rect.right = rc.left + GetExtent(m_ptBottomRight.x,rc.right - rc.left + 1,true);
			//rect.right =  GetExtent(m_ptBottomRight.x,rc.right - rc.left + 1,true);
		}

		if(m_ptBottomRight.mask & TDrawPointMask_STRY && ::_tcsicmp(m_ptBottomRight.pStrY,_T("."))==0)
			rect.bottom = rect.top;
		else if(m_ptBottomRight.mask & TDrawPointMask_CY )
			rect.bottom = rect.top +  GetExtent(m_ptBottomRight.y,rc.bottom - rc.top + 1,false);
		else
		{
			rect.bottom = rc.top + GetExtent(m_ptBottomRight.y,rc.bottom - rc.top + 1,false);
			//rect.bottom = GetExtent(m_ptBottomRight.y,rc.bottom - rc.top + 1,false);
		}

		if(!EqualRect(&FRect,&rect))
		{
			FRect = rect;
			if(m_pRefName)
			{
				XShapeObject * pShapeObject = NULL;
				if(!m_pRefUri)pShapeObject=m_pDataSvr->m_pContentSvr->m_pShapeSetItem->Find(m_pRefName);
				if(!pShapeObject)
				{
					xfApp::GetApp()->m_pShapeMgr->CreateShape(m_pRefUri,m_pRefName,pShapeObject);
				}
				if(pShapeObject)
				{
					this->m_phtPoints->clear();
					pShapeObject->GetHotedPoints(rect,this->m_phtPoints);
					this->m_pstPoints->clear();
					pShapeObject->GetSelectPoints(rect,this->m_pstPoints);
					this->m_phtArrows->clear();
					pShapeObject->GetHotedArrows(m_phtArrows);
				}
			}
		}
		if(m_pRefName)
		{
			XShapeObject * pShapeObject = NULL;
			if(!m_pRefUri)pShapeObject=m_pDataSvr->m_pContentSvr->m_pShapeSetItem->Find(m_pRefName);
			if(!pShapeObject)
			{
				xfApp::GetApp()->m_pShapeMgr->CreateShape(m_pRefUri,m_pRefName,pShapeObject);
				if(pShapeObject)
				{
					this->m_phtPoints->clear();
					pShapeObject->GetHotedPoints(rect,this->m_phtPoints);
					this->m_pstPoints->clear();
					pShapeObject->GetSelectPoints(rect,this->m_pstPoints);
					this->m_phtArrows->clear();
					pShapeObject->GetHotedArrows(m_phtArrows);
				}
			}
			if(pShapeObject)
			{
				int nTextHeight = 0;
				if(_tcsstr(m_pRefName,L"role")||_tcsstr(m_pRefName,L"roles"))
				{
					if(m_pStrCaption) nTextHeight = 24;
				}
				rect.bottom -= nTextHeight;
				if(this->m_FState & XFOSELECTED)
					pShapeObject->DoDraw(hPaintDC,rect,DMASKHOT| DMASKSELECTED);
				else
					pShapeObject->DoDraw(hPaintDC,rect,DMASKHOT);
				rect.bottom += nTextHeight;

				/*if(this->m_FState & XFOSELECTED)
				{

					int swidth = (rect.right - rect.left + 1)/2;
					int sheight = (rect.bottom - rect.top + 1)/2;

					::Rectangle(hPaintDC,rect.left - 3,rect.top - 3,rect.left + 3,rect.top + 3);
					::Rectangle(hPaintDC,rect.right - 3,rect.top - 3,rect.right + 3,rect.top + 3);
					::Rectangle(hPaintDC,rect.right - 3,rect.bottom - 3,rect.right + 3,rect.bottom + 3);
					::Rectangle(hPaintDC,rect.left - 3,rect.bottom - 3,rect.left + 3,rect.bottom + 3);

					::Rectangle(hPaintDC,rect.left - 3 + swidth,rect.top - 3,rect.left + 3 + swidth,rect.top + 3);
					::Rectangle(hPaintDC,rect.right - 3,rect.top - 3 + sheight,rect.right + 3,rect.top + 3 + sheight);
					::Rectangle(hPaintDC,rect.left - 3 + swidth,rect.bottom - 3,rect.left + 3 + swidth,rect.bottom + 3);
					::Rectangle(hPaintDC,rect.left -3 ,rect.top - 3 +sheight,rect.left +3 ,rect.top + 3 +sheight);
				}*/
				if(this->m_pStrCaption)
				{
					LOGFONT	logFont;
					XUtil::Css::GetFont(NULL,logFont);

					_variant_t var;
					bool bColor = false;
					COLORREF color;

					RECT rt = rect;
					if(nTextHeight > 0)
						rt.top = rect.bottom  - nTextHeight;

					if(m_pElement)
					{
						m_pElement->getAttribute(L"state",&var);
						if(var.bstrVal && _tcsstr(var.bstrVal,L"disabled"))
						{
							bColor = true;
							color = ::SetTextColor(hPaintDC,RGB(64,128,128));
						}else if(var.bstrVal && _tcsstr(var.bstrVal,L"hoted"))
						{
							bColor = true;
							color = ::SetTextColor(hPaintDC,RGB(255,0,0));
							logFont.lfWeight = 700;
						}else if(var.bstrVal && _tcsstr(var.bstrVal,L"current"))
						{
							bColor = true;
							color = ::SetTextColor(hPaintDC,::GetSysColor(COLOR_HIGHLIGHTTEXT));
						}
					}
					HFONT hFont = ::CreateFontIndirect(&logFont);
					hFont = (HFONT)::SelectObject(hPaintDC,hFont);

					int hbr = SetBkMode(hPaintDC,TRANSPARENT); 
					::InflateRect(&rt,-2,-2);
					if(_tcschr(m_pStrCaption,'\r'))
						::DrawTextW(hPaintDC,m_pStrCaption,(int)::_tcslen(m_pStrCaption),&rt,DT_CENTER|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK);
					else
					{
						SIZE sz;
						GetTextExtentPoint32(hPaintDC,m_pStrCaption,(int)::_tcslen(m_pStrCaption),&sz);
						if(sz.cx < rt.right - rt.left)
							::DrawTextW(hPaintDC,m_pStrCaption,(int)::_tcslen(m_pStrCaption),&rt,DT_SINGLELINE |DT_CENTER|DT_VCENTER|DT_EDITCONTROL);
						else
							::DrawTextW(hPaintDC,m_pStrCaption,(int)::_tcslen(m_pStrCaption),&rt,DT_CENTER|DT_VCENTER|DT_EDITCONTROL | DT_WORDBREAK);

					}
					::SetBkMode(hPaintDC,hbr);
					hFont = (HFONT)::SelectObject(hPaintDC,hFont);
					::DeleteObject(hFont);
					if(bColor) color = ::SetTextColor(hPaintDC,color);
				}

				if(_tcsicmp(m_pRefUri,L"workflow")==0 &&(_tcsicmp(m_pRefName,L"worknode")==0 || _tcsicmp(m_pRefName,L"processnode")==0))
				{
					double rand100 = 0;
					_variant_t var;
					if(m_pElement)
					{
						m_pElement->getAttribute(L"step",&var);
						if(var.vt!=VT_NULL && var.vt!=VT_EMPTY)
						{
							if(_tcschr(var.bstrVal,'%'))
							{
								_tcschr(var.bstrVal,'%')[0] = 0;
								rand100 = var;
							}
							else
							{
								rand100 = _ttoi(var.bstrVal);
								rand100 *= 100;
							}
						}
					}

					HBRUSH hb ;
					if(rand100<98)
					{
						hb = ::CreateSolidBrush(RGB(255,255,0));
						hb = (HBRUSH)::SelectObject(hPaintDC,hb);
						::Rectangle(hPaintDC,rect.left,rect.bottom - 5 + 3,rect.right,rect.bottom + 3);

						hb = (HBRUSH)::SelectObject(hPaintDC,hb);
						::DeleteObject(hb);
					}

					if(rand100 > 5)
					{
						RECT rt;
						hb = ::CreateSolidBrush(RGB(128,164,255));
						::SetRect(&rt,rect.left + 1,rect.bottom - 5 + 4, (int)(rect.left + (rect.right - rect.left)/100.0*rand100),rect.bottom + 2);
						::FillRect(hPaintDC,&rt,hb);

						::DeleteObject(hb);
					}
					
				}
			}
		}
	}

	int XFlowChartObject::TestHotHited(POINT pt,RECT &rc)
	{
		for(int i=0;i<(int)m_phtPoints->size();i++) 
		{
			if(pt.x >= (*m_phtPoints)[i].x - 3 && pt.x<= (*m_phtPoints)[i].x + 3 &&
				pt.y >= (*m_phtPoints)[i].y - 3 && pt.y<= (*m_phtPoints)[i].y + 3)
			{
				::SetRect(&rc,(*m_phtPoints)[i].x - 3 ,(*m_phtPoints)[i].y - 3, (*m_phtPoints)[i].x + 3 ,(*m_phtPoints)[i].y + 3);
				return i;
			}
		}
		return -1;
	}
	
	int XFlowChartObject::DrawHoted(HDC hPaintDC,int nIndex)
	{
		HPEN hPen = ::CreatePen(PS_DASH,1,RGB(0,0,0));
		HBRUSH hbrush = ::CreateSolidBrush(RGB(255,0,0));
		hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);

		POINT pt = (*m_phtPoints)[nIndex];
		::Rectangle(hPaintDC,pt.x - 3,pt.y - 3,pt.x + 4,pt.y + 4);

		hPen = (HPEN)SelectObject(hPaintDC,hPen);
		::MoveToEx(hPaintDC,pt.x - 2,pt.y - 2,NULL);
		::LineTo(hPaintDC, pt.x + 3,pt.y + 3);
		::MoveToEx(hPaintDC,pt.x + 2,pt.y - 2,NULL);
		::LineTo(hPaintDC, pt.x - 3,pt.y + 3);

		hPen = (HPEN)SelectObject(hPaintDC,hPen);
		::DeleteObject(hPen);
		hbrush = (HBRUSH)::SelectObject(hPaintDC,hbrush);
		::DeleteObject(hbrush);
		return 1;
	}

	LPTSTR XFlowChartObject::GetCaption()
	{
		return this->m_pStrCaption;
	}
	IXMLDOMElement* XFlowChartObject::GetElement()
	{
		return this->m_pElement;
	}
	LPTSTR XFlowChartObject::GetShapeType()
	{
		return this->m_pRefName;
	}
	RECT & XFlowChartObject::GetRect()
	{
		return FRect;
	}

	bool XFlowChartObject::SetUrl(LPTSTR pUrl)
	{
			IXMLDOMElement * pElement =m_pDataElement;
			IXMLDOMNode * pNode;
			IXMLDOMElement * pElementEx;
			xbXml xml;
			if(!pElement)
			{
				xml.m_pXmlDoc->createElement(L"data",&pElement);
				m_pDataElement = pElement;
			}
			pElement->selectSingleNode(L"devlink",&pNode);
			if(!pNode)
			{
				xml.m_pXmlDoc->createElement(L"devlink",&pElementEx);
				pElement->appendChild(pElementEx,NULL);
			}else
			{
				pNode->QueryInterface(IID_IXMLDOMElement,(void **)&pElementEx);
				pNode->Release();
			}
			pElementEx->setAttribute(L"url",(_variant_t)pUrl);
			pElementEx->Release();
			return true;
	}
	
	bool XFlowChartObject::SetCaption(LPTSTR pCaption)
	{
		if(m_pStrCaption)delete m_pStrCaption;
		m_pStrCaption = pCaption;
		return true;
	}
	
	LPTSTR XFlowChartObject::GetGuid()
	{
		return m_pGuid;
	}

	LPTSTR XFlowChartObject::GetUrl()
	{
		if(m_pDataElement)
		{
			IXMLDOMNode * pNode;
			m_pDataElement->selectSingleNode(L"devlink/@url",&pNode);
			if(pNode)
			{
				BSTR bstr;
				pNode->get_text(&bstr);
				pNode->Release();
				if(bstr)
				{
					LPTSTR pStr = _tcsdup(bstr);
					::SysFreeString(bstr);
					return pStr;
				}
			}
		}
		return NULL;
	}

}}}}
