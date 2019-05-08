//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================

#include <string.h>
#include "ac/global_plugin.h"
#include "ac/mouse.h"

int pluginSimulatedClick = NONE;
extern bool android_is_phone;

void PluginSimulateMouseClick(int pluginButtonID) {
    pluginSimulatedClick = pluginButtonID - 1;
}

//=============================================================================
// Stubs for plugin functions.

void ScriptStub_ShellExecute()
{
}
void srSetSnowDriftRange(int min_value, int max_value)
{
}
void srSetSnowDriftSpeed(int min_value, int max_value)
{
}
void srSetSnowFallSpeed(int min_value, int max_value)
{
}
void srChangeSnowAmount(int amount)
{
}
void srSetSnowBaseline(int top, int bottom)
{
}
void srSetSnowTransparency(int min_value, int max_value)
{
}
void srSetSnowDefaultView(int view, int loop)
{
}
void srSetSnowWindSpeed(int value)
{
}
void srSetSnowAmount(int amount)
{
}
void srSetSnowView(int kind_id, int event, int view, int loop)
{
}
void srChangeRainAmount(int amount)
{
}
void srSetRainView(int kind_id, int event, int view, int loop)
{
}
void srSetRainDefaultView(int view, int loop)
{
}
void srSetRainTransparency(int min_value, int max_value)
{
}
void srSetRainWindSpeed(int value)
{
}
void srSetRainBaseline(int top, int bottom)
{
}
void srSetRainAmount(int amount)
{
}
void srSetRainFallSpeed(int min_value, int max_value)
{
}
void srSetWindSpeed(int value)
{
}
void srSetBaseline(int top, int bottom)
{
}
int JoystickCount()
{
    return 0;
}
int Joystick_Open(int a)
{
    return 0;
}
int Joystick_IsButtonDown(int a)
{
    return 0;
}
void Joystick_EnableEvents(int a)
{
}
void Joystick_DisableEvents()
{
}
void Joystick_Click(int a)
{
}
int Joystick_Valid()
{
    return 0;
}
int Joystick_Unplugged()
{
    return 0;
}
int DrawAlpha(int destination, int sprite, int x, int y, int transparency)
{
    return 0;
}
int GetAlpha(int sprite, int x, int y)
{
    return 0;
}
int PutAlpha(int sprite, int x, int y, int alpha)
{
    return 0;
}
int Blur(int sprite, int radius)
{
    return 0;
}
int HighPass(int sprite, int threshold)
{
    return 0;
}
int DrawAdd(int destination, int sprite, int x, int y, float scale)
{
    return 0;
}

int GetFlashlightInt()
{
    return 0;
}
void SetFlashlightInt1(int Param1)
{
}
void SetFlashlightInt2(int Param1, int Param2)
{
}
void SetFlashlightInt3(int Param1, int Param2, int Param3)
{
}
void SetFlashlightInt5(int Param1, int Param2, int Param3, int Param4, int Param5)
{
}


#include <assert.h>
#if defined(ANDROID_VERSION)
#include <android/log.h>
#endif

bool wjuIsOnPhone()
{
#if defined (ANDROID_VERSION)
    //__android_log_print(ANDROID_LOG_DEBUG, "ACHIEVEMENTS", "COME ON FFS OK1");
  return android_is_phone;
#endif
  return false;
}

extern "C"
{
	void simulate_keypress(int keypress);
}

void wjuFakeKeypress(int keypress)
{
	simulate_keypress(keypress);
}

#ifdef ANDROID_VERSION
extern "C"
{
	extern int AndroidGetAchievement(char* name);
	extern void AndroidSetAchievement(char* name, int value);
	extern void AndroidShowAchievements();
	extern void AndroidResetAchievements();
};
#endif

void wjuSetAchievement(char* name, int value)
{
#ifdef ANDROID_VERSION
    AndroidSetAchievement(name, value);
#endif
}
int wjuGetAchievement(char* name)
{
#ifdef ANDROID_VERSION
  return AndroidGetAchievement(name);
#endif
  return -1;
}
void wjuShowAchievements()
{
#ifdef ANDROID_VERSION
  return AndroidShowAchievements();
#endif
}
void wjuResetAchievements()
{
#ifdef ANDROID_VERSION
	AndroidResetAchievements();
#endif
}

