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
*      Display driver.
*
*      By JJS for the Adventure Game Studio runtime port.
*      Based on the Allegro PSP port.
*
*      See readme.txt for copyright information.
*/

#include <GLES/gl.h>

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintand.h"


/* Software version of some blitting methods */
static void (*_orig_blit) (BITMAP * source, BITMAP * dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);

static BITMAP *android_display_init(int w, int h, int v_w, int v_h, int color_depth);
static void gfx_android_enable_acceleration(GFX_VTABLE *vtable);
static void android_hw_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);

void android_render();
void android_initialize_opengl();


/* Options controlled by the application */
int psp_gfx_scaling = 1;
int psp_gfx_smoothing = 1;


unsigned int android_screen_texture = 0;
unsigned int android_screen_width = 320;
unsigned int android_screen_height = 200;
unsigned int android_screen_physical_width = 320;
unsigned int android_screen_physical_height = 200;
unsigned int android_screen_color_depth = 32;
int android_screen_texture_width = 0;
int android_screen_texture_height = 0;
float android_screen_ar = 1.0f;
float android_device_ar = 1.0f;

int android_screen_initialized = 0;

GLfloat android_vertices[] =
{
   0, 0,
   320,  0,
   0,  200,
   320,  200
};

GLfloat android_texture_coordinates[] =
{
   0, 200.0f / 256.0f,
   320.0f / 512.0f, 200.0f / 256.0f,
   0, 0,
   320.0f / 512.0f, 0
};


GFX_DRIVER gfx_android =
{
   GFX_ANDROID,
   empty_string,
   empty_string,
   "Android gfx driver",
   android_display_init,         /* AL_METHOD(struct BITMAP *, init, (int w, int h, int v_w, int v_h, int color_depth)); */
   NULL,                         /* AL_METHOD(void, exit, (struct BITMAP *b)); */
   NULL,                         /* AL_METHOD(int, scroll, (int x, int y)); */
   NULL,                         /* AL_METHOD(void, vsync, (void)); */
   NULL,                         /* AL_METHOD(void, set_palette, (AL_CONST struct RGB *p, int from, int to, int retracesync)); */
   NULL,                         /* AL_METHOD(int, request_scroll, (int x, int y)); */
   NULL,                         /* AL_METHOD(int, poll_scroll, (void)); */
   NULL,                         /* AL_METHOD(void, enable_triple_buffer, (void)); */
   NULL,                         /* AL_METHOD(struct BITMAP *, create_video_bitmap, (int width, int height)); */
   NULL,                         /* AL_METHOD(void, destroy_video_bitmap, (struct BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(int, show_video_bitmap, (BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(int, request_video_bitmap, (BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(BITMAP *, create_system_bitmap, (int width, int height)); */
   NULL,                         /* AL_METHOD(void, destroy_system_bitmap, (BITMAP *bitmap)); */
   NULL,                         /* AL_METHOD(int, set_mouse_sprite, (BITMAP *sprite, int xfocus, int yfocus)); */
   NULL,                         /* AL_METHOD(int, show_mouse, (BITMAP *bmp, int x, int y)); */
   NULL,                         /* AL_METHOD(void, hide_mouse, (void)); */
   NULL,                         /* AL_METHOD(void, move_mouse, (int x, int y)); */
   NULL,                         /* AL_METHOD(void, drawing_mode, (void)); */
   NULL,                         /* AL_METHOD(void, save_video_state, (void)); */
   NULL,                         /* AL_METHOD(void, restore_video_state, (void)); */
   NULL,                         /* AL_METHOD(void, set_blender_mode, (int mode, int r, int g, int b, int a)); */
   NULL,                         /* AL_METHOD(int, fetch_mode_list, (void)); */
   0, 0,                         /* physical (not virtual!) screen size */
   TRUE,                         /* true if video memory is linear */
   0,                            /* bank size, in bytes */
   0,                            /* bank granularity, in bytes */
   0,                            /* video memory size, in bytes */
   0,                            /* physical address of video memory */
   FALSE                         /* true if driver runs windowed */
};


