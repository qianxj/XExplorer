#ifndef WPP_LOGGER_H
#define WPP_LOGGER_H

#include "vlang3.h"

/*����LoggerҪ��¼������
	1������ִ�еĲ���:
	2�������ϸ��:
	3����������:
	4���¼�����:
	5��������Ϣ
	6��������Ϣ
	7������״̬
	8��������Ϣ
	9��������Ϣ
*/
enum LoggerItem
{
	LOG_STEP = 1,
	LOG_COMPLIER =2,
	LOG_FUNCTIONCALL = 3,
	LOG_EVENT = 4,
	LOG_WARN = 5,
	LOG_ERROR = 6,
	LOG_RUNSTATE = 7,
	LOG_PROFILE = 8,
	LOG_CHECK = 9
};

class VLANG3_API Logger
{
public:
	static const wchar_t*  getLoggerItemName(LoggerItem whatItem)
	{
		static wchar_t* LoggerItemName[]={L"LOG_STEP",
										L"LOG_COMPLIER",
										L"LOG_FUNCTIONCALL",
										L"LOG_EVENT",
										L"LOG_WARN",
										L"LOG_ERROR",
										L"LOG_RUNSTATE",
										L"LOG_PROFILE",
										L"LOG_CHECK"};
		return LoggerItemName[whatItem];
	}

public:
  Logger();
  ~Logger();
public:
	void log(LoggerItem whatItem, wchar_t * format,...);
};

#endif //WPP_LOGGER_H
