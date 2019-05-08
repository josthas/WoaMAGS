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
//
//
//
//=============================================================================
#ifndef __AGS_EE_MAIN__MAIN_H
#define __AGS_EE_MAIN__MAIN_H

#include "main/version.h"

// Current engine version
extern AGS::Engine::Version EngineVersion;
// Lowest savedgame version, accepted by this engine
extern AGS::Engine::Version SavedgameLowestBackwardCompatVersion;
// Lowest engine version, which would accept current savedgames
extern AGS::Engine::Version SavedgameLowestForwardCompatVersion;

#if defined(WINDOWS_VERSION) && !defined(_DEBUG)
#define USE_CUSTOM_EXCEPTION_HANDLER
#endif

//=============================================================================

#ifdef WINDOWS_VERSION
extern int wArgc;
extern LPWSTR *wArgv;
#endif
extern char **global_argv;

AGS::Common::String GetPathFromCmdArg(int arg_index);

// Startup flags, set from parameters to engine
extern int datafile_argv, change_to_game_dir, force_window;
extern int override_start_room, force_16bit;
extern bool justRegisterGame;
extern bool justUnRegisterGame;
extern const char *loadSaveGameOnStartup;

extern int psp_video_framedrop;
extern int psp_audio_enabled;
extern int psp_midi_enabled;
extern int psp_ignore_acsetup_cfg_file;
extern int psp_clear_cache_on_room_change;

extern int psp_midi_preload_patches;
extern int psp_audio_cachesize;
extern char psp_game_file_name[];
extern int psp_gfx_smooth_sprites;
extern char psp_translation[];

#endif // __AGS_EE_MAIN__MAIN_H
void main_print_help();
