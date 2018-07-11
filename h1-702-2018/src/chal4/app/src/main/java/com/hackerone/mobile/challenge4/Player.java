package com.hackerone.mobile.challenge4;

import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.util.Log;

import java.io.Serializable;

public class Player extends Dot {
    public Player(Point start, int size) {
        super(size, start, getPaint());
    }

    private static Paint getPaint() {
        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setColor(Color.RED);
        return paint;
    }

    public void goTo(Point newPos) {
        point = newPos;
    }

    public int getX() {
        return point.x;
    }

    public int getY() {
        return point.y;
    }
}
