#ifndef WPP7_ASTTYPEDATA_H
#define WPP7_ASTTYPEDATA_H

#include "vlAstNode.hpp"

namespace xlang{

struct vlAstTypeData : public vlAstNode
{
	int	 typ;
	void * data;
	vlAstTypeData():vlAstNode(akTypeData),data(0),typ(0){};
	~vlAstTypeData()
	{
		if(data)delete data;
	}
};

}

#endif //WPP7_ASTTYPEDATA_H
