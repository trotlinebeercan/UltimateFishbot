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

#include "BodyParts/Eyes.h"

#include "Manager.h"

UF::BodyParts::Eyes::Eyes::Eyes(UF::Manager *manager)
{
    m_manager = manager;
}

void
UF::BodyParts::Eyes::startLooking()
{
    /*
    if (m_backgroundWorker->busy()) return;
    */

    m_manager->setActualState(UF::FishingState::SearchingForBobber);
    // m_backgroundWorker->runWorkerAsync();
}

void
UF::BodyParts::Eyes::lookForBobber()
{
    const int32_t scanningRetries = 3;
    const float scanningSteps     = 10.0;

    int32_t xStep   = (int32_t)((m_xPosMax - m_xPosMin) / scanningSteps);
    int32_t yStep   = (int32_t)((m_yPosMax - m_yPosMin) / scanningSteps);
    int32_t xOffset = (int32_t)(xStep / scanningRetries);

    for (int32_t tryCount = 0; tryCount < scanningRetries; ++tryCount)
    {
        for (int32_t x = (int)(m_xPosMin + (xOffset * tryCount)); x < m_xPosMax; x += xStep)
        {
            for (int32_t y = m_yPosMin; y < m_yPosMax; y += yStep)
            {
                if (moveMouseAndCheckCursor(x, y))
                {
                    return;
                }
            }
        }
    }

    // we didn't catch a fish! :(
}

void
UF::BodyParts::Eyes::lookForBobberSpiral()
{
    // TODO
}

bool
UF::BodyParts::Eyes::moveMouseAndCheckCursor(int32_t x, int32_t y)
{
    if (m_manager->isStoppedOrPaused())
    {
        // TODO throw exception
        return false;
    }

    // UF::OS::moveMouse(x, y);

    // sleep(9001);

    // UF::OS::getCursor();

    // if (!theCursorWeWant) return false;

    // if (!imageCompare(currentCursor, fishingCursor)) return false;

    return true;
}

bool
UF::BodyParts::Eyes::imageCompare()
{
    return true;
}
