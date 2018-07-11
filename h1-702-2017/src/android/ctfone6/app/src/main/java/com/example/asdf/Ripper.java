package com.example.asdf;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Base64;

import java.security.MessageDigest;

/**
 * Created by cthompson on 7/8/17.
 */

public class Ripper {
    private static final int VALID = 0;
    private static final int INVALID = 1;
    private static final String SIGNATURE = "4idKtlaGqS2VDhitx2h7UeeEThg=";

    public static int checkAppSignature(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager()
                    .getPackageInfo(context.getPackageName(),
                            PackageManager.GET_SIGNATURES);

            for (Signature signature : packageInfo.signatures) {
                MessageDigest md = MessageDigest.getInstance("SHA");
                md.update(signature.toByteArray());
                final String currentSignature = Base64.encodeToString(md.digest(), Base64.DEFAULT);
                if (SIGNATURE.trim().equals(currentSignature.trim())){
                    return VALID;
                }
            }
        } catch (Exception e) {
            System.exit(0);
        }

        return INVALID;

    }
}
