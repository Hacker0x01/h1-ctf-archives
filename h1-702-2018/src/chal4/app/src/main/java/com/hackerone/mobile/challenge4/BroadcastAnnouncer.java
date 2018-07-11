package com.hackerone.mobile.challenge4;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.InputStream;
import java.io.Serializable;
import java.lang.reflect.Field;
import java.net.HttpURLConnection;
import java.net.URL;

public class BroadcastAnnouncer extends StateController implements Serializable {
    private String stringVal;
    private String stringRef;
    private String destUrl;

    private static final long serialVersionUID = 1L;

    public BroadcastAnnouncer() {
        super();
    }

    public BroadcastAnnouncer(String tag, String stringRef, String destUrl) {
        super(tag);
        this.stringRef = stringRef;
        this.destUrl = destUrl;
    }

    public void save(Context context, Object obj) {
        (new Thread() {
            public void run() {
                try {
                    URL url = new URL(destUrl + "/announce?val=" + stringVal);
                    HttpURLConnection urlConnection = (HttpURLConnection) url.openConnection();
                    try {
                        InputStream in = new BufferedInputStream(urlConnection.getInputStream());
                        in.read();
                    } finally {
                        urlConnection.disconnect();
                    }
                } catch (java.net.MalformedURLException e) {
                    e.printStackTrace();
                } catch (java.io.IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    public Object load(Context context) {
        this.stringVal = "";
        File file = new File(this.stringRef);

        try {
            BufferedReader br = new BufferedReader(new FileReader(file));

            String st;
            while ((st = br.readLine()) != null) {
                this.stringVal += st;
            }
        } catch (java.io.FileNotFoundException e) {
            e.printStackTrace();
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public void setStringRef(String stringRef) {
        this.stringRef = stringRef;
    }

    public String getStringRef() {
        return stringRef;
    }
}
