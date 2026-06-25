package com.LOL.project

import android.app.Service
import android.content.Intent
import android.graphics.PixelFormat
import android.os.IBinder
import android.view.Gravity
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.view.WindowManager
import android.widget.Button
import android.widget.Switch
import android.widget.Toast

class MenuService : Service() {

    private lateinit var windowManager: WindowManager
    private lateinit var menuView: View
    private lateinit var params: WindowManager.LayoutParams

    override fun onBind(intent: Intent?): IBinder? = null

    override fun onCreate() {
        super.onCreate()

        // 1. Load your native library
        try {
            System.loadLibrary("LOL")
        } catch (e: UnsatisfiedLinkError) {
            e.printStackTrace()
        }

        // 2. Initialize WindowManager
        windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
        
        // 3. Inflate the UI
        val inflater = getSystemService(LAYOUT_INFLATER_SERVICE) as LayoutInflater
        menuView = inflater.inflate(R.layout.floating_menu, null)

        // 4. Setup Window Parameters
        params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
            PixelFormat.TRANSLUCENT
        )
        params.gravity = Gravity.TOP or Gravity.START
        params.x = 100
        params.y = 100

        // 5. Add view to WindowManager
        windowManager.addView(menuView, params)

        // 6. Handle UI interactions
        setupMenuControls()
    }

    private fun setupMenuControls() {
        val aimbotSwitch = menuView.findViewById<Switch>(R.id.switch_aimbot)
        val closeBtn = menuView.findViewById<Button>(R.id.btn_close)

        aimbotSwitch.setOnCheckedChangeListener { _, isChecked ->
            // Call your native function here
            // nativeToggleAimbot(isChecked) 
            Toast.makeText(this, "Aimbot: $isChecked", Toast.LENGTH_SHORT).show()
        }

        closeBtn.setOnClickListener {
            stopSelf()
        }

        // 7. Make the view draggable
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
                        return true
                    }
                    MotionEvent.ACTION_MOVE -> {
                        params.x = initialX + (event.rawX - touchX).toInt()
                        params.y = initialY + (event.rawY - touchY).toInt()
                        windowManager.updateViewLayout(menuView, params)
                        return true
                    }
                }
                return false
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
        if (::menuView.isInitialized) {
            windowManager.removeView(menuView)
        }
    }
}
