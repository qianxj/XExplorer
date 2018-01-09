#pragma once
#include "vmlang.hpp"

namespace vl
{
	class VL_API stylang : public vmlang3
	{
	public:
		stylang();
		~stylang();
	public:
		int emitd(int pv,unsigned char code, const tchar * item = null,int typ=0,int spec=0);
		int emitd(int pv,unsigned char code, int tk = -1,int typ=0,int spec=0);
	};
}