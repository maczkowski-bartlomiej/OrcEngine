#pragma once

#include <filesystem>
#include <source_location>

#include <fmt/core.h>

namespace orc {

using FilePath = std::filesystem::path;

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

	static void init(const FilePath& logPath);
	static void deinit();
	static void flush();

	static void logImpl(Level level, std::source_location sourceLocation, fmt::string_view format, fmt::format_args args);

	template <typename... Arguments>
	static void log(Level level, std::source_location sourceLocation, fmt::format_string<Arguments...> message, Arguments&&... arguments)
	{
		logImpl(level, sourceLocation, message.get(), fmt::make_format_args(arguments...));
	}
};

}

#define ORC_LOG_INFO(message, ...) orc::Logger::log(orc::Logger::Level::Info, std::source_location::current(), message __VA_OPT__(, ) __VA_ARGS__)
#define ORC_LOG_WARNING(message, ...) orc::Logger::log(orc::Logger::Level::Warning, std::source_location::current(), message __VA_OPT__(, ) __VA_ARGS__)
#define ORC_LOG_FATAL(message, ...) orc::Logger::log(orc::Logger::Level::Fatal, std::source_location::current(), message __VA_OPT__(, ) __VA_ARGS__)
