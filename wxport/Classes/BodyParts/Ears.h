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

#ifndef __ULTIMATEFISHBOT_EARS_H__
#define __ULTIMATEFISHBOT_EARS_H__

#include "Helpers/Global.h"

namespace UF
{
    class Manager;

    namespace BodyParts
    {
        class Ears
        {
        public:
            Ears(UF::Manager *manager);

        private:
            void listenTimerTick();
            void listenTimerTickAverage();
            int32_t getAverageVolume() const;

        private:
            UF::Manager *m_manager;
            // Timer m_lisenTimer;
            std::list<int32_t> m_volumeQueue;
        };
    };
};

#endif // __ULTIMATEFISHBOT_EARS_H__