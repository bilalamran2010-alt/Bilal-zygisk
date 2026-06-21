package com.LOL.project

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val editKey = findViewById<EditText>(R.id.editKey)
        val btnLogin = findViewById<Button>(R.id.OpenMenu)
        
        btnLogin.setOnClickListener {
            val key = editKey.text.toString()
            val intent = Intent(this, MenuService::class.java)
            intent.putExtra("user_key", key)
            startService(intent)
        }
    }
}
