package com.LOL.project

import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.net.HttpURLConnection
import java.net.URL
import java.nio.charset.StandardCharsets

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val editKey = findViewById<EditText>(R.id.editKey)
        val btnLogin = findViewById<Button>(R.id.OpenMenu)

        btnLogin.setOnClickListener {
            val key = editKey.text.toString()

            // 1. Check for Overlay Permission
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(this)) {
                val intent = Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:$packageName"))
                startActivity(intent)
                Toast.makeText(this, "Please grant overlay permission", Toast.LENGTH_LONG).show()
                return@setOnClickListener
            }

            // 2. Perform Authentication
            Thread {
                if (checkKeyOnServer(key)) {
                    runOnUiThread {
                        Toast.makeText(this, "Success", Toast.LENGTH_SHORT).show()
                        val intent = Intent(this, MenuService::class.java)
                        intent.putExtra("user_key", key)
                        startService(intent)
                        finish() 
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
            conn.setRequestProperty("Content-Type", "application/json; utf-8")
            conn.doOutput = true
            conn.connectTimeout = 10000

            val jsonInput = "{\"key\": \"$key\", \"device_id\": \"android_device_001\"}"
            conn.outputStream.use { os ->
                val input = jsonInput.toByteArray(StandardCharsets.UTF_8)
                os.write(input, 0, input.size)
            }

            return conn.responseCode == 200 && 
                   conn.inputStream.bufferedReader().use { it.readText() }.contains("true")
        } catch (e: Exception) {
            e.printStackTrace()
            false
        }
    }
}
