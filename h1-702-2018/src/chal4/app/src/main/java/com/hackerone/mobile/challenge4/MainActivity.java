package com.hackerone.mobile.challenge4;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.PersistableBundle;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;

public class MainActivity extends AppCompatActivity {
    private static MazeView view;
    private GestureDetector gestureDetector;
    public static Context context;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Log.i(MainActivity.class.getName(), "Title");
        GameManager gameManager = new GameManager();
        view = new MazeView(this, gameManager);

        gestureDetector = new GestureDetector(this, gameManager);
        setContentView(view);

        context = getApplicationContext();

        registerReceiver(new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                MazeMover.onReceive(context, intent);
            }
        }, new IntentFilter("com.hackerone.mobile.challenge4.broadcast.MAZE_MOVER"));
    }
    @Override
    public void onSaveInstanceState(Bundle outState, PersistableBundle outPresitentState){
        super.onSaveInstanceState(outState, outPresitentState);
        outState.putString("Title", "Hello");
    }
    @Override
    public boolean onTouchEvent(MotionEvent event){
        return gestureDetector.onTouchEvent(event);
    }

    public static MazeView getMazeView() {
        return view;
    }
}