package com.LOL.project

import android.app.Service
import android.content.Intent
import android.os.IBinder
import android.os.Handler
import android.os.Looper
import android.provider.Settings
import android.widget.Toast
import java.net.HttpURLConnection
import java.net.URL
import java.io.OutputStream

class MenuService : Service() {

    private val SERVER_URL = "https://bilal828.pythonanywhere.com/verify"

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
        val deviceId = Settings.Secure.getString(contentResolver, Settings.Secure.ANDROID_ID)
        
        Thread {
            try {
                val url = URL(SERVER_URL)
                val connection = url.openConnection() as HttpURLConnection
                connection.requestMethod = "POST"
                connection.setRequestProperty("Content-Type", "application/json; utf-8")
                connection.setRequestProperty("Accept", "application/json")
                connection.doOutput = true
                connection.connectTimeout = 10000

                val jsonInputString = "{\"key\": \"$key\", \"device_id\": \"$deviceId\"}"
                
                connection.outputStream.use { os: OutputStream ->
                    val input = jsonInputString.toByteArray(Charsets.UTF_8)
                    os.write(input, 0, input.size)
                }

                val responseCode = connection.responseCode
                val response = connection.inputStream.bufferedReader().use { it.readText() }
                
                if (responseCode == HttpURLConnection.HTTP_OK && (response.contains("\"valid\":true") || response.contains("\"valid\": true"))) {
                    showToast("Accepted")
                    System.loadLibrary("LOL")
                    // Add your code here to start the overlay or UI
                } else {
                    showToast("Rejected: $response")
                    stopSelf()
                }
            } catch (e: Exception) {
                showToast("Error: " + e.toString())
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
