#include "Log.h"

namespace Spindle {

    std::shared_ptr<spdlog::logger> Log::spindleCoreLogger;
    std::shared_ptr<spdlog::logger> Log::spindleClientLogger;
    std::shared_ptr<spdlog::logger> Log::spindleTestLogger;

    void Log::Init() {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        spindleCoreLogger = spdlog::stdout_color_mt("SPINDLE"); // log to files: basic_logger_mt("SPINDLE")
        spindleCoreLogger->set_level(spdlog::level::trace);

        spindleClientLogger = spdlog::stdout_color_mt("APP");   // log to console: stdout_color_mt("APP")
        spindleClientLogger->set_level(spdlog::level::trace);

        spindleTestLogger = spdlog::stdout_color_mt("TEST");   // log to console: stdout_color_mt("APP")
        spindleTestLogger->set_level(spdlog::level::trace);
    }

}