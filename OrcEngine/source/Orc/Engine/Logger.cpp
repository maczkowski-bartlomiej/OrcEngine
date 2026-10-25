#include "Engine/Logger.hpp"

#include <array>
#include <cstdlib>
#include <iostream>

#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace orc {

namespace {

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

		m_logger = std::make_shared<spdlog::logger>("ENGINE", sinks.begin(), sinks.end());
		m_logger->set_level(spdlog::level::info);
		m_logger->flush_on(spdlog::level::level_enum::err);

		m_logger->set_error_handler([](const std::string& msg) { m_logger->error("Tried to pass invalid arguments to Logger: {}", msg); });
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
	m_logger.reset();
	spdlog::shutdown();
}

void Logger::flush()
{
	if (m_logger)
	{
		m_logger->flush();
	}
}

}
