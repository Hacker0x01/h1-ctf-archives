package com.hackerone.candidatevote;

import com.google.gson.annotations.SerializedName;

/**
 * Created by breadchris on 2/11/18.
 */

public class Candidate {
    @SerializedName("id")
    final private int id;

    @SerializedName("name")
    final private String name;

    @SerializedName("votes")
    final private int votes;

    @SerializedName("url")
    final private String url;

    public Candidate(int id, String name, int votes, String url) {
        this.id = id;
        this.name = name;
        this.votes = votes;
        this.url = url;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getUrl() { return url; }

    public int getVotes() {
                        return votes;
    }

    public String toString() {
        return name;
    }
}
