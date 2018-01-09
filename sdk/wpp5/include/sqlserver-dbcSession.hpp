#ifndef WPP_SQLSERVERDBSESSION_H
#define WPP_SQLSERVERDBSESSION_H

#include "dbcsession.hpp"
#include "sqlserver-dbccollect.hpp"

namespace ATL
{
	class CSession;
}

class dbcSessionSqlServer : public dbcSession
{
public:
	dbcSessionSqlServer(ATL::CSession * session);
	~dbcSessionSqlServer();
public:
	//override
	void start();
	//override
	void abort();
	//override
	void commit();
public:
	//override
	dbcStatement * createStatement();
public:
	ATL::CSession * getSession();
private:
	ATL::CSession * session_;
};

#endif //WPP_SQLSERVERDBSESSION_H