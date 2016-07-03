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

#ifndef __ULTIMATEFISHBOT_HANDS_H__
#define __ULTIMATEFISHBOT_HANDS_H__

#include "Helpers/Global.h"

namespace UF
{
    namespace BodyParts
    {
        class Hands
        {
        public:
            Hands();

        public:
            void updateKeys();
            void cast();
            void loot();
            void resetBaitIndex();
            void doAction(UF::NeededAction action);

        private:
            int32_t m_baitIndex;
            std::vector<uint8_t> m_baitKeys;
        };
    };
};

#endif // __ULTIMATEFISHBOT_HANDS_H__