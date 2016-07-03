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

#ifndef __ULTIMATEFISHBOT_EYES_H__
#define __ULTIMATEFISHBOT_EYES_H__

#include "Helpers/Global.h"

namespace UF
{
    class Manager;

    namespace BodyParts
    {
        class Eyes
        {
        public:
            Eyes(UF::Manager *manager);

        public:
            void startLooking();

        private:
            void lookForBobber();
            void lookForBobberSpiral();
            bool moveMouseAndCheckCursor(int32_t x, int32_t y);
            bool imageCompare();

            //void EyeProcess_DoWork();
            //void EyeProcess_RunWorkerCompleted();

        private:
            UF::Manager *m_manager;
            int32_t m_xPosMin;
            int32_t m_xPosMax;
            int32_t m_yPosMin;
            int32_t m_yPosMax;
        };

    }; // namespace BodyParts
}; // namespace UF

#endif // __ULTIMATEFISHBOT_EYES_H__