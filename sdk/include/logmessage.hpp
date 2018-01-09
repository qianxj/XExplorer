#ifndef WPP_LOGMESSAGE_H
#define WPP_LOGMESSAGE_H

#include "vlang3.h"
#include "log.hpp"

class VLANG3_API LogMessage
{
public:
	LogMessage(Log* log);
	~LogMessage();
public:
	void appendString(const wchar_t* str);
	void appendChar(wchar_t ch);
	void appendNumber(int number);
	void appendNumber(double number);
	void append(const wchar_t* format,...);
public:
	void write();
public:
	inline const wchar_t* get_buffer()const {return log_->get_buffer();}
	inline int get_bufferUsed()const {return used_;}
	inline int get_bufferSize()const {return log_->get_bufferSize();}
public:
	Log * log_;
	int	  used_;
};

#endif //WPP_LOGMESSAGE_H