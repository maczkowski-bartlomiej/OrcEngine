#pragma once

#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>

#include <source_location>

namespace orc {

class Logger
{
public:
	enum class Level
	{
		Info = 2,
		Warning,
		Error,
		Fatal
	};

	static void init(const std::string& logPath);
	static void shutdown() ;

	template<typename... Arguments>
	static void log(Logger::Level level, std::source_location sourceLocation, fmt::format_string<Arguments...> message, Arguments&&... arguments) 
	{
		m_logger->log({ sourceLocation.file_name(), static_cast<std::int32_t>(sourceLocation.line()), sourceLocation.function_name() }, static_cast<spdlog::level::level_enum>(level), message, std::forward<Arguments>(arguments)...);
	}

private:
	inline static std::shared_ptr<spdlog::logger> m_logger;
};

}

#define ORC_LOG_INFO(message, ...)    orc::Logger::log(orc::Logger::Level::Info, std::source_location::current(), message, __VA_ARGS__)
#define ORC_LOG_WARNING(message, ...) orc::Logger::log(orc::Logger::Level::Warning, std::source_location::current(), message, __VA_ARGS__)
#define ORC_LOG_ERROR(message, ...)   orc::Logger::log(orc::Logger::Level::Error, std::source_location::current(), message, __VA_ARGS__)
#define ORC_LOG_FATAL(message, ...)   orc::Logger::log(orc::Logger::Level::Fatal, std::source_location::current(), message, __VA_ARGS__)
