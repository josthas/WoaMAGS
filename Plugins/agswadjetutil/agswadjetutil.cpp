/*

Utility functions for Wadjet eye games

*/
//#include <string>

//using namespace std;


#if defined(WINDOWS_VERSION)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable : 4244)
#endif

#if !defined(BUILTIN_PLUGINS)
#define THIS_IS_THE_PLUGIN
#endif

#include "plugin/agsplugin.h"

#if defined(BUILTIN_PLUGINS)
namespace agswadjetutil {
#endif

IAGSEngine* engine;

#if defined (IOS_VERSION) || defined (ANDROID_VERSION)
#define MOBILE_VERSION
#endif

extern "C"
{
  void simulate_keypress(int keypress);
  bool isPhone();

#ifdef MOBILE_VERSION
  int MobileGetAchievement(char* name);
  void MobileSetAchievement(char* name, int value);
  void MobileShowAchievements();
  void MobileResetAchievements();
#endif
}
#if defined(BUILTIN_PLUGINS)
#include "main/main.h"
#endif
// ********************************************
// ************  AGS Interface  ***************
// ********************************************

// Phone or pad?
bool IsOnPhone()
{
#if defined(IOS_VERSION)
    return isPhone();
#elif defined (ANDROID_VERSION)
    return android_is_phone;
#endif
	return false;
}

void FakeKeypress(int keypress)
{
#if defined(BUILTIN_PLUGINS)
	simulate_keypress(keypress);
#endif
}

void SetAchievement(char * name, int value)
{
#ifdef MOBILE_VERSION
	MobileSetAchievement(name, value);
#endif
/*#if defined(IOS_VERSION)
  SetAchievementValue(name, value);
#endif*/
}


int GetAchievement(char * name)
{
#ifdef MOBILE_VERSION
	return MobileGetAchievement(name);
#endif
/*#if defined(IOS_VERSION)
  return GetAchievementValue(name);
#endif*/
  return -1;
}

void ShowAchievements()
{
#ifdef MOBILE_VERSION
	MobileShowAchievements();
#endif
/*#if defined(IOS_VERSION)
  //ShowAchievements(); doesn't work
#endif*/
}

void ResetAchievements()
{
#ifdef MOBILE_VERSION
	MobileResetAchievements();
#endif
/*#if defined(IOS_VERSION)
    ResetAchievements();
#endif*/
}

void AGS_EngineStartup(IAGSEngine *lpEngine)
{
	lpEngine->RegisterScriptFunction("FakeKeypress", (void*)&FakeKeypress);
	lpEngine->RegisterScriptFunction("IsOnPhone", (void*)&IsOnPhone);
	lpEngine->RegisterScriptFunction("MobileGetAchievement", (int*)&GetAchievement);
	lpEngine->RegisterScriptFunction("MobileSetAchievement", (void*)&SetAchievement);
	lpEngine->RegisterScriptFunction("MobileShowAchievements", (void*)&ShowAchievements);
	lpEngine->RegisterScriptFunction("MobileResetAchievements", (void*)&ResetAchievements);
}

void AGS_EngineShutdown()
{

}

int AGS_EngineOnEvent(int event, int data)
{
  return 0;
}

int AGS_EngineDebugHook(const char *scriptName, int lineNum, int reserved)
{
  return 0;
}

void AGS_EngineInitGfx(const char *driverID, void *data)
{
}



#if defined(WINDOWS_VERSION) && !defined(BUILTIN_PLUGINS)

// ********************************************
// ***********  Editor Interface  *************
// ********************************************
    
    
const char* scriptHeader =
  "import void FakeKeypress(int);\r\n"
  "import bool IsOnPhone();\r\n"
  "import int MobileGetAchievement(String);\r\n"
  "import bool MobileSetAchievement(String, int);\r\n"
  "import void MobileShowAchievements();\r\n"
  "import void MobileResetAchievements();\r\n"
  ;


IAGSEditor* editor;


LPCSTR AGS_GetPluginName(void)
{
  // Return the plugin description
  return "Wadjet eye utilities";
}

int  AGS_EditorStartup(IAGSEditor* lpEditor)
{
  // User has checked the plugin to use it in their game

  // If it's an earlier version than what we need, abort.
  if (lpEditor->version < 1)
    return -1;

  editor = lpEditor;
  editor->RegisterScriptHeader(scriptHeader);

  // Return 0 to indicate success
  return 0;
}

void AGS_EditorShutdown()
{
  // User has un-checked the plugin from their game
  editor->UnregisterScriptHeader(scriptHeader);
}

void AGS_EditorProperties(HWND parent)
{
  // User has chosen to view the Properties of the plugin
  // We could load up an options dialog or something here instead
  MessageBoxA(parent, "Wadjet eye utilities plugin by Janet Gilbert", "About", MB_OK | MB_ICONINFORMATION);
}

int AGS_EditorSaveGame(char* buffer, int bufsize)
{
  // We don't want to save any persistent data
  return 0;
}

void AGS_EditorLoadGame(char* buffer, int bufsize)
{
  // Nothing to load for this plugin
}

#endif


#if defined(BUILTIN_PLUGINS)
}
#endif
