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
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // WIN32_LEAN_AND_MEAN
#elif defined(__linux__) // !_WIN32
#define __stdcall
#endif // _WIN32, __linux__

#define THIS_IS_THE_PLUGIN
#include "agsplugin.h"
#include "IAGSteam.h"
#include "ISteamAchievement.h"
#include "ISteamLeaderboard.h"
#include "ISteamStat.h"

// ***** DESIGN TIME CALLS *******

#ifdef WINDOWS_VERSION

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    // this is the WINDOWS entry point into the DLL
    return TRUE;
}

IAGSEditor *editor;

char const *ourScriptHeader;

LPCSTR AGS_GetPluginName()
{
	return AGSteam::GetAGSteam()->GetAGSPluginName();
}

int  AGS_EditorStartup(IAGSEditor *lpEditor)
{
    // User has checked the plugin to use it in their game

    // If it's an earlier version than what we need, abort.
    if (lpEditor->version < 1) return -1;

    editor = lpEditor;
    ourScriptHeader = AGSteam::GetAGSteam()->GetAGSScriptHeader();
    if (ourScriptHeader != NULL) editor->RegisterScriptHeader(ourScriptHeader);

    // Return 0 to indicate success
    return 0;
}

void AGS_EditorShutdown()
{
    // User has un-checked the plugin from their game
    if (ourScriptHeader != NULL) editor->UnregisterScriptHeader(ourScriptHeader);
}

void AGS_EditorProperties(HWND parent)
{
    // User has chosen to view the Properties of the plugin
    // We could load up an options dialog or something here instead
    MessageBox(parent, AGSteam::GetAGSteam()->GetAGSPluginDesc(), "About", MB_OK | MB_ICONINFORMATION);
}

int AGS_EditorSaveGame(char *buffer, int bufsize)
{
    // We don't want to save any persistent data
    return 0;
}

void AGS_EditorLoadGame(char *buffer, int bufsize)
{
    // Nothing to load for this plugin
}

#endif // WINDOWS_VERSION

// ******* END DESIGN TIME  *******


// ****** RUN TIME ********

IAGSEngine *engine;

IAGSEngine* GetAGSEngine()
{
    return engine;
}

void AGS_EngineStartup(IAGSEngine *lpEngine)
{
    engine = lpEngine;

    if (engine->version < 17)
    {
        engine->AbortGame("Engine interface is too old, need newer version of AGS.");
    }

    AGSteam::GetAGSteam()->IsInitialized(); // we don't care about the result, but try to initialize everything here if we can

    engine->RegisterScriptFunction("AGSteam::IsAchievementAchieved^1", reinterpret_cast<void*>(AGSteam::SteamAchievement_IsAchievementAchieved));
    engine->RegisterScriptFunction("AGSteam::SetAchievementAchieved^1", reinterpret_cast<void*>(AGSteam::SteamAchievement_SetAchievementAchieved));
    engine->RegisterScriptFunction("AGSteam::ResetAchievement^1", reinterpret_cast<void*>(AGSteam::SteamAchievement_ClearAchievement));
    engine->RegisterScriptFunction("AGSteam::GetIntStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetIntStat));
    engine->RegisterScriptFunction("AGSteam::GetFloatStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetFloatStat));
    engine->RegisterScriptFunction("AGSteam::GetAverageRateStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetAvgRateStat));
    engine->RegisterScriptFunction("AGSteam::SetIntStat^2", reinterpret_cast<void*>(AGSteam::SteamStat_SetIntStat));
    engine->RegisterScriptFunction("AGSteam::SetFloatStat^2", reinterpret_cast<void*>(AGSteam::SteamStat_SetFloatStat));
    engine->RegisterScriptFunction("AGSteam::UpdateAverageRateStat^3", reinterpret_cast<void*>(AGSteam::SteamStat_UpdateAvgRateStat));
    engine->RegisterScriptFunction("AGSteam::ResetStats^0", reinterpret_cast<void*>(AGSteam::SteamStat_ResetStats));
	engine->RegisterScriptFunction("AGSteam::ResetStatsAndAchievements^0", reinterpret_cast<void*>(AGSteam::AGSteam_ResetStatsAndAchievements));
    engine->RegisterScriptFunction("AGSteam::get_Initialized", reinterpret_cast<void*>(AGSteam::AGSteam_IsSteamInitialized));
    engine->RegisterScriptFunction("AGSteam::GetGlobalIntStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetGlobalIntStat));
    engine->RegisterScriptFunction("AGSteam::GetGlobalFloatStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetGlobalFloatStat));
    engine->RegisterScriptFunction("AGSteam::get_CurrentLeaderboardName", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetCurrentLeaderboardName));
    engine->RegisterScriptFunction("AGSteam::FindLeaderboard^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_FindLeaderboard));
    engine->RegisterScriptFunction("AGSteam::UploadScore^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_UploadScore));
    engine->RegisterScriptFunction("AGSteam::DownloadScores^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_DownloadScores));
    engine->RegisterScriptFunction("AGSteam::geti_LeaderboardNames", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderName));
    engine->RegisterScriptFunction("AGSteam::geti_LeaderboardScores", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderScore));
    engine->RegisterScriptFunction("AGSteam::get_LeaderboardCount", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderCount));
    engine->RegisterScriptFunction("AGSteam::GetCurrentGameLanguage^0", reinterpret_cast<void*>(AGSteam::AGSteam_GetCurrentGameLanguage));
    engine->RegisterScriptFunction("AGSteam::GetUserName^0", reinterpret_cast<void*>(AGSteam::AGSteam_GetUserName));
    engine->RequestEventHook(AGSE_FINALSCREENDRAW);
    engine->RequestEventHook(AGSE_KEYPRESS);
}

void AGS_EngineShutdown()
{
    AGSteam::GetAGSteam()->Shutdown();
}

int AGS_EngineOnEvent(int event, int data)
{
    if (event == AGSE_FINALSCREENDRAW)
    {
        // use this as main_game_loop
        // the screen is drawn every actual game loop anyway
        AGSteam::GetAGSteam()->Update();
    }
    else if (event == AGSE_KEYPRESS)
    {
        typedef int (*IsKeyPressed_t)(int);
        IsKeyPressed_t IsKeyPressed = reinterpret_cast<IsKeyPressed_t>(engine->GetScriptFunctionAddress("IsKeyPressed"));
        bool isShift = ((data == 403) || (data == 404)); // is pressed key shift
        bool isTab = (data == 9); // is pressed key tab
        bool isShiftTab = ((data == 0x00001111) || // shift+tab as a single key
            ((isShift) && (IsKeyPressed(9) != 0)) || // key is shift and tab is held
            ((isTab) && ((IsKeyPressed(403) != 0) || (IsKeyPressed(404) != 0)))); // key is tab and shift is held
        return isShiftTab; // Claim (Shift+Tab), ignore other keys
    }
    return 0;
}

// *** END RUN TIME ****
