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

#include "Manager.h"

#include "BodyParts/Ears.h"
#include "BodyParts/Eyes.h"
#include "BodyParts/Hands.h"
#include "BodyParts/Legs.h"

UF::Manager::Manager()
{
    m_ears  = new UF::BodyParts::Ears(this);
    m_eyes  = new UF::BodyParts::Eyes(this);
    m_hands = new UF::BodyParts::Hands();
    m_legs  = new UF::BodyParts::Legs();

    m_actualState  = UF::FishingState::Stopped;
    m_neededActions = UF::NeededAction::None;

    m_fishingStats.reset();

    // initialize timers
    m_nextActionTimer.setHandler(std::bind(&UF::Manager::takeNextAction, this));
    m_lureTimer.setHandler(std::bind(&UF::Manager::lureTimerTick, this));
    m_hearthStoneTimer.setHandler(std::bind(&UF::Manager::hearthStoneTimerTick, this));
    m_raftTimer.setHandler(std::bind(&UF::Manager::raftTimerTick, this));
    m_charmTimer.setHandler(std::bind(&UF::Manager::charmTimerTick, this));
    m_baitTimer.setHandler(std::bind(&UF::Manager::baitTimerTick, this));
    m_antiAFKTimer.setHandler(std::bind(&UF::Manager::antiAFKTimerTick, this));

    resetTimers();
}

UF::Manager::~Manager()
{
    UF::Memory::safeDelete(m_ears);
    UF::Memory::safeDelete(m_eyes);
    UF::Memory::safeDelete(m_hands);
    UF::Memory::safeDelete(m_legs);
}

void
UF::Manager::start()
{
    if (getActualState() == UF::FishingState::Stopped)
    {
        restart();
    }
    else if (getActualState() == UF::FishingState::Paused)
    {
        resume();
    }
}

void
UF::Manager::restart()
{
    resetTimers();
    switchTimerState(true);

    setActualState(UF::FishingState::Start);
}

void
UF::Manager::resume()
{
    setActualState(UF::FishingState::Start);
}

void
UF::Manager::stop()
{
    switchTimerState(false);
    setActualState(UF::FishingState::Stopped);
}

void
UF::Manager::pause()
{
    setActualState(UF::FishingState::Paused);
}

void
UF::Manager::hearFish()
{
    if (getActualState() != UF::FishingState::WaitingForFish)
    {
        return;
    }

    setActualState(UF::FishingState::Looting);
    m_hands->loot();
    m_fishWaitTime = 0;
    setActualState(UF::FishingState::Idle);
}

bool
UF::Manager::isStoppedOrPaused() const
{
    UF::FishingState state = getActualState();
    return (
        state == UF::FishingState::Stopped
        || state == UF::FishingState::Paused
    );
}

UF::FishingState
UF::Manager::getActualState() const
{
    return m_actualState;
}

void
UF::Manager::setActualState(UF::FishingState new_state)
{
    if (isStoppedOrPaused())
    {
        if (new_state != UF::FishingState::Start)
        {
            return;
        }
    }

    updateStats(new_state);

    m_actualState = new_state;
}

UF::FishingStats
UF::Manager::getFishingStats() const
{
    return m_fishingStats;
}

void
UF::Manager::resetFishingStats()
{
    m_fishingStats.reset();
}

int32_t
UF::Manager::getFishWaitTime() const
{
    return m_fishWaitTime;
}

void
UF::Manager::updateStats(UF::FishingState new_state)
{
    if (new_state == UF::FishingState::Idle)
    {
        switch (getActualState())
        {
            case UF::FishingState::Looting:
            {
                ++m_fishingStats.totalSuccessFishing;
                break;
            }
            case UF::FishingState::Casting:
            case UF::FishingState::SearchingForBobber:
            {
                ++m_fishingStats.totalNotFoundFish;
                break;
            }
            case UF::FishingState::WaitingForFish:
            {
                ++m_fishingStats.totalNotEaredFish;
                break;
            }
            default:
            {
                break;
            }
        };
    }
}

void
UF::Manager::switchTimerState(bool enabled)
{
    // TODO: Get these from parameters + user settings
    const bool autoLure   = false;
    const bool autoRaft   = false;
    const bool autoCharm  = false;
    const bool autoBait   = false;
    const bool autoHearth = false;
    const bool antiAfk    = false;

    m_nextActionTimer.setEnabled(enabled);

    m_lureTimer.setEnabled(autoLure && enabled);
    m_raftTimer.setEnabled(autoRaft && enabled);
    m_charmTimer.setEnabled(autoCharm && enabled);
    m_baitTimer.setEnabled(autoBait && enabled);
    m_hearthStoneTimer.setEnabled(autoHearth && enabled);
    m_antiAFKTimer.setEnabled(antiAfk && enabled);
}

