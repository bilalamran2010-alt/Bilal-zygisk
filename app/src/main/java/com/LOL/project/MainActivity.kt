package com.LOL.project

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val btnLogin = findViewById<Button>(R.id.btnLogin)
        
        btnLogin.setOnClickListener {
            // This intent starts the service that loads the cheat
            val intent = Intent(this, MenuService::class.java)
            startService(intent)
            
            // Close the app to show the menu over the game
            finish() 
        }
    }
}
