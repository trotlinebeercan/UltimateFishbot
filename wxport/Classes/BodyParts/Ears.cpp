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

// TODO: Hook up CoreAudio code

UF::BodyParts::Ears::Ears(UF::Manager *manager)
{
    m_manager = manager;

    // TODO: Get this from parameters + user settings
    const bool useAverageSound = false;
    if (useAverageSound)
    {
        m_listenTimer.setHandler(std::bind(&UF::BodyParts::Ears::listenTimerTickAverage, this));
    }
    else
    {
        m_listenTimer.setHandler(std::bind(&UF::BodyParts::Ears::listenTimerTick, this));
    }

    m_listenTimer.setInterval(kSoundTickRate);
    m_listenTimer.setEnabled(true);
}

void
UF::BodyParts::Ears::listenTimerTick()
{
    int32_t currentVolumeLevel = 100;

    if (currentVolumeLevel >= /*SplashLimit*/ 200)
    {
        m_manager->hearFish();
    }
}

void
UF::BodyParts::Ears::listenTimerTickAverage()
{
    int32_t currentVolumeLevel = 100;
    m_volumeQueue.push_front(currentVolumeLevel);

    if (m_volumeQueue.size() >= kMaxVolumeQueueLength)
    {
        m_volumeQueue.pop_back();
    }

    int32_t averageVolume = getAverageVolume();
    if (currentVolumeLevel - averageVolume >= /*SplashLimit*/ 200)
    {
        m_manager->hearFish();
    }
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