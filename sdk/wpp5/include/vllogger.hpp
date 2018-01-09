#ifndef WPP5_LOGGER_H
#define WPP5_LOGGER_H

#include "vlang5.h"

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

	class VLANG_API vlLogger
	{
	public:
		static const wchar_t*  getLoggerItemName(LoggerItem whatItem);

	public:
	  vlLogger();
	  ~vlLogger();
	public:
		void log(LoggerItem whatItem, wchar_t * format,...);
	};

}

#endif //WPP5_LOGGER_H
