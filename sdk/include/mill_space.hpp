#ifndef WPP_MILL_SPACE_H
#define WPP_MILL_SPACE_H

#include "mill_gc.hpp"

namespace mill
{

class mill_space_base_
{

public:
	mill_gc_base_* ptr_mill_gc;
public:
	void *	mill_alloc(int size);
	void	mill_free(void * p);
	void *	mill_new(int size);
	void	mill_delete(void * p);
	void	mill_copy(int size);
	void	mill_move(void * p);
public:
	void *	mill_large_alloc(int size);
	void	mill_large_free(void * p);
};

} //namespace mill

#endif //WPP_MILL_SPACE_H