void SetSpriteFont(int fontNum, int sprite, int rows, int columns, int charWidth, int charHeight, int charMin, int charMax, bool use32bit)
{

}

void SetVariableSpriteFont(int fontNum, int sprite)
{

}

void SetGlyph(int fontNum, int charNum, int x, int y, int width, int height)
{

}

void SetSpacing(int fontNum, int spacing)
{

}

void SetLineHeightAdjust(int fontNum, int LineHeight, int SpacingHeight, int SpacingOverride)
{

}

//=============================================================================
//
// Script API Functions
//
//=============================================================================

#include "debug/out.h"
#include "script/script_api.h"
#include "script/script_runtime.h"

// void ()
RuntimeScriptValue Sc_ScriptStub_ShellExecute(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID(ScriptStub_ShellExecute);
}

// void (int min_value, int max_value)
RuntimeScriptValue Sc_srSetSnowDriftRange(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetSnowDriftRange);
}

// void (int min_value, int max_value)
RuntimeScriptValue Sc_srSetSnowDriftSpeed(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetSnowDriftSpeed);
}

// void (int min_value, int max_value)
RuntimeScriptValue Sc_srSetSnowFallSpeed(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetSnowFallSpeed);
}

// void (int amount)
RuntimeScriptValue Sc_srChangeSnowAmount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(srChangeSnowAmount);
}

// void (int top, int bottom)
RuntimeScriptValue Sc_srSetSnowBaseline(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetSnowBaseline);
}

// void (int min_value, int max_value)
RuntimeScriptValue Sc_srSetSnowTransparency(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetSnowTransparency);
}

// void (int view, int loop)
RuntimeScriptValue Sc_srSetSnowDefaultView(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetSnowDefaultView);
}

// void (int value)
RuntimeScriptValue Sc_srSetSnowWindSpeed(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(srSetSnowWindSpeed);
}

// void (int amount)
RuntimeScriptValue Sc_srSetSnowAmount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(srSetSnowAmount);
}

// void (int kind_id, int event, int view, int loop)
RuntimeScriptValue Sc_srSetSnowView(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT4(srSetSnowView);
}

// void (int amount)
RuntimeScriptValue Sc_srChangeRainAmount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(srChangeRainAmount);
}

// void (int kind_id, int event, int view, int loop)
RuntimeScriptValue Sc_srSetRainView(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT4(srSetRainView);
}

// void (int view, int loop)
RuntimeScriptValue Sc_srSetRainDefaultView(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetRainDefaultView);
}

// void (int min_value, int max_value)
RuntimeScriptValue Sc_srSetRainTransparency(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetRainTransparency);
}

// void (int value)
RuntimeScriptValue Sc_srSetRainWindSpeed(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(srSetRainWindSpeed);
}

// void (int top, int bottom)
RuntimeScriptValue Sc_srSetRainBaseline(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetRainBaseline);
}

// void (int amount)
RuntimeScriptValue Sc_srSetRainAmount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(srSetRainAmount);
}

// void (int min_value, int max_value)
RuntimeScriptValue Sc_srSetRainFallSpeed(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetRainFallSpeed);
}

// void (int value)
RuntimeScriptValue Sc_srSetWindSpeed(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(srSetWindSpeed);
}

// void (int top, int bottom)
RuntimeScriptValue Sc_srSetBaseline(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(srSetBaseline);
}

// int ()
RuntimeScriptValue Sc_JoystickCount(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(JoystickCount);
}

// int (int a)
RuntimeScriptValue Sc_Joystick_Open(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT(Joystick_Open);
}

// int (int a)
RuntimeScriptValue Sc_Joystick_IsButtonDown(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT(Joystick_IsButtonDown);
}

