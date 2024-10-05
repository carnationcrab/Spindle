#pragma once

#include "../../Math/Vector.h"
#include "test.h"

namespace Spindle {
    class mathsTest : test {
    public:
        void testBaseMaths();
        void testVector2();
        void testVector3();
        void runAllTests() override;

    };
}
