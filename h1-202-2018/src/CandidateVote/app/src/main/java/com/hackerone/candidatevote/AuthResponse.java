package com.hackerone.candidatevote;

import com.google.gson.annotations.SerializedName;

/**
 * Created by breadchris on 2/13/18.
 */

public class AuthResponse {
    @SerializedName("token")
    private String token;

    @SerializedName("error")
    private String error = null;

    @SerializedName("admin")
    private boolean admin = false;

    public AuthResponse(String token, String error, boolean admin) {
        this.token = token;
        this.error = error;
        this.admin = admin;
    }

    public String getToken() { return token; }

    public String getError() {
        return error;
    }

    public boolean isAdmin() {
        return admin;
    }
}
