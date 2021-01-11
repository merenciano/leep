//  Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __LEEP_CORE_LOGGER_H__
#define __LEEP_CORE_LOGGER_H__

#include <stdint.h>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace leep
{
	class Logger
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_core_logger; }
		static std::shared_ptr<spdlog::logger>& GetAppLogger()  { return s_app_logger; }
		
	private:
		static std::shared_ptr<spdlog::logger> s_core_logger;
		static std::shared_ptr<spdlog::logger> s_app_logger;
	};
}

#ifndef LEEP_RELEASE
	// Engine macros
	#define LEEP_CORE_TRACE(...)    ::leep::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define LEEP_CORE_INFO(...)     ::leep::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define LEEP_CORE_WARNING(...)  ::leep::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define LEEP_CORE_ERROR(...)    ::leep::Logger::GetCoreLogger()->error(__VA_ARGS__)
	#define LEEP_CORE_CRITICAL(...) ::leep::Logger::GetCoreLogger()->critical(__VA_ARGS__)

	// App macros
	#define LEEP_TRACE(...)    ::leep::Logger::GetAppLogger()->trace(__VA_ARGS__)
	#define LEEP_INFO(...)     ::leep::Logger::GetAppLogger()->info(__VA_ARGS__)
	#define LEEP_WARNING(...)  ::leep::Logger::GetAppLogger()->warn(__VA_ARGS__)
	#define LEEP_ERROR(...)    ::leep::Logger::GetAppLogger()->error(__VA_ARGS__)
	#define LEEP_CRITICAL(...) ::leep::Logger::GetAppLogger()->critical(__VA_ARGS__)
#else
	#define LEEP_CORE_TRACE(...)
	#define LEEP_CORE_INFO(...)
	#define LEEP_CORE_WARNING(...)
	#define LEEP_CORE_ERROR(...)
	#define LEEP_CORE_CRITICAL(...)
	#define LEEP_TRACE(...)
	#define LEEP_INFO(...)
	#define LEEP_WARNING(...)
	#define LEEP_ERROR(...)
	#define LEEP_CRITICAL(...)
#endif

#endif // __LEEP_CORE_LOGGER_H__