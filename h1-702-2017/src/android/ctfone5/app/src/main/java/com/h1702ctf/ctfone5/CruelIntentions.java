package com.h1702ctf.ctfone5;

import android.app.IntentService;
import android.content.Intent;
import android.content.Context;
import android.util.Log;

/**
 * An {@link IntentService} subclass for handling asynchronous task requests in
 * a service on a separate handler thread.
 * <p>
 * TODO: Customize class - update intent actions, extra parameters and static
 * helper methods.
 */
public class CruelIntentions extends IntentService {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    // TODO: Rename actions, choose action names that describe tasks that this
    // IntentService can perform, e.g. ACTION_FETCH_NEW_ITEMS
    private static final String ACTION_HINT = "com.h1702ctf.ctfone5.action.HINT";

    // TODO: Rename parameters
    private static final String EXTRA_PARAM1 = "com.h1702ctf.ctfone5.extra.PARAM1";

    public CruelIntentions() {
        super("CruelIntentions");
    }

    /**
     * Starts this service to perform action Foo with the given parameters. If
     * the service is already performing a task this action will be queued.
     *
     * @see IntentService
     */
    // TODO: Customize helper method
    public static void startActionHint(Context context, String param1) {
        Intent intent = new Intent(context, CruelIntentions.class);
        intent.setAction(ACTION_HINT);
        intent.putExtra(EXTRA_PARAM1, param1);
        context.startService(intent);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        Log.i("BOOYA", "got intent");
        if (intent != null) {
            final String action = intent.getAction();
            if (ACTION_HINT.equals(action)) {
                Log.i("BOOYA", "got hint");
                final String param1 = intent.getStringExtra(EXTRA_PARAM1);
                Log.i("BOOYA", "param: " + param1);
                handleActionHint(param1);
            }
        }
    }

    private boolean rhymesWithOrange(String s) {
        return s.equalsIgnoreCase("orange");
    }
    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters.
     */
    private void handleActionHint(String param1) {
        if (rhymesWithOrange(param1)) {
            one();
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void one();
}
