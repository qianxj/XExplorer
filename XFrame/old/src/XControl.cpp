#include "stdafx.h"
#include "xwinbase.hpp"
#include "xcontrol.hpp"
#include "xnode.hpp"

namespace Hxsoft
{
	namespace XFrame
	{
		XControl::~XControl()
		{
			if(m_pNode)delete m_pNode;
			for(int i=0;i< (int) this->m_hImageLists.size();i++)
			{
				if(m_hImageLists[i])::ImageList_Destroy(m_hImageLists[i]);
			}
			m_hImageLists.clear();

			if(m_pstrXClass)delete m_pstrXClass;
			for(int i=0;i< (int)m_pChilds.size();i++)
			{
				if(m_pChilds[i])delete m_pChilds[i];
			}
			m_pChilds.clear();
		}
	}
}