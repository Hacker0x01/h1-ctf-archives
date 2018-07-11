package com.hackerone.candidatevote;

import com.google.gson.annotations.SerializedName;

/**
 * Created by breadchris on 2/12/18.
 */

public class User {
    @SerializedName("name")
    private String name;

    @SerializedName("password")
    private String password;

    public User(String name, String password) {
        this.name = name;
        this.password = password;
    }
}
