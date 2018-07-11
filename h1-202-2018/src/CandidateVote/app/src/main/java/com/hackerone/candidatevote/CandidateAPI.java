package com.hackerone.candidatevote;

import java.util.ArrayList;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Headers;
import retrofit2.http.POST;
import retrofit2.http.PUT;
import retrofit2.http.Path;

/**
 * Created by breadchris on 2/11/18.
 */

public interface CandidateAPI {
    @Headers("X-API-AGENT: ANDROID")
    @GET("/candidates")
    Call<ArrayList<Candidate>> getCandidates();

    @Headers("X-API-AGENT: ANDROID")
    @GET("/candidate/{id}")
    Call<Candidate> getCandidate(@Path("id") int id);

    @Headers("X-API-AGENT: ANDROID")
    @POST("/candidates")
    Call<APIResponse> createCandidate(@Header("X-API-TOKEN") String token, @Body Candidate candidate);

    @Headers("X-API-AGENT: ANDROID")
    @PUT("/vote/{id}")
    Call<APIResponse> voteForCandidate(@Header("X-API-TOKEN") String token, @Path("id") int id);

    @Headers("X-API-AGENT: ANDROID")
    @POST("/user/login")
    Call<AuthResponse> userLogin(@Body User user);

    @Headers("X-API-AGENT: ANDROID")
    @POST("/user/register")
    Call<AuthResponse> userRegister(@Body User user);
}
