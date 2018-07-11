package com.hackerone.mobile.challenge4;

import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;

/**
 * Created by vitaliy on 4/12/17.
 */

public class Exit extends Dot {
    public Exit(int size, Point point) {
        super(size, point, getPaint());
    }
    private static Paint getPaint(){
    Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
    paint.setColor(Color.GREEN);
        return paint;
    }

    public int getX() {
        return point.x;
    }

    public int getY() {
        return point.y;
    }
}
