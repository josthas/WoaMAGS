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
 *      Audio driver.
 *
 *      By JJS for the Adventure Game Studio runtime port.
 *      Based on the Allegro PSP port.
 *
 *      See readme.txt for copyright information.
 */

#include <pthread.h>
#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintand.h"

#ifndef ALLEGRO_ANDROID
#error something is wrong with the makefile
#endif


#define SAMPLES_PER_BUFFER 1024


static int digi_android_detect(int);
static int digi_android_init(int, int);
static void digi_android_exit(int);

static short sound_buffer[2][SAMPLES_PER_BUFFER][2];
int curr_buffer = 0;

pthread_t soundthread;

int android_audio_playing = 0;

DIGI_DRIVER digi_android =
{
   DIGI_ANDROID,
   empty_string,
   empty_string,
   "Android digital sound driver",
   0,
   0,
   MIXER_MAX_SFX,
   MIXER_DEF_SFX,

   digi_android_detect,
   digi_android_init,
   digi_android_exit,
   NULL,
   NULL,

   NULL,
   NULL,
   NULL,
   _mixer_init_voice,
   _mixer_release_voice,
   _mixer_start_voice,
   _mixer_stop_voice,
   _mixer_loop_voice,

   _mixer_get_position,
   _mixer_set_position,

   _mixer_get_volume,
   _mixer_set_volume,
   _mixer_ramp_volume,
   _mixer_stop_volume_ramp,

   _mixer_get_frequency,
   _mixer_set_frequency,
   _mixer_sweep_frequency,
   _mixer_stop_frequency_sweep,

   _mixer_get_pan,
   _mixer_set_pan,
   _mixer_sweep_pan,
   _mixer_stop_pan_sweep,

   _mixer_set_echo,
   _mixer_set_tremolo,
   _mixer_set_vibrato,
   0, 0,
   0,
   0,
   0,
   0,
   0,
   0
};



void* psp_audio_channel_thread()
{
  void* bufptr;

  /* Make the thread known to the Java VM */
  android_attach_current_thread();

  while (android_audio_playing)
  {
      bufptr = &sound_buffer[curr_buffer];
      /* Asks to the Allegro mixer to fill the buffer */
      _mix_some_samples((uintptr_t)bufptr, 0, TRUE);
      /* Send mixed buffer to sound card */
	  android_update_sound(bufptr, SAMPLES_PER_BUFFER * 2 * 2);
      curr_buffer = !curr_buffer;
	  usleep(1000 * 15);
  }
  
  android_detach_current_thread();

  return NULL;
}



static int digi_android_detect(int input)
{
   return TRUE;
}



static int digi_android_init(int input, int voices)
{
  _sound_bits = 16;
  _sound_stereo = TRUE;
  _sound_freq = 44100;  
  
  digi_android.voices = voices;  

  android_initialize_sound(SAMPLES_PER_BUFFER * (_sound_bits / 8) * (_sound_stereo ? 2 : 1));
  
  if (_mixer_init(SAMPLES_PER_BUFFER * 2, _sound_freq, _sound_stereo, (_sound_bits == 16), &digi_android.voices))
      return -1;

  android_audio_playing = 1;
  pthread_create(&soundthread, NULL, psp_audio_channel_thread, NULL);

   return 0;
}


static void digi_android_exit(int input)
{
   if (android_audio_playing)
   {
     android_audio_playing = 0;
     pthread_join(soundthread, NULL);
   }
}
