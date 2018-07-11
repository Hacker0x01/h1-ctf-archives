package com.hackerone.candidatevote;

import com.google.gson.annotations.SerializedName;

/**
 * Created by breadchris on 2/13/18.
 */

public class APIResponse {
    @SerializedName("result")
    private String result;

    @SerializedName("error")
    private String error = null;

    public APIResponse() {}

    public String getResult() { return result; }

    public String getError() {
        return error;
    }
}
