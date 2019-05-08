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
 *      Timer driver.
 *
 *      By JJS for the Adventure Game Studio runtime port.
 *      Based on the Unix timer driver by Peter Wang.
 *
 *      See readme.txt for copyright information.
 */


#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintand.h"

#ifndef ALLEGRO_ANDROID
   #error something is wrong with the makefile
#endif

#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>


/* See hack later.  */
#ifdef ALLEGRO_LINUX_VGA
#ifdef ALLEGRO_HAVE_SYS_IO_H
/* iopl() exists in here instead of unistd.h in glibc */
#include <sys/io.h>
#endif
#include "allegro/platform/aintlnx.h"
#endif


#define TIMER_TO_USEC(x)  ((long)((x) / 1.193181))
#define USEC_TO_TIMER(x)  ((long)((x) * (TIMERS_PER_SECOND / 1000000.)))


static int ptimer_init(void);
static void ptimer_exit(void);
void _unix_rest(unsigned int ms, void (*callback) (void));


TIMER_DRIVER timer_android =
{
   TIMER_ANDROID,
   empty_string,
   empty_string,
   "Unix pthreads timers",
   ptimer_init,
   ptimer_exit,
   NULL, NULL,		/* install_int, remove_int */
   NULL, NULL,		/* install_param_int, remove_param_int */
   NULL, NULL,		/* can_simulate_retrace, simulate_retrace */
   _unix_rest		/* rest */
};



static pthread_t thread;
static int thread_alive;





/* ptimer_thread_func:
 *  The timer thread.
 */
static void *ptimer_thread_func(void *unused)
{
   struct timeval old_time;
   struct timeval new_time;
   struct timeval delay;
   long interval = 0x8000;


   gettimeofday(&old_time, 0);

   while (thread_alive) {
      /* `select' is more accurate than `usleep' on my system.  */
      delay.tv_sec = interval / TIMERS_PER_SECOND;
      delay.tv_usec = TIMER_TO_USEC(interval) % 1000000L;
      select(0, NULL, NULL, NULL, &delay);

      /* Calculate actual time elapsed.  */
      gettimeofday(&new_time, 0);
      interval = USEC_TO_TIMER((new_time.tv_sec - old_time.tv_sec) * 1000000L
			       + (new_time.tv_usec - old_time.tv_usec));
      old_time = new_time;

      /* Handle a tick.  */
      interval = _handle_timer_tick(interval);
   }

   return NULL;
}



/* ptimer_init:
 *  Installs the timer thread.
 */
static int ptimer_init(void)
{
   thread_alive = 1;

   if (pthread_create(&thread, NULL, ptimer_thread_func, NULL) != 0) {
   thread_alive = 0;
      return -1;
   }
   return 0;
}



/* ptimer_exit:
 *  Shuts down the timer thread.
 */
static void ptimer_exit(void)
{
   if (thread_alive) {
      thread_alive = 0;
      pthread_join(thread, NULL);
   }
}








/* timeval_subtract:
 *  Subtract the `struct timeval' values X and Y, storing the result
 *  in RESULT.  Return 1 if the difference is negative, otherwise 0.
 *
 *  This function is from the glibc manual.  It handles weird platforms
 *  where the tv_sec is unsigned.
 */
static int timeval_subtract(struct timeval *result,
			    struct timeval *x,
			    struct timeval *y)
{
   /* Perform the carry for the later subtraction by updating Y. */
   if (x->tv_usec < y->tv_usec) {
      int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
      y->tv_usec -= 1000000 * nsec;
      y->tv_sec += nsec;
   }
   if (x->tv_usec - y->tv_usec > 1000000) {
      int nsec = (x->tv_usec - y->tv_usec) / 1000000;
      y->tv_usec += 1000000 * nsec;
      y->tv_sec -= nsec;
   }

   /* Compute the time remaining to wait.
    * `tv_usec' is certainly positive. */
   result->tv_sec = x->tv_sec - y->tv_sec;
   result->tv_usec = x->tv_usec - y->tv_usec;
   /* Return 1 if result is negative. */
   return x->tv_sec < y->tv_sec;
}



void _unix_rest(unsigned int ms, void (*callback) (void))
{
   if (callback) {
      struct timeval tv, tv_end;

      gettimeofday (&tv_end, NULL);
      tv_end.tv_usec += ms * 1000;
      tv_end.tv_sec  += (tv_end.tv_usec / 1000000L);
      tv_end.tv_usec %= 1000000L;

      while (1)
      {
         (*callback)();
         gettimeofday (&tv, NULL);
         if (tv.tv_sec > tv_end.tv_sec)
            break;
         if (tv.tv_sec == tv_end.tv_sec && tv.tv_usec >= tv_end.tv_usec)
             break;
      }
   }
   else {
      struct timeval now;
      struct timeval end;
      struct timeval delay;
      int result;

      gettimeofday(&now, NULL);

      end = now;
      end.tv_usec += ms * 1000;
      end.tv_sec  += (end.tv_usec / 1000000L);
      end.tv_usec %= 1000000L;

      while (1) {
	 if (timeval_subtract(&delay, &end, &now))
	    break;

#ifdef ALLEGRO_MACOSX
	 result = usleep((delay.tv_sec * 1000000L) + delay.tv_usec);
#else
	 result = select(0, NULL, NULL, NULL, &delay);
#endif
	 if (result == 0)	/* ok */
	    break;
	 if ((result != -1) || (errno != EINTR))
	    break;

	 /* interrupted */
	 gettimeofday(&now, NULL);
      }
   }
}
