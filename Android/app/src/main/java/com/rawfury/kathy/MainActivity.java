package com.rawfury.kathy; // TODO: Refactor -> Rename to your package name

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;

import com.bigbluecup.android.AgsEngine;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Looper;
import android.os.storage.OnObbStateChangeListener;
import android.os.storage.StorageManager;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.content.pm.ActivityInfo;
import android.view.View;
import android.widget.Button;

import com.android.vending.expansion.zipfile.ZipResourceFile;
import com.android.vending.expansion.zipfile.ZipResourceFile.ZipEntryRO;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;

public class MainActivity extends AppCompatActivity
{
    public void LogError(String category, String Message)
    {
        if(Looper.myLooper() == Looper.getMainLooper())
        {
            Log.d(category, Message);
            AlertDialog.Builder dialog = new AlertDialog.Builder(this);
            dialog.setTitle("Error");
            dialog.setMessage(Message);

            dialog.setPositiveButton("OK", new DialogInterface.OnClickListener()
            {
                public void onClick(DialogInterface dialog, int which)
                {
                    dialog.dismiss();
                    finish();
                }
            });

            dialog.show();
        }
        else {
            CopyOBBError = Message;
            throw new RuntimeException(Message);
        }
    }

    public String CopyOBBError = "";
    private class CopyOBBTask extends AsyncTask<Integer, Integer, Long>
    {
        protected Long doInBackground(Integer... urls)
        {
            if(copyOrDownloadExpansionFile())
                return (long)1;

            return (long)0;
        }

        protected void onProgressUpdate(Integer... progress)
        {

        }

        protected void onPostExecute(Long result)
        {
            if(result == 0)
                LogError("OBB_COPY", CopyOBBError);
        }
    }
    private static final int VERSION = BuildConfig.VERSION_CODE;
    static final int OBB_FILE_VERSION = 17; // default to app version, may change as needed
    static final long OBB_FILE_SIZE = 527195423L; // file size in bytes of expansion file
    private static final String OBB_KEY = null; // key used when generating expansion file
    private static final String PACKAGE_NAME = App.getContext().getPackageName();
    private static final String GAME_FILE_NAME = App.getContext().getResources()
            .getString(R.string.game_file_name);
    private static final String OBB_FILE_NAME = "main." + OBB_FILE_VERSION + "." + PACKAGE_NAME +
            ".obb";
    private boolean OBB_EMBEDDED = getObbEmbedded();
    // App.getContext().getExternalFilesDir(null)
    // ändra tillbaks till Environment.getExternalStorageDirectory()
    private static final String OBB_FOLDER = Environment.getExternalStorageDirectory() + "/Android/obb/" + PACKAGE_NAME + "/";
    private static final String OBB_FILE_EXTERNAL_PATH = OBB_FOLDER + OBB_FILE_NAME;

    private static final String EXTRACTED_GAME_PATH = App.getContext().getExternalFilesDir(null) + "/Game/";
    private static final int DOWNLOAD_REQUEST = 1;

    private static final int PERMISSION_CHECK_EXTERNAL_STORAGE = 1;

    boolean bDeniedPermission = false;

    private boolean getObbEmbedded() {
        try {
            return Arrays.asList(App.getContext().getResources().getAssets().list(""))
                    .contains(OBB_FILE_NAME);
        }
        catch (IOException e) {
            LogError("INIT", "IOException occurred during initialization: " + e.getMessage());
            return false;
        }
    }

    // once the expansion file is mounted, this starts the activity that launches the game
    private void startGame(String fileName) {
        Intent intent = new Intent(this, AgsEngine.class);
        Bundle b = new Bundle();
        b.putString("filename", fileName); // full path to game data
        b.putString("directory", getApplicationInfo().dataDir); // writable location (saves, etc.)
        b.putBoolean("loadLastSave", false); // TODO: auto-load last save?
        intent.putExtras(b);
        startActivity(intent);
        finish(); // TODO: do something other than just exit the app when the game exits?
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    }

    // checks if the expansion file exists (must have matching size in bytes)
    // TODO: add a CRC check on expansion file to check for corruption
    private boolean obbFileExists() {
        File obbFile = new File(OBB_FILE_EXTERNAL_PATH);
        return ((obbFile.isFile()) /*&& (obbFile.length() == OBB_FILE_SIZE)*/);
    }

