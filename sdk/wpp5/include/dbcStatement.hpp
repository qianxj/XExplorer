#ifndef WPP_DBCSTATEMENT_H
#define WPP_DBCSTATEMENT_H

#include "vlglobal.hpp"

using namespace xlang;
class dbcStatement
{
public:
	virtual int execute(wstring_t sqlstr)=0;
	virtual void abort()=0;
public:
	virtual int  getUpdateCount()=0;
public:

};

#endif //WPP_DBCTRANS_H