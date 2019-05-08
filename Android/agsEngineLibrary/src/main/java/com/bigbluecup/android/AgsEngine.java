package com.bigbluecup.android;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.media.AudioManager;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.PowerManager;
import android.support.v7.app.AppCompatActivity;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnLongClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Toast;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;

import java.lang.reflect.Method;

public class AgsEngine extends AppCompatActivity
		implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener
{
	public static final String TAG = "AGS Engine";

	private static final IAgsApp defaultApp = new DefaultApp();

	private static IAgsApp app = defaultApp;

	public boolean isInGame = false;

	private Toast toast = null;
	private EngineGlue glue;
	private PowerManager.WakeLock wakeLock;
	private AudioManager audio;
	public CustomGlSurfaceView surfaceView;
	public MessageHandler handler;

	private CustomLayout viewGroup;

	boolean ignoreNextPointerUp = false;
	boolean ignoreMovement = false;
	boolean initialized = false;
	boolean stopLongclick = false;
	boolean enableLongclick = false;
	private float lastX = 0.0f;
	private float lastY = 0.0f;
	private float downX = 0.0f;
	private float downY = 0.0f;

	private boolean ignoreNextActionUp_Back = false;
	private boolean ignoreNextActionUp_Menu = false;

	private boolean draggingMouse = false;

	// Client used to interact with Google APIs.
	public static GoogleApiClient mGoogleApiClient;

	private static final int GOOGLE_PLAY_RESOLVE = 1;
	private static final int REQUEST_ACHIEVEMENTS = 2;

	private boolean bForcedConnect = false;

	public void UnlockAchievement(String achName)
	{
		if(mGoogleApiClient != null && mGoogleApiClient.isConnected())
		{
			// Look up achievement string in a predefined list
			String Achievements[] = getResources().getStringArray(R.array.AchievementList);
			for (int i = 0; i < Achievements.length; i += 2) {
				if (Achievements[i].equals(achName) && (i + 1) < Achievements.length) {
					Log.d("Achievement", "Achievement unlocked: " + achName);
					Games.Achievements.unlockImmediate(mGoogleApiClient, Achievements[i + 1]);
					return;
				}
			}

			Log.d("Achievement", "Failed to look up achievement: " + achName);
		}
		else
		{
			Log.d("Achievement", "Failed to unlock achievement, not connected.");
		}
	}

	public void ShowAchievements()
	{
		Log.d("Achievement", "Showing achievements...");

		if(mGoogleApiClient != null)
		{
			if(mGoogleApiClient.isConnected()) {
				this.startActivityForResult(Games.Achievements.getAchievementsIntent(mGoogleApiClient), REQUEST_ACHIEVEMENTS);
			}
			else
			{
				mGoogleApiClient.connect();
				bForcedConnect = true;
				Log.d("Achievement", "Failed to display achievements, not connected to Google Play Services!");
			}
		}
	}

	public static class DefaultApp implements IAgsApp
	{
		@Override
		public void onMenuKeyPressed(AgsEngine engine, boolean longPress)
		{
			//if (longPress) engine.toggleKeyboard();
			//else engine.showInGameMenu();
			engine.keyboardEvent(KeyCode.KeyF2);
		}

		@Override
		public void onBackKeyPressed(AgsEngine engine, boolean longPress)
		{
			//if (longPress) engine.showExitConfirmation();
			//else engine.keyboardEvent(KeyCode.KeyBackHardKey);
			engine.keyboardEvent(KeyCode.KeyEscape);
		}

