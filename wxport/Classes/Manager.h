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

#ifndef __ULTIMATEFISHBOT_MANAGER_H__
#define __ULTIMATEFISHBOT_MANAGER_H__

#include "Helpers/Global.h"
#include "Helpers/Timer.h"

namespace UF
{
    namespace BodyParts
    {
        class Ears;
        class Eyes;
        class Hands;
        class Legs;
    };

    class Manager
    {
    public:
        Manager();
        ~Manager();

    public:
        void start();
        void restart();
        void resume();
        void stop();
        void pause();

        void hearFish();

        bool isStoppedOrPaused() const;

        UF::FishingState getActualState() const;
        void setActualState(UF::FishingState new_state);

        UF::FishingStats getFishingStats() const;
        void resetFishingStats();

        int32_t getFishWaitTime() const;

    private:
        void updateStats(UF::FishingState new_state);

        void initializeTimer(UF::Timer &timer, std::function<void()> handle);
        void switchTimerState(bool enabled);
        void resetTimers();

        void takeNextAction();
        void handleNeededAction(UF::NeededAction action);
        void addNeededAction(UF::NeededAction action);
        void removeNeededAction(UF::NeededAction action);
        bool hasNeededAction(UF::NeededAction action) const;

        void lureTimerTick();
        void raftTimerTick();
        void charmTimerTick();
        void baitTimerTick();
        void hearthStoneTimerTick();
        void antiAFKTimerTick();

    private:
        UF::Timer m_nextActionTimer;
        UF::Timer m_lureTimer;
        UF::Timer m_hearthStoneTimer;
        UF::Timer m_raftTimer;
        UF::Timer m_charmTimer;
        UF::Timer m_baitTimer;
        UF::Timer m_antiAFKTimer;

        int32_t m_fishWaitTime;

        UF::BodyParts::Eyes  *m_eyes;
        UF::BodyParts::Hands *m_hands;
        UF::BodyParts::Ears  *m_ears;
        UF::BodyParts::Legs  *m_legs;
        // UF::BodyParts::Mouth *m_mouth;

        UF::NeededAction m_neededActions;
        UF::FishingState m_actualState;
        UF::FishingStats m_fishingStats;
    };
}; // namespace UF

#endif // __ULTIMATEFISHBOT_MANAGER_H__