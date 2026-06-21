package com.LOL.project

import android.app.Service
import android.content.Intent
import android.os.IBinder
import android.os.Handler
import android.os.Looper
import android.widget.Toast
import java.net.HttpURLConnection
import java.net.URL

class MenuService : Service() {

    private val SERVER_URL = "http://YOUR_FLASK_SERVER_URL/check_key?key="

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        val userKey = intent?.getStringExtra("user_key") ?: ""
        
        if (userKey.isNotEmpty()) {
            verifyKey(userKey)
        } else {
            showToast("Please enter a key")
            stopSelf()
        }
        return START_STICKY
    }

    private fun verifyKey(key: String) {
        Thread {
            try {
                val url = URL(SERVER_URL + key)
                val connection = url.openConnection() as HttpURLConnection
                connection.requestMethod = "GET"
                connection.connectTimeout = 5000

                val response = connection.inputStream.bufferedReader().use { it.readText() }

                if (response.trim() == "true") {
                    showToast("Key Accepted!")
                    System.loadLibrary("LOL")
                } else {
                    showToast("Invalid Key!")
                    stopSelf()
                }
            } catch (e: Exception) {
                showToast("Connection Error: " + e.message)
                stopSelf()
            }
        }.start()
    }

    private fun showToast(message: String) {
        Handler(Looper.getMainLooper()).post {
            Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
        }
    }
}
