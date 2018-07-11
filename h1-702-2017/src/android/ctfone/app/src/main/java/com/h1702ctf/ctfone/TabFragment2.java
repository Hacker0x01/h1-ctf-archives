package com.h1702ctf.ctfone;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class TabFragment2 extends Fragment {
    TextView mHashView;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.tag_fragment2, container, false);
        mHashView = (TextView)v.findViewById(R.id.hashText);
        final Button button2 = (Button) v.findViewById(R.id.hashmebutton);
        button2.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                try {
                    String hash = InCryption.hashOfPlainText();
                    mHashView.setText(hash);
                    mHashView.setBackgroundColor(Color.WHITE);
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        });
        return v;
    }
}