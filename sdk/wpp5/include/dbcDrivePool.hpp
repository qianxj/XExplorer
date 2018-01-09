#ifndef WPP_DBCDRIVEPOOL_H
#define WPP_DBCDRIVEPOOL_H

#include "dbcDrive.hpp"

class dbcDrive;
class dbcDrivePool
{
public:
	virtual bool registerLink(const wstring_t linkname,const wstring_t drivetype, const wstring_t connectstring,dbcDrive* drive) =0;
public:
	virtual const wstring_t getConnectString(const wstring_t linkname) const =0;
	virtual const wstring_t getDriveString(const wstring_t linkname) const =0;	
public:
	virtual const dbcDrive* getDrive(const wstring_t linkname) =0;
	virtual const dbcCollect * getCollect(const wstring_t linkname) =0;
	virtual dbcSession * getSession(const wstring_t linkname) =0;
};

#endif //WPP_DBCDRIVEPOOL_H