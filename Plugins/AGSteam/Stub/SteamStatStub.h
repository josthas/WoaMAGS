// AGSteamStub
// Plugin stub for the Steam plugin for AGS
//
// This stub is a non-functional replacement for the AGSteam plugin. The
// full plugin cannot be made open-source due to restrictions enforced
// by Valve's strict licensing on the Steamworks API. If you are a
// developer who has signed an NDA with Valve and have access to
// Steamworks, access can be granted to the full plugin source. For more
// information you may contact monkey_05_06 on the AGS forums at:
// <http://www.adventuregamestudio.co.uk/forums/index.php?action=profile&u=2015>
//
// Copyright © 2014 MonkeyMoto Productions, Inc.
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the LICENSE file for more details.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
//
// Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
//
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//  0. You just DO WHAT THE FUCK YOU WANT TO.
//
#ifndef AGSTEAM_STEAMSTATSTUB_H
#define AGSTEAM_STEAMSTATSTUB_H

#include "ISteamStat.h"

namespace AGSteam
{
namespace Stub
{

class SteamStatStub : public ISteamStat
{
public:
    int GetIntStat(char const *)
    {
        return 0;
    }

    float GetFloatStat(char const*)
    {
        return 0.0f;
    }

    float GetAvgRateStat(char const*)
    {
        return 0.0f;
    }

    bool SetIntStat(char const*, int)
    {
        return false;
    }

    bool SetFloatStat(char const*, float)
    {
        return false;
    }

    bool UpdateAvgRateStat(char const*, float, float)
    {
        return false;
    }

    int GetGlobalIntStat(char const*)
    {
        return 0;
    }

    float GetGlobalFloatStat(char const*)
    {
        return 0.0f;
    }

    void ResetStats()
    {
    }

    ~SteamStatStub()
    {
    }
};

SteamStatStub& GetSteamStatStub();

} // namespace Stub
} // namespace AGSteam

#endif // AGSTEAM_STEAMSTATSTUB_H
