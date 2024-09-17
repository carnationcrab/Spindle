#pragma once

#include "Core.h"
#include <memory>
#include "spdlog/spdlog.h"


namespace Spindle {

    class SPINDLE_API Log {
        public:
            static void Init();

            inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
            inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        private:
            static std::shared_ptr<spdlog::logger> s_CoreLogger;
            static std::shared_ptr<spdlog::logger> s_ClientLogger;
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