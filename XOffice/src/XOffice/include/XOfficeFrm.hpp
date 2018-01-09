#pragma once

#include "xofbase.h"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class IXOffice;
	class XOfficeFrm :
		public XOFBaseFrm
	{
	public:
		XOfficeFrm(void);
		~XOfficeFrm(void);
	public:
		virtual	void Initial();
	public:
		xbObject * GetInterface(); 
	};
}}}
