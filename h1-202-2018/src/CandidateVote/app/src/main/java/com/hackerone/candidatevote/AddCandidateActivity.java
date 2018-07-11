package com.hackerone.candidatevote;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.webkit.JavascriptInterface;
import android.webkit.WebView;
import android.widget.EditText;

import java.net.URLEncoder;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class AddCandidateActivity extends AppCompatActivity {

    private CandidateAPI candAPI;
    private WebView candidateWebView;
    private String apiToken;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_candidate);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        Intent intent = getIntent();
        apiToken = intent.getStringExtra("token");

        candAPI = CandidateClient.getApiService();

        final EditText candidateNameText = (EditText) findViewById(R.id.candidateNameText);
        final EditText candidateImageText = (EditText) findViewById(R.id.candidateImageText);
        candidateWebView = (WebView)  findViewById(R.id.candidateWebView);
        candidateWebView.getSettings().setJavaScriptEnabled(true);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String name = candidateNameText.getText().toString();
                String image = candidateImageText.getText().toString();
                candidateWebView.addJavascriptInterface(new WebAppInterface(AddCandidateActivity.this, name, image), "A");
                runJs();

                Call<APIResponse> call = candAPI.createCandidate(apiToken, new Candidate(0, name, 0, image));

                call.enqueue(new Callback<APIResponse>() {
                    @Override
                    public void onResponse(Call<APIResponse> call, Response<APIResponse> response) {
                        if (response.isSuccessful()) {
                            APIResponse resp = response.body();
                            if (resp.getError() != null) {
                                finish();
                            }
                        }
                    }

                    @Override
                    public void onFailure(Call<APIResponse> call, Throwable t) {
                    }
                });
            }
        });
    }

    public void runJs() {
        candidateWebView.post(new Runnable() {
            @Override
            public void run() {
                candidateWebView.loadUrl("javascript:" + URLEncoder.encode(getJs()));
            }
        });
    }

    private class WebAppInterface {
        Context mContext;
        String mName;
        String mUrl;

        WebAppInterface(Context c, String name, String url) {
            mContext = c;
            mName = name;
            mUrl = url;
        }

        @JavascriptInterface
        public String getName() {
            return mName;
        }

        @JavascriptInterface
        public String getUrl() {
            return mUrl;
        }
    }

    private native String getJs();
}
