package com.LOL.project

import android.app.Service
import android.content.Intent
import android.os.IBinder
import android.widget.Toast

class MenuService : Service() {
    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    override fun onCreate() {
        super.onCreate()
        try {
            System.loadLibrary("LOL")
            Toast.makeText(this, "Library Loaded Successfully", Toast.LENGTH_SHORT).show()
        } catch (e: Exception) {
            Toast.makeText(this, "Library Load Failed: " + e.message, Toast.LENGTH_LONG).show()
        }
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        return START_STICKY
    }
}
