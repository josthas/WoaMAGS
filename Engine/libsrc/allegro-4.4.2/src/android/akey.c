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
*      Keyboard driver.
*
*      By JJS for the Adventure Game Studio runtime port.
*      Based on the Allegro PSP port.
*
*      See readme.txt for copyright information.
*/


#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintand.h"

#ifndef ALLEGRO_ANDROID
#error Something is wrong with the makefile
#endif


static int android_keyboard_init(void);
static void android_keyboard_exit(void);
static void android_keyboard_poll(void);
static int android_scancode_to_ascii(int scancode);


#define ANDROID_KEYCODE_COUNT 111


typedef struct
{
   char allegro_keycode;
   char asciicode;
} android_keymap_t;


static android_keymap_t android_scancode_to_allegro[ANDROID_KEYCODE_COUNT] =
{
   {   0,   0  }, // AKEYCODE_UNKNOWN         = 0,
   {   0,   0  }, // AKEYCODE_SOFT_LEFT       = 1,
   {   0,   0  }, // AKEYCODE_SOFT_RIGHT      = 2,
   {   0,   0  }, // AKEYCODE_HOME            = 3,
   {   KEY_ESC,   27  }, // AKEYCODE_BACK            = 4,
   {   0,   0  }, // AKEYCODE_CALL            = 5,
   {   0,   0  }, // AKEYCODE_ENDCALL         = 6,
   {   11, '0'  }, // AKEYCODE_0               = 7,
   {   2, '1'  }, // AKEYCODE_1               = 8,
   {   3, '2'  }, // AKEYCODE_2               = 9,
   {   4, '3'  }, // AKEYCODE_3               = 10,
   {   5, '4'  }, // AKEYCODE_4               = 11,
   {   6, '5'  }, // AKEYCODE_5               = 12,
   {   7, '6'  }, // AKEYCODE_6               = 13,
   {   8, '7'  }, // AKEYCODE_7               = 14,
   {   9, '8'  }, // AKEYCODE_8               = 15,
   {   10, '9'  }, // AKEYCODE_9               = 16,
   {   0, '*'  }, // AKEYCODE_STAR            = 17,
   {   0,   0  }, // AKEYCODE_POUND           = 18,
   {   0,   0  }, // AKEYCODE_DPAD_UP         = 19,
   {   0,   0  }, // AKEYCODE_DPAD_DOWN       = 20,
   {   0,   0  }, // AKEYCODE_DPAD_LEFT       = 21,
   {   0,   0  }, // AKEYCODE_DPAD_RIGHT      = 22,
   {   0,   0  }, // AKEYCODE_DPAD_CENTER     = 23,
   {   0,   0  }, // AKEYCODE_VOLUME_UP       = 24,
   {   0,   0  }, // AKEYCODE_VOLUME_DOWN     = 25,
   {   0,   0  }, // AKEYCODE_POWER           = 26,
   {   0,   0  }, // AKEYCODE_CAMERA          = 27,
   {   0,   0  }, // AKEYCODE_CLEAR           = 28,
   {   30, 'a'  }, // AKEYCODE_A               = 29,
   {   48, 'b'  }, // AKEYCODE_B               = 30,
   {   46, 'c'  }, // AKEYCODE_C               = 31,
   {   32, 'd'  }, // AKEYCODE_D               = 32,
   {   18, 'e'  }, // AKEYCODE_E               = 33,
   {   33, 'f'  }, // AKEYCODE_F               = 34,
   {   34, 'g'  }, // AKEYCODE_G               = 35,
   {   35, 'h'  }, // AKEYCODE_H               = 36,
   {   23, 'i'  }, // AKEYCODE_I               = 37,
   {   36, 'j'  }, // AKEYCODE_J               = 38,
   {   37, 'k'  }, // AKEYCODE_K               = 39,
   {   38, 'l'  }, // AKEYCODE_L               = 40,
   {   50, 'm'  }, // AKEYCODE_M               = 41,
   {   49, 'n'  }, // AKEYCODE_N               = 42,
   {   24, 'o'  }, // AKEYCODE_O               = 43,
   {   25, 'p'  }, // AKEYCODE_P               = 44,
   {   16, 'q'  }, // AKEYCODE_Q               = 45,
   {   19, 'r'  }, // AKEYCODE_R               = 46,
   {   31, 's'  }, // AKEYCODE_S               = 47,
   {   20, 't'  }, // AKEYCODE_T               = 48,
   {   22, 'u'  }, // AKEYCODE_U               = 49,
   {   47, 'v'  }, // AKEYCODE_V               = 50,
   {   17, 'w'  }, // AKEYCODE_W               = 51,
   {   45, 'x'  }, // AKEYCODE_X               = 52,
   {   21, 'y'  }, // AKEYCODE_Y               = 53,
   {   44, 'z'  }, // AKEYCODE_Z               = 54,
   {   51, ','  }, // AKEYCODE_COMMA           = 55,
   {   52, '.'  }, // AKEYCODE_PERIOD          = 56,
   {   56,   0  }, // AKEYCODE_ALT_LEFT        = 57,
   {   56,   0  }, // AKEYCODE_ALT_RIGHT       = 58,
   {   42,   0  }, // AKEYCODE_SHIFT_LEFT      = 59,
   {   54,   0  }, // AKEYCODE_SHIFT_RIGHT     = 60,
   {   KEY_TAB,   9  }, // AKEYCODE_TAB             = 61,
   {   KEY_SPACE, ' '  }, // AKEYCODE_SPACE           = 62,
   {   0,   0  }, // AKEYCODE_SYM             = 63,
   {   0,   0  }, // AKEYCODE_EXPLORER        = 64,
   {   0,   0  }, // AKEYCODE_ENVELOPE        = 65,
   {   KEY_ENTER,   13  }, // AKEYCODE_ENTER           = 66,
   {   KEY_BACKSPACE,   8  }, // AKEYCODE_DEL             = 67,
   {   0, '^'  }, // AKEYCODE_GRAVE           = 68,
   {   KEY_MINUS, '-'  }, // AKEYCODE_MINUS           = 69,
   {   KEY_EQUALS, '='  }, // AKEYCODE_EQUALS          = 70,
   {   26, '('  }, // AKEYCODE_LEFT_BRACKET    = 71,
   {   27, ')'  }, // AKEYCODE_RIGHT_BRACKET   = 72,
   {   KEY_BACKSLASH, '\\' }, // AKEYCODE_BACKSLASH       = 73,
   {   39, ';'  }, // AKEYCODE_SEMICOLON       = 74,
   {   40, '\'' }, // AKEYCODE_APOSTROPHE      = 75,
   {   KEY_SLASH, '/'  }, // AKEYCODE_SLASH           = 76,
   {   KEY_AT, '@'  }, // AKEYCODE_AT              = 77,
   {   69,   0  }, // AKEYCODE_NUM             = 78,
   {   0,   0  }, // AKEYCODE_HEADSETHOOK     = 79,
   {   0,   0  }, // AKEYCODE_FOCUS           = 80,   // *Camera* focus
   {   KEY_PLUS_PAD, '+'  }, // AKEYCODE_PLUS            = 81,
   {   0,   0  }, // AKEYCODE_MENU            = 82,
   {   0,   0  }, // AKEYCODE_NOTIFICATION    = 83,
   {   0,   0  }, // AKEYCODE_SEARCH          = 84,
   {   0,   0  }, // AKEYCODE_MEDIA_PLAY_PAUSE= 85,
   {   0,   0  }, // AKEYCODE_MEDIA_STOP      = 86,
   {   0,   0  }, // AKEYCODE_MEDIA_NEXT      = 87,
   {   0,   0  }, // AKEYCODE_MEDIA_PREVIOUS  = 88,
   {   0,   0  }, // AKEYCODE_MEDIA_REWIND    = 89,
   {   0,   0  }, // AKEYCODE_MEDIA_FAST_FORWARD = 90,
   {   0,   0  }, // AKEYCODE_MUTE            = 91,
   {   73,   0  }, // AKEYCODE_PAGE_UP         = 92,
   {   81,   0  }, // AKEYCODE_PAGE_DOWN       = 93,
   {   0,   0  }, // AKEYCODE_PICTSYMBOLS     = 94,
   {   0,   0  }, // AKEYCODE_SWITCH_CHARSET  = 95,
   {   0,   0  }, // AKEYCODE_BUTTON_A        = 96,
   {   0,   0  }, // AKEYCODE_BUTTON_B        = 97,
   {   0,   0  }, // AKEYCODE_BUTTON_C        = 98,
   {   0,   0  }, // AKEYCODE_BUTTON_X        = 99,
   {   0,   0  }, // AKEYCODE_BUTTON_Y        = 100,
   {   0,   0  }, // AKEYCODE_BUTTON_Z        = 101,
   {   0,   0  }, // AKEYCODE_BUTTON_L1       = 102,
   {   0,   0  }, // AKEYCODE_BUTTON_R1       = 103,
   {   0,   0  }, // AKEYCODE_BUTTON_L2       = 104,
   {   0,   0  }, // AKEYCODE_BUTTON_R2       = 105,
   {   0,   0  }, // AKEYCODE_BUTTON_THUMBL   = 106,
   {   0,   0  }, // AKEYCODE_BUTTON_THUMBR   = 107,
   {   0,   0  }, // AKEYCODE_BUTTON_START    = 108,
   {   0,   0  }, // AKEYCODE_BUTTON_SELECT   = 109,
   {   0,   0  }, // AKEYCODE_BUTTON_MODE     = 110,
};


