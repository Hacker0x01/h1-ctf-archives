package com.h1702ctf.ctfone;

// From https://github.com/tanaysoni/Monte-Carlo-Pi-Value/blob/master/PiValue.java

import android.util.Log;

import java.util.ArrayList;
import java.util.concurrent.*;
public class MonteCarlo {

    private static final String TAG = MonteCarlo.class.toString();

    public static class PiValue implements Callable {

        double pi, x, y, inside = 0, total = 0;

        public Double call() {
            for (double i = 0; i < 1000000; i++) {
                x = Math.random();
                y = Math.random();
                if (x * x + y * y <= 1) {
                    inside++;
                }
                total++;
            }
            pi = inside / total * 4;
            return pi;
        }
    }

    public static void start() throws InterruptedException, ExecutionException {
        long startTime = System.currentTimeMillis();
        int NTHREADS = 2;
        ArrayList<Future<Double>> values = new ArrayList<Future<Double>>();
        ExecutorService exec = Executors.newFixedThreadPool(2);

        for (int i = 0; i < NTHREADS; i++) {
            Callable<Double> callable = new PiValue();
            Future<Double> future = exec.submit(callable);
            values.add(future);
        }

        ArraysArraysArrays.start();

        Double sum = 0.0;
        for (Future<Double> f : values) {
            sum = sum + f.get();
        }
        Log.i(TAG, "" + (sum / (double)NTHREADS));
        long stopTime = System.currentTimeMillis();
        Log.i(TAG, "" + (stopTime-startTime)/1000);

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String functionnameLeftbraceOneCommaTwoCommaThreeCommaRightbraceFour(String lvl1Flag, String lvl2Flag, String lvl3Flag);

}