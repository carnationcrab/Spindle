#pragma once

#include "Core.h"

namespace Spindle {
    class SPINDLE_API Application {
        public:
            Application();
            virtual ~Application();

            void Run();
        };

    // To be defined in CLIENT
    Application* CreateApplication();
}