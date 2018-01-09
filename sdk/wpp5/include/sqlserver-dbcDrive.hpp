#ifndef WPP_SQLSERVER_DBCDRIVE_H
#define WPP_SQLSERVER_DBCDRIVE_H

#include "vlang5.h"
#include "dbcdrive.hpp"
#include "vlglobal.hpp"
#include <map>
#include <string>

class dbcSession;
class dbcCollect;
class dbcDriveSqlServer : public dbcDrive
{
public:
	dbcDriveSqlServer();
public:
	~dbcDriveSqlServer();
public:
	//override
	dbcSession * getSession(const wstring_t connectstr);
	//override
	const dbcCollect * getCollect(const wstring_t connectstr);
};

#endif //WPP_SQLSERVER_DBCDRIVE_H