#include "TimePoint.h"
#include <chrono>

namespace Spindle {
    TimePoint::TimePoint()
        : m_TimeInMicroseconds{ 0 }
    {
    }

    long long TimePoint::GetMicroseconds() const
    {
        return m_TimeInMicroseconds;
    }

    TimePoint TimePoint::Now()
    {
        TimePoint timePoint;
        std::chrono::steady_clock::time_point now 
            = std::chrono::high_resolution_clock::now();
        
        timePoint.m_TimeInMicroseconds 
            = std::chrono::time_point_cast<std::chrono::microseconds>
            (now).time_since_epoch().count();
        
        return timePoint;
    }
}
