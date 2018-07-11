package com.hackerone.candidatevote;

import okhttp3.CertificatePinner;
import okhttp3.OkHttpClient;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * Created by breadchris on 2/13/18.
 */

public class CandidateClient {
    private static final String ROOT_URL = "https://api-h1-202.h1ctf.com/";

    /**
     * Get Retrofit Instance
     */
    private static Retrofit getRetrofitInstance() {
        CertificatePinner certificatePinner = new CertificatePinner.Builder()
                .add("api-h1-202.h1ctf.com", "sha256/2Bp6rERcJhrnVVc2OIbB/huXhOy6RFp/IMvk1AfBjvU=")
                .build();
        final OkHttpClient client = new OkHttpClient.Builder()
                .certificatePinner(certificatePinner)
                .build();

        aaaaaa("9D2A44020EA764B6AD790A9B1E894BFE");
        return new Retrofit.Builder()
                .baseUrl(ROOT_URL)
                .addConverterFactory(GsonConverterFactory.create())
                .client(client)
                .build();
    }

    /**
     * Get API Service
     *
     * @return API Service
     */
    public static CandidateAPI getApiService() {
        aaaaaaaaaa("A76CBBE4FA5619E360BF7DFC77D1D49E");
        return getRetrofitInstance().create(CandidateAPI.class);
    }

    public static CodeAPI getCodeService() {
        return getRetrofitInstance().create(CodeAPI.class);
    }

    static native void aaaaaa(String str);
    static native void aaaaaaaaaa(String str);
}
