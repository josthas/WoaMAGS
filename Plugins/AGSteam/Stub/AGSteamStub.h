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
#ifndef AGSTEAM_AGSTEAMSTUB_H
#define AGSTEAM_AGSTEAMSTUB_H

#include "IAGSteam.h"

namespace AGSteam
{
namespace Stub
{

class AGSteamStub : public IAGSteam
{
public:
    bool IsInitialized()
    {
        return false;
    }

    void ResetStatsAndAchievements()
    {
    }

    char const* GetCurrentGameLanguage()
    {
        return 0U;
    }

    char const* GetUserName()
    {
        return 0U;
    }

    void Shutdown()
    {
    }

    void Update()
    {
    }

    char const* GetAGSScriptHeader()
    {
        return "#define AGSteam_VERSION 3.1\r\n"
            "\r\n"
            "enum AGSteamStatType\r\n"
            "{\r\n"
            "  eAGSteamStatInt = 0,\r\n"
            "  eAGSteamStatFloat = 1,\r\n"
            "  eAGSteamStatAverageRate = 2\r\n"
            "};\r\n"
            "\r\n"
            "enum AGSteamScoresRequestType\r\n"
            "{\r\n"
            "  eAGSteamScoresRequestGlobal = 0,\r\n"
            "  eAGSteamScoresRequestAroundUser = 1,\r\n"
            "  eAGSteamScoresRequestFriends = 2\r\n"
            "};\r\n"
            "\r\n"
            "managed struct AGSteam\r\n"
            "{\r\n"
            "  ///AGSteam: Returns whether the specified Steam achievement has been achieved\r\n"
            "  import static int IsAchievementAchieved(const string steamAchievementID);\r\n"
            "  ///AGSteam: Sets a Steam achievement as achieved\r\n"
            "  import static int SetAchievementAchieved(const string steamAchievementID);\r\n"
            "  ///AGSteam: Resets the specified Steam achievement, so it can be achieved again\r\n"
            "  import static int ResetAchievement(const string steamAchievementID);\r\n"
            "  ///AGSteam: Sets the value of a Steam INT stat\r\n"
            "  import static int SetIntStat(const string steamStatName, int value);\r\n"
            "  ///AGSteam: Sets the value of a Steam FLOAT stat\r\n"
            "  import static int SetFloatStat(const string steamStatName, float value);\r\n"
            "  ///AGSteam: Updates a Steam AVGRATE stat\r\n"
            "  import static int UpdateAverageRateStat(const string steamStatName, float numerator, float denominator);\r\n"
            "  ///AGSteam: Returns the value of a Steam INT stat\r\n"
            "  import static int GetIntStat(const string steamStatName);\r\n"
            "  ///AGSteam: Returns the value of a Steam FLOAT stat\r\n"
            "  import static float GetFloatStat(const string steamStatName);\r\n"
            "  ///AGSteam: Returns the value of a Steam AVGRATE stat\r\n"
            "  import static float GetAverageRateStat(const string steamStatName);\r\n"
            "  ///AGSteam: Resets all Steam stats to their default values\r\n"
            "  import static void ResetStats();\r\n"
            "  ///AGSteam: Resets all Steam stats and achievements\r\n"
            "  import static void ResetStatsAndAchievements();\r\n"
            "  ///AGSteam: Returns whether the Steam client is loaded and the user logged in\r\n"
            "  readonly import static attribute int Initialized;\r\n"
            "  ///AGSteam: Returns the value of a global Steam INT stat\r\n"
            "  import static int GetGlobalIntStat(const string steamStatName);\r\n"
            "  ///AGSteam: Returns the value of a global Steam FLOAT stat\r\n"
            "  import static float GetGlobalFloatStat(const string steamStatName);\r\n"
            "  ///AGSteam: Returns the name of the current leaderboard (call FindLeadboard first)\r\n"
            "  readonly import static attribute String CurrentLeaderboardName;\r\n"
            "  ///AGSteam: Requests to load the specified Steam leaderboard. This call is asynchronous and does not return the data immediately, check for results in repeatedly_execute.\r\n"
            "  import static void FindLeaderboard(const string leaderboardName);\r\n"
            "  ///AGSteam: Uploads the score to the current Steam leaderboard. Returns false if an error occurred.\r\n"
            "  import static int UploadScore(int score);\r\n"
            "  ///AGSteam: Downloads a list of ten scores from the current Steam leaderboard.\r\n"
            "  import static int DownloadScores(AGSteamScoresRequestType);\r\n"
            "  ///AGSteam: Returns the name associated with a downloaded score. Call DownloadScores first.\r\n"
            "  readonly import static attribute String LeaderboardNames[];\r\n"
            "  ///AGSteam: Returns a downloaded score. Call DownloadScores first.\r\n"
            "  readonly import static attribute int LeaderboardScores[];\r\n"
            "  ///AGSteam: Returns the number of downloaded scores (if any). Call DownloadScores first. Max is 10 scores.\r\n"
            "  readonly import static attribute int LeaderboardCount;\r\n"
            "  ///AGSteam: Returns the current game language as registered by the Steam client.\r\n"
            "  import static String GetCurrentGameLanguage();\r\n"
            "  ///AGSteam: Returns the Steam user's username.\r\n"
            "  import static String GetUserName();\r\n"
            "};\r\n"
            "\r\n";
    }

    char const* GetAGSPluginName()
    {
        return "AGSteam Stub";
    }

    char const* GetAGSPluginDesc()
    {
        return "AGSteam Stub: Steam Plugin Stub for AGS (C) 2011-2014 MonkeyMoto Productions, Inc.";
    }

    ~AGSteamStub()
    {
    }
};

AGSteamStub& GetAGSteamStub();

} // namespace Stub
} // namespace AGSteam

#endif // AGSTEAM_AGSTEAMSTUB_H