static BITMAP *android_display_init(int w, int h, int v_w, int v_h, int color_depth)
{
   GFX_VTABLE* vtable = _get_vtable(color_depth);

   /* Do the final screen blit to a 32 bit bitmap in palette mode */
   if (color_depth == 8)
      color_depth = 32;

   android_screen_width = w;
   android_screen_height = h;
   android_screen_color_depth = color_depth;

   displayed_video_bitmap = create_bitmap_ex(color_depth, w, h);
   gfx_android_enable_acceleration(vtable);

   android_create_screen(w, h, color_depth);

   return displayed_video_bitmap;
}


static void gfx_android_enable_acceleration(GFX_VTABLE *vtable)
{
   /* Keep the original blitting methods */
   _orig_blit = vtable->blit_to_self;

   /* Accelerated blits. */
   vtable->blit_from_memory = android_hw_blit;
   vtable->blit_to_memory = android_hw_blit;
   vtable->blit_from_system = android_hw_blit;
   vtable->blit_to_system = android_hw_blit;
   vtable->blit_to_self = android_hw_blit;

   _screen_vtable.blit_from_memory = android_hw_blit;
   _screen_vtable.blit_to_memory = android_hw_blit;
   _screen_vtable.blit_from_system = android_hw_blit;
   _screen_vtable.blit_to_system = android_hw_blit;
   _screen_vtable.blit_to_self = android_hw_blit;

   gfx_capabilities |= (GFX_HW_VRAM_BLIT | GFX_HW_MEM_BLIT);
}


/* All blitting goes through here, signal a screen update if the blit target is the screen bitmap */
static void android_hw_blit(BITMAP *source, BITMAP *dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)
{
   _orig_blit(source, dest, source_x, source_y, dest_x, dest_y, width, height);

   if (dest == displayed_video_bitmap)
      android_render();
}


int android_get_next_power_of_2(int value)
{
   int test = 1;

   while (test < value)
      test *= 2;

   return test;
}


/* Create the texture that holds the screen bitmap */
void android_create_screen_texture(int width, int height, int color_depth)
{
   char* empty;

   android_screen_texture_width = android_get_next_power_of_2(width);
   android_screen_texture_height = android_get_next_power_of_2(height);

   empty = (char*)malloc(android_screen_texture_width * android_screen_texture_height * color_depth / 8);
   memset(empty, 0, android_screen_texture_width * android_screen_texture_height * color_depth / 8);

   if (android_screen_texture != 0)
      glDeleteTextures(1, &android_screen_texture);

   glGenTextures(1, &android_screen_texture);
   glBindTexture(GL_TEXTURE_2D, android_screen_texture);

   if (color_depth == 16)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, android_screen_texture_width, android_screen_texture_height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, empty);
   else
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, android_screen_texture_width, android_screen_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, empty);

   free(empty);
}


