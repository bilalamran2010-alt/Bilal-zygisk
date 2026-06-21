package com.LOL.project

import android.app.Service
import android.content.Intent
import android.os.IBinder
import android.widget.Toast
import android.os.Handler
import android.os.Looper

class MenuService : Service() {

    external fun setAuthorized(status: Boolean)

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        // Change to true to trigger the hooks in Main.cpp
        setAuthorized(true) 
        
        showToast("Accepted")
        return START_STICKY
    }

    private fun showToast(message: String) {
        Handler(Looper.getMainLooper()).post {
            Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
        }
    }

    companion object {
        init {
            System.loadLibrary("LOL")
        }
    }
}
