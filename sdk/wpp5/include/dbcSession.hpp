#ifndef WPP_DBCSESSION_H
#define WPP_DBCSESSION_H

#include "dbcconnect.hpp"
#include "dbctrans.hpp"

class dbcStatement;
class dbcSession : public dbcConnect , public dbcTrans
{
public:
	virtual dbcStatement * createStatement() = 0;
};

#endif //WPP_DBCSESSION_H