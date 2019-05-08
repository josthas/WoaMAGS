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
 *      Internal header file for the SDL2 Allegro library port.
 *
 *      By Edward Rudd.
 *
 *      See readme.txt for copyright information.
 */


#ifndef AINTSDL2_H
#define AINTSDL2_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_Window* sdl2_window;
extern char sdl2_window_title[ALLEGRO_MESSAGE_SIZE];

void sdl2_gl_render();

void sdl2_keyboard_handler(SDL_Event * event);
void sdl2_mouse_handler(SDL_Event * event);

int sdl2_mouse_set_sprite(BITMAP *sprite, int xfocus, int yfocus);
int sdl2_mouse_show(BITMAP *bmp, int x, int y);
void sdl2_mouse_hide(void);
void sdl2_mouse_move(int x, int y);

extern SDL_mutex* sdl2_window_mutex;
extern SDL_mutex* sdl2_event_mutex;

/* Macros to enable and disable interrupts */
#define DISABLE() _sdl2_bg_man->disable_interrupts()
#define ENABLE()  _sdl2_bg_man->enable_interrupts()

/* Typedef for background functions, called frequently in the background.
 * `threaded' is nonzero if the function is being called from a thread.
 */
typedef void (*bg_func) (int threaded);

/* Background function manager -- responsible for calling background
 * functions.  `int' methods return -1 on failure, 0 on success. */
struct bg_manager
{
   int multi_threaded;
   int (*init) (void);
   void (*exit) (void);
   int (*register_func) (bg_func f);
   int (*unregister_func) (bg_func f);
   void (*enable_interrupts) (void);
   void (*disable_interrupts) (void);
   int (*interrupts_disabled) (void);
};

extern struct bg_manager _bg_man_sdl2;

extern struct bg_manager *_sdl2_bg_man;

#ifdef __cplusplus
}
#endif

#endif

/* Local variables:       */
/* mode: objc             */
/* c-basic-offset: 3      */
/* indent-tabs-mode: nil  */
/* End:                   */
