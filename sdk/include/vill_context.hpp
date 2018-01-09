#ifndef WPP_VILL_CONTEXT_H
#define WPP_VILL_CONTEXT_H

#include "vill_runtime.hpp"

namespace mill{

struct VLANG3_API vill_context
{
	vill_runtime* ptr_runtime;
	vill_context();
};

} //namespace mill

#endif //WPP_VILL_CONTEXT