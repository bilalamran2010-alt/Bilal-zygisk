package com.LOL.project

import android.app.Service
import android.content.Intent
import android.os.IBinder

class MenuService : Service() {

    external fun verifyKeyNative(key: String)

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        // Pass the user input key here to the C++ logic
        verifyKeyNative("SECRET_KEY_123") 
        
        return START_STICKY
    }

    companion object {
        init {
            System.loadLibrary("LOL")
        }
    }
}
