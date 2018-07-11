package com.hackerone.mobile.challenge4;

import android.content.Context;

import java.io.Serializable;

public abstract class StateController {

    private String location;

    public StateController() {}

    public StateController(String location) {
        this.location = location;
    }

    void save(Context context, Object obj) {}

    Object load(Context context) {
        return null;
    }

    String getLocation() {
        return this.location;
    }
}
