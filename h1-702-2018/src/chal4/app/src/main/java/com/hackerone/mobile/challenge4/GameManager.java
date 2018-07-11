package com.hackerone.mobile.challenge4;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Point;
import android.graphics.Rect;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;

import org.json.JSONObject;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class GameManager extends GestureDetector.SimpleOnGestureListener {
    private List<Drawable> drawables = new ArrayList<>();
    private Exit exit;
    private Player player;
    private Maze maze;
    private Rect rect = new Rect();
    private int screenSize;
    private BroadcastAnnouncer broadcastAnnouncer;
    private StateLoader loader;
    private long seed;

    public static int levelsCompleted;
    public static View view;

    static private final int LEVEL_SIZE = 5;

    public GameManager() {
        levelsCompleted = 0;

        loader = new StateLoader("game.state");
        broadcastAnnouncer = new BroadcastAnnouncer("MazeGame", "maze_game_win", "http://localhost");
    }

    private void create(long seed) {
        int size = (levelsCompleted + 1) * LEVEL_SIZE;

        System.gc();

        drawables.clear();
        maze = new Maze(size, seed);
        drawables.add(maze);
        exit = new Exit(size, maze.getEnd());
        drawables.add(exit);
        player = new Player(maze.getStart(), size);
        drawables.add(player);
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        Point diff = new Point(
                Math.round(e2.getX() - e1.getX()),
                Math.round(e2.getY() - e1.getY()));

        if (Math.abs(diff.x) > Math.abs(diff.y)) {
            diff.x = diff.x > 0 ? 1 : -1;
            diff.y = 0;
        } else {
            diff.x = 0;
            diff.y = diff.y > 0 ? 1 : -1;
        }

        movePlayer(diff);

        return super.onFling(e1, e2, velocityX, velocityY);
    }

    public boolean movePlayer(Point diff) {
        Point currentPos = player.getPoint();
        Point newPos = getNewPosition(currentPos, diff);

        boolean playerDidMove = newPos.x != currentPos.x || newPos.y != currentPos.y;

        player.goTo(newPos);

        if (exit.getPoint().equals(player.getPoint())) {
            levelsCompleted++;
            create(seed);

            broadcastAnnouncer.save(view.getContext(), this);

            GameState gs = new GameState(player.getX(), player.getY(), seed, levelsCompleted);
            loader.save(view.getContext(), gs);
        }
        view.invalidate();
        return playerDidMove;
    }

    public Point getNewPosition(Point pos, Point diff) {
        while (maze.canPlayerGoTo(pos.x + diff.x, pos.y + diff.y)) {
            pos.x += diff.x;
            pos.y += diff.y;
            if (diff.x != 0) {
                if (maze.canPlayerGoTo(pos.x, pos.y + 1) || maze.canPlayerGoTo(pos.x, pos.y - 1)) {
                    break;
                }
            }
            if (diff.y != 0) {
                if (maze.canPlayerGoTo(pos.x + 1, pos.y) || maze.canPlayerGoTo(pos.x - 1, pos.y)) {
                    break;
                }
            }
        }
        return pos;
    }

    public void draw(Canvas canvas) {
        for (Drawable drawableItem : drawables) {
            drawableItem.draw(canvas, rect);
        }
    }

    public void setView(View view) {
        this.view = view;

        String msg = "You won a level!";
        FileOutputStream outputStream;
        try {
            outputStream = view.getContext().openFileOutput(broadcastAnnouncer.getStringRef(), Context.MODE_PRIVATE);
            outputStream.write(msg.getBytes());
            outputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        broadcastAnnouncer.setStringRef(view.getContext().getFilesDir().getAbsolutePath() + "/" + broadcastAnnouncer.getStringRef());
        broadcastAnnouncer.load(view.getContext());

        GameState gameState = (GameState)loader.load(view.getContext());

        if (gameState == null) {
            Random generator = new Random();
            create(generator.nextLong());

            GameState gs = new GameState(player.getX(), player.getY(), seed, levelsCompleted);
            loader.save(view.getContext(), gs);
            return;
        }

        levelsCompleted = gameState.levelsCompleted;
        seed = gameState.seed;
        create(seed);

        player.point = new Point(gameState.playerX, gameState.playerY);
    }

    public void setScreenSize(int width, int height) {
        screenSize = Math.min(width, height);
        rect.set(
                (width - screenSize) / 2,
                (height - screenSize) / 2,
                (width + screenSize) / 2,
                (height - screenSize) / 2
        );
    }

    public Maze getMaze() {
        return maze;
    }

    public Player getPlayer() {
        return player;
    }

    public Exit getExit() {
        return exit;
    }
}
