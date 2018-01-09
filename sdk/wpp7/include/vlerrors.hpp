#ifndef WPP7_ERRORS_H
#define WPP7_ERRORS_H

#include "vlang7.h"
#include "vlglobal.hpp"

namespace xlang {

class error
{
public:
	enum lang {cn,en};
	#define WPP7_error(x,y,z,u,v) z,
	enum err
	{
		err_start = 0x2000,
		#include "vlerrorl.hpp"
		err_end
	};
	
	struct erritem
	{
		unsigned long	code;
		int				level;
		err				eitem;
		lang			elang;
		wchar_t*			text;
	};

public:
	static erritem		erroritems[];

public:
	//get error text by enum err
	VLANG_API static const wstring_t geterrtext(err e);
};

} // namespace xlang

#endif //WPP7_ERRORS_H