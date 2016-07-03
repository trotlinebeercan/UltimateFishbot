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

#ifndef __ULTIMATEFISHBOT_LEGS_H__
#define __ULTIMATEFISHBOT_LEGS_H__

#include "Helpers/Global.h"

namespace UF
{
    namespace BodyParts
    {
        class Legs
        {
        public:
            enum class Path
            {
                FrontBack = 0,
                LeftRight = 1,
                Jump      = 2
            };

        public:
            Legs();

        public:
            void doMovement();

        private:
            void movePath(std::vector<uint8_t> keys);
            void singleMove(uint8_t key);
        };
    };
};

#endif // __ULTIMATEFISHBOT_LEGS_H__