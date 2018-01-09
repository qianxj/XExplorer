#ifndef WPP_SQLSERVERDBCSTATEMENT_H
#define WPP_SQLSERVERDBCSTATEMENT_H

#include "dbcstatement.hpp"
#include "sqlserver-dbcsession.hpp"

class dbcStatementSqlServer : public dbcStatement
{
public:
	dbcStatementSqlServer(dbcSessionSqlServer *session,void * command);
	~dbcStatementSqlServer();
public:
	//override
	int execute(wstring_t sqlstr);
	//override
	void abort();
public:
	//override
	int  getUpdateCount();
private:
	void * command_;
	dbcSessionSqlServer * session_;
};

#endif //WPP_SQLSERVER-DBCSTATEMENT