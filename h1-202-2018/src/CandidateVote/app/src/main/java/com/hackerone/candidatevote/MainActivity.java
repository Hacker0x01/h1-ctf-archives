package com.hackerone.candidatevote;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import com.google.common.io.Files;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.crypto.SecretKey;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MainActivity extends AppCompatActivity {

    private static final int PERMISSIONS_REQUEST_INTERNET = 1;

    public static final int GET_USER_TOKEN_REQUEST = 0;

    private ArrayList<Candidate> candidates;
    private MyCandidateAdapter adapter;
    private ListView candidateListView;
    private CandidateAPI candAPI;
    private String userToken = null;

    static {
        System.loadLibrary("native-lib");
    }

    private final static char[] hexArray = "0123456789ABCDEF".toCharArray();
    public static String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        for ( int j = 0; j < bytes.length; j++ ) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        SecretKey secret = EncryptionHelper.generateKey(this);
        Log.d("TEST", "Helper for when I need to decrypt things: " + bytesToHex(EncryptionHelper.encryptMsg("testing encryption", secret)));

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.READ_CONTACTS)
                != PackageManager.PERMISSION_GRANTED) {

            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.INTERNET)) {
                Log.d("TEST", "Well, we tried...");
            } else {
                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.INTERNET},
                        PERMISSIONS_REQUEST_INTERNET);
            }
        }

        if (AntiTamper.classesTampered(this)) {
            finish();
        }

        candAPI = CandidateClient.getApiService();

        candidateListView = (ListView)findViewById(R.id.candidate_list);
        candidateListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(final AdapterView<?> adapterView, View view, int position, long id) {
                final Candidate c = candidates.get(position);

                Log.d("TEST", "" + c.getId());

                if (userToken == null) {
                    Snackbar.make(adapterView, "Please login first before you vote.", Snackbar.LENGTH_LONG).show();
                    return;
                }

                Call<APIResponse> call = candAPI.voteForCandidate(userToken, c.getId());
                call.enqueue(new Callback<APIResponse>() {
                    @Override
                    public void onResponse(@NonNull Call<APIResponse> call, @NonNull Response<APIResponse> response) {
                        if (response.isSuccessful()) {
                            APIResponse resp = response.body();
                            if (resp != null && resp.getError() == null) {
                                Snackbar.make(adapterView, "Voted for: " + c.getName(), Snackbar.LENGTH_LONG).show();
                            } else {
                                Snackbar.make(adapterView, "Voting failed: " + resp.getError(), Snackbar.LENGTH_LONG).show();
                            }
                        }

                        updateCandidates();
                    }

                    @Override
                    public void onFailure(Call<APIResponse> call, Throwable t) {
                        Snackbar.make(adapterView, "Unable to vote right now.", Snackbar.LENGTH_LONG).show();
                    }
                });
            }
        });
        aaaaaaaaa("DDC09B1C11F8675E0186310A6B36002D");

        getClient();

        Runnable candidatesThread = new Runnable() {
            @Override
            public void run() {
                updateCandidates();
            }
        };
        Thread thread = new Thread(candidatesThread);
        thread.start();

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // Check if logged in

                Intent intent = new Intent(MainActivity.this, LoginActivity.class);
                startActivityForResult(intent, GET_USER_TOKEN_REQUEST);
            }
        });
    }

    public void updateCandidates() {
        Log.d("TEST", "Getting candidates");
        Call<ArrayList<Candidate>> call = candAPI.getCandidates();

        aaaa("1E7746CB4B982418E917EDD07F6ACFFA");
        call.enqueue(new Callback<ArrayList<Candidate>>() {
            @Override
            public void onResponse(Call<ArrayList<Candidate>> call, Response<ArrayList<Candidate>> response) {
                if (response.isSuccessful()) {
                    if (AntiTamper.checkForHooks()) {
                        finish();
                    }
                    candidates = response.body();

                    adapter = new MyCandidateAdapter(MainActivity.this, candidates);
                    candidateListView.setAdapter(adapter);
                }
            }

            @Override
            public void onFailure(Call<ArrayList<Candidate>> call, Throwable t) {
                Log.d("TEST", t.getLocalizedMessage());

                CharSequence text = "Unable to get candidates!";
                int duration = Toast.LENGTH_SHORT;

                Toast toast = Toast.makeText(getApplicationContext(), text, duration);
                toast.show();
            }
        });
    }

    public void getClient() {
        if (userToken == null) {
            return;
        }

        CodeAPI codeAPI = CandidateClient.getCodeService();
        Call<ResponseBody> call = codeAPI.getCode(userToken, "client");

        call.enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                File file = new File("client.jar");
                try {
                    file.createNewFile();
                    Files.asByteSink(file).write(response.body().bytes());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case PERMISSIONS_REQUEST_INTERNET: {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Log.d("TEST", "We good");
                } else {
                    Log.d("TEST", "We not good");
                }
                return;
            }
        }
    }

    protected void onActivityResult(int requestCode, int resultCode,
                                    Intent data) {
        if (AntiTamper.checkRunningProcesses(this)) {
            finish();
        }
        userToken = data.getStringExtra("token");
        Log.d("TEST", "User token: " + userToken);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    native void aaaaaaaaa(String str);
    native void aaaa(String str);
    native String getCreds();
}
