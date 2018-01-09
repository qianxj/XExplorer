#pragma once

#include "xofbase.h"
#include "xuxutil.h"
#include <atldef.h>
#include <atlbase.h>
#include <ATLComTime.h>

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XProject
{
	struct TaskItem
	{
		int	id;
		LPTSTR m_pStr;
		COleDateTime sdate;
		COleDateTime edate;
		LPVOID m_pSource;
		template class XUtil::xuVector<TaskItem *>;
		XUtil::xuVector<int> m_nextTasks;
		int m_nIndexStyle;
		RECT rect;
		TaskItem():id(-1),m_nIndexStyle(-1),m_pStr(NULL),m_pSource(NULL)
		{
		}
		~TaskItem()
		{
			if(m_pStr)delete m_pStr;
		}
	};

	class XProjectTaskSvr :
		public CSvrbase
	{
	public:
		XProjectTaskSvr(void);
		~XProjectTaskSvr(void);
	public:
		int m_nIndexStyle;
	public:
		int adjustExtent(POINT ptOffset,int headerHeight,int height,int width, int nY);
		TaskItem * GetTaskItemById(int id);
	public:
		XUtil::xuVector<TaskItem *> m_Tasks;
	};
}}}}
