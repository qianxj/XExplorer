#ifndef WPP_DBCROWSET_H
#define WPP_DBCROWSET_H

#include "global.hpp"

class dbcRowSet
{
public:
	virtual wstring_t getString()  = 0;
	virtual string_t  getAString() = 0;

	virtual int		  getInteger() = 0;
	virtual double	  getDouble()  = 0;
};

#endif //WPP_DBCROWSET_H