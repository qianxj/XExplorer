#include "StdAfx.h"
#include "..\include\XFlowItem.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	XFlowItem::XFlowItem():m_eRgn(NULL),m_nRowHeight(-1)
	{
	}

	XFlowItem::~XFlowItem()
	{
	}

	POINT XFlowItem::GetRowPosition(int nRow)
	{
		POINT pt={FContentRect.left,FContentRect.top};
		if(m_nRowHeight>0)
		{
			pt.y += (nRow - 1)*m_nRowHeight;
		}else
		{
			for(int i=0;i<nRow;i++)
			{
				pt.y += m_spRowHeights[i] + m_RowHeights[i];
			}
		}
		return pt;
	}
}}}}