OCEANSPIRIT DENNIS: SCOURGE OF THE UNDERWORLD

This project was built with Android Studio using the gradle-
experimental 0.6.0-alpha5 plugin, and may not work with other
versions of the gradle plugin.

To build this project you will additionally need to download
the AGS native libraries from

     https://bitbucket.org/monkey0506/agsenginelibrary/downloads/libs.rar

(Or, rebuild the native libraries yourself)

This archive should be extracted to the root project folder.

To build the project you will need to update a few items:

     * Package name: Using "Refactor -> Rename" to update
       the package name should be sufficient. Make sure that
       it is changed in AndroidManifest.xml, strings.xml,
       and in the *.java files.

     * strings.xml: The app name and game file name need to
       be updated if you are reusing this app for another
       game. App name is the name used in the Google Play
       Store. Game file name refers to the file name of the
       game EXE or data file inside the OBB expansion file.

     * MainActivity.java: The OBB file version, size, and
       key need to be updated. The name of the OBB expansion
       file should always follow the naming convention of
       expansion files used by the Google Play Store --
         main.VERSION.PACKAGE_NAME.obb
       This file name is used by the source files when you
       include the correct OBB file version.

     * private.xml: This file is not included because, as
       the name indicates, it includes private data.
       Specifically, the RSA public key is specific to your
       app, and given to you in the Google Play Developer
       Console. You will also need to add an integer-array
       with some bytes for your salt. Details are included
       in strings.xml. These resources are needed only if
       distributing the app via Google Play, and may
       otherwise be defaulted with some junk data, but they
       must exist for the source to compile correctly.

To generate the expansion OBB file it is highly recommended
to use the jObbifier tool at

     https://github.com/monkey0506/jobbifier

The jobb tool included with the Android SDK has been
identified with bugs that prevent using game folders of
< 4 MB or > 511 MB. The jObbifier tool has fixes for these
bugs, and includes a GUI application. If you prefer the
command line, the jobb source included with the jObbifier
tool may still be run from the command line.

The expansion OBB file may be embedded into the APK by
placing it in the "assets" folder, but it should be
understood that this requires copying the OBB file onto
external storage before it can be used (duplicating the
size of your game files!). The preferred method therefore
is to use the Downloader Library, though this only
applies to apps published via Google Play. You may
explorer other methods of bundling the game files into
the APK, but these two methods are currently supported
by the code.

The final thing you'll need to change if using this project
to build another game is the graphic resources. Your
graphical assets can most likely just be copied in-place
over the existing assets at "app/src/main/res". You may
also choose to change the layout of the Downloader UI as
you see fit.