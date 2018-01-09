#ifndef WPP_DBCCOLLECT_H
#define WPP_DBCCOLLECT_H

#include "vlglobal.hpp"

class dbcDrive;
class dbcSession;
class dbcCollect
{
public:
	virtual dbcSession * getSession() = 0;
};

#endif //WPP_DBCCOLLECT_H