#pragma once

#include "TimePoint.h"

namespace Spindle {
    class IStopWatch
    {
    public:
        virtual ~IStopWatch() = default;
        virtual void Stop() = 0;
    };

    class StopWatch : public IStopWatch
    {
    private:
        TimePoint startTIme;
        bool m_Stopped;

    public:
        StopWatch();
        ~StopWatch() override;
        void Stop() override;
    };
}