    // ensures that the folders where the expansion file is copied/downloaded exist
    // also creates an empty file for the expansion file to be copied over to help
    // ensure it is created without exceptions being thrown
    private void ensureObbExternalPathExists() {
        File obbFile = new File(OBB_FILE_EXTERNAL_PATH);

        try {
            obbFile.getParentFile().mkdirs();
        }
        catch (Exception e)
        {
            LogError("OBB_COPY", "Could not create folder on external storage: " + e.getMessage());
            finish();
        }

        try {
            obbFile.createNewFile();
        }
        catch (IOException e)
        {
            LogError("OBB_COPY", "Could not create .obb on external storage: " + e.getMessage());
            finish();
        }
    }

    // copies the embedded expansion file to external storage
    // while not ideal, this is necessary for the expansion file to be mounted
    // you most likely want to upload the expansion file separately and use
    // the downloader interface instead
    private void copyEmbeddedObbToExternalStorage() {
        if ((!OBB_EMBEDDED) || (obbFileExists())) {
            return;
        }
        final int BUFFER_SIZE = 102400; // update as needed, your mileage may vary
        InputStream is = null;
        OutputStream os = null;
        try {
            is = getResources().getAssets().open(OBB_FILE_NAME);
            ensureObbExternalPathExists();
            os = new FileOutputStream(OBB_FILE_EXTERNAL_PATH);
            byte[] buffer = new byte[BUFFER_SIZE];
            for (int len; (len = is.read(buffer)) != -1; ) {
                os.write(buffer, 0, len);
            }
        }
        catch (FileNotFoundException e) {
            LogError("OBB_COPY", "File not found exception occurred copying expansion file: " + e.getMessage());
            finish();
        }
        catch (IOException e) {
            LogError("OBB_COPY", "IOException occurred copying expansion file: " + e.getMessage());
            finish();
        }
        finally {
            try {
                if (is != null) {
                    is.close(); // FFS, Java, I'm CLOSING a stream... why can this throw an exception???
                }
            }
            catch (IOException e) {
                LogError("OBB_COPY", "Exception occurred closing input file: " + e.getMessage());
                finish();
            }
            try {
                if (os != null) {
                    os.close();
                }
            }
            catch (IOException e) {
                LogError("OBB_COPY", "Exception occurred closing output file: " + e.getMessage());
                finish();
            }
        }
    }

    // Requests to download the expansion file from Google Play
    private void downloadExpansionFile() {
        if (OBB_EMBEDDED) {
            return;
        }
        if (obbFileExists()) {
            mountExpansionAndStartGame(OBB_KEY);
            return;
        }
        Intent intent = new Intent(this, ExpansionDownloaderActivity.class);
        startActivityForResult(intent, DOWNLOAD_REQUEST);
    }

    // helper to copy or download the expansion file
    // the expansion file will be mounted when finished, and the game will start
    private boolean copyOrDownloadExpansionFile() {
        try {
            if (OBB_EMBEDDED) {
                copyEmbeddedObbToExternalStorage();
                mountExpansionAndStartGame(OBB_KEY);
            } else {
                downloadExpansionFile();
                // expansion mounted when download finishes (async)
            }
        }
        catch(Exception e)
        {
            return false;
        }

        return true;
    }

    // mounts the expansion file and starts the game (async)
    private void mountExpansionAndStartGame(String key) {
       // final StorageManager storageManager =
           //     (StorageManager)getApplicationContext().getSystemService(Context.STORAGE_SERVICE);
       // String filePath = OBB_FILE_EXTERNAL_PATH;
       // final File mainFile = new File(filePath);

        try {
            // We use an extract.x file to log successful extractions.
            File f = new File(EXTRACTED_GAME_PATH + "extract." + OBB_FILE_VERSION);
            if (!f.exists())
            {
                Log.d("ZIP", "No log file found, extracting...");
                final int BUFFER_SIZE = 102400; // update as needed, your mileage may vary
                InputStream is = null;
                OutputStream os = null;

                ZipResourceFile OBBFile = new ZipResourceFile(OBB_FILE_EXTERNAL_PATH);
                ZipEntryRO FileList[] = OBBFile.getAllEntries();

                f.mkdirs();
                for (int i = 0; i < FileList.length; i++) {
                    is = OBBFile.getInputStream(FileList[i].mFileName);
                    os = new FileOutputStream(EXTRACTED_GAME_PATH + FileList[i].mFileName, false);
                    byte[] buffer = new byte[BUFFER_SIZE];
                    for (int len; (len = is.read(buffer)) != -1; ) {
                        os.write(buffer, 0, len);
                    }
                }

                // create a log file to indicate a successful extraction
                f.createNewFile();
                Log.d("ZIP", "Extraction successful! Creating log...");

                // then clean up any previous expansion log files
                for(int i = OBB_FILE_VERSION - 1; i >= 1; i--)
                {
                    f = new File(EXTRACTED_GAME_PATH + "extract." + i);
                    if(!f.exists())
                        break;
                    else
                        f.delete();
                }
            }
            else
                Log.d("ZIP", "Log file found, skipping extraction...");

            startGame(EXTRACTED_GAME_PATH + GAME_FILE_NAME);
        }
        catch (IOException e) {
            LogError("OBB_ZIP", "Exception occurred when trying to extract game data: " + e.getMessage());
        }

        /*
        if (!storageManager.isObbMounted(mainFile.getAbsolutePath())) {
            if (mainFile.exists()) {
                if (storageManager.mountObb(mainFile.getAbsolutePath(), key, expansionListener)) {
                    Log.d("STORAGE_MNT", "SUCCESSFULLY QUEUED");
                } else {
                    Log.d("STORAGE_MNT", "FAILED");
                }
            } else {
                Log.d("STORAGE", "Expansion file " + filePath + " not found!");
            }
        }*/
    }

