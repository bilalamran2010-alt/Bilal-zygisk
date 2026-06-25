package com.LOL.project

import android.app.Service
import android.content.Intent
import android.graphics.PixelFormat
import android.os.IBinder
import android.view.*
import android.widget.*

class MenuService : Service() {

    private lateinit var windowManager: WindowManager
    private lateinit var menuView: View
    private lateinit var params: WindowManager.LayoutParams

    // Declare native methods
    external fun setAimbot(type: Int, enabled: Boolean)
    external fun setSilentAim(enabled: Boolean)
    external fun setAutoFire(type: Int, enabled: Boolean)
    external fun setESP(type: Int, enabled: Boolean)

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()
        try { System.loadLibrary("LOL") } catch (e: Exception) { e.printStackTrace() }

        windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
        menuView = LayoutInflater.from(this).inflate(R.layout.floating_menu, null)

        params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        params.gravity = Gravity.TOP or Gravity.START
        windowManager.addView(menuView, params)

        setupMenuControls()
    }

    private fun setupMenuControls() {
        // Aimbot
        menuView.findViewById<Switch>(R.id.aimbot_head).setOnCheckedChangeListener { _, isChecked -> setAimbot(1, isChecked) }
        menuView.findViewById<Switch>(R.id.aimbot_body).setOnCheckedChangeListener { _, isChecked -> setAimbot(2, isChecked) }
        
        // Silent
        menuView.findViewById<Switch>(R.id.silent_aim).setOnCheckedChangeListener { _, isChecked -> setSilentAim(isChecked) }
        
        // ESP
        menuView.findViewById<Switch>(R.id.esp_line).setOnCheckedChangeListener { _, isChecked -> setESP(1, isChecked) }
        menuView.findViewById<Switch>(R.id.esp_box).setOnCheckedChangeListener { _, isChecked -> setESP(2, isChecked) }
        menuView.findViewById<Switch>(R.id.esp_health).setOnCheckedChangeListener { _, isChecked -> setESP(3, isChecked) }

        // Close
        menuView.findViewById<Button>(R.id.btn_close).setOnClickListener { stopSelf() }

        // Draggable
        menuView.setOnTouchListener(object : View.OnTouchListener {
            private var initialX = 0
            private var initialY = 0
            private var touchX = 0f
            private var touchY = 0f
            override fun onTouch(v: View, event: MotionEvent): Boolean {
                when (event.action) {
                    MotionEvent.ACTION_DOWN -> {
                        initialX = params.x
                        initialY = params.y
                        touchX = event.rawX
                        touchY = event.rawY
                    }
                    MotionEvent.ACTION_MOVE -> {
                        params.x = initialX + (event.rawX - touchX).toInt()
                        params.y = initialY + (event.rawY - touchY).toInt()
                        windowManager.updateViewLayout(menuView, params)
                    }
                }
                return true
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
        if (::menuView.isInitialized) windowManager.removeView(menuView)
    }
}
