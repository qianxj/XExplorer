#ifndef WPP7_LOGGER_H
#define WPP7_LOGGER_H

#include "vlang7.h"

namespace xlang
{

/*定义vlLogger要记录的事项
	1、正在执行的步骤:
	2、编译的细节:
	3、函数调用:
	4、事件处理:
	5、错误信息
	6、警告信息
	7、运行状态
	8、性能信息
	9、检查的信息
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
