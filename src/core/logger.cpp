// Lucas Merenciano Martinez  <lucasmermar@gmail.com>

#include "logger.h"

#include <memory>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace leep 
{
	std::shared_ptr<spdlog::logger> Logger::s_core_logger;
	std::shared_ptr<spdlog::logger> Logger::s_app_logger;

	void Logger::Init()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("leep.log", true));

		sinks[0]->set_pattern("%^[%T] %n: %v%$");
		sinks[1]->set_pattern("[%T] [%l] %n: %v");

		s_core_logger = std::make_shared<spdlog::logger>("CORE", begin(sinks), end(sinks));
		spdlog::register_logger(s_core_logger);
		s_core_logger->set_level(spdlog::level::trace);
		s_core_logger->flush_on(spdlog::level::trace);

		s_app_logger = std::make_shared<spdlog::logger>("APP", begin(sinks), end(sinks));
		spdlog::register_logger(s_app_logger);
		s_app_logger->set_level(spdlog::level::trace);
		s_app_logger->flush_on(spdlog::level::trace);	
	}
}
