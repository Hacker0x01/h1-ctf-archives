package com.hackerone.candidatevote;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Headers;
import retrofit2.http.Query;

/**
 * Created by breadchris on 2/16/18.
 */

public interface CodeAPI {
    @Headers("X-API-AGENT: ANDROID")
    @GET("/code")
    Call<ResponseBody> getCode(@Header("token") String token, @Query("app") String app);
}
