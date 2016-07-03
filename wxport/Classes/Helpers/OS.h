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

#ifndef __ULTIMATEFISHBOT_OS_H__
#define __ULTIMATEFISHBOT_OS_H__

#include "Helpers/Global.h"

namespace UF
{
    namespace OS
    {
        void moveMouse(int32_t x, int32_t y);
        void sendKeyPress(uint8_t key);
        void sendClick();
        void activateWarcraft();
        UF::Image8Bit getCursor();
    };
};

#endif // __ULTIMATEFISHBOT_OS_H__
