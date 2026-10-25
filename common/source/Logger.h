#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace common
{
	class Logger
	{
	public:
		static void Init();

		inline static const std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
};

//Client log macros
#define LOG_CLIENT_INFO(...)	::common::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_CLIENT_WARN(...)	::common::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_CLIENT_TRACE(...)	::common::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_CLIENT_ERROR(...)	::common::Logger::GetClientLogger()->error(__VA_ARGS__)
