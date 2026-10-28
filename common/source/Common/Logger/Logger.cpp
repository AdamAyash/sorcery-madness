#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

#define DEFAULT_LOGGER_PATTERN	"%^[%T] %n: %v%$"

namespace common
{
	std::shared_ptr<spdlog::logger> Logger::s_oLogger;

	void Logger::Init(const std::string& strLoggerName)
	{
		s_oLogger = spdlog::stdout_color_mt(strLoggerName);
		s_oLogger->set_level(spdlog::level::trace);
	}
}