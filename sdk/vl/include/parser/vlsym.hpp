#pragma once

#include "../lexer/lexer.hpp"
#include "vldef.hpp"
#include "vlutils.hpp"
#include "vlerr.hpp"

namespace vl
{
	//object table
	struct objTable
	{
		int tokenId;	//token id
		int	unitId;		//unit id
		int setId;		//set id
		int parentId;	//parent id
		int refId;		//ref id
	};

	#define OffsetField(x,y) offsetof(x,y)
	#define OffsetInt(x,y) (offsetof(x,y)/vIntSize)

	typedef  bool (*fnFetchData)(const tchar *,void* param); 

}