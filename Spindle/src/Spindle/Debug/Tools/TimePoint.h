#pragma once

namespace Spindle{
    class TimePoint
    {
    private:
        long long m_TimeInMicroseconds;

    public:
        TimePoint();
        long long GetMicroseconds() const;
        static TimePoint Now();
    };
}
