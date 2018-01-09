#pragma once

#include "xbxbase.h"
#include "xwordview.hpp"
#include "wpp4.h"
#include "xframe.h"

using namespace Hxsoft::XFrame;
using namespace Hxsoft::XFrame::XOffice;
using namespace Hxsoft::XFrame::XOffice::XWord;

namespace Hxsoft{ namespace XFrame{ namespace XOffice {namespace XWord
{
	class XWORD_API IXWord : public IXFControl
	{
	public:
		IXWord();
	public:
		vord_t GetVord();
		vord_t GetDataVord();
	public:
		void SetReadOnly(bool bReadOnly);
		bool GetReadOnly();
	public:
		XWordView * m_pWordSvr;
	};
}}}}
