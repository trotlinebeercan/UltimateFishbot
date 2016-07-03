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

#include "Helpers/OS.h"
#include "Manager.h"

UF::BodyParts::Eyes::Eyes::Eyes(UF::Manager *manager)
{
    m_manager = manager;

    // TODO: Load fishing cursor from file
}

void
UF::BodyParts::Eyes::startLooking()
{
    if (m_running) return;

    m_manager->setActualState(UF::FishingState::SearchingForBobber);

    m_found   = false;
    m_running = true;
    m_backgroundWorker = std::thread(std::bind(&UF::BodyParts::Eyes::backgroundThreadRun, this));
}

void
UF::BodyParts::Eyes::backgroundThreadRun()
{
    // TODO: Get window rect and use that over custom rect
    // TODO: Get option to ^ from parameters + user settings

    m_xPosMin = 40;
    m_xPosMax = 400;
    m_yPosMin = 40;
    m_yPosMin = 400;

    // TODO: Get option from parameters + user settings
    //lookForBobberSpiral();
    lookForBobber();
    backgroundThreadComplete();
}

void
UF::BodyParts::Eyes::backgroundThreadComplete()
{
    if (!m_found)
    {
        m_manager->setActualState(UF::FishingState::Idle);
        return;
    }

    m_manager->setActualState(UF::FishingState::WaitingForFish);
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
                    m_found = true;
                    return;
                }
            }
        }
    }

    // we didn't catch a fish! :(
    m_found = false;
}

void
UF::BodyParts::Eyes::lookForBobberSpiral()
{
    // TODO: Implement
}

bool
UF::BodyParts::Eyes::moveMouseAndCheckCursor(int32_t x, int32_t y)
{
    if (m_manager->isStoppedOrPaused())
    {
        // TODO throw exception
        return false;
    }

    UF::OS::moveMouse(x, y);

    // TODO: check if we need to sleep here, or if sleep is ok in moveMouse

    UF::Image8Bit cursor = UF::OS::getCursor();

    if (!imageCompare(cursor, m_fishingCursor)) return false;

    return true;
}

bool
UF::BodyParts::Eyes::imageCompare(UF::Image8Bit image0, UF::Image8Bit image1)
{
    if (image0.m_width != image1.m_width || image0.m_height != image0.m_height)
    {
        return false;
    }

    for (int32_t i = 0; i < image0.m_width; i++)
    {
        for (int32_t j = 0; j < image0.m_height; j++)
        {
            // TODO: Compare images (somehow)
        }
    }

    return true;
}
