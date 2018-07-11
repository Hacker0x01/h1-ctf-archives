package com.hackerone.mobile.challenge4;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Typeface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Button;
import android.widget.TextView;

public class MenuActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu);

        Button start = (Button) findViewById(R.id.StartGame);
        start.setOnClickListener(this);
        TextView name = (TextView) findViewById(R.id.Name);
        Button info = (Button) findViewById(R.id.info);
        info.setOnClickListener(this);

        Animation scale = AnimationUtils.loadAnimation(this, R.anim.myscale);
        scale.setFillAfter(true);
        scale.setStartOffset(0);
        name.setAnimation(scale);

        registerReceiver(new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                if (intent.hasExtra("start_game")) {
                    Intent startGameIntent = new Intent(context, MainActivity.class);
                    context.startActivity(startGameIntent);
                }
            }
        }, new IntentFilter("com.hackerone.mobile.challenge4.menu"));
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.StartGame:
                Intent intent = new Intent(this, MainActivity.class);
                startActivity(intent);
                break;
            case R.id.info:
                Intent intent1 = new Intent(this, InfoActivity.class);
                startActivity(intent1);
                break;
        }
    }
}