KEYBOARD_DRIVER keyboard_android =
{
   KEYBOARD_ANDROID,
   empty_string,
   empty_string,
   "Android keyboard",
   FALSE,  // int autorepeat;
   android_keyboard_init,
   android_keyboard_exit,
   android_keyboard_poll,
   NULL,   // AL_METHOD(void, set_leds, (int leds));
   NULL,   // AL_METHOD(void, set_rate, (int delay, int rate));
   NULL,   // AL_METHOD(void, wait_for_input, (void));
   NULL,   // AL_METHOD(void, stop_waiting_for_input, (void));
   android_scancode_to_ascii,   // AL_METHOD(int,  scancode_to_ascii, (int scancode));
   NULL    // scancode_to_name
};


static int android_last_scancode = -1;


static int android_scancode_to_ascii(int scancode)
{
   if (scancode > ANDROID_KEYCODE_COUNT)
      return 0;
   else
      return android_scancode_to_allegro[scancode].asciicode;
}


static int android_scancode_to_keycode(int scancode)
{
   if (scancode > ANDROID_KEYCODE_COUNT)
      return scancode - 0x1000;
   else
      return android_scancode_to_allegro[scancode].allegro_keycode;
}


static int android_keyboard_init(void)
{
   return 0;
}


static void android_keyboard_exit(void)
{
}


static void android_keyboard_poll(void)
{
   int ascii_code;
   int shift_pressed;
   int allegro_keycode;
   int polled_value = android_poll_keyboard();

   int scancode = (polled_value & 0x0000FFFF);

   // Hold the key down till the next keyboard poll
   if (android_last_scancode != -1)
   {
      _handle_key_release(android_last_scancode); 
      android_last_scancode = -1;
   }
   
   if (scancode < 1)
      return;
   
   shift_pressed = (polled_value & 0xF0000000) >> 30;

   ascii_code = (polled_value & 0x0FFF0000) >> 16;

   allegro_keycode = android_scancode_to_keycode(scancode);
   
   if (ascii_code == 0)
      ascii_code = android_scancode_to_ascii(scancode);

   // Replace LF with CR
   if (ascii_code == 10)
      ascii_code = 13;

   _handle_key_press(ascii_code, allegro_keycode);

   android_last_scancode = allegro_keycode;
}
