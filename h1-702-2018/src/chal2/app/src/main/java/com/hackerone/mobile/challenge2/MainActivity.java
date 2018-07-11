package com.hackerone.mobile.challenge2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import com.andrognito.pinlockview.PinLockListener;
import com.andrognito.pinlockview.PinLockView;
import com.andrognito.pinlockview.IndicatorDots;

import org.libsodium.jni.SodiumConstants;
import org.libsodium.jni.crypto.Random;
import org.libsodium.jni.crypto.SecretBox;
import org.libsodium.jni.encoders.Hex;
import org.libsodium.jni.keys.KeyPair;

import java.nio.charset.StandardCharsets;

import static org.libsodium.jni.encoders.Encoder.HEX;

public class MainActivity extends AppCompatActivity {

    String TAG = "PinLock";
    PinLockView mPinLockView;
    IndicatorDots mIndicatorDots;

    private byte[] cipherText;

    private PinLockListener mPinLockListener = new PinLockListener() {
        @Override
        public void onComplete(String pin) {
            Log.d(TAG, "Pin complete: " + pin);

            byte[] key = getKey(pin);
            Log.d("TEST", bytesToHex(key));

            SecretBox box = new SecretBox(key);

            byte[] nonce = "aabbccddeeffgghhaabbccdd".getBytes();

            try {
                byte[] result = box.decrypt(nonce, cipherText);

                String str = new String(result, StandardCharsets.UTF_8);

                Log.d("DECRYPTED", str);
            } catch (java.lang.RuntimeException e) {
                Log.d("PROBLEM", "Unable to decrypt text");
                e.printStackTrace();
            }
        }

        @Override
        public void onEmpty() {
            Log.d(TAG, "Pin empty");
        }

        @Override
        public void onPinChange(int pinLength, String intermediatePin) {
            Log.d(TAG, "Pin changed, new length " + pinLength + " with intermediate pin " + intermediatePin);
        }
    };

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        cipherText = new Hex().decode("9646D13EC8F8617D1CEA1CF4334940824C700ADF6A7A3236163CA2C9604B9BE4BDE770AD698C02070F571A0B612BBD3572D81F99");

        mPinLockView = (PinLockView) findViewById(R.id.pin_lock_view);
        mPinLockView.setPinLockListener(mPinLockListener);

        mIndicatorDots = (IndicatorDots) findViewById(R.id.indicator_dots);
        mPinLockView.attachIndicatorDots(mIndicatorDots);

        /*
        byte[] key = getKey("918264");

        Log.d("TEST", bytesToHex(key));

        SecretBox box = new SecretBox(key);

        byte[] nonce = "aabbccddeeffgghhaabbccdd".getBytes();
        byte[] message = "flag{wow_yall_called_a_lot_of_func$}".getBytes();

        byte[] result = box.encrypt(nonce, message);

        Log.d("ENCRYPTED", bytesToHex(result));

        box = new SecretBox(key);
        byte[] decrypted = box.decrypt(nonce, result);

        String str = new String(decrypted, StandardCharsets.UTF_8);
        Log.d("DECRYPTED", str);
        */
    }

    public native byte[] getKey(String pin);
    public native void resetCoolDown();
}
