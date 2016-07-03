////////////////////////////////////////////////////////////////////////////////
// UltimateFishbot - A simple World of Warcraft fishbot.
//
// Original Work (VB port)
//   Copyright (c) 2016, Gordon (LaBlazer).  All rights reserved.
// Modified Work (C# port)
//   Copyright (c) 2016, Pierre Ghiot (Mograine). All rights reserved.
// Modified Work (C++ port)
//   Copyright (c) 2016, Dustin Hopper (trotlinebeercan). All rights reserved.
//
// Copyrights licensed under an unknown license.
// https://github.com/UltimateFishbot/UltimateFishbot
////////////////////////////////////////////////////////////////////////////////

#ifndef __ULTIMATEFISHBOT_TIMER_H__
#define __ULTIMATEFISHBOT_TIMER_H__

#include "Helpers/Global.h"

namespace UF
{
    class Timer
    {
    public:
        typedef std::chrono::milliseconds Interval;
        typedef std::function<void()> Handler;

    public:
        Timer()
        {
            m_running = false;
            m_timePassed = 0;
        }

        Timer(Interval interval, Handler handler)
        {
            m_interval = interval;
            m_handler  = handler;
            m_running  = false;
            m_timePassed = 0;
        }

    public:
        void start()
        {
            if (m_running) return;

            m_timePassed = 0;
            m_running = true;
            m_thread = std::thread(std::bind(&UF::Timer::run, this));
        }

        void stop(bool wait = false)
        {
            m_running = false;
            m_enabled = false;

            m_timePassed = INT_MAX;

            if (wait)
            {
                m_thread.join();
            }
        }

    private:
        void run()
        {
            while (m_running)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

                m_timePassed += 50;

                // check if we have hit the interval
                if (m_timePassed >= m_interval.count())
                {
                    if (m_enabled)
                    {
                        m_handler();
                    }

                    m_timePassed = 0;
                }
            }
        }

    public:
        void setInterval(Interval interval)
        {
            m_interval = interval;
            m_timePassed = 0;
        }

        void setInterval(int32_t ms)
        {
            m_interval = Interval(ms);
            m_timePassed = 0;
        }

        void setHandler(Handler handler)
        {
            m_handler = handler;
        }

        void setEnabled(bool enabled)
        {
            m_enabled = enabled;
        }

    private:
        Interval m_interval;
        Handler  m_handler;

        std::thread m_thread;
        bool m_running;
        bool m_enabled;

        int32_t m_timePassed;
    };
};

#endif // __ULTIMATEFISHBOT_TIMER_H__
