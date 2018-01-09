#ifndef WPP_STRING_POOL_H
#define WPP_STRING_POOL_H

#include "data_pool.hpp"

namespace wpp  {

typedef class string_pool : public data_pool_t
{
public:
	wchar_t* append_string(const wchar_t* str);
	wchar_t* append_string(const wchar_t* str,int size);

} string_pool_t;

}

#endif //WPP_STRING_POOL_H