package com.hackerone.mobile.challenge3;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {

    private EditText editText;
    private static char[] key = {'t', 'h', 'i', 's', '_', 'i', 's', '_', 'a', '_', 'k', '3', 'y'};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final EditText editText = (EditText)findViewById(R.id.editText);
        editText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                MainActivity.checkFlag(editText.getText().toString());
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });
    }

    public static byte[] hexStringToByteArray(String hex) {
        int l = hex.length();
        byte[] data = new byte[l/2];
        for (int i = 0; i < l; i += 2) {
            data[i/2] = (byte) ((Character.digit(hex.charAt(i), 16) << 4)
                    + Character.digit(hex.charAt(i+1), 16));
        }
        return data;
    }

    public static boolean checkFlag(String input) {
        if (input.length() == 0) {
            return false;
        }

        String flagStr = "flag{";
        if (input.length() > flagStr.length() && !input.substring(0, flagStr.length()).equals(flagStr)) {
            return false;
        }

        if (!(input.charAt(input.length() - 1) == '}')) {
            return false;
        }

        StringBuilder str = new StringBuilder("kO13t41Oc1b2z4F5F1b2BO33c2d1c61OzOdOtO");
        String thing = str.reverse().toString();
        thing = thing.replace("O", "0");
        thing = thing.replace("t", "7");
        thing = thing.replace("B", "8");
        thing = thing.replace("z", "a");
        thing = thing.replace("F", "f");
        thing = thing.replace("k", "e");

        byte[] thingBytes = hexStringToByteArray(thing);

        String test = encryptDecrypt(key, thingBytes);
        if (input.length() > input.length() && !input.substring(flagStr.length(), input.length() - 1).equals(test)) {
            return false;
        }
        return true;
    }

    private static String encryptDecrypt(char[] key, byte[] input) {
        StringBuilder output = new StringBuilder();

        for(int i = 0; i < input.length; i++) {
            output.append((char) (input[i] ^ key[i % key.length]));
        }

        return output.toString();
    }
}
