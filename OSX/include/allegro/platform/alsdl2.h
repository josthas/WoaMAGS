/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      SDL2 specific header defines.
 *
 *      By Edward Rudd.
 *
 *      See readme.txt for copyright information.
 */

#ifndef ALSDL2_H
#define ALSDL2_H

#ifndef ALLEGRO_SDL2
#error bad include
#endif


#ifndef SCAN_DEPEND
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#endif

/* Magic to capture name of executable file */
extern int    __sdl2_argc;
extern char **__sdl2_argv;

#ifndef ALLEGRO_NO_MAGIC_MAIN
   #define ALLEGRO_MAGIC_MAIN
   #define main _mangled_main
   #undef END_OF_MAIN
   #define END_OF_MAIN() void *_mangled_main_address = (void*) _mangled_main;
#else
   #undef END_OF_MAIN
   #define END_OF_MAIN() void *_mangled_main_address;
#endif


/* System driver */
#define SYSTEM_SDL2              AL_ID('S','D','L','S')
AL_VAR(SYSTEM_DRIVER, system_sdl2);

/* Timer driver */
#define TIMERDRV_SDL2            AL_ID('S','D','L','T')
AL_VAR(TIMER_DRIVER, timerdrv_sdl2);

/* Keyboard driver */
#define KEYBOARD_SDL2            AL_ID('S','D','L','K')
AL_VAR(KEYBOARD_DRIVER, keyboard_sdl2);

/* Mouse driver */
#define MOUSE_SDL2               AL_ID('S','D','L','M')
AL_VAR(MOUSE_DRIVER, mouse_sdl2);

/* Gfx drivers */
#define GFX_SDL2_WINDOW          AL_ID('S','D','L','W')
#define GFX_SDL2_FULLSCREEN      AL_ID('S','D','L','F')
AL_VAR(GFX_DRIVER, gfx_sdl2_window);
AL_VAR(GFX_DRIVER, gfx_sdl2_full);

/* Digital sound drivers */
#define DIGI_SDL2                AL_ID('S','D','L','A')
AL_VAR(DIGI_DRIVER, digi_sdl2);

/* MIDI music drivers */
// No Midi driver

/* Joystick drivers */
#define JOYSTICK_SDL2            AL_ID('S','D','L','J')
AL_VAR(JOYSTICK_DRIVER, joystick_sdl2);


#endif
