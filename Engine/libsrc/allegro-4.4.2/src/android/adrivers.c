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
 *      List of Android drivers.
 *
 *      By JJS for the Adventure Game Studio runtime port.
 *      Based on the Allegro PSP port.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"

#ifndef ALLEGRO_ANDROID
#error Something is wrong with the makefile
#endif


_DRIVER_INFO _system_driver_list[] =
{
   { SYSTEM_ANDROID,          &system_android,          TRUE  },
   { SYSTEM_NONE,             &system_none,             FALSE },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _keyboard_driver_list[] =
{
   { KEYBOARD_ANDROID,        &keyboard_android,        TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _timer_driver_list[] =
{
   { TIMER_ANDROID,           &timer_android,           TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _mouse_driver_list[] =
{
   { MOUSE_ANDROID,           &mouse_android,           TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _gfx_driver_list[] =
{
   { GFX_ANDROID,             &gfx_android,             TRUE  },
   { 0,                       NULL,                     0     }
};


_DRIVER_INFO _digi_driver_list[] =
{
   { DIGI_ANDROID,            &digi_android,            TRUE  },
   { 0,                       NULL,                     0     }
};


BEGIN_MIDI_DRIVER_LIST
MIDI_DRIVER_DIGMID
END_MIDI_DRIVER_LIST

BEGIN_JOYSTICK_DRIVER_LIST
END_JOYSTICK_DRIVER_LIST