/* Set the values for the texture coord. and vertex arrays */
void android_create_arrays()
{
   if (psp_gfx_scaling == 1)
   {
      if (android_device_ar <= android_screen_ar)
      {
         android_vertices[2] = android_vertices[6] = android_screen_physical_width - 1;
         android_vertices[5] = android_vertices[7] = android_screen_physical_width * ((float)android_screen_height / (float)android_screen_width);
         
         android_mouse_setup(
            0, 
            android_screen_physical_width - 1, 
            (android_screen_physical_height - android_vertices[5]) / 2, 
            android_screen_physical_height - ((android_screen_physical_height - android_vertices[5]) / 2), 
            (float)android_screen_width / (float)android_screen_physical_width, 
            (float)android_screen_height / android_vertices[5]);
      }
      else
      {
         android_vertices[2] = android_vertices[6] = android_screen_physical_height * ((float)android_screen_width / (float)android_screen_height);
         android_vertices[5] = android_vertices[7] = android_screen_physical_height - 1;
         
         android_mouse_setup(
            (android_screen_physical_width - android_vertices[2]) / 2,
            android_screen_physical_width - ((android_screen_physical_width - android_vertices[2]) / 2),
            0,
            android_screen_physical_height - 1,
            (float)android_screen_width / android_vertices[2], 
            (float)android_screen_height / (float)android_screen_physical_height);
      }
   }
   else if (psp_gfx_scaling == 2)
   {
      android_vertices[2] = android_vertices[6] = android_screen_physical_width - 1;
      android_vertices[5] = android_vertices[7] = android_screen_physical_height - 1;
      
      android_mouse_setup(
         0, 
         android_screen_physical_width - 1, 
         0, 
         android_screen_physical_width - 1, 
         (float)android_screen_width / (float)android_screen_physical_width, 
         (float)android_screen_height / (float)android_screen_physical_height);	  
   }   
   else
   {
      android_vertices[0] = android_vertices[4] = android_screen_width * (-0.5f);
      android_vertices[2] = android_vertices[6] = android_screen_width * 0.5f;
      android_vertices[5] = android_vertices[7] = android_screen_height * 0.5f;
      android_vertices[1] = android_vertices[3] = android_screen_height * (-0.5f);
      
      android_mouse_setup(
         (android_screen_physical_width - android_screen_width) / 2,
         android_screen_physical_width - ((android_screen_physical_width - android_screen_width) / 2),
         (android_screen_physical_height - android_screen_height) / 2, 
         android_screen_physical_height - ((android_screen_physical_height - android_screen_height) / 2), 
         1.0f,
         1.0f);
   }

   android_texture_coordinates[1] = android_texture_coordinates[3] = (float)android_screen_height / (float)android_screen_texture_height;
   android_texture_coordinates[2] = android_texture_coordinates[6] = (float)android_screen_width / (float)android_screen_texture_width;
}


/* Called from the Java app to set up the screen */
void android_initialize_renderer(JNIEnv* env, jobject self, jint screen_width, jint screen_height)
{
   android_screen_physical_width = screen_width;
   android_screen_physical_height = screen_height;
   android_screen_initialized = 0;
}


void android_initialize_opengl()
{
   android_screen_ar = (float)android_screen_width / (float)android_screen_height;
   android_device_ar = (float)android_screen_physical_width / (float)android_screen_physical_height;

   glEnable(GL_CULL_FACE);
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_LIGHTING);
   glDisable(GL_BLEND);
   glShadeModel(GL_FLAT);

   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   glViewport(0, 0, android_screen_physical_width, android_screen_physical_height);
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);

   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glTexCoordPointer(2, GL_FLOAT, 0, android_texture_coordinates);
   glVertexPointer(2, GL_FLOAT, 0, android_vertices);

   android_create_screen_texture(android_screen_width, android_screen_height, android_screen_color_depth);
   android_create_arrays();

   glBindTexture(GL_TEXTURE_2D, android_screen_texture);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   if (psp_gfx_smoothing)
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   }
   else
   {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   }

   glViewport(0, 0, android_screen_physical_width, android_screen_physical_height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   glOrthof(0, android_screen_physical_width - 1, 0, android_screen_physical_height - 1, 0, 1);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   if (psp_gfx_scaling == 1)
   {
      if (android_device_ar <= android_screen_ar)
         glTranslatef(0, (android_screen_physical_height - android_vertices[5] - 1) / 2, 0);
      else
         glTranslatef((android_screen_physical_width - android_vertices[2] - 1) / 2, 0, 0);
   }
   else if (psp_gfx_scaling == 0)
   {
      glTranslatef(android_screen_physical_width / 2.0f, android_screen_physical_height / 2.0f, 0);
   }
}



void android_render()
{
   if (!android_screen_initialized)
   {
      android_initialize_opengl();
      android_screen_initialized = 1;
   }

   if (android_screen_color_depth == 16)
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, android_screen_width, android_screen_height, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, displayed_video_bitmap->line[0]);
   else
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, android_screen_width, android_screen_height, GL_RGBA, GL_UNSIGNED_BYTE, displayed_video_bitmap->line[0]);

   glClear(GL_COLOR_BUFFER_BIT);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

   glFinish();
   android_swap_buffers();
}
