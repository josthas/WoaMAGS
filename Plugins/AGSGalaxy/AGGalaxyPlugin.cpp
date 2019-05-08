#pragma region Defines_and_Includes

#if defined(WINDOWS_VERSION)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WinBase.h>
#endif

#if !defined(BUILTIN_PLUGINS)
#define THIS_IS_THE_PLUGIN
#endif
#include "agsplugin.h"
#ifdef USE_GOG
#ifdef MAC_VERSION
#include "GalaxyApi.h"
#else
#include <GalaxyApi.h>
#endif
#endif


#ifndef CPP11FIX_H
#define CPP11FIX_H

#if __cplusplus <= 199711L // NOT C++11

#define constexpr
#define noexcept
#define nullptr 0
#define STD_INT32_T int

//#define USE_GOG

#else // C++11

#include <cstdint>

#define STD_INT32_T std::int32_t

#endif // C++11

#endif // CPP11FIX_H

#pragma endregion

//define engine

#if defined(BUILTIN_PLUGINS)
namespace ags_galaxy {
#endif
  
  IAGSEngine *engine = 0;
  
#if defined(WINDOWS_VERSION) && !defined(BUILTIN_PLUGINS)
#endif
  
  // ***** Run time *****
  
  // Engine interface
  
  //------------------------------------------------------------------------------
  
#define REGISTER(x) engine->RegisterScriptFunction(#x, (void *) (x));
  
  bool bInitialized = false;
  
  IAGSEngine* GetAGSEngine()
  {
    return engine;
  }
  
  void Update()
  {
#ifdef USE_GOG
    try
    {
      galaxy::api::ProcessData();
    }
    catch (...)
    {
    }
#endif
  }
  
  bool IsInitialized()
  {
#ifdef USE_GOG
    return true;
#else
    return false;
#endif
  }
  
  //------------------------------------------------------------------------------
  
  // GOG API helper for user sign-in callbacks
#ifdef USE_GOG
  class AuthListener : public galaxy::api::GlobalAuthListener
  {
  public:
    virtual void OnAuthSuccess() override
    {
      try
      {
        if(galaxy::api::Stats() == NULL)
        {
          return;
        }
        galaxy::api::Stats()->RequestUserStatsAndAchievements();
        bInitialized = true;
      }
      catch (...)
      {
        
      }
    }
    
    virtual void OnAuthFailure(FailureReason reason) override
    {
      bInitialized = false;
    }
    virtual void OnAuthLost() override
    {
      bInitialized = false;
    }
  };
  
  AuthListener* pAuthlistener = NULL;
  
#endif
  
  bool SetAchievementAchieved(char const *ID)
  {
#ifdef USE_GOG
    if(bInitialized)
    {
      try
      {
        if (galaxy::api::Stats() == NULL)
        {
          return false;
        }
        galaxy::api::Stats()->SetAchievement(ID);
        galaxy::api::Stats()->StoreStatsAndAchievements();
        return true;
      }
      catch (...)
      {
        
      }
    }
#endif
    
    return false;
  }
  
  void ResetStatsAndAchievements()
  {
#ifdef USE_GOG
    if(bInitialized)
    {
      try
      {
        if (galaxy::api::Stats() == NULL)
        {
          return;
        }
        galaxy::api::Stats()->ResetStatsAndAchievements();
        // per API docs, this stores changes automatically
      }
      catch (...)
      {
      }
    }
#endif
  }
  
  char const* AGGalaxy_GetCurrentGameLanguage()
  {
#ifdef USE_GOG
    if (!IsInitialized()) return NULL;
    char const *language = "English";//SteamApps()->GetCurrentGameLanguage();
    return (language == NULL ? NULL : GetAGSEngine()->CreateScriptString(language));
#else
    return "English";
#endif
  }
  
  char const* GetUserName()
  {
    return "UNKNOWN";
  }
  
  // Initialize GOG API, sign in the user, and request user stats+achievements (called from user scripts with clientID and clientSecret)
  int AGSGalaxy_Initialize(char const *clientID, char const *clientSecret)
  {
#ifdef USE_GOG
    try
    {
	  galaxy::api::Init(galaxy::api::InitOptions(clientID, clientSecret));
    }
    catch (...)
    {
      return false;
    }
    
    try
    {
      // these CANNOT appear before the API is initialized! d'oh!!!!
      pAuthlistener = new AuthListener();
      
      if (galaxy::api::User() == NULL)
      {
        return false;
      }
      // API is initialized, sign user in (async, see callback classes above)
      
      galaxy::api::User()->SignInGalaxy();
      return true;
    }
    catch(...)
    {
      
    }
#endif
    
    return false;
  }
  
  int AGGalaxy_IsInitialized()
  {
    return IsInitialized() != 0;
  }
  
  void Shutdown()
  {
#ifdef USE_GOG
    if(pAuthlistener != NULL)
    {
      delete pAuthlistener;
      pAuthlistener = NULL;
    }
    try
    {
      galaxy::api::Shutdown();
    }
    catch (...)
    {
    }
#endif
  }
  
  void AGS_EngineStartup(IAGSEngine *lpEngine)
  {
    engine = lpEngine;
    
    IsInitialized(); // we don't care about the result, but try to initialize everything here if we can
    
    /*engine->RegisterScriptFunction("AGSteam::IsAchievementAchieved^1", reinterpret_cast<void*>(SteamAchievement_IsAchievementAchieved));*/
    engine->RegisterScriptFunction("AGS2Client::SetAchievementAchieved^1", reinterpret_cast<void*>(SetAchievementAchieved));/*
                                                                                                                             engine->RegisterScriptFunction("AGSteam::ResetAchievement^1", reinterpret_cast<void*>(SteamAchievement_ClearAchievement));*/
    engine->RegisterScriptFunction("AGS2Client::ResetStatsAndAchievements^0", reinterpret_cast<void*>(ResetStatsAndAchievements));
    engine->RegisterScriptFunction("AGS2Client::get_Initialized", reinterpret_cast<void*>(AGGalaxy_IsInitialized));
    engine->RegisterScriptFunction("AGS2Client::Initialize^2", reinterpret_cast<void*>(AGSGalaxy_Initialize));
    /*engine->RegisterScriptFunction("AGSteam::GetGlobalIntStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetGlobalIntStat));
     engine->RegisterScriptFunction("AGSteam::GetGlobalFloatStat^1", reinterpret_cast<void*>(AGSteam::SteamStat_GetGlobalFloatStat));
     engine->RegisterScriptFunction("AGSteam::get_CurrentLeaderboardName", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetCurrentLeaderboardName));
     engine->RegisterScriptFunction("AGSteam::FindLeaderboard^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_FindLeaderboard));
     engine->RegisterScriptFunction("AGSteam::UploadScore^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_UploadScore));
     engine->RegisterScriptFunction("AGSteam::DownloadScores^1", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_DownloadScores));
     engine->RegisterScriptFunction("AGSteam::geti_LeaderboardNames", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderName));
     engine->RegisterScriptFunction("AGSteam::geti_LeaderboardScores", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderScore));
     engine->RegisterScriptFunction("AGSteam::get_LeaderboardCount", reinterpret_cast<void*>(AGSteam::SteamLeaderboard_GetLeaderCount));*/
    engine->RegisterScriptFunction("AGGalaxy::GetCurrentGameLanguage^0", reinterpret_cast<void*>(AGGalaxy_GetCurrentGameLanguage));
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
#ifdef USE_GOG
    if(event== AGSE_FINALSCREENDRAW)
    {
      try
      {
        Update();
      }
      catch(...)
      {
        
      }
    }
#endif
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