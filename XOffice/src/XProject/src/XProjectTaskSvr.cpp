#include "StdAfx.h"
#include "..\include\XProjectTaskSvr.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	XProjectTaskSvr::XProjectTaskSvr(void):m_nIndexStyle(-1)
	{
	}

	XProjectTaskSvr::~XProjectTaskSvr(void)
	{
		for(int i=0;i<m_Tasks.size();i++)
		{
			if(m_Tasks[i]) delete m_Tasks[i]; 
		}
	}

	TaskItem * XProjectTaskSvr::GetTaskItemById(int id)
	{
		for(int i=0;i<m_Tasks.size();i++)
		{
			if(m_Tasks[i]->id == id) return m_Tasks[i];
		}
		return NULL;
	}
	int XProjectTaskSvr::adjustExtent(POINT ptOffset,int headerHeight, int height,int width, int nY)
	{
		RECT rect;
		int startX0;
		int startY0;
		int startY;
		
		struct tm  when;
		__time64_t now;
	   
		time( &now );
	   _localtime64_s( &when, &now );
   		COleDateTime curDate = now;

	    startX0 = ptOffset.x + width *  when.tm_wday;
		startY0 = ptOffset.y + headerHeight;// + 15;
		startY = startY0 + nY;
		for(int i=0;i<m_Tasks.size();i++)
		{
			/*rect.left = startX0 + width * (int)(m_Tasks[i]->sdate - curDate).GetTotalDays();
			rect.right   = startX0 + width * (int)(m_Tasks[i]->edate - curDate).GetTotalDays();
			rect.top = startY;
			rect.bottom = rect.top + height;
			startY += height + nY*2;
			m_Tasks[i]->rect = rect;*/
		}
		return 0;
	}
}}}}