void
UF::Manager::resetTimers()
{
    m_nextActionTimer.stop();
    m_nextActionTimer.setInterval(kActionTimerLength);

    // TODO: Get these from parameters + user settings
    const int32_t lureTime = 10;
    const int32_t hsTime = 15;
    const int32_t raftTime = 8;
    const int32_t charmTime = 60;
    const int32_t baitTime = 5;
    const int32_t afkTime = 15;

    m_lureTimer.setInterval(lureTime * kMinute + 22 * kSecond);
    m_hearthStoneTimer.setInterval(hsTime * kMinute);
    m_raftTimer.setInterval(raftTime * kMinute);
    m_charmTimer.setInterval(charmTime * kMinute);
    m_baitTimer.setInterval(baitTime * kMinute);
    m_antiAFKTimer.setInterval(afkTime * kMinute);

    m_fishWaitTime = 0;
}

void
UF::Manager::takeNextAction()
{
    switch (getActualState())
    {
        case UF::FishingState::Start:
        {
            setActualState(UF::FishingState::Idle);
            break;
        }
        case UF::FishingState::Idle:
        {
            for (UF::NeededAction a : kAllNeededActions)
            {
                if (hasNeededAction(a))
                {
                    handleNeededAction(a);
                    return;
                }
            }

            // if no other action is required, we can cast
            setActualState(UF::FishingState::Casting);
            m_hands->cast();
            break;
        }
        case UF::FishingState::Casting:
        {
            setActualState(UF::FishingState::SearchingForBobber);
            m_eyes->startLooking();
            // rather than calling setActualState here, the eyes
            // will set the state based on what it sees
            break;
        }
        case UF::FishingState::WaitingForFish:
        {
            if ((m_fishWaitTime += kActionTimerLength) >= 10)
            {
                setActualState(UF::FishingState::Idle);
                m_fishWaitTime = 0;
            }
            break;
        }
        default:
        {
            break;
        }
    };
}

void
UF::Manager::handleNeededAction(UF::NeededAction action)
{
    switch (action)
    {
        case UF::NeededAction::HearthStone:
        {
            // TODO: stop fishing
            m_hands->doAction(action);
        }
        case UF::NeededAction::Lure:
        case UF::NeededAction::Charm:
        case UF::NeededAction::Raft:
        case UF::NeededAction::Bait:
        {
            m_hands->doAction(action);
            break;
        }
        case UF::NeededAction::AntiAfkMove:
        {
            m_legs->doMovement();
            break;
        }
        default:
        {
            break;
        }
    };

    removeNeededAction(action);
}

void
UF::Manager::addNeededAction(UF::NeededAction action)
{
    // m_neededActions |= action;

    uint8_t neededActions = static_cast<uint8_t>(m_neededActions);
    uint8_t newAction     = static_cast<uint8_t>(action);
    uint8_t flags         = neededActions | newAction;

    m_neededActions       = static_cast<UF::NeededAction>(flags);
}

void
UF::Manager::removeNeededAction(UF::NeededAction action)
{
    // m_neededActions &= ~action;

    uint8_t neededActions = static_cast<uint8_t>(m_neededActions);
    uint8_t newAction     = static_cast<uint8_t>(action);
    uint8_t flags         = neededActions & ~newAction;

    m_neededActions       = static_cast<UF::NeededAction>(flags);
}

bool
UF::Manager::hasNeededAction(UF::NeededAction action) const
{
    // return (m_neededActions & action) != UF::NeededAction::None;

    uint8_t neededActions  = static_cast<uint8_t>(m_neededActions);
    uint8_t newAction      = static_cast<uint8_t>(action);
    uint8_t flags          = neededActions & newAction;
    UF::NeededAction final = static_cast<UF::NeededAction>(flags);

    return final != UF::NeededAction::None;
}

void
UF::Manager::lureTimerTick()
{
    addNeededAction(UF::NeededAction::Lure);
}

void
UF::Manager::raftTimerTick()
{
    addNeededAction(UF::NeededAction::Raft);
}

void
UF::Manager::charmTimerTick()
{
    addNeededAction(UF::NeededAction::Charm);
}

void
UF::Manager::baitTimerTick()
{
    addNeededAction(UF::NeededAction::Bait);
}

void
UF::Manager::hearthStoneTimerTick()
{
    addNeededAction(UF::NeededAction::HearthStone);
}

void
UF::Manager::antiAFKTimerTick()
{
    addNeededAction(UF::NeededAction::AntiAfkMove);
}
