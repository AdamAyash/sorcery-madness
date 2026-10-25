#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>

#define DEFAULT_LOGGER_PATTERN		"%^[%T] %n: %v%$"
#define APPLICATION_LOGGER_NAME		"Application"

namespace common
{
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Init()
	{
		s_ClientLogger = spdlog::stdout_color_mt(APPLICATION_LOGGER_NAME);
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}