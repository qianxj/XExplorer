#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XCell
{
	class XCELL_API  IBIReport :
		public IXFControl
	{
	public:
		IBIReport(void);
		~IBIReport(void);
	public:
		int Load(IXMLDOMElement * pElement);
	public:
		int GetObjectsCount();
		xbObject * GetObjectByIndent(int nIndex);
	};
}}}}
