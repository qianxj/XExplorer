#ifndef WPP7_WPP7_H
#define WPP7_WPP7_H

#include "vlang7.h"
#include "vlwerr.hpp"
#include <map>

using namespace std;
namespace xlang
{

	class VLANG_API WPP7
	{
	public:
		WPP7();
		~WPP7();
	public:
		bool parse(/*wpp_context*/ void * context,wchar_t* pSrc,bool btrace=false);
	};
}

//

#endif //WPP7_WPP7_H