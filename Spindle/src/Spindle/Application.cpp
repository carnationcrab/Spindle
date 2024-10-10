#include "Application.h"
#include "Math/Vector2.h"
#include "Log.h"

#include <string>

namespace Spindle {
    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        Vector2<float> v1{ 3.0f, 4.0f };
        Vector2<float> v2{ -1.0f, 2.0f };
        Vector2<float> v3{ 0.0f, 0.0f };
        Vector2<float> v4{ 1.5f, -3.5f };

        // v1 + v2
        Vector2<float> addResult1 = v1 + v2;
        SPINDLE_INFO("Addition {}", addResult1.toString());

        // v3 + v4
        Vector2<float> addResult2 = v3 + v4;
        SPINDLE_INFO("Addition {}", addResult2.toString());

        // v1 + v2
        Vector2<float> subResult1 = v1 - v2;
        SPINDLE_INFO("Subtraction {}", subResult1.toString());

        // v3 + v4
        Vector2<float> subResult2 = v3 - v4;
        SPINDLE_INFO("Subtraction {}", subResult2.toString());

        // v1 * 2.0f
        Vector2<float> multResult1 = v1 * 2.0f;
        SPINDLE_INFO("Multiplication {}", multResult1.toString());

        // v4 * -1.0f
        Vector2<float> multResult2 = v4 * -1.0f;
        SPINDLE_INFO("Multiplication {}", multResult2.toString());

        // dot product of v1 and v2
        float dotResult1 = v1.dot(v2);
        SPINDLE_INFO("Dot Product {}", dotResult1);

        // dot product of v4 and v3
        float dotResult2 = v4.dot(v3);
        SPINDLE_INFO("Dot Product {}", dotResult2);

        // might want to be able to do it like this?
        //float dotResult2 = Vector2<float>::dot(v4, v3);


        while (true) {
            // TODO replace with loop
        }
    }
}
