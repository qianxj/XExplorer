#pragma once

#include "svrbase.hpp"
#include "vxIOfficeStyleSvr.hxx"
#include "vxIContentSvr.hxx"

namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFOBJECT_API XOfficeDataSvr :
		public CSvrbase
	{
	public:
		XOfficeDataSvr(void);
		~XOfficeDataSvr(void);
	public:
		IOfficeStyleSvr * m_pStyleSvr;
		IContentSvr* m_pContentSvr;
	public:
		IContentSvr* vxGetIContentSvr(){return m_pContentSvr;}
		bool vxSetIContentSvr(IContentSvr* pContentSvr){m_pContentSvr = pContentSvr;return true;}
		
		IOfficeStyleSvr* vxGetIOfficeStyleSvr(){return m_pStyleSvr;}
		bool vxSetIOfficeStyleSvr(IOfficeStyleSvr* pStyleSvr){m_pStyleSvr = pStyleSvr;return true;}
	};
}}}
