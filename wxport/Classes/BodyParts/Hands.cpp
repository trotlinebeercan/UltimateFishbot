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

#include "BodyParts/Hands.h"

UF::BodyParts::Hands::Hands::Hands()
{
    m_baitIndex = 0;
    // init cursor
    updateKeys();
}

void
UF::BodyParts::Hands::updateKeys()
{
    m_baitKeys.clear();

    // TODO: Pull from parameters + user settings
    m_baitKeys.push_back('0');
    m_baitKeys.push_back('0');
    m_baitKeys.push_back('0');
    m_baitKeys.push_back('0');
    m_baitKeys.push_back('0');
    m_baitKeys.push_back('0');
    m_baitKeys.push_back('0');
}

void
UF::BodyParts::Hands::cast()
{
    // UF::OS::activateWarcraft();
    // sleep(casting delay);
    // UF::OS::sendKeyPress(FishKey);
}

void
UF::BodyParts::Hands::loot()
{
    // UF::OS::sendClick();
}

void
UF::BodyParts::Hands::resetBaitIndex()
{
    m_baitIndex = 0;
}

void
UF::BodyParts::Hands::doAction(UF::NeededAction action)
{
    uint8_t actionKey = '0';
    int32_t sleepTime = 0;

    // TODO: Set action keys from parameters + user settings
    switch (action)
    {
        case UF::NeededAction::HearthStone:
        {
            actionKey = '0';
            sleepTime = 0;
            break;
        }
        case UF::NeededAction::Lure:
        {
            actionKey = '0';
            sleepTime = 3;
            break;
        }
        case UF::NeededAction::Charm:
        {
            actionKey = '0';
            sleepTime = 3;
            break;
        }
        case UF::NeededAction::Raft:
        {
            actionKey = '0';
            sleepTime = 2;
            break;
        }
        case UF::NeededAction::Bait:
        {
            int32_t baitIndex = 0;

            // if (cycle through bait list)
            {
                if (m_baitIndex >= 6)
                {
                    m_baitIndex = 0;
                }

                baitIndex = m_baitIndex++;
            }

            actionKey = m_baitKeys.at(baitIndex);
            sleepTime = 3;
            break;
        }
        default:
        {
            break;
        }
    }

    // UF::OS::activateWarcraft();
    // UF::OS::sendKeyPress(actionKey);
    // sleep(sleepTime * 1000);
}
