#include "StdAfx.h"
#include "..\include\XProjectSheetSvr.hpp"
#include "time.h"
#include "XProjectSerializeSvr.hpp"
#include "xframe.h"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	XProjectSheetSvr::XProjectSheetSvr(void)
	{
		m_pSerializeSvr = new XProjectSerializeSvr;
		m_pDataSvr = new XProjectDataSvr;

	}

	XProjectSheetSvr::~XProjectSheetSvr(void)
	{
		if(m_pSerializeSvr) delete m_pSerializeSvr;
		if(m_pDataSvr) delete m_pDataSvr ;
	}

	int XProjectSheetSvr::DoDraw(HDC hPaintDC,RECT * pDrawRect)
	{
		XProjectDataSvr* pDataSvr = dynamic_cast<XProjectDataSvr *>(m_pDataSvr);
		int nwidth = 100;

		RECT	rect;
		if(pDrawRect)
			rect = *pDrawRect;
		else
			rect = this->FRect;

		RECT rt = rect;
		HBRUSH hBrushTaskEx = ::CreateSolidBrush(RGB(0xec,0xec,0xec));
		rt.right = rt.left + nwidth;
		FillRect(hPaintDC,&rt,hBrushTaskEx);

		rect.left += nwidth;
	   


		LOGFONT	logFont;
		logFont.lfHeight = - MulDiv(9,GetDeviceCaps(hPaintDC, LOGPIXELSY), 72);
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

		HFONT hFont = ::CreateFontIndirect(&logFont);
		hFont = (HFONT)::SelectObject(hPaintDC,hFont);

		struct tm  when;
		__time64_t now;
		TCHAR buf[80];

	   time( &now );
	   _localtime64_s( &when, &now );
	   when.tm_mday -= when.tm_wday;
	   mktime( &when );

	   RECT	rectex;
	   rectex = rect;
	   rectex.bottom = rectex.top+35;
	   HBRUSH hBrush = ::GetSysColorBrush(COLOR_3DFACE);
	   ::FillRect(hPaintDC,&rectex,hBrush);
	   ::DeleteObject(hBrush); 

	   HBRUSH hBrushEx = ::CreateSolidBrush(RGB(247,247,247));

	   HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(168,168,168));
	   hPen = (HPEN)::SelectObject(hPaintDC,hPen);
	   int bkMode = ::SetBkMode(hPaintDC,TRANSPARENT);
	   int x = rect.left;
	   LPTSTR pStrs[]={_T("日"),_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六")};
	   while(true)
	   {
		   if(x > rect.right) break;
		   ::_stprintf_s(buf,80,_T("%i年%i月%i日"),when.tm_year+1900,when.tm_mon+1,when.tm_mday + 1);
		   
		   RECT trect;
		   trect.left = x + 5;
		   trect.top = rect.top;
		   trect.right = trect.left + 7*18;
		   trect.bottom = trect.top + 18;
		   if(trect.right > rect.right )trect.right = rect.right;
		   if(trect.bottom > rect.bottom )trect.bottom = rect.bottom;
		   ::DrawText(hPaintDC,buf,(int)::_tcslen(buf),&trect,DT_SINGLELINE|DT_VCENTER|DT_WORD_ELLIPSIS);
		   ::MoveToEx(hPaintDC,x+7*18 > rect.right?rect.right:x+7*18,rect.top,NULL);
		   ::LineTo(hPaintDC,x+7*18 > rect.right?rect.right:x+7*18,18+rect.top);
		   ::LineTo(hPaintDC,x,18+rect.top);
		   
		   ::MoveToEx(hPaintDC,x+7*18 > rect.right?rect.right:x+7*18,rect.top + 35,NULL);
		   ::LineTo(hPaintDC,x,rect.top + 35);


		   rectex.left = x;
		   rectex.top = rect.top + 36;
		   rectex.right = rectex.left + 18;
		   rectex.bottom = rect.bottom - 2;
		   ::FillRect(hPaintDC,&rectex,hBrushEx);


		   int i;
		   for(i=0;i<7;i++)
		   {
			   if(x > rect.right) break;
			   ::TextOut(hPaintDC,x+2,18+4+rect.top,pStrs[i],(int)::_tcslen(pStrs[i]));
			   x+= 18;
			   if(x < rect.right)
			   {
				   ::MoveToEx(hPaintDC,x,18+rect.top,NULL);
				   ::LineTo(hPaintDC,x,35+rect.top);
			   }
		   }
		   when.tm_mday += 7;
		   mktime( &when );
	   }
	   ::DeleteObject(hBrushEx);

	 
	   int toX;
	   RECT rc;
	   HBRUSH hBrushTask = ::CreateSolidBrush(RGB(0xec,0xce,0xcc));
	   if(pDataSvr->m_pTaskSvr)
	   {
		   POINT	pt;
		   pt.x = rect.left;
		   pt.y = rect.top;
		   pDataSvr->m_pTaskSvr->adjustExtent(pt,35,18,18,4);
		   HPEN hPenEx = ::CreatePen(PS_SOLID,1,RGB(0x80,0x80,0xff));
		   hPenEx = (HPEN)::SelectObject(hPaintDC,hPenEx);
		   for(int i=0;i<pDataSvr->m_pTaskSvr->m_Tasks.size();i++)
		   {
			   ::FillRect(hPaintDC,&pDataSvr->m_pTaskSvr->m_Tasks[i]->rect,hBrushTask);
			   
			   rc = pDataSvr->m_pTaskSvr->m_Tasks[i]->rect;
			   rc.right = rect.left;
			   rc.left = rect.left - nwidth;
			   ::FillRect(hPaintDC,&rc,hBrushTaskEx);
			   if(pDataSvr->m_pTaskSvr->m_Tasks[i]->m_pStr)
			   {

				   ::DrawText(hPaintDC,pDataSvr->m_pTaskSvr->m_Tasks[i]->m_pStr,_tcslen(pDataSvr->m_pTaskSvr->m_Tasks[i]->m_pStr),&rc,DT_LEFT|DT_VCENTER);
			   }
				  
		   }
		   for(int i=0;i<pDataSvr->m_pTaskSvr->m_Tasks.size();i++)
		   {
			   for(int k=0;k<pDataSvr->m_pTaskSvr->m_Tasks[i]->m_nextTasks.size();k++)
			   {
				   ::MoveToEx(hPaintDC,pDataSvr->m_pTaskSvr->m_Tasks[i]->rect.right,
						pDataSvr->m_pTaskSvr->m_Tasks[i]->rect.top + 8,NULL);
				   int id = pDataSvr->m_pTaskSvr->m_Tasks[i]->m_nextTasks[k];
					rectex = pDataSvr->m_pTaskSvr->GetTaskItemById(id)->rect;
					toX = rectex.left;
					if(rectex.left == pDataSvr->m_pTaskSvr->m_Tasks[i]->rect.right)toX+= 5;
					::LineTo(hPaintDC,toX,pDataSvr->m_pTaskSvr->m_Tasks[i]->rect.top + 8);
					::LineTo(hPaintDC,toX,rectex.top);
					::LineTo(hPaintDC,toX - 5,rectex.top - 5);
					::MoveToEx(hPaintDC,toX,rectex.top,NULL);
					::LineTo(hPaintDC,toX + 5,rectex.top - 5);
			   }
		   }
		   hPenEx = (HPEN)::SelectObject(hPaintDC,hPenEx);
		   ::DeleteObject(hPenEx);
	   }

	   ::DeleteObject(hBrushTask);
	   ::DeleteObject(hBrushTaskEx);
	   hPen = (HPEN)::SelectObject(hPaintDC,hPen);
	   ::DeleteObject(hPen);
	   	hFont = (HFONT)::SelectObject(hPaintDC,hFont);
		::DeleteObject(hFont);
	   ::SetBkMode(hPaintDC,bkMode);

		return 1;
	}
}}}}
