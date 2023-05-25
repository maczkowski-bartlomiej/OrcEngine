#include "OrcPch.hpp"

#include "Engine\Logger.hpp"

#include <spdlog\async.h>
#include <spdlog\spdlog.h>
#include <spdlog\sinks\basic_file_sink.h>
#include <spdlog\sinks\stdout_color_sinks.h>

#include <array>

namespace orc {

class CapitalizedLevelNamesFormatter : public spdlog::custom_flag_formatter
{
public:
	void format(const spdlog::details::log_msg& message, const std::tm&, spdlog::memory_buf_t& destination) override
	{
		static constexpr std::array<std::string_view, 7u> names{ "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "OFF" };
		std::string_view name = names[static_cast<size_t>(message.level)];

		destination.append(name.data(), name.data() + name.size());
	}

	std::unique_ptr<custom_flag_formatter> clone() const override
	{
		return spdlog::details::make_unique<CapitalizedLevelNamesFormatter>();
	}
};

void Logger::init(const std::string& logPath)
{
	static bool initialized = false;

	if (initialized)
	{
		ORC_FATAL("Logger already initialized!!!");
		return;
	}

	initialized = true;

	spdlog::init_thread_pool(8192, 1);
	std::vector<spdlog::sink_ptr> sinks
	{
		std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
		std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath, true)
	};

	std::string pattern = "[%H:%M:%S %s:%#] [%^%*%$] %v";

	auto formatter = std::make_unique<spdlog::pattern_formatter>();
	formatter->add_flag<CapitalizedLevelNamesFormatter>('*').set_pattern(pattern);

	sinks[0]->set_formatter(formatter->clone());
	sinks[1]->set_formatter(formatter->clone());

	m_logger = std::make_shared<spdlog::async_logger>("ENGINE", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
	m_logger->set_level(spdlog::level::info);
	m_logger->flush_on(spdlog::level::err);
}

void Logger::shutdown() 
{
	spdlog::shutdown();
}

}
