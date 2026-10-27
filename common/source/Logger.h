#pragma once

#include <memory>
#include <string>
#include <spdlog/spdlog.h>

namespace common
{
	class Logger
	{
	public:
		static void Init(const std::string& strLoggerName);

		inline static const std::shared_ptr<spdlog::logger>& GetLogger()
		{
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
};

//Log macros
#define LOG_INFO(...)	::common::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)	::common::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_TRACE(...)	::common::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_ERROR(...)	::common::Logger::GetLogger()->error(__VA_ARGS__)
