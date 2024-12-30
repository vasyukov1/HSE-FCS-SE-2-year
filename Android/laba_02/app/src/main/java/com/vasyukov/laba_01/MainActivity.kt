package com.vasyukov.laba_01

import android.animation.ObjectAnimator
import android.content.Intent
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.widget.ImageView
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import android.widget.TextView

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        listOf(
            findViewById<TextView>(R.id.fioLabel),
            findViewById<TextView>(R.id.groupLabel),
            findViewById<ImageView>(R.id.avatarImage)
        ).forEach { view ->
            ObjectAnimator.ofFloat(view, "alpha", 0f, 1f).apply {
                duration = 1000
                start()
            }
        }

        Handler(Looper.getMainLooper()).postDelayed({
            val intent = Intent(this, InputTextScreen::class.java)
            startActivity(intent)
            finish()
        }, 1500)
    }
}