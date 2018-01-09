#ifndef WPP_LOG_H
#define WPP_LOG_H

#include "vlang4.h"
#include "global.hpp"

#include "vlogger.hpp"

class VLANG4_API log_t
{
public:
	log_t();
	~log_t();
public:
	const string_t get_defualtLogFile()const;
	bool_t openStdout();
	bool_t openFile(const string_t pFile);
	bool_t openFile();
	void_t closeFile();
public:
	int_t write(const string_t  msg, int length);
	int_t write(const wstring_t msg, int length);

public:
	logger_t * get_logger();
	void_t  set_logger(logger_t * logger);
private:
	logger_t * logger_;
	FILE* output_handle_;
	wchar_t buffer_[4096];
public:
	const wstring_t get_buffer();
	const int_t get_bufferSize();
public:
	static log_t *  get_Log();
};


#endif WPP_LOG_H