		public boolean onInGameMenuItemSelected(AgsEngine engine, MenuItem item)
		{
			int id = item.getItemId();

			// This must be if-else instead of switch-case because it is in a library
			if (id == R.id.key_f1)
				engine.keyboardEvent(KeyCode.KeyF1);
			else if (id == R.id.key_f2)
				engine.keyboardEvent(KeyCode.KeyF2);
			else if (id == R.id.key_f3)
				engine.keyboardEvent(KeyCode.KeyF3);
			else if (id == R.id.key_f4)
				engine.keyboardEvent(KeyCode.KeyF4);
			else if (id == R.id.key_f5)
				engine.keyboardEvent(KeyCode.KeyF5);
			else if (id == R.id.key_f6)
				engine.keyboardEvent(KeyCode.KeyF6);
			else if (id == R.id.key_f7)
				engine.keyboardEvent(KeyCode.KeyF7);
			else if (id == R.id.key_f8)
				engine.keyboardEvent(KeyCode.KeyF8);
			else if (id == R.id.key_f9)
				engine.keyboardEvent(KeyCode.KeyF9);
			else if (id == R.id.key_f10)
				engine.keyboardEvent(KeyCode.KeyF10);
			else if (id == R.id.key_f11)
				engine.keyboardEvent(KeyCode.KeyF11);
			else if (id == R.id.key_f12)
				engine.keyboardEvent(KeyCode.KeyF12);
			else if (id == R.id.exitgame)
				engine.showExitConfirmation();
			else if (id == R.id.toggle_keyboard)
				engine.toggleKeyboard();
			else
				return false;

			return true;
		}

		@Override
		public void onKeyboardEvent(AgsEngine engine, KeyCode key)
		{
			engine.keyboardEvent(key);
		}

		public int getInGameMenuID(AgsEngine engine)
		{
			return R.menu.default_ingame;
		}

		public static boolean DefaultInGameMenuItemSelection(AgsEngine engine, MenuItem item)
		{
			return defaultApp.onInGameMenuItemSelected(engine, item);
		}

		public static void DefaultKeyboardEvent(AgsEngine engine, KeyCode key)
		{
			defaultApp.onKeyboardEvent(engine, key);
		}

		public static void DefaultMenuKeyPress(AgsEngine engine, boolean longPress)
		{
			defaultApp.onMenuKeyPressed(engine, longPress);
		}

		public static void DefaultBackKeyPress(AgsEngine engine, boolean longPress)
		{
			defaultApp.onBackKeyPressed(engine, longPress);
		}
	};

	private void setAssistBlocked(View view, boolean blocked) {
		try {
			Method setAssistBlockedMethod = View.class.getMethod("setAssistBlocked", boolean.class);
			setAssistBlockedMethod.invoke(view, blocked);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		// Switch to the loading view and start the game
		isInGame = true;

		super.onCreate(savedInstanceState);

		// Get the game filename from the launcher activity
		Bundle extras = getIntent().getExtras();
		String gameFilename = extras.getString("filename");
		String baseDirectory = extras.getString("directory");
		boolean loadLastSave = extras.getBoolean("loadLastSave");

		// Get app directory
		String appDirectory = "";
		try
		{
			appDirectory = getPackageManager().getPackageInfo(getPackageName(), 0).applicationInfo.dataDir;
		}
		catch (NameNotFoundException e) { }

		// Set windows options
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		setDefaultKeyMode(DEFAULT_KEYS_DISABLE);

		// Stop the device from saving power
		PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
		wakeLock = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK, "fullwakelock");
		wakeLock.acquire();
		// FLAG_KEEP_SCREEN_ON

		// Set message handler for thread communication
		handler = new MessageHandler();

		audio = (AudioManager)getSystemService(Context.AUDIO_SERVICE);

		glue = new EngineGlue(this, gameFilename, baseDirectory, appDirectory, loadLastSave);
		glue.start();

		View decorView = getWindow().getDecorView();
		viewGroup = new CustomLayout(this);

		if(Build.VERSION.SDK_INT >= 14) {
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
			viewGroup.setSystemUiVisibility(flags);
			decorView.setSystemUiVisibility(flags);
		}

		if(false){//viewGroup != null) {
			// Create the Google Api Client with access to Plus and Games
			mGoogleApiClient = new GoogleApiClient.Builder(this)
					.addConnectionCallbacks(this)
					.addOnConnectionFailedListener(this)
					.addApi(Games.API).addScope(Games.SCOPE_GAMES)
					.setViewForPopups(viewGroup)
					.build();
		}
		else
		{
			// Create the Google Api Client with access to Plus and Games
			mGoogleApiClient = new GoogleApiClient.Builder(this)
					.addConnectionCallbacks(this)
					.addOnConnectionFailedListener(this)
					.addApi(Games.API).addScope(Games.SCOPE_GAMES)
					.build();
		}

		mGoogleApiClient.connect();
	}

