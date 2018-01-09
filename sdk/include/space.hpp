#ifndef SPACE_GC_H
#define SPACE_GC_H

#include "word.hpp"

using namespace std;
using namespace wpp::how;

struct xspace
{
	intptr_t alloc(int ty, size_t sz,int num = 1);
};

#endif //SPACE_GC_H