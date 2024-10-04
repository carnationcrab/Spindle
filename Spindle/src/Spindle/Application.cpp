#include "Application.h"
#include "Math/Vector.h"
#include "Test/tester.h"

namespace Spindle {
    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        tester testManager;

        testManager.runAllTests();

        while (true) {
            // TODO replace with loop
        }
    }
}