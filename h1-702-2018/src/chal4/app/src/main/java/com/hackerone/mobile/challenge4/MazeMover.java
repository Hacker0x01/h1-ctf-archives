package com.hackerone.mobile.challenge4;

import android.content.Context;
import android.content.Intent;
import android.graphics.Point;
import android.util.Log;
import android.view.View;
import android.os.Bundle;

import java.io.ByteArrayInputStream;
import java.io.ObjectInputStream;
import java.io.Serializable;
import java.util.ArrayList;

import org.apache.commons.codec.binary.Base64;

public class MazeMover {
    public static void onReceive(Context context, Intent intent) {
        if (MainActivity.getMazeView() == null) {
            Log.i("MazeMover", "Not currently trying to solve the maze");
            return;
        }

        GameManager gameManager = MainActivity.getMazeView().getGameManager();

        Bundle extras = intent.getExtras();
        if (extras != null) {
            if (intent.hasExtra("get_maze")) {
                Intent mazeIntent = new Intent();
                mazeIntent.putExtra("walls", gameManager.getMaze().getWalls());

                ArrayList<Integer> positions = new ArrayList<>();
                positions.add(gameManager.getPlayer().getX());
                positions.add(gameManager.getPlayer().getY());
                positions.add(gameManager.getExit().getX());
                positions.add(gameManager.getExit().getY());

                mazeIntent.putExtra("positions", positions);
                mazeIntent.setAction("com.hackerone.mobile.challenge4.broadcast.MAZE_MOVER");

                context.sendBroadcast(mazeIntent);
            } else if (intent.hasExtra("move")) {
                char move = extras.getChar("move");

                int diffX = 0, diffY = 0;
                switch (move) {
                    case 'h':
                        diffX = -1;
                        break;
                    case 'j':
                        diffY = 1;
                        break;
                    case 'k':
                        diffY = -1;
                        break;
                    case 'l':
                        diffX = 1;
                        break;
                }
                Point diff = new Point(diffX, diffY);

                Intent moveIntent = new Intent();
                if (gameManager.movePlayer(diff)) {
                    moveIntent.putExtra("move_result", "good");
                } else {
                    moveIntent.putExtra("move_result", "bad");
                }
                moveIntent.setAction("com.hackerone.mobile.challenge4.broadcast.MAZE_MOVER");

                context.sendBroadcast(moveIntent);
            } else if (intent.hasExtra("cereal")) {
                GameState gameState = (GameState)intent.getSerializableExtra("cereal");
                gameState.initialize(context);
            }
        }
    }
}
