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

#include "BodyParts/Legs.h"

#include "Helpers/OS.h"

UF::BodyParts::Legs::Legs()
{
    //
}

void
UF::BodyParts::Legs::doMovement()
{
    // TODO: Pull from parameters + user settings
    const UF::BodyParts::Legs::Path path = UF::BodyParts::Legs::Path::Jump;
    
    switch (path)
    {
        case UF::BodyParts::Legs::Path::FrontBack:
        {
            movePath({ '0', '1' });
            break;
        }
        case UF::BodyParts::Legs::Path::LeftRight:
        {
            movePath({ '0', '1' });
            break;
        }
        case UF::BodyParts::Legs::Path::Jump:
        default:
        {
            movePath({ '0' });
        }
    }
}

void
UF::BodyParts::Legs::movePath(std::vector<uint8_t> keys)
{
    for (uint8_t k : keys)
    {
        singleMove(k);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void
UF::BodyParts::Legs::singleMove(uint8_t key)
{
    // TODO: make sure sendKeyPress emits both key down and key up
    UF::OS::sendKeyPress(key);
}
