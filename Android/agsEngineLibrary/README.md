This module for Android Studio allows you to use the Adventure Game Studio
in your Android projects. The module depends on native libraries, which
can be downloaded at

     https://bitbucket.org/monkey0506/agsenginelibrary/downloads/libs.rar

This archive should be extracted at the ROOT of your Android Studio
project (NOT inside the agsEngineLibrary module). You will additionally
need to include the following in your project's build.gradle file, under
dependencies

     compile fileTree(dir: "$rootProject.projectDir/build/native-libs", include: 'native-libs.jar')

This line is necessary to ensure that the AGS libraries are built into
your APK. Otherwise your app will crash with an error that the library
could not be loaded.



The native libraries can also be rebuilt using the AGS source code from

     https://github.com/adventuregamestudio/ags

Create a project called "Studio" inside the Android folder and import
the agsEngineLibrary. Copy "Android/library/jni" into your Studio project
folder and rebuild. The AGS project files for Android will be updated
to Android Studio project files at a later time to eliminate these
additional steps. After rebuilding, copy the "libs" folder from your
Studio project into your project's root folder, as you normally would.