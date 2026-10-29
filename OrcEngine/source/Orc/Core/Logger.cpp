#include "Core/Logger.hpp"

#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Core/Platform.hpp"

namespace orc {

namespace {

std::shared_ptr<spdlog::logger> logger;

class CapitalizedLevelNamesFormatter : public spdlog::custom_flag_formatter
{
public:
	void format(const spdlog::details::log_msg& message, const std::tm&, spdlog::memory_buf_t& destination) override
	{
		static constexpr std::array<std::string_view, 7> names{{"TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "OFF"}};
		std::string_view name = names[static_cast<size_t>(message.level)];

		destination.append(name.data(), name.data() + name.size());
	}

	std::unique_ptr<spdlog::custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<CapitalizedLevelNamesFormatter>();
	}
};

}

void Logger::init(const FilePath& logPath)
{
	try
	{
		if (logPath.has_parent_path())
		{
			std::filesystem::create_directories(logPath.parent_path());
		}

		std::vector<spdlog::sink_ptr> sinks{
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
			std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath.string(), true)};

		std::string pattern = "[%H:%M:%S %s:%#] [%^%*%$] %v";

		auto formatter = std::make_unique<spdlog::pattern_formatter>();
		formatter->add_flag<CapitalizedLevelNamesFormatter>('*').set_pattern(pattern);

		sinks[0]->set_formatter(formatter->clone());
		sinks[1]->set_formatter(formatter->clone());

		logger = std::make_shared<spdlog::logger>("ENGINE", sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::info);
		logger->flush_on(spdlog::level::level_enum::err);

		logger->set_error_handler([](const std::string& msg) { logger->error("Tried to pass invalid arguments to Logger: {}", msg); });
	}
	catch (const spdlog::spdlog_ex& exception)
	{
		std::cerr << "[FATAL] Failed to initialize Logger...\n\tReason: " << exception.what() << std::endl;
		ORC_DEBUGBREAK();
		std::abort();
	}
}

void Logger::deinit()
{
	flush();
	logger.reset();
	spdlog::shutdown();
}

void Logger::flush()
{
	if (logger)
	{
		logger->flush();
	}
}

void Logger::logImpl(Level level, std::source_location sourceLocation, fmt::string_view format, fmt::format_args args)
{
	const auto spdlogLevel = static_cast<spdlog::level::level_enum>(level);
	if (!logger->should_log(spdlogLevel))
	{
		return;
	}

	fmt::memory_buffer buffer;
	fmt::vformat_to(std::back_inserter(buffer), format, args);

	spdlog::source_loc location{
		sourceLocation.file_name(),
		static_cast<std::int32_t>(sourceLocation.line()),
		sourceLocation.function_name()};

	logger->log(location, spdlogLevel, std::string_view{buffer.data(), buffer.size()});
}

}
