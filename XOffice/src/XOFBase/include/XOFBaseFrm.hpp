#pragma once
#include "xframe.h"
#include "XOFBaseFrmsvr.hpp"
namespace Hxsoft{ namespace XFrame{ namespace XOffice
{
	class XOFFICE_API XOFBaseFrm:
		public xfControl
	{
	public:
		XOFBaseFrm();
	public:
		~XOFBaseFrm();
	public:
		XOFBaseFrmSvr * m_pFrameSvr;
	public:
		virtual int DoDraw(HDC hPaintDC,RECT * pDrawRect=NULL);
	};
}}}
