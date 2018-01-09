#ifndef WPP_DBCDRIVE_H
#define WPP_DBCDRIVE_H

#include "vlang5.h"
#include "vlglobal.hpp"
using namespace xlang;

class dbcCollect;
class dbcSession;
class dbcConnect;
class dbcDrive
{
public:
	virtual dbcSession * getSession(const wstring_t connectstring) = 0;
	virtual const dbcCollect * getCollect(const wstring_t connectstring) = 0;
};

#endif //WPP_DBCDRIVE_H