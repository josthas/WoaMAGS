// AGS2Client
// Client Plugin Interface for AGS
// Copyright © 2015-2016 MonkeyMoto Productions, Inc.
//
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
#include "IAGS2Client.h"

// ***** DESIGN TIME CALLS *******

#ifdef WINDOWS_VERSION

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    // this is the WINDOWS entry point into the DLL
    return TRUE;
}

IAGSEditor *editor;

char const *ourScriptHeader = nullptr;

LPCSTR AGS_GetPluginName()
{
    return AGS2Client::GetClient()->GetAGSPluginName();
}

int  AGS_EditorStartup(IAGSEditor *lpEditor)
{
    // User has checked the plugin to use it in their game

    // If it's an earlier version than what we need, abort.
    if (lpEditor->version < 1) return -1;

    editor = lpEditor;
    ourScriptHeader = AGS2Client::GetClient()->GetAGSScriptHeader();
    if (ourScriptHeader != nullptr) editor->RegisterScriptHeader(ourScriptHeader);

    // Return 0 to indicate success
    return 0;
}

void AGS_EditorShutdown()
{
    // User has un-checked the plugin from their game
    if (ourScriptHeader != nullptr) editor->UnregisterScriptHeader(ourScriptHeader);
}

void AGS_EditorProperties(HWND parent)
{
    // User has chosen to view the Properties of the plugin
    // We could load up an options dialog or something here instead
    MessageBox(parent, AGS2Client::GetClient()->GetAGSPluginDesc(), "About", MB_OK | MB_ICONINFORMATION);
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

    AGS2Client::GetClient()->Startup();
    AGS2Client::GetClient()->RegisterScriptFunctions(engine);

    engine->RequestEventHook(AGSE_FINALSCREENDRAW);
    engine->RequestEventHook(AGSE_KEYPRESS);
}

void AGS_EngineShutdown()
{
    AGS2Client::GetClient()->Shutdown();
}

int AGS_EngineOnEvent(int event, int data)
{
    if (event == AGSE_FINALSCREENDRAW)
    {
        // use this as main_game_loop
        // the screen is drawn every actual game loop anyway
        AGS2Client::GetClient()->Update();
    }
    else if (event == AGSE_KEYPRESS)
    {
        return AGS2Client::GetClient()->ClaimKeyPress(data, reinterpret_cast<int(*)(int)>(engine->GetScriptFunctionAddress("IsKeyPressed")));
    }
    return 0;
}

// *** END RUN TIME ****
