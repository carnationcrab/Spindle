// timer.cpp
#include "StopWatch.h"
#include "../../Log.h"

// USAGE
/*
#include "StopWatch.h"

void SomeFunction()
{
    StopWatch stopWatch;

    // some code to time
    for (int i = 0; i < 1000; i++)
    {
        // does something
    }

    // the stopWatch will automatically stop and print the duration when it goes out of scope
}

*/

namespace Spindle {
    StopWatch::StopWatch()
        : m_Stopped(false)
    {
        // start the timer
        startTIme = TimePoint::Now();
    }

    StopWatch::~StopWatch()
    {
        // stop the timer if it hasn't been stopped already
        if (!m_Stopped)
        {
            Stop();
        }
    }

    void StopWatch::Stop()
    {
        Spindle::TimePoint endTimepoint = TimePoint::Now();
        long long start = startTIme.GetMicroseconds();
        long long end = endTimepoint.GetMicroseconds();

        long long duration{ end - start };
        double ms{ duration * 0.001 };

        // output the duration
        SPINDLE_INFO("Duration: {}us ( {} ms)", duration, ms);

        m_Stopped = true;
    }
}
