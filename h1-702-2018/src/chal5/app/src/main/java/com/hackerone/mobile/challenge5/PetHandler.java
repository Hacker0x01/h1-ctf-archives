package com.hackerone.mobile.challenge5;

import android.util.Log;
import android.webkit.JavascriptInterface;

import org.json.JSONArray;
import org.json.JSONException;

public class PetHandler {
    @JavascriptInterface
    public String toString() { return "Pets :)"; }

    @JavascriptInterface
    public String censorMyCats(String data) {
        try {
            JSONArray streamer = new JSONArray(data);
            byte[] dataParsed = new byte[streamer.length()];
            for (int i = 0; i < streamer.length(); i++) {
                Integer n = streamer.getInt(i);
                if (n > 255) {
                    return null;
                }
                dataParsed[i] = (byte)n.intValue();
            }

            JSONArray results;
            try {
                results = new JSONArray(censorCats(dataParsed));
            } catch (org.json.JSONException e) {
                return null;
            }
            return results.toString();
        } catch (org.json.JSONException e) {
            e.printStackTrace();
            return null;
        }
    }

    @JavascriptInterface
    public String censorMyDogs(int len, String dogString) {
        byte[] censored = censorDogs(len, dogString);
        JSONArray results;
        try {
            results = new JSONArray(censored);
        } catch (org.json.JSONException e) {
            return null;
        }
        return results.toString();
    }

    @JavascriptInterface
    public String getMySomething() {
        return String.valueOf(getSomething());
    }

    public native long getSomething();

    public native byte[] censorDogs(int len, String dogString);

    public native byte[] censorCats(byte[] catString);
}
