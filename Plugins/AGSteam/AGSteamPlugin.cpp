#pragma region Defines_and_Includes

#if defined(WINDOWS_VERSION)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinBase.h>
#endif

#if !defined(BUILTIN_PLUGINS)
#define THIS_IS_THE_PLUGIN
#endif

#ifdef USE_STEAM
#include "steam/steam_api.h"
#endif USE_STEAM
#include "agsplugin.h"

#pragma endregion

//define engine

#if defined(BUILTIN_PLUGINS)
namespace ags_steam {
#endif
	
IAGSEngine *engine = 0;

#if defined(WINDOWS_VERSION) && !defined(BUILTIN_PLUGINS)
#endif

// ***** Run time *****

 // Engine interface

//------------------------------------------------------------------------------

#define REGISTER(x) engine->RegisterScriptFunction(#x, (void *) (x));

IAGSEngine* GetAGSEngine()
{
	return engine;
}

void Update()
{
#ifdef USE_STEAM
	SteamAPI_RunCallbacks();
#endif USE_STEAM
}

bool IsInitialized()
{
#ifdef USE_STEAM
	static bool steamInitialized = false;
	if (!steamInitialized)
	{
		steamInitialized = SteamAPI_Init();
		if (steamInitialized)
		{
			//SteamAchievements = new SteamAchievement;
			//SteamLeaderboards = new SteamLeaderboard;
			//SteamStats = new SteamStat;
			Update(); // when first initialized, ensure user achievement and stats are updated in-game immediately
		}
	}
	return steamInitialized;
#else
	return false;
#endif
}

//------------------------------------------------------------------------------

bool SetAchievementAchieved(char const *ID)
{
#ifdef USE_STEAM
	if (!IsInitialized()) 
		return false; // Steam not initialized or haven't received call back from Steam yet, so we can't set achievements

	int result =SteamUserStats()->SetAchievement(ID);
	SteamUserStats()->StoreStats();
	SteamAPI_RunCallbacks();
	return (result != 0);
#else
	return false;
#endif
}

void ResetStatsAndAchievements()
{
#ifdef USE_STEAM
	if (!IsInitialized()) return;
		SteamUserStats()->ResetAllStats(true);
#endif
}

char const* AGSteam_GetCurrentGameLanguage()
{
#ifdef USE_STEAM
	if (!IsInitialized()) return NULL;
	char const *language = SteamApps()->GetCurrentGameLanguage();
	return (language == NULL ? NULL : GetAGSEngine()->CreateScriptString(language));
#else
	return "English";
#endif
}

char const* GetUserName()
{
#ifdef USE_STEAM
	if (!IsInitialized()) return NULL;
	return GetAGSEngine()->CreateScriptString(SteamFriends()->GetPersonaName()); // GetPersonaName is GUARANTEED to not be NULL
#else
	return "UNKNOWN";
#endif
}

int AGSteam_IsSteamInitialized()
{
	return IsInitialized() != 0;
}

void Shutdown()
{
	/*delete SteamAchievements;
	SteamAchievements = NULL;
	delete SteamLeaderboards;
	SteamLeaderboards = NULL;
	delete SteamStats;
	SteamStats = NULL;*/
}

void AGS_EngineStartup(IAGSEngine *lpEngine)
{
	engine = lpEngine;

	IsInitialized(); // we don't care about the result, but try to initialize everything here if we can

	/*engine->RegisterScriptFunction("AGSteam::IsAchievementAchieved^1", reinterpret_cast<void*>(SteamAchievement_IsAchievementAchieved));*/
	engine->RegisterScriptFunction("AGSteam::SetAchievementAchieved^1", reinterpret_cast<void*>(SetAchievementAchieved));/*
	engine->RegisterScriptFunction("AGSteam::ResetAchievement^1", reinterpret_cast<void*>(SteamAchievement_ClearAchievement));*/
	engine->RegisterScriptFunction("AGSteam::ResetStatsAndAchievements^0", reinterpret_cast<void*>(ResetStatsAndAchievements));

	/*
	engine->RegisterScriptFunction("AGSteam::GetIntStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetIntStat));
	engine->RegisterScriptFunction("AGSteam::GetFloatStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetFloatStat));
	engine->RegisterScriptFunction("AGSteam::GetAverageRateStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetAvgRateStat));
	engine->RegisterScriptFunction("AGSteam::SetIntStat^2", reinterpret_cast<void*>(AGSteam::SteamStat_SetIntStat));
	engine->RegisterScriptFunction("AGSteam::SetFloatStat^2", reinterpret_cast<void*>(AGSteam::SteamStat_SetFloatStat));
	engine->RegisterScriptFunction("AGSteam::UpdateAverageRateStat^3", reinterpret_cast<void*>(AGSteam::SteamStat_UpdateAvgRateStat));
	engine->RegisterScriptFunction("AGSteam::ResetStats^0", reinterpret_cast<void*>(AGSteam::SteamStat_ResetStats));
	engine->RegisterScriptFunction("AGSteam::ResetStatsAndAchievements^0", reinterpret_cast<void*>(AGSteam::AGSteam_ResetStatsAndAchievements));*/
	engine->RegisterScriptFunction("AGSteam::get_Initialized", reinterpret_cast<void*>(AGSteam_IsSteamInitialized));
	/*engine->RegisterScriptFunction("AGSteam::GetGlobalIntStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetGlobalIntStat));
	engine->RegisterScriptFunction("AGSteam::GetGlobalFloatStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetGlobalFloatStat));
	engine->RegisterScriptFunction("AGSteam::get_CurrentLeaderboardName", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetCurrentLeaderboardName));
	engine->RegisterScriptFunction("AGSteam::FindLeaderboard^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_FindLeaderboard));
	engine->RegisterScriptFunction("AGSteam::UploadScore^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_UploadScore));
	engine->RegisterScriptFunction("AGSteam::DownloadScores^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_DownloadScores));
	engine->RegisterScriptFunction("AGSteam::geti_LeaderboardNames", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderName));
	engine->RegisterScriptFunction("AGSteam::geti_LeaderboardScores", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderScore));
	engine->RegisterScriptFunction("AGSteam::get_LeaderboardCount", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderCount));*/
	engine->RegisterScriptFunction("AGSteam::GetCurrentGameLanguage^0", reinterpret_cast<void*>(AGSteam_GetCurrentGameLanguage));
	//engine->RegisterScriptFunction("AGSteam::GetUserName^0", reinterpret_cast<void*>(AGSteam::AGSteam_GetUserName));*/
	engine->RequestEventHook(AGSE_FINALSCREENDRAW);
	engine->RequestEventHook(AGSE_KEYPRESS);
}


void AGS_EngineShutdown()
{
	// Called by the game engine just before it exits.
	// This gives you a chance to free any memory and do any cleanup
	// that you need to do before the engine shuts down.
}

void AGS_EngineInitGfx(const char *driverID, void *data)
{
}

//------------------------------------------------------------------------------

int AGS_EngineOnEvent(int event, int data)                    //*** optional ***
{
	/*switch (event)
	{

		case AGSE_KEYPRESS:
		case AGSE_MOUSECLICK:
		case AGSE_POSTSCREENDRAW:
		case AGSE_PRESCREENDRAW:
		case AGSE_SAVEGAME:
		case AGSE_RESTOREGAME:
		case AGSE_PREGUIDRAW:
		case AGSE_LEAVEROOM:
		case AGSE_ENTERROOM:
		case AGSE_TRANSITIONIN:
		case AGSE_TRANSITIONOUT:
		case AGSE_FINALSCREENDRAW:
		case AGSE_TRANSLATETEXT:
		case AGSE_SCRIPTDEBUG:
		case AGSE_SPRITELOAD:
		case AGSE_PRERENDER:
		case AGSE_PRESAVEGAME:
		case AGSE_POSTRESTOREGAME:

		default:
			break;
	}*/
	
	// Return 1 to stop event from processing further (when needed)
	return (0);
}
#if defined(BUILTIN_PLUGINS)
}
#endif