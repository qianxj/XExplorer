#ifndef H_SQLSERVERDBCOLLECT
#define H_SQLSERVERDBCOLLECT

#include "dbcCollect.hpp"

#include <map>
#include <string>
using namespace std;

using namespace xlang;

namespace ATL
{
	class CDataSource;
}

class dbcCollectSqlServer : public dbcCollect
{
public:
	typedef dbcCollectSqlServer* Ptr;
public:
	dbcCollectSqlServer();
	~dbcCollectSqlServer();
public:
	//override
	dbcSession * getSession();
public:
	int open();
	int close();
public:
	void setConnectString(const wstring_t connectString);
	const wstring_t getConnectString();
public:
	ATL::CDataSource * getDataSource();
private:
	wstring connectString_;
private:
	ATL::CDataSource * dataSource_;
};

#endif //SQLSERVERDBCOLLECT