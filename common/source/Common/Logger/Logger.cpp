#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <vector>

#define DEFAULT_LOGGER_PATTERN	"%^[%T] %n: %v%$"

namespace common
{
	std::shared_ptr<spdlog::logger> Logger::s_oLogger;

	void Logger::Init(const std::string& strLoggerName)
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		//logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log.log", true));

		logSinks[0]->set_pattern(DEFAULT_LOGGER_PATTERN);
		//logSinks[1]->set_pattern(DEFAULT_LOGGER_PATTERN);

		s_oLogger = std::make_shared<spdlog::logger>(strLoggerName, begin(logSinks), end(logSinks));

		s_oLogger->set_level(spdlog::level::trace);
	}
}