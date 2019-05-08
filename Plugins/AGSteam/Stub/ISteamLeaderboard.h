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
// Copyright © 2014-2015 MonkeyMoto Productions, Inc.
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
#ifndef AGSTEAM_ISTEAMLEADERBOARD_H
#define AGSTEAM_ISTEAMLEADERBOARD_H

namespace AGSteam
{
class ISteamLeaderboard
{
public:
    virtual void FindLeaderboard(char const *leaderboardName) = 0;
    virtual bool UploadScore(int score) = 0;
    virtual bool DownloadScores(int type) = 0;
    virtual char const* GetCurrentLeaderboardName() = 0;
    virtual char const* GetLeaderName(int index) = 0;
    virtual int GetLeaderScore(int index) = 0;
    virtual int GetLeaderCount() = 0;
    virtual ~ISteamLeaderboard() = 0;
};

ISteamLeaderboard* GetSteamLeaderboard();
void SteamLeaderboard_FindLeaderboard(char const *leaderboardName);
int SteamLeaderboard_UploadScore(int score);
int SteamLeaderboard_DownloadScores(int type);
char const* SteamLeaderboard_GetCurrentLeaderboardName();
char const* SteamLeaderboard_GetLeaderName(int index);
int SteamLeaderboard_GetLeaderScore(int index);
int SteamLeaderboard_GetLeaderCount();

} // namespace AGSteam

#endif // AGSTEAM_ISTEAMLEADERBOARD_H
