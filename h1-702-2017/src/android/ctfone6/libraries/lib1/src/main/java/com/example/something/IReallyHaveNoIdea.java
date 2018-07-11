/*
 * Copyright 2011 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.example.something;

import android.content.Context;
import android.content.BroadcastReceiver;
import android.content.IntentFilter;
import android.util.Log;

import java.io.BufferedInputStream;

public class IReallyHaveNoIdea {
    private BroadcastReceiver mReceiver;

    public void getOffMyCase(Context c, String path) {
        IntentFilter intentFilter = new IntentFilter(
                "com.example.asdf.SEND");

        BufferedInputStream bis = new BufferedInputStream(c.getResources().openRawResource(
                c.getResources().getIdentifier(path, "raw", c.getPackageName())));

        mReceiver = new Pooper(bis);
        c.registerReceiver(mReceiver, intentFilter);
    }
}
