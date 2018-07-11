package com.hackerone.mobile.challenge4;

import android.content.Context;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class StateLoader extends StateController implements Serializable {

    private static final long serialVersionUID = 1L;

    public StateLoader(String filename) {
        super(filename);
    }

    public void save(Context context, Object obj) {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        try {
            ObjectOutputStream oos = new ObjectOutputStream(baos);
            oos.writeObject(obj);
            oos.close();
        } catch(java.io.IOException e) {
            e.printStackTrace();
        }

        FileOutputStream outputStream;
        try {
            outputStream = context.openFileOutput(this.getLocation(), Context.MODE_PRIVATE);
            outputStream.write(baos.toByteArray());
            outputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Object load(Context context) {
        byte[] bytes = new byte[1024];
        try {
            FileInputStream inputStream = context.openFileInput(this.getLocation());
            BufferedInputStream buf = new BufferedInputStream(inputStream);
            buf.read(bytes, 0, bytes.length);
            buf.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }

        GameState gameState = null;
        try {
            ObjectInputStream ois = new ObjectInputStream(
                    new ByteArrayInputStream(bytes));
            Object o  = ois.readObject();
            ois.close();

            gameState = (GameState) o;
        } catch(java.io.IOException e) {
            e.printStackTrace();
        } catch(java.lang.ClassNotFoundException e) {
            e.printStackTrace();
        }
        return gameState;
    }

}
