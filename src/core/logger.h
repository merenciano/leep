//  Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#ifndef __MOTORET_LOG_H__
#define __MOTORET_LOG_H__

#include <stdint.h>

#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace motoret 
{
	class Logger
	{
	public:
		static void init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_core_logger; }
		static std::shared_ptr<spdlog::logger>& GetAppLogger()  { return s_app_logger; }
		
	private:
		static std::shared_ptr<spdlog::logger> s_core_logger;
		static std::shared_ptr<spdlog::logger> s_app_logger;
	};
}

#ifndef MOTORET_RELEASE
	// Engine macros
	#define MOTORET_CORE_TRACE(...)    ::motoret::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define MOTORET_CORE_INFO(...)     ::motoret::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define MOTORET_CORE_WARNING(...)  ::motoret::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define MOTORET_CORE_ERROR(...)    ::motoret::Logger::GetCoreLogger()->error(__VA_ARGS__)
	#define MOTORET_CORE_CRITICAL(...) ::motoret::Logger::GetCoreLogger()->critical(__VA_ARGS__)

	// App macros
	#define MOTORET_TRACE(...)    ::motoret::Logger::GetAppLogger()->trace(__VA_ARGS__)
	#define MOTORET_INFO(...)     ::motoret::Logger::GetAppLogger()->info(__VA_ARGS__)
	#define MOTORET_WARNING(...)  ::motoret::Logger::GetAppLogger()->warn(__VA_ARGS__)
	#define MOTORET_ERROR(...)    ::motoret::Logger::GetAppLogger()->error(__VA_ARGS__)
	#define MOTORET_CRITICAL(...) ::motoret::Logger::GetAppLogger()->critical(__VA_ARGS__)
#else
	#define MOTORET_CORE_TRACE(...)
	#define MOTORET_CORE_INFO(...)
	#define MOTORET_CORE_WARNING(...)
	#define MOTORET_CORE_ERROR(...)
	#define MOTORET_CORE_CRITICAL(...)
	#define MOTORET_TRACE(...)
	#define MOTORET_INFO(...)
	#define MOTORET_WARNING(...)
	#define MOTORET_ERROR(...)
	#define MOTORET_CRITICAL(...)
#endif

#endif // __MOTORET_LOGGER_H__