// void (int a)
RuntimeScriptValue Sc_Joystick_EnableEvents(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(Joystick_EnableEvents);
}

// void ()
RuntimeScriptValue Sc_Joystick_DisableEvents(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID(Joystick_DisableEvents);
}

// void (int a)
RuntimeScriptValue Sc_Joystick_Click(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(Joystick_Click);
}

// int ()
RuntimeScriptValue Sc_Joystick_Valid(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(Joystick_Valid);
}

// int ()
RuntimeScriptValue Sc_Joystick_Unplugged(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(Joystick_Unplugged);
}

// int (int destination, int sprite, int x, int y, int transparency)
RuntimeScriptValue Sc_DrawAlpha(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT5(DrawAlpha);
}

// int (int sprite, int x, int y)
RuntimeScriptValue Sc_GetAlpha(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT3(GetAlpha);
}

// int (int sprite, int x, int y, int alpha)
RuntimeScriptValue Sc_PutAlpha(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT4(PutAlpha);
}

// int (int sprite, int radius)
RuntimeScriptValue Sc_Blur(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT2(Blur);
}

// int (int sprite, int threshold)
RuntimeScriptValue Sc_HighPass(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT2(HighPass);
}

// int (int destination, int sprite, int x, int y, float scale)
RuntimeScriptValue Sc_DrawAdd(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_PINT4_PFLOAT(DrawAdd);
}

// int ()
RuntimeScriptValue Sc_GetFlashlightInt(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT(GetFlashlightInt);
}

// void (int Param1)
RuntimeScriptValue Sc_SetFlashlightInt1(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(SetFlashlightInt1);
}

// void (int Param1, int Param2)
RuntimeScriptValue Sc_SetFlashlightInt2(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT2(SetFlashlightInt2);
}

// void (int Param1, int Param2, int Param3)
RuntimeScriptValue Sc_SetFlashlightInt3(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT3(SetFlashlightInt3);
}

// void (int Param1, int Param2, int Param3, int Param4, int Param5)
RuntimeScriptValue Sc_SetFlashlightInt5(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT5(SetFlashlightInt5);
}

// bool ()
RuntimeScriptValue Sc_wjuIsOnPhone(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_BOOL(wjuIsOnPhone);
}

// void (int)
RuntimeScriptValue Sc_wjuFakeKeypress(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_PINT(wjuFakeKeypress);
}

// void (char*, int)
RuntimeScriptValue Sc_wjuIosSetAchievementValue(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID_POBJ_PINT(wjuSetAchievement, char);
}

// int (char*)
RuntimeScriptValue Sc_wjuIosGetAchievementValue(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_INT_POBJ(wjuGetAchievement, char);
}

// void ()
RuntimeScriptValue Sc_wjuIosShowAchievements(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID(wjuShowAchievements);
}

// void ()
RuntimeScriptValue Sc_wjuIosResetAchievements(const RuntimeScriptValue *params, int32_t param_count)
{
    API_SCALL_VOID(wjuResetAchievements);
}

// void ()
RuntimeScriptValue Sc_SetSpriteFont(const RuntimeScriptValue *params, int32_t param_count)
{
	API_SCALL_VOID_PINT2(SetVariableSpriteFont);
}

// void ()
RuntimeScriptValue Sc_SetVariableSpriteFont(const RuntimeScriptValue *params, int32_t param_count)
{
	API_SCALL_VOID_PINT2(SetVariableSpriteFont);
}

// void ()
RuntimeScriptValue Sc_SetGlyph(const RuntimeScriptValue *params, int32_t param_count)
{
	API_SCALL_VOID_PINT6(SetGlyph);
}

// void ()
RuntimeScriptValue Sc_SetLineHeightAdjust(const RuntimeScriptValue *params, int32_t param_count)
{
	API_SCALL_VOID_PINT4(SetLineHeightAdjust);
}

