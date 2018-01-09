#ifndef WPP_ERRORS_H
#define WPP_ERRORS_H

#include "vlang4.h"
#include "global.hpp"

namespace wpp {
namespace how {

class error
{
public:
	enum lang {cn,en};
	#define WPP_error(x,y,z,u,v) z,
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
		wchar_t*			text;
	};

public:
	static erritem		erroritems[];

public:
	//get error text by enum err
	VLANG4_API static const wstring_t geterrtext(err e);
};

}} // namespace wpp::how

#endif //WPP_ERRORS_H