#ifndef WPP_DBCTRANS_H
#define WPP_DBCTRANS_H

class dbcTrans
{
public:
	virtual void start() = 0;
	virtual void abort() = 0;
	virtual void commit()= 0;
};

#endif //WPP_DBCTRANS_H