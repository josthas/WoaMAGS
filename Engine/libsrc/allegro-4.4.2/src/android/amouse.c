/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      Mouse driver.
 *
 *      By JJS for the Adventure Game Studio runtime port.
 *      Based on the Allegro PSP port.
 *
 *      See readme.txt for copyright information.
 */

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintand.h"

#include "math.h"

#ifndef ALLEGRO_ANDROID
#error something is wrong with the makefile
#endif

static int  android_mouse_init(void);
static void android_mouse_exit(void);
static void android_mouse_position(int, int);
static void android_mouse_set_range(int, int, int, int);
static void android_mouse_get_mickeys(int *, int *);
static void android_mouse_poll(void);


static int mouse_minx = 0;
static int mouse_miny = 0;
static int mouse_maxx = 320;
static int mouse_maxy = 200;
int android_left_mouse_stay = 0;
int android_right_mouse_stay = 0;
int android_mouse_last_x = 0;
int android_mouse_last_y = 0;

int config_mouse_control_mode = 0; // 0 = direct, 1 = relative


MOUSE_DRIVER mouse_android =
{
   MOUSE_ANDROID,
   empty_string,
   empty_string,
   "android mouse",
   android_mouse_init,
   android_mouse_exit,
   android_mouse_poll,       // AL_METHOD(void, poll, (void));
   NULL,                 // AL_METHOD(void, timer_poll, (void));
   android_mouse_position,
   android_mouse_set_range,
   NULL,       // AL_METHOD(void, set_speed, (int xspeed, int yspeed));
   android_mouse_get_mickeys,
   NULL,       // AL_METHOD(int,  analyse_data, (AL_CONST char *buffer, int size));
   NULL,       // AL_METHOD(void,  enable_hardware_cursor, (AL_CONST int mode));
   NULL        // AL_METHOD(int,  select_system_cursor, (AL_CONST int cursor));
};



static int android_mouse_init(void)
{
   return 3; //Num of buttons.
}


int android_mouse_clip_left = 0;
int android_mouse_clip_right = 799;
int android_mouse_clip_top = 0;
int android_mouse_clip_bottom = 479;
float android_mouse_scaling_x = 1.0f;
float android_mouse_scaling_y = 1.0f;


void android_clip_mouse(int* x, int* y)
{
   if (*x < android_mouse_clip_left)
      *x = android_mouse_clip_left;

   if (*y < android_mouse_clip_top)
      *y = android_mouse_clip_top;

   if (*x > android_mouse_clip_right)
      *x = android_mouse_clip_right;

   if (*y > android_mouse_clip_bottom)
      *y = android_mouse_clip_bottom;

   *x -= android_mouse_clip_left;
   *y -= android_mouse_clip_top;
}

void android_scale_mouse(int* x, int* y)
{
   *x = (float)*x * android_mouse_scaling_x;
   *y = (float)*y * android_mouse_scaling_y;
}


void android_mouse_setup(int left, int right, int top, int bottom, float scaling_x, float scaling_y)
{
   android_mouse_clip_left = left;
   android_mouse_clip_right = right;
   android_mouse_clip_top = top;
   android_mouse_clip_bottom = bottom;
   android_mouse_scaling_x = scaling_x;
   android_mouse_scaling_y = scaling_y;
}


static void android_mouse_poll(void)
{
  int new_x;
  int new_y;
  int new_click = android_poll_mouse_buttons();
  
  if (config_mouse_control_mode == 1)
  {
    unsigned int new = android_poll_mouse_relative();
    new_x = (short)(new & 0x0000FFFF);
    new_y = (short)((new & 0xFFFF0000) >> 16);

    android_scale_mouse(&new_x, &new_y);
  
    new_x += _mouse_x;
    new_y += _mouse_y;
  }
  else
  {
    unsigned int new = android_poll_mouse_absolute();
    new_x = (new & 0x0000FFFF);
    new_y = (new & 0xFFFF0000) >> 16;
    android_clip_mouse(&new_x, &new_y);	
    android_scale_mouse(&new_x, &new_y);
  }
  
  if (new_x < mouse_minx)
    new_x = mouse_minx;

  if (new_y < mouse_miny)
    new_y = mouse_miny;

  if (new_x > mouse_maxx)
    new_x = mouse_maxx;

  if (new_y > mouse_maxy)
    new_y = mouse_maxy;

  _mouse_x = new_x;
  _mouse_y = new_y;
  
  /* Keep button pressed for a couple of frames */
  if (new_click == 1)
    android_left_mouse_stay = 10;
  else if (new_click == 2)
    android_right_mouse_stay = 10;
  else if (new_click == 10)
    android_left_mouse_stay = 10 * 1000 * 1000;
  
  if (android_left_mouse_stay > 0)
    android_left_mouse_stay--;

  if (android_right_mouse_stay > 0)
    android_right_mouse_stay--;

  if (android_left_mouse_stay > 0)
    _mouse_b = 1;
  else if (android_right_mouse_stay > 0)
    _mouse_b = 2;
  else
    _mouse_b = 0;
}




static void android_mouse_position(int x, int y)
{
  _mouse_x = x;
  _mouse_y = y;
}



static void android_mouse_set_range(int x1, int y1, int x2, int y2)
{
  mouse_minx = x1;
  mouse_miny = y1;
  mouse_maxx = x2;
  mouse_maxy = y2;
}



static void android_mouse_get_mickeys(int *mickeyx, int *mickeyy)
{

}



static void android_mouse_exit(void)
{
}

