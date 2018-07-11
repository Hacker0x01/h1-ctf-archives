package com.h1702ctf.ctfone;

import java.io.IOException;

import okhttp3.CertificatePinner;
import okhttp3.OkHttpClient;
import okhttp3.Request;

public class Requestor {
    // gets set in native code
    private static String sHostname = "h1702ctf.com";
    private static String sUrl = "https://h1702ctf.com/About";
    public static void request() {
        CertificatePinner certificatePinner = new CertificatePinner.Builder()
                .add(sHostname, "sha256/8yKUtMm6FtEse2v0yDMtT0hKagvpKSWHpnufb1JP5g8=")
                .add(sHostname, "sha256/YLh1dUR9y6Kja30RrAn7JKnbQG/uEtLMkBgFF2Fuihg=")
                .add(sHostname, "sha256/Vjs8r4z+80wjNcr1YKepWQboSIRi63WsWXhIMN+eWys=")
                .build();
        OkHttpClient client = new OkHttpClient.Builder()
                .certificatePinner(certificatePinner)
                .build();
        // Create request for remote resource.
        String headerName = hName();
        String headerVal = hVal();
        Request request = new Request.Builder()
                .url(sUrl)
                .addHeader(headerName, headerVal)
                .build();
        // Execute the request and retrieve the response.

        try {
            client.newCall(request).execute();
        } catch (IOException e) {
            // eat this...this makes the game easier if we don't as it prints the call stack
            // if there's no network connectivity, revealing the fact that this method gets called
            // from native code.
            //
            // DIFFICULTY NOTE: If we wanted to make level 3 easier, just rethrow or print this exception
            //android.util.Log.i("BOOYA", e.toString());
        }

        // uncomment to test
        //android.util.Log.i("NETWORK", "BOOYA" + hName() + hVal());

    }

    public static native String hName();
    public static native String hVal();
}
