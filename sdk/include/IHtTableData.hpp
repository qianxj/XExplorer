#pragma once
#include "xbxbase.h"
#include "Ihttableaxis.hpp"
#include "Ihttablecontent.hpp"
#include "Ihttablestyle.hpp"
#include "Ihttablespan.hpp"

namespace Hxsoft{ namespace XFrame {namespace XControl
{
	class IHtTableData : public xbObject
	{
	public:
		virtual IHtTableAxis *		GetAxis() = 0;
		virtual IHtTableContent *	GetContent() = 0;
		virtual IHtTableStyle *		GetStyle() =0;
		virtual IHtTableSpan *		GetSpan() =0;

		virtual bool LoadTable(IXMLDOMElement * pElement) =0;
		virtual bool LoadTable(LPTSTR pStr) =0;
	};

}}}