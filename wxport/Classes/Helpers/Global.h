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

#ifndef __ULTIMATEFISHBOT_GLOBAL_H__
#define __ULTIMATEFISHBOT_GLOBAL_H__

#include <cstdint>
#include <list>
#include <numeric>
#include <vector>

// constants for timers
static const int32_t kSecond               = 1000;
static const int32_t kMinute               = 60 * kSecond;
static const int32_t kActionTimerLength    = 500;

// ms pause between sound checks
static const int32_t kSoundTickRate        = 100;

// max length of the volume queue
static const int32_t kMaxVolumeQueueLength = 5;

namespace UF
{
    namespace Helpers
    {
        template<class C>
        inline void safeDelete(C c)
        {
            if (c)
            {
                delete c;
                c = nullptr;
            }
        }
    }; // namespace Helpers

    enum class FishingState
    {
        Idle                = 0,
        Start               = 1,
        Casting             = 2,
        SearchingForBobber  = 3,
        WaitingForFish      = 4,
        Looting             = 5,
        Paused              = 6,
        Stopped             = 7
    };

    enum class NeededAction : uint8_t
    {
        None        = 0x00,
        HearthStone = 0x01,
        Lure        = 0x02,
        Charm       = 0x04,
        Raft        = 0x08,
        Bait        = 0x10,
        AntiAfkMove = 0x20
    };

    static const NeededAction kAllNeededActions[] = {
        UF::NeededAction::None,
        UF::NeededAction::HearthStone,
        UF::NeededAction::Lure,
        UF::NeededAction::Charm,
        UF::NeededAction::Raft,
        UF::NeededAction::Bait,
        UF::NeededAction::AntiAfkMove
    };

    struct FishingStats
    {
        int32_t totalSuccessFishing;
        int32_t totalNotFoundFish;
        int32_t totalNotEaredFish;

        void reset()
        {
            totalSuccessFishing = 0;
            totalNotFoundFish   = 0;
            totalNotEaredFish   = 0;
        }

        int32_t total() const
        {
            return (
                totalSuccessFishing
                + totalNotFoundFish
                + totalNotEaredFish
            );
        }
    };
}; // namespace UF

// helper macros for variadic macro overloading
#define VA_HELPER_EXPAND(_X)                    _X  // workaround for Visual Studio
#define VA_COUNT_HELPER(_1, _2, _3, _4, _5, _6, _Count, ...) _Count
#define VA_COUNT(...)                           VA_HELPER_EXPAND(VA_COUNT_HELPER(__VA_ARGS__, 6, 5, 4, 3, 2, 1))
#define VA_SELECT_CAT(_Name, _Count, ...)       VA_HELPER_EXPAND(_Name##_Count(__VA_ARGS__))
#define VA_SELECT_HELPER(_Name, _Count, ...)    VA_SELECT_CAT(_Name, _Count, __VA_ARGS__)
#define VA_SELECT(_Name, ...)                   VA_SELECT_HELPER(_Name, VA_COUNT(__VA_ARGS__), __VA_ARGS__)

// overloads for NAMESPACE_BEGIN
#define UF_NAMESPACE_BEGIN_HELPER1(_Ns1)             namespace _Ns1 {
#define UF_NAMESPACE_BEGIN_HELPER2(_Ns1, _Ns2)       namespace _Ns1 { UF_NAMESPACE_BEGIN_HELPER1(_Ns2)
#define UF_NAMESPACE_BEGIN_HELPER3(_Ns1, _Ns2, _Ns3) namespace _Ns1 { UF_NAMESPACE_BEGIN_HELPER2(_Ns2, _Ns3)

// overloads for UF_NAMESPACE_END
#define UF_NAMESPACE_END_HELPER1(_Ns1)               }
#define UF_NAMESPACE_END_HELPER2(_Ns1, _Ns2)         } UF_NAMESPACE_END_HELPER1(_Ns2)
#define UF_NAMESPACE_END_HELPER3(_Ns1, _Ns2, _Ns3)   } UF_NAMESPACE_END_HELPER2(_Ns2, _Ns3)

// final macros
#define UF_NAMESPACE_BEGIN(_Namespace, ...)    VA_SELECT(UF_NAMESPACE_BEGIN_HELPER, _Namespace, __VA_ARGS__)
#define UF_NAMESPACE_END(_Namespace, ...)      VA_SELECT(UF_NAMESPACE_END_HELPER,   _Namespace, __VA_ARGS__)

#endif // __ULTIMATEFISHBOT_GLOBAL_H__