    public void SetupUI()
    {
        setContentView(R.layout.loading);

        if(Build.VERSION.SDK_INT >= 14) {
            View view = findViewById(R.id.the_loading_screen);
            int flags = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
            if(Build.VERSION.SDK_INT >= 16)
            {
                flags |= (View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        | View.SYSTEM_UI_FLAG_FULLSCREEN);
            }
            if(Build.VERSION.SDK_INT >= 19)
            {
                flags |= View.SYSTEM_UI_FLAG_IMMERSIVE | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY;
            }
            if(view != null) {
                view.setVisibility(View.VISIBLE);
                view.setSystemUiVisibility(flags);
            }

            view = getWindow().getDecorView();
            if(view != null)
                view.setSystemUiVisibility(flags);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        SetupUI();

       /* expansionListener = new OnObbStateChangeListener() {
            @Override
            public void onObbStateChange(String path, int state) {
                super.onObbStateChange(path, state);
                final StorageManager storageManager =
                        (StorageManager) getApplicationContext()
                                .getSystemService(Context.STORAGE_SERVICE);
                Log.d("PATH = ", path);
                Log.d("STATE = ", state + "");
                if (state == OnObbStateChangeListener.MOUNTED) {
                    String mountedPath = storageManager.getMountedObbPath(path);
                    Log.d("STORAGE", "-->MOUNTED");
                    startGame(new File(mountedPath, GAME_FILE_NAME).getPath());
                } else {
                    Log.d("##", "Path: " + path + "; state: " + state);
                }
            }
        };*/
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        if(Build.VERSION.SDK_INT >= 23) {
            if (!bDeniedPermission) {
                bDeniedPermission = true;
                int permissionCheck = ContextCompat.checkSelfPermission(this, "android.permission.READ_EXTERNAL_STORAGE");
                if (permissionCheck != PackageManager.PERMISSION_GRANTED) {
                    AlertDialog.Builder dialog = new AlertDialog.Builder(this);
                    dialog.setTitle("Info");
                    dialog.setMessage("Kathy Rain requires explicit permission to access your external file storage. This is necessary to read all game files.");

                    final Activity activity = this;

                    dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            ActivityCompat.requestPermissions(activity,
                                    new String[]{"android.permission.READ_EXTERNAL_STORAGE"},
                                    PERMISSION_CHECK_EXTERNAL_STORAGE);
                        }
                    });

                    dialog.show();
                } else
                    new CopyOBBTask().execute(0, 0, 0);
            }
        }
        else
            new CopyOBBTask().execute(0, 0, 0);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // Check which request we're responding to
        if (requestCode == DOWNLOAD_REQUEST) {
            if (resultCode == RESULT_OK) {
                SetupUI();
                // Download was successful
                mountExpansionAndStartGame(OBB_KEY);
            }
            else {
                // The download failed! Try again... we can't do anything it!
                downloadExpansionFile();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case PERMISSION_CHECK_EXTERNAL_STORAGE: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {

                    new CopyOBBTask().execute(0, 0, 0);
                }
                else
                {
                    if(Build.VERSION.SDK_INT >= 23)
                    {
                        if(!shouldShowRequestPermissionRationale("android.permission.READ_EXTERNAL_STORAGE")) {
                            AlertDialog.Builder dialog = new AlertDialog.Builder(this);
                            dialog.setTitle("Error");
                            dialog.setMessage("You have denied the necessary app permissions for Kathy Rain. If you wish to change this, you can do so in the app settings screen.");

                            dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                    finish();
                                }
                            });

                            dialog.show();
                        }
                        else
                        {
                            bDeniedPermission = false;
                        }
                    }
                }
            }
        }
    }
}
