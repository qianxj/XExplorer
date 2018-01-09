#pragma once

#include "../vl.h"
#include "windows.h"

namespace vl
{
	class VL_API  vlock
	{
	private:
		CRITICAL_SECTION m_crit;
	public:
		vlock();
		~vlock();
	public:
		void Enter();
		void Leave();
	};
}