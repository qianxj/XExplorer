#ifndef WPP_LOG_H
#define WPP_LOG_H

#include "logger.hpp"
#include <stdio.h>



class VLANG3_API Log
{
public:
	Log();
	~Log();
public:
	const char * get_defualtLogFile()const {return "wpp.log";}
	bool openStdout();
	bool openFile(const char* pFile);
	bool openFile();
	void closeFile();
public:
	int Log::Write(const char* msg, int length);
	int Log::Write(const wchar_t* msg, int length);

public:
	Logger * get_Logger(){return logger_;}
	void  set_Logger(Logger * logger){logger_ = logger;}
private:
	Logger * logger_;
	FILE* output_handle_;
	wchar_t buffer_[4096];
public:
	inline const wchar_t * get_buffer(){return buffer_;}
	inline const int get_bufferSize(){return sizeof(buffer_)/sizeof(buffer_[0]);}
public:
	static Log *  get_Log();
};



#endif WPP_LOG_H