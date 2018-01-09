/*
* Copyright 2010, Hxsoft Col.Ltd
* All rights reserved.
*
* Contributor(s):
*   Qian xian jie<qianxj@hxsoft.com>
*/

#pragma once

#include "../vl.h"

namespace vl
{
	class error
	{
	public:
		enum lang {cn,en};
#define VL_error(x,y,z,u,v) z,
		enum err
		{
			err_start = 0x2000,
#include "errorl.hpp"
			err_end
		};
		
	struct erritem
	{
		unsigned long	code;
		int				level;
		err				eitem;
		lang			elang;
		tchar*			text;
	};

	public:
		static erritem		erroritems[];

	public:
		//get error text by enum err
		static const tchar * geterrtext(err e);
	};

}