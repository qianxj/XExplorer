#pragma once
#include "../vl.h"
#include "vldef.hpp"
#include "vlsym.hpp"

namespace vl
{
	class vArrayMgr;
	class vmenumer
	{
	public:
		virtual int next(int celt, void** ppElements,int pceltFetched) = 0;
		virtual int reset() = 0;
		virtual int skip(int celt) = 0;
		virtual int clone(void ** ppEnumer) = 0; 
	};

	//class vmenum
}