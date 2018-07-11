package com.hackerone.mobile.challenge4;

import android.content.Context;
import android.graphics.Point;
import android.util.Log;

import java.io.Serializable;

public class GameState implements Serializable {
    public int playerX;
    public int playerY;
    public long seed;
    public int levelsCompleted;
    public String cleanupTag;
    public StateController stateController;

    private Context context;

    private static final long serialVersionUID = 1L;

    public GameState(int playerX, int playerY, long seed, int levelsCompleted) {
        this.playerX = playerX;
        this.playerY = playerY;
        this.seed = seed;
        this.levelsCompleted = levelsCompleted;
    }

    public GameState(String cleanupTag, StateController stateController) {
        this.cleanupTag = cleanupTag;
        this.stateController = stateController;
    }

    public void initialize(Context context) {
        this.context = context;

        GameState gs = (GameState)this.stateController.load(context);
        if (gs == null) {
            return;
        }

        this.playerX = gs.playerX;
        this.playerY = gs.playerY;
        this.seed = gs.seed;
        this.levelsCompleted = gs.levelsCompleted;
    }

    public void finalize() {
        Log.d("GameState", "Called finalize on GameState: " + GameManager.levelsCompleted);

        if (GameManager.levelsCompleted > 2) {
            if (context != null) {
                this.stateController.save(context, this);
            }
        }
    }
}
