#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Core.h"


namespace Spindle {

    class SPINDLE_API Log {
        public:
            static void Init();

            inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return spindleCoreLogger; }
            inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return spindleClientLogger; }
            inline static std::shared_ptr<spdlog::logger>& GetTestLogger() { return spindleTestLogger; }


        private:
            static std::shared_ptr<spdlog::logger> spindleCoreLogger;
            static std::shared_ptr<spdlog::logger> spindleClientLogger;
            static std::shared_ptr<spdlog::logger> spindleTestLogger;

        };

}

// Core log macros
#define SPINDLE_CORE_TRACE(...)    ::Spindle::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SPINDLE_CORE_INFO(...)     ::Spindle::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SPINDLE_CORE_WARN(...)     ::Spindle::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SPINDLE_CORE_ERROR(...)    ::Spindle::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SPINDLE_CORE_FATAL(...)    ::Spindle::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define SPINDLE_TRACE(...)         ::Spindle::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SPINDLE_INFO(...)          ::Spindle::Log::GetClientLogger()->info(__VA_ARGS__)
#define SPINDLE_WARN(...)          ::Spindle::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SPINDLE_ERROR(...)         ::Spindle::Log::GetClientLogger()->error(__VA_ARGS__)
#define SPINDLE_FATAL(...)         ::Spindle::Log::GetClientLogger()->critical(__VA_ARGS__)

// Test log macros
#define SPINDLE_TEST_PASS(...)    ::Spindle::Log::GetTestLogger()->info(__VA_ARGS__)
#define SPINDLE_TEST_INFO(...)    ::Spindle::Log::GetTestLogger()->trace(__VA_ARGS__)
#define SPINDLE_TEST_FAIL(...)    ::Spindle::Log::GetTestLogger()->error(__VA_ARGS__)

