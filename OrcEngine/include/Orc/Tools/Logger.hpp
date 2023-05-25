#pragma once

#include <source_location>

#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>

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

	static void init(const std::string& logPath, const std::string& gameLoggerName);
	static void shutdown() ;

	template<typename... Arguments>
	static void engineLog(Logger::Level level, std::source_location sourceLocation, fmt::format_string<Arguments...> message, Arguments&&... arguments) 
	{
		m_engineLogger->log({ sourceLocation.file_name(), static_cast<std::int32_t>(sourceLocation.line()), sourceLocation.function_name() }, static_cast<spdlog::level::level_enum>(level), message, std::forward<Arguments>(arguments)...);
	}

	template<typename... Arguments>
	static void clientLog(Logger::Level level, std::source_location sourceLocation, fmt::format_string<Arguments...> message, Arguments&&... arguments) 
	{
		m_clientLogger->log({ sourceLocation.file_name(), static_cast<std::int32_t>(sourceLocation.line()), sourceLocation.function_name() }, static_cast<spdlog::level::level_enum>(level), message, std::forward<Arguments>(arguments)...);
	}

private:
	inline static std::shared_ptr<spdlog::logger> m_engineLogger;
	inline static std::shared_ptr<spdlog::logger> m_clientLogger;
};

}

#define ORC_CORE_LOG_INFO(message, ...)     orc::Logger::engineLog(orc::Logger::Level::Info, std::source_location::current(), message, __VA_ARGS__)
#define ORC_CORE_LOG_WARNING(message, ...)  orc::Logger::engineLog(orc::Logger::Level::Warning, std::source_location::current(), message, __VA_ARGS__)
#define ORC_CORE_LOG_ERROR(message, ...)    orc::Logger::engineLog(orc::Logger::Level::Error, std::source_location::current(), message, __VA_ARGS__)
#define ORC_CORE_LOG_FATAL(message, ...)    orc::Logger::engineLog(orc::Logger::Level::Fatal, std::source_location::current(), message, __VA_ARGS__)

#define ORC_LOG_INFO(message, ...)     orc::Logger::clientLog(orc::Logger::Level::Info, std::source_location::current(), message, __VA_ARGS__)
#define ORC_LOG_WARNIING(message, ...)  orc::Logger::clientLog(orc::Logger::Level::Warning, std::source_location::current(), message, __VA_ARGS__)
#define ORC_LOG_ERROR(message, ...)    orc::Logger::clientLog(orc::Logger::Level::Error, std::source_location::current(), message, __VA_ARGS__)
#define ORC_LOG_FATAL(message, ...)    orc::Logger::clientLog(orc::Logger::Level::Fatal, std::source_location::current(), message, __VA_ARGS__)
