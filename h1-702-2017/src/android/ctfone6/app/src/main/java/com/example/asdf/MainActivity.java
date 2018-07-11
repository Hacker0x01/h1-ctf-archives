/*
 * Copyright 2011 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.asdf;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Debug;
import android.os.Environment;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;


import android.content.Intent;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;

import dalvik.system.DexClassLoader;
import android.content.pm.ApplicationInfo;
import android.widget.TextView;

import java.lang.reflect.*;
import java.security.MessageDigest;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class MainActivity extends Activity {
    private static final String SECONDARY_DEX_NAME = "something.jar";
    // may want to tweak it based on actual size of the secondary dex file involved.
    private static final int BUF_SIZE = 8 * 1024;

    private ProgressDialog mProgressDialog = null;

    private Button thisShit;
    private TextView textFuck;

    public native void doSomethingCool(Context context);

    private BroadcastReceiver mReceiver;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        if (Debug.isDebuggerConnected()) {
            finishAffinity();
            System.exit(0);
        }

        if (Ripper.checkAppSignature(getApplicationContext()) != 0) {
            finishAffinity();
            System.exit(0);
        }

        thisShit = (Button)findViewById(R.id.button);
        textFuck = (TextView)findViewById(R.id.editText);

        thisShit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Before the secondary dex file can be processed by the DexClassLoader,
                // it has to be first copied from asset resource to a storage location.
                final File dexInternalStoragePath = new File(getDir("dex", Context.MODE_PRIVATE),
                        SECONDARY_DEX_NAME);
                // Perform the file copying in an AsyncTask.
                (new PrepareDexTask()).execute(dexInternalStoragePath);
            }
        });
    }

    public static boolean isDebuggable(Context context) {
        return ((context.getApplicationContext().getApplicationInfo().flags & ApplicationInfo.FLAG_DEBUGGABLE) != 0);
    }

    // File I/O code to copy the secondary dex file from asset resource to internal storage.
    private boolean prepareDex(File dexInternalStoragePath) {
        BufferedInputStream bis = null;
        OutputStream dexWriter = null;

        try {
            bis = new BufferedInputStream(getResources().openRawResource(getResources().getIdentifier("something", "raw", getPackageName())));
            dexWriter = new BufferedOutputStream(new FileOutputStream(dexInternalStoragePath));

            byte[] buf = new byte[bis.available()];

            bis.read(buf);

            String a = textFuck.getText().toString();
            String b = getResources().getString(R.string.booper);
            if (!a.equals(b)) {
                finishAffinity();
                System.exit(0);
            }

            byte[] dec = decrypt(getResources().getString(R.string.booper), getResources().getString(R.string.dooper), buf);
            dexWriter.write(dec);

            dexWriter.close();
            bis.close();

            try {
                System.loadLibrary("idk-really");
            } catch (UnsatisfiedLinkError e) {
                System.err.println("Native code library failed to load.\n" + e);
                return false;
            }

            this.doSomethingCool(getApplicationContext());
            return true;
        } catch (IOException e) {
            if (dexWriter != null) {
                try {
                    dexWriter.close();
                } catch (IOException ioe) {
                    ioe.printStackTrace();
                }
            }
            if (bis != null) {
                try {
                    bis.close();
                } catch (IOException ioe) {
                    ioe.printStackTrace();
                }
            }
            return false;
        }
    }

    public static byte[] decrypt(String key, String initVector, byte[] encrypted) {
        try {
            IvParameterSpec iv = new IvParameterSpec(initVector.getBytes("UTF-8"));
            SecretKeySpec skeySpec = new SecretKeySpec(key.getBytes("UTF-8"), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);

            byte[] original = cipher.doFinal(encrypted);

            return original;
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return null;
    }

    private class PrepareDexTask extends AsyncTask<File, Void, Boolean> {

        @Override
        protected void onCancelled() {
            super.onCancelled();
            if (mProgressDialog != null) mProgressDialog.cancel();
        }

        @Override
        protected void onPostExecute(Boolean result) {
            super.onPostExecute(result);
            if (mProgressDialog != null) mProgressDialog.cancel();
        }

        @Override
        protected Boolean doInBackground(File... dexInternalStoragePaths) {

            /*
            if (isDebuggable(getApplicationContext())) {
                finishAffinity();
                System.exit(0);
            }
            */

            prepareDex(dexInternalStoragePaths[0]);
            return null;
        }
    }
}
