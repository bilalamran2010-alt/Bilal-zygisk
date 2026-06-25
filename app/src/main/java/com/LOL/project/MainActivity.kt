package com.LOL.project

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.net.HttpURLConnection
import java.net.URL
import java.util.Scanner

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
            val url = URL("https://bilal828.pythonanywhere.com/verify?key=$key")
            val conn = url.openConnection() as HttpURLConnection
            conn.requestMethod = "GET"
            conn.connectTimeout = 5000
            
            if (conn.responseCode == 200) {
                val response = Scanner(conn.inputStream).useDelimiter("\\A").next()
                return response.contains("true")
            }
            false
        } catch (e: Exception) {
            false
        }
    }
}
