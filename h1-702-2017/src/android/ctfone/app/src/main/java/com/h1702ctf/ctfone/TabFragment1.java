package com.h1702ctf.ctfone;

import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;

import java.io.IOException;
import java.io.InputStream;
import java.util.Random;

public class TabFragment1 extends Fragment {
    ImageView mImage;
    EditText mInput;
    Button mButton;

    public void loadDataFromAsset(String name) {
//        // load text
//        try {
//            // get input stream for text
//            InputStream is = getAssets().open(name);
//            // check size
//            int size = is.available();
//            // create buffer for IO
//            byte[] buffer = new byte[size];
//            // get data to buffer
//            is.read(buffer);
//            // close stream
//            is.close();
//            // set result to TextView
//            mText.setText(new String(buffer));
//        }
//        catch (IOException ex) {
//            return;
//        }

        // load image
        try {
            // get input stream
            InputStream ims = getActivity().getAssets().open(name);
            // load image as Drawable
            Drawable d = Drawable.createFromStream(ims, null);
            // set image to ImageView
            mImage.setImageDrawable(d);
        }
        catch(IOException ex) {
            return;
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.tab_fragment1, container, false);
        // need image view set
        mImage = (ImageView)v.findViewById(R.id.imageView);
        mInput = ((EditText)v.findViewById(R.id.Level1TextInput));
        mButton = (Button) v.findViewById(R.id.lvl1button);

        mButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                String input = mInput.getText().toString();
                if (input.isEmpty()) {
                    int i = (new Random().nextInt() % 10) + 1;
                    input = "asset" + i;
                }
                loadDataFromAsset(input);
            }
        });
        return v;
    }
}