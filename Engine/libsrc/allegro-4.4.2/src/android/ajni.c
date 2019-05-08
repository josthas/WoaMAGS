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
*      Java glue code.
*
*      By JJS for the Adventure Game Studio runtime port.
*
*      See readme.txt for copyright information.
*/


#include <allegro.h>
#include <jni.h>
#include <android/log.h>

#include "allegro.h"
#include "allegro/internal/aintern.h"
#include "allegro/platform/aintand.h"

JavaVM* android_jni_vm;
JNIEnv* android_jni_env_sound_thread;
JNIEnv* android_jni_env;
jclass android_jni_class;
jobject android_jni_object;
jbyteArray android_sound_buffer;

jmethodID android_jni_createScreen;
jmethodID android_jni_pollKeyboard;
jmethodID android_jni_initializeSound;
jmethodID android_jni_updateSound;
jmethodID android_jni_pollMouse;
jmethodID android_jni_pollMouseButtons;
jmethodID android_jni_pollMouseRelative;
jmethodID android_jni_pollMouseAbsolute;
jmethodID android_jni_swapBuffers;



/* Native methods exported to the Java glue code */
const JNINativeMethod android_jni_native_methods[] =
{
   { "nativeInitializeRenderer", "(II)V", (void*)android_initialize_renderer }
};


/* Called when the library is loaded */
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* jvm, void* reserved)
{
   android_jni_vm = jvm;
   
   return JNI_VERSION_1_6;
}


/*
Must be called with the Java Environment and calling class before
any JNI function can be used!
*/
void android_allegro_initialize_jni(JNIEnv* env, jclass java_class, jobject java_object)
{
   android_jni_env = env;
   android_jni_class = java_class;
   android_jni_object = java_object;
   
   (*android_jni_env)->RegisterNatives(android_jni_env, android_jni_class, android_jni_native_methods, 1);

   android_jni_createScreen = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "createScreen", "(III)V");
   android_jni_pollKeyboard = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "pollKeyboard", "()I");
   android_jni_pollMouseButtons = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "pollMouseButtons", "()I");
   android_jni_pollMouseRelative = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "pollMouseRelative", "()I");
   android_jni_pollMouseAbsolute = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "pollMouseAbsolute", "()I");
   android_jni_initializeSound = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "initializeSound", "([BI)V");
   android_jni_updateSound = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "updateSound", "()V"); 
   android_jni_swapBuffers = (*android_jni_env)->GetMethodID(android_jni_env, android_jni_class, "swapBuffers", "()V"); 
}

void android_debug_printf(char* format, ...)
{
   char buffer[200];
   va_list ap;
   va_start(ap, format);
   vsprintf(buffer, format, ap);
   va_end(ap);

   __android_log_print(ANDROID_LOG_DEBUG, "Allegro", "%s", buffer);
}

void android_attach_current_thread()
{
   (*android_jni_vm)->AttachCurrentThread(android_jni_vm, &android_jni_env_sound_thread, NULL);
}

void android_detach_current_thread()
{
   (*android_jni_vm)->DetachCurrentThread(android_jni_vm);
}

void android_update_sound(char* buffer, unsigned int size)
{
   (*android_jni_env_sound_thread)->SetByteArrayRegion(android_jni_env_sound_thread, android_sound_buffer, 0, size, buffer);
   (*android_jni_env_sound_thread)->CallVoidMethod(android_jni_env_sound_thread, android_jni_object, android_jni_updateSound);
}

void android_initialize_sound(int size)
{
   android_sound_buffer = (*android_jni_env)->NewGlobalRef(android_jni_env, (*android_jni_env)->NewByteArray(android_jni_env, size));
   (*android_jni_env)->CallVoidMethod(android_jni_env, android_jni_object, android_jni_initializeSound, android_sound_buffer, size);
}

void android_create_screen(int width, int height, int color_depth)
{
   (*android_jni_env)->CallVoidMethod(android_jni_env, android_jni_object, android_jni_createScreen, (jint)width, (jint)height, (jint)color_depth);
}

void android_swap_buffers()
{
   (*android_jni_env)->CallVoidMethod(android_jni_env, android_jni_object, android_jni_swapBuffers);
}

int android_poll_mouse_buttons()
{
   return (*android_jni_env)->CallIntMethod(android_jni_env, android_jni_object, android_jni_pollMouseButtons);
}

int android_poll_mouse_relative()
{
   return (*android_jni_env)->CallIntMethod(android_jni_env, android_jni_object, android_jni_pollMouseRelative);
}

int android_poll_mouse_absolute()
{
   return (*android_jni_env)->CallIntMethod(android_jni_env, android_jni_object, android_jni_pollMouseAbsolute);
}

int android_poll_keyboard()
{
   return (*android_jni_env)->CallIntMethod(android_jni_env, android_jni_object, android_jni_pollKeyboard);
}
