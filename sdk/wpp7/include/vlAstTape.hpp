#ifndef WPP7_ASTvlTapeChar_H
#define WPP7_ASTvlTapeChar_H

#include "vlAstNode.hpp"
#include <vector>

using namespace std;

namespace xlang{
	struct vlAstvlTapeChar : public vlAstNode
	{
		void * data;

	};
}

#endif //WPP7_ASTvlTapeChar_H
