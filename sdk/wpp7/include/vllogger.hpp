#ifndef WPP7_LOGGER_H
#define WPP7_LOGGER_H

#include "vlang7.h"

namespace xlang
{

/*����vlLoggerҪ��¼������
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
enum LoggerKind
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

class VLANG_API vlLogger
{
public:
	static const wchar_t*  kindName(LoggerKind kind);

public:
  vlLogger();
  ~vlLogger();
public:
	void log(LoggerKind kind, wchar_t * format,...);
};

}//namespace vlang

#endif //WPP7_LOGGER_H