	@Override
	public void onDestroy()
	{
		Log.d("DEBUG","onDestroy()");
		glue.shutdownEngine();
		super.onDestroy();
	}

	@Override
	protected void onPause()
	{
		Log.d("DEBUG","onPause()");
		super.onPause();
		wakeLock.release();
		pauseGame();

		/*if(Build.VERSION.SDK_INT < 24)
		{
			if(viewGroup != null)
				viewGroup.setVisibility(View.GONE);
			if(surfaceView != null)
				surfaceView.setVisibility(View.GONE);
		}*/
	}

	@Override
	protected void onResume()
	{
		Log.d("DEBUG","onResume()");
		super.onResume();
		wakeLock.acquire();

		resumeGame();

		if(Build.VERSION.SDK_INT >= 14) {
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
			viewGroup.setSystemUiVisibility(flags);

			if(surfaceView != null)
				surfaceView.setSystemUiVisibility(flags);
		}

		/*if(Build.VERSION.SDK_INT < 24) {
			if (viewGroup != null) {
				viewGroup.setVisibility(View.VISIBLE);
			}

			if (surfaceView != null) {
				surfaceView.setVisibility(View.VISIBLE);
			}
		}*/
	}

	// Prevent the activity from being destroyed on a configuration change
	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		Log.d("DEBUG", "onConfigurationChanged()");
		super.onConfigurationChanged(newConfig);
	}

	// Handle messages from the engine thread
	class MessageHandler extends Handler
	{
		@Override
		public void handleMessage(Message msg)
		{
			switch (msg.what)
			{
				case EngineGlue.MSG_SWITCH_TO_INGAME:
					switchToIngame();
					break;

				case EngineGlue.MSG_SHOW_MESSAGE:
					showMessage(msg.getData().getString("message"));
					break;

				case EngineGlue.MSG_SHOW_TOAST:
					showToast(msg.getData().getString("message"));
					break;

				case EngineGlue.MSG_SET_ORIENTATION:
					//setRequestedOrientation(msg.getData().getInt("orientation"));
					break;

				case EngineGlue.MSG_ENABLE_LONGCLICK:
					enableLongclick = true;
					break;
			}
		}
	}


	@Override
	public boolean dispatchTouchEvent(MotionEvent ev)
	{
		switch (ev.getAction() & 0xFF)
		{
			case MotionEvent.ACTION_DOWN:
			{
				downX = lastX = ev.getX();
				downY = lastY = ev.getY();
				ignoreMovement = false;
				initialized = false;
				stopLongclick = false;
				glue.moveMouse(0, 0, downX, downY);
				glue.clickMouse(EngineGlue.MOUSE_HOLD_LEFT);
				break;
			}

			case MotionEvent.ACTION_MOVE:
			{
				if (!initialized)
				{
					lastX = ev.getX();
					lastY = ev.getY();
					initialized = true;
				}

				if (!ignoreMovement)
				{
					float x = ev.getX() - lastX;
					float y = ev.getY() - lastY;

					lastX = ev.getX();
					lastY = ev.getY();

					glue.moveMouse(x, y, lastX, lastY);

					try
					{
						// Delay a bit to not get flooded with events
						Thread.sleep(50, 0);
					}
					catch (InterruptedException e) {}
				}

				break;
			}

			case MotionEvent.ACTION_UP:
			{
				ignoreMovement = false;

				long down_time = ev.getEventTime() - ev.getDownTime();

				glue.clickMouse(EngineGlue.MOUSE_CLICK_LEFT);

				if (down_time < 200)
				{
					// Quick tap for clicking the left mouse button

					draggingMouse = false;
				}

				try
				{
					// Delay a bit to not get flooded with events
					Thread.sleep(50, 0);
				}
				catch (InterruptedException e) {}

				break;
			}

			// Second finger down
			case 5: //MotionEvent.ACTION_POINTER_DOWN:
			{
				stopLongclick = true;
				ignoreMovement = true;
				ignoreNextPointerUp = true;
			}

			// Second finger lifted
			case 6: //MotionEvent.ACTION_POINTER_UP:
			{
				if (!ignoreNextPointerUp)
				{
					glue.clickMouse(EngineGlue.MOUSE_CLICK_RIGHT);

					ignoreMovement = false;
					try
					{
						// Delay a bit to not get flooded with events
						Thread.sleep(50, 0);
					}
					catch (InterruptedException e) {}
				}
				ignoreNextPointerUp = false;
				break;
			}
		}

		return super.dispatchTouchEvent(ev);
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item)
	{
		if (app.onInGameMenuItemSelected(this, item)) return true;
		return super.onOptionsItemSelected(item);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(app.getInGameMenuID(this), menu);
		return true;
	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent ev)
	{

		// Very simple key processing for now, just one key event per poll
		switch (ev.getAction())
		{
			case KeyEvent.ACTION_DOWN:
			{
				int key = ev.getKeyCode();

				if ((key == KeyEvent.KEYCODE_BACK) && ((ev.getFlags() & 0x80) > 0)) // FLAG_LONG_PRESS
				{
					ignoreNextActionUp_Back = true;
					app.onBackKeyPressed(this, true);
				}

				if ((key == KeyEvent.KEYCODE_MENU) && ((ev.getFlags() & 0x80) > 0)) // FLAG_LONG_PRESS
				{
					ignoreNextActionUp_Menu = true;
					app.onMenuKeyPressed(this, true);
				}

				if (key == KeyEvent.KEYCODE_VOLUME_UP)
					audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_RAISE, AudioManager.FLAG_SHOW_UI);

				if (key == KeyEvent.KEYCODE_VOLUME_DOWN)
					audio.adjustStreamVolume(AudioManager.STREAM_MUSIC, AudioManager.ADJUST_LOWER, AudioManager.FLAG_SHOW_UI);

				break;
			}

			case KeyEvent.ACTION_UP:
			{
				int key = ev.getKeyCode();

				if (key == KeyEvent.KEYCODE_MENU)
				{
					if (!ignoreNextActionUp_Menu) app.onMenuKeyPressed(this, false);
					ignoreNextActionUp_Menu = false;
					return isInGame;
				}
				else if (key == KeyEvent.KEYCODE_BACK)
				{
					if (!ignoreNextActionUp_Back)
					{
						app.onBackKeyPressed(this, false);
					}
					ignoreNextActionUp_Back = false;
					return isInGame;
				}

				if (   (key == KeyEvent.KEYCODE_MENU)
						|| (key == KeyEvent.KEYCODE_VOLUME_UP)
						|| (key == KeyEvent.KEYCODE_VOLUME_DOWN)
						|| (key == 164) // KEYCODE_VOLUME_MUTE
						|| (key == KeyEvent.KEYCODE_SEARCH)
						|| (key == KeyEvent.KEYCODE_ALT_LEFT)
						|| (key == KeyEvent.KEYCODE_ALT_RIGHT)
						|| (key == KeyEvent.KEYCODE_SHIFT_LEFT)
						|| (key == KeyEvent.KEYCODE_SHIFT_RIGHT))
				{
					return isInGame;
				}
				// key wasn't a special key, allow the app to intercept
				app.onKeyboardEvent(this, KeyCode.findByAndroidKeyCode(key, ev.isShiftPressed()));
				break;
			}
		}

		return isInGame;
	}


	public void toggleKeyboard()
	{
		InputMethodManager manager = (InputMethodManager)getSystemService(Context.INPUT_METHOD_SERVICE);
		manager.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
	}


	// Exit confirmation dialog displayed when hitting the "back" button
	public void showExitConfirmation()
	{
		onPause();

		AlertDialog.Builder ad = new AlertDialog.Builder(this);
		ad.setMessage("Are you sure you want to quit?");

		ad.setOnCancelListener(new OnCancelListener()
		{
			public void onCancel(DialogInterface dialog)
			{
				onResume();
			}
		});

		ad.setPositiveButton("Yes", new OnClickListener()
		{
			public void onClick(DialogInterface dialog, int which)
			{
				onResume();
				onDestroy();
			}
		});

		ad.setNegativeButton("No", new OnClickListener()
		{
			public void onClick(DialogInterface dialog, int which)
			{
				onResume();
			}
		});

		ad.show();
	}


	// Display a game message
	public void showMessage(String message)
	{
		onPause();

		AlertDialog.Builder dialog = new AlertDialog.Builder(this);
		dialog.setTitle("Error");
		dialog.setMessage(message);

		dialog.setPositiveButton("OK", new OnClickListener()
		{
			public void onClick(DialogInterface dialog, int which)
			{
				onResume();
			}
		});

		dialog.show();
	}

	public void showToast(String message)
	{
		if (toast == null)
			toast = Toast.makeText(this, message, Toast.LENGTH_LONG);
		else
			toast.setText(message);

		toast.show();
	}

	public void showInGameMenu()
	{
		openOptionsMenu();
	}

	// Switch to the game view after loading is done
	public void switchToIngame()
	{
		surfaceView = new CustomGlSurfaceView(this);

		if(Build.VERSION.SDK_INT >= 14) {
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
			viewGroup.setSystemUiVisibility(flags);
			surfaceView.setSystemUiVisibility(flags);
		}

		surfaceView.setOnLongClickListener(new OnLongClickListener()
		{
			public boolean onLongClick(View v)
			{
				if (!draggingMouse && !stopLongclick && (Math.abs(downX - lastX) < 4.0f) && (Math.abs(downY - lastY) < 4.0f))
				{
					draggingMouse = true;
					glue.clickMouse(EngineGlue.MOUSE_HOLD_LEFT);
					return true; // Produces haptic feedback (vibration)
				}
				return false;
			}
		});

		surfaceView.setLongClickable(enableLongclick);

		viewGroup.removeAllViews();
		viewGroup.addView(surfaceView, ViewGroup.LayoutParams.FILL_PARENT);

		Display display = this.getWindowManager().getDefaultDisplay();
		int realWidth;
		int realHeight;

		if (Build.VERSION.SDK_INT >= 17){
			//new pleasant way to get real metrics
			DisplayMetrics realMetrics = new DisplayMetrics();
			display.getRealMetrics(realMetrics);
			realWidth = realMetrics.widthPixels;
			realHeight = realMetrics.heightPixels;

		} else if (Build.VERSION.SDK_INT >= 14) {
			//reflection for this weird in-between time
			try {
				Method mGetRawH = Display.class.getMethod("getRawHeight");
				Method mGetRawW = Display.class.getMethod("getRawWidth");
				realWidth = (Integer) mGetRawW.invoke(display);
				realHeight = (Integer) mGetRawH.invoke(display);
			} catch (Exception e) {
				//this may not be 100% accurate, but it's all we've got
				realWidth = display.getWidth();
				realHeight = display.getHeight();
				Log.e("Display Info", "Couldn't use reflection to get the real display metrics.");
			}

		} else {
			//This should be close, as lower API devices should not have window navigation bars
			realWidth = display.getWidth();
			realHeight = display.getHeight();
		}
		float fheight = (float)realHeight;
		float fwidth = (float)realWidth;
		float aspect = fwidth / fheight;

		Log.d("ASPECT", "height: " + fheight + " width: " + fwidth + " aspect: " + aspect);
		if(aspect < 1.32 || aspect > 1.34) // 4:3 = 1.333333
		{
			LayoutInflater inflater = (LayoutInflater)this.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			LinearLayout layout = (LinearLayout) inflater.inflate(R.layout.credits, null, false);
			viewGroup.addView(layout, ViewGroup.LayoutParams.MATCH_PARENT);
		}
		setContentView(viewGroup);

		isInGame = true;
	}

	public void keyboardEvent(KeyCode key)
	{
		glue.keyboardEvent(key.getAndroidKeyCode(), key.getUnicodeChar(), key.hasShiftKey());
	}

	public void pauseGame()
	{
		if (isInGame) glue.pauseGame();
	}

	public void resumeGame()
	{
		if (isInGame) glue.resumeGame();
	}

	public static void setApp(IAgsApp app)
	{
		if (app == null) AgsEngine.app = defaultApp;
		else AgsEngine.app = app;
	}

	public static IAgsApp getApp()
	{
		return app;
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// Check which request we're responding to
		if (requestCode == GOOGLE_PLAY_RESOLVE)
		{
			// Alright, we successfully resolved the problem. Try connecting again.
			if(resultCode == RESULT_OK)
			{
				Log.d(TAG, "Successfully resolved! Reconnecting...");
				mGoogleApiClient.connect();
			}
			else
			{
				Log.d(TAG, "Unable to resolve, result code: " + resultCode + " Bypassing Google Play Services...");
			}
		}
		else if(requestCode == REQUEST_ACHIEVEMENTS) {
			if (resultCode != RESULT_OK) {
				Log.d(TAG, "Failed to display achievements, result code: + " + resultCode);
			}
			else
			{

			}
		}
	}

	@Override
	public void onConnected(Bundle bundle) {
		Log.d(TAG, "onConnected() called. Sign in successful!");

		if(bForcedConnect) {
			this.startActivityForResult(Games.Achievements.getAchievementsIntent(mGoogleApiClient), REQUEST_ACHIEVEMENTS);
		}
		else
		{

		}
	}

	@Override
	public void onConnectionSuspended(int i) {
		Log.d(TAG, "onConnectionSuspended() called. Ignoring.");
		// mGoogleApiClient.connect();
	}

	@Override
	public void onConnectionFailed(ConnectionResult connectionResult)
	{
		Log.d(TAG, "onConnectionFailed() called, result: " + connectionResult);

		if(connectionResult.hasResolution())
		{
			try
			{
				connectionResult.startResolutionForResult(this, GOOGLE_PLAY_RESOLVE);
			}
			catch (Exception e)
			{
				Log.d(TAG, "Resolving connection error, exception thrown!" + e.getMessage());
				Log.d(TAG, "Unable to recover, bypassing Google Play Services.");

				if(bForcedConnect)
					this.showMessage("Failed to connect to Google Play Services!");

				switchToIngame();
			}
		}
		else
		{
			GoogleApiAvailability GAA = GoogleApiAvailability.getInstance();
			boolean bFailed = true;
			if(GAA != null) {
				if (GAA.isUserResolvableError(connectionResult.getErrorCode())) {
					Dialog d = GAA.getErrorDialog(this, connectionResult.getErrorCode(), 0);
					if(d != null) {
						d.show();
						bFailed = false;
					}
				}
			}

			if(bFailed)
				this.showMessage("Failed to connect to Google Play Services! Error : " + connectionResult);
		}
	}
}
