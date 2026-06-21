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

    private val SERVER_URL = "https://bilal828.pythonanywhere.com/verify?key="

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        val userKey = intent?.getStringExtra("user_key") ?: ""
        
        if (userKey.isNotEmpty()) {
            verifyKey(userKey)
        } else {
            showToast("Enter a key")
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
                connection.connectTimeout = 10000

                val responseCode = connection.responseCode
                if (responseCode == HttpURLConnection.HTTP_OK) {
                    val response = connection.inputStream.bufferedReader().use { it.readText() }.trim()
                    
                    if (response == "true") {
                        showToast("Accepted")
                        System.loadLibrary("LOL")
                    } else {
                        showToast("Invalid Key")
                        stopSelf()
                    }
                } else {
                    showToast("Server Error: $responseCode")
                    stopSelf()
                }
            } catch (e: Exception) {
                showToast("Connection Error: " + e.toString())
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
