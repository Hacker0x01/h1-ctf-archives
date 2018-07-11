package com.hackerone.mobile.challenge5;

import android.app.ActivityManager;
import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.UnsupportedEncodingException;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private WebView mWebView;

    public String generateString(char c, int len) {
        String out = "";
        for (int i = 0; i < len; i++) {
            out += c;
        }
        return out;
    }

    public String generateString(String str, int len) {
        String out = "";
        for (int i = 0; i < len; i++) {
            out += str;
        }
        return out;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        /*
        PetHandler petHandler = new PetHandler();
        HexDumpUtil hd = new HexDumpUtil(32);

        byte[] out = petHandler.censorDogs(2048, "x");
        Log.d("TAG", hd.formatHexDump(out, 0, out.length));

        byte[] cookie = Arrays.copyOfRange(out, 512 + 8, 512 + 16);
        Log.d("TAG", hd.formatHexDump(cookie, 0, cookie.length));

        byte[] stackAddr = Arrays.copyOfRange(out, 40, 48);
        Log.d("TAG", hd.formatHexDump(stackAddr, 0, stackAddr.length));

        byte[] eip = {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'};

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try {
            baos.write(generateString("cat", (512 + 0x8)/4).getBytes());
            baos.write(cookie);
            baos.write(generateString("A", 8).getBytes());
            baos.write(eip);
        } catch (java.io.IOException e) {
        }

        petHandler.censorCats(baos.toByteArray());
        */

        String url = null;
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            url = extras.getString("url");
        }

        mWebView = (WebView) findViewById(R.id.activity_main_webview);

        // Force links and redirects to open in the WebView instead of in a browser
        mWebView.setWebViewClient(new WebViewClient());
        mWebView.clearCache(true);

        // Enable Javascript
        WebSettings webSettings = mWebView.getSettings();
        webSettings.setJavaScriptEnabled(true);

        // REMOTE RESOURCE
        if (url == null) {
            mWebView.loadUrl("http://10.0.2.2:8001");
        } else {
            mWebView.loadUrl(url);
        }
        mWebView.setWebViewClient(new CoolWebViewClient());
        mWebView.addJavascriptInterface(new PetHandler(), "PetHandler");
    }

    // Prevent the back-button from closing the app
    @Override
    public void onBackPressed() {
        if(mWebView.canGoBack()) {
            mWebView.goBack();
        } else {
            super.onBackPressed();
        }
    }

    /*
    private final class HexDumpUtil {
        private int width;

        public HexDumpUtil(int width) {
            this.width = width;
        }

        public String formatHexDump(byte[] array, int offset, int length) {
            StringBuilder builder = new StringBuilder();

            for (int rowOffset = offset; rowOffset < offset + length; rowOffset += width) {
                builder.append(String.format("%06d:  ", rowOffset));

                for (int index = 0; index < width; index++) {
                    if (rowOffset + index < array.length) {
                        builder.append(String.format("%02x ", array[rowOffset + index]));
                    } else {
                        builder.append("   ");
                    }
                }

                if (rowOffset < array.length) {
                    int asciiWidth = Math.min(width, array.length - rowOffset);
                    builder.append("  |  ");
                    try {
                        builder.append(new String(array, rowOffset, asciiWidth, "UTF-8").replaceAll("\r\n", " ").replaceAll("\n", " "));
                    } catch (UnsupportedEncodingException ignored) {
                        //If UTF-8 isn't available as an encoding then what can we do?!
                    }
                }

                builder.append(String.format("%n"));
            }

            return builder.toString();
        }
    }
    */
}
