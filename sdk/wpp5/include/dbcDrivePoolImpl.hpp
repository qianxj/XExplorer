#ifndef WPP_DBCDRIVEPOOLIMPL_H
#define WPP_DBCDRIVEPOOLIMPL_H
#include "dbcDrivePool.hpp"
#include <map>
#include <string>

using namespace std;

class dbcDrive;
class dbcDrivePoolImpl : public dbcDrivePool
{
public: 
	dbcDrivePoolImpl();
public:
	/*override*/
	bool registerLink(const wstring_t linkname,const wstring_t drivetype, const wstring_t connectstring,dbcDrive* drive = 0);
public:
	/*override*/
	const wstring_t getConnectString(const wstring_t linkname) const;
	/*override*/
	const wstring_t getDriveString(const wstring_t linkname) const;	
public:
	/*override*/
	const dbcDrive* getDrive(const wstring_t linkname);
	/*override*/
	const dbcCollect * getCollect(const wstring_t linkname);
	/*override*/
	dbcSession * getSession(const wstring_t linkname);

private:
	struct linkInfo
	{
		wstring drivetype;
		wstring connectstring;
		dbcDrive* drive;
		linkInfo():drivetype(0),connectstring(0),drive(0)
		{
		}
		linkInfo(wstring adrivetype,wstring aconnectstring,dbcDrive* adrive):
		  drivetype(adrivetype),connectstring(aconnectstring),drive(adrive){}
	};
	map<wstring/*name*/, linkInfo /*link*/> links_;
	map<wstring/*drivetype*/, dbcDrive* /*drive*/> drives_;
};

#endif //WPP_DBCDRIVEPOOLIMPL_H