// void ()
RuntimeScriptValue Sc_SetSpacing(const RuntimeScriptValue *params, int32_t param_count)
{
	API_SCALL_VOID_PINT2(SetSpacing);
}
/*
void SetSpriteFont(int fontNum, int sprite, int rows, int columns, int charWidth, int charHeight, int charMin, int charMax, bool use32bit)
void SetVariableSpriteFont(int fontNum, int sprite)
void SetGlyph(int fontNum, int charNum, int x, int y, int width, int height)
void SetSpacing(int fontNum, int spacing)
*/
bool RegisterPluginStubs(const char* name)
{
  // Stubs for plugin functions.

  if (strncmp(name, "ags_shell", strlen("ags_shell")) == 0)
  {
    // ags_shell.dll
    ccAddExternalStaticFunction("ShellExecute",                 Sc_ScriptStub_ShellExecute);
    return true;
  }
  else if (strncmp(name, "ags_snowrain", strlen("ags_snowrain")) == 0)
  {
    // ags_snowrain.dll
    ccAddExternalStaticFunction("srSetSnowDriftRange",          Sc_srSetSnowDriftRange);
    ccAddExternalStaticFunction("srSetSnowDriftSpeed",          Sc_srSetSnowDriftSpeed);
    ccAddExternalStaticFunction("srSetSnowFallSpeed",           Sc_srSetSnowFallSpeed);
    ccAddExternalStaticFunction("srChangeSnowAmount",           Sc_srChangeSnowAmount);
    ccAddExternalStaticFunction("srSetSnowBaseline",            Sc_srSetSnowBaseline);
    ccAddExternalStaticFunction("srSetSnowTransparency",        Sc_srSetSnowTransparency);
    ccAddExternalStaticFunction("srSetSnowDefaultView",         Sc_srSetSnowDefaultView);
    ccAddExternalStaticFunction("srSetSnowWindSpeed",           Sc_srSetSnowWindSpeed);
    ccAddExternalStaticFunction("srSetSnowAmount",              Sc_srSetSnowAmount);
    ccAddExternalStaticFunction("srSetSnowView",                Sc_srSetSnowView);
    ccAddExternalStaticFunction("srChangeRainAmount",           Sc_srChangeRainAmount);
    ccAddExternalStaticFunction("srSetRainView",                Sc_srSetRainView);
    ccAddExternalStaticFunction("srSetRainDefaultView",         Sc_srSetRainDefaultView);
    ccAddExternalStaticFunction("srSetRainTransparency",        Sc_srSetRainTransparency);
    ccAddExternalStaticFunction("srSetRainWindSpeed",           Sc_srSetRainWindSpeed);
    ccAddExternalStaticFunction("srSetRainBaseline",            Sc_srSetRainBaseline);
    ccAddExternalStaticFunction("srSetRainAmount",              Sc_srSetRainAmount);
    ccAddExternalStaticFunction("srSetRainFallSpeed",           Sc_srSetRainFallSpeed);
    ccAddExternalStaticFunction("srSetWindSpeed",               Sc_srSetWindSpeed);
    ccAddExternalStaticFunction("srSetBaseline",                Sc_srSetBaseline);
    return true;
  }
  else if (strncmp(name, "agsjoy", strlen("agsjoy")) == 0)
  {
    // agsjoy.dll
    ccAddExternalStaticFunction("JoystickCount",                Sc_JoystickCount);
    ccAddExternalStaticFunction("Joystick::Open^1",             Sc_Joystick_Open);
    ccAddExternalStaticFunction("Joystick::IsButtonDown^1",     Sc_Joystick_IsButtonDown);
    ccAddExternalStaticFunction("Joystick::EnableEvents^1",     Sc_Joystick_EnableEvents);
    ccAddExternalStaticFunction("Joystick::DisableEvents^0",    Sc_Joystick_DisableEvents);
    ccAddExternalStaticFunction("Joystick::Click^1",            Sc_Joystick_Click);
    ccAddExternalStaticFunction("Joystick::Valid^0",            Sc_Joystick_Valid);
    ccAddExternalStaticFunction("Joystick::Unplugged^0",        Sc_Joystick_Unplugged);
    return true;
  }
  else if (strncmp(name, "agsblend", strlen("agsblend")) == 0)
  {
    // agsblend.dll
    ccAddExternalStaticFunction("DrawAlpha",                    Sc_DrawAlpha);
    ccAddExternalStaticFunction("GetAlpha",                     Sc_GetAlpha);
    ccAddExternalStaticFunction("PutAlpha",                     Sc_PutAlpha);
    ccAddExternalStaticFunction("Blur",                         Sc_Blur);
    ccAddExternalStaticFunction("HighPass",                     Sc_HighPass);
    ccAddExternalStaticFunction("DrawAdd",                      Sc_DrawAdd);
    return true;
  }
  else if (strncmp(name, "agsflashlight", strlen("agsflashlight")) == 0)
  {
    // agsflashlight.dll
    ccAddExternalStaticFunction("SetFlashlightTint",            Sc_SetFlashlightInt3);
    ccAddExternalStaticFunction("GetFlashlightTintRed",         Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightTintGreen",       Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightTintBlue",        Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightMinLightLevel",   Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightMaxLightLevel",   Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightDarkness",        Sc_SetFlashlightInt1);
    ccAddExternalStaticFunction("GetFlashlightDarkness",        Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightDarknessSize",    Sc_SetFlashlightInt1);
    ccAddExternalStaticFunction("GetFlashlightDarknessSize",    Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightBrightness",      Sc_SetFlashlightInt1);
    ccAddExternalStaticFunction("GetFlashlightBrightness",      Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightBrightnessSize",  Sc_SetFlashlightInt1);
    ccAddExternalStaticFunction("GetFlashlightBrightnessSize",  Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightPosition",        Sc_SetFlashlightInt2);
    ccAddExternalStaticFunction("GetFlashlightPositionX",       Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightPositionY",       Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightFollowMouse",     Sc_SetFlashlightInt1);
    ccAddExternalStaticFunction("GetFlashlightFollowMouse",     Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightFollowCharacter", Sc_SetFlashlightInt5);
    ccAddExternalStaticFunction("GetFlashlightFollowCharacter", Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightCharacterDX",     Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightCharacterDY",     Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightCharacterHorz",   Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("GetFlashlightCharacterVert",   Sc_GetFlashlightInt);
    ccAddExternalStaticFunction("SetFlashlightMask",            Sc_SetFlashlightInt1);
    ccAddExternalStaticFunction("GetFlashlightMask",            Sc_GetFlashlightInt);
    return true;
  }
  else if (strncmp(name, "agswadjetutil", strlen("agswadjetutil")) == 0)
  {
    // agswadjetutil.dll
    ccAddExternalStaticFunction("IsOnPhone",                    Sc_wjuIsOnPhone);
    ccAddExternalStaticFunction("FakeKeypress",                 Sc_wjuFakeKeypress);
    ccAddExternalStaticFunction("MobileGetAchievement",       Sc_wjuIosGetAchievementValue);
    ccAddExternalStaticFunction("MobileSetAchievement",       Sc_wjuIosSetAchievementValue);
    ccAddExternalStaticFunction("MobileShowAchievements",          Sc_wjuIosShowAchievements);
    ccAddExternalStaticFunction("MobileResetAchievements",         Sc_wjuIosResetAchievements);
    return true;
  }
  else if (strncmp(name, "ags-spritefont-plugin", strlen("ags-spritefont-plugin")) == 0)
  {
	  ccAddExternalStaticFunction("SetSpriteFont",          Sc_SetSpriteFont);
	  ccAddExternalStaticFunction("SetVariableSpriteFont",  Sc_SetVariableSpriteFont);
	  ccAddExternalStaticFunction("SetGlyph",				Sc_SetGlyph);
	  ccAddExternalStaticFunction("SetSpacing",				Sc_SetSpacing);
	  ccAddExternalStaticFunction("SetLineHeightAdjust",	Sc_SetLineHeightAdjust);

	 return true;
  }
  else if (strncmp(name, "agsteam", strlen("agsteam")) == 0)
  {
	  // TO DO

	  return true;
  }
  return false;
}
