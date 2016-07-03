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

#include "BodyParts/Ears.h"

#include "Manager.h"

UF::BodyParts::Ears::Ears(UF::Manager *manager)
{
    m_manager = manager;
}

void
UF::BodyParts::Ears::listenTimerTick()
{
    //
}

void
UF::BodyParts::Ears::listenTimerTickAverage()
{
    //
}

int32_t
UF::BodyParts::Ears::getAverageVolume() const
{
    int32_t sum = std::accumulate(
        m_volumeQueue.begin(),
        m_volumeQueue.end(),
        0
    );

    return sum /= m_volumeQueue.size();
}