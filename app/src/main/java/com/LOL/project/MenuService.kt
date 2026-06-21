package com.LOL.project

import android.app.Service
import android.content.Intent
import android.os.IBinder

class MenuService : Service() {
    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    override fun onCreate() {
        super.onCreate()
        System.loadLibrary("LOL")
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        return START_STICKY
    }
}
