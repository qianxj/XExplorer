#ifndef WPP_vlogmessage_t_H
#define WPP_vlogmessage_t_H

#include "vlang4.h"
#include "vlog.hpp"

class VLANG4_API logmessage_t
{
public:
	logmessage_t(log_t* log);
	~logmessage_t();
public:
	void appendString(const wstring_t str);
	void appendChar(wchar_t ch);
	void appendNumber(int_t number);
	void appendNumber(double_t number);
	void append(const wstring_t format,...);
public:
	void write();
public:
	inline const wstring_t get_buffer()const {return log_->get_buffer();}
	inline int get_bufferUsed()const {return used_;}
	inline int get_bufferSize()const {return log_->get_bufferSize();}
public:
	log_t * log_;
	int		used_;
};

#endif //WPP_vlogmessage_t_H