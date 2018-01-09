#pragma once

#include "xofbase.h"
#include "ixcell.hpp"

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API XCellSheet :
		public XOfficeSheet
	{
	public:
		XCellSheet(void);
		~XCellSheet(void);
	public:
		virtual int FocusedSheet();
	public:
		void * GetDwInterFace(); 
		xbObject * GetInterface(); 
	public:
		IXCell * m_pIXCell;
		xbObject * QueryItem(LPTSTR pItem);
	public:
		SIZE GetContentExtent();
	};
}}}}
