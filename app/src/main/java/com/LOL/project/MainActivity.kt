package com.LOL.project

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.net.HttpURLConnection
import java.net.URL

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val editKey = findViewById<EditText>(R.id.editKey)
        val btnLogin = findViewById<Button>(R.id.OpenMenu)

        btnLogin.setOnClickListener {
            val key = editKey.text.toString()

            Thread {
                if (checkKeyOnServer(key)) {
                    runOnUiThread {
                        Toast.makeText(this, "Success", Toast.LENGTH_SHORT).show()
                        val intent = Intent(this, MenuService::class.java)
                        intent.putExtra("user_key", key)
                        startService(intent)
                    }
                } else {
                    runOnUiThread {
                        Toast.makeText(this, "Invalid Key", Toast.LENGTH_SHORT).show()
                    }
                }
            }.start()
        }
    }

    private fun checkKeyOnServer(key: String): Boolean {
    return try {
        
        val url = URL("https://bilal828.pythonanywhere.com/verify")
        val conn = url.openConnection() as HttpURLConnection
        conn.requestMethod = "POST"
        conn.setRequestProperty("Content-Type", "application/json")
        conn.setRequestProperty("Accept", "application/json")
        conn.doOutput = true
        conn.connectTimeout = 10000 
        

        val jsonInput = "{\"key\": \"$key\", \"device_id\": \"android_device_001\"}"
        conn.outputStream.use { os ->
            os.write(jsonInput.toByteArray(Charsets.UTF_8))
        }

        val responseCode = conn.responseCode
        if (responseCode == 200) {
            val response = conn.inputStream.bufferedReader().use { it.readText() }
            return response.contains("true")
        } else {
            android.util.Log.e("SERVER_ERROR", "Code: $responseCode")
            return false
        }
    } catch (e: Exception) {
        android.util.Log.e("SERVER_ERROR", e.toString())
        false
    }
}
