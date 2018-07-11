package com.hackerone.candidatevote;

import android.app.ActivityManager;
import android.content.Context;
import android.util.Log;

import java.io.IOException;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * Created by breadchris on 2/14/18.
 */

public class AntiTamper {
    public static boolean classesTampered(Context context) {
        long dexCrc = Long.parseLong(context.getString(R.string.title_for_another_day));

        ZipFile zf = null;
        try {
            zf = new ZipFile(context.getPackageCodePath());
        } catch (IOException e) {
            e.printStackTrace();
        }
        ZipEntry ze = zf.getEntry("classes.dex");

        Log.d("TAMPER", "" + ze.getCrc());

        if ( ze.getCrc() != dexCrc ) {
            return true;
        } else {
            aaaaaaaaaaaaaa("5055DEAA9850A19FB67D4E76BC8FD825");
            return false;
        }
    }

    public static boolean checkRunningProcesses(Context context) {
        boolean returnValue = false;

        ActivityManager manager = (ActivityManager)context.getSystemService(Context.ACTIVITY_SERVICE);
        // Get currently running application processes
        List<ActivityManager.RunningServiceInfo> list = manager.getRunningServices(300);

        aaaaaaaaaaaa("44648798D358E60D7C4D29B5469CAEA8");

        if (list != null) {
            for (int i = 0; i < list.size(); ++i) {
                String tempName = list.get(i).process;

                if (tempName.contains("fridaserver")) {
                    returnValue = true;
                }
            }
        }
        return returnValue;
    }

    public static boolean checkForHooks() {
        try {
            aa("91C6DD1299FD5D1DE9C4A0C78616D244");
            throw new Exception();
        } catch (Exception e) {
            int zygoteInitCallCount = 0;
            for (StackTraceElement stackTraceElement : e.getStackTrace()) {
                if (stackTraceElement.getClassName().equals("com.android.internal.os.ZygoteInit")) {
                    zygoteInitCallCount++;
                    if(zygoteInitCallCount == 2) {
                        // Substrate is active on the device
                        return true;
                    }
                }
                if(stackTraceElement.getClassName().equals("com.saurik.substrate.MS$2") &&
                        stackTraceElement.getMethodName().equals("invoked")) {
                    return true;
                }
                if(stackTraceElement.getClassName().equals("de.robv.android.xposed.XposedBridge") &&
                        stackTraceElement.getMethodName().equals("main")) {
                    // Xposed is active on the device
                    return true;
                }
                if(stackTraceElement.getClassName().equals("de.robv.android.xposed.XposedBridge") &&
                        stackTraceElement.getMethodName().equals("handleHookedMethod")) {
                    //A method on the stack trace has been hooked using Xposed
                    return true;
                }
            }
        }
        return false;
    }

    static native void aaaaaaaaaaaaaa(String str);
    static native void aa(String str);
    static native void aaaaaaaaaaaa(String str);
}
